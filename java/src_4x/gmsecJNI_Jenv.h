/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#ifndef GMSEC_JNI_UTILS_H
#define GMSEC_JNI_UTILS_H

#if defined(__GNUC__)
typedef long long __int64; /* to satisfy gcc on Windows */
#endif

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/Connection.h>
#include <gmsec4/Exception.h>
#include <gmsec4/HeartbeatGenerator.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>

#include <jni.h>

#include <stdexcept>
#include <string>
#include <cstring>


#define JNI_POINTER_TO_JLONG(x)            reinterpret_cast<jlong>(x)
#define JNI_JLONG_TO_STATUS(x)             reinterpret_cast<gmsec::api::Status *>(x)
#define JNI_JLONG_TO_AUTO_DISPATCHER(x)    reinterpret_cast<gmsec::api::AutoDispatcher*>(x)
#define JNI_JLONG_TO_CONFIG_FILE(x)        reinterpret_cast<gmsec::api::ConfigFile *>(x)
#define JNI_JLONG_TO_CONFIG_FILE_ITER(x)   reinterpret_cast<gmsec::api::ConfigFileIterator *>(x)
#define JNI_JLONG_TO_CONFIG(x)             reinterpret_cast<gmsec::api::Config *>(x)
#define JNI_JLONG_TO_CONNECTION(x)         reinterpret_cast<gmsec::api::Connection *>(x)
#define JNI_JLONG_TO_MESSAGE(x)            reinterpret_cast<gmsec::api::Message *>(x)
#define JNI_JLONG_TO_MSG_FIELD_ITER(x)     reinterpret_cast<gmsec::api::MessageFieldIterator *>(x)

#define JNI_JLONG_TO_HEARTBEAT_GENERATOR(x) reinterpret_cast<gmsec::api::HeartbeatGenerator*>(x)

#define JNI_JLONG_TO_CONFIG_ENTRY(x)       reinterpret_cast<gmsec::api::ConfigFile::ConfigEntry *>(x)
#define JNI_JLONG_TO_MESSAGE_ENTRY(x)      reinterpret_cast<gmsec::api::ConfigFile::MessageEntry *>(x)
#define JNI_JLONG_TO_SUBSCRIPTION_ENTRY(x) reinterpret_cast<gmsec::api::ConfigFile::SubscriptionEntry *>(x)

#define JNI_JLONG_TO_FIELD(x)              reinterpret_cast<gmsec::api::Field *>(x)
#define JNI_JLONG_TO_BINARYFIELD(x)        reinterpret_cast<gmsec::api::BinaryField*>(x)
#define JNI_JLONG_TO_BOOLEANFIELD(x)       reinterpret_cast<gmsec::api::BooleanField*>(x)
#define JNI_JLONG_TO_CHARFIELD(x)          reinterpret_cast<gmsec::api::CharField*>(x)
#define JNI_JLONG_TO_F32FIELD(x)           reinterpret_cast<gmsec::api::F32Field*>(x)
#define JNI_JLONG_TO_F64FIELD(x)           reinterpret_cast<gmsec::api::F64Field*>(x)
#define JNI_JLONG_TO_I8FIELD(x)            reinterpret_cast<gmsec::api::I8Field*>(x)
#define JNI_JLONG_TO_I16FIELD(x)           reinterpret_cast<gmsec::api::I16Field*>(x)
#define JNI_JLONG_TO_I32FIELD(x)           reinterpret_cast<gmsec::api::I32Field*>(x)
#define JNI_JLONG_TO_I64FIELD(x)           reinterpret_cast<gmsec::api::I64Field*>(x)
#define JNI_JLONG_TO_U8FIELD(x)            reinterpret_cast<gmsec::api::U8Field*>(x)
#define JNI_JLONG_TO_U16FIELD(x)           reinterpret_cast<gmsec::api::U16Field*>(x)
#define JNI_JLONG_TO_U32FIELD(x)           reinterpret_cast<gmsec::api::U32Field*>(x)
#define JNI_JLONG_TO_U64FIELD(x)           reinterpret_cast<gmsec::api::U64Field*>(x)
#define JNI_JLONG_TO_STRINGFIELD(x)        reinterpret_cast<gmsec::api::StringField*>(x)

