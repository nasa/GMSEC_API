/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Mutex
%{
#include <gmsec5/util/Mutex.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::util::AutoMutex;

%rename("enter_mutex") enterMutex;
%rename("leave_mutex") leaveMutex;

%include "dox/Mutex_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/Mutex.h>
