/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**  @file gmsec4_defs.h
 *
 *  @brief This file contains all the type defs for GMSEC API 4.x
 */

#ifndef GMSEC4_API_DEFS_H
#define GMSEC4_API_DEFS_H


#include <gmsec_version.h>


#if (defined WIN32) && (_MSC_VER == 1500)
#define GMSEC_USING_VS2008 1
#endif

// Undefine macros defined by the perl libraries which cause problems with the 4.X Perl
// linking on Windows
//
#if (defined WIN32)
#if (defined connect)
#undef connect
#endif

#if (defined wait)
#undef wait
#endif
#endif


// The following section defines GMSEC types that are being used with API 4.x, but that also
// are used by API 3.x.  In order to avoid collisions with definitions, we will block this
// section from consideration when building API 3.x related modules.
//
#ifndef IGNORE_GMSEC_API_4_0_TYPES

#include <limits.h>


/* Ensure we are on a platform with 8 bit chars */
#if CHAR_BIT != 8
#	error Unsupported platform: number of bits in char is not 8.
#endif


/** @name GMSEC data types
 * @{
 */

/**
 *  @typedef GMSEC_BOOL
 *  @brief typedef for GMSEC_BOOL
 */
typedef enum
{
	GMSEC_FALSE = 0, /**< FALSE */
	GMSEC_TRUE = 1   /**< TRUE  */
} GMSEC_BOOL;


/** @typedef GMSEC_CHAR
 *  @brief typedef for GMSEC_CHAR
 */
typedef char GMSEC_CHAR;


/** @typedef GMSEC_I8
 *  @brief typedef for GMSEC_I8
 */
typedef signed char GMSEC_I8;
#define GMSEC_USING_SCHAR 1


/** @typedef GMSEC_U8
 *  @brief typedef for GMSEC_U8
 */
typedef unsigned char GMSEC_U8;
#define GMSEC_USING_UCHAR 1


#if GMSEC_S16IL32LL64
	typedef signed short   GMSEC_I16;
	typedef unsigned short GMSEC_U16;
	#define GMSEC_USING_SHORT 1

	typedef signed long    GMSEC_I32;
	typedef unsigned long  GMSEC_U32;
	#define GMSEC_USING_LONG 1

	typedef signed long long   GMSEC_I64;
	typedef unsigned long long GMSEC_U64;
	#define GMSEC_USING_LONG_LONG 1

#elif GMSEC_S16I32L64
	typedef signed short   GMSEC_I16;
	typedef unsigned short GMSEC_U16;
	#define GMSEC_USING_SHORT 1

	typedef signed int   GMSEC_I32;
	typedef unsigned int GMSEC_U32;
	#define GMSEC_USING_INT 1

	typedef signed long   GMSEC_I64;
	typedef unsigned long GMSEC_U64;
	#define GMSEC_USING_LONG 1

#elif GMSEC_VC6
	#define GMSEC_USE_ENUM_FOR_CONSTANTS

	typedef signed short   GMSEC_I16;
	typedef unsigned short GMSEC_U16;
	#define GMSEC_USING_SHORT 1

	typedef signed int   GMSEC_I32;
	typedef unsigned int GMSEC_U32;
	#define GMSEC_USING_INT 1

	typedef signed __int64   GMSEC_I64;
	typedef unsigned __int64 GMSEC_U64;
	#define GMSEC_USING_LONG_LONG 1

