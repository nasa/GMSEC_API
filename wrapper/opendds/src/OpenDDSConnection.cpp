/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// This file contains the implementation for the OpenDDS Connection.
#include <OpenDDSConnection.h>

#include <gmsec4_defs.h>

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/InternalConnection.h>
#include <gmsec4/internal/MessageBuddy.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>
#include <gmsec4/internal/Middleware.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/TimeUtil.h>

#include <dds/DCPS/WaitSet.h>
#include <tao/Condition.h>
#include <ace/Init_ACE.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::internal;

namespace gmsec_opendds
{

class OpenDDSMiddleware : public Middleware
{
public:
	OpenDDSMiddleware()
	{
		ACE::init();

		GMSEC_INFO << "Initialized OpenDDS";
	}

	~OpenDDSMiddleware()
	{
		GMSEC_INFO << "Finalizing OpenDDS";

		TheServiceParticipant->shutdown();

		ACE::fini();
	}
};


static std::string topicToFilenamePattern(const std::string& gmsecTopic)
{
	std::vector<std::string> elements = StringUtil::split(gmsecTopic, '.');

	std::string pattern;

	for (size_t i = 0; i < elements.size(); ++i)
	{
		if (elements[i] == ">" || elements[i] == "*")
		{
			pattern += ".*";
		}
		else if (elements[i] == "+")
		{
			pattern += "*";
		}
		else
		{
			if (!pattern.empty())
			{
				pattern += ".";
			}

			pattern += elements[i];
		}
	}

	return pattern;
}


static Mutex& getClassMutex()
{
	static Mutex mutex;
	return mutex;
}


OpenDDSConnection::OpenDDSConnection(const Config& config)
	: m_brokerService(),
	  m_openddsConfig(),
	  m_dcpsInfoRepo(),
	  m_mwInfo(),
	  m_queue(),
	  m_requestCounter(0),
	  m_requestSpecs(),
	  m_selfSubscribed(false),
	  m_subscriptions(),
	  m_domainId(0)
{
	static bool initialized = false;

	if (!initialized)
	{
		AutoMutex hold(getClassMutex());

		if (!initialized)
		{
			// Register our custom Middleware object so that TheServiceParticipant can be shutdown.
			Middleware::addMiddleware(OPENDDS_VERSION_STRING, new OpenDDSMiddleware());
			initialized = true;
		}
	}

	m_openddsConfig = config.getValue("mw-opendds-config-file", m_openddsConfig.c_str());
	m_dcpsInfoRepo  = config.getValue("server", m_dcpsInfoRepo.c_str());
	m_domainId      = config.getIntegerValue("mw-domain-id", m_domainId);
}


OpenDDSConnection::~OpenDDSConnection()
{
}


const char* OpenDDSConnection::getLibraryVersion()
{
	return OPENDDS_VERSION_STRING;
}


const char* OpenDDSConnection::getMWInfo()
{
	if (m_mwInfo.empty())
	{
		m_mwInfo = getLibraryRootName();
	}

	return m_mwInfo.c_str();
}


void OpenDDSConnection::mwConnect()
{
	int    argc;
	char** argv = NULL;

	if (!m_openddsConfig.empty() && !m_dcpsInfoRepo.empty())
	{
		// Have both DCPSConfigFile and DCPSInfoRepo
		argc = 5;
		argv = new char*[argc];

		argv[0] = strdup("client");	
		argv[1] = strdup("-DCPSConfigFile");
		argv[2] = strdup(m_openddsConfig.c_str());
		argv[3] = strdup("-DCPSInfoRepo");
		argv[4] = strdup(m_dcpsInfoRepo.c_str());
	}
	else if (!m_openddsConfig.empty())
	{
		// Have only DCPSConfigFile
		argc = 3;
		argv = new char*[argc];

		argv[0] = strdup("client");	
		argv[1] = strdup("-DCPSConfigFile");
		argv[2] = strdup(m_openddsConfig.c_str());
	}
	else if (!m_dcpsInfoRepo.empty())
	{
		// Have only DCPSInfoRepo
		argc = 3;
		argv = new char*[argc];

		argv[0] = strdup("client");	
		argv[1] = strdup("-DCPSInfoRepo");
		argv[2] = strdup(m_dcpsInfoRepo.c_str());
	}
	else
	{
		throw Exception(MIDDLEWARE_ERROR, INVALID_CONNECTION_TYPE,
			"Error: Missing server or mw-opendds-config-file configuration option");
	}

	bool errorConnecting = false;
	std::string errorReason;

	try
	{
		// Initialize DomainParticipantFactory
		int tmp_argc = argc;
		m_dpf = TheParticipantFactoryWithArgs(tmp_argc, argv);

		// Create DomainParticipant
		m_participant = m_dpf->create_participant(
			m_domainId, 
			PARTICIPANT_QOS_DEFAULT, 
			DDS::DomainParticipantListener::_nil(), 
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (CORBA::is_nil(m_participant))
		{
			errorConnecting = true;
			errorReason     = "Failed to create participant";
		}
	}
	catch (const std::exception& e)
	{
		errorConnecting = true;
		errorReason     = e.what();
	}

	for (int i = 0; i < argc; i++)
	{
		free(argv[i]);
	}
	delete [] argv;

	if (errorConnecting)
	{
		throw Exception(MIDDLEWARE_ERROR, INITIALIZATION_ERROR, errorReason.c_str());
	}

	// Initialize the request specs
	m_requestSpecs = getExternal().getRequestSpecs();

	if (m_requestSpecs.requestReplyEnabled && m_requestSpecs.useSubjectMapping)
	{
		// If here, subscribe to receive replies that are unique to this connection.
		Config config;
		config.addValue("__IS_FOR_REPLIES__", "true");
		mwSubscribe(m_requestSpecs.replySubject.c_str(), config);
	}

	getExternal().setConnectionEndpoint( getConnectionEndpoint() );

	Status status(NO_ERROR_CLASS, CONNECTION_CONNECTED, "Connection established");
	GMSEC_INFO << status.get();
	getExternal().dispatchEvent(Connection::CONNECTION_SUCCESSFUL_EVENT, status);
}


void OpenDDSConnection::mwDisconnect()
{
	try
	{
		for (OpenDDSSubscriptions::iterator it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it)
		{
			OpenDDSSubscriber* sub = it->second;

			(void) sub->subscriber->delete_datareader(sub->reader.in());

			delete sub;
		}
		m_subscriptions.clear();

		for (OpenDDSWriters::iterator it = m_writers.begin(); it != m_writers.end(); ++it)
		{
			OpenDDSWriter* openddsWriter = it->second;
			openddsWriter->publisher->delete_datawriter(openddsWriter->writer.in());
			m_participant->delete_publisher(openddsWriter->publisher.in());
			delete openddsWriter;
		}
		m_writers.clear();

		if (!CORBA::is_nil(m_participant.in()))
		{
			m_participant->delete_contained_entities();
		}

		if (!CORBA::is_nil(m_dpf.in()))
		{
			m_dpf->delete_participant(m_participant.in());
		}
	}
	catch (...)
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: Exception caught in cleanup.");
	}

	m_participant = 0;

	Status status(NO_ERROR_CLASS, CONNECTION_LOST, "Connection disconnected");
	GMSEC_INFO << status.get();
	getExternal().dispatchEvent(Connection::CONNECTION_BROKEN_EVENT, status);
}


void OpenDDSConnection::mwSubscribe(const char* subject, const Config &config)
{
	std::string subject1(subject);

	if (m_subscriptions.find(subject1) != m_subscriptions.end())
	{
		if (m_selfSubscribed)
		{
			mwUnsubscribe(subject);
			m_selfSubscribed = false;
		}
		else
		{
			throw Exception(MIDDLEWARE_ERROR, INVALID_SUBJECT_NAME, "Duplicate subscription");
		}
	}

	bool isForReplies = config.getBooleanValue("__IS_FOR_REPLIES__", false);
	bool dropMessages = config.getBooleanValue("__DROP_MESSAGES__", false);

	// Register Type (OpenDDSMessage::Message)
	OpenDDSMessage::MessageTypeSupport_var ts = new OpenDDSMessage::MessageTypeSupportImpl();

	if (ts->register_type(m_participant.in(), "") != DDS::RETCODE_OK)
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: register_type failed!");
	}

