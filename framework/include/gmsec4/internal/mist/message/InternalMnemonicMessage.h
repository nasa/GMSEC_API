/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMnemonicMessage.h
 *
 * @brief This file contains the interface to InternalMnemonicMessage.
 */


#ifndef GMSEC_API_MESSAGE_INTERNAL_MNEMONIC_MESSAGE_H
#define GMSEC_API_MESSAGE_INTERNAL_MNEMONIC_MESSAGE_H

#include <gmsec4/internal/mist/message/InternalMistMessage.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicIterator.h>

#include <vector>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace message
{
namespace internal
{

class GMSEC_API InternalMnemonicMessage : public InternalMistMessage
{
public:
	InternalMnemonicMessage(const char* subject, 
							const char* schemaID,
							const Specification& spec);

	InternalMnemonicMessage(const char* subject, 
							const char* schemaID,
							const gmsec::api::Config& config, 
							const Specification& spec);

	InternalMnemonicMessage(const InternalMnemonicMessage& other);


	InternalMnemonicMessage(const char* data);


	InternalMnemonicMessage(const Specification& spec, const char* data);


	virtual ~InternalMnemonicMessage();


	InternalMnemonicMessage& CALL_TYPE operator=(const InternalMnemonicMessage& other);


	void CALL_TYPE addMnemonic(const Mnemonic& mnemonic);


	const Mnemonic& CALL_TYPE getMnemonic(size_t index) const;


	size_t CALL_TYPE getNumMnemonics() const;


	MnemonicIterator& getMnemonicIterator();


	void resetMnemonicIterator();


	bool hasNextMnemonic() const;


	const Mnemonic& nextMnemonic();


private:
	Mnemonic extractMessageMnemonic(size_t index) const;


	void init();


	std::vector<Mnemonic>              m_list;
	std::vector<Mnemonic>::iterator    m_listIter;
	gmsec::api::mist::MnemonicIterator m_mnemonicIterator;
};

} // namespace internal
} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
