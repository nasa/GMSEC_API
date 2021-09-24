/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ConnectionManagerEventCallback
%{
#include <gmsec4/mist/ConnectionManagerEventCallback.h>
using namespace gmsec::api::mist;
%}

//Turn on director mapping for the Callback class
%feature("director") ConnectionManagerEventCallback;

%rename("on_event") onEvent;

%include "dox/ConnectionManagerEventCallback_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerEventCallback.h>

