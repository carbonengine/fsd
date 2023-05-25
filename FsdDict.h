////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		June 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef FsdDict_h
#define FsdDict_h

#include "StdAfx.h"
#include "FsdSchema.h"
#include "FsdList.h"
#include "FsdDictOffsetLookup.h"

class FsdDict : public PyObject
{
public:
	FsdDict();
	~FsdDict();

	void Initialize(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, FsdDictOffsetLookup* offsetLookup, const char* path);
	PyObject* GetValue(PyObject* key) const;
	bool Contains(PyObject* key) const;

private:
	bool FindKeyValueOffset(PyObject* key, FooterOffsetSize &offset) const;

	FsdSchemaAttributes m_schemaAttributes;
	FsdSchemaAttributes m_valueSchemaAttributes;
	const char* m_data;
	std::string m_path;
	uint32_t m_offsetToData;

	FsdDictOffsetLookup* m_offsetLookup;
};

PyObject* FsdDict_Subscript(PyObject* self, PyObject* key);
PyObject* FsdDict_GetAttrString(PyObject *o, char* attr_name);
PyObject* FsdDict_Dir(PyObject *self, PyObject * args);
int FsdDict_Contains(PyObject *self, PyObject * args);
void FsdDict_dealloc(PyObject* self);
PyObject* FsdDict_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int FsdDict_init(FsdDict *self, PyObject *args, PyObject *kwds);
Py_ssize_t FsdDict_Length(PyObject *selfO);
PyObject *FsdDict_GetIndex(PyObject *selfO, Py_ssize_t i);
FsdDict* CreateFsdDict(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path);

#endif
