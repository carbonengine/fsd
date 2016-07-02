////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef FsdVector_h
#define FsdVector_h

#include "StdAfx.h"
#include "FsdSchema.h"

enum VectorType {
	FSD_VECTOR2,
	FSD_VECTOR3,
	FSD_VECTOR4,
	FSD_VECTOR2D,
	FSD_VECTOR3D,
	FSD_VECTOR4D
};

BLUE_CLASS(FsdVector) :
public IRoot
{
public:
	EXPOSE_TO_BLUE();

	FsdVector(IRoot* lockobj = NULL);
	~FsdVector();

	void SetVectorData(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path);
	BlueStdResult GetAttr(const char* attributeName, PyObject*& result);
	BlueStdResult GetItem(uint32_t index, PyObject*& result);
private:
	std::map<std::string, uint8_t> m_aliasIndices;
	VectorType m_vectorType;
	FsdSchemaAttributes m_schemaAttributes;
	const char* m_data;
	std::string m_path;

	PyObject* m_exposedData;
};

TYPEDEF_BLUECLASS(FsdVector);
#endif