/**
 * @file CanIf_Can.h
 * @author Maxime Verreault
 * @date 2023-01-22
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief STUB FILE ONLY
 * @ingroup Stubs
 * @addtogroup CanIf_Can
 * @{
 */


#ifndef CANIF_CAN_H
#define CANIF_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <Can_GeneralTypes.h>
#include "ComStack_Types.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
 * @satisfies [SRS_BSW_00318] Each AUTOSAR Basic Software Module file
 *            shall provide version numbers in the header file
 * @satisfies [SRS_BSW_00321] The version numbers of AUTOSAR Basic
 *            Software Modules shall be enumerated according to specific rules
 * */
#define CANIF_CAN_MODULE_ID 69 /**<@brief AUTOSAR module ID defined in AUTOSAR_TR_BSWModuleList.pdf
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_VENDOR_ID 0x0099 /**<@brief KA's unique vendor ID assigned by AUTOSAR
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_AR_RELEASE_MAJOR_VERSION 4u /**<@brief AUTOSAR release major version number
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_AR_RELEASE_MINOR_VERSION 6u /**<@brief AUTOSAR release minor version number
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_AR_RELEASE_REVISION_VERSION 0u /**<@brief AUTOSAR release reversion version number
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_SW_MAJOR_VERSION 1 /**<@brief Software major version number
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_SW_MINOR_VERSION 0 /**<@brief Software minor version number
* @implements [SRS_BSW_00318] */
#define CANIF_CAN_SW_PATCH_VERSION 0 /**<@brief Software patch version number
* @implements [SRS_BSW_00318] */


/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/





/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/


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


/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/
void CanIf_ControllerBusOff(uint8 ControllerId);
void CanIf_ControllerModeIndication(uint8 ControllerId,
                                    Can_ControllerStateType ControllerMode);
void CanIf_RxIndication(const Can_HwType* Mailbox,
                        const PduInfoType* PduInfoPtr);
void CanIf_TxConfirmation(PduIdType CanTxPduId);
void CanIf_ControllerErrorStatePassive(void);
void CanIf_ErrorNotification(void);


#ifdef __cplusplus
}
#endif

#endif /* CANIF_CAN_H */


/**@} */ /* END OF addtogroup <> */
