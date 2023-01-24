/**
 * @file xldriver.cpp
 * @author Maxime Verreault
 * @date 2023-01-12
 * @copyright COPYRIGHT(c) Maxime Verreault All rights reserved.
 * @brief TODO: add description
 * @ingroup TODO: add or remove ingroup
 * @addtogroup xldriver
 * @{  TODO: close the doxygen group at the end of the file with @}
 */



#define UNUSED_PARAM(a) { a=a; }

#define RECEIVE_EVENT_SIZE         1        // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE              4096     // internal driver queue size in CAN events
#define RX_QUEUE_SIZE_FD           16384    // driver queue size for CAN-FD Rx events
#define ENABLE_CAN_FD_MODE_NO_ISO  0        // switch to activate no iso mode on a CAN FD channel

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "vxlapi.h"
#include <cstring>
#include <windows.h>
#include <minwindef.h>
#include <winerror.h>
#include <synchapi.h>
#include <array>
#include <numeric>
#include <atomic>
#include <thread>
#include <boost/program_options.hpp>
#include <fmt/format.h>
#include <condition_variable>
#include <CanIf_Can.h>
#include "Can_XLdriver.h"


namespace po = boost::program_options;

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
struct FrameType
{
    static constexpr uint8 StandardCan = 0;
    static constexpr uint8 StandardCanFd = 1;
    static constexpr uint8 ExtendedCan = 2;
    static constexpr uint8 ExtendedCanFd = 3;
    static constexpr uint32 mask = 0xC0000000;
    const uint8 selected;

    constexpr explicit FrameType(const Can_IdType id): selected((id & mask) >> 30)
    {
    }

    bool operator ==(uint8& other) const
    {
        return selected == other;
    }

    bool operator ==(const uint8& other) const
    {
        return selected == other;
    }
};

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
std::string g_AppName = "xlCANdemo";               //!< Application name which is displayed in VHWconf
XLportHandle    g_xlPortHandle              = XL_INVALID_PORTHANDLE;      //!< Global porthandle (we use only one!)
XLdriverConfig  g_xlDrvConfig;                                            //!< Contains the actual hardware configuration
XLaccess        g_xlChannelMask             = 0;                          //!< Global channelmask (includes all founded channels)
XLaccess        g_xlPermissionMask          = 0;                          //!< Global permissionmask (includes all founded channels)
unsigned int    g_BaudRate                  = 500000;                     //!< Default baudrate
int             g_silent                    = 0;                          //!< flag to visualize the message events (on/off)
unsigned int    g_TimerRate                 = 0;                          //!< Global timerrate (to toggel)
unsigned int    g_canFdSupport              = 0;                          //!< Global CAN FD support flag
unsigned int    g_canFdModeNoIso            = ENABLE_CAN_FD_MODE_NO_ISO;  //!< Global CAN FD ISO (default) / no ISO mode flag
XLaccess      xlChanMaskTx = 0;

std::atomic<bool> consumerThreadRun{true};                                        //!< flag to start/stop the RX thread

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

template<typename T>
constexpr void initToZero(T& source)
{
    (void) source;
}

template<>
constexpr void initToZero(XLcanTxEvent& source)
{
    source.channelIndex = 0;
    source.tag = 0;
    source.transId = 0;
    for(unsigned char & i : source.reserved)
    {
        i = 0;
    }
    source.tagData.canMsg.canId = 0;
    source.tagData.canMsg.dlc = 0;
    source.tagData.canMsg.msgFlags = 0;
    for(unsigned char & i : source.tagData.canMsg.data)
    {
        i = 0;
    }
    for(unsigned char & i : source.tagData.canMsg.reserved)
    {
        i = 0;
    }
}

template<>
constexpr void initToZero(XLevent & source)
{
    source.tag = 0;
    source.chanIndex = 0;
    source.transId = 0;
    source.portHandle = 0;
    source.flags = 0;
    source.reserved = 0;
    source.timeStamp = 0;
    source.tagData.msg.dlc = 0;
    source.tagData.msg.flags = 0;
    source.tagData.msg.id = 0;
    source.tagData.msg.res1 = 0;
    source.tagData.msg.res2 = 0;
    for(unsigned char & i: source.tagData.msg.data)
    {
        i = 0;
    }
}

