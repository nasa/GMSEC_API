/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Condition

%{
#include <gmsec5/util/Condition.h>
using namespace gmsec::api5;
%}

%rename("get_mutex") getMutex;
%rename("wait_until") waitUntil;

%include "dox/Condition_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/Condition.h>