#else
	/* determine sizes from limits.h macros */

	/* Prefer short for GMSEC_I16 for backward-compatibility, then int */
	#if (SHRT_MAX >> 14) > 0
		typedef signed short   GMSEC_I16;
		typedef unsigned short GMSEC_U16;
		#define GMSEC_USING_SHORT 1

	#elif (INT_MAX >> 14) > 0
		typedef signed int   GMSEC_I16;
		typedef unsigned int GMSEC_U16;
		#define GMSEC_USING_INT 1

	#else
		#error Unsupported platform: missing type for GMSEC_I16
	#endif


	/* Prefer long for GMSEC_x32 if it is 32 bit for backward-compatibility,
	 * then int if still available */
	/* Shifts of more than 31 bits are broken up to avoid for HP-UX aCC. */

	#if (LONG_MAX >> 30) > 0
		#if ((LONG_MAX >> 16) >> 16) == 0
			typedef signed long   GMSEC_I32;
			typedef unsigned long GMSEC_U32;
			#define GMSEC_USING_LONG 1

		#elif !defined(GMSEC_USING_INT) && ((INT_MAX >> 30) > 0)
			typedef signed int   GMSEC_I32;
			typedef unsigned int GMSEC_U32;
			#define GMSEC_USING_INT 1

		#else
			#error Unsupported platform: missing type for GMSEC_I32
		#endif /* ((LONG_MAX >> 16) >> 16) == 0 */

	#else
		#error Unsupported platform: missing type for GMSEC_I32
	#endif /* (LONG_MAX >> 30) > 0 */


	/* Prefer long for GMSEC_x64 if available and 64 bit, then long long */
	#if !defined(GMSEC_USING_LONG) && ((LONG_MAX >> 30) >> 30) > 0
		typedef signed long   GMSEC_I64;
		typedef unsigned long GMSEC_U64;
		#define GMSEC_USING_LONG 1

	#elif ((LLONG_MAX >> 30) >> 30) > 0
		typedef signed long long   GMSEC_I64;
		typedef unsigned long long GMSEC_U64;
		#define GMSEC_USING_LONG_LONG 1

	#elif !defined(LLONG_MAX)
		#ifdef GMSEC_WARN_MISSING_LLONG
			/* in order to avoid a warning for each translation unit, only set and change
			 * the GMSEC_USING_LONG_LONG macro if GMSEC_WARN_MISSING_LLONG is defined
			 */
			#define GMSEC_USING_LONG_LONG warning: limits.h does not define LLONG_MAX
		#endif

		typedef signed long long   GMSEC_I64;
		typedef unsigned long long GMSEC_U64;
		#define GMSEC_USING_LONG_LONG 1

	#else
		#error Unsupported platform: missing type for GMSEC_I64
	#endif /* !defined(GMSEC_USING_LONG) && ... */

#endif /* !defined(GMSEC_S16I32L64) */



/** @typedef GMSEC_F32
 *  @brief typedef for GMSEC_F32
 */
typedef float     GMSEC_F32;


/** @typedef GMSEC_F64
 *  @brief typedef for GMSEC_F64
 */
typedef double    GMSEC_F64;


/** @typedef GMSEC_BIN
 *  @brief typedef for GMSEC_BIN
 */
typedef unsigned char* GMSEC_BIN;


#if 0
// Deprecated as of API 4.x
/**
 * Provide backward-compatible aliases
 */
typedef GMSEC_I16 GMSEC_SHORT;
typedef GMSEC_U16 GMSEC_USHORT;
typedef GMSEC_I32 GMSEC_LONG;
typedef GMSEC_U32 GMSEC_ULONG;
typedef GMSEC_I64 GMSEC_LONGLONG;
typedef GMSEC_U64 GMSEC_ULONGLONG;
typedef GMSEC_F32 GMSEC_FLOAT;
typedef GMSEC_F64 GMSEC_DOUBLE;
#endif

/*  @} */



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


/**
 * @enum LogLevel
 *  @brief Logging level identifiers
 */
enum LogLevel
{
	logNONE = 0,      /**< Disable logging */
	logERROR = 1,     /**< Error level logging */
	logSECURE = 2,    /**< Secure level logging */
	logWARNING = 3,   /**< Warning level logging */
	logINFO = 4,      /**< Info level logging */
	logVERBOSE = 5,   /**< Verbose level logging */
	logDEBUG = 6,     /**< Debug level logging */
	logNLEVEL = 7
};


/* Lots of GMSEC API code depends on NULL being defined. */
#ifndef NULL
	#define NULL 0
#endif

#endif  // IGNORE_GMSEC_API_4_0_TYPES


/**
 * @enum GMSEC_ConnectionEvent
 *
 * @sa connectionRegisterEventCallback
 */
