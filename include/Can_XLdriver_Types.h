/**
 * @file Can_XLdriver_Types.h
 * @author Maxime Verreault
 * @date 2023-01-24
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Can driver which communicates with the Vector XL API on Windows in order to test on CANoe without a MCU
 * @ingroup Can_XLdriver
 * @addtogroup Can_XLdriver_Types
 * @{
 */


#ifndef CAN_XLDRIVER_TYPES_H
#define CAN_XLDRIVER_TYPES_H

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/


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
#define CAN_XLDRIVER_TYPES_MODULE_ID 80u /**<@brief AUTOSAR module ID defined in AUTOSAR_TR_BSWModuleList.pdf
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_VENDOR_ID 0x0099 /**<@brief KA's unique vendor ID assigned by AUTOSAR
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_AR_RELEASE_MAJOR_VERSION 4u /**<@brief AUTOSAR release major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_AR_RELEASE_MINOR_VERSION 6u /**<@brief AUTOSAR release minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_AR_RELEASE_REVISION_VERSION 0u /**<@brief AUTOSAR release reversion version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_SW_MAJOR_VERSION 1 /**<@brief Software major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_SW_MINOR_VERSION 0 /**<@brief Software minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_TYPES_SW_PATCH_VERSION 0 /**<@brief Software patch version number
* @implements [SRS_BSW_00318] */


/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_INTERMODULE_ASR_CHECK

#ifndef DISABLE_MCAL_FILE_VERSION_CHECK



#endif /* DISABLE_MCAL_FILE_VERSION_CHECK */

#endif /* DISABLE_INTERMODULE_ASR_CHECK */


#ifndef DISABLE_INTRAMODULE_ASR_CHECK



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
typedef struct
{

} Can_ConfigType;




#ifdef __cplusplus
}
#endif

#endif /* CAN_XLDRIVER_TYPES_H */


/**@} */
