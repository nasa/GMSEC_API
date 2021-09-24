/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_MESSAGE_PRODUCT_FILE_ITERATOR_H
#define GMSEC_API_INTERNAL_MESSAGE_PRODUCT_FILE_ITERATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	class ProductFile;

	namespace message
	{
		namespace internal
		{
			class InternalProductFileMessage;
		}
	}

namespace internal
{

class GMSEC_API InternalProductFileIterator
{

public:
	InternalProductFileIterator(gmsec::api::mist::message::internal::InternalProductFileMessage& msg);


    ~InternalProductFileIterator();


    bool CALL_TYPE hasNext();


    const ProductFile& CALL_TYPE next();


	void CALL_TYPE reset();


private:
	// Declared, but not implemented
	InternalProductFileIterator(const InternalProductFileIterator&);
	InternalProductFileIterator& operator=(const InternalProductFileIterator&);


	bool CALL_TYPE makeReady();


	gmsec::api::mist::message::internal::InternalProductFileMessage& m_msg;
	bool                                                             m_ready;
	const ProductFile*                                               m_nextProductFile;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
