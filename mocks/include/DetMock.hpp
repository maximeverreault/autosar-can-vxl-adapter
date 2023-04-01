/**
 * @file DetMock.hpp
 * @author Maxime Verreault
 * @date 2023-01-27
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Mock functions for Det
 * @ingroup Can_XLdriver
 * @addtogroup DetMock
 * @{
 */


#ifndef DETMOCK_HPP
#define DETMOCK_HPP

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <functional>
#include "Std_Types.h"
#include "Det.h"
#include <gmock/gmock.h>


struct DetMock
{
    DetMock();
    MOCK_METHOD(Std_ReturnType, Det_ReportError, (uint16, uint8, uint8, uint8));
    ~DetMock();
};

#endif /* DETMOCK_HPP */


/**@} */
