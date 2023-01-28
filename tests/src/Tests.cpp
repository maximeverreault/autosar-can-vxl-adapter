/**
 * @file Tests
 * @author Maxime Verreault
 * @date 2023-01-24
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief 
 */




/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "DetMock.hpp"
#include "Can_XLdriver.h"
#include "vxlapi.h"
#include "Can_XLdriver_Internal.hpp"
#include "CanIfMock.hpp"
using namespace ::testing;

class GetControllerErrorState: public testing::Test
{
protected:
    StrictMock<DetMock> detMock;
    StrictMock<CanIfMock> canIfMock;

    GetControllerErrorState(){
        XLstatus xLstatus;
        g_canFdSupport = false;
        xLstatus = xlOpenDriver();
        if(xLstatus == XL_SUCCESS)
            xLstatus = xlGetDriverConfig(&g_xlDrvConfig);
        auto mask = xlGetChannelMask(XL_HWTYPE_VIRTUAL,-1, -1);
        g_xlPermissionMask = mask;
        if(xLstatus == XL_SUCCESS)
            xLstatus = xlOpenPort(&g_xlPortHandle, std::string{"vxlAdapter"}.data(), mask, &g_xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
        consumerThread = std::jthread(eventConsumer);
        if(xLstatus == XL_SUCCESS)
            xlActivateChannel(g_xlPortHandle, mask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
    }

    ~GetControllerErrorState() override
    {
        consumerThread.request_stop();
    }
};

TEST_F(GetControllerErrorState, VerifyReturnedState)
{
    Can_ErrorStateType errorState = CAN_ERRORSTATE_PASSIVE;
    
    EXPECT_EQ(Can_XLdriver_GetControllerErrorState(0, &errorState), E_OK);
    EXPECT_EQ(errorState, CAN_ERRORSTATE_ACTIVE);
}
 
