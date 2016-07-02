////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef SchemaTypes_h
#define SchemaTypes_h

// This enumeration defines all the schema types
// unlike the python version, it it slightly more specific -
// this version defines a difference between signed and unsigned integers, for example
enum SchemaType {
	// used for uninitialized values
	UNKNOWN_SCHEMA_TYPE,

	BOOL_SCHEMA_TYPE,

	UNSIGNED_32_BIT_INT_SCHEMA_TYPE,
	SIGNED_32_BIT_INT_SCHEMA_TYPE,

	FLOAT32_SCHEMA_TYPE,
	DOUBLE_SCHEMA_TYPE,

	STRING_SCHEMA_TYPE,
	UTF8_SCHEMA_TYPE,

	FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE,
	FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE,
	FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE,

	DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE,
	DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE,
	DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE,

	OBJECT_SCHEMA_TYPE,
	UNION_SCHEMA_TYPE,

	SCHEMA_TYPES_COUNT
};

class PY_SCHEMA_CONSTANTS{
public:
	static const char* TYPE_BOOL;
	static const char* TYPE_INT;
	static const char* TYPE_TYPEID;
	static const char* TYPE_FLOAT;
	static const char* TYPE_STRING;
	static const char* TYPE_UNICODE;
	static const char* TYPE_RESPATH;
	static const char* TYPE_VECTOR2;
	static const char* TYPE_VECTOR3;
	static const char* TYPE_VECTOR4;
	static const char* TYPE_LIST;
	static const char* TYPE_DICT;
	static const char* TYPE_OBJECT;
	static const char* TYPE_UNION;
	static const char* ATTRIBUTE_TYPE;
	static const char* ATTRIBUTE_SIZE;
	static const char* ATTRIBUTE_MIN;
	static const char* ATTRIBUTE_EXCLUSIVE_MIN;
	static const char* ATTRIBUTE_DEFAULT;
	static const char* ATTRIBUTE_PRECISION;
	static const char* ATTRIBUTE_ALIASES;
	static const char* ATTRIBUTE_ITEMTYPES;
	static const char* ATTRIBUTE_KEYTYPES;
	static const char* ATTRIBUTE_VALUETYPES;
	static const char* ATTRIBUTE_INDEXBY;
	static const char* ATTRIBUTE_MULTIINDEX;
	static const char* ATTRIBUTE_BUILDINDEX;
	static const char* ATTRIBUTE_ATTRIBUTES;
	static const char* ATTRIBUTE_ISOPTIONAL;
	static const char* PRECISION_FLOAT;
	static const char* PRECISION_DOUBLE;
	static const char* OBJECT_END_OF_FIXED_SIZED_DATA;
	static const char* OBJECT_OPTIONAL_VALUE_LOOKUP;
	static const char* OBJECT_ATTRIBUTES_WITH_VARIABLE_OFFSET;
	static const char* OBJECT_MAX_BIT_FIELD_VALUE;
	static const char* OBJECT_CONSTANT_ATTRIBUTE_OFFSETS;

};



#endif