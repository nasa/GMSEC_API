/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ProductFileIterator
%{
#include <gmsec4/mist/ProductFileIterator.h>
using namespace gmsec::api::mist;
%}

%include "dox/ProductFileIterator_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFileIterator.h>
