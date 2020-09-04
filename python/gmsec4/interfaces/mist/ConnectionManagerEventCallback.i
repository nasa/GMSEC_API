/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Article 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ConnectionManagerEventCallback
%{
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
using namespace gmsec::api::mist;
%}

//Turn on director mapping for the Callback class
%feature("director") ConnectionManagerEventCallback;

%include "dox/ConnectionManagerEventCallback_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerEventCallback.h>

