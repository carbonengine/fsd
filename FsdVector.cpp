////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "FsdVector.h"

void FsdVector::Initialize(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path)
{
	m_data = &data[offset];
	m_schemaAttributes = schemaAttributes;
	m_path = path;

	switch (m_schemaAttributes.schemaType)
	{
	case FLOAT32_VECTOR2_TUPLE_SCHEMA_TYPE:
		m_vectorType = FSD_VECTOR2;
		m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector2*>(m_data));
		break;
	case FLOAT32_VECTOR3_TUPLE_SCHEMA_TYPE:
		m_vectorType = FSD_VECTOR3;
		m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector3*>(m_data));
		break;
	case FLOAT32_VECTOR4_TUPLE_SCHEMA_TYPE:
		m_vectorType = FSD_VECTOR4;
		m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector4*>(m_data));
		break;
	case DOUBLE_VECTOR2_TUPLE_SCHEMA_TYPE:
		m_vectorType = FSD_VECTOR2D;
		m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector2d*>(m_data));
		break;
	case DOUBLE_VECTOR3_TUPLE_SCHEMA_TYPE:
		m_vectorType = FSD_VECTOR3D;
		m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector3d*>(m_data));
		break;
	case DOUBLE_VECTOR4_TUPLE_SCHEMA_TYPE:
    default:
		m_vectorType = FSD_VECTOR4D;
		m_exposedData = BlueWrapReturnValue(BlueScriptArguments(), *reinterpret_cast<const Vector4d*>(m_data));
		break;
	}
}

PyObject* FsdVector::GetByAttribute(const char* attribute_name)
{
	std::string attributeName = std::string(attribute_name);

	std::map<std::string, uint8_t> aliases = m_schemaAttributes.vectorAttributes->aliases;
	if (aliases.find(attributeName) != aliases.end())
	{
		return GetValueByIndex(aliases[attributeName]);
	}
	if (attributeName == "data")
	{
		return BlueWrapReturnValue(BlueScriptArguments(), m_exposedData);
	}
	std::string message = "Attribute '" + std::string(attributeName) + "' does not exist on this instance";
	PyErr_SetString(PyExc_AttributeError, message.c_str());
	return nullptr;
}

PyObject* FsdVector::GetValueByIndex(uint8_t index)
{
	if (m_vectorType <= FSD_VECTOR4)
	{
		const char* data = &m_data[4 * index];
		float value = *reinterpret_cast<const float*>(data);
		return BlueWrapReturnValue(BlueScriptArguments(), value);
	}

	const char* data = &m_data[4 * index];
	double value = *reinterpret_cast<const double*>(data);
	return BlueWrapReturnValue(BlueScriptArguments(), value);
}

PyObject* FsdVector::GetByIndex(uint8_t index)
{
	uint8_t length = uint8_t(GetLength());
	uint8_t originalIndex = index;

	if (index < 0)
	{
		index += length;
	}
	if (index > length || index < 0)
	{
        PyErr_Format(
            PyExc_IndexError,
            "FsdVector: %s trying to get index %d of a vector that is %d long.",
            m_schemaAttributes.schemaTypeAsString.c_str(),
            int( originalIndex ),
            int( length ) );
		return 0;
	}
	return GetValueByIndex(index);
}

Py_ssize_t FsdVector::GetLength()
{
	switch (m_vectorType)
	{
	case FSD_VECTOR2:
	case FSD_VECTOR2D:
		return 2;
	case FSD_VECTOR3:
	case FSD_VECTOR3D:
		return 3;
	case FSD_VECTOR4:
	case FSD_VECTOR4D:
		return 4;
	default:
		PyErr_Format(PyExc_AttributeError, "pyFSD.FsdVector: Could not determine length of vector with vectorType: %d", int( m_vectorType ) );
		return 0;

	}
}

// The python class functions on this enum type
static PyMethodDef PyFsdVectorType_methods[] = {
	{
		"__dir__",
		(PyCFunction)FsdVector_Dir, METH_NOARGS,
		"Returns a list of the enumeration names for this enum"
	},
	{ NULL, NULL, 0, NULL }
};

static PySequenceMethods fsdVectorSequenceMethods = {
	FsdVector_Length,
	0,
	0,
	FsdVector_GetIndex,
	0,
	0,
	0
};

PyTypeObject PyFsdVectorType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"pyFSD.FsdVector",
	sizeof(FsdVector),
	0,
	FsdVector_dealloc,			/* tp_dealloc */
	0,								/* tp_print */
	FsdVector_GetAttrString,		/* tp_getattr */
	0,								/* tp_setattr        */
	0,								/* tp_compare        */
	0,								/* tp_repr           */
	0,								/* tp_as_number      */
	&fsdVectorSequenceMethods,		/* tp_as_sequence    */
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
	PyFsdVectorType_methods,		/* tp_methods        */
	0,								/* tp_members        */
	0,								/* tp_getset         */
	0,								/* tp_base           */
	0,								/* tp_dict           */
	0,								/* tp_descr_get      */
	0,								/* tp_descr_set      */
	0,								/* tp_dictoffset     */
	(initproc)FsdVector_init,		/* tp_init           */
	0,								// tp_alloc
	FsdVector_new,				/* tp_new */
};

PyObject* FsdVector_Dir(PyObject *self, PyObject * args)
{
	return nullptr;
}

void FsdVector_dealloc(PyObject* self)
{
	PyObject_Del(self);
};

PyObject* FsdVector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	FsdVector *self;
	self = (FsdVector*)type->tp_alloc(type, 0);

	return (PyObject *)self;
}

int FsdVector_init(FsdVector *self, PyObject *args, PyObject *kwds)
{
	PyErr_SetString(PyExc_NotImplementedError, "pyFSD.FsdVector::__init__  cannot initialize from python");
	return -1;
}

PyObject* FsdVector_GetAttrString(PyObject *o, char* attr_name)
{
	FsdVector* fsdVector = static_cast<FsdVector*>(o);
	return fsdVector->GetByAttribute(attr_name);
}

Py_ssize_t FsdVector_Length(PyObject *selfO)
{
	FsdVector* fsdVector = static_cast<FsdVector*>(selfO);
	return fsdVector->GetLength();
}

PyObject *FsdVector_GetIndex(PyObject *selfO, Py_ssize_t i)
{
	FsdVector* fsdVector = static_cast<FsdVector*>(selfO);
	return fsdVector->GetByIndex(uint8_t(i));
}

FsdVector* CreateFsdVector(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes, std::string path)
{
	FsdVector* fsdVector = static_cast<FsdVector*>(FsdVector_new(&PyFsdVectorType, Py_None, Py_None));
	fsdVector->Initialize(data, offset, schemaAttributes, path);
	
	return fsdVector;
}
