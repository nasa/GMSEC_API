/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") EventCallback
%{
#include <gmsec5/EventCallback.h>
using namespace gmsec::api5;
%}
// Turn on director mapping for the Callback class
%feature("director") EventCallback;

%rename("on_event") onEvent;

%include "dox/EventCallback_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/EventCallback.h>