	// Create Topic
	CORBA::String_var type_name = ts->get_type_name();
	
	DDS::Topic_var topic = m_participant->create_topic(
		"Message",
		type_name, 
		TOPIC_QOS_DEFAULT, 
		DDS::TopicListener::_nil(), 
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(topic.in())) 
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: create_topic failed!");
	}

	// Create Subscriber
    DDS::SubscriberQos sub_qos;
    m_participant->get_default_subscriber_qos(sub_qos);

    sub_qos.partition.name.length(1);
    sub_qos.partition.name[0] = topicToFilenamePattern(subject1).c_str();

	DDS::Subscriber_var subscriber = m_participant->create_subscriber(
		sub_qos,
		DDS::SubscriberListener::_nil(), 
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(subscriber.in())) 
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: create_subscriber failed!");
	}

	// Create DataReader Listener
	DDS::DataReaderListener_var listener(new OpenDDSMessageListener(subject, this, m_queue, m_requestSpecs, isForReplies, dropMessages));

	if (CORBA::is_nil(listener.in()))
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: listener is nil.");
	}

	// Create DataReader
	DDS::DataReader_var reader = subscriber->create_datareader(
		topic.in(), 
		DATAREADER_QOS_DEFAULT, 
		listener.in(), 
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(reader.in())) 
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: create_datareader failed!");
	}

	// Insert Subscriber into list of subscriptions
	OpenDDSSubscriber* sub = new OpenDDSSubscriber();
	sub->subscriber = subscriber;
	sub->listener = listener;
	sub->reader = reader;

	m_subscriptions[subject] = sub;
}


