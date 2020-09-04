/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%rename(MIST_SubscriptionInfo) gmsec::api::mist::SubscriptionInfo;

%module SubscriptionInfo
%{
#include <gmsec4/mist/SubscriptionInfo.h>
using namespace gmsec::api::mist;
%}

%include "dox/MIST_SubscriptionInfo_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/SubscriptionInfo.h>
