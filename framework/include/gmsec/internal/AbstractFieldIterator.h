
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file AbstractFieldIterator.h
 *
 *  @brief This file contains the internal message field iterator.
 *
**/


#ifndef gmsec_AbstractFieldIterator_h
#define gmsec_AbstractFieldIterator_h


#include <gmsec/FieldIterator.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

namespace internal
{

/** @class AbstractFieldIterator
 *
 * @brief The abstract based class of field iterators.
 *
 * The associated public interface class is gmsec::FieldIterator.
*/

class GMSEC_API AbstractFieldIterator
{
public:

	AbstractFieldIterator();


	/** @fn ~AbstractFieldIterator()
	 * @brief Destructor
	 */
	virtual ~AbstractFieldIterator();


	void update(FieldIterator &external);
	virtual void CALL_TYPE associate(FieldIterator &external, AbstractFieldIterator *internal);


	/** @fn HasNext()
	 * @brief Indicates whether a Next call may be made.
	 *
	 * @return Returns true if there is a Next field.
	 */
	virtual bool CALL_TYPE HasNext() = 0;


	/** @fn Next(gmsec::Field &field)
	 * @brief Retrieves the next Field in the iteration.
	 */
	virtual Status CALL_TYPE Next(Field &field) = 0;

private:

	// Declared, but not implemented.
	AbstractFieldIterator(const AbstractFieldIterator &);
	AbstractFieldIterator &operator=(const AbstractFieldIterator &);

}; // class AbstractFieldIterator



/** @class NullFieldIterator
 * @brief Default backing of FieldIterator which provides a null iterator.
 */

class GMSEC_API NullFieldIterator : public AbstractFieldIterator
{
public:

	NullFieldIterator();

	virtual ~NullFieldIterator();


	virtual bool CALL_TYPE HasNext();

	virtual Status CALL_TYPE Next(Field &field);

private:

	NullFieldIterator(const NullFieldIterator &);
	NullFieldIterator &operator=(const NullFieldIterator &);

}; // NullFieldIterator



class GMSEC_API SelectorFieldIterator : public AbstractFieldIterator
{
public:

	/** @fn ~SelectorFieldIterator()
	 * @brief Destructor
	 */
	SelectorFieldIterator(FieldSelector selector);
	virtual ~SelectorFieldIterator();


	virtual void CALL_TYPE associate(FieldIterator &external, AbstractFieldIterator *internal);


	/** @fn HasNext()
	 * @brief Indicates whether a Next call may be made.
	 *
	 * @return Returns true if there is a Next field.
	 */
	virtual bool CALL_TYPE HasNext();


	/** @fn Next(gmsec::Field &field)
	 * @brief Retrieves the next field (which matches the selector).
	 */
	virtual Status CALL_TYPE Next(Field &field);

private:

	SelectorFieldIterator(const SelectorFieldIterator &);
	SelectorFieldIterator &operator=(const SelectorFieldIterator &);


	/* queue up the next selected field (if there is one) */
	bool makeReady();


	FieldSelector fSelector;
	AbstractFieldIterator * fChild;
	bool fReady;
	Field fField;

}; // class SelectorFieldIterator


} //namespace internal

} //namespace gmsec


#endif /* gmsec_AbstractFieldIterator_h */

