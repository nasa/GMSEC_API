/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") LogHandler
%{
#include <gmsec4/util/LogHandler.h>
using namespace gmsec::api;
%}

// Turn on director mapping for the Callback class
%feature("director") LogHandler;

%rename("on_message") onMessage;

%apply SWIGTYPE *DISOWN { LogHandler *cb };

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/LogHandler.h>

