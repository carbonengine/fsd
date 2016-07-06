////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "BinaryLoader.h"
#include "FsdVector.h"
#include "FsdObject.h"

const UINT UTF8 = 65001;

PyObject* BinaryLoader::LoadBinaryFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	PyObject* retValue = nullptr;

	switch (schemaAttributes.schemaType)
	{
	case BOOL_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadBoolFromBinaryString(data, offset, path);
		break;
	case SIGNED_32_BIT_INT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadSigned32BitIntFromBinaryString(data, offset, path);
		break;
	case UNSIGNED_32_BIT_INT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadUnsigned32BitIntFromBinaryString(data, offset, path);
		break;
	case FLOAT32_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadFloatFromBinaryString(data, offset, path);
		break;
	case DOUBLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadDoubleFromBinaryString(data, offset, path);
		break;
	case STRING_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadStringFromBinaryString(data, offset, path);
		break;
	case UTF8_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadUnicodeStringFromBinaryString(data, offset, path);
		break;
	case FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadVector2(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadVector3(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadVector4(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadVector2d(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadVector3d(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadVector4d(data, offset, schemaAttributes, path);
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

PyObject* BinaryLoader::LoadBoolFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	bool value = *reinterpret_cast<const bool*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadUnsigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	uint32_t value = *reinterpret_cast<const uint32_t*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadSigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	int32_t value = *reinterpret_cast<const int32_t*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadFloatFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	float value = *reinterpret_cast<const float*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadDoubleFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	double value = *reinterpret_cast<const double*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadStringFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	uint32_t stringLength = *reinterpret_cast<const uint32_t*>(&data[offset]);
	std::string retVal(&data[offset + 4], stringLength);
	return BlueWrapReturnValue(BlueScriptArguments(), retVal);
}

PyObject* BinaryLoader::LoadUnicodeStringFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	uint32_t stringLength = *reinterpret_cast<const uint32_t*>(&data[offset]);

	// WHY IS THERE NO STL UNICODE ENCODING THING?
	const int requiredBufferSize = MultiByteToWideChar( UTF8, 0, &data[4 + offset], stringLength, nullptr, 0 );

	std::wstring retVal;
	retVal.resize(requiredBufferSize);

	if ( MultiByteToWideChar( UTF8, 0, &data[4 + offset], stringLength, &retVal[0], requiredBufferSize ) == 0 )
	{
		PyErr_SetExcFromWindowsErr(PyExc_RuntimeError, GetLastError());
		return nullptr;
	}

	return BlueWrapReturnValue(BlueScriptArguments(), retVal);
}

PyObject* BinaryLoader::CreateVectorWithAliases(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdVectorPtr fsdVector;
	fsdVector.CreateInstance();

	fsdVector->SetVectorData(data, offset, schemaAttributes, path);
	return BlueWrapReturnValue(BlueScriptArguments(), fsdVector->GetRawRoot());
}

PyObject* BinaryLoader::LoadVector2(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector2 value = *reinterpret_cast<const Vector2*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadVector3(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector3 value = *reinterpret_cast<const Vector3*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadVector4(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector4 value = *reinterpret_cast<const Vector4*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadVector2d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector2d value = *reinterpret_cast<const Vector2d*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadVector3d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector3d value = *reinterpret_cast<const Vector3d*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadVector4d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector4d value = *reinterpret_cast<const Vector4d*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* BinaryLoader::LoadObjectFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdObjectPtr fsdObject;
	fsdObject.CreateInstance();

	fsdObject->SetObjectData(data, offset, schemaAttributes, path);
	return BlueWrapReturnValue(BlueScriptArguments(), fsdObject->GetRawRoot());
}


