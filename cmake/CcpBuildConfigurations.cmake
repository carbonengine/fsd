# Copyright © 2025 CCP ehf.

set(CMAKE_CONFIGURATION_TYPES Debug TrinityDev Internal Release)

get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(is_multi_config)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
        CACHE STRING "Reset the configurations to what we need" FORCE
    )
else()
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "No CMAKE_BUILD_TYPE was specified, defaulting to 'Debug'")
        set(CMAKE_BUILD_TYPE "Debug")
    endif()

    set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}"
        CACHE STRING "Choose the type of build, options are: ${CMAKE_CONFIGURATION_TYPES}."
        FORCE
    )

    if(CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Building configuration ${CMAKE_BUILD_TYPE}")
    else()
        message(FATAL_ERROR "Invalid configuration ${CMAKE_BUILD_TYPE}. Available options are: ${CMAKE_CONFIGURATION_TYPES}")
    endif()
endif()

function(create_new_build_config config prototype)
    string(TOUPPER ${config} CONFIG)
    string(TOUPPER ${prototype} PROTOTYPE)

    set(CMAKE_CXX_FLAGS_${CONFIG} ${CMAKE_CXX_FLAGS_${PROTOTYPE}} CACHE STRING
        "Flags used by the C++ compiler during ${config} builds." FORCE)
    set(CMAKE_C_FLAGS_${CONFIG} ${CMAKE_C_FLAGS_${PROTOTYPE}} CACHE STRING
        "Flags used by the C compiler during ${config} builds." FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_${CONFIG} ${CMAKE_EXE_LINKER_FLAGS_${PROTOTYPE}} CACHE STRING
        "Flags used for linking binaries during ${config} builds." FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS_${CONFIG} ${CMAKE_MODULE_LINKER_FLAGS_${PROTOTYPE}} CACHE STRING
        "Flags used for linking modules during ${config} builds." FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_${CONFIG} ${CMAKE_SHARED_LINKER_FLAGS_${PROTOTYPE}} CACHE STRING
        "Flags used by the shared libraries linker during ${config} builds." FORCE)
endfunction()

create_new_build_config(Internal Release)
create_new_build_config(TrinityDev Internal)

if(MSVC)
    add_compile_options($<IF:$<OR:$<CONFIG:Release>,$<CONFIG:Internal>>,/Zi,>)
    add_link_options($<IF:$<CONFIG:Release>,/DEBUG:FULL,/DEBUG:FASTLINK>)

    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_TRINITYDEV "")
    set(CMAKE_C_FLAGS_TRINITYDEV "")
    set(CMAKE_SHARED_LINKER_FLAGS_TRINITYDEV "")
    set(CMAKE_EXE_LINKER_FLAGS_TRINITYDEV "")

    add_compile_options($<IF:$<CONFIG:Debug>,/ZI,>)
    add_compile_options($<IF:$<CONFIG:Debug>,/Od,>)
    add_compile_options($<IF:$<CONFIG:TrinityDev>,/Zi,>)
    add_compile_options($<IF:$<CONFIG:TrinityDev>,/O2,>)
endif()
