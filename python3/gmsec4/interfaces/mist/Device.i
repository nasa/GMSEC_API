/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
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

%ignore gmsec::api::mist::Device::operator=(const Device&);

%rename("get_name") getName;
%rename("number_available") numberAvailable;
%rename("get_number") getNumber;
%rename("set_number") setNumber;
%rename("model_available") modelAvailable;
%rename("get_model") getModel;
%rename("set_model") setModel;
%rename("serial_available") serialAvailable;
%rename("get_serial") getSerial;
%rename("set_serial") setSerial;
%rename("version_available") versionAvailable;
%rename("get_version") getVersion;
%rename("set_version") setVersion;
%rename("group_available") groupAvailable;
%rename("get_group") getGroup;
%rename("set_group") setGroup;
%rename("role_available") roleAvailable;
%rename("get_role") getRole;
%rename("set_role") setRole;
%rename("get_status") getStatus;
%rename("set_status") setStatus;
%rename("info_available") infoAvailable;
%rename("get_info") getInfo;
%rename("set_info") setInfo;
%rename("get_param_count") getParamCount;
%rename("get_param") getParam;
%rename("add_param") addParam;

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