template<>
constexpr void initToZero(XLcanFdConf& source)
{
    source.arbitrationBitRate = 0;
    source.sjwAbr = 0;
    source.tseg1Abr = 0;
    source.tseg2Abr = 0;
    source.dataBitRate = 0;
    source.sjwDbr = 0;
    source.tseg1Dbr = 0;
    source.tseg2Dbr = 0;
    source.reserved = 0;
    source.options = 0;
    for(auto & data: source.reserved1)
    {
        data = 0;
    }
    source.reserved2 = 0;
}

struct CanData
{
    std::vector<uint8> data{};
    const uint8 dlc;
    const uint8 frameSize;

    CanData(const uint8* rawData, uint8 length): dlc(getDLC(length)), frameSize(getPayloadSize(dlc))
    {
        data.resize(frameSize);
        for(auto i = 0; i < length; ++i)
        {
            data[i] = rawData[i];
        }
        pad(0x55, length);
    }

    static constexpr uint8 getDLC(uint8 length)
    {
        if(length <= 8)         return length;
        else if(length <= 12)   return 9;
        else if(length <= 16)   return 10;
        else if(length <= 20)   return 11;
        else if(length <= 24)   return 12;
        else if(length <= 32)   return 13;
        else if(length <= 48)   return 14;
        else                    return 15;
    }

    static constexpr uint8 getPayloadSize(uint8 dlc)
    {
        if(dlc <= 8)            return dlc;
        else if(dlc == 9)       return 12;
        else if(dlc == 10)      return 16;
        else if(dlc == 11)      return 20;
        else if(dlc == 12)      return 24;
        else if(dlc == 13)      return 32;
        else if(dlc == 14)      return 48;
        else if(dlc == 15)      return 64;
        else                    return 0;
    }
private:
    void pad(uint8 value, uint8 length)
    {
        for(auto i = length; i < frameSize; ++i)
        {
            data[i] = value;
        }
    }
};

std::condition_variable chipStateCV;
s_xl_chip_state g_ChipState{1, 0, 0};
XL_CAN_EV_CHIP_STATE g_CanFdChipState{1, 0, 0, 0, 0};

[[noreturn]] void eventConsumer()
{
    unsigned int rcvSize = 1;
    XLevent xlEvent;
    while(true)
    {
        rcvSize = 1;
        auto xlStatus = xlReceive(g_xlPortHandle, &rcvSize, &xlEvent);
        if (xlStatus != XL_ERR_QUEUE_IS_EMPTY && rcvSize > 0)
        {
            if (!g_silent)
            {
                fmt::print("{}\n", xlGetEventString(&xlEvent));
            }
            switch (xlEvent.tag)
            {
                case XL_RECEIVE_MSG:
                    if (xlEvent.tagData.msg.flags == XL_CAN_MSG_FLAG_TX_COMPLETED)
                    {
                        CanIf_TxConfirmation(xlEvent.tagData.msg.id);
                    }
                    break;
                case XL_CHIP_STATE:
                    g_ChipState = xlEvent.tagData.chipState;
                    chipStateCV.notify_all();
                    break;
                default:
                    fmt::print("{} event parsing currently unsupported", xlGetEventString(&xlEvent));
                    break;
            }
        }
    }
}

[[noreturn]] void eventCanFdConsumer()
{
    XLcanRxEvent xlEvent;
    while(true)
    {
        auto xlStatus = xlCanReceive(g_xlPortHandle, &xlEvent);
        if (xlStatus != XL_ERR_QUEUE_IS_EMPTY)
        {
            if (!g_silent)
            {
                fmt::print("{}\n", xlCanGetEventString(&xlEvent));
            }
            switch (xlEvent.tag)
            {
                case XL_CAN_EV_TAG_RX_OK:
                    if (xlEvent.tagData.canRxOkMsg.msgFlags == 0)
                    {
                        Can_HwType mailbox{
                                xlEvent.tagData.canRxOkMsg.canId,
                                0U, /* TODO: Set real HRH based on config and CanId */
                                static_cast<uint8>(xlEvent.channelIndex)
                        };
                        PduInfoType pduInfo{
                            xlEvent.tagData.canRxOkMsg.data,
                            nullptr,
                            CanData::getPayloadSize(xlEvent.tagData.canRxOkMsg.dlc)
                        };
                        CanIf_RxIndication(&mailbox, &pduInfo);
                    }
                    break;
                case XL_CAN_EV_TAG_TX_OK:
                    if (xlEvent.tagData.canRxOkMsg.msgFlags == 0)
                    {
                        CanIf_TxConfirmation(xlEvent.tagData.canRxOkMsg.canId);
                    }
                    break;
                case XL_CAN_EV_TAG_CHIP_STATE:
                    g_CanFdChipState = xlEvent.tagData.canChipState;
                    chipStateCV.notify_all();
                    break;
                default:
                    fmt::print("{} event parsing currently unsupported", xlCanGetEventString(&xlEvent));
                    break;
            }
        }
    }
}

