/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Device

%include "std_list.i"

%{
#include <gmsec4/mist/Device.h>
#include <list>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Device::Device(const char*, gmsec::api::mist::Device::DeviceStatus, const gmsec::api::util::DataList<DeviceParam*>&);
%ignore gmsec::api::mist::Device::addParam(const DeviceParam&);
%ignore gmsec::api::mist::Device::getParam(size_t) const;

%ignore gmsec::api::mist::Device::operator=(const Device&);

%rename("GetName") getName;
%rename("NumberAvailable") numberAvailable;
%rename("GetNumber") getNumber;
%rename("SetNumber") setNumber;
%rename("ModelAvailable") modelAvailable;
%rename("GetModel") getModel;
%rename("SetModel") setModel;
%rename("SerialAvailable") serialAvailable;
%rename("GetSerial") getSerial;
%rename("SetSerial") setSerial;
%rename("VersionAvailable") versionAvailable;
%rename("GetVersion") getVersion;
%rename("SetVersion") setVersion;
%rename("GroupAvailable") groupAvailable;
%rename("GetGroup") getGroup;
%rename("SetGroup") setGroup;
%rename("RoleAvailable") roleAvailable;
%rename("GetRole") getRole;
%rename("SetRole") setRole;
%rename("GetStatus") getStatus;
%rename("SetStatus") setStatus;
%rename("InfoAvailable") infoAvailable;
%rename("GetInfo") getInfo;
%rename("SetInfo") setInfo;
%rename("GetParamCount") getParamCount;
%rename("GetParam") getParam;
%rename("AddParam") addParam;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Device.h>


PTR_REF_TYPEMAPS(DeviceParam, gmsec::api::mist::DeviceParam*)
SWIG_STD_LIST_SPECIALIZE(DeviceParam, gmsec::api::mist::DeviceParam*)

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

    const gmsec::api::mist::DeviceParam& CALL_TYPE getParam(size_t idx)
    {
        return self->getParam(idx);
    }

    void CALL_TYPE addParam(const gmsec::api::mist::DeviceParam& param)
    {
        self->addParam(param);
    }
}

