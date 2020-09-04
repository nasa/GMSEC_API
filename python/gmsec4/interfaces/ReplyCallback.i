/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ReplyCallback
%{
#include <gmsec4/ReplyCallback.h>
using namespace gmsec::api;
%}

// Turn on director mapping for the Callback class
%feature("director") ReplyCallback;

%include "dox/ReplyCallback_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ReplyCallback.h>

