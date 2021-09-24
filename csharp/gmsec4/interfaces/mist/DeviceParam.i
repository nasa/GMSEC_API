/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module DeviceParam

%{
#include <gmsec4/mist/DeviceParam.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::DeviceParam::operator==(const DeviceParam&) const;
%ignore gmsec::api::mist::DeviceParam::operator!=(const DeviceParam&) const;

%rename("GetName") getName;
%rename("GetTimestamp") getTimestamp;
%rename("GetValue") getValue;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/DeviceParam.h>
