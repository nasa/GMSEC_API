/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Callback

%{
#include <gmsec5/Callback.h>
using namespace gmsec::api5;
%}


/* Turn on director mapping for the Callback package */
%feature("director") Callback;

%rename("OnMessage") onMessage;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Callback.h>

