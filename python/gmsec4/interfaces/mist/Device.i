/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Article 17, U.S. Code.
 * All Rights Reserved.
 */

%module Device
%{
#include <gmsec4/mist/Device.h>
#include <list>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Device::Device(const char*, gmsec::api::mist::Device::DeviceStatus, const gmsec::api::util::DataList<gmsec::api::mist::DeviceParam*>&);
%ignore gmsec::api::mist::Device::addParam(const DeviceParam&);

%include "dox/Device_dox.i"
%include "dox/DeviceParamList_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Device.h>

%include <std_list.i>
namespace std 
{
    %template(DeviceParamList) list<gmsec::api::mist::DeviceParam*>;
}

%extend gmsec::api::mist::Device {
    
    Device(const char* name, gmsec::api::mist::Device::DeviceStatus status, const std::list<gmsec::api::mist::DeviceParam*>& params)
    {
        gmsec::api::util::DataList<gmsec::api::mist::DeviceParam*> list;

        for(std::list<gmsec::api::mist::DeviceParam*>::const_iterator it = params.begin(); it != params.end(); ++it)
            list.push_back(*it);

        return new Device(name, status, list);
    }

    void CALL_TYPE addParam(const gmsec::api::mist::DeviceParam& param)
    {
        self->addParam(param);
    }
}

