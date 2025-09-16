/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**  @file gmsec5_defs.h
 *
 *  @brief This file contains all the type defs for GMSEC API 5.x
 */

#ifndef GMSEC5_DEFS_H
#define GMSEC5_DEFS_H


#include <gmsec_version.h>

#include <limits.h>


// Undefine macros defined by the Perl libraries which cause problems with the
// linking the 5.x Perl binding on Windows
//
#if (defined WIN32)
#if (defined connect)
#undef connect
#endif

#if (defined wait)
#undef wait
#endif
#endif



/* Ensure we are on a platform with 8 bit chars */
#if CHAR_BIT != 8
#	error Unsupported platform: number of bits in char is not 8.
#endif


/* Lots of GMSEC API code depends on NULL being defined. */
#ifndef NULL
	#define NULL 0
#endif


/**
 * Macros for maintaining binary compatibility with the C# binding of API 5.x
 */
#ifdef SWIG_BINDING
#define GMSEC_USING_SCHAR      1
#define GMSEC_USING_UCHAR      1
#define GMSEC_USING_SHORT      1
#define GMSEC_USING_LONG       1
#define GMSEC_USING_LONG_LONG  1
#endif


/** @name GMSEC Data Types
 * @{
 */

/**
 * @typedef GMSEC_BOOL
 * @brief typedef for GMSEC_BOOL
 */
#ifdef __cplusplus
enum class GMSEC_BOOL
#else
typedef enum
#endif
{
	GMSEC_FALSE = 0, /**< FALSE */
	GMSEC_TRUE  = 1  /**< TRUE  */
#ifdef __cplusplus
};
#else
} GMSEC_BOOL;
#endif


/**
 * @typedef GMSEC_CHAR
 * @brief typedef for GMSEC_CHAR
 */
typedef char GMSEC_CHAR;


/**
 * @typedef GMSEC_I8
 * @brief typedef for GMSEC_I8
 */
typedef signed char GMSEC_I8;


/**
 * @typedef GMSEC_U8
 * @brief typedef for GMSEC_U8
 */
typedef unsigned char GMSEC_U8;


/**
 * @typedef GMSEC_I16
 * @brief typedef for GMSEC_I16
 */
typedef signed short GMSEC_I16;


/**
 * @typedef GMSEC_U16
 * @brief typedef for GMSEC_U16
 */
typedef unsigned short GMSEC_U16;


/**
 * @typedef GMSEC_I32
 * @brief typedef for GMSEC_I32
 */
#ifdef WIN32
typedef signed long GMSEC_I32;
#else
typedef signed int GMSEC_I32;
#endif


/**
 * @typedef GMSEC_U32
 * @brief typedef for GMSEC_U32
 */
#ifdef WIN32
typedef unsigned long GMSEC_U32;
#else
typedef unsigned int GMSEC_U32;
#endif


/**
 * @typedef GMSEC_I64
 * @brief typedef for GMSEC_I64
 */
#if defined(WIN32) || defined(SWIG_BINDING)
typedef signed long long GMSEC_I64;
#else
typedef signed long GMSEC_I64;
#endif


/**
 * @typedef GMSEC_U64
 * @brief typedef for GMSEC_U64
 */
#if defined(WIN32) || defined(SWIG_BINDING)
typedef unsigned long long GMSEC_U64;
#else
typedef unsigned long GMSEC_U64;
#endif


/**
 * @typedef GMSEC_F32
 * @brief typedef for GMSEC_F32
 */
typedef float GMSEC_F32;


/**
 * @typedef GMSEC_F64
 * @brief typedef for GMSEC_F64
 */
typedef double GMSEC_F64;
/**@}*/


/** @name GMSEC Timeout Constants
 *
 * @{
 */
/** @def GMSEC_NO_WAIT
 * @brief timeout "no wait" constant - causes methods with timeouts to not block
 */
#define GMSEC_NO_WAIT	0


/** @def GMSEC_WAIT_FOREVER
 * @brief timeout "wait forever" constant - causes methods with timeouts to block
 */
#define GMSEC_WAIT_FOREVER	-1


/** @def GMSEC_REQUEST_REPUBLISH_NEVER
 * @brief Value to indicate that a request message should only be issued once
 * should a response message not be received.
 */
#define GMSEC_REQUEST_REPUBLISH_NEVER	-1
/**@}*/


/** @name GMSEC Message Specification Constants
 *
 * @{
 */
/**
 * @brief 2014 version of the GMSEC Interface Specification Document (ISD)
 */
