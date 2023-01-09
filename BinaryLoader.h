////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef BinaryLoader_h
#define BinaryLoader_h

#include "Python.h"
#include "BoolBeResult.h"
#include <BlueExposure.h>
#include "FsdSchema.h"
#include "FsdVector.h"
#include "FsdObject.h"
#include "FsdList.h"
#include "FsdDict.h"

class BinaryLoader
{
public:
	// Initial starting point
	static PyObject* LoadBinaryFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);

	// Individual binary laoders that return c types
	static bool CLoadBoolFromBinaryString(const char* data, uint32_t offset, const char* path);
	static uint32_t CLoadUnsigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path);
	static int32_t CLoadSigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path);
	static float CLoadFloatFromBinaryString(const char* data, uint32_t offset, const char* path);
	static double CLoadDoubleFromBinaryString(const char* data, uint32_t offset, const char* path);
	static std::string CLoadStringFromBinaryString(const char* data, uint32_t offset, const char* path);
	static std::wstring CLoadUnicodeStringFromBinaryString(const char* data, uint32_t offset, const char* path, bool &success);
	
	// Individual binary loaders that return python objects
	static PyObject* LoadBoolFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadUnsigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadSigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadFloatFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadDoubleFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadStringFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadUnicodeStringFromBinaryString(const char* data, uint32_t offset, const char* path);
	static PyObject* LoadObjectFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadListFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadDictFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);

	static PyObject* LoadVector2(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector3(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector4(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector2d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector3d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
	static PyObject* LoadVector4d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);

	// Vector helper function
	static PyObject* CreateVectorWithAliases(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path);
};

#endif //BinaryLoader_h