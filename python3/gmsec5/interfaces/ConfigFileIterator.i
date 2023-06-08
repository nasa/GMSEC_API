/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFileIterator
%{
#include <gmsec5/ConfigFileIterator.h>
using namespace gmsec::api5;
%}


%ignore gmsec::api5::ConfigFileIterator::nextConfig() const;
%ignore gmsec::api5::ConfigFileIterator::nextMessage() const;
%ignore gmsec::api5::ConfigFileIterator::nextSubscription() const;

%rename("has_next_config") hasNextConfig;
%rename("has_next_message") hasNextMessage;
%rename("has_next_subscription") hasNextSubscription;
%rename("next_config") nextConfig;
%rename("next_message") nextMessage;
%rename("next_subscription") nextSubscription;


%include "dox/ConfigFileIterator_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/ConfigFileIterator.h>

// Note: %extend places new additions into public member data
%extend gmsec::api5::ConfigFileIterator {
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

