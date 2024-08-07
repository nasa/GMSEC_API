/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Status

%{
#include <gmsec5/Status.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::Status::Status(const GmsecException&);
%ignore gmsec::api5::Status::operator=(const Status&);


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Status.h>

