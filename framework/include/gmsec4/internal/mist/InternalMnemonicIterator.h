/*
 * Copyright 2007-2016 United States Government as represented by the
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

namespace internal
{
class InternalMnemonicMessage;


class GMSEC_API InternalMnemonicIterator
{
public:
	InternalMnemonicIterator(InternalMnemonicMessage& msg);


    ~InternalMnemonicIterator();


    bool CALL_TYPE hasNext();


    const Mnemonic& CALL_TYPE next();


	void CALL_TYPE reset();


private:
	// Declared, but not implemented
	InternalMnemonicIterator(const InternalMnemonicIterator&);
	InternalMnemonicIterator& operator=(const InternalMnemonicIterator&);


	bool CALL_TYPE makeReady();


	InternalMnemonicMessage& m_msg;
	bool                     m_ready;
	const Mnemonic*          m_nextMnemonic;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
