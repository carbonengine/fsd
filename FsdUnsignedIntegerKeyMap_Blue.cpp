////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Daniel Speed
// Created:		Feb 2013
// Copyright:	CCP 2013
//

#include "StdAfx.h"
#include "FsdUnsignedIntegerKeyMap.h"

BLUE_DEFINE( FsdUnsignedIntegerKeyMap );
BLUE_DEFINE( FsdUnsignedIntegerKeyMapItemsIterator );

static PyObject* PyGetItem( PyObject* self, PyObject* args )
{
	FsdUnsignedIntegerKeyMap* pThis = BluePythonCast<FsdUnsignedIntegerKeyMap*>( self );

	unsigned key;
	if( !PyArg_ParseTuple( args, "I", &key ) )
	{
		return nullptr;
	}

	OffsetSize out;

	if( pThis->GetItem( key, out ) )
	{
		PyObject* v = Py_BuildValue(
				"(ii)", 
				out.offset, 
				out.size
			);

		return v;
	}
	else
	{
		PyErr_SetNone( PyExc_IndexError );
		return nullptr;
	}
}


const Be::ClassInfo* FsdUnsignedIntegerKeyMap::ExposeToBlue()
{
	EXPOSURE_BEGIN( FsdUnsignedIntegerKeyMap, "FsdUnsignedIntegerKeyMap is used to store offsets and sizes corresponding to an integer key in a hashmap" )
		MAP_METHOD_AND_WRAP( "Initialize", Initialize, "Initializes the key map with data." )
		MAP_METHOD_AND_WRAP( "iteritems", GetNewIteratorObject, "Gets an iterator object for this class" )
		MAP_METHOD_AND_WRAP( "length", length, "__len__" )
		MAP_METHOD( "Get", PyGetItem, "equivalent to __next__()" )
	EXPOSURE_END()
}


static PyObject* IteratorNext( PyObject* self, PyObject* args )
{
	FsdUnsignedIntegerKeyMapItemsIterator* pThis = BluePythonCast<FsdUnsignedIntegerKeyMapItemsIterator*>( self );

	if( pThis->m_iterator != pThis->m_end )
	{
		PyObject* v = Py_BuildValue(
						"(i(ii))", 
						pThis->m_iterator->first, 
						pThis->m_iterator->second.offset, 
						pThis->m_iterator->second.size
					);

		pThis->m_iterator++;

		return v;
	}
	else
	{
		PyErr_SetNone( PyExc_StopIteration );
		return nullptr;
	}
}


const Be::ClassInfo* FsdUnsignedIntegerKeyMapItemsIterator::ExposeToBlue()
{
	EXPOSURE_BEGIN( FsdUnsignedIntegerKeyMapItemsIterator, "An items iterator for FsdUnsignedIntegerKeyMap" )
		MAP_METHOD( "next", IteratorNext, "equivalent to __next__()" )
	EXPOSURE_END()
}
