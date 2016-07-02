////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "BinaryLoader.h"
#include "VectorLoader.h"
#include "FsdObject.h"

const UINT UTF8 = 65001;

PyObject* BinaryLoader::LoadBinaryFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	PyObject* retValue = nullptr;

	switch (schemaAttributes.schemaType)
	{
	case BOOL_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadBoolFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case SIGNED_32_BIT_INT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadSigned32BitIntFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case UNSIGNED_32_BIT_INT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadUnsigned32BitIntFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadFloatFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadDoubleFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case STRING_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadStringFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case UTF8_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadUnicodeStringFromBinaryString(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE:
		retValue = VectorLoader::LoadVector2(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE:
		retValue = VectorLoader::LoadVector3(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE:
		retValue = VectorLoader::LoadVector4(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE:
		retValue = VectorLoader::LoadVector2d(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE:
		retValue = VectorLoader::LoadVector3d(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE:
		retValue = VectorLoader::LoadVector4d(data, offset, schemaAttributes, path);
		break;
	case OBJECT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadObjectFromString(data, offset, schemaAttributes, path);
		break;
	default:
		std::string error = "unsupported schema type '" + schemaAttributes.schemaTypeAsString + "'";
		PyErr_Format(PyExc_TypeError, error.c_str());
	}

	return retValue;
}

PyObject* BinaryLoader::LoadBoolFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	bool value = *reinterpret_cast<const bool*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadUnsigned32BitIntFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	uint32_t value = *reinterpret_cast<const uint32_t*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadSigned32BitIntFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	int32_t value = *reinterpret_cast<const int32_t*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadFloatFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	float value = *reinterpret_cast<const float*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadDoubleFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	double value = *reinterpret_cast<const double*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadStringFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	uint32_t stringLength = *reinterpret_cast<const uint32_t*>(&data[offset]);
	std::string retVal(&data[offset + 4], stringLength);
	return BlueWrapReturnValue(BlueScriptArguments(), retVal);
}

PyObject* BinaryLoader::LoadUnicodeStringFromBinaryString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	uint32_t stringLength = *reinterpret_cast<const uint32_t*>(&data[offset]);

	// WHY IS THERE NO STL UNICODE ENCODING THING?
	const int requiredBufferSize = MultiByteToWideChar(
		UTF8,
		0,
		&data[4 + offset],
		stringLength,
		nullptr,
		0
		);

	std::wstring retVal;
	retVal.resize(requiredBufferSize);

	if (
		MultiByteToWideChar(
		UTF8,
		0,
		&data[4 + offset],
		stringLength,
		&retVal[0],
		requiredBufferSize
		) == 0
		)
	{
		PyErr_SetExcFromWindowsErr(PyExc_RuntimeError, GetLastError());
		return nullptr;
	}

	return BlueWrapReturnValue(BlueScriptArguments(), retVal);
}

PyObject* BinaryLoader::LoadObjectFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdObjectPtr fsdObject;
	fsdObject.CreateInstance();

	fsdObject->SetObjectData(data, offset, schemaAttributes, path);
	return BlueWrapReturnValue(BlueScriptArguments(), fsdObject->GetRawRoot());
}


