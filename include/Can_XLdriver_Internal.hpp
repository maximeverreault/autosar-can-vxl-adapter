/**
 * @file Can_XLdriver_Internal.hpp
 * @author Maxime Verreault
 * @date 2023-01-24
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Internal data and functions for CAN XL Driver
 * @ingroup Can_XLdriver
 * @addtogroup Can_XLdriver_Internal
 * @{
 */


#ifndef CAN_XLDRIVER_INTERNAL_HPP
#define CAN_XLDRIVER_INTERNAL_HPP

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <string>
#include <condition_variable>
#include "vxlapi.h"
#include <thread>
#include <map>

#include "Det.h"

#include "Can_XLdriver_InternalTypes.hpp"
#include "Can_XLdriver.h"


/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
 * @satisfies [SRS_BSW_00318] Each AUTOSAR Basic Software Module file
 *            shall provide version numbers in the header file
 * @satisfies [SRS_BSW_00321] The version numbers of AUTOSAR Basic
 *            Software Modules shall be enumerated according to specific rules
 * */
#define CAN_XLDRIVER_INTERNAL_MODULE_ID 80 /**<@brief AUTOSAR module ID defined in AUTOSAR_TR_BSWModuleList.pdf
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_VENDOR_ID 0x0099 /**<@brief KA's unique vendor ID assigned by AUTOSAR
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_AR_RELEASE_MAJOR_VERSION 4u /**<@brief AUTOSAR release major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_AR_RELEASE_MINOR_VERSION 6u /**<@brief AUTOSAR release minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_AR_RELEASE_REVISION_VERSION 0u /**<@brief AUTOSAR release reversion version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_SW_MAJOR_VERSION 1 /**<@brief Software major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_SW_MINOR_VERSION 0 /**<@brief Software minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_SW_PATCH_VERSION 0 /**<@brief Software patch version number
* @implements [SRS_BSW_00318] */


/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_INTERMODULE_ASR_CHECK

#if ((CAN_XLDRIVER_INTERNAL_AR_RELEASE_MAJOR_VERSION != DET_AR_RELEASE_MAJOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_AR_RELEASE_MINOR_VERSION != DET_AR_RELEASE_MINOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_AR_RELEASE_REVISION_VERSION != DET_AR_RELEASE_REVISION_VERSION))
#error "AUTOSAR Version Numbers of Can_XLdriver_Internal.hpp and Det.h are different"
#endif

#endif /* DISABLE_INTERMODULE_ASR_CHECK */


#ifndef DISABLE_INTRAMODULE_ASR_CHECK

#if (CAN_XLDRIVER_INTERNAL_VENDOR_ID != CAN_XLDRIVER_VENDOR_ID)
#error "Vendor ID of Can_XLdriver_Internal.hpp and Can_XLdriver.h are different"
#endif

#if (CAN_XLDRIVER_INTERNAL_MODULE_ID != CAN_XLDRIVER_MODULE_ID)
#error "Module ID of Can_XLdriver_Internal.hpp and Can_XLdriver.h are different"
#endif

#if ((CAN_XLDRIVER_INTERNAL_AR_RELEASE_MAJOR_VERSION != CAN_XLDRIVER_AR_RELEASE_MAJOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_AR_RELEASE_MINOR_VERSION != CAN_XLDRIVER_AR_RELEASE_MINOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_AR_RELEASE_REVISION_VERSION != CAN_XLDRIVER_AR_RELEASE_REVISION_VERSION))
#error "AUTOSAR Version Numbers of Can_XLdriver_Internal.hpp and Can_XLdriver.h are different"
#endif

#if ((CAN_XLDRIVER_INTERNAL_SW_MAJOR_VERSION != CAN_XLDRIVER_SW_MAJOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_SW_MINOR_VERSION != CAN_XLDRIVER_SW_MINOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_SW_PATCH_VERSION != CAN_XLDRIVER_SW_PATCH_VERSION))
#error "Software Version Numbers of Can_XLdriver_Internal.hpp and Can_XLdriver.h are different"
#endif

