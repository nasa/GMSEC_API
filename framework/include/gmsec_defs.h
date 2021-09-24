/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**  @file gmsec_defs.h
 *
 *  @author Goddard Space Flight Center
 *  @date April 24, 2003
 *
 *	@brief This file contains all the type defs for the GMSEC API
*/
#ifndef gmsec_defs_h
#define gmsec_defs_h


#define IGNORE_GMSEC_API_4_0_TYPES

#include <gmsec_version.h>
#include <gmsec_errors.h>

#include <limits.h>

/* Connection identifiers have been deprecated
 *------------------------------------------------
 */
#define GMSEC_CONNECTION_TYPE	unsigned short
#define GMSEC_CONNECTION_ICSSWB			1
#define GMSEC_CONNECTION_RENDEZVOUS		2
#define GMSEC_CONNECTION_SMARTSOCKETS	3


/** @name Event error codes.
 *  @brief These event error codes can be used with Connection::RegisterErrorCallback()
 */
#define GMSEC_CONNECTION_DISPATCHER_ERROR              "CONNECTION_DISPATCHER_ERROR"
#define GMSEC_CONNECTION_REQUEST_TIMEOUT               "CONNECTION_REQUEST_TIMEOUT"

#define GMSEC_CONNECTION_DISPATCHER_ERROR_EVENT GMSEC_CONNECTION_DISPATCHER_ERROR
#define GMSEC_CONNECTION_REQUEST_TIMEOUT_EVENT  GMSEC_CONNECTION_REQUEST_TIMEOUT
#define GMSEC_CONNECTION_SUCCESSFUL_EVENT              "CONNECTION_SUCCESSFUL"
#define GMSEC_CONNECTION_BROKEN_EVENT                  "CONNECTION_BROKEN"
#define GMSEC_CONNECTION_RECONNECT_EVENT               "CONNECTION_RECONNECT"
#define GMSEC_CONNECTION_EXCEPTION_EVENT               "CONNECTION_EXCEPTION"
#define GMSEC_CONNECTION_WSMQ_ASYNC_STATUS_CHECK_EVENT "WSMQ_ASYNC_STATUS_CHECK_EVENT"
#define GMSEC_ALL_ERRORS_EVENT                         "*"


/** @name Field type identifiers
 * @{
 */

/** @typedef GMSEC_TYPE
 *  @brief field type identifier value type
 *  @sa GMSEC_TYPE_UNSET @n
 *		GMSEC_TYPE_CHAR @n
 *		GMSEC_TYPE_BOOL @n
 *		GMSEC_TYPE_I16 @n
 *		GMSEC_TYPE_SHORT @n
 *		GMSEC_TYPE_U16 @n
 *		GMSEC_TYPE_USHORT @n
 *		GMSEC_TYPE_I32 @n
 *		GMSEC_TYPE_LONG @n
 *		GMSEC_TYPE_U32 @n
 *		GMSEC_TYPE_ULONG @n
 *		GMSEC_TYPE_F32 @n
 *		GMSEC_TYPE_FLOAT @n
 *		GMSEC_TYPE_F64 @n
 *		GMSEC_TYPE_DOUBLE @n
 *		GMSEC_TYPE_STR @n
 *		GMSEC_TYPE_STRING @n
 *		GMSEC_TYPE_BLOB @n
 *		GMSEC_TYPE_BIN @n
 *		GMSEC_TYPE_I8 @n
 *		GMSEC_TYPE_U8 @n
 *		GMSEC_TYPE_I64 @n
 *		GMSEC_TYPE_U64 @n
 *		GMSEC_TYPE_COMPOUND @n
 */
typedef unsigned short GMSEC_TYPE;

