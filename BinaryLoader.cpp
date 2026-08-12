// Copyright © 2016 CCP ehf.

#include "StdAfx.h"
#include "BinaryLoader.h"
#include "BinaryLoaderHelper.h"
#ifndef _WIN32
#include <locale>
#include <codecvt>
#endif


const uint32_t UTF8 = 65001;

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
		return static_cast<FsdVector*>(BinaryLoader::LoadVector2(data, offset, schemaAttributes, path));
		break;
	case FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE:
		return BinaryLoader::LoadVector3(data, offset, schemaAttributes, path);
		break;
	case FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE:
		return BinaryLoader::LoadVector4(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE:
		return BinaryLoader::LoadVector2d(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE:
		return BinaryLoader::LoadVector3d(data, offset, schemaAttributes, path);
		break;
	case DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE:
		return BinaryLoader::LoadVector4d(data, offset, schemaAttributes, path);
		break;
	case OBJECT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadObjectFromString(data, offset, schemaAttributes, path);
		break;
	case LIST_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadListFromString(data, offset, schemaAttributes, path);
		break;
	case DICT_SCHEMA_TYPE:
		retValue = BinaryLoader::LoadDictFromString(data, offset, schemaAttributes, path);
		break;
	default:
		std::string error = "unsupported schema type '" + schemaAttributes.schemaTypeAsString + "'";
		PyErr_SetString(PyExc_TypeError, error.c_str());
	}

	return retValue;
}

bool BinaryLoader::CLoadBoolFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	std::string dataString(&data[offset], sizeof(bool));
	return *reinterpret_cast<const bool*>(dataString.c_str());
}

PyObject* BinaryLoader::LoadBoolFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	return BlueWrapReturnValue(BlueScriptArguments(), CLoadBoolFromBinaryString(data, offset, path));
}

uint32_t BinaryLoader::CLoadUnsigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	std::string dataString(&data[offset], sizeof(uint32_t));
	return *reinterpret_cast<const uint32_t*>(dataString.c_str());
}

PyObject* BinaryLoader::LoadUnsigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path)
{	
	return BlueWrapReturnValue(BlueScriptArguments(), CLoadUnsigned32BitIntFromBinaryString(data, offset, path));
}

int32_t BinaryLoader::CLoadSigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	std::string dataString(&data[offset], sizeof(int32_t));
	return *reinterpret_cast<const int32_t*>(dataString.c_str());
}

PyObject* BinaryLoader::LoadSigned32BitIntFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	return BlueWrapReturnValue(BlueScriptArguments(), CLoadSigned32BitIntFromBinaryString(data, offset, path));
}

float BinaryLoader::CLoadFloatFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	std::string dataString(&data[offset], sizeof(float));
	return *reinterpret_cast<const float*>(dataString.c_str());
}

PyObject* BinaryLoader::LoadFloatFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	return BlueWrapReturnValue(BlueScriptArguments(), CLoadFloatFromBinaryString(data, offset, path));
}

double BinaryLoader::CLoadDoubleFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	std::string dataString(&data[offset], sizeof(double));
	return *reinterpret_cast<const double*>(dataString.c_str());
}

PyObject* BinaryLoader::LoadDoubleFromBinaryString(const char* data, uint32_t offset, const char* path)
{	
	return BlueWrapReturnValue(BlueScriptArguments(), CLoadDoubleFromBinaryString(data, offset, path));
}

std::string BinaryLoader::CLoadStringFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	std::string sizeString(&data[offset], sizeof(uint32_t));
	uint32_t stringLength = *reinterpret_cast<const uint32_t*>(sizeString.c_str());
	std::string retVal(&data[offset + 4], stringLength);
	return retVal;
}

PyObject* BinaryLoader::LoadStringFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	return BlueWrapReturnValue(BlueScriptArguments(), CLoadStringFromBinaryString(data, offset, path));
}

std::wstring BinaryLoader::CLoadUnicodeStringFromBinaryString(const char* data, uint32_t offset, const char* path, bool &success)
{
    std::wstring retVal;
	std::string sizeString(&data[offset], sizeof(uint32_t));
	uint32_t stringLength = *reinterpret_cast<const uint32_t*>(sizeString.c_str());
#if _WIN32
	// WHY IS THERE NO STL UNICODE ENCODING THING?
	const int requiredBufferSize = MultiByteToWideChar(UTF8, 0, &data[4 + offset], stringLength, nullptr, 0);

	retVal.resize(requiredBufferSize);

	if (MultiByteToWideChar(UTF8, 0, &data[4 + offset], stringLength, &retVal[0], requiredBufferSize) == 0)
	{
		PyErr_SetExcFromWindowsErr(PyExc_RuntimeError, GetLastError());
		success = false;
		return retVal;
	}
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    try
    {
        retVal = conv.from_bytes( data + 4 + offset, data + 4 + offset + stringLength );
    }
    catch (const std::range_error& error)
    {
        PyErr_SetString(PyExc_RuntimeError, error.what());
        success = false;
        return retVal;
    }
#endif
	success = true;
	return retVal;
}

PyObject* BinaryLoader::LoadUnicodeStringFromBinaryString(const char* data, uint32_t offset, const char* path)
{
	bool success;
	std::wstring retVal = CLoadUnicodeStringFromBinaryString(data, offset, path, success);

	if (!success)
	{
		return nullptr;
	}

	return BlueWrapReturnValue(BlueScriptArguments(), retVal);
}

PyObject* BinaryLoader::CreateVectorWithAliases(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdVector* fsdVector = CreateFsdVector(data, offset, schemaAttributes, path);
	Py_INCREF(fsdVector);
	return fsdVector;
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

PyObject* BinaryLoader::LoadListFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdList* fsdList = CreateFsdList(data, offset, schemaAttributes, path);
	Py_INCREF(fsdList);
	return fsdList;
}

PyObject* BinaryLoader::LoadDictFromString(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdDict* fsdDict = CreateFsdDict(data, offset, schemaAttributes, path);
	Py_INCREF(fsdDict);
	return fsdDict;
}
