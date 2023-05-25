#include "StdAfx.h"
#include "FsdDictOffsetLookup.h"
#include "BinaryLoaderHelper.h"
#include "BinaryLoader.h"

template<typename T>
struct KeyedFooterOffset
{
	T key;
	uint32_t offset;
};

template<typename T>
struct KeyedFooterOffsetWithSize
{
	T key;
	uint32_t offset;
	uint32_t size;
};

template<typename KEY_TYPE> 
FastLoadingHashedOffsetLookup<KEY_TYPE>::FastLoadingHashedOffsetLookup(const char* footerData, const char* keyConverter, bool hasSize)
{
	m_keyConverter = keyConverter;

	uint32_t dictLength = BinaryLoaderHelper::GetUnsignedIntFromData(footerData, 0);

	if (hasSize)
	{
		for (size_t i = 0; i < dictLength; i++)
		{
			// Need to do a c style cast 
			KeyedFooterOffsetWithSize<KEY_TYPE>* castKey = (KeyedFooterOffsetWithSize<KEY_TYPE>*)(&footerData[(i * sizeof(KeyedFooterOffsetWithSize<KEY_TYPE>) + 4)]);
			FooterOffsetSize fos;
			fos.offset = castKey->offset;
			fos.size = castKey->size;
			m_hash[castKey->key] = fos;
		}
	}
	else
	{
		for (size_t i = 0; i < dictLength; i++)
		{
			KeyedFooterOffset<KEY_TYPE>* castKey = (KeyedFooterOffset<KEY_TYPE>*)(&footerData[(i * sizeof(KeyedFooterOffset<KEY_TYPE>) + 4)]);
			FooterOffsetSize fos;
			fos.offset = castKey->offset;
			fos.size = -1;
			m_hash[castKey->key] = fos;
		}
	}
}

template<typename KEY_TYPE> 
FastLoadingHashedOffsetLookup<KEY_TYPE>::~FastLoadingHashedOffsetLookup()
{

}

template<typename KEY_TYPE>
OFFSET_LOOKUP_RESULT FastLoadingHashedOffsetLookup<KEY_TYPE>::GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos)
{
	KEY_TYPE convertedKey(PyLong_AsLong(key));
	
	if (PyErr_Occurred() != nullptr)
	{
		return OFFSET_LOOKUP_ERROR_INVALID_KEY;
	}

	auto findResult = m_hash.find(convertedKey);
	if (findResult == m_hash.cend()){
		return OFFSET_LOOKUP_ERROR_KEY_NOT_PRESENT;
	}
	fos = findResult->second;
	return OFFSET_LOOKUP_OK;
}

template<typename KEY_TYPE>
bool FastLoadingHashedOffsetLookup<KEY_TYPE>::Contains(PyObject* key)
{
	KEY_TYPE convertedKey( PyLong_AsLong(key) );

	// We go in here if the key isn't an int
	if (convertedKey == -1 && PyErr_Occurred() != nullptr)
	{
		return false;
	}

	auto findResult = m_hash.find(convertedKey);
	// We go in here if the key wasn't found in the hashmap
	if (findResult == m_hash.cend()){
		return false;
	}

	// Success! We found it
	return true;
}

SemiFastStringLoadingHashedOffsetLookup::SemiFastStringLoadingHashedOffsetLookup(const char* footerData, const FsdSchemaAttributes &offsetTableSchema, bool hasSize)
{
	ObjectSchemaAttributes keyOffsetObject = *offsetTableSchema.listAttributes->listItemSchema->objectAttributes.get();
	
	auto offsetToKey = keyOffsetObject.endOfFixedSizedData + 12;
	auto offsetToOffset = keyOffsetObject.constantAttributeOffsets["offset"];
	uint32_t offsetToSize = 0;
	if (hasSize){
		offsetToSize = keyOffsetObject.constantAttributeOffsets["size"];
	}

	uint32_t dictLength = BinaryLoaderHelper::GetUnsignedIntFromData(footerData, 0);
	std::string dataAsString(footerData);
	uint32_t offsetToStartOfData = 4 + dictLength * 4;
	for (size_t i = 0; i < dictLength; i++)
	{
		// footer data is a  list that doesn't have a fixed size
		// so firstly we need to find the offset to the object in the list		
		std::string key = BinaryLoader::CLoadStringFromBinaryString(footerData, offsetToStartOfData + offsetToKey, "");

		uint32_t offset = BinaryLoader::CLoadUnsigned32BitIntFromBinaryString(footerData, offsetToStartOfData + offsetToOffset, "");
		int32_t size = -1;
		
		if (hasSize)
		{
			size = BinaryLoader::CLoadSigned32BitIntFromBinaryString(footerData, offsetToStartOfData + offsetToSize, "");
		}
		
		FooterOffsetSize fos;
		fos.offset = offset;
		fos.size = size;
		m_hash[key] = fos;
		offsetToStartOfData += offsetToKey + 4 + uint32_t(key.length());
	}	
}