/** @def GMSEC_TYPE_UNSET
 *  @brief field type identifier for UNSET field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_UNSET		0

/** @def GMSEC_TYPE_CHAR
 *  @brief field type identifier for GMSEC_CHAR field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_CHAR			1

/** @def GMSEC_TYPE_BOOL
 *  @brief field type identifier for GMSEC_BOOL field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_BOOL			2

/** @def GMSEC_TYPE_I16
 *  @brief field type identifier for GMSEC_I16 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_I16			3

/** @def GMSEC_TYPE_SHORT
 *  @brief field type identifier for GMSEC_SHORT field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_SHORT		GMSEC_TYPE_I16

/** @def GMSEC_TYPE_U16
 *  @brief field type identifier for GMSEC_U16 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_U16			4

/** @def GMSEC_TYPE_USHORT
 *  @brief field type identifier for GMSEC_USHORT field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_USHORT		GMSEC_TYPE_U16

/** @def GMSEC_TYPE_I32
 *  @brief field type identifier for GMSEC_I32 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_I32			5

/** @def GMSEC_TYPE_LONG
 *  @brief field type identifier for GMSEC_LONG field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_LONG			GMSEC_TYPE_I32

/** @def GMSEC_TYPE_U32
 *  @brief field type identifier for GMSEC_U32 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_U32			6

/** @def GMSEC_TYPE_ULONG
 *  @brief field type identifier for GMSEC_ULONG field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_ULONG		GMSEC_TYPE_U32

/** @def GMSEC_TYPE_F32
 *  @brief field type identifier for GMSEC_F32 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_F32			7

/** @def GMSEC_TYPE_FLOAT
 *  @brief field type identifier for GMSEC_FLOAT field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_FLOAT		GMSEC_TYPE_F32

/** @def GMSEC_TYPE_F64
 *  @brief field type identifier for GMSEC_F64 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_F64			8

/** @def GMSEC_TYPE_DOUBLE
 *  @brief field type identifier for GMSEC_DOUBLE field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_DOUBLE		GMSEC_TYPE_F64

/** @def GMSEC_TYPE_STRING
 *  @brief field type identifier for GMSEC_STRING field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_STR			9
#define GMSEC_TYPE_STRING		GMSEC_TYPE_STR

/** @def GMSEC_TYPE_BLOB
 *  @brief field type identifier for GMSEC_BLOB field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_BLOB			10

/** @def GMSEC_TYPE_BIN
 *  @brief field type identifier for GMSEC_BIN field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_BIN			GMSEC_TYPE_BLOB

/** @def GMSEC_TYPE_I8
 *  @brief field type identifier for GMSEC_I8 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_I8			20

/** @def GMSEC_TYPE_U8
 *  @brief field type identifier for GMSEC_U8 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_U8			21

/** @def GMSEC_TYPE_I64
 *  @brief field type identifier for GMSEC_I64 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_I64			22

/** @def GMSEC_TYPE_U64
 *  @brief field type identifier for GMSEC_U64 field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_U64			23

/** @def GMSEC_TYPE_COMPOUND
 *  @brief field type identifier for GMSEC_COMPOUND field
 *  @sa GMSEC_TYPE
 */
#define GMSEC_TYPE_COMPOUND		24


/*  @} */



/* Ensure we are on a platform with 8 bit chars */
#if CHAR_BIT != 8
#	error Unsupported platform: number of bits in char is not 8.
#endif

/** @name Field types
 * @{
 */

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

/**
 *  @typedef GMSEC_BOOL
 *  @brief typedef for GMSEC_BOOL
 */
typedef enum
{
	GMSEC_FALSE, /**< FALSE */
	GMSEC_TRUE   /**< TRUE  */
} GMSEC_BOOL;


#if GMSEC_S16IL32LL64

typedef signed short GMSEC_I16;
typedef unsigned short GMSEC_U16;
#	define GMSEC_USING_SHORT 1

typedef signed long GMSEC_I32;
typedef unsigned long GMSEC_U32;
#	define GMSEC_USING_LONG 1

typedef signed long long GMSEC_I64;
typedef unsigned long long GMSEC_U64;
#	define GMSEC_USING_LONG_LONG 1

#elif GMSEC_S16I32L64

typedef signed short GMSEC_I16;
typedef unsigned short GMSEC_U16;
#	define GMSEC_USING_SHORT 1

typedef signed int GMSEC_I32;
typedef unsigned int GMSEC_U32;
#	define GMSEC_USING_INT 1

typedef signed long GMSEC_I64;
typedef unsigned long GMSEC_U64;
#	define GMSEC_USING_LONG 1

#elif GMSEC_VC6

#	define GMSEC_USE_ENUM_FOR_CONSTANTS

	typedef signed short GMSEC_I16;
	typedef unsigned short GMSEC_U16;
#	define GMSEC_USING_SHORT 1

	typedef signed int GMSEC_I32;
	typedef unsigned int GMSEC_U32;
#	define GMSEC_USING_INT 1

	typedef signed __int64 GMSEC_I64;
	typedef unsigned __int64 GMSEC_U64;
#	define GMSEC_USING_LONG_LONG 1

#else

