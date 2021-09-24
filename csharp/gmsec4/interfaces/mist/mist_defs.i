/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module mist_defs

%{
#include <gmsec4/mist/mist_defs.h>
using namespace gmsec::api::mist;
%}

%include<gmsec4/util/wdllexp.h>
%include <gmsec4/mist/mist_defs.h>
