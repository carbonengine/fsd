////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//
#include "StdAfx.h"
#include "VectorLoader.h"
#include "FsdVector.h"

PyObject* VectorLoader::CreateVectorWithAliases(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	FsdVectorPtr fsdVector;
	fsdVector.CreateInstance();

	fsdVector->SetVectorData(data, offset, schemaAttributes, path);
	return BlueWrapReturnValue(BlueScriptArguments(), fsdVector->GetRawRoot());
}

PyObject* VectorLoader::LoadVector2(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector2 value = *reinterpret_cast<const Vector2*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* VectorLoader::LoadVector3(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector3 value = *reinterpret_cast<const Vector3*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* VectorLoader::LoadVector4(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector4 value = *reinterpret_cast<const Vector4*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* VectorLoader::LoadVector2d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector2d value = *reinterpret_cast<const Vector2d*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* VectorLoader::LoadVector3d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector3d value = *reinterpret_cast<const Vector3d*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* VectorLoader::LoadVector4d(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, const char* path)
{
	if (schemaAttributes.vectorAttributes->hasAliases){
		return CreateVectorWithAliases(data, offset, schemaAttributes, path);
	}
	Vector4d value = *reinterpret_cast<const Vector4d*>(&data[offset]);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}
