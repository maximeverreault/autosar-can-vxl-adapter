/**
 * @file Can_XLdriver_InternalTypes.hpp
 * @author Maxime Verreault
 * @date 2023-01-24
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Can driver which communicates with the Vector XL API on Windows in order to test on CANoe without a MCU
 * @ingroup Can_XLdriver
 * @addtogroup Can_XLdriver_InternalTypes
 * @{
 */


#ifndef CAN_XLDRIVER_INTERNALTYPES_HPP
#define CAN_XLDRIVER_INTERNALTYPES_HPP



/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <Can_GeneralTypes.h>
#include <vector>

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
 * @satisfies [SRS_BSW_00318] Each AUTOSAR Basic Software Module file
 *            shall provide version numbers in the header file
 * @satisfies [SRS_BSW_00321] The version numbers of AUTOSAR Basic
 *            Software Modules shall be enumerated according to specific rules
 * */
#define CAN_XLDRIVER_INTERNAL_TYPES_MODULE_ID 80u /**<@brief AUTOSAR module ID defined in AUTOSAR_TR_BSWModuleList.pdf
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_VENDOR_ID 0x0099 /**<@brief KA's unique vendor ID assigned by AUTOSAR
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_MAJOR_VERSION 4u /**<@brief AUTOSAR release major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_MINOR_VERSION 6u /**<@brief AUTOSAR release minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_REVISION_VERSION 0u /**<@brief AUTOSAR release reversion version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_SW_MAJOR_VERSION 1 /**<@brief Software major version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_SW_MINOR_VERSION 0 /**<@brief Software minor version number
* @implements [SRS_BSW_00318] */
#define CAN_XLDRIVER_INTERNAL_TYPES_SW_PATCH_VERSION 0 /**<@brief Software patch version number
* @implements [SRS_BSW_00318] */


/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_INTERMODULE_ASR_CHECK

#ifndef DISABLE_MCAL_FILE_VERSION_CHECK

#if ((CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_MAJOR_VERSION != CAN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_MINOR_VERSION != CAN_GENERALTYPES_AR_RELEASE_MINOR_VERSION) || \
(CAN_XLDRIVER_INTERNAL_TYPES_AR_RELEASE_REVISION_VERSION != CAN_GENERALTYPES_AR_RELEASE_REVISION_VERSION))
#error "AUTOSAR Version Numbers of Can_XLdriver_InternalTypes.hpp and Can_GeneralTypes.h are different"
#endif

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

    bool operator ==(const uint8& other) const
    {
        return selected == other;
    }
};


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/




#endif /* CAN_XLDRIVER_INTERNALTYPES_HPP */


/**@} */
