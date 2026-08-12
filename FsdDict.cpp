// Copyright © 2016 CCP ehf.

#include "StdAfx.h"
#include "FsdDict.h"
#include "BinaryLoader.h"
#include "BinaryLoaderHelper.h"
#include <sstream>
#include <iostream>

FsdDict::FsdDict():
m_data(nullptr),
m_offsetToData(0), 
m_offsetLookup(nullptr)
{
}


FsdDict::~FsdDict()
{
}

void FsdDict::Initialize(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, FsdDictOffsetLookup* offsetLookup, const char* path)
{
	m_data = data;
	m_schemaAttributes = schemaAttributes;
	m_valueSchemaAttributes = *schemaAttributes.dictAttributes->valueSchema;

	m_path = path;
	
	m_offsetToData = offset + 4; // the plus 4 comes from the fact that there is an unsigned int at the top of the data
	m_offsetLookup = offsetLookup;
}

PyObject* FsdDict::GetValue(PyObject* key) const
{
	FooterOffsetSize offsetAndSize;

	PyObject* keyRepresentation = PyObject_Repr(key);
	const char* s = PyUnicode_AsUTF8(keyRepresentation);

	if (!FindKeyValueOffset(key, offsetAndSize))
	{
		std::stringstream errorString;
		errorString << "FsdDict '" << m_path << "': does not have the following key " << s;
		PyErr_SetString(PyExc_KeyError, errorString.str().c_str());
		Py_RETURN_NONE;
	}
	
	std::stringstream newPath;
	newPath << m_path << "[" << s << "]";

	if (offsetAndSize.size > 0)
	{
		std::string objectData = BinaryLoaderHelper::GetSubsetOfData(m_data, m_offsetToData + offsetAndSize.offset, offsetAndSize.size);
		return BinaryLoader::LoadBinaryFromString(objectData.c_str(), 0, m_valueSchemaAttributes, newPath.str().c_str());
	}

	return BinaryLoader::LoadBinaryFromString(m_data, m_offsetToData + offsetAndSize.offset, m_valueSchemaAttributes, newPath.str().c_str());
}

bool FsdDict::FindKeyValueOffset(PyObject* key, FooterOffsetSize &offset) const
{	
	OFFSET_LOOKUP_RESULT result = m_offsetLookup->GetOffsetAndSize(key, offset);
	return result == OFFSET_LOOKUP_OK;
}

/* -----------------------------------------------
	This function checks if a specific key exists in the m_offsetLookup.
	If the function 
   -----------------------------------------------*/
bool FsdDict::Contains(PyObject* key) const
{
	return m_offsetLookup->Contains(key);
}

// The python class functions on this enum type
static PyMethodDef PyFsdDictType_methods[] = {
	{
		"__dir__",
		(PyCFunction)FsdDict_Dir, METH_NOARGS,
		"Returns a list of the enumeration names for this enum"
	},
	{ NULL, NULL, 0, NULL }
};

static PyMappingMethods fsdDictMappingMethods =
{
	FsdDict_Length,			// lenfunc mp_length;
	FsdDict_Subscript,		// binaryfunc mp_subscript;
	0	// objobjargproc mp_ass_subscript;
};

static PySequenceMethods fsdDictSequenceMethods =
{
	NULL,				// lenfunc sq_length;
	NULL,				// binaryfunc sq_concat;
	NULL,				// ssizeargfunc sq_repeat;
	NULL,				// ssizeargfunc sq_item;
	NULL,				// ssizessizeargfunc sq_slice;
	NULL,				// ssizeobjargproc sq_ass_item;
	NULL,				// ssizessizeobjargproc sq_ass_slice;
	FsdDict_Contains,		// objobjproc sq_contains;
};

PyTypeObject PyFsdDictType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"pyFSD.FsdDict",
	sizeof(FsdDict),
	0,
	FsdDict_dealloc,				/* tp_dealloc */
	0,								/* tp_print */
	FsdDict_GetAttrString,			/* tp_getattr */
	0,								/* tp_setattr        */
	0,								/* tp_compare        */
	0,								/* tp_repr           */
	0,								/* tp_as_number      */
	&fsdDictSequenceMethods,		/* tp_as_sequence    */
	&fsdDictMappingMethods,			/* tp_as_mapping     */
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
	0, //TODO						/* tp_iter           */
	0, // TODO						/* tp_iternext       */
	PyFsdDictType_methods,		    /* tp_methods        */
	0,								/* tp_members        */
	0,								/* tp_getset         */
	0,								/* tp_base           */
	0,								/* tp_dict           */
	0,								/* tp_descr_get      */
	0,								/* tp_descr_set      */
	0,								/* tp_dictoffset     */
	(initproc)FsdDict_init,		/* tp_init           */
	0,								// tp_alloc
	FsdDict_new,				/* tp_new */
};

PyObject* FsdDict_Dir(PyObject *self, PyObject * args)
{
	return nullptr;
}

void FsdDict_dealloc(PyObject* self)
{
	FsdDict* fsdDict = static_cast<FsdDict*>(self);
	delete fsdDict;
}

PyObject* FsdDict_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	auto self = new FsdDict;
	self->ob_refcnt = 1;
	self->ob_type = type;
	return self;
}

int FsdDict_init(FsdDict *self, PyObject *args, PyObject *kwds)
{
	PyErr_SetString(PyExc_NotImplementedError, "pyFSD.FsdDict::__init__  cannot initialize from python");
	return -1;
}

PyObject* FsdDict_GetAttrString(PyObject *o, char* attr_name)
{
	FsdDict* fsdDict = static_cast<FsdDict*>(o);
	std::string attributeName(attr_name);
	
	std::string message = "Attribute '" + std::string(attributeName) + "' does not exist on this instance";
	PyErr_SetString(PyExc_AttributeError, message.c_str());
	return nullptr;
}

Py_ssize_t FsdDict_Length(PyObject *selfO)
{
	std::string message = "FsdDict: Could not determine length of FsdDict";
	PyErr_SetString(PyExc_AttributeError, message.c_str());
	return 0;
}

PyObject* FsdDict_Subscript(PyObject* self, PyObject* key)
{
	FsdDict* fsdDict = static_cast<FsdDict*>(self);
	return fsdDict->GetValue(key);
}

int FsdDict_Contains(PyObject *self, PyObject *key)
{
	FsdDict* fsdDict = static_cast<FsdDict*>(self);

	if (fsdDict->Contains(key))
	{
		return 1;
	}
	return 0;
}

FsdDict* CreateFsdDict(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path)
{
	FsdDictOffsetLookup* fsdDictOffset = CreateFsdDictOffsetLookup(data, offset, schemaAttributes);
	FsdDict* fsdDict = static_cast<FsdDict*>(FsdDict_new(&PyFsdDictType, Py_None, Py_None));

	fsdDict->Initialize(data, offset, schemaAttributes, fsdDictOffset, path.c_str());
	return fsdDict;
}