void OpenDDSConnection::mwUnsubscribe(const char* subject)
{
	OpenDDSSubscriptions::iterator it = m_subscriptions.find(subject);
	
	if (it != m_subscriptions.end())
	{
		OpenDDSSubscriber* sub = it->second;

		DDS::ReturnCode_t ret = sub->subscriber->delete_datareader(sub->reader.in());

		delete sub;
		m_subscriptions.erase(it);

		if (ret != DDS::RETCODE_OK)
		{
			throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, ret, "Error: DataReader delete failed");
		}
	}
	else
	{
		GMSEC_WARNING << "Received request to unsubscribe unmanaged subject";
	}
}


void OpenDDSConnection::mwPublish(const Message& msg, const Config& config)
{
	std::string subject(msg.getSubject());

	// Set up a subscription using our message subject/topic; this will allow
	// for the publisher to send the message even though there might not be
	// any external subscribers. Otherwise the publisher would block until a
	// subscriber does become available.
	//
	// If the publisher has already subscribed to the message subject/topic,
	// then there is no need to subscribe again.
	//
	// If the publisher receives the message it sends using the subscription
	// set up below, then messages that are published will not be delivered
	// to the publisher itself. Other subscribers will receive the message.
	//
	// When the publisher disconnects from the bus, then the subscription
	// will be removed.
	//
	// TODO: Maybe should allow user to configure whether or not the
	// subscription will be made; it may be desirable to not publish a
	// message until an external subscriber becomes available.
	//
	if (m_subscriptions.find(subject) == m_subscriptions.end())
	{
		try
		{
			Config config;
			config.addValue("__DROP_MESSAGES__", "true");

			mwSubscribe(subject.c_str(), config);

			TimeUtil::millisleep(25);

			m_selfSubscribed = true;
		}
		catch (...)
		{
			GMSEC_VERBOSE << "Publisher has already subscribed to topic: " << subject.c_str();
		}
	}

	// Create DataWriter, unless one already exists for the message subject
	OpenDDSWriter* openddsWriter;
	OpenDDSWriters::iterator it = m_writers.find(subject);

	if (it == m_writers.end())
	{
		openddsWriter = createWriter(subject, config);

		m_writers[subject] = openddsWriter;
	}
	else
	{
		openddsWriter = it->second;
	}

	// Block until Subscriber is available
	DDS::StatusCondition_var condition = openddsWriter->writer->get_statuscondition();
	condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);

	DDS::WaitSet_var ws = new DDS::WaitSet;
	ws->attach_condition(condition);

	while (true)
	{
		DDS::PublicationMatchedStatus matches;

		if (openddsWriter->writer->get_publication_matched_status(matches) != DDS::RETCODE_OK)
		{
			throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: get_publication_matched_status failed!");
		}

		if (matches.current_count >= 1)
		{
			break;
		}

		DDS::ConditionSeq conditions;
		DDS::Duration_t timeout = {5, 0};
		if (ws->wait(conditions, timeout) != DDS::RETCODE_OK)
		{
			GMSEC_WARNING << "Still waiting for a subscriber.";
		}
	}

	ws->detach_condition(condition);

	// Prepare the message
	OpenDDSMessage::Message openddsMsg = prepareMsg(msg);

	// Write the message
	OpenDDSMessage::MessageDataWriter_var message_writer = OpenDDSMessage::MessageDataWriter::_narrow(openddsWriter->writer.in());

	if (CORBA::is_nil(message_writer.in()))
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: _narrow failed!");
	}

	DDS::ReturnCode_t ret = message_writer->write(openddsMsg, DDS::HANDLE_NIL);
	if (ret != DDS::RETCODE_OK)
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, ret, "Error: message_writer failed to write");
	}	

	// Ensure message has been delivered
	DDS::Duration_t timeout = {30,0}; // 30 seconds
	openddsWriter->writer->wait_for_acknowledgments(timeout);
}


