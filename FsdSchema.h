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

struct FsdSchemaAttributes
{
	FsdSchemaAttributes();
	~FsdSchemaAttributes();

	SchemaType schemaType;	

	std::string schemaTypeAsString;
	
	bool isOptional;
	bool hasDefault;

	int size;
	bool hasSize;

	PyObject* defaultValue;
	std::shared_ptr<VectorSchemaAttributes> vectorAttributes;
	std::shared_ptr<ListSchemaAttributes> listAttributes;
	std::shared_ptr<DictSchemaAttributes> dictAttributes;
	std::shared_ptr<ObjectSchemaAttributes> objectAttributes;
};
TYPEDEF_BLUECLASS(FsdSchemaAttributes)

struct VectorSchemaAttributes
{
	bool hasAliases;
	std::map<std::string, uint8_t> aliases;
};

struct ListSchemaAttributes
{
	ListSchemaAttributes();
	~ListSchemaAttributes();

	std::shared_ptr<FsdSchemaAttributes> listItemSchema;
};

struct DictSchemaAttributes
{
	DictSchemaAttributes();
	~DictSchemaAttributes();

	bool multiIndex;
	bool buildIndex;
	std::string indexBy;

	std::shared_ptr<FsdSchemaAttributes> keySchema;
	std::shared_ptr<FsdSchemaAttributes> valueSchema;
};

struct ObjectSchemaAttributes
{
	ObjectSchemaAttributes();
	~ObjectSchemaAttributes();

	// Keep the attributeNames as strings for quicker fetching
	std::map<std::string, std::shared_ptr<FsdSchemaAttributes>> attributes;
	std::map<std::string, uint32_t> constantAttributeOffsets;
	uint32_t endOfFixedSizedData;

	std::vector<std::string> attributesWithVariableOffset;
	std::map<std::string, uint64_t> optionalValueLookups;
};

PyObject* GetDictValue(PyObject* dict, std::string key);
bool BlueExtractArgumentImpl(PyObject* argument, FsdSchemaAttributes &result, unsigned int argID, std::false_type isBlueType);

#endif // FsdSchema_h
