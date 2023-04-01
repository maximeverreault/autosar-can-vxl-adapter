/**
 * @file CanIfMock.cpp
 * @author Maxime Verreault
 * @date 2023-01-27
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Mock functions for CanIf
 * @ingroup Can_XLdriver
 * @addtogroup CanIfMock
 * @{
 */





/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CanIfMock.hpp"

std::function<void (uint8)> _CanIf_ControllerBusOff;
std::function<void (uint8, Can_ControllerStateType)> _CanIf_ControllerModeIndication;
std::function<void (const Can_HwType*, const PduInfoType*)> _CanIf_RxIndication;
std::function<void (PduIdType)> _CanIf_TxConfirmation;
std::function<void ()> _CanIf_ControllerErrorStatePassive;
std::function<void ()> _CanIf_ErrorNotification;

CanIfMock::CanIfMock()
{
    _CanIf_ControllerBusOff = [this](uint8 ControllerId){
        return CanIf_ControllerBusOff(ControllerId);
    };
    _CanIf_ControllerModeIndication = [this](uint8 ControllerId, Can_ControllerStateType ControllerMode){
        return CanIf_ControllerModeIndication(ControllerId, ControllerMode);
    };
    _CanIf_RxIndication = [this](const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr){
        return CanIf_RxIndication(Mailbox, PduInfoPtr);
    };
    _CanIf_TxConfirmation = [this](PduIdType CanTxPduId){
        return CanIf_TxConfirmation(CanTxPduId);
    };
    _CanIf_ControllerErrorStatePassive = [this](){
        return CanIf_ControllerErrorStatePassive();
    };
    _CanIf_ErrorNotification = [this](){
        return CanIf_ErrorNotification();
    };
}

void CanIf_ControllerBusOff(uint8 ControllerId)
{
    return _CanIf_ControllerBusOff(ControllerId);
}
void CanIf_ControllerModeIndication(uint8 ControllerId, Can_ControllerStateType ControllerMode)
{
    return _CanIf_ControllerModeIndication(ControllerId, ControllerMode);
}
void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr)
{
    return _CanIf_RxIndication(Mailbox, PduInfoPtr);
}
void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
    return _CanIf_TxConfirmation(CanTxPduId);
}
void CanIf_ControllerErrorStatePassive(void)
{
    return _CanIf_ControllerErrorStatePassive();
}
void CanIf_ErrorNotification(void)
{
    return _CanIf_ErrorNotification();
}

CanIfMock::~CanIfMock()
{
    _CanIf_ControllerBusOff = nullptr;
    _CanIf_ControllerModeIndication = nullptr;
    _CanIf_RxIndication = nullptr;
    _CanIf_TxConfirmation = nullptr;
    _CanIf_ControllerErrorStatePassive = nullptr;
    _CanIf_ErrorNotification = nullptr;
}


/**@} */
