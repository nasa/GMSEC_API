/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Callback

%{
#include <gmsec4/Callback.h>
using namespace gmsec::api;
%}


/* Turn on director mapping for the Callback package */
%feature("director") Callback;

%rename("OnMessage") onMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Callback.h>