typedef enum
{
	GMSEC_DISPATCHER_ERROR_EVENT = 0,        /**< Error occurred while attempting to dispatch message */
	GMSEC_REQUEST_TIMEOUT_EVENT = 1,         /**< A timeout occurred while attempting to receive a reply for a pending request. */
	GMSEC_CONN_SUCCESSFUL_EVENT = 2,         /**< Successfully connected to the middleware server. */
	GMSEC_CONN_BROKEN_EVENT = 3,             /**< %Connection to middleware server has been broken. */
	GMSEC_CONN_RECONNECT_EVENT = 4,          /**< An attempt is being made to reconnect to the middleware server. */
	GMSEC_CONN_EXCEPTION_EVENT = 5,          /**< An error, possibly fatal, has occurred with the connection to the middleware. */
	GMSEC_GMD_ERROR_EVENT = 6,               /**< SmartSockets Guaranteed Message Delivery (GMD) error. */
	GMSEC_WSMQ_ASYNC_STATUS_CHECK_EVENT = 7, /**< WebSphere MQ Asynchronous Put Status reporting event. */
	GMSEC_ALL_EVENTS = 8,                    /**< Monitor all events. */
	// @cond
	MSG_PUBLISH_FAILURE_EVENT = 9,           /* Deprecated; use GMSEC_MSG_PUBLISH_FAILURE_EVENT instead */
	// @endcond
	GMSEC_MSG_PUBLISH_FAILURE_EVENT = 9,     /**< Failure occurred while attempting to asynchronously publish a message. */
	GMSEC_INVALID_MESSAGE_EVENT = 10         /**< Message failed validation */
} GMSEC_ConnectionEvent;


/**
 * @typedef GMSEC_Config
 *
 * @desc A handle to a Config object.
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
 * @sa configFileCreate
 * @sa configFileDestroy
 */
typedef void* GMSEC_ConfigFile;


/**
 * @typedef GMSEC_ConfigFileIterator
 *
 * @sa configFileGetIterator
 */
typedef void* GMSEC_ConfigFileIterator;


typedef void* GMSEC_SubscriptionEntry_Handle;

/**
 * @typedef GMSEC_Connection
 *
 * @desc A handle to a Connection object.
 *
 * @sa connectionCreate
 * @sa connectionDestroy
 */
typedef void* GMSEC_Connection;


/**
 * @typedef GMSEC_ConnectionMgr
 *
 * @desc A handle to a MIST ConnectionManager object.
 *
 * @sa connectionManagerCreate
 * @sa connectionManagerDestroy
 */
typedef void* GMSEC_ConnectionMgr;


/**
 * @typedef GMSEC_Device
 *
 * @desc A handle to a MIST Device object.
 *
 * @sa deviceCreate
 * @sa deviceDestroy
 */
typedef void* GMSEC_Device;


/**
 * @typedef GMSEC_DeviceIterator
 *
 * @sa deviceMessageGetIterator
 */
typedef void* GMSEC_DeviceIterator;


/**
 * @typedef GMSEC_DeviceParam
 *
 * @desc A handle to a MIST DeviceParam object.
 *
 * @sa deviceParamCreate
 * @sa deviceParamDestroy
 */
typedef void* GMSEC_DeviceParam;


/**
 * @typedef GMSEC_Field
 *
 * @desc A handle to an opaque Field object.
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
 * @desc Typedef for a handle to a Heartbeat Generator object.
 *
 * @sa heartbeatGeneratorCreate
 * @sa heartbeatGeneratorCreateWithFields
 * @sa heartbeatGeneratorDestroy
 */
typedef void* GMSEC_HeartbeatGenerator;


/**
 * @typedef GMSEC_Message
 *
 * @desc Typedef for a handle to a Message object.
 *
 * @sa messageCreate
 * @sa messageCreateWithConfig
 * @sa messageCreateUsingData
 * @sa messageCreateCopy
 * @sa messageDestroy
 */
typedef void* GMSEC_Message;


/**
 * @typedef GMSEC_Mnemonic
 *
 * @desc A handle to a MIST Mnemonic object.
 *
 * @sa mnemonicCreate
 * @sa mnemonicDestroy
 */
typedef void* GMSEC_Mnemonic;


/**
 * @typedef GMSEC_MnemonicIterator
 *
 * @sa mnemonicMessageGetIterator
 */
typedef void* GMSEC_MnemonicIterator;


