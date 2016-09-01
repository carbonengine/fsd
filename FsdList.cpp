////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		June 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "FsdList.h"
#include "BinaryLoader.h"
#include "BinaryLoaderHelper.h"
#include <sstream>


void FsdList::Initialize(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path)
{
	m_schemaAttributes = *schemaAttributes.listAttributes;
	m_path = path;
	m_offset = offset;
	uint32_t offsetToData = 0;

	// figure out the list size
	if (m_schemaAttributes.fixedSized)
	{
		m_listLength = m_schemaAttributes.listSize;
	}
	else
	{
		offsetToData = 4;
		m_listLength = BinaryLoaderHelper::GetUnsignedIntFromData(data, offset);
	}

	m_data = &data[offset];

	// Precompute the offset lookups for the list
	if (m_schemaAttributes.listItemSchema->hasSize)
	{
		// if the items are fixed sized, we can just compute the offsets
		uint32_t itemSize = m_schemaAttributes.listItemSchema->size;
		for (uint32_t i = 0; i < m_listLength; i++)
		{
			m_offsetLookup.push_back(offsetToData + i * itemSize);
		}
	}
	else
	{
		// read the offset header and create a lookup table for it
		for (uint32_t i = 0; i < m_listLength; i++)
		{
			m_offsetLookup.push_back(BinaryLoaderHelper::GetUnsignedIntFromData(m_data, offsetToData + i * 4));
		}
	}
}

PyObject* FsdList::GetIndex(int32_t index)
{
	uint32_t originalIndex = index;
	if (index < 0)
	{
		index += m_listLength;
	}

	if (uint32_t(index) > m_listLength || index < 0)
	{
		std::stringstream errorString;
		errorString << "FsdList: trying to get index " << originalIndex << "from a list that has " << m_listLength << "elements";
		PyErr_SetString(PyExc_IndexError, errorString.str().c_str());
		Py_RETURN_NONE;
	}

	uint32_t offset = m_offsetLookup[index];
	std::stringstream fmt;
	fmt << m_path << "[" << index << "]";

	return BinaryLoader::LoadBinaryFromString(m_data, offset, *m_schemaAttributes.listItemSchema, fmt.str().c_str());
}


// The python class functions on this enum type
static PyMethodDef PyFsdListType_methods[] = {
	{
		"__dir__",
		(PyCFunction)FsdList_Dir, METH_NOARGS,
		"Returns a list of the enumeration names for this enum"
	},
	{ NULL, NULL, 0, NULL }
};

static PySequenceMethods fsdListSequenceMethods = {
	FsdList_Length,
	0,
	0,
	FsdList_GetIndex,
	0,
	0,
	0
};

PyTypeObject PyFsdListType = {
	PyObject_HEAD_INIT(NULL)
	0,
	"pyFSD.FsdList",
	sizeof(FsdList),
	0,
	FsdList_dealloc,				/* tp_dealloc */
	0,								/* tp_print */
	0,								/* tp_getattr */
	0,								/* tp_setattr        */
	0,								/* tp_compare        */
	0,								/* tp_repr           */
	0,								/* tp_as_number      */
	&fsdListSequenceMethods,		/* tp_as_sequence    */
	0,								/* tp_as_mapping     */
	0,								/* tp_hash           */
	0,								/* tp_call           */
	0,								/* tp_str            */
	0,								/* tp_getattro       */
	0,								/* tp_setattro       */
	0,								/* tp_as_buffer      */
	Py_TPFLAGS_DEFAULT,				/* tp_flags          */
	0,								/* tp_doc            */
	0,								/* tp_traverse       */
	0,								/* tp_clear          */
	0,								/* tp_richcompare    */
	0,								/* tp_weaklistoffset */
	0,								/* tp_iter           */
	0,								/* tp_iternext       */
	PyFsdListType_methods,		    /* tp_methods        */
	0,								/* tp_members        */
	0,								/* tp_getset         */
	0,								/* tp_base           */
	0,								/* tp_dict           */
	0,								/* tp_descr_get      */
	0,								/* tp_descr_set      */
	0,								/* tp_dictoffset     */
	(initproc)FsdList_init,		/* tp_init           */
	0,								// tp_alloc
	FsdList_new,				/* tp_new */
};

static PyObject* FsdList_Dir(PyObject *self, PyObject * args)
{
	return nullptr;
}

void FsdList_dealloc(PyObject* self)
{
	FsdDict* fsdDict = static_cast<FsdDict*>(self);
	delete fsdDict;
};

PyObject* FsdList_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	auto self = new FsdList;
	self->ob_refcnt = 1;
	self->ob_type = type;
	return self;
}

static int FsdList_init(FsdList *self, PyObject *args, PyObject *kwds)
{
	PyErr_SetString(PyExc_NotImplementedError, "pyFSD.FsdList::__init__  cannot initialize from python");
	return -1;
}

Py_ssize_t FsdList_Length(PyObject *selfO)
{
	FsdList* fsdList = static_cast<FsdList*>(selfO);
	return fsdList->GetLength();
}

PyObject *FsdList_GetIndex(PyObject *selfO, Py_ssize_t i)
{
	FsdList* fsdList = static_cast<FsdList*>(selfO);
	return fsdList->GetIndex(int32_t(i));
}

FsdList* CreateFsdList(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path)
{
	FsdList* fsdList = static_cast<FsdList*>(FsdList_new(&PyFsdListType, Py_None, Py_None));
	fsdList->Initialize(data, offset, schemaAttributes, path);

	return fsdList;
}