#define GMSEC_MSG_SPEC_2014_00 201400

/**
 * @brief 2016 version of the GMSEC Interface Specification Document (ISD)
 */
#define GMSEC_MSG_SPEC_2016_00 201600

/**
 * @brief 2018 version of the Command and Control Message Specification (C2MS)
 */
#define GMSEC_MSG_SPEC_2018_00 201800

/**
 * @brief 2019 version of the Command and Control Message Specification(C2MS)
 */
#define GMSEC_MSG_SPEC_2019_00 201900

/**
 * @brief Current/latest version of the message specification
 */
#define GMSEC_MSG_SPEC_CURRENT GMSEC_MSG_SPEC_2019_00
/**@}*/


/**
 * @enum LogLevel
 * @brief Logging level identifiers
 */
#ifdef __cplusplus
enum class LogLevel
#else
typedef enum
#endif
{
	logNONE = 0,      /**< Disable logging */
	logERROR = 1,     /**< Error level logging */
	logSECURE = 2,    /**< Secure level logging */
	logWARNING = 3,   /**< Warning level logging */
	logINFO = 4,      /**< Info level logging */
	logVERBOSE = 5,   /**< Verbose level logging */
	logDEBUG = 6,     /**< Debug level logging */
	logNLEVEL = 7
#ifdef __cplusplus
};
#else
} LogLevel;
#endif


/**
 * @typedef GMSEC_LogLevel
 */
typedef LogLevel GMSEC_LogLevel;


/**
 * @enum DataType
 *
 * @brief The type of data to parse/ingest.
 */
#ifdef __cplusplus
enum class DataType
#else
typedef enum
#endif
{
	XML_DATA,         /**< XML formatted configuration data string */
	JSON_DATA,        /**< JSON formatted configuration data string */
	KEY_VALUE_DATA    /**< Key=Value formatted configuration data string */
#ifdef __cplusplus
};
#else
} DataType;
#endif


/**
 * @typedef GMSEC_Config
 *
 * @brief A C binding handle to a Config object.
 *
 * @sa configCreate
 * @sa configCreateWithArgs
 * @sa configCreateCopy
 * @sa configDestroy
 */
typedef void* GMSEC_Config;


/**
 * @typedef GMSEC_ConfigFile
 *
 * @brief A C binding handle to a ConfigFile object.
 *
 * @sa configFileCreate
 * @sa configFileDestroy
 */
typedef void* GMSEC_ConfigFile;


/**
 * @typedef GMSEC_ConfigFileIterator
 *
 * @brief A C binding handle to a ConfigFileIterator object.
 *
 * @sa configFileGetIterator
 */
typedef void* GMSEC_ConfigFileIterator;


/**
 * @typedef GMSEC_Connection
 *
 * @brief A C binding handle to a Connection object.
 *
 * @sa connectionCreate
 * @sa connectionDestroy
 */
typedef void* GMSEC_Connection;


/**
 * @typedef GMSEC_Field
 *
 * @brief A C binding handle to an opaque Field object.
 *
 * @sa binaryFieldCreate
 * @sa booleanFieldCreate
 * @sa charFieldCreate
 * @sa f32FieldCreate
 * @sa f64FieldCreate
 * @sa i16FieldCreate
 * @sa i32FieldCreate
 * @sa i64FieldCreate
 * @sa i8FieldCreate
 * @sa stringFieldCreate
 * @sa u16FieldCreate
 * @sa u32FieldCreate
 * @sa u64FieldCreate
 * @sa u8FieldCreate
 * @sa fieldDestroy
 */
typedef void* GMSEC_Field;


/**
 * @typedef GMSEC_HeartbeatGenerator
 *
 * @brief A C binding handle to a HeartbeatGenerator object.
 *
 * @sa heartbeatGeneratorCreate
 * @sa heartbeatGeneratorCreateWithFields
 * @sa heartbeatGeneratorDestroy
 */
typedef void* GMSEC_HeartbeatGenerator;


/**
 * @typedef GMSEC_Message
 *
 * @brief A C binding handle to a Message object.
 *
 * @sa messageCreate
 * @sa messageCreateWithConfig
 * @sa messageCreateUsingData
 * @sa messageCreateCopy
 * @sa messageDestroy
 */
typedef void* GMSEC_Message;


/**
 * @typedef GMSEC_MessageFactory
 *
 * @brief A C binding handle to a MessageFactory object.
 *
 * @sa connectionGetMessageFactory
 */
typedef void* GMSEC_MessageFactory;