/* determine sizes from limits.h macros */

/* Prefer short for GMSEC_I16 for backward-compatibility, then int */
#if (SHRT_MAX >> 14) > 0
typedef signed short GMSEC_I16;
typedef unsigned short GMSEC_U16;
#	define GMSEC_USING_SHORT 1
#elif (INT_MAX >> 14) > 0
typedef signed int GMSEC_I16;
typedef unsigned int GMSEC_U16;
#	define GMSEC_USING_INT 1
#else
#	error Unsupported platform: missing type for GMSEC_I16
#endif


/* Prefer long for GMSEC_x32 if it is 32 bit for backward-compatibility,
	then int if still available */
/* Shifts of more than 31 bits are broken up to avoid for HP-UX aCC. */

#if (LONG_MAX >> 30) > 0
#	if ((LONG_MAX >> 16) >> 16) == 0
typedef signed long GMSEC_I32;
typedef unsigned long GMSEC_U32;
#		define GMSEC_USING_LONG 1
#	elif !defined(GMSEC_USING_INT) && ((INT_MAX >> 30) > 0)
typedef signed int GMSEC_I32;
typedef unsigned int GMSEC_U32;
#		define GMSEC_USING_INT 1
#	else
#		error Unsupported platform: missing type for GMSEC_I32
#	endif /* (LONG_MAX >> 30) > 0 */
#else
#	error Unsupported platform: missing type for GMSEC_I32
#endif /* (LONG_MAX >> 30) > 0 */


/* Prefer long for GMSEC_x64 if available and 64 bit, then long long */
#if !defined(GMSEC_USING_LONG) && ((LONG_MAX >> 30) >> 30) > 0
typedef signed long GMSEC_I64;
typedef unsigned long GMSEC_U64;
#	define GMSEC_USING_LONG 1
#elif ((LLONG_MAX >> 30) >> 30) > 0
typedef signed long long GMSEC_I64;
typedef unsigned long long GMSEC_U64;
#	define GMSEC_USING_LONG_LONG 1
#elif !defined(LLONG_MAX)
#ifdef GMSEC_WARN_MISSING_LLONG
/* in order to avoid a warning for each translation unit, only set and change
the GMSEC_USING_LONG_LONG macro if GMSEC_WARN_MISSING_LLONG is defined */
#	define GMSEC_USING_LONG_LONG warning: limits.h does not define LLONG_MAX
#endif
typedef signed long long GMSEC_I64;
typedef unsigned long long GMSEC_U64;
#	define GMSEC_USING_LONG_LONG 1
#else
#	error Unsupported platform: missing type for GMSEC_I64
#endif /* !defined(GMSEC_USING_LONG) && ... */

#endif /* !defined(GMSEC_S16I32L64) */



/** @typedef GMSEC_F32
 *  @brief typedef for GMSEC_F32
 */
typedef float  GMSEC_F32;

/** @typedef GMSEC_F64
 *  @brief typedef for GMSEC_F64
 */
typedef double GMSEC_F64;
/* we do not know how big a long double will be */


/* provide backward-compatible aliases */

/** @typedef GMSEC_SHORT
 *  @brief typedef for GMSEC_SHORT
 */
typedef GMSEC_I16  GMSEC_SHORT;
/** @typedef GMSEC_LONG
 *  @brief typedef for GMSEC_LONG
 */
typedef GMSEC_I32  GMSEC_LONG;

/** @typedef GMSEC_USHORT
 *  @brief typedef for GMSEC_USHORT
 */
typedef GMSEC_U16  GMSEC_USHORT;
/** @typedef GMSEC_ULONG
 *  @brief typedef for GMSEC_ULONG
 */
typedef GMSEC_U32  GMSEC_ULONG;

/** @typedef GMSEC_FLOAT
 *  @brief typedef for GMSEC_FLOAT
 */
typedef GMSEC_F32  GMSEC_FLOAT;
/** @typedef GMSEC_DOUBLE
 *  @brief typedef for GMSEC_DOUBLE
 */
typedef GMSEC_F64  GMSEC_DOUBLE;

/** @typedef GMSEC_STRING
 *  @brief typedef for GMSEC_STRING
 */
typedef char*      GMSEC_STRING;

/** @typedef GMSEC_BIN
 *  @brief typedef for GMSEC_BIN
 */
typedef unsigned char*      GMSEC_BIN;

/** @typedef GMSEC_STR
 *  @brief typedef for GMSEC_STR
 */