void demoPrintConfig() {

    fmt::print("{0:─^58}\n", ""); /* have 58 minus character centered */
    fmt::print("- {}channels       Hardware Configuration               -\n", g_xlDrvConfig.channelCount);
    fmt::print("{0:─^58}\n", ""); /* have 58 minus character centered */

    for (unsigned int i = 0; i < g_xlDrvConfig.channelCount; i++) {

        fmt::print("- Ch:{}, CM:{:#X},", static_cast<unsigned>(g_xlDrvConfig.channel[i].channelIndex), g_xlDrvConfig.channel[i].channelMask);
        fmt::print(" {},", std::string(g_xlDrvConfig.channel[i].name));

        if (g_xlDrvConfig.channel[i].transceiverType != XL_TRANSCEIVER_TYPE_NONE) {
            fmt::print("{} -\n", std::string(g_xlDrvConfig.channel[i].transceiverName));
        }
        else fmt::print("{0: ^14}\n", "no Cab!");


    }
    fmt::print("{0:─^58}\n\n", ""); /* have 58 minus character centered */
}


XLstatus demoInitDriver(XLaccess &pxlChannelMaskTx, unsigned int &pxlChannelIndex) {

    XLstatus xlStatus;
    XLaccess xlChannelMaskFd = 0;
    XLaccess xlChannelMaskFdNoIso = 0;

    // ------------------------------------
    // open the driver
    // ------------------------------------
    xlStatus = xlOpenDriver ();

    // ------------------------------------
    // get/print the hardware configuration
    // ------------------------------------
    if(XL_SUCCESS == xlStatus) {
        xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
    }

    if(XL_SUCCESS == xlStatus) {
        demoPrintConfig();

        // ------------------------------------
        // select the wanted channels
        // ------------------------------------
        g_xlChannelMask = 0;
        for (unsigned int i = 0; i < g_xlDrvConfig.channelCount; i++) {

            // we take all hardware we found and supports CAN
            if (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) {

                if (pxlChannelMaskTx == 0) {
                    pxlChannelMaskTx = g_xlDrvConfig.channel[i].channelMask;
                    pxlChannelIndex  = g_xlDrvConfig.channel[i].channelIndex;
                }

                // check if we can use CAN FD - the virtual CAN driver supports CAN-FD, but we don't use it
                if (g_xlDrvConfig.channel[i].channelCapabilities & XL_CHANNEL_FLAG_CANFD_ISO_SUPPORT) {
                    xlChannelMaskFd |= g_xlDrvConfig.channel[i].channelMask;

                    // check CAN FD NO ISO support
                    if (g_xlDrvConfig.channel[i].channelCapabilities & XL_CHANNEL_FLAG_CANFD_BOSCH_SUPPORT) {
                        xlChannelMaskFdNoIso |= g_xlDrvConfig.channel[i].channelMask;
                    }
                }
                else {
                    g_xlChannelMask |= g_xlDrvConfig.channel[i].channelMask;
                }

            }
        }

        // if we found a CAN FD supported channel - we use it.
        if (xlChannelMaskFd && !g_canFdModeNoIso) {
            g_xlChannelMask = xlChannelMaskFd;
            fmt::print("- Use CAN-FD for   : CM={:#X}\n", g_xlChannelMask);
            g_canFdSupport = 1;
        }

        if (xlChannelMaskFdNoIso && g_canFdModeNoIso) {
            g_xlChannelMask = xlChannelMaskFdNoIso;
            fmt::print("- Use CAN-FD NO ISO for   : CM={:#X}\n", g_xlChannelMask);
            g_canFdSupport = 1;
        }

        if (!g_xlChannelMask) {
            fmt::print("ERROR: no available channels found! (e.g. no CANcabs...)\n\n");
            xlStatus = XL_ERROR;
        }
    }

    g_xlPermissionMask = g_xlChannelMask;

    // ------------------------------------
    // open ONE port including all channels
    // ------------------------------------
    if(XL_SUCCESS == xlStatus) {

        // check if we can use CAN FD
        if (g_canFdSupport) {
            xlStatus = xlOpenPort(&g_xlPortHandle, g_AppName.data(), g_xlChannelMask, &g_xlPermissionMask, RX_QUEUE_SIZE_FD, XL_INTERFACE_VERSION_V4, XL_BUS_TYPE_CAN);
        }
            // if not, we make 'normal' CAN
        else {
            xlStatus = xlOpenPort(&g_xlPortHandle, g_AppName.data(), g_xlChannelMask, &g_xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);

        }
        fmt::print("- OpenPort         : CM={:#X}, PH={:#X}, PM={:#X}, {}\n", g_xlChannelMask, g_xlPortHandle, g_xlPermissionMask, xlGetErrorString(xlStatus));
    }

    if ( (XL_SUCCESS == xlStatus) && (XL_INVALID_PORTHANDLE != g_xlPortHandle) ) {

        // ------------------------------------
        // if we have permission we set the
        // bus parameters (baudrate)
        // ------------------------------------
        if (g_xlChannelMask == g_xlPermissionMask) {

            if(g_canFdSupport) {
                XLcanFdConf fdParams;
                initToZero(fdParams);

                // arbitration bitrate
                fdParams.arbitrationBitRate = 1000000;
                fdParams.tseg1Abr           = 6;
                fdParams.tseg2Abr           = 3;
                fdParams.sjwAbr             = 2;

                // data bitrate
                fdParams.dataBitRate = fdParams.arbitrationBitRate*2;
                fdParams.tseg1Dbr    = 6;
                fdParams.tseg2Dbr    = 3;
                fdParams.sjwDbr      = 2;

                if (g_canFdModeNoIso) {
                    fdParams.options = CANFD_CONFOPT_NO_ISO;
                }

                xlStatus = xlCanFdSetConfiguration(g_xlPortHandle, g_xlChannelMask, &fdParams);
                fmt::print("- SetFdConfig.     : ABaudr.={}, DBaudr.={}, {}\n", fdParams.arbitrationBitRate, fdParams.dataBitRate, xlGetErrorString(xlStatus));
            }
            else {
                xlStatus = xlCanSetChannelBitrate(g_xlPortHandle, g_xlChannelMask, g_BaudRate);
                fmt::print("- SetChannelBitrate: baudr.={}, {}\n", g_BaudRate, xlGetErrorString(xlStatus));
            }
        }
        else {
            fmt::print("-                  : we have NO init access!\n");
        }
    }
    else {

        xlClosePort(g_xlPortHandle);
        g_xlPortHandle = XL_INVALID_PORTHANDLE;
        xlStatus = XL_ERROR;
    }

    return xlStatus;
}

