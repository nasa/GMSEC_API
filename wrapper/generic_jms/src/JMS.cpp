/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file JMS.cpp
 *  @brief This file contains utilities for interfacing with JMS/JNI.
 *
 * This support is intended to be generally available (through the gmsec_jni
 * library) but it is not ready for v3.4.
 */


#include <gmsec4/internal/JMS.h>
#include <gmsec4/internal/JNI.h>
#include <gmsec4/internal/Subject.h>

#include <sstream>



namespace gmsec {
namespace api {
namespace jni {


using std::string;
using std::vector;

using namespace gmsec::api::internal;
using namespace gmsec::api::util;


JMSCache::JMSCache()
	: classClassLoader(0),

	  classHashtable(0),
	  methodHashtableInit(0),
	  methodHashtablePut(0),

	  classContext(0),
	  methodContextLookup(0),

	  classInitialContext(0),
	  methodInitialContextInit(0),

	  classConnectionFactory(0),
	  methodCreateConnection(0),
	  methodCreateConnectionCreds(0),

	  classConnection(0),
	  methodCreateSession(0),
	  methodConnectionStart(0),
	  methodConnectionClose(0),

	  classSession(0),
	  methodCreateConsumer(0),
	  methodCreateProducer(0),
	  methodSessionClose(0),
	  methodCreateBytesMessage(0),

	  classBytesMessage(0),
	  methodGetBodyLength(0),
	  methodReadBytes(0),
	  methodWriteBytes(0),

	  classConsumer(0),
	  methodConsumerClose(0),
	  methodConsumerReceive(0),

	  classProducer(0),
	  methodProducerClose(0),
	  methodProducerSend(0),

	  classDestination(0),

