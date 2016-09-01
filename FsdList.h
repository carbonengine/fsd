////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		June 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef FsdList_h
#define FsdList_h

#include "StdAfx.h"
#include "FsdSchema.h"


class FsdList : public PyObject
{
public:
	FsdList() :
		m_data(nullptr),
		m_path(""),
		m_listLength(0), 
		m_itemsAreFixedSize(false)
	{}

	void Initialize(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path);
	uint32_t GetLength() const { return m_listLength; };
	PyObject* GetIndex(int32_t index);

private:
	ListSchemaAttributes m_schemaAttributes;
	const char* m_data;
	std::string m_path;

	uint32_t m_offset;
	uint32_t m_listLength;
	bool m_itemsAreFixedSize;

	std::vector<uint32_t> m_offsetLookup;
};

PyObject* FsdList_Dir(PyObject *self, PyObject * args);
void FsdList_dealloc(PyObject* self);
PyObject* FsdList_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int FsdList_init(FsdList *self, PyObject *args, PyObject *kwds);
Py_ssize_t FsdList_Length(PyObject *selfO);
PyObject *FsdList_GetIndex(PyObject *selfO, Py_ssize_t i);
FsdList* CreateFsdList(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path);

#endif