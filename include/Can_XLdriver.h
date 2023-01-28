/**
 * @file Can_XLdriver.h
 * @author Maxime Verreault
 * @date 2023-01-22
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Can driver which communicates with the Vector XL API on Windows in order to test on CANoe without a MCU
 * @ingroup Can_XLdriver
 * @addtogroup Can_XLdriver
 * @{
 */


#ifndef CAN_XLDRIVER_H
#define CAN_XLDRIVER_H


/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <Std_Types.h>
#include <Can_GeneralTypes.h>

#include "Can_XLdriver_Types.h"


#ifdef __cplusplus
extern "C" {
#endif

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

#ifndef DISABLE_INTERMODULE_ASR_CHECK

    #ifndef DISABLE_MCAL_FILE_VERSION_CHECK

        #if ((CAN_XLDRIVER_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
        (CAN_XLDRIVER_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION) || \
        (CAN_XLDRIVER_AR_RELEASE_REVISION_VERSION != STD_AR_RELEASE_REVISION_VERSION))
            #error "AUTOSAR Version Numbers of Can_XLdriver.h and Standard_Types.h are different"
        #endif

        #if ((CAN_XLDRIVER_AR_RELEASE_MAJOR_VERSION != CAN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION) || \
        (CAN_XLDRIVER_AR_RELEASE_MINOR_VERSION != CAN_GENERALTYPES_AR_RELEASE_MINOR_VERSION) || \
        (CAN_XLDRIVER_AR_RELEASE_REVISION_VERSION != CAN_GENERALTYPES_AR_RELEASE_REVISION_VERSION))
            #error "AUTOSAR Version Numbers of Can_XLdriver.h and Can_GeneralTypes.h are different"
        #endif

    #endif /* DISABLE_MCAL_FILE_VERSION_CHECK */

#endif /* DISABLE_INTERMODULE_ASR_CHECK */


#ifndef DISABLE_INTRAMODULE_ASR_CHECK

    #if (CAN_XLDRIVER_VENDOR_ID != CAN_XLDRIVER_TYPES_VENDOR_ID)
        #error "Vendor ID of Can_XLdriver.h and  are different"
    #endif

    #if (CAN_XLDRIVER_MODULE_ID != CAN_XLDRIVER_TYPES_MODULE_ID)
        #error "Module ID of Can_XLdriver.h and  are different"
    #endif

    #if ((CAN_XLDRIVER_AR_RELEASE_MAJOR_VERSION != CAN_XLDRIVER_TYPES_AR_RELEASE_MAJOR_VERSION) || \
    (CAN_XLDRIVER_AR_RELEASE_MINOR_VERSION != CAN_XLDRIVER_TYPES_AR_RELEASE_MINOR_VERSION) || \
    (CAN_XLDRIVER_AR_RELEASE_REVISION_VERSION != CAN_XLDRIVER_TYPES_AR_RELEASE_REVISION_VERSION))
        #error "AUTOSAR Version Numbers of Can_XLdriver.h and Can_XLdriver_Types.h are different"
    #endif

    #if ((CAN_XLDRIVER_SW_MAJOR_VERSION != CAN_XLDRIVER_TYPES_SW_MAJOR_VERSION) || \
    (CAN_XLDRIVER_SW_MINOR_VERSION != CAN_XLDRIVER_TYPES_SW_MINOR_VERSION) || \
    (CAN_XLDRIVER_SW_PATCH_VERSION != CAN_XLDRIVER_TYPES_SW_PATCH_VERSION))
        #error "Software Version Numbers of Can_XLdriver.h and Can_XLdriver_Types.h are different"
    #endif

#endif /* DISABLE_INTRAMODULE_ASR_CHECK */



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
Std_ReturnType Can_XLdriver_GetControllerMode(uint8 Controller, Can_ControllerStateType* ControllerModePtr);
Std_ReturnType Can_XLdriver_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);
void Can_XLdriver_EnableControllerInterrupts(uint8 Controller);
void Can_XLdriver_DisableControllerInterrupts(uint8 Controller);
Std_ReturnType Can_XLdriver_CheckWakeup(uint8 Controller);
void Can_XLdriver_Init(const Can_ConfigType* Config);
void Can_XLdriver_GetVersionInfo(Std_VersionInfoType* versionInfo);
void Can_XLdriver_DeInit(void);
Std_ReturnType Can_XLdriver_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);


#ifdef __cplusplus
}
#endif

#endif /* CAN_XLDRIVER_H */


/**@} */
