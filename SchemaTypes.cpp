////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//
#include "StdAfx.h"
#include "SchemaTypes.h"

Be::VarChooser SchemaTypeEnumChooser[] =
{
	{
		"UNKNOWN",
		{ BeCast(UNKNOWN_SCHEMA_TYPE) },
		"Currently unsupported type"
	},
	{
		"UNSIGNED_32_BIT_INT",
		{ BeCast(UNSIGNED_32_BIT_INT_SCHEMA_TYPE) },
		"UNSIGNED_32_BIT_INT"
	},
	{
		"SIGNED_32_BIT_INT",
		{ BeCast(SIGNED_32_BIT_INT_SCHEMA_TYPE) },
		"SIGNED_32_BIT_INT"
	},
	{
		"FLOAT32",
		{ BeCast(FLOAT32_SCHEMA_TYPE) },
		"FLOAT32"
	},
	{
		"STRING",
		{ BeCast(STRING_SCHEMA_TYPE) },
		"STRING"
	},
	{
		"UTF8",
		{ BeCast(UTF8_SCHEMA_TYPE) },
		"UTF8"
	},
	{
		"BOOL",
		{ BeCast(BOOL_SCHEMA_TYPE) },
		"BOOL"
	},
	{
		"FLOAT32_VECTOR2_TUPLE",
		{ BeCast(FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE) },
		"FLOAT32_VECTOR2_TUPLE"
	},
	{
		"FLOAT32_VECTOR3_TUPLE",
		{ BeCast(FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE) },
		"FLOAT32_VECTOR3_TUPLE"
	},
	{
		"FLOAT32_VECTOR4_TUPLE",
		{ BeCast(FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE) },
		"FLOAT32_VECTOR4_TUPLE"
	},
	{
		"DOUBLE_VECTOR2_TUPLE",
		{ BeCast(DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE) },
		"DOUBLE_VECTOR2_TUPLE"
	},
	{
		"DOUBLE_VECTOR3_TUPLE",
		{ BeCast(DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE) },
		"DOUBLE_VECTOR3_TUPLE"
	},
	{
		"DOUBLE_VECTOR4_TUPLE",
		{ BeCast(DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE) },
		"DOUBLE_VECTOR4_TUPLE"
	},
	{
		"OBJECT",
		{ BeCast(OBJECT_SCHEMA_TYPE) },
		"OBJECT"
	},
	{
		"UNION",
		{ BeCast(UNION_SCHEMA_TYPE) },
		"UNION"
	},
	{ 0 }
};

BLUE_REGISTER_ENUM_EX(
	"SCHEMA_TYPES",
	SchemaType,
	SchemaTypeEnumChooser,
	ENUM_REG_ENUM_OBJECT_ON_MODULE
);



const char* PY_SCHEMA_CONSTANTS::TYPE_BOOL = "bool";
const char* PY_SCHEMA_CONSTANTS::TYPE_INT = "int";
const char* PY_SCHEMA_CONSTANTS::TYPE_TYPEID = "typeID";
const char* PY_SCHEMA_CONSTANTS::TYPE_FLOAT = "float";
const char* PY_SCHEMA_CONSTANTS::TYPE_STRING = "string";
const char* PY_SCHEMA_CONSTANTS::TYPE_UNICODE = "unicode";
const char* PY_SCHEMA_CONSTANTS::TYPE_RESPATH = "respath";
const char* PY_SCHEMA_CONSTANTS::TYPE_VECTOR2 = "vector2";
const char* PY_SCHEMA_CONSTANTS::TYPE_VECTOR3 = "vector3";
const char* PY_SCHEMA_CONSTANTS::TYPE_VECTOR4 = "vector4";
const char* PY_SCHEMA_CONSTANTS::TYPE_LIST = "list";
const char* PY_SCHEMA_CONSTANTS::TYPE_DICT = "dict";
const char* PY_SCHEMA_CONSTANTS::TYPE_OBJECT = "object";
const char* PY_SCHEMA_CONSTANTS::TYPE_UNION = "union";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_TYPE = "type";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_SIZE = "size";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_MIN = "min";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_EXCLUSIVE_MIN = "exclusiveMin";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT = "default";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_PRECISION = "precision";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_ALIASES = "aliases";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_ITEMTYPES = "itemTypes";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_KEYTYPES = "keyTypes";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_VALUETYPES = "valueTypes";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_INDEXBY = "indexBy";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_MULTIINDEX = "multiIndex";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_BUILDINDEX = "buildIndex";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_ATTRIBUTES = "attributes";
const char* PY_SCHEMA_CONSTANTS::ATTRIBUTE_ISOPTIONAL = "isOptional";
const char* PY_SCHEMA_CONSTANTS::PRECISION_FLOAT = "float";
const char* PY_SCHEMA_CONSTANTS::PRECISION_DOUBLE = "double";
const char* PY_SCHEMA_CONSTANTS::OBJECT_END_OF_FIXED_SIZED_DATA = "endOfFixedSizeData";
const char* PY_SCHEMA_CONSTANTS::OBJECT_OPTIONAL_VALUE_LOOKUP= "optionalValueLookups";
const char* PY_SCHEMA_CONSTANTS::OBJECT_ATTRIBUTES_WITH_VARIABLE_OFFSET = "attributesWithVariableOffsets";
const char* PY_SCHEMA_CONSTANTS::OBJECT_MAX_BIT_FIELD_VALUE = "maxBitFieldValue";
const char* PY_SCHEMA_CONSTANTS::OBJECT_CONSTANT_ATTRIBUTE_OFFSETS = "constantAttributeOffsets";


