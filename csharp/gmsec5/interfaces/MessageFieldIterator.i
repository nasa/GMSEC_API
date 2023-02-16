/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFieldIterator

%{
#include <gmsec5/MessageFieldIterator.h>
using namespace gmsec::api5;
%}

%rename("HasNext") hasNext;
%rename("Next") next;
%rename("Reset") reset;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFieldIterator.h>