typedef const char* GMSEC_STR;

/** @typedef GMSEC_BLOB
 *  @brief typedef for GMSEC_BLOB
 */
typedef const void* GMSEC_BLOB;

/** @typedef GMSEC_LONGLONG
 *  @brief typedef for GMSEC_LONGLONG
 */
typedef GMSEC_I64  GMSEC_LONGLONG;
/** @typedef GMSEC_ULONGLONG
 *  @brief typedef for GMSEC_ULONGLONG
 */
typedef GMSEC_U64  GMSEC_ULONGLONG;

/*  @} */



#define MESSAGE_TRACKINGFIELDS_UNSET	-1
#define MESSAGE_TRACKINGFIELDS_ON		GMSEC_TRUE
#define MESSAGE_TRACKINGFIELDS_OFF		GMSEC_FALSE

#define REPLY_SUBJECT_FIELD     "GMSEC_REPLY_SUBJECT"

/** @name Message kind identifiers
 * @{
 */

/** @typedef GMSEC_MSG_KIND
 *  @brief message type identifier value type
 *  @sa GMSEC_MSG_UNSET @n
 *		GMSEC_MSG_PUBLISH @n
 *		GMSEC_MSG_REQUEST @n
 *		GMSEC_MSG_REPLY
 */
typedef GMSEC_U16 GMSEC_MSG_KIND;

/** @def GMSEC_MSG_UNSET
 *  @brief unset message kind
 */
#define GMSEC_MSG_UNSET		0

/** @def GMSEC_MSG_PUBLISH
 *  @brief publish message kind
 */
#define GMSEC_MSG_PUBLISH	1

/** @def GMSEC_MSG_REQUEST
 *  @brief request message kind
 */
#define GMSEC_MSG_REQUEST	2

/** @def GMSEC_MSG_REPLY
 *  @brief reply message kind
 */
#define GMSEC_MSG_REPLY		3
/*  @} */


/** @name Timeout Constants
 *
 * @{
 */

/** @def GMSEC_NO_WAIT
 * @brief timeout "no wait" constant - causes methods with timeouts to not block
 * @sa RequestWCallback() @n
 *	   Request() @n
 *     GetNextMsg()
 */
#define GMSEC_NO_WAIT	0

/** @def GMSEC_WAIT_FOREVER
 * @brief timeout "wait forever" constant - causes methods with timeouts to block
 * @sa RequestWCallback() @n
 *	   Request() @n
 *     GetNextMsg()
 */
#define GMSEC_WAIT_FOREVER	-1

/** @def GMSEC_REQUEST_REPUBLISH_NEVER
 * @brief Value to indicate that a request message should only be issued once
 * should a response message not be received.
 */
#define GMSEC_REQUEST_REPUBLISH_NEVER	-1
/**@}*/

/** @name Message kind identifiers
 * @{
 */

/** @enum LogLevel
 *  @brief Logging level identifiers
 */
enum LogLevel
{
	logNONE,      /**< Disable logging */
	logERROR,     /**< Error level logging */
	logSECURE,    /**< Secure level logging */
	logWARNING,   /**< Warning level logging */
	logINFO,      /**< Info level logging */
	logVERBOSE,   /**< Verbose level logging */
	logDEBUG,     /**< Debug level logging */
	logNLEVEL     
};


/** @name C API Handle types
 * @{
 */

/** @typedef GMSEC_STATUS_HANDLE
 *  @brief handle to status
 *  @sa CreateStatus() @n
 *		DestroyStatus()
 */
typedef void * GMSEC_STATUS_HANDLE;
/** @typedef GMSEC_CONNECTION_HANDLE
 *  @brief handle to connection
 *  @sa CreateConnection() @n
 *		DestroyConnection()
 */
typedef void * GMSEC_CONNECTION_HANDLE;
/** @typedef GMSEC_MESSAGE_HANDLE
 *  @brief handle to message
 *  @sa CreateMessage() @n
 *		DestroyMessage()
 */
typedef void * GMSEC_MESSAGE_HANDLE;
/** @typedef GMSEC_FIELD_HANDLE
 *  @brief handle to field
 *  @sa CreateField() @n
 *		DestroyField()
 */
typedef void * GMSEC_FIELD_HANDLE;
/** @typedef GMSEC_CONFIG_HANDLE
 *  @brief handle to config
 *  @sa CreateConfig() @n
 *		DestroyConfig()
 */
