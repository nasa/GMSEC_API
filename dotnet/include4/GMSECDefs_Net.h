/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#ifndef GMSEC_API_GMSEC_DEFS_NET_H
#define GMSEC_API_GMSEC_DEFS_NET_H

// There is a compiler bug which indicates that -1 is out of range for a signed type.
// This pragma disables it here.
#pragma warning( disable: 4341 )


namespace GMSEC
{
namespace API
{

/// <summary>Special timeout values</summary>
public enum class GMSECWaitDefs
{
	/// <summary>timeout "no wait" constant - causes methods with timeouts to not block</summary>
	NO_WAIT = 0,

	/// <summary>timeout "wait forever" constant - causes methods with timeouts to block</summary>
	WAIT_FOREVER = -1,

	/// <summary>
	/// Value to indicate that a request message should only be issued once
	/// should a response message not be received.
	/// </summary>
	REQUEST_REPUBLISH_NEVER = -1
};

}
}

#endif
