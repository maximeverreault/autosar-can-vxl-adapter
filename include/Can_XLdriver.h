/**
 * @file Can_XLdriver.h
 * @author Maxime Verreault
 * @date 2023-01-22
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief TODO: add description
 * @ingroup TODO: add or remove ingroup
 * @addtogroup Can_XLdriver
 * @{  TODO: close the doxygen group at the end of the file with @}
 */


#ifndef CAN_XLDRIVER_H
#define CAN_XLDRIVER_H

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

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
 * @satisfies [SRS_BSW_00318] Each AUTOSAR Basic Software Module file
 *            shall provide version numbers in the header file
 * @satisfies [SRS_BSW_00321] The version numbers of AUTOSAR Basic
 *            Software Modules shall be enumerated according to specific rules
 * */
#define CAN_XLDRIVER_MODULE_ID 80 /**<@brief AUTOSAR module ID defined in AUTOSAR_TR_BSWModuleList.pdf
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_VENDOR_ID 0x0099 /**<@brief KA's unique vendor ID assigned by AUTOSAR
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_AR_RELEASE_MAJOR_VERSION 4u /**<@brief AUTOSAR release major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_AR_RELEASE_MINOR_VERSION 6u /**<@brief AUTOSAR release minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_AR_RELEASE_REVISION_VERSION 0u /**<@brief AUTOSAR release reversion version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_SW_MAJOR_VERSION 1 /**<@brief Software major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_SW_MINOR_VERSION 0 /**<@brief Software minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_SW_PATCH_VERSION 0 /**<@brief Software patch version number
* @implements [SRS_BSW_00318] */


/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
// TODO: file version check for each included file from other module. TIP: use live template (CTRL + J)

#endif //DISABLE_MCAL_INTERMODULE_ASR_CHECK


#ifndef DISABLE_MCAL_INTRAMODULE_ASR_CHECK

// TODO: file version check for each included file from the same module. TIP: use live template (CTRL + J)

#endif //DISABLE_MCAL_INTRAMODULE_ASR_CHECK



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
Std_ReturnType Can_XLdriver_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);
Std_ReturnType Can_XLdriver_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr);
Std_ReturnType Can_XLdriver_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr);
Std_ReturnType Can_XLdriver_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);


#ifdef __cplusplus
}
#endif

#endif //CAN_XLDRIVER_H

// TODO: indicate group
/**@} */ // END OF addtogroup <>
