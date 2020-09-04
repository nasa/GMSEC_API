/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ServiceParam

%{
#include <gmsec4/mist/ServiceParam.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::ServiceParam::operator==(const ServiceParam&) const;
%ignore gmsec::api::mist::ServiceParam::operator!=(const ServiceParam&) const;

%rename("GetName") getName;
%rename("GetValue") getValue;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ServiceParam.h>
