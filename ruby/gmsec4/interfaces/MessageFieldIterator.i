/*
 * Copyright 2007-2020 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFieldIterator

%{
#include <gmsec4/MessageFieldIterator.h>
using namespace gmsec::api;
%}

%rename("has_next") hasNext;
%rename("next") next;
%rename("reset") reset;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/MessageFieldIterator.h>

