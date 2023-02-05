/**
 * @file CanIfMock.hpp
 * @author Maxime Verreault
 * @date 2023-01-27
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Mocked functions for CanIf
 * @ingroup Can_XLdriver
 * @addtogroup CanIfMock
 * @{
 */


#ifndef CANIFMOCK_HPP
#define CANIFMOCK_HPP

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <CanIf_Can.h>
#include <gmock/gmock.h>

struct CanIfMock
{
    CanIfMock();
    MOCK_METHOD(void, CanIf_ControllerBusOff, (uint8));
    MOCK_METHOD(void, CanIf_ControllerModeIndication, (uint8, Can_ControllerStateType));
    MOCK_METHOD(void, CanIf_RxIndication, (const Can_HwType*, const PduInfoType*));
    MOCK_METHOD(void, CanIf_TxConfirmation, (PduIdType));
    MOCK_METHOD(void, CanIf_ControllerErrorStatePassive, ());
    MOCK_METHOD(void, CanIf_ErrorNotification, ());
    ~CanIfMock();
};


#endif /* CANIFMOCK_HPP */


/**@} */