#if (CAN_XLDRIVER_INTERNAL_VENDOR_ID != CAN_XLDRIVER_INTERNAL_TYPES_VENDOR_ID)
#error "Vendor ID of Can_XLdriver_Internal.hpp and Can_XLdriver_InternalTypes.hpp are different"
#endif

#if (CAN_XLDRIVER_INTERNAL_MODULE_ID != CAN_XLDRIVER_INTERNAL_TYPES_MODULE_ID)
#error "Module ID of Can_XLdriver_Internal.hpp and Can_XLdriver_InternalTypes.hpp are different"
#endif

#if ((CAN_XLDRIVER_INTERNAL_AR_RELEASE_MAJOR_VERSION != CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_MAJOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_AR_RELEASE_MINOR_VERSION != CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_MINOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_AR_RELEASE_REVISION_VERSION != CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_REVISION_VERSION))
#error "AUTOSAR Version Numbers of Can_XLdriver_Internal.hpp and Can_XLdriver_InternalTypes.hpp are different"
#endif

#if ((CAN_XLDRIVER_INTERNAL_SW_MAJOR_VERSION != CAN_XLDRIVER_INTERNAL_TYPES_SW_MAJOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_SW_MINOR_VERSION != CAN_XLDRIVER_INTERNAL_TYPES_SW_MINOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_SW_PATCH_VERSION != CAN_XLDRIVER_INTERNAL_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Can_XLdriver_Internal.hpp and Can_XLdriver_InternalTypes.hpp are different"
#endif

#endif /* DISABLE_INTRAMODULE_ASR_CHECK */



/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
constexpr unsigned ENABLE_CAN_FD_MODE_NO_ISO = 0;        // switch to activate no iso mode on a CAN FD channel
constexpr unsigned RX_QUEUE_SIZE             = 4096;     // internal driver queue size in CAN events
constexpr unsigned RX_QUEUE_SIZE_FD          = 16384;    // driver queue size for CAN-FD Rx events

constexpr uint8 CAN_XLDRIVER_INIT_SID = 0x00;
constexpr uint8 CAN_XLDRIVER_GET_VERSION_INFO_SID = 0x07;
constexpr uint8 CAN_XLDRIVER_DEINIT_SID = 0x10;
constexpr uint8 CAN_XLDRIVER_SET_BAUDRATE_SID = 0x0F;
constexpr uint8 CAN_XLDRIVER_SET_CONTROLLER_MODE_SID = 0x03;
constexpr uint8 CAN_XLDRIVER_DISABLE_CONTROLLER_INTERRUPTS_SID = 0x04;
constexpr uint8 CAN_XLDRIVER_ENABLE_CONTROLLER_INTERRUPTS_SID = 0x05;
constexpr uint8 CAN_XLDRIVER_CHECK_WAKEUP_SID = 0x0B;
constexpr uint8 CAN_XLDRIVER_GET_CONTROLLER_ERROR_STATE_SID = 0x11;
constexpr uint8 CAN_XLDRIVER_GET_CONTROLLER_MODE_SID = 0x12;
constexpr uint8 CAN_XLDRIVER_GET_CONTROLLER_RX_ERROR_COUNTER_SID = 0x30;
constexpr uint8 CAN_XLDRIVER_GET_CONTROLLER_TX_ERROR_COUNTER_SID = 0x31;
constexpr uint8 CAN_XLDRIVER_WRITE_SID = 0x06;
constexpr uint8 CAN_XLDRIVER_MAIN_FUNCTION_WRITE_SID = 0x01;
constexpr uint8 CAN_XLDRIVER_MAIN_FUNCTION_READ_SID = 0x08;
constexpr uint8 CAN_XLDRIVER_MAIN_FUNCTION_BUSOFF_SID = 0x09;
constexpr uint8 CAN_XLDRIVER_MAIN_FUNCTION_WAKEUP_SID = 0x0A;
constexpr uint8 CAN_XLDRIVER_MAIN_FUNCTION_MODE_SID = 0x0C;

