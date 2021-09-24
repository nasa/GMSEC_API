/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file MsgFieldMap.h
 *
 * @brief This file contains the class for storing GMSEC Message Fields in
 * either a tree-map (STL map) or an unordered-map (hash map).
 */


#ifndef GMSEC_INTERNAL_MSGFIELDMAP_H
#define GMSEC_INTERNAL_MSGFIELDMAP_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/internal/ci_less.h>

#include <map>
#include <string>
#include <cstddef>


#if defined(_MSC_VER)
	#if _MSC_VER >= 1700
		#include <unordered_map>
	#else
		#include <hash_map>
	#endif

#elif defined(__sun)
	#if !defined (__sparc)
		#include <hash_map>
	#endif

#elif defined(__APPLE__)
        #include <unordered_map>

#else 
	#include <tr1/unordered_map>
#endif


namespace gmsec
{
namespace api
{
	class Field;

namespace internal
{


/*
 * @class MsgFieldMap
 * @brief The MsgFieldMap class provides a simple wrapper implementation
 * around the STL map and non-standard Hash Map (unordered map).
*/

class GMSEC_API MsgFieldMap
{
public:
	/*
	 * @enum StorageType
	 * @brief The storage type that the MsgFieldMap should employ.
	 */
	enum StorageType
	{
		BINARY_TREE_MAP,   ///< Binary Tree Map (e.g. std::map)
		HASH_MAP           ///< Hash Map (e.g. unordered map)
	};


	/*
	 * @enum FieldDisplay
	 * @brief Indicator for how message fields ought to be displayed
	 * when toXML() or toJSON() is called.
	 */
	enum FieldDisplay
	{
		DEFAULT,
		SORTED,
		UNSORTED
	};


	/*
	 * @fn MsgFieldMap(const StorageType type, const int rolloverLimit)
	 * @brief Default constructor - initializes the MessageFieldMap.
	 *
	 * @param type - the type of storage container to rely on to store Fields.
	 * @param rolloverLimit - number of Fields that can be stored before rolling over, if necessary, to a HashMap.
	 */
	MsgFieldMap(StorageType type, size_t rolloverLimit);


	/*
	 * @fn ~MsgFieldMap()
	 * @brief Destructor - cleans up the MsgFieldMap of its contents.
	 */
	virtual ~MsgFieldMap();


	/*
	 * @fn MsgFieldMap& operator=(const MsgFieldMap& other)
	 * @brief Makes a copy of other MsgFieldMap.
	 * @return Reference to the MsgFieldMap containing copied Fields.
	 */
	MsgFieldMap& CALL_TYPE operator=(const MsgFieldMap& other);


	/*
	 * @fn setStorageType(StorageType type)
	 * @brief Accessor to set the storage type employed by this object.
	 *
	 * @param type - the type of storage container to rely on to store Fields.
	 */
	void CALL_TYPE setStorageType(StorageType type);


	/*
	 * @fn StorageType getStorageType() const
	 * @brief Accessor to get the storage type employed by this object.
	 *
	 * @return StorageType enum value.
	 */
	StorageType CALL_TYPE getStorageType() const;


	/*
	 * @fn setRolloverLimit(int limit)
	 * @brief Accessor to set the maximum size allowed for Binary Tree Map before
	 * it is converted to a Hash Map.  If set to zero (or less) rollover never occurs.
	 *
	 * @param limit - the maximum size for the Binary Tree Map.
	 */
	void CALL_TYPE setRolloverLimit(size_t limit);


	/*
	 * @fn size_t getRolloverLimit() const
	 * @brief Accessor to get the maximum size allowed for Binary Tree Map.
	 *
	 * @return an integer value.
	 */
	size_t CALL_TYPE getRolloverLimit() const;


	/**
	 * @fn void setFieldDisplay(FieldDisplay display)
	 * @brief Set how fields will be displayed when toXML() or toJSON() is called.
	 */
	void CALL_TYPE setFieldDisplay(FieldDisplay display);


