/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%rename(MIST_SubscriptionInfo) gmsec::api::mist::SubscriptionInfo;

%module SubscriptionInfo
%{
#include <gmsec4/mist/SubscriptionInfo.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/SubscriptionInfo.h>
