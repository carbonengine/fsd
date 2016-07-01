////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "FsdSchema.h"

// forward declarations
bool CreateSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID);

ObjectSchemaAttributes::ObjectSchemaAttributes() :
endOfFixedSizedData(0)
{}	

/*ObjectSchemaAttributes::~ObjectSchemaAttributes()
{
	for (auto attr = attributes.begin(); attr != attributes.end(); attr++)
	{
		delete attr->second;
	}
	attributes.clear();
}*/

DictSchemaAttributes::DictSchemaAttributes():
	keySchema(nullptr),
	valueSchema(nullptr),
	multiIndex(false),
	buildIndex(false),
	indexBy("")
{}
/*
DictSchemaAttributes::~DictSchemaAttributes()
{
	delete keySchema;
	delete valueSchema;
}
*/

ListSchemaAttributes::ListSchemaAttributes() : 
	listItemSchema(nullptr)
{}
/*
ListSchemaAttributes::~ListSchemaAttributes()
{
	delete listItemSchema;	
}*/

FsdSchemaAttributes::FsdSchemaAttributes() :
	boolAttributes(nullptr),
	intAttributes(nullptr),
	floatAttributes(nullptr),
	stringAttributes(nullptr),
	vectorAttributes(nullptr),
	objectAttributes(nullptr),
	listAttributes(nullptr),
	dictAttributes(nullptr),
	schemaType(UNKNOWN_SCHEMA_TYPE),
	attrType(UNKNOWN_ATTRIBUTE_TYPE),
	isOptional(false),
	hasDefault(false),
	hasSize(false),
	size(0)
{}

FsdSchemaAttributes::~FsdSchemaAttributes()
{
	delete boolAttributes;
	delete intAttributes;
	delete floatAttributes;
	delete stringAttributes;
	delete vectorAttributes;
	delete objectAttributes;
	delete dictAttributes;
}

bool GetRequiredStringDictValue(PyObject* dict, const std::string dictKey, unsigned int argID, std::string &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());

	bool success = BlueExtractString(item, out);
	if (!success)
	{
		std::string msg = "Schema has no '" + dictKey + "' key";
		PyErr_Format(PyExc_TypeError,msg.c_str() , argID);
		return false;
	}
	return true;
}

bool GetRequiredIntDictValue(PyObject* dict, const std::string dictKey, unsigned int argID, int &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());

	bool success = BlueExtractInt(item, out);
	if (!success)
	{
		std::string msg = "Schema has no '" + dictKey + "' key";
		PyErr_Format(PyExc_TypeError, msg.c_str(), argID);
		return false;
	}
	return true;
}

bool GetOptionalStringDictValue(PyObject* dict, const std::string dictKey, unsigned int argID, std::string &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());
	if (item)
	{
		bool success = BlueExtractString(item, out);
		
		if (!success)
		{
			std::string msg = "Schema key '" + dictKey + "' is not a string";
			PyErr_Format(PyExc_TypeError, msg.c_str(), argID);
			return false;
		}
	}
	return true;
}

bool GetOptionalIntDictValue(PyObject* dict, const std::string dictKey, unsigned int argID, int &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());
	if (item)
	{
		bool success = BlueExtractInt(item, out);
		
		if (!success)
		{
			std::string msg = "Schema key '" + dictKey + "' is not a int";
			PyErr_Format(PyExc_TypeError, msg.c_str(), argID);
			return false;
		}
	}
	return true;
}

bool GetOptionalBoolDictValue(PyObject* dict, const std::string dictKey, unsigned int argID,  bool &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());
	if (item)
	{
		bool success = BlueExtractBool(item, out);
		
		if (!success)
		{
			std::string msg = "Schema key '" + dictKey + "' is not a bool";
			PyErr_Format(PyExc_TypeError, msg.c_str(), argID);
			return false;
		}
	}
	return true;
}

bool GetOptionalFloatDictValue(PyObject* dict, const std::string dictKey, unsigned int argID, float &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());
	if (item)
	{
		bool success = BlueExtractFloat(item, out);
		if (!success)
		{
			std::string msg = "Schema key '" + dictKey + "' is not a float";
			PyErr_Format(PyExc_TypeError, msg.c_str(), argID);
			return false;
		}
	}
	return true;
}