#define JNI_JLONG_TO_SUBSCRIPTION_INFO(x)  reinterpret_cast<gmsec::api::SubscriptionInfo*>(x)

#define JNI_JLONG_TO_TIMESPEC(x)           reinterpret_cast<GMSEC_TimeSpec*>(x)
#define JNI_JLONG_TO_LOGHANDLER(x)         reinterpret_cast<gmsec::api::util::LogHandler*>(x)

#define JNI_JLONG_TO_CONNECTION_MANAGER(x) reinterpret_cast<gmsec::api::mist::ConnectionManager*>(x)
#define JNI_JLONG_TO_DEVICE(x)             reinterpret_cast<gmsec::api::mist::Device*>(x)
#define JNI_JLONG_TO_DEVICE_ITER(x)        reinterpret_cast<gmsec::api::mist::DeviceIterator*>(x)
#define JNI_JLONG_TO_DEVICE_PARAM(x)       reinterpret_cast<gmsec::api::mist::DeviceParam*>(x)
#define JNI_JLONG_TO_MNEMONIC(x)           reinterpret_cast<gmsec::api::mist::Mnemonic*>(x)
#define JNI_JLONG_TO_MNEMONIC_ITER(x)      reinterpret_cast<gmsec::api::mist::MnemonicIterator*>(x)
#define JNI_JLONG_TO_MNEMONIC_SAMPLE(x)    reinterpret_cast<gmsec::api::mist::MnemonicSample*>(x)
#define JNI_JLONG_TO_PRODUCT_FILE(x)       reinterpret_cast<gmsec::api::mist::ProductFile*>(x)
#define JNI_JLONG_TO_PRODUCT_FILE_ITER(x)  reinterpret_cast<gmsec::api::mist::ProductFileIterator*>(x)
#define JNI_JLONG_TO_SERVICE_PARAM(x)      reinterpret_cast<gmsec::api::mist::ServiceParam*>(x)
#define JNI_JLONG_TO_SCHEMA_ID_ITER(x)     reinterpret_cast<gmsec::api::mist::SchemaIDIterator*>(x);
#define JNI_JLONG_TO_SPECIFICATION(x)      reinterpret_cast<gmsec::api::mist::Specification*>(x);

#define JNI_JLONG_TO_CONN_MGR_SUBSCRIPTION_INFO(x)  reinterpret_cast<gmsec::api::mist::SubscriptionInfo*>(x)

#define JNI_JLONG_TO_DEVICE_MSG(x)         reinterpret_cast<gmsec::api::mist::message::DeviceMessage*>(x)
#define JNI_JLONG_TO_MISTMESSAGE(x)        reinterpret_cast<gmsec::api::mist::message::MistMessage*>(x)
#define JNI_JLONG_TO_MNEMONIC_MSG(x)       reinterpret_cast<gmsec::api::mist::message::MnemonicMessage*>(x)
#define JNI_JLONG_TO_PRODUCT_FILE_MSG(x)   reinterpret_cast<gmsec::api::mist::message::ProductFileMessage*>(x)


#ifdef WIN32
	#ifdef GMSEC_VC6
		#define FPRINTF fprintf
	#else
		#define FPRINTF fprintf_s
	#endif
#else
	#define FPRINTF fprintf
#endif


/* The JNI_CATCH macro prevents any exceptions from propagating
   through a JNI call.  The JVM knows nothing about C++ exceptions.
 */
#define JNI_CATCH \
    catch (gmsec::api::Exception& e) { \
		GMSEC_DEBUG << "Encountered gmsec::api::Exception - " << e.what(); \
		ThrowGmsecException(jenv, e.what()); \
    } \
    catch (std::bad_alloc& e) { \
		GMSEC_DEBUG << "Encountered std::bad_alloc - " << e.what(); \
        if (jvmOk(jenv, "catch(bad_alloc&)") && ::strlen(e.what()) < 200) { \
            char buffer[256]; \
            gmsec::api::util::StringUtil::stringFormat(buffer, sizeof(buffer), "GMSEC JNI encountered bad_alloc %s", e.what()); \
            gmsec::api::jni::SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, buffer); \
        } \
    } \
    catch (std::exception& e) { \
		GMSEC_DEBUG << "Encountered std::exception - " << e.what(); \
        if (jvmOk(jenv, "catch(exception&)") && ::strlen(e.what()) < 200) { \
            char buffer[256]; \
            gmsec::api::util::StringUtil::stringFormat(buffer, sizeof(buffer), "GMSEC JNI encountered %s", e.what()); \
            gmsec::api::jni::SWIG_JavaThrowException(jenv, SWIG_JavaUnknownError, buffer); \
        } \
    } \
    catch (...) { \
		GMSEC_DEBUG << "Encountered unknown exception"; \
        if (jvmOk(jenv, "catch(...)")) { \
            gmsec::api::jni::SWIG_JavaThrowException(jenv, SWIG_JavaUnknownError, "GMSEC JNI encountered unknown exception"); \
        } \
    }


