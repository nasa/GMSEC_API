/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file JMSConnection.cpp
 *
 *  @brief This file contains the implementation for the Generic JMS Connection.
 */

// Preprocessor definition to ignore Visual Studio warnings
// indicating that *_s functions should be used instead.
#define _CRT_SECURE_NO_DEPRECATE

#include <JMSConnection.h>
#include <GenericJNI.h>

#include <gmsec5/ConfigOptions.h>

#include <gmsec5/internal/Encoder.h>
#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/InternalMessageFactory.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/Rawbuf.h>
#include <gmsec5/internal/SystemUtil.h>

#include <gmsec5/GmsecException.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/TimeUtil.h>

#include <sstream>


#define NONE                "NONE"
#define GENERIC_JMS_REPLY   "GENERIC_JMS_REPLY_ADDR"
#define GENERIC_JMS_SUBJECT "GMSEC_SUBJECT_GENERIC_JMS"
#define GENERIC_TMP_SUBJECT "GMSEC.BOGUS.SUBJECT"

#define JMS_111             111   // Connection not specified
#define JMS_122             122   // JMS internal error
#define JMS_194             194   // JMS session object is invalid 


using namespace gmsec::api5;
using namespace gmsec::api5::util;
using namespace gmsec::api5::internal;

using namespace gmsec_generic_jms;

static const int DEBUG_JMS_TYPE = 0;


// --- MessagePullState -----------------------------------------------

void runPullThread(MessagePullState* mpt)
{
	mpt->run();
}


MessagePullState::MessagePullState()
	:
	shouldStop(false),
	shutdownLatch(1),
	stopListening(false),
	isRunning(false),
	parent(NULL),
	envPoint(NULL),
	jvm(NULL),
	masterConsumer(NULL),
	newConsumer(NULL),
	cache(NULL)
{
	GMSEC_DEBUG << "+ MPT:" << this << " constructor";
}


MessagePullState::~MessagePullState()
{
	GMSEC_DEBUG << "~ MPT:" << this << " destructor";
}


void MessagePullState::initialize(JMSConnection* conn, JavaVM* parentjvm, const JMSCache* c)
{
	parent = conn;
	newConsumer = NULL;
	jvm = parentjvm;
	cache = c;
}


void MessagePullState::stop()
{
	GMSEC_DEBUG << "~ MPT:" << this << " stop";
	shouldStop.set(true);
	shutdownLatch.await(50);
}


void MessagePullState::run()
{
	GMSEC_VERBOSE << "MPT:" << this << ':' << fID.c_str() << "> starting";

	envPoint = attachMe(jvm);

	if (!envPoint)
	{
		GMSEC_ERROR << "Unable to attach to the JVM!";
		return;
	}
	isRunning = true;

	while (!shouldStop.get())
	{
		try
		{
			//
			// The checkJVM() calls in the following section can throw a
			// StatusException, but an easy way to trigger such failures in
			// a unit test was not found.
			//
			if (newConsumer || stopListening)
			{
				AutoMutex lock(consumerSwitch.getMutex());
				GMSEC_DEBUG << "MPT:" << this << ':' << fID.c_str() << "> switching consumer to " << pendingSelector.c_str();
				if (masterConsumer)
				{
					envPoint->CallVoidMethod(masterConsumer, cache->methodConsumerClose);
					checkJVM(envPoint, "close masterConsumer");
					envPoint->DeleteGlobalRef(masterConsumer);
				}
	
				masterConsumer = newConsumer;
				newConsumer = NULL;
				stopListening = false;
				consumerSwitch.signal(Condition::USER);
			}
	
			jlong timeout = 100;
			jobject msg = NULL;
			if (masterConsumer)
			{
				msg = envPoint->CallObjectMethod(masterConsumer, cache->methodConsumerReceive, timeout);
	
				if (msg != NULL)
				{
					parent->handle(envPoint, msg);
					checkJVM(envPoint, "postHandle");
					envPoint->DeleteLocalRef(msg);
				}
			}
	
			TimeUtil::millisleep(1);
		}
		catch (const GmsecException& e)
		{
			GMSEC_ERROR << "In MessagePullState::run(), exception - " << e.what();
		}
	}

	detachMe(jvm);

	shutdownLatch.countDown();

	GMSEC_VERBOSE << "MPT:" << this << " done";
}


