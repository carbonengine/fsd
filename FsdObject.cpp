// Copyright © 2016 CCP ehf.

#include "StdAfx.h"
#include "FsdObject.h"
#include "BinaryLoader.h"
#include <sstream>
#include <iostream>

FsdObject::FsdObject(IRoot* lockobj) :
m_data(nullptr),
m_offset(0),
m_path("")
{
}


FsdObject::~FsdObject()
{
	m_objectSchemaAttributes.reset();
}

void FsdObject::SetObjectData(const char* data, uint32_t offset, const FsdSchemaAttributes& schemaAttributes, const char * path)
{
	m_data = data;
	m_offset = offset;
	m_objectSchemaAttributes = schemaAttributes.objectAttributes;
	m_path = path;
	m_isFixedSize = schemaAttributes.hasSize;

	// We don't have to do anything if this is a fixed sized object
	if (m_isFixedSize){
		return;
	}

	// Here we need to plot out which attributes are present on the instance and their offsets
	// 1. Find the optional attributes that are present on the instance
	std::vector<std::string> attributesWithVariableOffset = m_objectSchemaAttributes->attributesWithVariableOffset;
	std::map<std::string, uint64_t> optionalValueLookup = m_objectSchemaAttributes->optionalValueLookups;
	if (optionalValueLookup.size() != 0)
	{
		const char* optionalAttributesFieldData = &m_data[offset + m_objectSchemaAttributes->endOfFixedSizedData];
		
		uint64_t optionalAttributesField = *reinterpret_cast<const uint64_t*>(optionalAttributesFieldData);
		for (auto lookup = optionalValueLookup.begin(); lookup != optionalValueLookup.end(); ++lookup)
		{
			uint64_t attributeBit = (*lookup).second;

			if (!(optionalAttributesField & attributeBit))
			{
				const char * attributeName = (*lookup).first.c_str();
				attributesWithVariableOffset.erase(find(attributesWithVariableOffset.begin(), attributesWithVariableOffset.end(), std::string(attributeName)));
			}
		}
	}

	// 2. Store the offset to attributes in memory
	uint32_t offsetAttributeArrayStart = m_offset + m_objectSchemaAttributes->endOfFixedSizedData + 8;
	uint32_t sizeOfOffsetAttributeTable = uint32_t(4 * attributesWithVariableOffset.size());

	m_offsetToVariableSizedData = offsetAttributeArrayStart + sizeOfOffsetAttributeTable;

	uint32_t offsetTableSize = m_offsetToVariableSizedData - offsetAttributeArrayStart;
	for (uint32_t offsetTableOffset = 0; offsetTableOffset < offsetTableSize; offsetTableOffset += 4)
	{
		int index = int(offsetTableOffset / 4);
		std::string name(attributesWithVariableOffset[index]);
		std::string offsetData(&m_data[offsetAttributeArrayStart + offsetTableOffset], 4);
		m_offsetAttributeLookupTable[name] = *reinterpret_cast<const uint32_t*>(offsetData.c_str()) + m_offsetToVariableSizedData;
	}
}

BlueStdResult FsdObject::GetAttr(const char* attributeName, PyObject*& result)
{
	BlueStdResult success;
	std::string attributeNameString(attributeName);
	std::stringstream newPath;
	newPath << std::string(m_path) << std::string(".") << attributeNameString;

	if (m_objectSchemaAttributes->attributes.find(attributeNameString) == m_objectSchemaAttributes->attributes.end())
	{
		std::stringstream errorString;
		errorString << "Object: " << std::string(m_path) << " - Attribute '" << attributeNameString << "' is not in the schema for this object. It may be removed by the 'usage' flag under the build configuration that produced this data.";
		return BlueStdResult(BLUE_STD_RESULT_KEY_ERROR, errorString.str().c_str());
	}

	// The attribute should exist, first check the constant offset attributes, then the variable offset attributes
	// If it is neither it is an optional attribute that is not set on this instance
	if (m_objectSchemaAttributes->constantAttributeOffsets.find(attributeNameString) != m_objectSchemaAttributes->constantAttributeOffsets.end())
	{
		// found it in the constant attributes
		uint32_t offset = m_objectSchemaAttributes->constantAttributeOffsets[attributeNameString];
		result = BinaryLoader::LoadBinaryFromString(m_data, m_offset + offset, *m_objectSchemaAttributes->attributes[attributeNameString], newPath.str().c_str());
		return BlueStdResult(BLUE_STD_RESULT_OK);
	}
	
	if (m_offsetAttributeLookupTable.find(attributeNameString) != m_offsetAttributeLookupTable.end())
	{
		// found it in the constant attributes
		uint32_t offset = m_offsetAttributeLookupTable[attributeNameString];
		result = BinaryLoader::LoadBinaryFromString(m_data, offset, *m_objectSchemaAttributes->attributes[attributeNameString], newPath.str().c_str());
		return BlueStdResult(BLUE_STD_RESULT_OK);
	}

	// check for default values
	BlueStdResult defaultAttributeLookupResult = GetDefaultValue(attributeNameString, result);

	if (defaultAttributeLookupResult.GetType() == BLUE_STD_RESULT_OK)
	{
		return defaultAttributeLookupResult;
	}

	std::stringstream errorString;
	errorString << "Object: " << std::string(m_path) << " - Attribute '" << attributeNameString << "' does not exist on this instance";
	return BlueStdResult(BLUE_STD_RESULT_ATTRIBUTE_ERROR, errorString.str().c_str());
}

BlueStdResult FsdObject::GetDefaultValue(const std::string attributeName, PyObject*& result)
{
	auto attributeSchema = m_objectSchemaAttributes->attributes.find(attributeName);

	if (attributeSchema == m_objectSchemaAttributes->attributes.end())
	{
		// return something
		return BlueStdResult(BLUE_STD_RESULT_ATTRIBUTE_ERROR);
	}

	std::shared_ptr<FsdSchemaAttributes> schema = attributeSchema->second;
	if (!schema->hasDefault)
	{
		// we don't have a default value for this optional attribute
		return BlueStdResult(BLUE_STD_RESULT_ATTRIBUTE_ERROR);
	}
	Py_INCREF(schema->defaultValue);
	result = schema->defaultValue;
	
	return BlueStdResult(BLUE_STD_RESULT_OK);
}