constexpr uint8 CAN_E_PARAM_POINTER = 0x01;
constexpr uint8 CAN_E_PARAM_HANDLE = 0x02;
constexpr uint8 CAN_E_PARAM_DATA_LENGTH = 0x03;
constexpr uint8 CAN_E_PARAM_CONTROLLER = 0x04;
constexpr uint8 CAN_E_UNINIT = 0x05;
constexpr uint8 CAN_E_TRANSITION = 0x06;
constexpr uint8 CAN_E_PARAM_BAUDRATE = 0x07;
constexpr uint8 CAN_E_INIT_FAILED = 0x09;

constexpr uint8 CAN_E_DATALOST = 0x01;

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
inline std::string g_AppName = "xlCANdemo";               //!< Application name which is displayed in VHWconf
inline XLportHandle    g_xlPortHandle              = XL_INVALID_PORTHANDLE;      //!< Global porthandle (we use only one!)
inline XLdriverConfig  g_xlDrvConfig;                                            //!< Contains the actual hardware configuration
inline XLaccess        g_xlChannelMask             = 0;                          //!< Global channelmask (includes all founded channels)
inline XLaccess        g_xlPermissionMask          = 0;                          //!< Global permissionmask (includes all founded channels)
inline unsigned int    g_BaudRate                  = 500000;                     //!< Default baudrate
inline int             g_silent                    = 0;                          //!< flag to visualize the message events (on/off)
inline unsigned int    g_canFdSupport              = 0;                          //!< Global CAN FD support flag
inline unsigned int    g_canFdModeNoIso            = ENABLE_CAN_FD_MODE_NO_ISO;  //!< Global CAN FD ISO (default) / no ISO mode flag
inline XLaccess      xlChanMaskTx = 0;
inline std::condition_variable chipStateCV;
inline s_xl_chip_state g_ChipState{1, 0, 0};
inline XL_CAN_EV_CHIP_STATE g_CanFdChipState{1, 0, 0, 0, 0};

inline std::atomic<bool> consumerThreadRun{true};                                        //!< flag to start/stop the RX thread
inline std::atomic<bool> canModuleActive{false};
inline std::jthread consumerThread;

inline std::map<Can_IdType, Can_HwHandleType> Id2HwHandleMap;

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
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
    for(unsigned char & el : source.reserved)
    {
        el = 0;
    }
    source.tagData.canMsg.canId = 0;
    source.tagData.canMsg.dlc = 0;
    source.tagData.canMsg.msgFlags = 0;
    for(unsigned char & el : source.tagData.canMsg.data)
    {
        el = 0;
    }
    for(unsigned char & el : source.tagData.canMsg.reserved)
    {
        el = 0;
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

XLstatus demoInitDriver(XLaccess &pxlChannelMaskTx, unsigned int &pxlChannelIndex);
void eventConsumer(const std::stop_token& stoken);
void eventCanFdConsumer(const std::stop_token& stoken);

inline void Can_XLdriver_DetReportError(const uint8 ApiId, const uint8 ErrorId)
{
    (void) Det_ReportError(CAN_XLDRIVER_MODULE_ID, 0U, ApiId, ErrorId);
}

inline void demoCreateRxThread(std::jthread& threadRef)
{
    threadRef = std::jthread(g_canFdSupport ? eventCanFdConsumer : eventConsumer);
}

inline Std_ReturnType getHwHandle(const std::map<Can_IdType, Can_HwHandleType>& map, const Can_IdType& canId, Can_HwHandleType& hwHandle)
{
    Std_ReturnType ret = E_NOT_OK;
    try
    {
        hwHandle = map.at(canId);
        ret = E_OK;
    }
    catch(std::out_of_range& e)
    {
        (void) e;
        hwHandle = 0;
    }
    return ret;
}


#endif /* CAN_XLDRIVER_INTERNAL_HPP */


/**@} */
