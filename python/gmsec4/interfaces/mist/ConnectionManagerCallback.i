/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights reserved.
 */

%module(directors="1") ConnectionManagerCallback
%{
#include <gmsec4/mist/ConnectionManagerCallback.h>
using namespace gmsec::api::mist;
%}

//Turn on director mapping for the Callback package
%feature("director") ConnectionManagerCallback;

%include "dox/ConnectionManagerCallback_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerCallback.h>