XLstatus demoCreateRxThread() {
    XLstatus      xlStatus = XL_ERROR;

    if (g_xlPortHandle != XL_INVALID_PORTHANDLE)
    {
        if(g_canFdSupport)
        {
            std::thread consumerThread(eventCanFdConsumer);
            consumerThread.detach();
            xlStatus = XL_SUCCESS;
        }
        else
        {
            std::thread consumerThread(eventConsumer);
            consumerThread.detach();
            xlStatus = XL_SUCCESS;
        }
    }
    return xlStatus;
}


XLstatus demoTransmit(unsigned int txID)
{
    XLstatus xlStatus;
    unsigned int messageCount = 1;
    static int cnt = 0;

    if(g_canFdSupport) {
        constexpr std::array<unsigned int, 3>  fl = {{
                0 , // CAN (no FD)
                XL_CAN_TXMSG_FLAG_EDL,
                XL_CAN_TXMSG_FLAG_EDL | XL_CAN_TXMSG_FLAG_BRS,
        }};

        XLcanTxEvent canTxEvt;
        unsigned int cntSent;

        initToZero(canTxEvt);
        canTxEvt.tag = XL_CAN_EV_TAG_TX_MSG;

        canTxEvt.tagData.canMsg.canId     = txID;
        canTxEvt.tagData.canMsg.msgFlags  = fl[cnt % fl.size()];
        canTxEvt.tagData.canMsg.dlc       = 8;

        // if EDL is set, demonstrate transmit with DLC=15 (64 bytes)
        if (canTxEvt.tagData.canMsg.msgFlags & XL_CAN_TXMSG_FLAG_EDL) {
            canTxEvt.tagData.canMsg.dlc = 15;
        }

        ++cnt;
        std::iota(std::begin(canTxEvt.tagData.canMsg.data), std::end(canTxEvt.tagData.canMsg.data), 1);
        xlStatus = xlCanTransmitEx(g_xlPortHandle, xlChanMaskTx, messageCount, &cntSent, &canTxEvt);
    }
    else {
        static XLevent       xlEvent;

        initToZero(xlEvent);

        xlEvent.tag                 = XL_TRANSMIT_MSG;
        xlEvent.tagData.msg.id      = txID;
        xlEvent.tagData.msg.dlc     = 8;
        xlEvent.tagData.msg.flags   = 0;
        std::iota(std::begin(xlEvent.tagData.msg.data), std::end(xlEvent.tagData.msg.data), 1);

        xlStatus = xlCanTransmit(g_xlPortHandle, xlChanMaskTx, &messageCount, &xlEvent);
    }

    fmt::print("- Transmit         : CM({:#X}), {}\n", xlChanMaskTx, xlGetErrorString(xlStatus));

    return xlStatus;
}