	  classMessage(0),
	  methodSetJMSType(0),
	  methodGetJMSType(0),
	  methodSetIntProperty(0),
	  methodSetStringProperty(0)
{
}


bool JMSCache::initialize(JNIEnv *env, bool global, int bits)
{
	bool ok = false;

	try
	{
		ok = _initialize(env, global, bits);
	}
	JNI_CATCH(env)

	return ok;
}


bool JMSCache::_initialize(JNIEnv *env, bool global, int bits)
{
	if (!checkJVM(env, "JMSCache::initialize"))
		return false;

	JNILookup lookup(env, global);

	if (bits & B_CLASSLOADER)
	{
		classClassLoader = lookup.getClass("java/lang/ClassLoader");
	}

	if (bits & B_CONTEXT)
	{
		classHashtable = lookup.getClass("java/util/Hashtable");
		methodHashtableInit = lookup.getMethod(classHashtable, "<init>", "()V");
		methodHashtablePut = lookup.getMethod(classHashtable, "put",
				"(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

		classContext = lookup.getClass("javax/naming/Context");
		methodContextLookup = lookup.getMethod(classContext,
				"lookup", "(Ljava/lang/String;)Ljava/lang/Object;");

		classInitialContext = lookup.getClass("javax/naming/InitialContext");
		methodInitialContextInit = lookup.getMethod(classInitialContext,
				"<init>", "(Ljava/util/Hashtable;)V");
	}

	if (bits & B_CONNECTION)
	{
		classConnectionFactory = lookup.getClass("javax/jms/ConnectionFactory");
		methodCreateConnection = lookup.getMethod(classConnectionFactory,
				"createConnection", "()Ljavax/jms/Connection;");

		methodCreateConnectionCreds = lookup.getMethod(classConnectionFactory,
				"createConnection", "(Ljava/lang/String;Ljava/lang/String;)Ljavax/jms/Connection;");

		classConnection = lookup.getClass("javax/jms/Connection");
		methodConnectionStart = lookup.getMethod(classConnection, "start", "()V");
		methodConnectionClose = lookup.getMethod(classConnection, "close", "()V");
		methodCreateSession = lookup.getMethod(classConnection,
				"createSession", "(ZI)Ljavax/jms/Session;");
	}

	if (bits & B_SESSION)
	{
		classSession = lookup.getClass("javax/jms/Session");
		methodCreateConsumer = lookup.getMethod(classSession, "createConsumer",
				"(Ljavax/jms/Destination;Ljava/lang/String;)Ljavax/jms/MessageConsumer;");
		methodCreateProducer = lookup.getMethod(classSession, "createProducer",
				"(Ljavax/jms/Destination;)Ljavax/jms/MessageProducer;");
		methodSessionClose = lookup.getMethod(classSession, "close", "()V");
		methodCreateBytesMessage = lookup.getMethod(classSession,
				"createBytesMessage", "()Ljavax/jms/BytesMessage;");
	}

	if (bits & B_MESSAGE)
	{
		classMessage = lookup.getClass("javax/jms/Message");
		methodGetJMSType = lookup.getMethod(classMessage,
				"getJMSType", "()Ljava/lang/String;");
		methodSetJMSType = lookup.getMethod(classMessage,
				"setJMSType", "(Ljava/lang/String;)V");
		methodSetIntProperty = lookup.getMethod(classMessage,
				"setIntProperty", "(Ljava/lang/String;I)V");
		methodSetStringProperty = lookup.getMethod(classMessage,
				"setStringProperty", "(Ljava/lang/String;Ljava/lang/String;)V");

		classBytesMessage = lookup.getClass("javax/jms/BytesMessage");
		methodGetBodyLength = lookup.getMethod(classBytesMessage,
				"getBodyLength", "()J");
		methodReadBytes = lookup.getMethod(classBytesMessage,
				"readBytes", "([BI)I");
		methodWriteBytes = lookup.getMethod(classBytesMessage,
				"writeBytes", "([BII)V");
	}

	if (bits & B_CONSUMER)
	{
		classConsumer = lookup.getClass("javax/jms/MessageConsumer");
		methodConsumerClose = lookup.getMethod(classConsumer, "close", "()V");
		methodConsumerReceive = lookup.getMethod(classConsumer,
				"receive", "(J)Ljavax/jms/Message;");
	}

	if (bits & B_PRODUCER)
	{
		classProducer = lookup.getClass("javax/jms/MessageProducer");
		methodProducerClose = lookup.getMethod(classProducer, "close", "()V");
		methodProducerSend = lookup.getMethod(classProducer,
				"send", "(Ljavax/jms/Message;)V");
	}

	// lookup.storeGlobals(globalReferences);

	return true;
}


const char* kindToJMSType(Message::MessageKind kind)
{
	const char* type = "VOID";

	switch (kind)
	{
	case Message::PUBLISH:
		type = TYPE_PUBLISH;
		break;
	case Message::REQUEST:
		type = TYPE_REQUEST;
		break;
	case Message::REPLY:
		type = TYPE_REPLY;
		break;
	}

	return type;
}


Message::MessageKind jmsTypeToKind(const char* jmsType)
{
	Message::MessageKind kind = Message::PUBLISH;

	if (jmsType)
	{
		if (StringUtil::stringEquals(jmsType, TYPE_PUBLISH))
		{
			kind = Message::PUBLISH;
		}
		else if (StringUtil::stringEquals(jmsType, TYPE_REQUEST))
		{
			kind = Message::REQUEST;
		}
		else if (StringUtil::stringEquals(jmsType, TYPE_REPLY))
		{
			kind = Message::REPLY;
		}
	}

	return kind;
}


bool makeSelectorForTopic(const string &topic, string &selector)
{
	vector<string> elements;

	if (!Subject::getElements(topic, elements))
		return false;

	size_t count = elements.size();
	const string prefix(TOPIC_PROP_PREFIX);

	std::ostringstream work;

	if (elements[count-1] == ">"){
		work << prefix << "N>=" << count;
	}else if(elements[count-1] == "+"){
		work << prefix << "N>=" << count - 1;
	}else{
		work << prefix << "N=" << count;
	}

	for (size_t i = 0; i < count; ++i)
	{
		string e = elements[i];
		if (e != "*" && e != ">" && e != "+")
			work << " AND " << prefix << "E" << i << "='" << e << '\'';
	}

	selector = work.str();

	return true;
}

bool getTopicProperties(const string &topic, std::list<TopicProp> &props)
{
	vector<string> elements;

	if (!Subject::getElements(topic, elements))
		return false;

	TopicProp p;

	// add the element count property
	p.isInteger = true;
	p.key = string(TOPIC_PROP_PREFIX) + "N";
	p.iValue = int(elements.size());
	props.push_back(p);

	// add a string property for each topic element
	p.isInteger = false;
	for (size_t i = 0; i < elements.size(); ++i)
	{
		string e = elements[i];

		std::ostringstream os;
		os << TOPIC_PROP_PREFIX << 'E' << i;
		p.key = os.str();
		p.sValue = elements[i];

		props.push_back(p);
	}

	return true;
}


} // namespace jni
} // namespace api
} // namespace gmsec