/**
 * @typedef GMSEC_MnemonicSample
 *
 * @desc A handle to a MIST MnemonicSample object.
 *
 * @sa mnemonicSampleCreate
 * @sa mnemonicSampleDestroy
 */
typedef void* GMSEC_MnemonicSample;


/**
 * @typedef GMSEC_MessageFieldIterator
 *
 * @desc Typedef for a handle to a MessageFieldIterator object.
 *
 * @sa messageGetFieldIterator
 */
typedef void* GMSEC_MessageFieldIterator;


/**
 * @typedef GMSEC_ProductFile
 *
 * @desc A handle to a MIST ProductFile object.
 *
 * @sa productFileCreate
 * @sa productFileDestroy
 */
typedef void* GMSEC_ProductFile;


/**
 * @typedef GMSEC_ProductFileIterator
 *
 * @sa productFileMessageGetIterator
 */
typedef void* GMSEC_ProductFileIterator;


/**
 * @typedef GMSEC_SchemaIDIterator
 *
 * @desc A handle to a MIST SchemaIDIterator object.
 *
 * @sa specificationGetSchemaIDIterator
 */
typedef void* GMSEC_SchemaIDIterator;


/**
 * @typedef GMSEC_ServiceParam
 *
 * @desc A handle to a MIST ServerParam object.
 *
 * @sa serviceParamCreate
 * @sa serviceParamDestroy
 */
typedef void* GMSEC_ServiceParam;


/**
 * @typedef GMSEC_Specification
 *
 * @desc A handle to a MIST Specification object.
 *
 * @sa specificationCreate
 * @sa specificationCreateCopy
 * @sa specificationDestroy
 */
typedef void* GMSEC_Specification;


/**
 * @typedef GMSEC_Status
 *
 * @desc A handle to a Status object.
 *
 * @sa statusCreate
 * @sa statusDestroy
 */
typedef void* GMSEC_Status;


/**
 * @typedef GMSEC_SubscriptionInfo
 *
 * @desc Typedef for a handle to a SubscriptionInfo object.
 *
 * @sa subscriptionInfoGetSubject
 * @sa subscriptionInfoGetCallback
 * @sa connectionSubscribe
 * @sa connectionSubscribeWithCallback
 * @sa connectionUnsubscribe
 */
typedef struct
{
	void*      info;
	GMSEC_BOOL fromMIST;
} GMSEC_SubscriptionInfo;


/**
 * @struct GMSEC_ConfigEntry
 *
 * @sa configFileIteratorNextConfig
 */
typedef struct GMSEC_ConfigEntry
{
	const char*  name;
	GMSEC_Config config;
} GMSEC_ConfigEntry;


/**
 * @struct GMSEC_MessageEntry
 *
 * @sa configFileIteratorNextMessage
 */
typedef struct GMSEC_MessageEntry
{
	const char*   name;
	GMSEC_Message message;
} GMSEC_MessageEntry;


/**
 * @struct GMSEC_SubscriptionEntry
 *
 * @sa configFileIteratorNextSubscription
 */
typedef struct GMSEC_SubscriptionEntry
{
	const char* name;
	const char* subject;
} GMSEC_SubscriptionEntry;


/**
 * @typedef GMSEC_Callback
 *
 * @brief Typedef for GMSEC_Callback
 *
 * @code
 * void GMSEC_Callback(GMSEC_Connection conn, const GMSEC_Message msg)
 * @endcode
 *
 * @param conn - handle to the Connection
 * @param msg  - handle to the received Message
 *
 * @sa connectionSubscribeWithCallback
 */
typedef void GMSEC_Callback(GMSEC_Connection conn, const GMSEC_Message msg);


/**
 * @typedef GMSEC_EventCallback
 *
 * @brief Typedef for GMSEC_EventCallback
 *
 * @code
 * void GMSEC_EventCallback(GMSEC_Connection conn, const GMSEC_Status status, GMSEC_ConnectionEvent event)
 * @endcode
 *
 * @param conn   - handle to the Connection
 * @param status - handle to the event Status
 * @param event  - the event that caused the callback to be summoned
 *
 * @sa connectionRegisterEventCallback
 */
