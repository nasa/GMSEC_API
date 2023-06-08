/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/Subject.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdUniquePtr.h>

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include <OpenDDSMessageListener.h>
#include <OpenDDSMessageTypeSupportC.h>
#include <OpenDDSMessageTypeSupportImpl.h>


using namespace gmsec::api5;
using namespace gmsec::api5::util;
using namespace gmsec::api5::internal;
using namespace gmsec_opendds;

OpenDDSMessageListener::OpenDDSMessageListener(const char* subjectPattern,
                                               OpenDDSConnection* conn,
                                               Queue &queue,
                                               bool dropMessages)
	: subjectPattern(subjectPattern),
	  connection(conn),
	  queue(queue),
	  dropMessages(dropMessages)
{
}

OpenDDSMessageListener::~OpenDDSMessageListener()
{
	connection = 0;
}

void OpenDDSMessageListener::on_requested_deadline_missed(DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus&)
{
	GMSEC_WARNING << "OpenDDSMessageListener::on_requested_deadline_missed";
}


void OpenDDSMessageListener::on_requested_incompatible_qos( DDS::DataReader_ptr reader, const DDS::RequestedIncompatibleQosStatus&)
{
	GMSEC_WARNING << "OpenDDSMessageListener::on_requested_incompatible_qos";
}


void OpenDDSMessageListener::on_sample_rejected(DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus&)
{
	GMSEC_WARNING << "OpenDDSMessageListener::on_sample_rejected";
}


void OpenDDSMessageListener::on_liveliness_changed(DDS::DataReader_ptr reader, const DDS::LivelinessChangedStatus&)
{
}


void OpenDDSMessageListener::on_data_available(DDS::DataReader_ptr reader)
{
	try
	{
		OpenDDSMessage::MessageDataReader_var reader_i = OpenDDSMessage::MessageDataReader::_narrow(reader);
	
		if (!reader_i) 
		{
			throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE,
				"Error: OpenDDSMessageListener.cpp: on_date_available()- _narrowr failed!");
		}

		OpenDDSMessage::Message DDSmessage;
		DDS::SampleInfo         info;
		
		DDS::ReturnCode_t retCode = reader_i->take_next_sample(DDSmessage, info);
	
		if (retCode == DDS::RETCODE_OK && info.valid_data == 1 && !dropMessages)
		{
			// Extract the contents of the encoded GMSEC message from the OpenDDS message
			DataBuffer gmsecBuffer((GMSEC_U8*) DDSmessage.gmsec_data.get_buffer(), DDSmessage.gmsec_data.length(), false);

			// Extract the contents of the encoded meta object from the OpenDDS message
			DataBuffer metaBuffer((GMSEC_U8*) DDSmessage.meta_data.get_buffer(), DDSmessage.meta_data.length(), false);

			StdUniquePtr<Message> message( MessageFactoryBuddy::getInternal(connection->getExternal().getMessageFactory()).newMessage() );

			message->setSubject(DDSmessage.subject.in());
			message->setConfig(connection->getExternal().getMessageConfig());

			// Decode the meta object
			ValueMap meta;
			parseProperties(meta, metaBuffer, *message.get());

			// If we receive a message, and its subject does not match the topic pattern we are seeking,
			// then drop like hot potato.
			if (Subject::doesSubjectMatchPattern(message.get()->getSubject(), subjectPattern) == false)
			{
				return;
			}

			MessageInfo info;

			info.result = connection->getExternal().getPolicy().unpackage(*message.get(), gmsecBuffer, meta);
		
			if (info.result.hasError())
			{
				queue.push(info);
				return;
			}

			MessageFactoryBuddy::getInternal(connection->getExternal().getMessageFactory()).addMessageTemplate(*message.get());

			// Check to see if is type reply and whether or not to enqueue message/result
			if (message->getKind() == Message::Kind::REPLY)
			{
				Message* reply = message.release();

				// Create a clone of the reply message
				// which will later be enqueued.
				message.reset(new Message(*reply));

				connection->getExternal().onReply(reply);
			}

			// Expose original message to subscribers (if any)
			info.msg = message.release();
			queue.push(info);
		}
	}
	catch (...)
	{
		GMSEC_WARNING << "Encountered unexpected exception!";
	}
}


void OpenDDSMessageListener::on_subscription_matched(DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus& notUsed)
{
}


void OpenDDSMessageListener::on_sample_lost(DDS::DataReader_ptr reader, const DDS::SampleLostStatus&)
{
	GMSEC_WARNING << "OpenDDSMessageListener::on_sample_lost";
}


Status OpenDDSMessageListener::parseProperties(ValueMap& meta, const DataBuffer& in, Message& message)
{
	Status result;

	Decoder* decoder = Decoder::getDecoder();

	const char* inputBuffer = reinterpret_cast<const char*>(in.get());

	// Determine the number of Values in the meta blob
	GMSEC_I32 numValues = 0;
	decoder->getI32(inputBuffer, &numValues);

	// Begin processing each Value out of the meta blob
	for (GMSEC_I32 i = 0; i < numValues; i++)
	{
		GMSEC_U16 type;
		decoder->getU16(inputBuffer, &type);

		const char* valueName    = 0;
		GMSEC_U32   valueNameLen = 0;

		decoder->getString(inputBuffer, valueNameLen, valueName);

		Field* 		field 		= NULL;
		Field::Type fieldType 	= static_cast<Field::Type>(type);

		switch (fieldType)
		{
			case Field::Type::BOOL:
			{
				GMSEC_U8 value;
				decoder->getU8(inputBuffer, &value);

				// Create a ValueMap entry
				meta.setBoolean(valueName, value ? true: false);

				field = new BooleanField(valueName, value ? true: false);
				break;
			}
			case Field::Type::I32:
			{
				GMSEC_I32 value;
				decoder->getI32(inputBuffer, &value);
				meta.setI32(valueName, value);

				// No need to store the MSG-KIND as a field
				if (strcmp(valueName, "MSG-KIND") == 0)
				{
					MessageBuddy::getInternal(message).setKind(static_cast<Message::Kind>(value));
					continue;
				}

				field = new I32Field(valueName, value);
				break;
			}
			case Field::Type::F64:
			{
				GMSEC_F64 value;
				decoder->getF64(inputBuffer, &value);
				meta.setF64(valueName, value);

				field = new F64Field(valueName, value);
				break;
			}
			case Field::Type::STRING:
			{
				const char* value    = 0;
				GMSEC_U32   valueLen = 0;

				decoder->getString(inputBuffer, valueLen, value);

				meta.setString(valueName, std::string(value));

				field = new StringField(valueName, value);
				break;
			}
			default:
				GMSEC_WARNING << "Unexpected meta-type decoded from input buffer: " << type;
				break;
		}

		if (field != NULL)
		{
			message.addField(*field);

			delete field;
		}
	}

	return result;
}
