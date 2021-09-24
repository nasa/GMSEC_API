/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file MessageFieldMap.h
 *  @brief This file contains the class for storing GMSEC Message Fields in
 * either a tree-map (STL map) or an unordered-map (hash map).
 *
**/


#ifndef gmsec_internal_MessageFieldMap_h
#define gmsec_internal_MessageFieldMap_h

#include <gmsec_defs.h>
#include <gmsec/Status.h>
#include <gmsec/internal/BaseField.h>

#include <map>
#include <string>
#include <cstddef>

#if defined(_MSC_VER)
	#if _MSC_VER >= 1700
		#include <unordered_map>
		typedef std::unordered_map<std::string, gmsec::internal::BaseField*> HashMap;
	#else
		#include <hash_map>
		typedef stdext::hash_map<std::string, gmsec::internal::BaseField*> HashMap;
	#endif

#elif defined(__sun)
	#if defined (__sparc)
		typedef std::map<std::string, gmsec::internal::BaseField*> HashMap;
	#else
		#include <hash_map>
		typedef std::hash_map<std::string, gmsec::internal::BaseField*> HashMap;
	#endif

#elif defined(__APPLE__)
        #include <unordered_map>
	typedef std::unordered_map<std::string, gmsec::internal::BaseField*> HashMap;

#else 
        #include <tr1/unordered_map>
        typedef std::tr1::unordered_map<std::string, gmsec::internal::BaseField*> HashMap;
#endif

typedef std::map<std::string, gmsec::internal::BaseField*> BinaryTreeMap;


namespace gmsec
{
namespace internal
{

/** @class MessageFieldMap
 * @brief The MessageFieldMap class provides a simple wrapper implementation
 * around the STL map and non-standard Hash Map (unordered map).
 *
*/

class GMSEC_API MessageFieldMap
{
public:

	enum StorageType { BINARY_TREE_MAP, HASH_MAP };

	/** @fn MessageFieldMap(const StorageType type, const int rolloverLimit)
	 * @brief Default constructor - initializes the MessageFieldMap.
	 *
	 * @param type - the type of storage container to rely on to store Fields.
	 * @param rolloverLimit - number of Fields that can be stored before rolling over, if necessary, to a HashMap.
	 */
	MessageFieldMap(const StorageType type, const int rolloverLimit);


	/** @fn ~MessageFieldMap()
	 * @brief Destructor - cleans up the MessageFieldMap of its contents.
	 */
	virtual ~MessageFieldMap();


	/** @fn SetStorageType(StorageType type)
	 * @brief Accessor to set the storage type employed by this object.
	 *
	 * @param type - the type of storage container to rely on to store Fields.
	 */
	void CALL_TYPE SetStorageType(StorageType type);


	/** @fn GetStorageType()
	 * @brief Accessor to get the storage type employed by this object.
	 *
	 * @return StorageType enum value.
	 */
	StorageType CALL_TYPE GetStorageType() const;


	/** @fn SetRolloverLimit(const int limit)
	 * @brief Accessor to set the maximum size allowed for Binary Tree Map before
	 * it is converted to a Hash Map.  If set to zero (or less) rollover never occurs.
	 *
	 * @param limit - the maximum size for the Binary Tree Map.
	 */
	void CALL_TYPE SetRolloverLimit(const int limit);


	/** @fn GetRolloverLimit()
	 * @brief Accessor to get the maximum size allowed for Binary Tree Map.
	 *
	 * @return an integer value.
	 */
	int CALL_TYPE GetRolloverLimit() const;


	/** @fn AddField(const std::string &name, Field &value)
	 * @brief Adds the name/field pair to the internal storage map.
	 *
	 * @param name - the name to associate with the field value.
	 * @param value - the field value.
	 *
	 * @return status - the result of the operation.
	 */
	Status CALL_TYPE AddField(const std::string &name, Field &value);


	/** @fn GetField(const std::string &name, BaseField &field)
	 * @brief If found, returns the field associated with the given name.
	 *
	 * @param name - the name to associate with the field value.
	 * @param value - the field value.
	 *
	 * @return status - the result of the operation.
	 */
	Status CALL_TYPE GetField(const std::string &name, BaseField &field) const;


	/** @fn ClearFields()
	 * @brief Removes all fields from the internal storage map.
	 *
	 * @return status - always returns success.
	 */
	Status CALL_TYPE ClearFields();


	/** @fn ClearField(const std::string &name)
	 * @brief Removes the field from the internal storage map that is associated
	 * with the given field name.
	 *
	 * @return status - the result of the operation.
	 */
	Status CALL_TYPE ClearField(const std::string &name);


	/** @fn GetFieldCount()
	 * @brief Returns the number of fields stored in the internal storage map.
	 *
	 * @return size - number that is 0 or more.
	 */
	size_t CALL_TYPE GetFieldCount() const;


	/** @fn InitIterator()
	 * @brief Initializes the iterator for traversing the internal storage map.
	 */
	void CALL_TYPE InitIterator();


	/** @fn HasNext()
	 * @brief Determines if there are additional elements that have yet to be
	 * iterated over in the internal storage map.
	 *
	 * @note This method is not declared const so as to avoid an compiler error with Sun SPARC.
	 *
	 * @return true if other elements are available; false otherwise.
	 */
	bool CALL_TYPE HasNext();


	/** @fn Next()
	 * @brief Returns the field at the current iterated position of the internal
	 * storage map.
	 *
	 * @return Field object.
	 */
	BaseField* CALL_TYPE Next();

private:

	/** @fn Rollover()
	 * @brief Copies all elements from a Binary Tree Map to a Hash Map.
	 */
	void CALL_TYPE Rollover();


	// Declared, but not implemented.
	MessageFieldMap &operator=(const MessageFieldMap &);
	MessageFieldMap(const MessageFieldMap &);

	// Member data
	StorageType   fType;
	int           fRolloverLimit;
	BinaryTreeMap fBinTreeMap;
	HashMap       fHashMap;

	// Iterators
	BinaryTreeMap::iterator fBinTreeMapIter;
	HashMap::iterator       fHashMapIter;

};

} //namespace internal
} //namespace gmsec


#endif	// gmsec_internal_MessageFieldMap_h