bool GetOptionalDoubleDictValue(PyObject* dict, const std::string dictKey, unsigned int argID, double &out)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());
	if (item)
	{
		bool success = BlueExtractDouble(item, out);
		if (!success)
		{
			std::string msg = "Schema key '" + dictKey + "' is not a double";
			PyErr_Format(PyExc_TypeError, msg.c_str(), argID);
			return false;
		}
	}

	return true;
}

bool HasOptionalValue(PyObject* dict, const std::string dictKey)
{
	PyObject* item = PyDict_GetItemString(dict, dictKey.c_str());
	
	return item != nullptr;
}

bool CreateBoolSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	result.schemaType = BOOL_SCHEMA_TYPE;
	if (result.hasDefault)
	{
		result.boolAttributes = new BoolSchemaAttributes();
		if (!GetOptionalBoolDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT, argID, result.boolAttributes->defaultValue))
		{
			return false;
		}
	}

	return true;
}

bool CreateIntSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	CCP_LOGNOTICE("Creating int schema");
	int min = -100;
	int exclusiveMin = -100;
	if (!GetOptionalIntDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_MIN, argID, min))
	{
		return false;
	}
	if (!GetOptionalIntDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_EXCLUSIVE_MIN, argID, exclusiveMin))
	{
		return false;
	}
	if (result.hasDefault)
	{
		result.intAttributes = new IntSchemaAttributes();
		if (!GetOptionalIntDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT, argID, result.intAttributes->defaultValue))
		{
			return false;
		}
	}

	SchemaType type = SIGNED_32_BIT_INT_SCHEMA_TYPE;

	if (min >= 0 || exclusiveMin >= -1)
	{
		CCP_LOGNOTICE("Int schema is unsigned");
		type = UNSIGNED_32_BIT_INT_SCHEMA_TYPE;
	}

	result.schemaType = type;
	return true;
}

bool CreateFloatSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	std::string precision = PY_SCHEMA_CONSTANTS::PRECISION_FLOAT;

	if (!GetOptionalStringDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_PRECISION, argID, precision))
	{
		return false;
	}
	
	SchemaType type = FLOAT32_SCHEMA_TYPE;

	result.floatAttributes = new FloatSchemaAttributes();
	if (precision == PY_SCHEMA_CONSTANTS::PRECISION_FLOAT)
	{		
		if (!GetOptionalFloatDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT, argID, result.floatAttributes->floatDefaultValue))
		{
			return false;
		}
	}
	else
	{
		if (!GetOptionalDoubleDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT, argID, result.floatAttributes->doubleDefaultValue))
		{
			return false;
		}
		type = DOUBLE_SCHEMA_TYPE;
	}
	
	result.schemaType = type;
	return true;
}

bool CreateStringSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	if (result.hasDefault)
	{
		result.stringAttributes = new StringSchemaAttributes();
		if (!GetOptionalStringDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT, argID, result.stringAttributes->defaultValue))
		{
			return false;
		}
	}

	result.schemaType = STRING_SCHEMA_TYPE;
	return true;
}

bool CreateUnicodeSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	if (result.hasDefault)
	{
		result.stringAttributes = new StringSchemaAttributes();
		if (!GetOptionalStringDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT, argID, result.stringAttributes->defaultValue))
		{
			return false;
		}
	}

	result.schemaType = UTF8_SCHEMA_TYPE;
	return true;
}