void OpenDDSConnection::mwRequest(const Message& request, std::string& uniqueID)
{
	uniqueID = generateUniqueId(++m_requestCounter);

	MessageBuddy::getInternal(request).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());

	if (m_requestSpecs.useSubjectMapping)
	{
		MessageBuddy::getInternal(request).addField(GMSEC_REPLY_SUBJECT, m_requestSpecs.replySubject.c_str());
	}
	else
	{
		MessageBuddy::getInternal(request).getDetails().setBoolean(GMSEC_REQ_RESP_BEHAVIOR, true);
	}

	// Send request
	mwPublish(request, getExternal().getConfig());
}


void OpenDDSConnection::mwReply(const Message& request, const Message& reply)
{
	// Get the Request's Unique ID, and put it into a field in the Reply
	std::string uniqueID = getExternal().getReplyUniqueID(request);
	const StringField* replySubj = dynamic_cast<const StringField*>(request.getField(GMSEC_REPLY_SUBJECT));

	if (uniqueID.empty())
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain unique ID field");
	}
	if (!replySubj)
	{
		throw Exception(CONNECTION_ERROR, INVALID_MSG, "Request does not contain MW-REPLY-SUBJECT field");
	}

	MessageBuddy::getInternal(reply).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, uniqueID.c_str());
	MessageBuddy::getInternal(reply).setSubject(replySubj->getValue());

	// Send reply
	mwPublish(reply, getExternal().getConfig());
}


void OpenDDSConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	msg = NULL;

	bool done = false;

	while (!done)
	{
		if (m_queue.isEmpty())
		{
			if (timeout == GMSEC_WAIT_FOREVER)
			{
				m_queue.wait();
			}
			else if (timeout == 0)
			{
				done = true;
			}
			else
			{
				done = (m_queue.wait(timeout) == Queue::TIMEOUT);
			}
		}
		else
		{
			// got message 
			MessageInfo info = m_queue.pop();

			if (info.result.isError())
			{
				throw Exception(info.result);
			}

			msg = info.msg;

			done = true;
		}
	}
}


