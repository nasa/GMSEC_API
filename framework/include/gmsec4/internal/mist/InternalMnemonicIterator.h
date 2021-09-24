/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_MNEMONIC_ITERATOR_H
#define GMSEC_API_INTERNAL_MNEMONIC_ITERATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	class Mnemonic;

	namespace message
	{
		namespace internal
		{
			class InternalMnemonicMessage;
		}
	}

namespace internal
{

class GMSEC_API InternalMnemonicIterator
{
public:
	InternalMnemonicIterator(gmsec::api::mist::message::internal::InternalMnemonicMessage& msg);


    ~InternalMnemonicIterator();


    bool CALL_TYPE hasNext();


    const Mnemonic& CALL_TYPE next();


	void CALL_TYPE reset();

private:
	// Declared, but not implemented
	InternalMnemonicIterator(const InternalMnemonicIterator&);
	InternalMnemonicIterator& operator=(const InternalMnemonicIterator&);


	bool CALL_TYPE makeReady();


	gmsec::api::mist::message::internal::InternalMnemonicMessage& m_msg;
	bool                                                          m_ready;
	const Mnemonic*                                               m_nextMnemonic;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
