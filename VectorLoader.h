////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef VectorLoader_h
#define VectorLoader_h

#include "FsdSchema.h"

class VectorLoader
{
public:
	static PyObject* LoadVector2(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector3(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector4(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);

	static PyObject* LoadVector2d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector3d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector4d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);

	static PyObject* CreateVectorWithAliases(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
};

#endif //VectorLoader_h
