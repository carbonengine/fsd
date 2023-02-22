////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		May 2016
// Copyright:	CCP 2016
//

#include "StdAfx.h"
#include "FsdObject.h"


BLUE_DEFINE(FsdObject);

const Be::ClassInfo* FsdObject::ExposeToBlue()
{
	EXPOSURE_BEGIN(FsdObject, "FsdObject, aka an object stored in FSD")
		MAP_METHOD_AND_WRAP("__getattr__", GetAttr, "gets  an attribute by name")
		EXPOSURE_END()
}