typedef void * GMSEC_CONFIG_HANDLE;
/** @typedef GMSEC_CONFIGFILE_HANDLE
 *  @brief handle to configfile
 *  @sa CreateConfigFile() @n
 *		DestroyConfigFile()
 */
typedef void * GMSEC_CONFIGFILE_HANDLE;

/** @name C Alternate API Handle types
 * @{
 */

/** @typedef GMSEC_STATUS_HANDLE
 *  @brief handle to status
 *  @sa CreateStatus() @n
 *		DestroyStatus()
 */
/*
typedef struct
    {
    } GMSEC_STATUS_STRUCT, *GMSEC_STATUS_OBJECT;
*/
typedef struct GMSEC_STATUS_STRUCT GMSEC_STATUS_STRUCT;

typedef struct GMSEC_STATUS_STRUCT * GMSEC_STATUS_OBJECT;
/** @typedef GMSEC_CONNECTION_HANDLE
 *  @brief handle to connection
 *  @sa CreateConnection() @n
 *		DestroyConnection()
 */
/*
typedef struct
    {
    } GMSEC_CONNECTION_STRUCT, *GMSEC_CONNECTION_OBJECT;
*/

typedef struct GMSEC_CONNECTION_STRUCT GMSEC_CONNECTION_STRUCT;

typedef struct GMSEC_CONNECTION_STRUCT * GMSEC_CONNECTION_OBJECT;
/** @typedef GMSEC_MESSAGE_HANDLE
 *  @brief handle to message
 *  @sa CreateMessage() @n
 *		DestroyMessage()
 */
/*
typedef struct
    {
    } GMSEC_MESSAGE_STRUCT, *GMSEC_MESSAGE_OBJECT;
*/
typedef struct GMSEC_MESSAGE_STRUCT GMSEC_MESSAGE_STRUCT;

typedef struct GMSEC_MESSAGE_STRUCT * GMSEC_MESSAGE_OBJECT;

/** @typedef GMSEC_FIELD_HANDLE
 *  @brief handle to field
 *  @sa CreateField() @n
 *		DestroyField()
 */
/*
typedef struct
    {
    } GMSEC_FIELD_STRUCT, *GMSEC_FIELD_OBJECT;
*/

typedef struct GMSEC_FIELD_STRUCT GMSEC_FIELD_STRUCT;

typedef struct GMSEC_FIELD_STRUCT * GMSEC_FIELD_OBJECT;

/** @typedef GMSEC_CONFIG_HANDLE
 *  @brief handle to config
 *  @sa CreateConfig() @n
 *		DestroyConfig()
 */
/*
typedef struct
    {
    } GMSEC_CONFIG_STRUCT, *GMSEC_CONFIG_OBJECT;
*/
typedef struct GMSEC_CONFIG_STRUCT GMSEC_CONFIG_STRUCT;

typedef struct GMSEC_CONFIG_STRUCT * GMSEC_CONFIG_OBJECT;

/** @typedef GMSEC_CONFIGFILE_HANDLE
 *  @brief handle to configfile
 *  @sa CreateConfigFile() @n
 *		DestroyConfigFile()
 */
/*
typedef struct
    {
    } GMSEC_CONFIGFILE_STRUCT, *GMSEC_CONFIGFILE_OBJECT;
*/
typedef struct GMSEC_CONFIGFILE_STRUCT GMSEC_CONFIGFILE_STRUCT;

typedef struct GMSEC_CONFIGFILE_STRUCT * GMSEC_CONFIGFILE_OBJECT;


/*  @} */

/** @struct GMSEC_LOG_ENTRY
 *
 * @brief This class defines the contents of a GMSEC_LOG_ENTRY
 *
 * @sa Log
**/
typedef struct
{
	const char *file;
	int line;
	enum LogLevel level;
	double time;
	const char *message;
} GMSEC_LOG_ENTRY;


/** @name C API callback typedefs
 * @{
 */

/** @typedef GMSEC_CALLBACK
 *	@brief This is the function prototype for C API message callbacks.
 *
 *  Example callback function:
 *  @code
 * void CALL_TYPE PublishOnMessage(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE msg)
 * {
 *	char *tmp = NULL;
 *	MsgToXML(msg, &tmp, NULL);
 *	printf ("%s\n", tmp );
 * }
 *  @endcode
 *
 *  Example callback registration
 *  @code
 * SubscribeWCallback(conn, "gmsec.test.publish",PublishOnMessage, result);
 * if( isStatusError(result) )
 *	//handle error
 *  @endcode
 *
 * @note <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
 * They are owned by the API and do not need to be managed by the client program. Also, they can
 * not be stored by the client program beyond the scope of this callback function. In order to store
 * the gmsec::Message, it must be cloned with CloneMessage().
 *
 *	@sa GMSEC_CONNECTION_HANDLE @n
 *		GMSEC_MESSAGE_HANDLE @n
 *		SubscribeWCallback
 */