namespace gmsec
{
namespace api
{
namespace jni
{

class JNIException : public std::exception
{
private:
	std::string message;

public:
	JNIException(const std::string& s);

	virtual ~JNIException() throw();

	const char* what() const throw();
};


/* Support for throwing Java exceptions */
typedef enum
{
	SWIG_JavaOutOfMemoryError = 1,
	SWIG_JavaIOException,
	SWIG_JavaRuntimeException,
	SWIG_JavaIndexOutOfBoundsException,
	SWIG_JavaArithmeticException,
	SWIG_JavaIllegalArgumentException,
	SWIG_JavaNullPointerException,
	SWIG_JavaUnknownError,
} SWIG_JavaExceptionCodes;


typedef struct
{
	SWIG_JavaExceptionCodes code;
	const char* java_exception;
} SWIG_JavaExceptions_t;


void ThrowGmsecException(JNIEnv* jenv, const char* errorMsg);

void SWIG_JavaThrowException(JNIEnv* jenv, SWIG_JavaExceptionCodes code, const char* msg);


/** @fn jvmOk
 * @brief Checks for pending exceptions from the JVM.
 */
bool jvmOk(JNIEnv* jenv, const char* context = 0);


/** @fn makeJavaString
 * @brief Helper function to convert a const char* to a Java String
 * while properly handling String encoding issues.
 */
jstring makeJavaString(JNIEnv *jenv, const char* cStr);


/**
   @brief JStringManager assists in conversions from jstring to char *.
    The destructor releases the char * in the event of an exception.
    This implementation assumes the native character set is ASCII.
    If this is not the case, google JNU_GetStringNativeChars.
*/
class JStringManager
{
private:
	JNIEnv* jenv;
	jstring jtext;
	const char* p;

public:
	JStringManager(JNIEnv* e, jstring s, bool checkJVM = false);

	~JStringManager();

	const char* c_str() const;
};


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

public:
	JByteArrayManager(JNIEnv *e, jbyteArray a);

	~JByteArrayManager();

	const jbyte* jbytes() const;
};


class AutoJEnv
{
public:
	AutoJEnv();

	~AutoJEnv();

	JNIEnv* getEnv();

	JavaVM* getVM();

private:
	JNIEnv* jenv;
	JavaVM* jvm;
	bool    doDetach;
};


jobject createJavaMessage(JNIEnv* jenv, const gmsec::api::Message& message);


jobject createJavaStatus(JNIEnv* jenv, const gmsec::api::Status& status);


int messageKindToJava(JNIEnv* jenv, gmsec::api::Message::MessageKind msgKind);


gmsec::api::Message::MessageKind messageKindToNative(JNIEnv* jenv, jint msgKind);


int fieldTypeToJava(JNIEnv* jenv, gmsec::api::Field::FieldType fieldType);


jobject lookupEvent(gmsec::api::Connection::ConnectionEvent event);


/*
   @brief Find the class for a given name throwing an exception if a problem occurs.
    Create a global reference to the jclass so it remains valid.
*/
jclass getClass(JNIEnv *jenv, const std::string& name);


/** @brief Look up a field for a class given its name and type.
*/
jfieldID getFieldID(JNIEnv *jenv, jclass klazz, const std::string& fname, const std::string& ftype);


/** @brief Look up a method for a class given its signature.
*/
jmethodID getMethod(JNIEnv *jenv, jclass klazz, const std::string& name, const std::string& signature);


}  // end namespace jni
}  // end namespace api
}  // end namespace gmsec

#endif
