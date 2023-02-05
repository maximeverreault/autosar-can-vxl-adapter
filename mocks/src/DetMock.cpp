/**
 * @file DetMock.cpp
 * @author Maxime Verreault
 * @date 2023-01-27
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Mock functions for Det
 * @ingroup Can_XLdriver
 * @addtogroup DetMock
 * @{
 */



/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <functional>
#include "Std_Types.h"
#include <Det.h>
#include "DetMock.hpp"

std::function<Std_ReturnType (uint16, uint8, uint8, uint8)> _Det_ReportError;

DetMock::DetMock()
{
    _Det_ReportError = [this](uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId){
        return Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId);
    };
}

DetMock::~DetMock()
{
    _Det_ReportError = nullptr;
}
Std_ReturnType Det_ReportError ( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId )
{
    return _Det_ReportError(ModuleId, InstanceId, ApiId, ErrorId);
}



/**@} */
