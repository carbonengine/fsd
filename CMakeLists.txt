cmake_minimum_required(VERSION 3.16)
project(pyFSD VERSION 1.0.0)

include(cmake/CcpTargetConfigurations.cmake)
include(cmake/CcpPackageConfigHelpers.cmake)
include(cmake/CcpBuildConfigurations.cmake)

find_package(carbon-blue REQUIRED CONFIG)

set(SRC_FILES
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
    FsdVector.cpp
    FsdVector.h
    SchemaTypes.cpp
    SchemaTypes.h
    StdAfx.cpp
    StdAfx.h
    pyFSD.cpp
)

ccp_add_library(pyFSD SHARED ${SRC_FILES})
target_link_libraries(pyFSD PRIVATE Blue)
target_include_directories(pyFSD PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(pyFSD PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)
set_target_properties(pyFSD PROPERTIES OUTPUT_NAME "_pyfsd")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(INSTALL_TO_MONOLITH
        "Install using the Carbon vendor folder layout for backwards compatibility"
        OFF
    )

    if(INSTALL_TO_MONOLITH)
        set(_LIB_DESTINATION "lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}")
        set(_BIN_DESTINATION "bin/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}")
    else()
        set(_LIB_DESTINATION lib)
        set(_BIN_DESTINATION bin)
    endif()

    install(
        TARGETS pyFSD
        EXPORT pyFSDTargets
        CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
        LIBRARY DESTINATION ${_LIB_DESTINATION}
        ARCHIVE DESTINATION ${_LIB_DESTINATION}
        RUNTIME DESTINATION ${_BIN_DESTINATION}
    )

    install(EXPORT pyFSDTargets
        DESTINATION share/carbon-fsd
        FILE carbon-fsdConfig.cmake
    )

    install(DIRECTORY python/
        DESTINATION python
        FILES_MATCHING
        PATTERN "*.py"
        PATTERN "package.yaml"
    )
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(BUILD_TESTING "Build and run tests" ON)
    include(CTest)

    if(BUILD_TESTING)
        find_package(carbon-exefile-interpreter REQUIRED CONFIG)
        find_package(Python3 COMPONENTS Interpreter REQUIRED)

        ccp_prepare_interpreter(
            TARGET pyFSD
            BUILD_TYPE ${CMAKE_BUILD_TYPE}
            COMMAND_OUT EXEFILE_INTERPRETER
            PYTHONPATH_OUT PYTHONPATH
            RUN_DIR $<TARGET_FILE_DIR:pyFSD>
            BLUE_DIR_OUT BLUE_LOCATION_DIR
        )

        if(WIN32)
            set(PYTHONPATH
                "${PYTHONPATH}\;$<TARGET_FILE_DIR:pyFSD>\;${CMAKE_CURRENT_SOURCE_DIR}/python"
            )
            set(TESTS_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        elseif(APPLE)
            set(PYTHONPATH
                "${PYTHONPATH}:$<TARGET_FILE_DIR:pyFSD>:${CMAKE_CURRENT_SOURCE_DIR}/tests:${CMAKE_CURRENT_SOURCE_DIR}/python"
            )
            set(TESTS_WORKING_DIRECTORY $<TARGET_FILE_DIR:pyFSD>)
        else()
            message(FATAL_ERROR "Unsupported platform")
        endif()

        set(ENV{PYTHONPATH} ${CMAKE_CURRENT_SOURCE_DIR}/python)
        execute_process(
            COMMAND ${Python3_EXECUTABLE} discover.py
            OUTPUT_VARIABLE PYTHON_TESTS_STR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE PYTHON_TEST_DISCOVERY_RETVAL
            ERROR_VARIABLE PYTHON_TEST_DISCOVERY_ERRORS
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
        )

        if(PYTHON_TEST_DISCOVERY_RETVAL)
            message(FATAL_ERROR "Error encountered during test discovery:\n=== stderr: ===\n${PYTHON_TEST_DISCOVERY_ERRORS}\n=== stdout: ===\n${PYTHON_TESTS_STR}")
        endif()

        separate_arguments(PYTHON_TESTS UNIX_COMMAND ${PYTHON_TESTS_STR})
        foreach(PYTHON_TEST ${PYTHON_TESTS})
            message(STATUS "Found test ${PYTHON_TEST}")
            add_test(
                NAME ${PYTHON_TEST}
                COMMAND ${EXEFILE_INTERPRETER}
                    /inherit
                    /buildflavor=$<LOWER_CASE:$<CONFIG>>
                    /py
                    -m
                    unittest
                    -v
                    ${PYTHON_TEST}
                WORKING_DIRECTORY ${TESTS_WORKING_DIRECTORY}
                COMMAND_EXPAND_LISTS
            )
            set_tests_properties(${PYTHON_TEST} PROPERTIES
                ENVIRONMENT "PYTHONPATH=${PYTHONPATH};BUILDFLAVOR=$<LOWER_CASE:$<CONFIG>>"
            )
        endforeach()
    endif()
endif()
