/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MFieldMap.h
 *  @brief This file contains the class for storing GMSEC Message Fields in
 * either a tree-map (STL map) or an unordered-map (hash map).
 *
**/


#include <gmsec4/internal/MsgFieldMap.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Fields.h>

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/field/InternalField.h>

#include <gmsec4/util/Log.h>


using gmsec::api::util::StringConverter;


namespace gmsec
{
namespace api
{
namespace internal
{

MsgFieldMap::MsgFieldMap(StorageType type, size_t rolloverLimit)
	: m_type(type),
	  m_rolloverLimit(rolloverLimit),
	  m_fieldDisplay(DEFAULT),
	  m_binTreeMap(),
	  m_hashMap(),
	  m_binTreeMapIter(m_binTreeMap.begin()),
	  m_hashMapIter(m_hashMap.begin())
{
}


MsgFieldMap::~MsgFieldMap()
{
	clearFields();
}


MsgFieldMap& MsgFieldMap::operator=(const MsgFieldMap& other)
{
	if (this != &other)
	{
		this->clearFields();

		if (other.m_type == BINARY_TREE_MAP)
		{
			for (BinaryTreeMap::const_iterator it = other.m_binTreeMap.begin(); it != other.m_binTreeMap.end(); ++it)
			{
				this->addField(*it->second);
			}
		}
		else
		{
			for (HashMap::const_iterator it = other.m_hashMap.begin(); it != other.m_hashMap.end(); ++it)
			{
				this->addField(*it->second);
			}
		}
	}

	return *this;
}


void MsgFieldMap::setStorageType(StorageType type)
{
	StorageType oldType = m_type;

	m_type = type;

	if (oldType != type)
	{
		if (oldType == BINARY_TREE_MAP)
		{
			// We're moving to a HASH tree
			m_hashMap.insert(m_binTreeMap.begin(), m_binTreeMap.end());

			m_binTreeMap.clear();
		}
		else
		{
			// We're moving to a BINARY tree
			m_binTreeMap.insert(m_hashMap.begin(), m_hashMap.end());

			m_hashMap.clear();

			// Note: If additional fields are added to a Message,
			// there is no guarantee that a binary tree map will
			// continue to be used. To ensure that a binary tree
			// map is always used, then set the rollover limit
			// to 0 (zero).
		}
	}
}


MsgFieldMap::StorageType MsgFieldMap::getStorageType() const
{
	return m_type;
}


void MsgFieldMap::setRolloverLimit(size_t limit)
{
	m_rolloverLimit = limit;
}


size_t MsgFieldMap::getRolloverLimit() const
{
	return m_rolloverLimit;
}


void MsgFieldMap::setFieldDisplay(FieldDisplay display)
{
	m_fieldDisplay = display;
}


bool MsgFieldMap::addField(const Field& field, bool makeCopy)
{
	const std::string name = field.getName();

	bool replaced = false;

	if (m_type == BINARY_TREE_MAP)
	{
		if (m_rolloverLimit > 0 && (m_binTreeMap.size() + 1) > m_rolloverLimit)
		{
			rollover();
			return addField(field, makeCopy);   // re-call by recursion.
		}
		else
		{
			Field* fieldToStore = (makeCopy ? InternalField::makeFieldCopy(field) : const_cast<Field*>(&field));

			BinaryTreeMap::iterator it = m_binTreeMap.lower_bound(name);

			if (it == m_binTreeMap.end() || m_binTreeMap.key_comp()(name, it->first))
			{
				// insert new entry.
				m_binTreeMap.insert(it, BinaryTreeMap::value_type(name, fieldToStore));
			}
			else
			{
				// existing entry; replace value.
				delete it->second;
				it->second = fieldToStore;
				replaced = true;
			}
		}
	}
	else
	{
		Field* fieldToStore = (makeCopy ? InternalField::makeFieldCopy(field) : const_cast<Field*>(&field));

		HashMap::iterator it = m_hashMap.find(name);

		if (it == m_hashMap.end())
		{
			// insert new entry.
			m_hashMap.insert(HashMap::value_type(name, fieldToStore));
		}
		else
		{
			// existing entry; replace value.
			delete it->second;
			it->second = fieldToStore;
			replaced = true;
		}
	}

	return replaced;
}


const Field* MsgFieldMap::getField(const char* name) const
{
	const Field* field = NULL;

	std::string fieldName = StringConverter::instance().convertString(name);

	if (m_type == BINARY_TREE_MAP)
	{
		BinaryTreeMap::const_iterator it = m_binTreeMap.find(fieldName);

		if (it != m_binTreeMap.end())
		{
			field = it->second;
		}
		else
		{
			GMSEC_DEBUG << "MsgFieldMap::getField(" << name << ") : Field not found";
		}
	}
	else
	{
		HashMap::const_iterator it = m_hashMap.find(fieldName);

		if (it != m_hashMap.end())
		{
			field = it->second;
		}
		else
		{
			GMSEC_DEBUG << "MsgFieldMap::getField(" << name << ") : Field not found";
		}
	}

	return field;
}


void MsgFieldMap::clearFields()
{
	if (m_type == BINARY_TREE_MAP)
	{
		for (BinaryTreeMap::iterator it = m_binTreeMap.begin(); it != m_binTreeMap.end(); ++it)
		{
			delete it->second;
		}
		m_binTreeMap.clear();
	}
	else
	{
		for (HashMap::iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
		{
			delete it->second;
		}
		m_hashMap.clear();
	}
}


bool MsgFieldMap::clearField(const char* name)
{
	bool cleared = false;

	std::string fieldName = StringConverter::instance().convertString(name);

	if (m_type == BINARY_TREE_MAP)
	{
		BinaryTreeMap::iterator it = m_binTreeMap.find(fieldName);

		if (it != m_binTreeMap.end())
		{
			Field* field = it->second;
			m_binTreeMap.erase(it);
			delete field;
			cleared = true;
		}
	}
	else
	{
		HashMap::iterator it = m_hashMap.find(fieldName);

		if (it != m_hashMap.end())
		{
			Field* field = it->second;
			m_hashMap.erase(it);
			delete field;
			cleared = true;
		}
	}

	return cleared;
}


size_t MsgFieldMap::getFieldCount() const
{
	return (m_type == BINARY_TREE_MAP ? m_binTreeMap.size() : m_hashMap.size());
}


void MsgFieldMap::initIterator()
{
	m_binTreeMapIter = m_binTreeMap.begin();
	m_hashMapIter    = m_hashMap.begin();
}


bool MsgFieldMap::hasNext() const
{
	return (m_type == BINARY_TREE_MAP ? (m_binTreeMapIter != m_binTreeMap.end()) : (m_hashMapIter != m_hashMap.end()));
}


const Field& MsgFieldMap::next()
{
	if (!hasNext())
	{
		throw Exception(MSG_ERROR, ITER_INVALID_NEXT, "No more message fields.");
	}

	Field* field = NULL;

	if (m_type == BINARY_TREE_MAP)
	{
		field = m_binTreeMapIter->second;
		++m_binTreeMapIter;
	}
	else
	{
		field = m_hashMapIter->second;
		++m_hashMapIter;
	}

	return *field;
}


const char* MsgFieldMap::toXML() const
{
	std::ostringstream oss;

	if (m_fieldDisplay == DEFAULT)
	{
		if (m_type == BINARY_TREE_MAP)
		{
			for (BinaryTreeMap::const_iterator it = m_binTreeMap.begin(); it != m_binTreeMap.end(); ++it)
			{
				oss << "\t" << it->second->toXML() << "\n";
			}
		}
		else
		{
			for (HashMap::const_iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
			{
				oss << "\t" << it->second->toXML() << "\n";
			}
		}
	}
	else if (m_fieldDisplay == SORTED)
	{
		BinaryTreeMap tmpMap;

		if (m_type == BINARY_TREE_MAP)
		{
			tmpMap.insert(m_binTreeMap.begin(), m_binTreeMap.end());
		}
		else
		{
			tmpMap.insert(m_hashMap.begin(), m_hashMap.end());
		}

		for (BinaryTreeMap::const_iterator it = tmpMap.begin(); it != tmpMap.end(); ++it)
		{
			oss << "\t" << it->second->toXML() << "\n";
		}
	}
	else // UNSORTED
	{
		HashMap tmpMap;

		if (m_type == BINARY_TREE_MAP)
		{
			tmpMap.insert(m_binTreeMap.begin(), m_binTreeMap.end());
		}
		else
		{
			tmpMap.insert(m_hashMap.begin(), m_hashMap.end());
		}

		for (HashMap::const_iterator it = tmpMap.begin(); it != tmpMap.end(); ++it)
		{
			oss << "\t" << it->second->toXML() << "\n";
		}
	}

	m_xml = oss.str();

	return m_xml.c_str();
}


const char* MsgFieldMap::toJSON() const
{
	std::ostringstream oss;

	if (m_fieldDisplay == DEFAULT)
	{
		if (m_type == BINARY_TREE_MAP)
		{
			for (BinaryTreeMap::const_iterator it = m_binTreeMap.begin(); it != m_binTreeMap.end(); ++it)
			{
				oss << it->second->toJSON() << ",";
			}
		}
		else
		{
			for (HashMap::const_iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
			{
				oss << it->second->toJSON() << ",";
			}
		}
	}
	else if (m_fieldDisplay == SORTED)
	{
		BinaryTreeMap tmpMap;

		if (m_type == BINARY_TREE_MAP)
		{
			tmpMap.insert(m_binTreeMap.begin(), m_binTreeMap.end());
		}
		else
		{
			tmpMap.insert(m_hashMap.begin(), m_hashMap.end());
		}

		for (BinaryTreeMap::const_iterator it = tmpMap.begin(); it != tmpMap.end(); ++it)
		{
			oss << it->second->toJSON() << ",";
		}
	}
	else // UNSORTED
	{
		HashMap tmpMap;

		if (m_type == BINARY_TREE_MAP)
		{
			tmpMap.insert(m_binTreeMap.begin(), m_binTreeMap.end());
		}
		else
		{
			tmpMap.insert(m_hashMap.begin(), m_hashMap.end());
		}

		for (HashMap::const_iterator it = tmpMap.begin(); it != tmpMap.end(); ++it)
		{
			oss << it->second->toJSON() << ",";
		}
	}

	// Remove the trailing comma
	m_json = oss.str().substr(0,oss.str().size()-1);

	return m_json.c_str();
}


void MsgFieldMap::rollover()
{
	// we only go one way; from BinaryTreeMap to HashMap.
	if (m_type == BINARY_TREE_MAP)
	{
		m_type = HASH_MAP;

		m_hashMap.insert(m_binTreeMap.begin(), m_binTreeMap.end());

		m_binTreeMap.clear();

		initIterator();
	}
}

} //namespace internal
} //namespace api 
} //namespace gmsec
