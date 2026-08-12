// Copyright © 2016 CCP ehf.

#include "StdAfx.h"
#include "BinaryLoader.h"

static PyObject* LoadBinaryFromString(const char* binaryData, uint32_t offset, const FsdSchemaAttributes& schemaAttributes, const char* path)
{	
	return BinaryLoader::LoadBinaryFromString(binaryData, offset, schemaAttributes, path);
}

MAP_FUNCTION_AND_WRAP("LoadBinaryFromString", LoadBinaryFromString, "Loads binary fsd data and returns the expected python object");