typedef void GMSEC_EventCallback(GMSEC_Connection conn, const GMSEC_Status status, GMSEC_ConnectionEvent event);


/**
 * @typedef GMSEC_ReplyCallback
 *
 * @brief Typedef for GMSEC_ReplyCallback
 *
 * @code
 * void GMSEC_ReplyCallback(GMSEC_Connection conn, const GMSEC_Message request, const GMSEC_Message reply)
 * @endcode
 *
 * @param conn    - handle to the Connection
 * @param request - handle to the request Message
 * @param reply   - handle to the reply Message
 *
 * @sa connectionRequestWithCallback
 */
typedef void GMSEC_ReplyCallback(GMSEC_Connection conn, const GMSEC_Message request, const GMSEC_Message reply);


/**
 * @typedef GMSEC_ConnectionMgrCallback
 *
 * @brief Typedef for GMSEC_ConnectionMgrCallback
 *
 * @code
 * void GMSEC_ConnectionMgrCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg)
 * @endcode
 *
 * @param connMgr - handle to the ConnectionManager
 * @param msg     - handle to the received Message
 *
 * @sa connectionManagerSubscribeWithCallback
 */
typedef void GMSEC_ConnectionMgrCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message msg);


/**
 * @typedef GMSEC_ConnectionMgrEventCallback
 *
 * @brief Typedef for GMSEC_ConnectionMgrEventCallback
 *
 * @code
 * void GMSEC_ConnectionMgrEventCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Status status, GMSEC_ConnectionEvent event)
 * @endcode
 *
 * @param connMgr - handle to the ConnectionManager
 * @param status  - handle to the event Status
 * @param event   - the event that caused the callback to be summoned
 *
 * @sa connectionManagerRegisterEventCallback
 */
typedef void GMSEC_ConnectionMgrEventCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Status status, GMSEC_ConnectionEvent event);


/**
 * @typedef GMSEC_ConnectionMgrReplyCallback
 *
 * @brief Typedef for GMSEC_ConnectionMgrReplyCallback
 *
 * @code
 * void GMSEC_ConnectionMgrReplyCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, const GMSEC_Message reply)
 * @endcode
 *
 * @param connMgr - handle to the ConnectionManager
 * @param request - handle to the request Message
 * @param reply   - handle to the reply Message
 *
 * @sa connectionManagerRequestWithCallback
 */
typedef void GMSEC_ConnectionMgrReplyCallback(GMSEC_ConnectionMgr connMgr, const GMSEC_Message request, const GMSEC_Message reply);


/**
 * @typedef GMSEC_MessageValidator
 *
 * @brief Typedef for GMSEC_MessageValidator function.
 *
 * @code
 * void CustomMessageValidator(const GMSEC_Message msg, GMSEC_Status status)
 * {
 *    // Perform validation check on message
 *
 *    if (error == GMSEC_TRUE)
 *        statusSet(status, clazz, code, reason, customCode);
 * }
 * @endcode
 *
 * @param[in]  msg    - the message to validate
 * @param[out] status - the status of the operation, should one need to be returned
 *
 * @sa connectionManagerRegisterCustomMessageValidator
 */
typedef void GMSEC_MessageValidator(const GMSEC_Message msg, GMSEC_Status status);


/**
 * @typedef GMSEC_TimeSpec
 */
typedef struct
{
    long seconds;      // seconds since 1970 Jan 1 00:00:00 GMT
    long nanoseconds;  // fractional portion of a second; range is 0 .. 1e9-1
} GMSEC_TimeSpec;


/**
 * @desc Buffer size that is used by GMSEC to store time strings.
 */
#define GMSEC_TIME_BUFSIZE 28


/**
 * @typedef GMSEC_Time
 *
 * @desc Typedef used to represent GMSEC time in string format of YYYY-DDD-HH:MM:SS.mmm
 *
 * @sa timeUtilFormatTime_s
 */
typedef char GMSEC_Time[GMSEC_TIME_BUFSIZE];


/**
 * @typedef GMSEC_LogLevel
 */
typedef enum LogLevel GMSEC_LogLevel;


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
    const char*    file;
    int            line;
    GMSEC_LogLevel level;
    GMSEC_TimeSpec time;
    const char*    message;
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