typedef void GMSEC_CALLBACK(GMSEC_CONNECTION_HANDLE,GMSEC_MESSAGE_HANDLE);

typedef void GMSEC_C_CALLBACK(GMSEC_CONNECTION_OBJECT,GMSEC_MESSAGE_OBJECT);
/** @typedef GMSEC_ERROR_CALLBACK
 *  @brief This is the function prototype for C API error callbacks.
 *
 *  Example callback function:
 *	@code
 * void CALL_TYPE RequestOnError(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
 * {
 *	char *tmp = NULL;
 *	MsgToXML(msg, &tmp, NULL);
 *	printf ("%s\n", tmp );
 * }
 *  @endcode
 *
 *  Example callback registration:
 *  @code
 * RegisterErrorCallback (conn, GMSEC_CONNECTION_REQUEST_TIMEOUT, RequestOnError, result, const char* );
 * if( isStatusError(result) )
 *	//handle error
 *  @endcode
 *
 * @note <b>DO NOT DESTROY</b> the Connection, Message, or Status that is passed into this function by the API.
 * They are owned by the API and do not need to be managed by the client program. Also, they can
 * not be stored by the client program beyond the scope of this callback function. In order to store
 * the gmsec::Message, it must be cloned with CloneMessage().
 *
 *	@sa GMSEC_CONNECTION_HANDLE @n
 *		GMSEC_MESSAGE_HANDLE @n
 *		GMSEC_STATUS_HANDLE @n
 *		RegisterErrorCallback
 */
typedef void GMSEC_ERROR_CALLBACK(GMSEC_CONNECTION_HANDLE,GMSEC_MESSAGE_HANDLE,GMSEC_STATUS_HANDLE,const char*);

typedef void GMSEC_C_ERROR_CALLBACK(GMSEC_CONNECTION_OBJECT,GMSEC_MESSAGE_OBJECT,GMSEC_STATUS_OBJECT,const char*);

/** @typedef GMSEC_REPLY_CALLBACK
 *	@brief This is the function prototype for C API request/reply callbacks.
 *
 *  Example callback function:
 *  @code
 * void CALL_TYPE PublishOnReply(GMSEC_CONNECTION_HANDLE conn, GMSEC_MESSAGE_HANDLE req, GMSEC_MESSAGE_HANDLE rpl)
 * {
 *	char *tmp = NULL;
 *	// request
 *	MsgToXML(req, &tmp, NULL);
 *	printf ("%s\n", tmp );
 *	// reply
 *	MsgToXML(rpl, &tmp, NULL);
 *	printf ("%s\n", tmp );
 * }
 *  @endcode
 *
 *  Example callback registration
 *  @code
 * SubscribeWCallback(conn, "gmsec.test.publish",PublishOnMessage, result);
 * if( isStatusError(result) )
 *	//handle error
 *  @endcode
 *
 * @note <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
 * They are owned by the API and do not need to be managed by the client program. Also, they can
 * not be stored by the client program beyond the scope of this callback function. In order to store
 * the gmsec::Message, it must be cloned with CloneMessage().
 *
 *	@sa GMSEC_CONNECTION_HANDLE @n
 *		GMSEC_MESSAGE_HANDLE @n
 *		SubscribeWCallback
 */
typedef void GMSEC_REPLY_CALLBACK(GMSEC_CONNECTION_HANDLE,GMSEC_MESSAGE_HANDLE,GMSEC_MESSAGE_HANDLE);
typedef void GMSEC_C_REPLY_CALLBACK(GMSEC_CONNECTION_OBJECT,GMSEC_MESSAGE_OBJECT,GMSEC_MESSAGE_OBJECT);

typedef void GMSEC_LOGGER_HANDLER(const GMSEC_LOG_ENTRY *);
/* @} */


/* Lots of GMSEC API code depends on NULL being defined. */
#ifndef NULL
#define NULL 0
#endif


#endif /* gmsec_defs_h */