extern "C" Std_ReturnType Can_XLdriver_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr)
{
    std::mutex mtx;
    std::unique_lock<std::mutex> lk(mtx);
    xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId);
    chipStateCV.wait(lk);
    switch(g_ChipState.busStatus)
    {
        case XL_CHIPSTAT_BUSOFF:
            *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF;
            break;
        case XL_CHIPSTAT_ERROR_PASSIVE:
            *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE;
            break;
        case XL_CHIPSTAT_ERROR_WARNING:
        case XL_CHIPSTAT_ERROR_ACTIVE:
            *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE;
            break;
        default:
            break;
    }
    return 0;
}

extern "C" Std_ReturnType Can_XLdriver_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr)
{
    std::mutex mtx;
    std::unique_lock<std::mutex> lk(mtx);
    xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId);
    chipStateCV.wait(lk);
    *RxErrorCounterPtr = g_ChipState.rxErrorCounter;
    return 0;
}

extern "C" Std_ReturnType Can_XLdriver_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr)
{
    std::mutex mtx;
    std::unique_lock<std::mutex> lk(mtx);
    xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId);
    chipStateCV.wait(lk);
    *TxErrorCounterPtr = g_ChipState.txErrorCounter;
    return 0;
}

extern "C" Std_ReturnType Can_XLdriver_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
{
    (void) Controller;
    (void) Transition;
    return 0;
}

