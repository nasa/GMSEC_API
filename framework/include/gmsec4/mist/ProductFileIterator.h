/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileIterator.h
 *
 * @brief Defines a class that supports the iteration over the ProductFile objects stored in a ProductFileMessage.
 */

#ifndef GMSEC_API_PRODUCT_FILE_ITERATOR_H
#define GMSEC_API_PRODUCT_FILE_ITERATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	class ProductFile;
	namespace internal
	{
		class InternalProductFileIterator;
	}

namespace message
{
	namespace internal
	{
		class InternalProductFileMessage;
	}
}


/** @class ProductFileIterator
 * 
 * @brief The ProductFileIterator supports iterating over the ProductFile objects of a ProductFileMessage.
 *
 * @note ProductFileIterator is not thread safe.
 *
 * @sa ProductFileMessage::getProductFileIterator()
*/
class GMSEC_API ProductFileIterator
{
public:
	/*
	 * @fn ~ProductFileIterator()
	 *
	 * @brief Destructor.
	 */
    ~ProductFileIterator();


	/**
	 * @fn bool hasNext() const
	 *
	 * @brief Provides information as to whether there are additional ProductFile objects that can be referenced using next().
	 *
	 * @return True if additional fields are available, false otherwise.
	 */
    bool CALL_TYPE hasNext() const;


	/**
	 * @fn const ProductFile& next() const
	 *
	 * @brief Returns a reference to the next available ProductFile object.
	 *
	 * @return A reference to a ProductFile object.
	 *
	 * @throw Exception is thrown if the iterator has reached the end (i.e. there are no more ProductFiles).
	 */
    const ProductFile& CALL_TYPE next() const;


	/**
	 * @fn void reset()
	 *
	 * @desc Resets the interator to the beginning of the ProductFile list that is maintained by the ProductFileMessage.
	 */
	void CALL_TYPE reset();


private:
	friend class gmsec::api::mist::message::internal::InternalProductFileMessage;

	/**
	 * @fn ProductFileIterator(const ProductFileMessage& msg)
	 *
	 * @brief Default constructor.
	 *
	 * @param msg - the ProductFileMessage object to use when iterating over the ProductFile objects.
	 */
	ProductFileIterator(gmsec::api::mist::message::internal::InternalProductFileMessage& msg);


	// Declared, but not implemented
	ProductFileIterator(const ProductFileIterator&);

	//TODO - Figure out why Visual Studio thinks this method must be implicitly defined.
	//       Might be related to the (false) C4355 warning.
	//ProductFileIterator& operator=(const ProductFileIterator&);


	gmsec::api::mist::internal::InternalProductFileIterator* m_iter;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
