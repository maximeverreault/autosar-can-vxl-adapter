/**
 * @file main.cpp
 * @author Maxime Verreault
 * @date 2023-01-24
 * @copyright COPYRIGHT(c) KONGSBERG AUTOMOTIVE All rights reserved.
 * @brief Main function to run in standalone mode
 * @ingroup Can_XLdriver
 * @addtogroup main
 * @{
 */




/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Can_XLdriver.h"
#include "Can_XLdriver_Internal.hpp"
#include "vxlapi.h"
#include <fmt/format.h>
#include <boost/program_options.hpp>
#include <thread>

namespace po = boost::program_options;


/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
[[noreturn]] void mainThread()
{
    while(true);
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(int argc, char *argv[])
{
    XLstatus      xlStatus;
    unsigned int  xlChanIndex = 0;
    unsigned int  txID = 0x01;

    fmt::print(
            "┌{0:─^{3}}┐\n"
            "│{1: ^{3}}│\n"
            "│{2: ^{3}}│\n"
            #ifdef WIN64
            "│{4: ^{3}}│\n"
            #endif
            "└{0:─^{3}}┘\n", "", "xlCANdemo - Test Application for XL Family Driver API", fmt::format("Vector Informatik GmbH, {}", __DATE__), 58, "- 64bit Version -");

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("baudrate", po::value<unsigned int>(), "set baudrate (kbps)")
            ("appname", po::value<std::string>(), "Name of the application to be read (e.g. \"xlCANcontrol\").\nApplication names are listed in the Vector Hardware Configuration tool.")
            ("txid", po::value<unsigned int>(), "set ID for sending data")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::stringstream str{};
        str << desc;
        fmt::print("{}\n", str.str());
        return 1;
    }

    if (vm.count("baudrate")) {
        g_BaudRate = vm["baudrate"].as<unsigned int>() * 1000;
        fmt::print("Baudrate = {}kbps\n", vm["baudrate"].as<unsigned int>());
    } else {
        fmt::print("Baudrate was not set. Default Baudrate selected ({}kbps)\n", g_BaudRate/1000);
    }

    if (vm.count("appname")) {
        g_AppName = vm["appname"].as<std::string>();
        fmt::print("AppName = {}\n", vm["appname"].as<std::string>());
    } else {
        fmt::print("AppName was not set. Default AppName selected (\"{}\")\n", g_AppName);
    }

    if (vm.count("txid")) {
        txID = vm["txid"].as<unsigned int>();
        fmt::print("TX ID = {:#X}\n", vm["txid"].as<unsigned int>());
    } else {
        fmt::print("TX ID was not set. Default TX ID selected ({})\n", txID);
    }

    xlStatus = demoInitDriver(xlChanMaskTx, xlChanIndex);
    fmt::print("- Init             : {}\n",  xlGetErrorString(xlStatus));

    if(XL_SUCCESS == xlStatus) {
        demoCreateRxThread(consumerThread);
        fmt::print("- Create RX thread : {}\n",  xlGetErrorString(XL_SUCCESS));
    }

    if(XL_SUCCESS == xlStatus) {
        xlStatus = xlActivateChannel(g_xlPortHandle, g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
        fmt::print("- ActivateChannel  : CM={:#X}, {}\n", g_xlChannelMask, xlGetErrorString(xlStatus));
    }
    Can_ErrorStateType res;
    Can_XLdriver_GetControllerErrorState(0, &res);
    std::array<uint8, 8> data{69, 21, 87, 34, 0, 1, 2, 4};
    const Can_PduType pduinfo{
            0x69 | 0x40000000, 0, data.size(), data.data()
    };
    Can_XLdriver_Write(0, &pduinfo);
    std::jthread mainLoopThread{mainThread};
    mainLoopThread.join();

    return xlStatus;
}

/**@} */
