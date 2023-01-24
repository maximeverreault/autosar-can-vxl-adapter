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
*   @file           Compiler.h
*   @implements     Compiler.h_Artifact
*   @version 2.0.0
*
*   @brief   AUTOSAR Base - SWS Compiler abstraction
*   @details The file Compiler.h provides macros for the encapsulation of definitions and
*            declarations.
*            This file contains sample code only. It is not part of the production code deliverables
*
*   @addtogroup BASE_COMPONENT
*   @{
*/

/**
* @requirements COMPILER047
*/
#ifndef COMPILER_H
#define COMPILER_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Compiler_Cfg.h"
#include "CompilerDefinition.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
* @brief  Parameters that shall be published within the compiler abstraction header file and also in
          the module's description file.
@{
*/
#define COMPILER_VENDOR_ID                      43
#define COMPILER_AR_RELEASE_MAJOR_VERSION       4
#define COMPILER_AR_RELEASE_MINOR_VERSION       4
#define COMPILER_AR_RELEASE_REVISION_VERSION    0
#define COMPILER_SW_MAJOR_VERSION               2
#define COMPILER_SW_MINOR_VERSION               0
#define COMPILER_SW_PATCH_VERSION               0
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
* @brief The memory class AUTOMATIC shall be provided as empty definition, used for the declaration
*        of local pointers.
*/
#define AUTOMATIC

/**
* @brief The memory class TYPEDEF shall be provided as empty definition. This memory class shall be
*        used within type definitions, where no memory qualifier can be specified. This can be
*        necessary for defining pointer types, with e.g. P2VAR, where the macros require two
*        parameters. First parameter can be specified in the type definition (distance to the memory
*        location referenced by the pointer), but the second one (memory allocation of the pointer
*        itself) cannot be defined at this time. Hence memory class TYPEDEF shall be applied.
*/
#define TYPEDEF

/**
* @brief The compiler abstraction shall provide the NULL_PTR define with a void pointer to zero
*        definition.
*/
#define NULL_PTR ((void *)0)

/**************************************** Green Hills *********************************************/
#ifdef _GREENHILLS_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE              __inline

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static __inline

    /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC      __interrupt
#endif /* #ifdef _GREENHILLS_C_S32K3XX_ */

/**************************************** Wind River Diab *****************************************/
#ifdef _DIABDATA_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE              inline __attribute__((always_inline))

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static inline __attribute__((always_inline))

    /**
    * @brief Compiler abstraction for specifying an interrupt handler. Diab version 7 and above use clang
    */
#ifdef __clang__
    #define INTERRUPT_FUNC      __attribute__((interrupt))
#else
    #define INTERRUPT_FUNC      __interrupt__
#endif /* __clang__*/

#endif /* #ifdef _DIABDATA_C_S32K3XX_ */

/*************************************** CodeWarrior **********************************************/
#ifdef _CODEWARRIOR_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE              inline

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static inline

     /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC      __declspec(interrupt nowarn)
#endif /* #ifdef _CODEWARRIOR_C_S32K3XX_ */

/*************************************** Cosmic ***************************************************/
#ifdef _COSMIC_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE @inline

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static @inline

    /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC @interrupt
#endif /* #ifdef _COSMIC_C_S32K3XX_ */
/*************************************** HighTec **********************************************/
#ifdef _HITECH_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE              __inline

/**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
*/
    #define LOCAL_INLINE            static __inline

    /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC      __attribute__((interrupt));
#endif /* #ifdef _HITECH_C_S32K3XX_ */

/**************************************** Linaro *********************************************/
#ifdef _LINARO_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE                  inline __attribute__((always_inline))

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static inline __attribute__((always_inline))

    /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC
#endif /* #ifdef _LINARO_C_S32K3XX_ */
/**************************************** DS5 *************************************************/
#ifdef _ARM_DS5_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE              __inline

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static __inline

    /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC
#endif /* #ifdef _ARM_DS5_C_S32K3XX_ */

/**************************************** IAR *************************************************/
#ifdef _IAR_C_S32K3XX_
/* Prototypes for intrinsic functions */
    /**
    * @brief The compiler abstraction shall provide the INLINE define for abstraction of the keyword
    *        inline.
    */
    #define INLINE              inline __attribute__((always_inline))

    /**
    * @brief The compiler abstraction shall provide the LOCAL_INLINE define for abstraction of the
    *       keyword inline in functions with "static" scope
    */
    #define LOCAL_INLINE            static inline __attribute__((always_inline))

    /**
    * @brief Compiler abstraction for specifying an interrupt handler.
    */
    #define INTERRUPT_FUNC
#endif /* #ifdef _IAR_C_S32K3XX_ */



/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef COMPILER_H */

/** @} */