	/*
	 * @fn addField(const Field& field, bool makeCopy = true)
	 *
	 * @brief Adds the field object to the internal storage map.
	 *
	 * @param field    - the field object.
	 * @param makeCopy - flag to indicate whether to make a copy of the field.
	 *
	 * @returns True if the Field was replaced; false otherwise.
	 */
	bool CALL_TYPE addField(const Field& field, bool makeCopy = true);


	/*
	 * @fn const Field* getField(const char* name) const
	 * @brief If found, returns the field associated with the given name.
	 *
	 * @param name - the name to associate with the field value.
	 * @return The Field object (if found).
	 * @throw Exception if field is not found.
	 */
	const Field* CALL_TYPE getField(const char* name) const;


	/*
	 * @fn clearFields()
	 * @brief Removes all fields from the internal storage map.
	 */
	void CALL_TYPE clearFields();


	/*
	 * @fn bool clearField(const char* name)
	 * @brief Removes the field from the internal storage map that is associated
	 * with the given field name.
	 *
	 * @return Returns true if the field was removed; false otherwise.
	 */
	bool CALL_TYPE clearField(const char* name);


	/** @fn size_t getFieldCount() const
	 * @brief Returns the number of fields stored in the internal storage map.
	 *
	 * @return size - number that is 0 or more.
	 */
	size_t CALL_TYPE getFieldCount() const;


	/** @fn initIterator()
	 * @brief Initializes the iterator for traversing the internal storage map.
	 */
	void CALL_TYPE initIterator();


	/** @fn bool hasNext() const
	 * @brief Determines if there are additional elements that have yet to be
	 * iterated over in the internal storage map.
	 *
	 * @note This method is not declared const so as to avoid an compiler error with Sun SPARC.
	 *
	 * @return true if other elements are available; false otherwise.
	 */
	bool CALL_TYPE hasNext() const;


	/** @fn const Field& next()
	 * @brief Returns the field at the current iterated position of the internal
	 * storage map.
	 *
	 * @return Field object.
	 * @throw An exception is thrown if there are no more iterable fields.
	 */
	const Field& CALL_TYPE next();


	/*
	 * const char* toXML() const
	 * @brief Returns the XML representation of all Fields stored in the MsgFieldMap.
	 * @returns An XML string.
	 */
	const char* CALL_TYPE toXML() const;


	/*
	 * const char* toJSON() const
	 * @brief Returns the JSON representation of all Fields stored in the MsgFieldMap.
	 * @returns An JSON string.
	 */
	const char* CALL_TYPE toJSON() const;

private:
	/** @fn rollover()
	 * @brief Copies all elements from a Binary Tree Map to a Hash Map.
	 */
	void CALL_TYPE rollover();

	// Declared, but not implemented.
	MsgFieldMap(const MsgFieldMap&);


#if defined(_MSC_VER)
	#if _MSC_VER >= 1700
	typedef std::unordered_map<std::string, gmsec::api::Field*> HashMap;
	#else
	typedef stdext::hash_map<std::string, gmsec::api::Field*> HashMap;
	#endif

#elif defined(__sun)
	#if defined (__sparc)
	typedef std::map<std::string, gmsec::api::Field*> HashMap;
	#else
	typedef std::hash_map<std::string, gmsec::api::Field*> HashMap;
	#endif

#elif defined(__APPLE__)
        typedef std::unordered_map<std::string, gmsec::api::Field*> HashMap;

#else 
	typedef std::tr1::unordered_map<std::string, gmsec::api::Field*> HashMap;
#endif

	typedef std::map<std::string, gmsec::api::Field*> BinaryTreeMap;

	// Member data
	StorageType   m_type;
	size_t        m_rolloverLimit;
	FieldDisplay  m_fieldDisplay;
	BinaryTreeMap m_binTreeMap;
	HashMap       m_hashMap;

	BinaryTreeMap::const_iterator m_binTreeMapIter;
	HashMap::const_iterator m_hashMapIter;

	mutable std::string m_xml;
	mutable std::string m_json;
};

} //namespace internal
} //namespace api 
} //namespace gmsec

#endif
