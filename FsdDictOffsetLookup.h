////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Olafur Thor Gunnarsson
// Created:		August 2016
// Copyright:	CCP 2016
//

#pragma once
#ifndef FsdDictOffsetLookup_h
#define FsdDictOffsetLookup_h

#include "StdAfx.h"
#include "FsdSchema.h"
#include "FsdList.h"

struct FooterOffsetSize {

	uint32_t offset;
	int32_t size;
};

enum OFFSET_LOOKUP_RESULT {
	OFFSET_LOOKUP_OK,
	OFFSET_LOOKUP_ERROR_KEY_NOT_PRESENT,
	OFFSET_LOOKUP_ERROR_INVALID_KEY,
	OFFSET_LOOKUP_ERROR_NOT_IMPLEMENTED
};

struct FsdDictOffsetLookup
{
public:
	virtual ~FsdDictOffsetLookup()
	{
	}

	virtual OFFSET_LOOKUP_RESULT GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos) = 0;
	virtual bool Contains(PyObject* key) = 0;
};

/*
	This is used for signed and unsigned integers, they can be quickly loaded by using casting
*/
template<typename KEY_TYPE>
class FastLoadingHashedOffsetLookup : public FsdDictOffsetLookup
{
public:
	FastLoadingHashedOffsetLookup<KEY_TYPE>(const char* data, const char* keyConverter, bool hasSize);
	~FastLoadingHashedOffsetLookup<KEY_TYPE>();
	
	OFFSET_LOOKUP_RESULT GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos) override;
	bool Contains(PyObject* key) override;
private:
	std::unordered_map<KEY_TYPE, FooterOffsetSize> m_hash;
	const char* m_keyConverter;
};


/*
	This is used for string keys that can't be quickly read from the binary data
	(we need to use the binaryloader to figure out what the key is)
*/
class SemiFastStringLoadingHashedOffsetLookup : public FsdDictOffsetLookup
{
public:
	SemiFastStringLoadingHashedOffsetLookup(const char* data, const FsdSchemaAttributes &s, bool hasSize);
	~SemiFastStringLoadingHashedOffsetLookup();

	OFFSET_LOOKUP_RESULT GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos) override;
	bool Contains(PyObject* key) override;
private:
	std::unordered_map<std::string, FooterOffsetSize> m_hash;
};


class SlowBinarySearchOffsetLookup : public FsdDictOffsetLookup
{
public:
	SlowBinarySearchOffsetLookup(const char* data, const FsdSchemaAttributes &s, bool hasSize);
	~SlowBinarySearchOffsetLookup();

	OFFSET_LOOKUP_RESULT GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos) override;
	bool Contains(PyObject* key) override;
private:
	FsdList* m_list;
};

FsdDictOffsetLookup* CreateFsdDictOffsetLookup(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes);

#endif