void MessagePullState::setMasterConsumer(jobject consumer, const std::string& s)
{
	AutoMutex lock(consumerSwitch.getMutex());
	pendingSelector = s;
	newConsumer = consumer;
	if (newConsumer == NULL)
	{
		stopListening = true;
	}
	consumerSwitch.wait(2000);
}


/*
    @brief JByteArrayManager assists in conversions from jbyteArray to jbyte *.
    The destructor releases the jbyte * in the event of an exception.
*/
class JByteArrayManager
{
private:
	JNIEnv*    jenv;
	jbyteArray array;
	jbyte*     p;
	bool       owned;

public:
	JByteArrayManager(JNIEnv *e, jbyteArray a, bool ownership)
		: jenv(e), array(a), p(0), owned(ownership)
	{
		if (array)
			p = jenv->GetByteArrayElements(array, 0);
	}

	~JByteArrayManager()
	{
		if (p)
			jenv->ReleaseByteArrayElements(array, p, 0);
		p = 0;

		if (owned && array)
			jenv->DeleteLocalRef(array);

		owned = false;
		array = 0;
	}

	const jbyte *get() const
	{
		return p;
	}
};



static void mwConfig(const Config& config, const char* key, std::string& out, bool requirePrefix = false)
{
	std::string prefixedKey = std::string("MW-") + key;

	const char* value = config.getValue(prefixedKey.c_str());

	if (value != 0)
	{
		out = value;
	}
	else if (!requirePrefix)
	{
		value = config.getValue(key);

		if (value != 0)
		{
			out = value;
		}
	}
}


static Mutex& getClassMutex()
{
	static Mutex m;
	return m;
}


static const JMSCache* getCache(JNIEnv* env)
{
	static int initialized = 0;
	static JMSCache *cache = 0;

	if (initialized)
		return cache;

	try
	{
		JMSCache *tmp = new JMSCache();
		tmp->initialize(env, true);

		cache = tmp;
	}
	JNI_CATCH(env)

	// it has been initialized, though possibly unsuccessfully...
	initialized = 1;

	return cache;
}


