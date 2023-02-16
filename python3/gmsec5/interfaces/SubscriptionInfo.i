/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SubscriptionInfo
%{
#include <gmsec5/SubscriptionInfo.h>
using namespace gmsec::api5;
%}

%rename("get_subject_pattern") getSubjectPattern;


%include "dox/SubscriptionInfo_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/SubscriptionInfo.h>