OpenDDSConnection::OpenDDSWriter* OpenDDSConnection::createWriter(const std::string& subject, const Config& config)
{
	// Register Type (OpenDDSMessage::Message)
	OpenDDSMessage::MessageTypeSupport_var ts = new OpenDDSMessage::MessageTypeSupportImpl(); 

	if (ts->register_type(m_participant, "") != DDS::RETCODE_OK)
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: register_type failed!");
	}

	// Get QoS to use for our topic
	// Could also use TOPIC_QOS_DEFAULT instead
	DDS::TopicQos default_topic_qos;
	m_participant->get_default_topic_qos(default_topic_qos);
	
	// Create Topic
	CORBA::String_var type_name = ts->get_type_name();
	DDS::Topic_var topic = m_participant->create_topic(
		"Message",
		type_name, 
		default_topic_qos, 
		DDS::TopicListener::_nil(), 
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(topic.in())) 
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: create_topic failed!");
	}

	// Create Publisher
	DDS::PublisherQos pub_qos;
	m_participant->get_default_publisher_qos(pub_qos);
	pub_qos.partition.name.length(1);
	pub_qos.partition.name[0] = subject.c_str();

    DDS::Publisher_var publisher = m_participant->create_publisher(
		pub_qos, 
		DDS::PublisherListener::_nil(), 
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(publisher.in()))
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: create_publisher failed!");
	}
	
	// Get the default QoS for our Data Writers
	// Could also use DATAWRITER_QOS_DEFAULT
	DDS::DataWriterQos dw_default_qos;
	publisher->get_default_datawriter_qos(dw_default_qos);

	// Create DataWriter
    DDS::DataWriter_var writer = publisher->create_datawriter(
		topic, 
		dw_default_qos, 
		DDS::DataWriterListener::_nil(), 
		OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(writer.in()))
	{
		throw Exception(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error: create_datawriter failed!");
	}

	OpenDDSWriter* openddsWriter = new OpenDDSWriter();
	openddsWriter->publisher     = publisher;
	openddsWriter->writer        = writer;

	return openddsWriter;
}


OpenDDSMessage::Message OpenDDSConnection::prepareMsg(const gmsec::api::Message& msg)
{
	std::string subject(msg.getSubject());

	DataBuffer gmsecBuffer;
	ValueMap   meta;

	Status result = getExternal().getPolicy().package(const_cast<Message&>(msg), gmsecBuffer, meta);

	if (result.isError())
	{
		throw Exception(result);
	}

	// Add the Message Kind to the message meta data
	meta.setI32("MSG-KIND", msg.getKind());
	
	// Encode the Message meta data
	DataBuffer metaBuffer;
	storeProperties(meta, metaBuffer);

	// Populate IDL data types with GMSEC and meta data
	OpenDDSMessage::Message openddsMsg;
	openddsMsg.subject_id 	= CORBA::string_dup(getExternal().getID());
	openddsMsg.subject 		= CORBA::string_dup(subject.c_str());
	openddsMsg.meta_data.length(metaBuffer.size());
	openddsMsg.gmsec_data.length(gmsecBuffer.size());

	openddsMsg.meta_data  = OpenDDSMessage::OctetSeq(metaBuffer.size(), metaBuffer.size(), const_cast<unsigned char*>(metaBuffer.get()), false);
	openddsMsg.gmsec_data = OpenDDSMessage::OctetSeq(gmsecBuffer.size(), gmsecBuffer.size(), const_cast<unsigned char*>(gmsecBuffer.get()), false);

	return openddsMsg;
}


std::string OpenDDSConnection::generateUniqueId(GMSEC_U32 id)
{
	std::ostringstream uniqueId;
	uniqueId << getExternal().getID() << "_" << SystemUtil::getProcessID() << "_" << id;
	return uniqueId.str();
}


