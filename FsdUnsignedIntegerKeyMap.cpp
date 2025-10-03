////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Daniel Speed
// Created:		Feb 2013
// Copyright:	CCP 2013
//

#include "StdAfx.h"
#include <iostream>

// This is the stored structure in our hashmap
struct OffsetSize
{
	unsigned offset;
	unsigned size;
};

// used to interpret the binary bytes
struct KeyOffsetSize {
	unsigned key;
	unsigned offset;
	unsigned size;
};

typedef std::unordered_map<unsigned, OffsetSize> UnsignedHashMap;

struct FsdUnsignedIntegerKeyMap : public PyObject {
	PyObject_HEAD
	UnsignedHashMap lookup;
    UnsignedHashMap::const_iterator cur;
};

int FsdUnsignedIntegerKeyMap_init(FsdUnsignedIntegerKeyMap* self, PyObject* args, PyObject* /*unused*/) {
	Py_buffer bytes = {0};
	if(!PyArg_ParseTuple(args, "|y*", &bytes)) {
		return -1;
	}

    if (!bytes.buf) {
        // this is fine, we got initialized without any data, all operations will operate on an empty container
        return 0;
    }

	auto data = (const char*)bytes.buf;
    auto size = bytes.len / sizeof(KeyOffsetSize);

    // Sometimes the datastream is prefixed with some irrelevant data.
    // For example, there is a Python construct called `binaryRepresenter` that is used to generate input data for this
    // type, and that can be prefixed with a 32-bit length value. We therefore need to adjust the start of input data
    // stream.
    data += (bytes.len % sizeof(KeyOffsetSize));

    CCP_ASSERT( PyBuffer_IsContiguous( &bytes, 'A' ) == 1 );

    self->lookup.reserve(size);

	for( size_t i = 0; i < size; i++ )
	{
		auto* castKey = (KeyOffsetSize*)(&data[ i * sizeof(KeyOffsetSize) ]);

		OffsetSize os = {
            castKey->offset,
            castKey->size
        };

		self->lookup[castKey->key] = os;
	}
    self->cur = self->lookup.cbegin();

	return 0;
}

PyObject* FsdUnsignedIntegerKeyMap_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    auto self = new FsdUnsignedIntegerKeyMap;
    self->ob_refcnt = 1;
    self->ob_type = type;
    self->cur = self->lookup.cend();
    return self;
}

Py_ssize_t FsdUnsignedIntegerKeyMap_len( FsdUnsignedIntegerKeyMap* self ) {
    return self->lookup.size();
}

PyObject* FsdUnsignedIntegerKeyMap_length( FsdUnsignedIntegerKeyMap* self )
{
    return Py_BuildValue( "n", FsdUnsignedIntegerKeyMap_len(self) );
}

PyObject* FsdUnsignedIntegerKeyMap_GetItem( FsdUnsignedIntegerKeyMap* self, Py_ssize_t index ) {
    UnsignedHashMap::const_iterator f = self->lookup.find((unsigned) index);
    if (f == self->lookup.cend()) {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return nullptr;
    }
    return Py_BuildValue("(ii)", f->second.offset, f->second.size);
}

PyObject* FsdUnsignedIntegerKeyMap_get( FsdUnsignedIntegerKeyMap* self, PyObject *const *args, Py_ssize_t nargs )
{
    PyObject* i;
    if ( !_PyArg_UnpackStack( args, nargs, "Get", 1, 1, &i ) ) {
        return nullptr;
    }
    return FsdUnsignedIntegerKeyMap_GetItem( self, PyLong_AS_LONG( i ) );
}

PyObject* FsdUnsignedIntegerKeyMap_iternext( FsdUnsignedIntegerKeyMap* self )
{
    if ( self->lookup.cend() == self->cur ) {
        PyErr_SetString(PyExc_StopIteration, "");
		self->cur = self->lookup.cbegin();
        return nullptr;
    }

    auto ret = Py_BuildValue("(i(ii))", self->cur->first, self->cur->second.offset, self->cur->second.size);
    self->cur++;
    return ret;
}

static PySequenceMethods PyFsdUnsignedIntegerKeyMapType_SequenceMethods =
{
    (lenfunc)FsdUnsignedIntegerKeyMap_len,
    nullptr,
    nullptr,
    (ssizeargfunc)FsdUnsignedIntegerKeyMap_GetItem,
    nullptr
};

static PyMethodDef PyFsdUnsignedIntegerKeyMapType_Methods[] = {
    {"length", (PyCFunction)FsdUnsignedIntegerKeyMap_length, METH_NOARGS, nullptr},
    {"Get", (PyCFunction)FsdUnsignedIntegerKeyMap_get, METH_FASTCALL, nullptr },
    {nullptr, nullptr, 0, nullptr}
};

PyTypeObject PyFsdUnsignedIntegerKeyMapType
{
	PyVarObject_HEAD_INIT( nullptr, 0 )
	"pyFSD.FsdUnsignedIntegerKeyMap",
	sizeof( FsdUnsignedIntegerKeyMap ),
	0,
	0, /* tp_dealloc */
	0, /* tp_print */
	0, /* tp_getattr */
	0, /* tp_setattr        */
	0, /* tp_as_async       */
	0, /* tp_repr           */
	0, /* tp_as_number      */
    &PyFsdUnsignedIntegerKeyMapType_SequenceMethods,
	0, /* tp_as_mapping     */
	0, /* tp_hash           */
	0, /* tp_call           */
	0, /* tp_str            */
	0, /* tp_getattro       */
	0, /* tp_setattro       */
	0, /* tp_as_buffer      */
	Py_TPFLAGS_DEFAULT, /* tp_flags          */
	0, /* tp_doc            */
	0, /* tp_traverse       */
	0, /* tp_clear          */
	0, /* tp_richcompare    */
	0, /* tp_weaklistoffset */
	PyObject_SelfIter, /* tp_iter           */
    (iternextfunc)FsdUnsignedIntegerKeyMap_iternext, /* tp_iternext       */
	PyFsdUnsignedIntegerKeyMapType_Methods, /* tp_methods        */
	0, /* tp_members        */
	0, /* tp_getset         */
	0, /* tp_base           */
	0, /* tp_dict           */
	0, /* tp_descr_get      */
	0, /* tp_descr_set      */
	0, /* tp_dictoffset     */
    (initproc)FsdUnsignedIntegerKeyMap_init, /* tp_init           */
	0, /* tp_alloc */
	FsdUnsignedIntegerKeyMap_new /* tp_new */
};
