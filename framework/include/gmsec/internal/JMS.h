/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file JMS.h
 *  @brief This file contains utilities for interfacing with JMS.
**/

#ifndef gmsec_internal_jms_h
#define gmsec_internal_jms_h


#include <jni.h>

#include <gmsec_defs.h>

#include <string>
#include <list>

namespace gmsec {

namespace jni {


const char TYPE_VOID[]     = "VOID";
const char TYPE_UNSET[]    = "UNSET";
const char TYPE_PUBLISH[]  = "PUBLISH";
const char TYPE_REQUEST[]  = "REQUEST";
const char TYPE_REPLY[]    = "REPLY";

const char GMSEC_PROP_TYPES[]   = "G_TYPES";
const char GMSEC_PROP_PREFIX[]  = "G_";
const char TOPIC_PROP_PREFIX[]  = "GT_";
const char JMS_PROP_PREFIX[]    = "JMS";


class JMSCache
{
public:

	enum Bits {
		B_CLASSLOADER   = 0x0001,
		B_CONTEXT       = 0x0002,
		B_CONNECTION    = 0x0004,
		B_MESSAGE       = 0x0010,
		B_SESSION       = 0x0100,
		B_CONSUMER      = 0x0200,
		B_PRODUCER      = 0x0400,
		B_DESTINATION   = 0x1000,
		B_END
	};

	JMSCache();
	~JMSCache();

	bool initialize(JNIEnv *env, bool global, int bits = 0xfffffff);

	jclass classClassLoader;

	jclass classHashtable;
	jmethodID methodHashtableInit;
	jmethodID methodHashtablePut;

	jclass classContext;
	jmethodID methodContextLookup;

	jclass classInitialContext;
	jmethodID methodInitialContextInit;

	jclass classConnectionFactory;
	jmethodID methodCreateConnection;

	jclass classConnection;
	jmethodID methodCreateSession;
	jmethodID methodConnectionStart;
	jmethodID methodConnectionClose;

	jclass classSession;
	jmethodID methodCreateConsumer;
	jmethodID methodCreateProducer;
	jmethodID methodSessionClose;
	jmethodID methodCreateBytesMessage;

	jclass classBytesMessage;
	jmethodID methodGetBodyLength;
	jmethodID methodReadBytes;
	jmethodID methodWriteBytes;

	jclass classConsumer;
	jmethodID methodConsumerClose;
	jmethodID methodConsumerReceive;

	jclass classProducer;
	jmethodID methodProducerClose;
	jmethodID methodProducerSend;

	jclass classDestination;

	jclass classMessage;
	jmethodID methodSetJMSType;
	jmethodID methodGetJMSType;
	jmethodID methodSetIntProperty;
	jmethodID methodSetStringProperty;


private:

	// Declared, but not implemented.
	JMSCache(const JMSCache &);
	JMSCache &operator=(const JMSCache &);

	bool _initialize(JNIEnv *env, bool global, int bits);

	// std::list<jobject> globals;

};



const char *kindToJMSType(GMSEC_MSG_KIND kind);
GMSEC_MSG_KIND jmsTypeToKind(const char *type);


/* ACK: another case for bolt::Options */
struct TopicProp
{
	std::string key;
	bool isInteger;

	int iValue;
	std::string sValue;
};

bool getTopicProperties(const std::string &topic, std::list<TopicProp> &props);

bool makeSelectorForTopic(const std::string &topic, std::string &selector);



} // namespace jni
} // namespace gmsec


#endif /* gmsec_internal_jms_h */

