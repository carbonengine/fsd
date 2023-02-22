////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Daniel Speed
// Created:		Feb 2013
// Copyright:	CCP 2013
//

#include "StdAfx.h"
#include "FsdUnsignedIntegerKeyMap.h"
#include <iostream>

FsdUnsignedIntegerKeyMap::FsdUnsignedIntegerKeyMap( IRoot* lockobj )
{
}

// used to interpret the binary bytes
struct KeyOffsetSize {
	unsigned key;
	unsigned offset;
	unsigned size;
};

typedef std::pair<unsigned, OffsetSize> KeyOffsetSizeItem;

void FsdUnsignedIntegerKeyMap::Initialize( const std::string& data ) {
	size_t size = (data.length() - 4) / sizeof(KeyOffsetSize);
	
	for( size_t i = 0; i < size; i++ )
	{
		KeyOffsetSize* castKey = (KeyOffsetSize*)(&data[ (i * sizeof(KeyOffsetSize)) + 4 ]);

		OffsetSize os;
		os.offset = castKey->offset;
		os.size = castKey->size;

		m_lookup.insert( 
			KeyOffsetSizeItem( castKey->key, os )
		);
	}
}

Be::Result<bool> FsdUnsignedIntegerKeyMap::GetNewIteratorObject( FsdUnsignedIntegerKeyMapItemsIterator** out )
{
	FsdUnsignedIntegerKeyMapItemsIteratorPtr newIt;
	newIt.CreateInstance();

	if( !newIt )
	{
		*out = nullptr;
		return Be::Result<bool>( false );
	}

	newIt->SetParent( this );
	*out = newIt.Detach();
	return Be::Result<bool>( true );
}

size_t FsdUnsignedIntegerKeyMap::length() 
{
	return m_lookup.size();
}


FsdUnsignedIntegerKeyMapItemsIterator::FsdUnsignedIntegerKeyMapItemsIterator( IRoot* lockobj )
{
}

void FsdUnsignedIntegerKeyMapItemsIterator::SetParent( FsdUnsignedIntegerKeyMap* p ) 
{
	// just for reference counting. Make sure the dict stays alive underneath us.
	m_parent = p;
	p->InitializeConstIterators( m_iterator, m_end );
}