JMSConnection::JMSConnection(const Config& config)
	:
	persistent(false),
	uniquecounter(0),
	requestCounter(0),
	jvm(0),
	connectionFactory(0),
	connection(0),
	masterDestination(0),
	inputSession(0),
	outputSession(0),
	masterProducer(0),
	cache(0),
	pullThread(&runPullThread, &pullState)
{
	static int CID = 0;

	++CID;
	{
		Rawbuf<64> raw;
		raw.stream() << CID;
		fID = raw.str();
	}

	std::string lookupFactory = "ConnectionFactory";
	std::string lookupTopic   = "DefaultTopic";

	uniquecounter = 0;

	std::string            jvmClasspath;
	std::list<std::string> jvmArgs;
	bool                   jvmIgnoreUnrecognized = false;

	// Try to get the needed values out of the config object.
	mwConfig(config, "username", username);
	mwConfig(config, "password", password);

	mwConfig(config, "initial-context-factory", initial_context_factory, true);
	mwConfig(config, "provider-url", provider_url, true);

	mwConfig(config, "lookup-connection-factory", lookupFactory, true);
	mwConfig(config, "lookup-topic", lookupTopic, true);

	persistent = config.getBooleanValue(JMS_PERSISTENT, false);

	mwConfig(config, "JVM-CLASSPATH", jvmClasspath, true);
	jvmIgnoreUnrecognized = config.getBooleanValue(JMS_JVM_IGNORE_UNRECOGNIZED, false);

	std::string key;
	std::string value;
	for (int i = 0; ; ++i)
	{
		Rawbuf<64> buffer;
		buffer.stream() << "JVM-ARG" << i;
		key = buffer.str();
		value = "";
		mwConfig(config, key.c_str(), value, true);
		if (value.length())
		{
			jvmArgs.push_back(value);
		}
		else
		{
			break;
		}
	}

	if (jvmClasspath == "")
	{
		const char* cp = std::getenv("CLASSPATH");
		if (cp)
		{
			jvmClasspath = cp;
		}
		else
		{
			jvmClasspath = "CLASSPATH_NOT_SET";
			GMSEC_WARNING << "neither MW-JVM-CLASSPATH nor CLASSPATH environment set";
		}
	}

	AutoMutex hold(getClassMutex());
	static bool first = true;

	const int MAX_JVMS = 4;
	JavaVM* jvms[MAX_JVMS] = { NULL };
	jsize count = 0;
	int code = JNI_GetCreatedJavaVMs(jvms, MAX_JVMS, &count);
	GMSEC_DEBUG << "getJavaVM: JNI_GetCreatedJavaVMs=" << code << " count=" << count;
	JNIEnv* env = 0;

	if (count == 1)
	{
		jvm = jvms[0];
		int code = jvm->AttachCurrentThread((void **) &env, NULL);
		if (code < 0 || !env)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, code, "getJNIEnv: attach failed");
		}

		// this potentially has to move for Java
		if (!first)
		{
			updateClassLoader(env);
		}
	}
	else if (count > 1)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_111, "getJavaVM: multiple JVMs!");
	}
	else
	{
		jvmArgs.push_front("-Djava.compiler=NONE");
		jvmArgs.push_front("-Djava.class.path=" + jvmClasspath);
#if !defined(JNI_VERSION_1_8) && defined(JNI_VERSION_1_6)
		jvmArgs.push_front("-Xss2m");  // Set JVM stack size; see https://issues.apache.org/jira/browse/DAEMON-363
#endif

		JavaVMOption *options = new JavaVMOption[jvmArgs.size()];
		int n = 0;
		GMSEC_VERBOSE << "initializing JVM with arguments:";
		for (std::list<std::string>::const_iterator i = jvmArgs.begin(); i != jvmArgs.end(); ++i, ++n)
		{
			options[n].optionString = (char*) i->c_str();
			GMSEC_VERBOSE << "\t" << i->c_str();
		}

		JavaVMInitArgs vm_args;
#if defined JNI_VERSION_1_8
		vm_args.version = JNI_VERSION_1_8;
#elif defined JNI_VERSION_1_6
		vm_args.version = JNI_VERSION_1_6;
#else
		vm_args.version = JNI_VERSION_1_4;
#endif
		vm_args.options = &options[0];
		vm_args.nOptions = n;
		vm_args.ignoreUnrecognized = jvmIgnoreUnrecognized ? JNI_TRUE : JNI_FALSE;

		jint result = JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);

		delete [] options;

		if (result == JNI_ERR || !env)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, result, "Error invoking the JVM");
		}
	}

	first = false;
	cache = getCache(env);
	if (!cache)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_111, "Error installing JMS cache");
	}

	GmsecException error(NO_ERROR_CLASS, NO_ERROR_CODE, "");
	try
	{
		//
		// checkJVM() calls in the following section can trigger a
		// StatusException(); the catching of such exceptions is done
		// in unit test 0051.MiddlewareError.cpp
		//

		// Retrieve ConnectionFactory
		jobject hashtable = env->NewObject(cache->classHashtable,
				cache->methodHashtableInit);

		env->CallObjectMethod(hashtable,
				cache->methodHashtablePut,
				env->NewStringUTF("java.naming.factory.initial"),
				env->NewStringUTF(initial_context_factory.c_str()));

		env->CallObjectMethod(hashtable,
				cache->methodHashtablePut,
				env->NewStringUTF("java.naming.provider.url"),
				env->NewStringUTF(provider_url.c_str()));

		jobject context = env->NewObject(cache->classInitialContext,
				cache->methodInitialContextInit,
				hashtable);
		checkJVM(env, "InitialContext.new(Hashtable)");

		jobject tmp = env->CallObjectMethod(context,
				cache->methodContextLookup,
				env->NewStringUTF(lookupFactory.c_str()));
		checkJVM(env, "InitialContext.lookup");
		GMSEC_VERBOSE << "ConnectionFactory[lookup=" << lookupFactory.c_str() << "] => " << tmp;

		connectionFactory = makeGlobalRef(env, tmp);

		tmp = env->CallObjectMethod(context,
				cache->methodContextLookup,
				env->NewStringUTF(lookupTopic.c_str()));
		GMSEC_VERBOSE << "masterDestination[lookup=" << lookupTopic.c_str() << "] => " << tmp;
		masterDestination = makeGlobalRef(env, tmp);
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "In JMSConnection Constructor, exception [ " << e.what() << " ]";
		error = e;
	}
	JNI_CATCH(env)

	detachMe(jvm);

	if (error.getErrorClass() != NO_ERROR_CLASS)
	{
		throw error;
	}
}


