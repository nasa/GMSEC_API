/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file MessageFieldMap.cpp
 *  @brief This file contains the class for storing GMSEC Message Fields in
 * either a tree-map (STL map) or an unordered-map (hash map).
 *
**/


#include <gmsec/Field.h>

#include <gmsec/internal/MessageFieldMap.h>
#include <gmsec/internal/BaseField.h>

#include <gmsec/internal/Log.h>


using namespace std;

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::util;


void MessageFieldMap::Rollover()
{
	// we only go one way; from BinaryTreeMap to HashMap.
	if (fType == BINARY_TREE_MAP)
	{
		fType = HASH_MAP;

		fHashMap.insert(fBinTreeMap.begin(), fBinTreeMap.end());

		fBinTreeMap.clear();
	}
}


MessageFieldMap::MessageFieldMap(const StorageType type, const int rolloverLimit)
	:
	fType(type),
	fRolloverLimit(rolloverLimit),
	fBinTreeMap(),
	fHashMap(),
	fBinTreeMapIter(fBinTreeMap.begin()),
	fHashMapIter(fHashMap.begin())
{
}


MessageFieldMap::~MessageFieldMap()
{
    ClearFields();
}


void MessageFieldMap::SetStorageType(StorageType type)
{
	fType = type;
}


MessageFieldMap::StorageType MessageFieldMap::GetStorageType() const
{
	return fType;
}


void MessageFieldMap::SetRolloverLimit(const int limit)
{
	fRolloverLimit = limit;
}


int MessageFieldMap::GetRolloverLimit() const
{
	return fRolloverLimit;
}


Status MessageFieldMap::AddField(const std::string &name, Field &field)
{
	Status result;

	if (fType == BINARY_TREE_MAP)
	{
		if (fRolloverLimit > 0 && int(fBinTreeMap.size() + 1) > fRolloverLimit)
		{
			Rollover();
			return AddField(name, field);	// re-call by recursion.
		}
		else
		{
			BaseField &base = *FieldBuddy::getInternal(&field);

			BinaryTreeMap::iterator it = fBinTreeMap.lower_bound(name);

			if (it == fBinTreeMap.end() || fBinTreeMap.key_comp()(name, it->first))
			{
				// insert new entry.
				fBinTreeMap.insert(it, BinaryTreeMap::value_type(name, new BaseField(base)));
			}
			else
			{
				// existing entry; replace value.
				*(it->second) = base;
			}
		}
	}
	else
	{
		BaseField &base = *FieldBuddy::getInternal(&field);

		HashMap::iterator it = fHashMap.find(name);

		if (it == fHashMap.end())
		{
			// insert new entry.
			fHashMap.insert(HashMap::value_type(name, new BaseField(base)));
		}
		else
		{
			// existing entry; replace value.
			*(it->second) = base;
		}
	}

	return result;
}


Status MessageFieldMap::GetField(const std::string& name, BaseField &field) const
{
	Status result;

	if (fType == BINARY_TREE_MAP)
	{
		BinaryTreeMap::const_iterator it = fBinTreeMap.find(name);
		if (it != fBinTreeMap.end())
		{
			field = *(it->second);
		}
		else
		{
			result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_INVALID_FIELD_NAME, "Field not found.");
			LOG_DEBUG << "MessageFieldMap::GetField(" << name << ") : " << result.Get();
		}
	}
	else
	{
		HashMap::const_iterator it = fHashMap.find(name);
		if (it != fHashMap.end())
		{
			field = *(it->second);
		}
		else
		{
			result.Set(GMSEC_STATUS_FIELD_ERROR, GMSEC_INVALID_FIELD_NAME, "Field not found.");
			LOG_DEBUG << "MessageFieldMap::GetField(" << name << ") : " << result.Get();
		}
	}

	return result;
}


Status MessageFieldMap::ClearFields()
{
	Status result;

	if (fType == BINARY_TREE_MAP)
	{
		for (BinaryTreeMap::iterator it = fBinTreeMap.begin(); it != fBinTreeMap.end(); ++it)
		{
			delete it->second;
		}
		fBinTreeMap.clear();
	}
	else
	{
		for (HashMap::iterator it = fHashMap.begin(); it != fHashMap.end(); ++it)
		{
			delete it->second;
		}
		fHashMap.clear();
	}

	return result;
}


Status MessageFieldMap::ClearField(const std::string &name)
{
	Status result;

	if (fType == BINARY_TREE_MAP)
	{
		BinaryTreeMap::iterator it = fBinTreeMap.find(name);

		if (it != fBinTreeMap.end())
		{
			BaseField *field = it->second;
			fBinTreeMap.erase(it);
			delete field;
		}
	}
	else
	{
		HashMap::iterator it = fHashMap.find(name);

		if (it != fHashMap.end())
		{
			BaseField *field = it->second;
			fHashMap.erase(it);
			delete field;
		}
	}

	return result;
}


size_t MessageFieldMap::GetFieldCount() const
{
	return (fType == BINARY_TREE_MAP ? fBinTreeMap.size() : fHashMap.size());
}


void MessageFieldMap::InitIterator()
{
	fBinTreeMapIter = fBinTreeMap.begin();
	fHashMapIter    = fHashMap.begin();
}


bool MessageFieldMap::HasNext()
{
	return (fType == BINARY_TREE_MAP ? (fBinTreeMapIter != fBinTreeMap.end()) : (fHashMapIter != fHashMap.end()));
}


BaseField* MessageFieldMap::Next()
{
	BaseField *field = NULL;

	if (fType == BINARY_TREE_MAP)
	{
		if (fBinTreeMapIter != fBinTreeMap.end())
		{
			field = fBinTreeMapIter->second;
			++fBinTreeMapIter;
		}
	}
	else
	{
		if (fHashMapIter != fHashMap.end())
		{
			field = fHashMapIter->second;
			++fHashMapIter;
		}
	}

	return field;
}