Status OpenDDSConnection::storeProperties(const ValueMap& meta, DataBuffer& out)
{
	Status result;

	// Determine the length of the buffer and resize it accordingly
	size_t length = 0;

	findLength(meta, length);

	out.resize(length);

	// Build up the meta blob
	char* currentBuffer = reinterpret_cast<char*>(out.raw());

	// Frame 0: Number of Values
	GMSEC_I32 numValues = (GMSEC_I32) meta.size();
	Encoder::getEncoder()->putI32(&numValues, currentBuffer);

	// Frames 1..n: Each Value
	ValueMap::Iterator i;
	meta.reset(i);
	
	const Value* value = i.next();
	while (value != NULL)
	{
		result = storeProperty(i.getID().c_str(), *value, &currentBuffer);
		value = i.next();
	}

	return result;
}


void OpenDDSConnection::findLength(const ValueMap& meta, size_t& length)
{
	// Account for the I32 which contains the number of Values in the meta object
	length = 4;

	ValueMap::Iterator itr;
	meta.reset(itr);
	
	const Value* value = itr.next();
	while (value != NULL)
	{
		// Store the size of the type ID
		length += gmsec::api::internal::GMSEC_TYPE_BYTES;

		// Store the size of the String ID
		length += gmsec::api::internal::GMSEC_LENGTH_BYTES + strlen(itr.getID().c_str()) + 1;

		if (value->isBoolean())
		{
			length += 1;
		}
		else if (value->isInteger())
		{
			length += 4;
		}
		else if (value->isReal())
		{
			length += 8;
		}
		else if (value->isString())
		{
			std::string valueString;
			value->getString(valueString);

			size_t count = valueString.length();

			if (count > size_t(gmsec::api::internal::GMSEC_STRING_LIMIT))
			{
				length = 0;

				std::ostringstream statusError;
				statusError << "Excessive string length for meta value: " << itr.getID();
				throw Exception(CONNECTION_ERROR, ENCODING_ERROR, statusError.str().c_str());
			}
			else
			{
				length += gmsec::api::internal::GMSEC_LENGTH_BYTES + count + 1;
			}
		}

		value = itr.next();
	}
}


Status OpenDDSConnection::storeProperty(const char* valueId, const Value& value, char** currentBuffer)
{
	Status result;

	Encoder* encoder = Encoder::getEncoder();

	// Note: Code based off of MessageEncoder::encode(Field &field)
	if (value.isBoolean())
	{
		// Store the value type
		GMSEC_U16 type = Field::BOOL_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		bool out = false;
		value.getBoolean(out);
		GMSEC_U8 tmp = out ? 1 : 0;
		encoder->putU8(&tmp, *currentBuffer);
	}
	else if (value.isInteger())
	{
		// Store the value type
		GMSEC_U16 type = Field::I32_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		GMSEC_I32 out = 0;
		value.getI32(out);
		encoder->putI32(&out, *currentBuffer);
	}
	else if (value.isReal())
	{
		// Store the value type
		GMSEC_U16 type = Field::F64_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		GMSEC_F64 out = 0;
		value.getF64(out);
		encoder->putF64(&out, *currentBuffer);
	}
	else if (value.isString())
	{
		// Store the value type
		GMSEC_U16 type = Field::STRING_TYPE;
		encoder->putU16(&type, *currentBuffer);

		// Store the name of the value
		encoder->putString(valueId, *currentBuffer);

		// Store the actual value
		std::string valueString;
		value.getString(valueString);
		encoder->putString(valueString.c_str(), *currentBuffer);
	}

	return result;
}


std::string OpenDDSConnection::getConnectionEndpoint()
{
	std::string endpoint = "unknown";

	if (!m_dcpsInfoRepo.empty())
	{
		endpoint = m_dcpsInfoRepo;
	}
	else if (!m_openddsConfig.empty())
	{
		std::ifstream configFile(m_openddsConfig.c_str());
		std::string   line;

		if (configFile)
		{
			while (std::getline(configFile, line))
			{
				size_t pos = line.find("DCPSInfoRepo=");

				if (pos != std::string::npos)
				{
					endpoint = line.substr(pos + std::string("DCPSInfoRepo=").length());
					break;
				}
			}
		}
	}

	return endpoint;
}

}
