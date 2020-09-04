/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFileIterator
%{
#include <gmsec4/ConfigFileIterator.h>
using namespace gmsec::api;
%}


%ignore gmsec::api::ConfigFileIterator::nextConfig() const;
%ignore gmsec::api::ConfigFileIterator::nextMessage() const;
%ignore gmsec::api::ConfigFileIterator::nextSubscription() const;

%include "dox/ConfigFileIterator_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ConfigFileIterator.h>

// Note: %extend places new additions into public member data
%extend gmsec::api::ConfigFileIterator {
	const ConfigEntry& CALL_TYPE nextConfig()
	{
		return self->nextConfig();
	}

	const MessageEntry& CALL_TYPE nextMessage()
	{
		return self->nextMessage();
	}

	const SubscriptionEntry& CALL_TYPE nextSubscription()
	{
		return self->nextSubscription();
	}
};

