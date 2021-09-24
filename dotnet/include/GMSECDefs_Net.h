
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#ifndef GMSECDefs_Net
#define GMSECDefs_Net

// This is a compiler bug in thinking -1 is out of range for a signed type.  This pragma disables it here.
#pragma warning( disable: 4341 )

using namespace System;

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// Forward references
ref class Connection;

/// <summary>
/// Field type identifiers
/// </summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECTypeDefs : UInt16
{
	/// <summary>field type identifier for UNSET field</summary>
	UNSET = 0,
	
	/// <summary>field type identifier for GMSEC_CHAR field</summary>
	CHAR = 1,
	
	/// <summary>field type identifier for GMSEC_BOOL field</summary>
	BOOL = 2,
	
	/// <summary>field type identifier for GMSEC_I16 field</summary>
	I16 = 3,
	
	/// <summary>field type identifier for GMSEC_U16 field</summary>
	U16 = 4,
	
	/// <summary>field type identifier for GMSEC_I32 field</summary>
	I32 = 5,
	
	/// <summary>field type identifier for GMSEC_U32 field</summary>
	U32 = 6,
	
	/// <summary>field type identifier for GMSECF32 field</summary>
	F32 = 7,
	
	/// <summary>field type identifier for GMSECF64 field</summary>
	F64 = 8,
	
	/// <summary>field type identifier for GMSECSTRING field</summary>
	STRING = 9,
	
	/// <summary>field type identifier for GMSECBLOB field</summary>
	BIN = 10,
	
	/// <summary>field type identifier for GMSECI8 field</summary>
	I8 = 20,
	
	/// <summary>field type identifier for GMSECU8 field</summary>
	U8 = 21,
	
	/// <summary>field type identifier for GMSECI64 field</summary>
	I64 = 22,
	
	/// <summary>field type identifier for GMSECU64 field</summary>
	U64 = 23,
	
	/// <summary>field type identifier for GMSECCOMPOUND field</summary>
	COMPOUND = 24
};

/// <summary>GMSEC Boolean</summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECBooleanDefs : Int16
{
	/// <summary>
	/// GMSEC version of true</summary>
	GMSEC_TRUE_Net = 1,

	/// <summary>
	/// GMSEC version of false</summary>
	GMSEC_FALSE_Net = 0
};

/// <summary>Tracking Field Defs</summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECMessageTrackingFieldsDefs : System::Int16
{
	/// <summary>GMSEC Tracking fields unset</summary>
	UNSET = -1,
	/// <summary>GMSEC Tracking fields on/true</summary>
	ON = GMSECBooleanDefs::GMSEC_TRUE_Net,
	/// <summary>GMSEC Tracking fields off/false</summary>
	OFF = GMSECBooleanDefs::GMSEC_FALSE_Net,
};

/// <summary>Message kind identifiers</summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECMsgKindDefs : UInt16
{
	/// <summary>unset message kind</summary>
	UNSET = 0,

	/// <summary>publish message kind</summary>
	PUBLISH = 1,

	/// <summary>request message kind</summary>
	REQUEST = 2,

	/// <summary>reply message kind</summary>
	REPLY = 3
};

/// <summary>Special timeout values</summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public enum class GMSECWaitDefs
{
	/// <summary>timeout "no wait" constant - causes methods with timeouts to not block</summary>
	/// <seealso cref="Connection::Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, Nasa::Gmsec::Net::ReplyCallback ^ rcb, Int32 republish_ms)" />
	/// <seealso cref="Connection::Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, [Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %reply_msg, Int32 republish_ms)" />
	/// <seealso cref="Connection::GetNextMsg([Out] Nasa::Gmsec::Net::Message ^ %msg, Int32 timeout)" />
	NO_WAIT = 0,

	/// <summary>timeout "wait forever" constant - causes methods with timeouts to block</summary>
	/// <seealso cref="Connection::Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, Nasa::Gmsec::Net::ReplyCallback ^ rcb, Int32 republish_ms)" />
	/// <seealso cref="Connection::Request(Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, [Runtime::InteropServices::Out] Nasa::Gmsec::Net::Message ^ %reply_msg, Int32 republish_ms)" />
	/// <seealso cref="Connection::GetNextMsg([Out] Nasa::Gmsec::Net::Message ^ %msg, Int32 timeout)" />
	WAIT_FOREVER = -1
};

}
}
}

#endif // GMSECDefs_Net
