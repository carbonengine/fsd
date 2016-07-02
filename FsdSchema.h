////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef FsdSchema_h
#define FsdSchema_h

#include "StdAfx.h"

// forward declarations 
struct BoolSchemaAttributes;
struct IntSchemaAttributes;
struct FloatSchemaAttributes;
struct StringSchemaAttributes;
struct VectorSchemaAttributes;
struct ListSchemaAttributes;
struct DictSchemaAttributes;
struct ObjectSchemaAttributes;

enum AttributeType
{
	UNKNOWN_ATTRIBUTE_TYPE = 0,
	REQUIRED_FIXED_SIZE_ATTRIBUTE = 1,
	OPTIONAL_ATTRIBUTE = 2,
	VARIABLE_SIZE_REQUIRED_ATTRIBUTE = 3,
	ATTRIBUTE_TYPE_COUNT
};


struct FsdSchemaAttributes
{
	FsdSchemaAttributes();
	~FsdSchemaAttributes();

	SchemaType schemaType;
	AttributeType attrType;	

	std::string schemaTypeAsString;
	
	bool isOptional;
	bool hasDefault;

	int size;
	bool hasSize;

	BoolSchemaAttributes *boolAttributes;
	IntSchemaAttributes *intAttributes;
	FloatSchemaAttributes *floatAttributes;
	StringSchemaAttributes *stringAttributes;
	VectorSchemaAttributes *vectorAttributes;
	ListSchemaAttributes *listAttributes;
	DictSchemaAttributes *dictAttributes;
	ObjectSchemaAttributes *objectAttributes;
};
TYPEDEF_BLUECLASS(FsdSchemaAttributes)


struct BoolSchemaAttributes
{
	bool defaultValue;
};

struct IntSchemaAttributes
{
	int defaultValue;
};

struct FloatSchemaAttributes
{
	float floatDefaultValue;
	double doubleDefaultValue;
};

struct StringSchemaAttributes
{
	std::string defaultValue;
};

struct VectorSchemaAttributes
{
	bool hasAliases;
	std::map<std::string, uint8_t> aliases;
};

struct ListSchemaAttributes
{
	ListSchemaAttributes();
	//~ListSchemaAttributes();

	FsdSchemaAttributes *listItemSchema;
};

struct DictSchemaAttributes
{
	DictSchemaAttributes();
	//~DictSchemaAttributes();

	bool multiIndex;
	bool buildIndex;
	std::string indexBy;

	FsdSchemaAttributes *keySchema;
	FsdSchemaAttributes *valueSchema;
};

struct ObjectSchemaAttributes
{
	ObjectSchemaAttributes();
	//~ObjectSchemaAttributes();

	// Keep the attributeNames as strings for quicker fetching
	std::map<std::string, FsdSchemaAttributes*> attributes;
	std::map<std::string, uint32_t> constantAttributeOffsets;
	uint32_t endOfFixedSizedData;

	std::vector<std::string> attributesWithVariableOffset;
	std::map<std::string, uint64_t> optionalValueLookups;
};

PyObject* GetDictValue(PyObject* dict, std::string key);
bool BlueExtractArgumentImpl(PyObject* argument, FsdSchemaAttributes &result, unsigned int argID, std::false_type isBlueType);

#endif // FsdSchema_h
