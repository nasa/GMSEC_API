/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file SimpleFieldIterator.h
 *
 *  @brief This file contains a definition for a simple message field iterator.
 *
**/


#ifndef gmsec_SimpleFieldIterator_h
#define gmsec_SimpleFieldIterator_h


#include <gmsec/Status.h>
#include <gmsec/internal/AbstractFieldIterator.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

namespace internal
{

class MessageFieldMap;


/** @class SimpleFieldIterator
 * 
 * @brief The Field Iterator allows the fields of a message to be iterated over
 * in the manner of a Java Iterator.
 *
 * This iterator may be used entirely independently of the GetFirstField & GetNextField
 * methods of a message. If the user desires to edit the message with AddField and
 * ClearField while the iterator is in use, use the call Message::Reset(FieldIterator, true)
 * when resetting the iterator. Otherwise, use Message::Reset(FieldIterator, false)
*/

class GMSEC_API SimpleFieldIterator : public AbstractFieldIterator
{
public:

	/** @fn SimpleFieldIterator(FieldMap &fields)
	 * @brief Constructor - Accepts the fields which will be iterated.
	 *
	 * @param fields - The fields to iterate over.
	 */
	SimpleFieldIterator(MessageFieldMap &fields);


	/** @fn ~SimpleFieldIterator()
	 * @brief Destructor - cleans up the FieldIterator instance
	 */
	virtual ~SimpleFieldIterator();


	/** @fn HasNext()
	 * @brief Indicates whether a Next call may be made
	 *
	 * @return Returns true if there is a Next field.
	 */
	virtual bool CALL_TYPE HasNext();


	/** @fn Next(gmsec::Field &field)
	 * @brief Populates the field reference with the contents of the next field
	 *
	 * @param field - Reference to the field into which the contents of the Next
	 * field will be copied. If Next is called when there is no field to be read, 
	 * and error is return with the class GMSEC_STATUS_FIELDITERATOR_ERROR and the
	 * code GMSEC_INVALID_NEXT.
	 *
	 * @return Returns a Status object indicating the success or failure of this call.
	 */
	virtual Status CALL_TYPE Next(Field &field);


private:

	// Declared, but not implemented.
	SimpleFieldIterator();
	SimpleFieldIterator(const SimpleFieldIterator &);
	SimpleFieldIterator &operator=(const SimpleFieldIterator &);

	MessageFieldMap &fFields;
};

} //namespace internal

} //namespace gmsec


#endif /* gmsec_internal_SimpleFieldIterator_h */

