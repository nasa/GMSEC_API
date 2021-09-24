/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SubscriptionInfo
%{
#include <gmsec4/SubscriptionInfo.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::SubscriptionInfo::getCallbackFunction() const;

%rename("get_subject") getSubject;
%rename("get_callback") getCallback;


%include "dox/SubscriptionInfo_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/SubscriptionInfo.h>

