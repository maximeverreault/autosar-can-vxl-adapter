/**
 * @file Can_XLdriver.cpp
 * @author Maxime Verreault
 * @date 2023-01-12
 * @copyright COPYRIGHT(c) Maxime Verreault All rights reserved.
 * @brief Can driver which communicates with the Vector XL API on Windows in order to test on CANoe without a MCU
 * @ingroup Can_XLdriver
 * @addtogroup Can_XLdriver
 * @{
 */


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "vxlapi.h"
#include <windows.h>
#include <array>
#include <numeric>
#include <atomic>
#include <thread>
#include <fmt/format.h>
#include <condition_variable>

#include <CanIf_Can.h>

#include "Can_XLdriver.h"
#include "Can_XLdriver_Internal.hpp"


using namespace std::chrono_literals;

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


void eventConsumer(const std::stop_token& stoken)
{
    unsigned int rcvSize = 1;
    XLevent xlEvent;
    while(!stoken.stop_requested())
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
                    if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_TX_COMPLETED)
                    {
                        CanIf_TxConfirmation(static_cast<PduIdType>(xlEvent.tagData.msg.id));
                    }
                    else if(xlEvent.tagData.msg.flags == 0)
                    {
                        Can_HwHandleType hwHandle;
                        if(getHwHandle(Id2HwHandleMap, xlEvent.tagData.msg.id, hwHandle) == E_NOT_OK)
                        {
                            Can_XLdriver_DetReportError(CAN_XLDRIVER_MAIN_FUNCTION_READ_SID, CAN_E_PARAM_HANDLE);
                        }
                        else
                        {
                            Can_HwType mailbox{
                                    xlEvent.tagData.msg.id,
                                    hwHandle,
                                    xlEvent.chanIndex
                            };
                            PduInfoType pduInfo{
                                    xlEvent.tagData.msg.data,
                                    nullptr,
                                    CanData::getPayloadSize(static_cast<uint8>(xlEvent.tagData.msg.dlc))
                            };
                            CanIf_RxIndication(&mailbox, &pduInfo);
                        }
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

void eventCanFdConsumer(const std::stop_token& stoken)
{
    XLcanRxEvent xlEvent;
    while(!stoken.stop_requested())
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
                        Can_HwHandleType hwHandle;
                        if(getHwHandle(Id2HwHandleMap, xlEvent.tagData.canRxOkMsg.canId, hwHandle) == E_NOT_OK)
                        {
                            Can_XLdriver_DetReportError(CAN_XLDRIVER_MAIN_FUNCTION_READ_SID, CAN_E_PARAM_HANDLE);
                        }
                        else
                        {
                            Can_HwType mailbox{
                                    xlEvent.tagData.canRxOkMsg.canId,
                                    hwHandle,
                                    static_cast<uint8>(xlEvent.channelIndex)
                            };
                            PduInfoType pduInfo{
                                    xlEvent.tagData.canRxOkMsg.data,
                                    nullptr,
                                    CanData::getPayloadSize(xlEvent.tagData.canRxOkMsg.dlc)
                            };
                            CanIf_RxIndication(&mailbox, &pduInfo);
                        }
                    }
                    break;
                case XL_CAN_EV_TAG_TX_OK:
                    if (xlEvent.tagData.canRxOkMsg.msgFlags == 0)
                    {
                        CanIf_TxConfirmation(static_cast<PduIdType>(xlEvent.tagData.canRxOkMsg.canId));
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

void demoPrintConfig(const s_xl_driver_config& xlDrvConfig) {

    fmt::print("{0:─^58}\n", ""); /* have 58 minus character centered */
    fmt::print("- {}channels       Hardware Configuration               -\n", xlDrvConfig.channelCount);
    fmt::print("{0:─^58}\n", ""); /* have 58 minus character centered */

    for (unsigned int i = 0; i < xlDrvConfig.channelCount; i++)
    {
        fmt::print("- Ch:{}, CM:{:#X},", static_cast<unsigned>(xlDrvConfig.channel[i].channelIndex), xlDrvConfig.channel[i].channelMask);
        fmt::print(" {},", std::string(xlDrvConfig.channel[i].name));

        if (xlDrvConfig.channel[i].transceiverType != XL_TRANSCEIVER_TYPE_NONE)
        {
            fmt::print("{} -\n", std::string(xlDrvConfig.channel[i].transceiverName));
        }
        else
        {
            fmt::print("{0: ^14}\n", "no Cab!");
        }
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
        demoPrintConfig(g_xlDrvConfig);

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

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

extern "C" Std_ReturnType Can_XLdriver_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;
    std::mutex mtx;
    std::unique_lock lk(mtx);
    if(auto xLstatus = xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId); (xLstatus == XL_SUCCESS) && (chipStateCV.wait_for(lk, 500ms) == std::cv_status::no_timeout))
    {
        switch (g_canFdSupport ? g_CanFdChipState.busStatus : g_ChipState.busStatus)
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
        ret = E_OK;
    }
    else
    {
        Can_XLdriver_DetReportError(CAN_XLDRIVER_GET_CONTROLLER_ERROR_STATE_SID, CAN_E_PARAM_CONTROLLER);
    }
    return ret;
}

extern "C" Std_ReturnType Can_XLdriver_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    std::mutex mtx;
    std::unique_lock lk(mtx);
    if(auto xlStatus = xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId); (xlStatus == XL_SUCCESS) && (chipStateCV.wait_for(lk, 500ms) == std::cv_status::no_timeout))
    {
        *RxErrorCounterPtr = g_canFdSupport ? g_CanFdChipState.rxErrorCounter : g_ChipState.rxErrorCounter;
        ret = E_OK;
    }
    else
    {
        Can_XLdriver_DetReportError(CAN_XLDRIVER_GET_CONTROLLER_RX_ERROR_COUNTER_SID, CAN_E_PARAM_CONTROLLER);
    }
    return ret;
}

extern "C" Std_ReturnType Can_XLdriver_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    std::mutex mtx;
    std::unique_lock lk(mtx);
    if(auto xlStatus = xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId); (xlStatus == XL_SUCCESS) && (chipStateCV.wait_for(lk, 500ms) == std::cv_status::no_timeout))
    {
        *TxErrorCounterPtr = g_canFdSupport ? g_CanFdChipState.txErrorCounter : g_ChipState.txErrorCounter;
        ret = E_OK;
    }
    else
    {
        Can_XLdriver_DetReportError(CAN_XLDRIVER_GET_CONTROLLER_TX_ERROR_COUNTER_SID, CAN_E_PARAM_CONTROLLER);
    }
    return ret;
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
    return static_cast<Std_ReturnType>(xlStatus);
}

/**@} */
