
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file FieldIterator.h
 *
 *  @brief This file contains the external message field iterator.
 *
**/


#ifndef gmsec_FieldIterator_h
#define gmsec_FieldIterator_h


#include <gmsec/Field.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

namespace internal
{
class AbstractFieldIterator;
}



/** @type FieldSelector
 * @brief FieldSelectors allow iterating over a selected Message Fields.
 *
*/

typedef bool (*FieldSelector)(Field &);

GMSEC_API bool CALL_TYPE selectAllFields(Field &);
GMSEC_API bool CALL_TYPE selectHeaderFields(Field &);
GMSEC_API bool CALL_TYPE selectNonheaderFields(Field &);



/** @class FieldIterator
 * 
 * @brief The FieldIterator supports iterating over the Fields of a Message.
 *
 * FieldIterators are non-intrusive.  Since they are non-intrusive there can
 * be more than one iterating over a single Message at the same time.
 * This differs from Message GetFirstElement/GetNextElement.  It is allowed
 * to use both GetFirstElement/GetNextElement and FieldIterator(s) on a
 * Message.
 * 
 * After constructing a FieldIterator, iteration is initiated by invoking
 * Message::Reset(FieldIterator &iterator).  For example,
 *
 * @code
 * Status applyToHeaderFields (Message &message, FieldSelector function)
 * {
 *	FieldIterator i(&selectHeaderFields);
 *	Field field;
 *	Status result = message.Reset(i);
 *
 *	while (!result.isError() && i.HasNext())
 *	{
 *		result = i.Next(field);
 *		if (!result.isError())
 *			(*function)(field);
 *	}
 *
 *	return result;
 * }
 * @endcode
 *
 * Note that modifying the Fields of a Message (e.g., by calling AddField,
 * ClearFields, or ClearFields) while iterating over it is not supported.
 * 
 * Further note that FieldIterators are not thread safe.
*/

class GMSEC_API FieldIterator
{
public:

	/** @fn FieldIterator()
	 * @brief Initialize a FieldIterator without a selector.
	 */
	GMSEC_DEPRECATED FieldIterator();


	/** @fn FieldIterator(FieldSelector &selector)
	 * @brief Initialize a FieldIterator with a selector.
	 * Only Fields which the selector function returns true for will be
	 * returned by the iterator.
	 */
	GMSEC_DEPRECATED FieldIterator(FieldSelector selector);


	/** @fn ~FieldIterator()
	 * @brief Destructor - cleans up the FieldIterator instance
	 */
	~FieldIterator();


	/** @fn HasNext()
	 * @brief Indicates whether a Next call may be made
	 *
	 * @return Returns true if there is a Next field.
	 */
	bool CALL_TYPE HasNext();


	/** @fn Next(gmsec::Field &field)
	 * @brief Retrieves the next Field in the iteration.
	 *
	 * If Next is called when there is no field to be read, an error is
	 * returned with class GMSEC_STATUS_ITERATOR_ERROR and code
	 * GMSEC_INVALID_NEXT.
	 *
	 * @param field - Reference through which the next Field is returned.
	 *
	 * @return Returns a Status object indicating the success or failure of this call.
	 */
	Status CALL_TYPE Next(Field &field);


private:
	// not implemented
	FieldIterator &operator=(const FieldIterator &);
	FieldIterator(const FieldIterator &);


	friend class internal::AbstractFieldIterator;


	internal::AbstractFieldIterator *ptr;

}; // class FieldIterator

} //namespace gmsec

#endif /* gmsec_FieldIterator_h */

