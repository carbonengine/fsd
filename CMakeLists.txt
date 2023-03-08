cmake_minimum_required(VERSION 3.16)
project(pyFSD VERSION 1.0.0)

if(NOT DEFINED ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "Missing required environment variable CCP_EVE_PERFORCE_BRANCH_PATH")
elseif(NOT IS_ABSOLUTE $ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "CCP_EVE_PERFORCE_BRANCH_PATH needs to be an absolute path")
elseif(NOT EXISTS $ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "CCP_EVE_PERFORCE_BRANCH_PATH is pointing at a non-existing location $ENV{CCP_EVE_PERFORCE_BRANCH_PATH}")
elseif(NOT IS_DIRECTORY $ENV{CCP_EVE_PERFORCE_BRANCH_PATH})
    message(FATAL_ERROR "CCP_EVE_PERFORCE_BRANCH_PATH needs to be a directory")
else()
    message(STATUS "Using CCP_EVE_PERFORCE_BRANCH_PATH located at $ENV{CCP_EVE_PERFORCE_BRANCH_PATH}")
endif()

file(TO_CMAKE_PATH "$ENV{CCP_EVE_PERFORCE_BRANCH_PATH}" BRANCH_ROOT_DIR)
list(INSERT CMAKE_MODULE_PATH 0 ${BRANCH_ROOT_DIR}/cmake)

include(CcpGlobalSettings)
include(CcpPackageConfigHelpers)

find_package(Blue CONFIG REQUIRED NO_CMAKE_PATH)

ccp_add_library(pyFSD SHARED)
target_link_libraries(pyFSD PRIVATE Blue)

target_sources(pyFSD PRIVATE
    BinaryLoader.cpp
    BinaryLoader.h
    BinaryLoaderHelper.h
    BinaryLoader_Blue.cpp
    BoolBeResult.h
    FsdDict.cpp
    FsdDict.h
    FsdDictOffsetLookup.cpp
    FsdDictOffsetLookup.h
    FsdList.cpp
    FsdList.h
    FsdObject.cpp
    FsdObject.h
    FsdObject_Blue.cpp
    FsdSchema.cpp
    FsdSchema.h
    FsdSchema_Blue.cpp
    FsdUnsignedIntegerKeyMap.cpp
    FsdUnsignedIntegerKeyMap.h
    FsdUnsignedIntegerKeyMap_Blue.cpp
    FsdVector.cpp
    FsdVector.h
    SchemaTypes.cpp
    SchemaTypes.h
    StdAfx.cpp
    StdAfx.h
    pyFSD.cpp
)
get_target_property(_SOURCES pyFSD SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
        PREFIX "Sources"
        FILES ${_SOURCES}
)

target_precompile_headers(pyFSD PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)

set_target_properties(pyFSD PROPERTIES OUTPUT_NAME "_pyfsd")

set(PYTHON_EXECUTABLE "${BRANCH_ROOT_DIR}/vendor/python/2.7.1+ccp-stackless/bin/Windows/x64/v141/python.exe")
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; sys.modules['test_pyFSD']=sys;import unittest; print_suite = lambda x: print(x.id()) if not hasattr(x, '__iter__') else [print_suite(x) for x in x];print_suite(unittest.defaultTestLoader.discover('.'))"
        OUTPUT_VARIABLE PYTHON_TESTS_STR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
        )
separate_arguments(PYTHON_TESTS UNIX_COMMAND ${PYTHON_TESTS_STR})
foreach(PYTHON_TEST ${PYTHON_TESTS})
    add_test(NAME ${PYTHON_TEST}
            COMMAND ${PYTHON_EXECUTABLE} -m unittest -v ${PYTHON_TEST}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
            )
    set_tests_properties(${PYTHON_TEST} PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}")
endforeach()

# Provide an install target if this is the top level project only
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Install rule to ensure that our runtime and linker files are in the expected, platform-specific folders
    install(
            TARGETS pyFSD
            EXPORT pyFSDTargets
            CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
            LIBRARY DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
            ARCHIVE DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
            RUNTIME DESTINATION bin/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
    )
    # Install rule for available public headers
    install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include)

    configure_ccp_vendor_config_file(
            TARGET pyFSD
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/pyFSDConfig.cmake"
            DEPENDENCIES ""
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyFSDConfig.cmake DESTINATION .)

    # Install python package
    install(DIRECTORY python/ DESTINATION python
            FILES_MATCHING
            PATTERN "*.py"
            PATTERN "package.yaml"
            )
endif()
