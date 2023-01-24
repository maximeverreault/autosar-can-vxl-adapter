cmake_minimum_required(VERSION 3.20)

project(autosar_can_vxl_adapter C CXX)

add_executable(${PROJECT_NAME} "")

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32)
target_compile_definitions(${PROJECT_NAME} PUBLIC _DEBUG)
target_compile_definitions(${PROJECT_NAME} PUBLIC _CONSOLE)
target_compile_definitions(${PROJECT_NAME} PUBLIC _MT)
target_compile_definitions(${PROJECT_NAME} PUBLIC _DEBUG_FUNCTIONAL_MACHINERY)

target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
target_compile_options(${PROJECT_NAME} PUBLIC -Wextra)
target_compile_options(${PROJECT_NAME} PUBLIC -Werror)
if(${CMAKE_CXX_COMPILER_ID} EQUAL CLANG)
    target_compile_options(${PROJECT_NAME} PUBLIC -fms-compatibility-version=19.10)
    target_compile_options(${PROJECT_NAME} PUBLIC -Wmicrosoft)
    target_compile_options(${PROJECT_NAME} PUBLIC -Wno-invalid-token-paste)
endif()
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-unknown-pragmas)
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-unused-value)
target_compile_options(${PROJECT_NAME} PUBLIC -Wshadow)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>)
target_compile_options(${PROJECT_NAME} PUBLIC -pedantic)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vxlapi)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vxlapi/vxlapi64.lib)

set(Boost_NO_WARN_NEW_VERSIONS 1)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(BOOST_ROOT        F:/libs)
#set(BOOST_LIBRARYDIR F:/libs/lib)

find_package(Boost 1.81.0 EXACT REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)
endif()

find_package(fmt REQUIRED)

if(fmt_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
endif()

add_subdirectory(src)

if(DEFINED STANDALONE)
    message("Building for standalone environment")
    add_subdirectory(stubs)
endif()
