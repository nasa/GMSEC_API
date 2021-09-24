/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
* @file Enums.h
* Holds the declaration of ZeroMQ HeartbeatType enum
*/

#ifndef ZMQ_ENUMS_H
#define ZMQ_ENUMS_H

#include <gmsec4/util/Condition.h>

namespace gmsec_zeromq
{

	enum Reasons
	{
		QUIT = gmsec::api::util::Condition::USER + 1,
		TIMEOUT,
		GOT_MESSAGE,
		LISTENING,
	};

} // namespace gmsec_zeromq

#endif