JMSConnection::~JMSConnection()
{
	if (connection != NULL)
	{
		mwDisconnect();
	}

	GMSEC_VERBOSE << "~JMSConnection:" << fID.c_str();

	pullState.stop();

	while (!queue.empty())
	{
		gmsec::api5::Message* msg = queue.front();
		delete msg;
		queue.pop();
	}

	JNIEnv* envLocal = attachMe(jvm);

	if (envLocal)
	{
		envLocal->DeleteGlobalRef(connectionFactory);
		envLocal->DeleteGlobalRef(masterDestination);
	}

	detachMe(jvm);
}


const char* JMSConnection::getLibraryRootName()
{
	return JMS_VERSION_STRING;
}


const char* JMSConnection::getLibraryVersion()
{
	return JMS_VERSION_STRING;
}


const char* JMSConnection::getMWInfo()
{
	if (mwInfo.empty())
	{
		const char* info = getLibraryVersion();

		if (info)
		{
			mwInfo = info;
		}
		else
		{
			mwInfo = "NOT AVAILABLE";
		}
	}

	return mwInfo.c_str();
}



void JMSConnection::mwConnect()
{
	if (!connectionFactory)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_111, "ConnectionFactory not available");
	}

	JNIEnv* envLocal = attachMe(jvm);

	if (!envLocal)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_122, "Unable to attach to the JVM");
	}

	// initialize request specs
	requestSpecs = getExternal().getRequestSpecs();

	try
	{
		jobject tmp = 0;
		if (!username.empty() && !password.empty())
		{
			tmp = envLocal->CallObjectMethod(connectionFactory, cache->methodCreateConnectionCreds,
			                                 envLocal->NewStringUTF(username.c_str()),
			                                 envLocal->NewStringUTF(password.c_str()));
		}
		else
		{
			tmp = envLocal->CallObjectMethod(connectionFactory, cache->methodCreateConnection);
		}
		checkJVM(envLocal, "ConnectionFactory.createConnection");

		connection = makeGlobalRef(envLocal, tmp);
		if (connection == NULL)
		{
			GMSEC_ERROR << "Error establishing JMS connection";
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_111, "Error establishing JMS connection");
		}

		tmp = envLocal->CallObjectMethod(connection, cache->methodCreateSession, false, 1);//Session.AUTO_ACKNOWLEDGE=1
		inputSession = makeGlobalRef(envLocal, tmp);
		if (inputSession == NULL)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_194, "Error establishing JMS input session");
		}

		tmp = envLocal->CallObjectMethod(connection, cache->methodCreateSession, false, 1);//Session.AUTO_ACKNOWLEDGE=1
		outputSession = makeGlobalRef(envLocal, tmp);
		if (outputSession == NULL)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_194, "Error establishing JMS output session");
		}

		tmp = envLocal->CallObjectMethod(outputSession, cache->methodCreateProducer, masterDestination);
		masterProducer = makeGlobalRef(envLocal, tmp);
		if (masterProducer == NULL)
		{
			throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_122, "Error establishing JMS masterProducer");
		}

		envLocal->CallVoidMethod(connection, cache->methodConnectionStart);

		pullState.initialize(this, jvm, cache);
		if (!pullState.getIsRunning())
		{
			pullState.setID(fID);
			pullThread.start();
		}
	}
	catch (const GmsecException& e)
	{
		detachMe(jvm);
		throw e;
	}
	JNI_CATCH(envLocal)

	detachMe(jvm);

	getExternal().setConnectionEndpoint(provider_url);
}