extern "C" Std_ReturnType Can_XLdriver_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
    (void) Hth;
    const auto frametype = FrameType(PduInfo->id);
    const auto id = PduInfo->id & 0x3FFFFFFF;
    XLstatus xlStatus = XL_ERROR;

    if(frametype == FrameType::StandardCan || frametype == FrameType::ExtendedCan)
    {
        static XLevent       xlEvent;
        unsigned int messageCount = 1;

        initToZero(xlEvent);
        xlEvent.tag                 = XL_TRANSMIT_MSG;
        xlEvent.tagData.msg.id      = id;
        auto canFrame = CanData(PduInfo->sdu, PduInfo->length);
        xlEvent.tagData.msg.dlc     = canFrame.dlc;
        xlEvent.tagData.msg.flags   = 0;
        std::ranges::copy_n(canFrame.data.begin(), canFrame.frameSize, std::begin(xlEvent.tagData.msg.data));

        xlStatus = xlCanTransmit(g_xlPortHandle, xlChanMaskTx, &messageCount, &xlEvent);
    }
    else if(frametype == FrameType::StandardCanFd || frametype == FrameType::ExtendedCanFd)
    {
        XLcanTxEvent canTxEvt;
        unsigned int cntSent;

        initToZero(canTxEvt);
        canTxEvt.tag = XL_CAN_EV_TAG_TX_MSG;

        canTxEvt.tagData.canMsg.canId     = id;
        canTxEvt.tagData.canMsg.msgFlags  = XL_CAN_TXMSG_FLAG_EDL | XL_CAN_TXMSG_FLAG_BRS;
        auto canFrame = CanData(PduInfo->sdu, PduInfo->length);
        canTxEvt.tagData.canMsg.dlc = canFrame.dlc;
        std::ranges::copy_n(canFrame.data.begin(), canFrame.frameSize, std::begin(canTxEvt.tagData.canMsg.data));

        xlStatus = xlCanTransmitEx(g_xlPortHandle, xlChanMaskTx, 1, &cntSent, &canTxEvt);
    }
    return xlStatus;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(int argc, char *argv[])
{
    XLstatus      xlStatus;


    unsigned int  xlChanIndex = 0;
    unsigned int  txID = 0x01;

    fmt::print(
            "┌{0:─^{3}}┐\n"
            "│{1: ^{3}}│\n"
            "│{2: ^{3}}│\n"
#ifdef WIN64
            "│{4: ^{3}}│\n"
#endif
            "└{0:─^{3}}┘\n", "", "xlCANdemo - Test Application for XL Family Driver API", fmt::format("Vector Informatik GmbH, {}", __DATE__), 58, "- 64bit Version -");

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("baudrate", po::value<unsigned int>(), "set baudrate (kbps)")
            ("appname", po::value<std::string>(), "Name of the application to be read (e.g. \"xlCANcontrol\").\nApplication names are listed in the Vector Hardware Configuration tool.")
            ("txid", po::value<unsigned int>(), "set ID for sending data")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::stringstream str{};
        str << desc;
        fmt::print("{}\n", str.str());
        return 1;
    }

    if (vm.count("baudrate")) {
        g_BaudRate = vm["baudrate"].as<unsigned int>() * 1000;
        fmt::print("Baudrate = {}kbps\n", vm["baudrate"].as<unsigned int>());
    } else {
        fmt::print("Baudrate was not set. Default Baudrate selected ({}kbps)\n", g_BaudRate/1000);
    }

    if (vm.count("appname")) {
        g_AppName = vm["appname"].as<std::string>();
        fmt::print("AppName = {}\n", vm["appname"].as<std::string>());
    } else {
        fmt::print("AppName was not set. Default AppName selected (\"{}\")\n", g_AppName);
    }

    if (vm.count("txid")) {
        txID = vm["txid"].as<unsigned int>();
        fmt::print("TX ID = {:#X}\n", vm["txid"].as<unsigned int>());
    } else {
        fmt::print("TX ID was not set. Default TX ID selected ({})\n", txID);
    }

    xlStatus = demoInitDriver(xlChanMaskTx, xlChanIndex);
    fmt::print("- Init             : {}\n",  xlGetErrorString(xlStatus));

    if(XL_SUCCESS == xlStatus) {
        xlStatus = demoCreateRxThread();
        fmt::print("- Create RX thread : {}\n",  xlGetErrorString(xlStatus));
    }

    if(XL_SUCCESS == xlStatus) {
        xlStatus = xlActivateChannel(g_xlPortHandle, g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
        fmt::print("- ActivateChannel  : CM={:#X}, {}\n", g_xlChannelMask, xlGetErrorString(xlStatus));
    }
    Can_ErrorStateType res;
    Can_XLdriver_GetControllerErrorState(0, &res);
    std::array<uint8, 8> data{69, 21, 87, 34, 0, 1, 2, 4};
    const Can_PduType pduinfo{
            0x69 | 0x40000000, 0, data.size(), data.data()
    };
    Can_XLdriver_Write(0, &pduinfo);
    while(true);
    xlStatus = demoTransmit(txID);
    return xlStatus;
}



/**@} */ // END OF addtogroup <>
