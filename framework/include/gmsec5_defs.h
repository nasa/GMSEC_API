/*
 * Copyright 2007-2022 United States Government as represented by the
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


#include <limits.h>


/* Ensure we are on a platform with 8 bit chars */
#if CHAR_BIT != 8
#	error Unsupported platform: number of bits in char is not 8.
#endif


/* Lots of GMSEC API code depends on NULL being defined. */
#ifndef NULL
	#define NULL 0
#endif


/** @name GMSEC Data Types
 * @{
 */

/**
 *  @typedef GMSEC_BOOL
 *  @brief typedef for GMSEC_BOOL
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
	/** @typedef GMSEC_I16
	 *  @brief typedef for GMSEC_I16
	 */
	typedef signed short   GMSEC_I16;

	/** @typedef GMSEC_U16
	 *  @brief typedef for GMSEC_U16
	 */
	typedef unsigned short GMSEC_U16;

	#define GMSEC_USING_SHORT 1

	/** @typedef GMSEC_I32
	 *  @brief typedef for GMSEC_I32
	 */
	typedef signed long    GMSEC_I32;

	/** @typedef GMSEC_U32
	 *  @brief typedef for GMSEC_U32
	 */
	typedef unsigned long  GMSEC_U32;

	#define GMSEC_USING_LONG 1

	/** @typedef GMSEC_I64
	 *  @brief typedef for GMSEC_I64
	 */
	typedef signed long long   GMSEC_I64;

	/** @typedef GMSEC_U64
	 *  @brief typedef for GMSEC_U64
	 */
	typedef unsigned long long GMSEC_U64;

	#define GMSEC_USING_LONG_LONG 1

#elif GMSEC_S16I32L64
	/** @typedef GMSEC_I16
	 *  @brief typedef for GMSEC_I16
	 */
	typedef signed short   GMSEC_I16;

	/** @typedef GMSEC_U16
	 *  @brief typedef for GMSEC_U16
	 */
	typedef unsigned short GMSEC_U16;

	#define GMSEC_USING_SHORT 1

	/** @typedef GMSEC_I32
	 *  @brief typedef for GMSEC_I32
	 */
	typedef signed int   GMSEC_I32;

	/** @typedef GMSEC_U32
	 *  @brief typedef for GMSEC_U32
	 */
	typedef unsigned int GMSEC_U32;

	#define GMSEC_USING_INT 1

	/** @typedef GMSEC_I64
	 *  @brief typedef for GMSEC_I64
	 */
	typedef signed long   GMSEC_I64;

	/** @typedef GMSEC_U64
	 *  @brief typedef for GMSEC_U64
	 */
	typedef unsigned long GMSEC_U64;

	#define GMSEC_USING_LONG 1

#elif GMSEC_VC6
	#define GMSEC_USE_ENUM_FOR_CONSTANTS

	/** @typedef GMSEC_I16
	 *  @brief typedef for GMSEC_I16
	 */
	typedef signed short   GMSEC_I16;

	/** @typedef GMSEC_U16
	 *  @brief typedef for GMSEC_U16
	 */
	typedef unsigned short GMSEC_U16;

	#define GMSEC_USING_SHORT 1

	/** @typedef GMSEC_I32
	 *  @brief typedef for GMSEC_I32
	 */
	typedef signed int   GMSEC_I32;

	/** @typedef GMSEC_U32
	 *  @brief typedef for GMSEC_U32
	 */
	typedef unsigned int GMSEC_U32;

	#define GMSEC_USING_INT 1

	/** @typedef GMSEC_I64
	 *  @brief typedef for GMSEC_I16
	 */
	typedef signed __int64   GMSEC_I64;

	/** @typedef GMSEC_U64
	 *  @brief typedef for GMSEC_U16
	 */
	typedef unsigned __int64 GMSEC_U64;

	#define GMSEC_USING_LONG_LONG 1