void JMSConnection::mwDisconnect()
{
	namesToSelector.clear();
	pullState.setMasterConsumer(NULL);

	JNIEnv* envLocal = attachMe(jvm);

	if (envLocal)
	{
		envLocal->CallObjectMethod(masterProducer, cache->methodProducerClose);
		envLocal->CallObjectMethod(inputSession, cache->methodSessionClose);
		envLocal->CallObjectMethod(outputSession, cache->methodSessionClose);
		envLocal->CallObjectMethod(connection, cache->methodConnectionClose);

		envLocal->DeleteGlobalRef(masterProducer);
		envLocal->DeleteGlobalRef(inputSession);
		envLocal->DeleteGlobalRef(outputSession);
		envLocal->DeleteGlobalRef(connection);
		connection = NULL;
	}

	detachMe(jvm);
}


void JMSConnection::mwSubscribe(const char* topic, const Config& config)
{
	std::string selector;
	makeSelectorForTopic(topic, selector);

	namesToSelector[topic] = selector;

	resetConsumer();
}


void JMSConnection::mwUnsubscribe(const char* subject)
{
	if (namesToSelector.find(subject) == namesToSelector.end())
	{
		throw GmsecException(CONNECTION_ERROR, INVALID_SUBJECT_NAME, "Not subscribed to subject");
	}

	namesToSelector.erase(subject);

	resetConsumer();
}


void JMSConnection::mwPublish(const Message& msg, const Config& config)
{
	JNIEnv* envLocal = attachMe(jvm);

	if (!envLocal)
	{
		throw GmsecException(MIDDLEWARE_ERROR, CUSTOM_ERROR_CODE, JMS_122, "Unable to attach to the JVM");
	}
	
	std::string   subject = msg.getSubject();
	Message::Kind kind    = msg.getKind();

	try
	{
		//
		// The checkJVM() calls in the following block can throw
		// a StatusException, but an easy method to trigger
		// errors in a unit test was not found.
		//
		jobject bytesMsg = envLocal->CallObjectMethod(outputSession, cache->methodCreateBytesMessage);
		if (bytesMsg == NULL)
		{
			throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error creating JMS Bytes Message");
		}

		DataBuffer buffer;
		prepareBytes(msg, buffer);

		jbyteArray byteArr = envLocal->NewByteArray(static_cast<jsize>(buffer.size()));
		jbyte *jbytearr = (jbyte *) buffer.get();
		envLocal->SetByteArrayRegion(byteArr, 0, static_cast<jsize>(buffer.size()), jbytearr);
		envLocal->CallVoidMethod(bytesMsg, cache->methodWriteBytes, byteArr, 0, buffer.size());
		checkJVM(envLocal, "JMSBytesMessage.writeBytes");

#if 0
		envLocal->DeleteLocalRef(byteArr);
		checkJVM(envLocal, "DeleteLocalRef(byteArr)");
#endif

		const char* type = kindToJMSType(kind);
		envLocal->CallVoidMethod(bytesMsg, cache->methodSetJMSType, envLocal->NewStringUTF(type));
		checkJVM(envLocal, "setJMSType");

		addPropertiesToMessage(subject, bytesMsg, envLocal);
		envLocal->CallVoidMethod(masterProducer, cache->methodProducerSend, bytesMsg);
		checkJVM(envLocal, "JMSProducer.send");
		envLocal->DeleteLocalRef(bytesMsg);
	}
	catch (const GmsecException& e)
	{
		detachMe(jvm);
		throw e;
	}
	JNI_CATCH(envLocal)

	detachMe(jvm);
}


