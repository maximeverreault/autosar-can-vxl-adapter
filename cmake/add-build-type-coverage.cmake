macro(add_gcc_coverage_build_type)
    set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -g --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fprofile-abs-path" CACHE STRING
            "Flags used by the C++ compiler during coverage builds."
            FORCE)
    set(CMAKE_C_FLAGS_COVERAGE "-O0 -g --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fprofile-abs-path" CACHE STRING
            "Flags used by the C compiler during coverage builds."
            FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
            "--coverage -fprofile-abs-path" CACHE STRING
            "Flags used for linking binaries during coverage builds."
            FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
            "--coverage -fprofile-abs-path" CACHE STRING
            "Flags used by the shared libraries linker during coverage builds."
            FORCE)
    mark_as_advanced(
            CMAKE_CXX_FLAGS_COVERAGE
            CMAKE_C_FLAGS_COVERAGE
            CMAKE_EXE_LINKER_FLAGS_COVERAGE
            CMAKE_SHARED_LINKER_FLAGS_COVERAGE)
    # Update the documentation string of CMAKE_BUILD_TYPE for GUIs
    set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage."
            FORCE)
endmacro()