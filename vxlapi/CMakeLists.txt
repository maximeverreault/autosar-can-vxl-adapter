cmake_minimum_required(VERSION 3.14)

project(vxlapi LANGUAGES C)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_definitions(${PROJECT_NAME} INTERFACE WIN32)
target_compile_definitions(${PROJECT_NAME} INTERFACE _DEBUG)
target_compile_definitions(${PROJECT_NAME} INTERFACE _CONSOLE)
target_compile_definitions(${PROJECT_NAME} INTERFACE _MT)
target_compile_definitions(${PROJECT_NAME} INTERFACE _DEBUG_FUNCTIONAL_MACHINERY)

if(${CMAKE_CXX_COMPILER_ID} EQUAL CLANG)
    target_compile_options(${PROJECT_NAME} INTERFACE -fms-compatibility-version=19.10)
    target_compile_options(${PROJECT_NAME} INTERFACE -Wmicrosoft)
    target_compile_options(${PROJECT_NAME} INTERFACE -Wno-invalid-token-paste)
endif()

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/vxlapi.h)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/vxlapi64.lib)