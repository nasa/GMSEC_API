/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalMessageFactory.h
 *
 * @brief This file contains the internal interface for the MessageFactory.
 */

#ifndef GMSEC_API5_INTERNAL_MESSAGE_FACTORY_H
#define GMSEC_API5_INTERNAL_MESSAGE_FACTORY_H

#include <gmsec5/internal/CustomMessageValidator.h>
#include <gmsec5/internal/TicketMutex.h>
#include <gmsec5/internal/SchemaIdMapping.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/Specification.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Field;
	class MessageValidator;

namespace internal
{
	// Forward declaration(s)
	class MessageTemplate;


class GMSEC_API InternalMessageFactory
{
public:
	InternalMessageFactory();

	InternalMessageFactory(const Config& config);

	~InternalMessageFactory();

	void CALL_TYPE setStandardFields(const gmsec::api5::util::List<Field*>& standardFields);

	void CALL_TYPE clearStandardFields();

	void CALL_TYPE setMessageConfig(const Config& msgConfig);

	Message CALL_TYPE createMessage() const;

	Message CALL_TYPE createMessage(const char* schemaID);

	Message* CALL_TYPE newMessage() const;

	const Config& CALL_TYPE getMessageConfig() const;

	Message CALL_TYPE fromData(const char* data, DataType type);

	const Specification& CALL_TYPE getSpecification() const;

    void CALL_TYPE registerMessageValidator(MessageValidator* validator);

    void CALL_TYPE registerMessageValidator(GMSEC_MessageValidator* validator);

	void CALL_TYPE addMessageTemplate(Message& msg);

private:
	// Defined, but not implemented
	InternalMessageFactory(const InternalMessageFactory& other);
	InternalMessageFactory& operator=(const InternalMessageFactory& other);

	typedef std::map<std::string, gmsec::api5::util::StdSharedPtr<MessageTemplate> >  MessageTemplateCache;

	// Helper methods
	std::string deduceSchemaID(const Message& msg);

	gmsec::api5::util::StdSharedPtr<MessageTemplate> createMessageTemplate(const std::string& schemaID);

	Message::Kind deduceMessageKind(const char* schemaID, unsigned int version);

	// Member data
	Config                                                    m_msgConfig;
	gmsec::api5::util::StdUniquePtr<Specification>            m_spec;
	bool                                                      m_validateMsgs;
	gmsec::api5::util::List<Field*>                           m_standardFields;
	gmsec::api5::internal::SchemaIdMapping                    m_schemaIdMap;
	MessageValidator*                                         m_customMessageValidator;
	gmsec::api5::util::StdUniquePtr<CustomMessageValidator>   m_ceeMessageValidator;

	MessageTemplateCache									  m_mtCache;

	mutable gmsec::api5::internal::TicketMutex                m_mutex;
};


class MessageFactoryBuddy
{
public:
	static InternalMessageFactory& getInternal(const MessageFactory& msgFactory)
	{
		return *(msgFactory.m_internal);
	}
};

}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec


#endif
