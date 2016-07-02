////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "FsdVector.h"

static PyObject* PyGetItem(PyObject* self, PyObject* args)
{
	FsdVector* pThis = BluePythonCast<FsdVector*>(self);

	uint32_t key;
	if (!PyArg_ParseTuple(args, "I", &key))
	{
		return nullptr;
	}
	PyObject* out;
	if (pThis->GetItem(key, out))
	{
		return out;
	}
	else
	{
		PyErr_SetNone(PyExc_IndexError);
		return nullptr;
	}
}

BLUE_DEFINE(FsdVector);

const Be::ClassInfo* FsdVector::ExposeToBlue()
{
	EXPOSURE_BEGIN(FsdVector, "FsdVector, aka a vector stored in FSD")
		MAP_METHOD_AND_WRAP("__getattr__", GetAttr, "gets  an attribute by alias")
		MAP_ATTRIBUTE("data", m_exposedData, "The vector as a tuple", Be::READ)
		MAP_METHOD("__getitem__", PyGetItem, "gets  an attribute by index")
		EXPOSURE_END()
}