#else
	/* determine sizes from limits.h macros */

	/* Prefer short for GMSEC_I16 for backward-compatibility, then int */
	#if (SHRT_MAX >> 14) > 0
		/** @typedef GMSEC_I16
		 *  @brief typedef for GMSEC_I16
		 */
		typedef signed short   GMSEC_I16;

		/** @typedef GMSEC_U16
		 *  @brief typedef for GMSEC_U16
		 */
		typedef unsigned short GMSEC_U16;

		#define GMSEC_USING_SHORT 1

	#elif (INT_MAX >> 14) > 0
		/** @typedef GMSEC_I16
		 *  @brief typedef for GMSEC_I16
		 */
		typedef signed int   GMSEC_I16;

		/** @typedef GMSEC_U16
		 *  @brief typedef for GMSEC_U16
		 */
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
			/** @typedef GMSEC_I32
			 *  @brief typedef for GMSEC_I32
			 */
			typedef signed long   GMSEC_I32;

			/** @typedef GMSEC_U32
			 *  @brief typedef for GMSEC_U32
			 */
			typedef unsigned long GMSEC_U32;

			#define GMSEC_USING_LONG 1

		#elif !defined(GMSEC_USING_INT) && ((INT_MAX >> 30) > 0)
			/** @typedef GMSEC_I32
			 *  @brief typedef for GMSEC_I32
			 */
			typedef signed int   GMSEC_I32;

			/** @typedef GMSEC_U32
			 *  @brief typedef for GMSEC_U32
			 */
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
		/** @typedef GMSEC_I64
		 *  @brief typedef for GMSEC_I64
		 */
		typedef signed long   GMSEC_I64;

		/** @typedef GMSEC_U64
		 *  @brief typedef for GMSEC_U64
		 */
		typedef unsigned long GMSEC_U64;

		#define GMSEC_USING_LONG 1

	#elif ((LLONG_MAX >> 30) >> 30) > 0
		/** @typedef GMSEC_I64
		 *  @brief typedef for GMSEC_I64
		 */
		typedef signed long long   GMSEC_I64;

		/** @typedef GMSEC_U64
		 *  @brief typedef for GMSEC_U64
		 */
		typedef unsigned long long GMSEC_U64;

		#define GMSEC_USING_LONG_LONG 1

	#elif !defined(LLONG_MAX)
		#ifdef GMSEC_WARN_MISSING_LLONG
			/* in order to avoid a warning for each translation unit, only set and change
			 * the GMSEC_USING_LONG_LONG macro if GMSEC_WARN_MISSING_LLONG is defined
			 */
			#define GMSEC_USING_LONG_LONG warning: limits.h does not define LLONG_MAX
		#endif

		/** @typedef GMSEC_I64
		 *  @brief typedef for GMSEC_I64
		 */
		typedef signed long long   GMSEC_I64;

		/** @typedef GMSEC_U64
		 *  @brief typedef for GMSEC_U64
		 */
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
 * @brief A handle to a Config object.
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


/**
 * @typedef GMSEC_Connection
 *
 * @brief A handle to a Connection object.
 *
 * @sa connectionCreate
 * @sa connectionDestroy
 */
typedef void* GMSEC_Connection;


/**
 * @typedef GMSEC_Field
 *
 * @brief A handle to an opaque Field object.
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
 * @brief Typedef for a handle to a Heartbeat Generator object.
 *
 * @sa heartbeatGeneratorCreate
 * @sa heartbeatGeneratorCreateWithFields
 * @sa heartbeatGeneratorDestroy
 */
typedef void* GMSEC_HeartbeatGenerator;


/**
 * @typedef GMSEC_Message
 *
 * @brief Typedef for a handle to a Message object.
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
 * @brief Typedef for a handle to a MessageFactory object.
 *
 * @sa connectionGetMessageFactory
 */
typedef void* GMSEC_MessageFactory;


/**
 * @typedef GMSEC_MessageFieldIterator
 *
 * @brief Typedef for a handle to a MessageFieldIterator object.
 *
 * @sa messageGetFieldIterator
 */
typedef void* GMSEC_MessageFieldIterator;


/**
 * @typedef GMSEC_ResourceGenerator
 *
 * @brief Typedef for a handle to a Resource Generator object.
 *
 * @sa resourceGeneratorCreate
 * @sa resourceGeneratorCreateWithFields
 * @sa resourceGeneratorDestroy
 */
typedef void* GMSEC_ResourceGenerator;


/**
 * @typedef GMSEC_SchemaIDIterator
 *
 * @brief A handle to a SchemaIDIterator object.
 *
 * @sa specificationGetSchemaIDIterator
 */
typedef void* GMSEC_SchemaIDIterator;


/**
 * @typedef GMSEC_Specification
 *
 * @brief A handle to a Specification object.
 *
 * @sa specificationCreate
 * @sa specificationCreateCopy
 * @sa specificationDestroy
 */
typedef void* GMSEC_Specification;


/**
 * @typedef GMSEC_Status
 *
 * @brief A handle to a Status object.
 *
 * @sa statusCreate
 * @sa statusDestroy
 */
typedef void* GMSEC_Status;


/**
 * @struct GMSEC_SubscriptionEntry
 * @brief A SubscriptionEntry in a Configuration file.
 * @sa configFileIteratorNextSubscription
 */
typedef void* GMSEC_SubscriptionEntry;


/**
 * @typedef GMSEC_SubscriptionInfo
 *
 * @brief A handle to a SubscriptionInfo object.
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
