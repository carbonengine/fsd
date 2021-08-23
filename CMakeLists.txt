project(pyFSD VERSION 1.0.0)

add_library(pyFSD SHARED)
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
target_include_directories(pyFSD PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../../carbon/src)
if(APPLE)
    set_target_properties(
        pyFSD
        PROPERTIES 
            PREFIX ""
            SUFFIX ".so"
    )
endif()

set_target_properties(pyFSD PROPERTIES OUTPUT_NAME "_pyfsd")