void JMSConnection::mwRequest(const Message& request, std::string& id)
{
	++requestCounter;

	id = generateUniqueId(requestCounter);

	MessageBuddy::getInternal(request).addField(GMSEC_REPLY_UNIQUE_ID_FIELD, id.c_str());

	mwPublish(request, getExternal().getConfig());
}


void JMSConnection::mwReceive(Message*& msg, GMSEC_I32 timeout)
{
	double start_s;
	bool   first = true;
	bool   done  = false;

	msg = NULL;

	if (timeout != GMSEC_WAIT_FOREVER)
	{
		start_s = TimeUtil::getCurrentTime_s();
	}

	while (!done)
	{
		AutoMutex inner(queueCondition.getMutex());
		int reason = 0;

		if (!queue.empty())
		{
			// have a message, don't need to wait
			reason = GOT_MESSAGE;
		}
		else if (timeout == GMSEC_WAIT_FOREVER)
		{
			reason = queueCondition.wait();
		}
		else
		{
			double elapsed_ms = 1000 * (TimeUtil::getCurrentTime_s() - start_s);
			int timeout_ms = int(timeout - elapsed_ms);

			if (timeout_ms < 1 && first)
			{
				first = false;
				timeout_ms = 1;
			}
			if (timeout_ms > 0)
			{
				reason = queueCondition.wait(timeout_ms);
			}
			else
			{
				// timeout (not an error)
				done = true;
			}
		}

		if (reason == GOT_MESSAGE)
		{
			if (!queue.empty())
			{
				msg = queue.front();
				done = true;
				queue.pop();
			}
		}
	}
}


bool JMSConnection::handle(JNIEnv* envPoint, jobject msg)
{
	jlong length = envPoint->CallLongMethod(msg, cache->methodGetBodyLength);

	if (length == 0)
	{
		GMSEC_WARNING << "Improperly formatted message";
		return false;
	}

	Message::Kind kind = Message::Kind::PUBLISH;

	jobject jmsType = envPoint->CallObjectMethod(msg, cache->methodGetJMSType);

	if (jmsType)
	{
		JStringManager manager(envPoint, jstring(jmsType));

		const char* q = manager.c_str();

		if (!q)
		{
			q = "(null)";
		}

		kind = jmsTypeToKind(q);

		if (DEBUG_JMS_TYPE)
		{
			GMSEC_VERBOSE << "handle: JMSType='" << q << "'" << " kind=" << static_cast<int>(kind);
		}
	}
	else
	{
		GMSEC_WARNING << "Message missing JMS type; assuming TYPE_PUBLISH";
	}

	jbyteArray byteArr = envPoint->NewByteArray((jsize) length);
	int bytesRead = envPoint->CallIntMethod(msg, cache->methodReadBytes, byteArr, length);

	JByteArrayManager manager(envPoint, byteArr, true);

	DataBuffer  buffer((GMSEC_U8*) manager.get(), bytesRead, false);
	Message*    gmsecMsg = 0;
	std::string inSubject;

	unloadBytes(buffer, kind, gmsecMsg, inSubject);

	MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).addMessageTemplate(*gmsecMsg);

	if (kind == Message::Kind::REPLY)
	{
		Message* reply = gmsecMsg;

		// we need to clone the message since ownership of
		// gmsecMsg is being transferred to handleReply
		gmsecMsg = new Message(*reply);

		handleReply(reply);
	}

	if (gmsecMsg)
	{
		AutoMutex queueHold(queueCondition.getMutex());

		queue.push(gmsecMsg);

		queueCondition.signal(JMSConnection::GOT_MESSAGE);
	}

	return true;
}


void JMSConnection::handleReply(Message* gmsecMessage)
{
	getExternal().onReply(gmsecMessage);
}


void JMSConnection::prepareBytes(const Message& msg, DataBuffer& buffer)
{
	MessageBuddy::getInternal(msg).addField(GENERIC_JMS_SUBJECT, msg.getSubject());

	getExternal().getPolicy().encode(const_cast<Message&>(msg), buffer);

	MessageBuddy::getInternal(msg).clearField(GENERIC_JMS_SUBJECT);
}