SemiFastStringLoadingHashedOffsetLookup::~SemiFastStringLoadingHashedOffsetLookup()
{

}

OFFSET_LOOKUP_RESULT SemiFastStringLoadingHashedOffsetLookup::GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos)
{
	std::string convertedKey = PyUnicode_AsUTF8(key);

	if (PyErr_Occurred() != nullptr)
	{
		return OFFSET_LOOKUP_ERROR_INVALID_KEY;
	}

	auto findResult = m_hash.find(convertedKey);
	if (findResult == m_hash.cend()){
		return OFFSET_LOOKUP_ERROR_KEY_NOT_PRESENT;
	}
	fos = findResult->second;
	return OFFSET_LOOKUP_OK;
}

bool SemiFastStringLoadingHashedOffsetLookup::Contains(PyObject* key)
{
	const char* keyAsCharArray = PyUnicode_AsUTF8(key);
	
	// We go in here if the key isn't an int
	if (keyAsCharArray == nullptr && PyErr_Occurred() != nullptr)
	{
		return false;
	}
	std::string convertedKey = std::string(keyAsCharArray);

	auto findResult = m_hash.find(convertedKey);
	// We go in here if the key wasn't found in the hashmap
	if (findResult == m_hash.cend()){
		return false;
	}

	// Success! We found it
	return true;
}


SlowBinarySearchOffsetLookup::SlowBinarySearchOffsetLookup(const char* footerData, const FsdSchemaAttributes &s, bool hasSize)
{
	m_list = CreateFsdList(footerData, 0, s, "offsetFooter");
}

SlowBinarySearchOffsetLookup::~SlowBinarySearchOffsetLookup()
{
	delete m_list;
}

OFFSET_LOOKUP_RESULT SlowBinarySearchOffsetLookup::GetOffsetAndSize(PyObject* key, FooterOffsetSize &fos)
{
	return OFFSET_LOOKUP_ERROR_INVALID_KEY;
}

bool SlowBinarySearchOffsetLookup::Contains(PyObject* key)
{

	return false;
}

FsdDictOffsetLookup* CreateFsdDictOffsetLookup(const char* data, uint32_t offset, const FsdSchemaAttributes &schemaAttributes)
{
	SchemaType dictKeySchemaType = schemaAttributes.dictAttributes->keySchema->schemaType;

	// the size of the data is situated at the top of the dictionary 
	uint32_t totalDataSize = BinaryLoaderHelper::GetUnsignedIntFromData(data, offset);
	// the footer size is situated at the end of the data 
	// (offset + size of the data header (4) + size of the data (m_totalDataSize) - the size of the footer (4))
	uint32_t footerSize = BinaryLoaderHelper::GetUnsignedIntFromData(data, offset + 4 + totalDataSize - 4);

	// The keys are held in a key footer that contains the keys and the offsets (and size if known) to the data
	std::string footerData(&data[offset + totalDataSize - footerSize], footerSize);

	auto offsetSizeObjectSchema = schemaAttributes.dictAttributes->keyFooterSchema->listAttributes->listItemSchema;
	auto objectAttributeNames = offsetSizeObjectSchema->objectAttributes->attributes;
	bool footerOffsetHasSize = objectAttributeNames.find("size") != objectAttributeNames.end();

	switch (dictKeySchemaType)
	{
	case UNSIGNED_32_BIT_INT_SCHEMA_TYPE:
		return new FastLoadingHashedOffsetLookup<uint32_t>(footerData.c_str(), "I", footerOffsetHasSize);
	case SIGNED_32_BIT_INT_SCHEMA_TYPE:
		return new FastLoadingHashedOffsetLookup<int32_t>(footerData.c_str(), "i", footerOffsetHasSize);
	case STRING_SCHEMA_TYPE:
		return new SemiFastStringLoadingHashedOffsetLookup(footerData.c_str(), *schemaAttributes.dictAttributes->keyFooterSchema, footerOffsetHasSize);
	default:
		return new SlowBinarySearchOffsetLookup(footerData.c_str(), *schemaAttributes.dictAttributes->keyFooterSchema, footerOffsetHasSize);
	};

	return nullptr;
}
