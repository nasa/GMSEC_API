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

#include <gmsec/internal/strutil.h>

#include <jni.h>

#include <stdexcept>
#include <string>
#include <cstring>


// Forward declarations
namespace gmsec
{
	class Config;
	class ConfigFile;
	class Connection;
	class Field;
	class Message;
	class Status;

	namespace mist
	{
		class ConnectionManager;
	};

	namespace util
	{
		class LogHandler;
	};
};


#define JNI_POINTER_TO_JLONG(x)            reinterpret_cast<jlong>(x)
#define JNI_JLONG_TO_STATUS(x)             reinterpret_cast<gmsec::Status *>(x)
#define JNI_JLONG_TO_CONFIGFILE(x)         reinterpret_cast<gmsec::ConfigFile *>(x)
#define JNI_JLONG_TO_CONFIG(x)             reinterpret_cast<gmsec::Config *>(x)
#define JNI_JLONG_TO_CONNECTION(x)         reinterpret_cast<gmsec::Connection *>(x)
#define JNI_JLONG_TO_MESSAGE(x)            reinterpret_cast<gmsec::Message *>(x)
#define JNI_JLONG_TO_FIELD(x)              reinterpret_cast<gmsec::Field *>(x)
#define JNI_JLONG_TO_LOGHANDLER(x)         reinterpret_cast<gmsec::util::LogHandler *>(x)
#define JNI_JLONG_TO_CONNECTION_MANAGER(x) reinterpret_cast<gmsec::mist::ConnectionManager *>(x)


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
    catch (std::bad_alloc& e) { \
        FPRINTF(stderr, "%s:%d: encountered bad_alloc %s\n", __FILE__, __LINE__, e.what()); \
        if (jvmOk(jenv, "catch(bad_alloc&)") && ::strlen(e.what()) < 200) { \
            char buffer[256]; \
            gmsec::util::stringFormat(buffer, sizeof(buffer), "GMSEC JNI encountered bad_alloc %s", e.what()); \
            SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, buffer); \
        } \
    } \
    catch (std::exception& e) { \
        FPRINTF(stderr, "%s:%d: encountered exception %s\n", __FILE__, __LINE__, e.what()); \
        if (jvmOk(jenv, "catch(exception&)") && ::strlen(e.what()) < 200) { \
            char buffer[256]; \
            gmsec::util::stringFormat(buffer, sizeof(buffer), "GMSEC JNI encountered %s", e.what()); \
            SWIG_JavaThrowException(jenv, SWIG_JavaUnknownError, buffer); \
        } \
    } \
    catch (...) { \
        FPRINTF(stderr, "%s:%d: encountered unknown exception\n", __FILE__, __LINE__); \
        if (jvmOk(jenv, "catch(...)")) { \
            SWIG_JavaThrowException(jenv, SWIG_JavaUnknownError, "GMSEC JNI encountered unknown exception"); \
        } \
    }



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
	SWIG_JavaUnknownError
} SWIG_JavaExceptionCodes;


typedef struct
{
	SWIG_JavaExceptionCodes code;
	const char* java_exception;
} SWIG_JavaExceptions_t;


void SWIG_JavaThrowException(JNIEnv* jenv, SWIG_JavaExceptionCodes code, const char* msg);


/** @fn jvmOk
 * @brief Checks for pending exceptions from the JVM.
 */
bool jvmOk(JNIEnv* jenv, const char* context = 0);


/** @fn makeJavaString
 * @brief Helper function to convert a const char* to a Java GMSEC_String
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


jobject createJavaMessage(JNIEnv *jenv, gmsec::Message *message);

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

#endif