void JMSConnection::unloadBytes(const DataBuffer& buffer, Message::Kind kind, Message*& gmsecMessage, std::string& rawSubject)
{
	StdUniquePtr<Message> msg( MessageFactoryBuddy::getInternal(getExternal().getMessageFactory()).newMessage() );
	ValueMap              meta;

	msg->setConfig(getExternal().getMessageConfig());

	Status status = getExternal().getPolicy().unpackage(*msg.get(), buffer, meta);

	if (status.hasError())
	{
		throw GmsecException(status);
	}

	try {
		const StringField& jmsSubject = msg->getStringField(GENERIC_JMS_SUBJECT);

		rawSubject = jmsSubject.getValue();

		MessageBuddy::getInternal(*msg.get()).setSubject(jmsSubject.getValue());

		msg->clearField(GENERIC_JMS_SUBJECT);
	}
	catch (...) {
		throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Unpackaged message is missing Generic JMS Subject");
	}

	gmsecMessage = msg.release();
}


std::string JMSConnection::generateUniqueId()
{
	Rawbuf<128> buffer;
	buffer.stream() << getExternal().getID() << '_' << ++uniquecounter;
	return buffer.str();
}


std::string JMSConnection::generateUniqueId(long id)
{
	Rawbuf<128> buffer;
	buffer.stream() << getExternal().getID() << '_' << ++uniquecounter << '_' << id;
	return buffer.str();
}


void JMSConnection::resetConsumer()
{
	if (namesToSelector.size() == 0)
	{
		pullState.setMasterConsumer(NULL);
		return;
	}

	try
	{
		std::string subscribeStr;
		combineSelectors(subscribeStr);
	
		JNIEnv* envLocal = attachMe(jvm);

		if (!envLocal)
		{
			throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, JMS_122, "Unable to attach to the JVM");
		}
	
		jobject tmp = envLocal->CallObjectMethod(inputSession,
		                                         cache->methodCreateConsumer,
		                                         masterDestination,
		                                         envLocal->NewStringUTF(subscribeStr.c_str())); 
		checkJVM(envLocal, "Session.createConsumer");
	
		jobject consumer = makeGlobalRef(envLocal, tmp);
		if (consumer == NULL)
		{
			throw GmsecException(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "Error establishing JMS consumer");
		}
	
		pullState.setMasterConsumer(consumer, subscribeStr);
	}
	catch (const GmsecException& e)
	{
		detachMe(jvm);
		throw e;
	}

	detachMe(jvm);
}


bool JMSConnection::addPropertiesToMessage(const std::string& topic, jobject msg, JNIEnv* env)
{
	std::list<TopicProp> props;

	if (!getTopicProperties(topic, props))
	{
		return false;
	}

	try
	{
		for (std::list<TopicProp>::iterator i = props.begin(); i != props.end(); ++i)
		{
			TopicProp p = *i;
	
			if (p.isInteger)
			{
				env->CallVoidMethod(msg, cache->methodSetIntProperty, env->NewStringUTF(p.key.c_str()), p.iValue);
			}
			else
			{
				env->CallVoidMethod(msg, cache->methodSetStringProperty,
				                    env->NewStringUTF(p.key.c_str()),
				                    env->NewStringUTF(p.sValue.c_str()));
			}
	
			checkJVM(env, "applying properties");
		}
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "In JMSConnection::addPropertiesToMessage(), exception [ " << e.what() << " ]";
		return false;
	}

	return true;
}


bool JMSConnection::combineSelectors(std::string& combined)
{
	std::map<std::string, std::string>::iterator iter = namesToSelector.begin();

	if (namesToSelector.size() == 1)
	{
		combined = iter->second;
	}
	else
	{
		std::ostringstream work;
		bool first = true;
		for (; iter != namesToSelector.end(); ++iter)
		{	
			if (!first)
				work << " OR ";

			first = false;

			work << "(" << iter->second << ")";
		}
		combined = work.str();
	}

	return true;
}
