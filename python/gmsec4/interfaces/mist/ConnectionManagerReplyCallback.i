/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Article 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") ConnectionManagerReplyCallback
%{
#include <gmsec4/mist/ConnectionManagerReplyCallback.h>
using namespace gmsec::api::mist;
%}

%feature("director") ConnectionManagerReplyCallback;

%include "dox/ConnectionManagerReplyCallback_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManagerReplyCallback.h>

