cmake_minimum_required(VERSION 3.20)

project(autosar-can-vxl-adapter C CXX)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(cmake/add-build-type-coverage.cmake)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    add_gcc_coverage_build_type()
endif()


add_library(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_compile_definitions(${PROJECT_NAME} PUBLIC DISABLE_MCAL_FILE_VERSION_CHECK)

target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
target_compile_options(${PROJECT_NAME} PUBLIC -Wextra)
target_compile_options(${PROJECT_NAME} PUBLIC -Werror)
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-unknown-pragmas)
target_compile_options(${PROJECT_NAME} PUBLIC -Wno-unused-value)
target_compile_options(${PROJECT_NAME} PUBLIC -Wshadow)
target_compile_options(${PROJECT_NAME} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>)
target_compile_options(${PROJECT_NAME} PUBLIC -pedantic)

add_subdirectory(vxlapi)
target_link_libraries(${PROJECT_NAME} PRIVATE vxlapi)

find_package(fmt REQUIRED)

if(fmt_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
endif()

add_subdirectory(src)

if(DEFINED STANDALONE)
    message("Building for standalone environment")

    set(Boost_NO_WARN_NEW_VERSIONS 1)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME OFF)

    add_executable(${PROJECT_NAME}_main src/main.cpp)

    set_target_properties(${PROJECT_NAME}_main PROPERTIES CXX_STANDARD 20)

    find_package(Boost 1.81.0 EXACT REQUIRED COMPONENTS program_options)

    if(Boost_FOUND)
        target_link_libraries(${PROJECT_NAME}_main PRIVATE Boost::program_options)
    endif()

    if(fmt_FOUND)
        target_link_libraries(${PROJECT_NAME}_main PRIVATE fmt::fmt)
    endif()

    target_link_libraries(${PROJECT_NAME}_main PUBLIC ${PROJECT_NAME} vxlapi)
    add_subdirectory(stubs)

    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(${PROJECT_NAME}_Tests)
    set_target_properties(${PROJECT_NAME}_Tests PROPERTIES CXX_STANDARD 20)
    target_link_libraries(
            ${PROJECT_NAME}_Tests
            GTest::gtest_main
            GTest::gmock_main
            ${PROJECT_NAME}
            vxlapi
    )
    add_subdirectory(tests)
    add_subdirectory(mocks)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_Tests XML_OUTPUT_DIR report)
endif()
