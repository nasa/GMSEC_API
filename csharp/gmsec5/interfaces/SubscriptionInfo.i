/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SubscriptionInfo

%{
#include <gmsec5/SubscriptionInfo.h>
using namespace gmsec::api5;
%}

%rename("GetSubjectPattern") getSubjectPattern;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/SubscriptionInfo.h>

