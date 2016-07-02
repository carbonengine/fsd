////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "FsdVector.h"


FsdVector::FsdVector(IRoot* lockobj) :
m_data(nullptr),
m_path("")
{
}


FsdVector::~FsdVector()
{

}

void FsdVector::SetVectorData(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path)
{
	m_data = &data[offset];
	m_schemaAttributes = schemaAttributes;
	m_path = path;

	m_aliasIndices = m_schemaAttributes.vectorAttributes->aliases;
	switch (m_schemaAttributes.schemaType)
	{
		case FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE:
			m_vectorType = FSD_VECTOR2;
			m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector2*>(data));
			break;
		case FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE:
			m_vectorType = FSD_VECTOR3;
			m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector3*>(data));
			break;
		case FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE:
			m_vectorType = FSD_VECTOR4;
			m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector4*>(data));
			break;
		case DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE:
			m_vectorType = FSD_VECTOR2D;
			m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector2d*>(data));
			break;
		case DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE:
			m_vectorType = FSD_VECTOR3D;
			m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector3d*>(data));
			break;
		case DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE:
			m_vectorType = FSD_VECTOR4D;
			m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector4d*>(data));
			break;
	} 

}

BlueStdResult FsdVector::GetAttr(const char* attributeName, PyObject*& result)
{
	BlueStdResult success;
	if (m_aliasIndices.find(attributeName) != m_aliasIndices.end())
	{
		if (m_vectorType <= FSD_VECTOR4)
		{
			const char* data = &m_data[4 * m_aliasIndices[attributeName]];
			float value = *reinterpret_cast<const float*>(data);
			result = BlueWrapReturnValue(BlueScriptArguments(), value);
			return BlueStdResult(BLUE_STD_RESULT_OK);
		}
		else
		{
			const char* data = &m_data[4 * m_aliasIndices[attributeName]];
			double value = *reinterpret_cast<const double*>(data);
			result = BlueWrapReturnValue(BlueScriptArguments(), value);
			return BlueStdResult(BLUE_STD_RESULT_OK);
		}
	}
	std::string message = "Attribute '" + std::string(attributeName) + "' does not exist on this instance";
	return BlueStdResult(BLUE_STD_RESULT_ATTRIBUTE_ERROR, message.c_str());
}

BlueStdResult FsdVector::GetItem( uint32_t index, PyObject*& result )
{
	uint8_t maxIndex = 0;
	switch (m_vectorType)
	{
	case FSD_VECTOR2:
	case FSD_VECTOR2D:
		maxIndex = 1;
		break;
	case FSD_VECTOR3:
	case FSD_VECTOR3D:
		maxIndex = 2;
		break;
	case FSD_VECTOR4:
	case FSD_VECTOR4D:
		maxIndex = 3;
		break;
	}

	// todo: add exception handling
	const char* data = &m_data[4 * index];
	float value = *reinterpret_cast<const float*>(data);
	result = BlueWrapReturnValue(BlueScriptArguments(), value);

	std::string message = "Index out of range";
	return BlueStdResult(BLUE_STD_RESULT_INDEX_ERROR, message.c_str());
}