// Copyright © 2016 CCP ehf.

#pragma once
#ifndef BinaryLoaderHelper_h
#define BinaryLoaderHelper_h

class BinaryLoaderHelper
{
public:
	static std::string GetSubsetOfData(const char* data, uint32_t offset, uint32_t dataSize)
	{
		std::string valueAsString(&data[offset], dataSize);
		return valueAsString;
	}

	static const uint32_t GetUnsignedIntFromData(const char* data, uint32_t offset)
	{
		std::string valueAsString(&data[offset], 4);
		return *reinterpret_cast<const uint32_t*>(valueAsString.c_str());
	}
};

#endif // BinaryLoaderHelper_h