/**
 * @typedef GMSEC_MessageFieldIterator
 *
 * @brief A C binding handle to a MessageFieldIterator object.
 *
 * @sa messageGetFieldIterator
 */
typedef void* GMSEC_MessageFieldIterator;


/**
 * @typedef GMSEC_ResourceGenerator
 *
 * @brief A C binding handle to a ResourceGenerator object.
 *
 * @sa resourceGeneratorCreate
 * @sa resourceGeneratorCreateWithFields
 * @sa resourceGeneratorDestroy
 */
typedef void* GMSEC_ResourceGenerator;


/**
 * @typedef GMSEC_SchemaIDIterator
 *
 * @brief A C binding handle to a SchemaIDIterator object.
 *
 * @sa specificationGetSchemaIDIterator
 */
typedef void* GMSEC_SchemaIDIterator;


/**
 * @typedef GMSEC_Specification
 *
 * @brief A C binding handle to a Specification object.
 *
 * @sa specificationCreate
 * @sa specificationCreateCopy
 * @sa specificationDestroy
 */
typedef void* GMSEC_Specification;


/**
 * @typedef GMSEC_Status
 *
 * @brief A C binding handle to a Status object.
 *
 * @sa statusCreate
 * @sa statusDestroy
 */
typedef void* GMSEC_Status;


/**
 * @struct GMSEC_SubscriptionEntry
 *
 * @brief A C binding handle to a SubscriptionEntry object.
 *
 * @sa configFileIteratorNextSubscription
 */
typedef void* GMSEC_SubscriptionEntry;


/**
 * @typedef GMSEC_SubscriptionInfo
 *
 * @brief A C binding handle to a SubscriptionInfo object.
 *
 * @sa subscriptionInfoGetSubject
 * @sa subscriptionInfoGetCallback
 * @sa connectionSubscribe
 * @sa connectionSubscribeWithCallback
 * @sa connectionUnsubscribe
 */
typedef void* GMSEC_SubscriptionInfo;


/**
 * @struct GMSEC_ConfigEntry
 * @brief Struct used to represent a Config entry in a Configuration file.
 * @sa configFileIteratorNextConfig
 */
typedef struct
{
	const char*  name;        /**< The name of the config entry */
	GMSEC_Config config;      /**< The reference to the config */
} GMSEC_ConfigEntry;


/**
 * @struct GMSEC_MessageEntry
 * @brief Struct used to represent a Message entry in a Configuration file.
 * @sa configFileIteratorNextMessage
 */
typedef struct
{
	const char*   name;       /**< The name of the message entry */
	GMSEC_Message message;    /**< The reference to the message */
} GMSEC_MessageEntry;


/**
 * @struct GMSEC_TimeSpec
 * @brief Struct used to represent GMSEC time in whole seconds and fractional time.
 */
typedef struct
{
    long seconds;      /**< Seconds since 1970 Jan 1 00:00:00 GMT */
    long nanoseconds;  /**< Fractional portion of a second; range is 0 .. 1e9-1 */
} GMSEC_TimeSpec;


/**
 * @brief Buffer size that is used by GMSEC to store time strings.
 */
#define GMSEC_TIME_BUFSIZE 28


/**
 * @typedef GMSEC_Time
 *
 * @brief Typedef used to represent GMSEC time in string format of YYYY-DDD-HH:MM:SS.mmm
 *
 * @sa timeUtilFormatTime_s
 */
typedef char GMSEC_Time[GMSEC_TIME_BUFSIZE];


/**
 * @struct GMSEC_LogEntry
 *
 * @brief The structure that is passed to a LogHandler (or a C GMSEC_LogHandler) object when a message is logged.
 * The structure is defined as:
 *
 * @sa Log
 * @sa LogHandler
 */
typedef struct
{
    const char*    file;      /**< Name of file where log originated */
    int            line;      /**< Line number in the file where log orginated */
    GMSEC_LogLevel level;     /**< Log level being reported */
    GMSEC_TimeSpec time;      /**< Time of the log being reported */
    const char*    message;   /**< Log message being reported */
} GMSEC_LogEntry;


/**
 * @typedef GMSEC_LogHandler
 *
 * @brief Typedef for custom log handler.
 *
 * @code
 * void GMSEC_LogHandler(const GMSEC_LogEntry* entry)
 * @endcode
 *
 * @param logEntry - pointer to a GMSEC_LogEntry structure
 *
 * @sa logRegisterHandler
 */
typedef void GMSEC_LogHandler(const GMSEC_LogEntry* logEntry);

#endif
