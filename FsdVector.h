// Copyright © 2016 CCP ehf.

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
	FSD_VECTOR4D,
	UNKNOWN_VECTOR_TYPE
};

class FsdVector: public PyObject
{
public:
	FsdVector() :
		m_data(nullptr),
		m_vectorType(UNKNOWN_VECTOR_TYPE),
		m_path(""),
		m_exposedData(nullptr)
	{}

	void Initialize(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path);
	PyObject* GetByAttribute(const char* attribute_name);
	PyObject* GetByIndex(uint8_t index);
	Py_ssize_t GetLength();
private:
	PyObject* GetValueByIndex(uint8_t index);


	VectorType m_vectorType;
	FsdSchemaAttributes m_schemaAttributes;
	const char* m_data;
	std::string m_path;
	PyObject* m_exposedData;
};

PyObject* FsdVector_GetAttrString(PyObject *o, char* attr_name);
PyObject* FsdVector_Dir(PyObject *self, PyObject * args);
void FsdVector_dealloc(PyObject* self);
PyObject* FsdVector_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int FsdVector_init(FsdVector *self, PyObject *args, PyObject *kwds);
Py_ssize_t FsdVector_Length(PyObject *selfO);
PyObject *FsdVector_GetIndex(PyObject *selfO, Py_ssize_t i);
FsdVector* CreateFsdVector(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path);

#endif