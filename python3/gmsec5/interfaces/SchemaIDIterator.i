/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module SchemaIDIterator
%{
#include <gmsec5/SchemaIDIterator.h>
using namespace gmsec::api5;
%}

%rename("has_next") hasNext;

%include "dox/SchemaIDIterator_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/SchemaIDIterator.h>
