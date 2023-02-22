////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef FsdObject_h
#define FsdObject_h

#include "StdAfx.h"
#include "FsdSchema.h"

BLUE_CLASS(FsdObject) :
public IRoot
{
public:
	EXPOSE_TO_BLUE();

	FsdObject(IRoot* lockobj = NULL);
	~FsdObject();

	void SetObjectData(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	BlueStdResult GetAttr(const char* attributeName, PyObject*& result);
private:
	std::shared_ptr<ObjectSchemaAttributes>  m_objectSchemaAttributes;
	
	BlueStdResult GetDefaultValue(const std::string attributeName, PyObject*& result);

	// This represents a lookup table for all variable offset items
	std::map<std::string, uint32_t> m_offsetAttributeLookupTable;
	uint32_t m_offsetToVariableSizedData;
	
	bool m_isFixedSize;

	const char* m_data;
	uint32_t m_offset;
	std::string m_path;
};

TYPEDEF_BLUECLASS(FsdObject);
#endif //FsdObject_h