bool CreateVectorSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	std::string precision = PY_SCHEMA_CONSTANTS::PRECISION_FLOAT;

	if (!GetOptionalStringDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_PRECISION, argID, precision))
	{
		return false;
	}

	int vectorSize = std::strtol(&result.schemaTypeAsString[result.schemaTypeAsString.length() - 1], nullptr, 10);
	SchemaType type;
	if (precision == PY_SCHEMA_CONSTANTS::PRECISION_FLOAT)
	{
		switch (vectorSize)
		{
		case 2:
			type = FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE;
			break;
		case 3:
			type = FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE;
			break;
		case 4:
			type = FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE;
			break;
		}
	}
	else{
		switch (vectorSize)
		{
		case 2:
			type = DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE;
			break;
		case 3:
			type = DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE;
			break;
		case 4:
			type = DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE;
			break;
		}
	}
	result.schemaType = type;
	result.vectorAttributes = new VectorSchemaAttributes();
	result.vectorAttributes->hasAliases = false;
	if (HasOptionalValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_ALIASES))
	{
		result.vectorAttributes->hasAliases = true;

		PyObject *key, *value;
		Py_ssize_t pos = 0;

		while (PyDict_Next(PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_ALIASES), &pos, &key, &value)) {
			std::string alias;
			int index;

			BlueExtractString(key, alias);
			BlueExtractInt(value, index);
			result.vectorAttributes->aliases[alias] = (uint8_t)index;
		}
	}

	return true;
}

bool CreateListSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{

	PyObject* itemTypes = PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_ITEMTYPES);
	result.listAttributes = new ListSchemaAttributes();
	CreateSchema(itemTypes, *result.listAttributes->listItemSchema, argID);
	return true;
}

bool CreateDictSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	PyObject* keyTypes = PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_KEYTYPES);
	PyObject* valueTypes = PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_VALUETYPES);

	result.dictAttributes= new DictSchemaAttributes();
	CreateSchema(keyTypes, *result.dictAttributes->keySchema, argID);
	CreateSchema(valueTypes, *result.dictAttributes->valueSchema, argID);

	if (!GetOptionalBoolDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_BUILDINDEX, argID, result.dictAttributes->buildIndex))
	{
		return false;
	}
	if (!GetOptionalBoolDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_MULTIINDEX, argID, result.dictAttributes->multiIndex))
	{
		return false;
	}
	if (!GetOptionalBoolDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_BUILDINDEX, argID, result.dictAttributes->buildIndex))
	{
		return false;
	}

	return true;
}

bool CreateObjectSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{	

	result.schemaType = OBJECT_SCHEMA_TYPE;
	result.objectAttributes = new ObjectSchemaAttributes();
	result.objectAttributes->attributes = std::map<std::string, FsdSchemaAttributes*>();
	result.objectAttributes->attributesWithVariableOffset = std::vector<std::string>();
	result.objectAttributes->optionalValueLookups = std::map<std::string, uint64_t>();
	result.objectAttributes->constantAttributeOffsets = std::map<std::string, uint32_t>();
		
	PyObject *key, *value;
	Py_ssize_t pos = 0;

	while (PyDict_Next(PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_ATTRIBUTES), &pos, &key, &value)) {
		std::string attributeName;
		BlueExtractString(key, attributeName);
		FsdSchemaAttributes* attributeSchema = new FsdSchemaAttributes();

		CreateSchema(value, *attributeSchema, argID);
		result.objectAttributes->attributes[attributeName] = attributeSchema;
	}

	pos = 0;

	while (PyDict_Next(PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::OBJECT_CONSTANT_ATTRIBUTE_OFFSETS), &pos, &key, &value)) {
		std::string attributeName;
		int offset;

		BlueExtractString(key, attributeName);
		BlueExtractInt(value, offset);
		result.objectAttributes->constantAttributeOffsets[attributeName] = (uint32_t)(offset);
	}

	// If we have a size, we don't have to worry about the optional attributes, variable offset attributes and the end of fixed sized data
	if (result.hasSize){
		return true;
	}

	if (HasOptionalValue(pySchema, PY_SCHEMA_CONSTANTS::OBJECT_END_OF_FIXED_SIZED_DATA))
	{
		int endOfFixedSizeData;
		if (!GetRequiredIntDictValue(pySchema, PY_SCHEMA_CONSTANTS::OBJECT_END_OF_FIXED_SIZED_DATA, argID, endOfFixedSizeData))
		{
			return false;
		}
		result.objectAttributes->endOfFixedSizedData = (uint32_t)(endOfFixedSizeData);
	}

	PyObject* attributesWithVariableOffset = PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::OBJECT_ATTRIBUTES_WITH_VARIABLE_OFFSET);
	Py_ssize_t variableOffsetSize = PyList_Size(attributesWithVariableOffset);
	
	for (Py_ssize_t i = 0; i < variableOffsetSize; i++)
	{
		std::string attributeName;

		PyObject* attributeNameItem = PyList_GetItem(attributesWithVariableOffset, i);
		BlueExtractString(attributeNameItem, attributeName);
		result.objectAttributes->attributesWithVariableOffset.push_back(attributeName);
	}

	pos = 0;

	while (PyDict_Next(PyDict_GetItemString(pySchema, PY_SCHEMA_CONSTANTS::OBJECT_OPTIONAL_VALUE_LOOKUP), &pos, &key, &value)) {
		std::string attributeName;
		int lookupIndex;

		BlueExtractString(key, attributeName);
		BlueExtractInt(value, lookupIndex);
		result.objectAttributes->optionalValueLookups[attributeName] = (uint64_t)(lookupIndex);
	}

	return true;
}

