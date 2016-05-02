////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Daniel Speed
// Created:		Feb 2013
// Copyright:	CCP 2013
//

#pragma once
#ifndef FsdUnsignedIntegerKeyMap_h
#define FsdUnsignedIntegerKeyMap_h

#include "BlueExposure/include/BlueExposure.h"
#include <hash_map>

BLUE_DECLARE( FsdUnsignedIntegerKeyMap );

// This is the stored structure in our hashmap
struct OffsetSize {
	unsigned offset;
	unsigned size;
};

typedef stdext::hash_map<unsigned,OffsetSize> UnsignedHashMap;

// FwdDecl
class FsdUnsignedIntegerKeyMapItemsIterator;

/*
	FsdIntegerKeyMap stores offsets and sizes that are needed to read data from disk
	in a hash_map in memory. This is more memory efficient than python, and faster by
	a fairly significant margin.

	At the moment, there's no easy way to make this look like a proper dict in Blue
	but we can accomplish that by using a helper iterator class, and by using python wrappers.

	Internally, the key can actually be signed or unsigned, so long as it is interpreted consistently
*/


BLUE_CLASS( FsdUnsignedIntegerKeyMap ) : 
	public IRoot
{
public:
	EXPOSE_TO_BLUE();

	FsdUnsignedIntegerKeyMap( IRoot* lockobj = nullptr );

	// Initialize the key map with binary data
	void Initialize( const std::string& data );

	// for exposure to python
	size_t length();

	// construct a new iterator object
	Be::Result<bool> GetNewIteratorObject( FsdUnsignedIntegerKeyMapItemsIterator** out );

	//
	void InitializeConstIterators( UnsignedHashMap::const_iterator &start, UnsignedHashMap::const_iterator &end )
	{
		start = m_lookup.cbegin();
		end = m_lookup.cend();
	}

	bool GetItem( unsigned key, OffsetSize& out )
	{
		UnsignedHashMap::const_iterator f = m_lookup.find( key );
		if( f == m_lookup.cend() )
		{
			return false;
		}
		else
		{
			out.offset = f->second.offset;
			out.size = f->second.size;
			return true;
		}
	}

private:
	UnsignedHashMap m_lookup;
};

TYPEDEF_BLUECLASS( FsdUnsignedIntegerKeyMap );

// Iterator for above class

BLUE_CLASS( FsdUnsignedIntegerKeyMapItemsIterator ):
	public IRoot
{
public:
	EXPOSE_TO_BLUE();

	FsdUnsignedIntegerKeyMapItemsIterator( IRoot* lockobj = nullptr );

	void SetParent( FsdUnsignedIntegerKeyMap* p );

	UnsignedHashMap::const_iterator m_iterator;
	UnsignedHashMap::const_iterator m_end;

private:
	FsdUnsignedIntegerKeyMapPtr m_parent;
	
};

TYPEDEF_BLUECLASS( FsdUnsignedIntegerKeyMapItemsIterator );

#endif // FsdUnsignedIntegerKeyMap_h