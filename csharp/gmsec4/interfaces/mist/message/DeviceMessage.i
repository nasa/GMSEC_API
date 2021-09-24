/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%rename(DeviceMessage) gmsec::api::mist::message::DeviceMessage;

%module DeviceMessage

%{
#include <gmsec4/mist/message/DeviceMessage.h>
using namespace gmsec::api::mist::message;
%}

%ignore gmsec::api::mist::message::DeviceMessage::operator=(const DeviceMessage&);

/* Ignore deprecated methods */
%ignore gmsec::api::mist::message::DeviceMessage::getNumDevices() const;
%ignore gmsec::api::mist::message::DeviceMessage::getDevice(size_t) const;

%rename("AddDevice") addDevice;
%rename("GetDeviceIterator") getDeviceIterator;
%rename("ConvertMessage") convertMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/DeviceMessage.h>
