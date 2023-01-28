/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.0
* Build Version : S32K3_RTD_2_0_0_D2203_ASR_REL_4_4_REV_0000_20220331
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/
/**
*   @file    Can_GeneralTypes.h
*   @implements Can_GeneralTypes.h_Artifact
*   @version 2.0.0
*
*   @brief   AUTOSAR Base - General Types.
*   @details API header for all types and constants that are shared among the AUTOSAR CAN
*            modules Can, CanIf and CanTrcv.
*            This file contains sample code only. It is not part of the production code deliverables
*   @addtogroup BASE_COMPONENT
*   @{
*/

#ifndef CAN_GENERAL_TYPES_H
#define CAN_GENERAL_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "ComStack_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/**
@{
* @brief Parameters that shall be published within the modules header file.
*       The integration of incompatible files shall be avoided.
*/
#define CAN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION      4
#define CAN_GENERALTYPES_AR_RELEASE_MINOR_VERSION      4
#define CAN_GENERALTYPES_AR_RELEASE_REVISION_VERSION      0

/**@}*/
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/**
* @brief Transmit request could not be processed because no
*        transmit object was available
* @implements SymbolDefinitions_enum
*/
#define CAN_BUSY    0x02
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/**
* @brief          CAN Controller State Modes of operation.
* @details        States that are used by the several ControllerMode functions
* @implements     Can_ControllerStateType_enum
*/
typedef enum
{
    CAN_CS_UNINIT  = 0U,    /**< @brief CAN controller state UNINIT */
    CAN_CS_STARTED,         /**< @brief CAN controller state STARTED */
    CAN_CS_STOPPED,         /**< @brief CAN controller state STOPPED */
    CAN_CS_SLEEP            /**< @brief CAN controller state SLEEP */
} Can_ControllerStateType;

/**
* @brief          CAN Controller State Modes of operation.
* @details        Error states of a CAN controller
* @implements     Can_ErrorStateType_enum
*/
typedef enum
{
    CAN_ERRORSTATE_ACTIVE = 0U,     /**< @brief The CAN controller takes fully part in communication. */
    CAN_ERRORSTATE_PASSIVE,         /**< @brief The CAN controller takes part in communication, but does not send active error frames. */
    CAN_ERRORSTATE_BUSOFF           /**< @brief The CAN controller does not take part in communication. */
} Can_ErrorStateType;

/**
* @brief          CAN Transceiver modes.
* @details        Operating modes of the CAN Transceiver Driver.
*
*/
typedef enum
{
    CANTRCV_TRCVMODE_NORMAL = 0U, /**< @brief Transceiver mode NORMAL */
    CANTRCV_TRCVMODE_STANDBY,     /**< @brief Transceiver mode STANDBY */
    CANTRCV_TRCVMODE_SLEEP        /**< @brief Transceiver mode SLEEP */
} CanTrcv_TrcvModeType;

/**
* @brief
* @details   This type shall be used to control the CAN transceiver concerning wake up events and wake up notifications.
* According to [SWS_CanTrcv_00164] it should be present in Can_GeneralTypes.h
*/
typedef enum
{
    CANTRCV_WUMODE_ENABLE = 0x00,   /* The notification for wakeup events is enabled on the addressed transceiver. */
    CANTRCV_WUMODE_DISABLE = 0x01,  /* The notification for wakeup events is disabled on the addressed transceiver. */
    CANTRCV_WUMODE_CLEAR = 0x02	 	/* A stored wakeup event is cleared on the addressed transceiver. */
} CanTrcv_TrcvWakeupModeType;


/**
* @brief
* @details   This type denotes the wake up reason detected by the CAN transceiver in detail.
* According to [SWS_CanTrcv_00165] it should be present in Can_GeneralTypes.h
*/
typedef enum
{
    CANTRCV_WU_ERROR	 		= 0, /* Due to an error wake up reason was not detected. This value may only be reported when error was reported to DEM before. */
    CANTRCV_WU_NOT_SUPPORTED 	= 1, /* The transceiver does not support any information for the wake up reason.  */
    CANTRCV_WU_BY_BUS			= 2, /* The transceiver has detected, that the network has caused the wake up of the ECU. */
    CANTRCV_WU_INTERNALLY		= 3, /* The transceiver has detected, that the network has woken up by the ECU via a request to NORMAL mode. */
    CANTRCV_WU_RESET			= 4, /* The transceiver has detected, that the "wake up" is due to an ECU reset. */
    CANTRCV_WU_POWER_ON			= 5, /* The transceiver has detected, that the "wake up" is due to an ECU reset after power on. */
    CANTRCV_WU_BY_PIN			= 6, /* The transceiver has detected a wake-up event at one of the transceiver's pins (not at the CAN bus). */
    CANTRCV_WU_BY_SYSERR		= 7  /* The transceiver has detected, that the wake up of the ECU was caused by a HW related device failure. */
} CanTrcv_TrcvWakeupReasonType;
/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @{
* @brief          Can_IdType
* @details        Represents the Identifier of an L-PDU. The two most significant bits specify theframe type:
* -00 CAN message with Standard CAN ID
* -01 CAN FD frame with Standard CAN ID
* -10 CAN message with Extended CAN ID
* -11 CAN FD frame with Extended CAN ID
* @implements     Can_IdType_type
*/
typedef  uint32 Can_IdType;


/**
* @brief          Can_PduType
* @details        Type used to provide ID, DLC, SDU from CAN interface to CAN driver.
*                 HTH or HRH = ID+DLC+SDU.
* @implements     Can_PduType_structure
*/
typedef struct
{
    Can_IdType id; /**< @brief CAN L-PDU = Data Link Layer Protocol Data Unit.
                                             Consists of Identifier, DLC and Data(SDU)  It is
                                             uint32 for CAN_EXTENDEDID=STD_ON, else is uint16.
                                             */
    PduIdType swPduHandle; /**< @brief The L-PDU Handle = defined and placed
                                                     inside the CanIf module layer. Each handle
                                                     represents an L-PDU, which is a constant
                                                     structure with information for Tx/Rx
                                                     processing. */
    uint8 length; /**< @brief DLC = Data Length Code (part of L-PDU that describes
                                            the SDU length). */
    uint8 * sdu; /**< @brief CAN L-SDU = Link Layer Service Data
                                                          Unit. Data that is transported inside
                                                          the L-PDU. */
} Can_PduType;


/**
* @{
* @brief          Can_HwHandleType
* @details        Represents the hardware object handles of a CAN hardware unit.
                  For CAN hardware units with more than 255 HW objects use extended range.
*                    used by "Can_Write" function.
*                 The driver does not distinguish between Extended and Mixed transmission modes.
*                 Extended transmission mode of operation behaves the same as Mixed mode.
* @implements     Can_HwHandleType_type
*/
typedef uint16 Can_HwHandleType;

/**
* @brief          Can_HwType
* @details        This type defines a data structure which clearly provides an Hardware Object
                    Handle including its corresponding CAN Controller and therefore CanDrv as well
                    as the specific CanId.
* @implements     Can_HwType_structure
*/

typedef struct
{
    Can_IdType CanId; /**< @brief Standard/Extended CAN ID of CAN L-PDU. */

    Can_HwHandleType  Hoh; /**< @brief ID of the corresponding Hardware Object Range */

    uint8 ControllerId; /**< @brief ControllerId provided by CanIf clearly
                                            identify the corresponding controller */

} Can_HwType;

/**
* @brief          Can_TimeStampType
* @details        Variables of this type are used to express time stamps based on relative time.
*/
typedef struct
{
    uint32 nanoseconds;   /**< @brief Nanoseconds part of the time. */
    uint32 seconds;       /**< @brief Seconds part of the time. */
}Can_TimeStampType;
/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

#endif /*CAN_GENERAL_TYPES_H*/

/** @} */