bool IsSchemaType(const std::string schemaType, const char* expectedSchemaType)
{
	return strcmp(schemaType.c_str(), expectedSchemaType) == 0;
}

bool IsIntSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_INT) || IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_TYPEID);
}

bool IsBoolSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_BOOL);
}

bool IsFloatSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_FLOAT);
}

bool IsStringSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_STRING) || IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_RESPATH);
}

bool IsUnicodeSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_UNICODE);
}

bool IsVectorSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_VECTOR2) || IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_VECTOR3) || IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_VECTOR4);
}

bool IsListSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_LIST);
}

bool IsDictSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_DICT);
}

bool IsObjectSchema(const std::string schemaType)
{
	return IsSchemaType(schemaType, PY_SCHEMA_CONSTANTS::TYPE_OBJECT);
}

bool CreateSchema(PyObject* pySchema, FsdSchemaAttributes &result, unsigned int argID)
{
	if (!PyDict_Check(pySchema))
	{
		PyErr_Format(PyExc_TypeError,
			"Could not match pySchema %i to expected type: dictionary.",
			argID);
		return false;
	}

	// Type is the only shcema attribute that is required
	std::string schemaType;
	if (!GetRequiredStringDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_TYPE, argID, schemaType))
	{
		return false;
	}

	if (HasOptionalValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_DEFAULT))
	{
		result.hasDefault = true;
	}

	if (HasOptionalValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_SIZE))
	{
		result.hasSize = true;
		if (!GetRequiredIntDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_SIZE, argID, result.size))
		{
			return false;
		}
	}

	if (!GetOptionalBoolDictValue(pySchema, PY_SCHEMA_CONSTANTS::ATTRIBUTE_ISOPTIONAL, argID, result.isOptional))
	{
		return false;
	}

	result.schemaTypeAsString = schemaType;

	if (IsBoolSchema(schemaType))
	{
		return CreateBoolSchema(pySchema, result, argID);
	}
	else if (IsIntSchema(schemaType))
	{
		return CreateIntSchema(pySchema, result, argID);
	}
	else if (IsFloatSchema(schemaType))
	{
		return CreateFloatSchema(pySchema, result, argID);
	}
	else if (IsStringSchema(schemaType))
	{
		return CreateStringSchema(pySchema, result, argID);
	}
	else if (IsUnicodeSchema(schemaType))
	{
		return CreateUnicodeSchema(pySchema, result, argID);
	}
	else if (IsVectorSchema(schemaType))
	{
		return CreateVectorSchema(pySchema, result, argID);
	}
	else if (IsListSchema(schemaType))
	{
		return CreateListSchema(pySchema, result, argID);
	}
	else if (IsDictSchema(schemaType))
	{
		return CreateDictSchema(pySchema, result, argID);
	}
	else if (IsObjectSchema(schemaType))
	{
		return CreateObjectSchema(pySchema, result, argID);
	}
	return nullptr;
}

bool BlueExtractArgumentImpl(PyObject* argument, FsdSchemaAttributes &result, unsigned int argID, std::false_type isBlueType)
{
	return CreateSchema(argument, result, argID);
}
