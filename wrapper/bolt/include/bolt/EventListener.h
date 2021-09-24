/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef bolt_EventListener_h
#define bolt_EventListener_h

#include <bolt/Types.h>

namespace bolt
{

class EventListener
{
public:
	virtual void onEvent(State state) = 0;
};

} // namespace bolt

#endif
