/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module HeartbeatGenerator

%{
#include <gmsec4/HeartbeatGenerator.h>
using namespace gmsec::api;
%}

/* Methods containing lists that will be redefined */
%ignore gmsec::api::HeartbeatGenerator::HeartbeatGenerator(const Config&, const char*, GMSEC_U16, const gmsec::api::util::DataList<gmsec::api::Field*>&);

%rename("is_running") isRunning;
%rename("change_publish_rate") changePublishRate;
%rename("set_field") setField;

%include "dox/HeartbeatGenerator_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/HeartbeatGenerator.h>

%extend gmsec::api::HeartbeatGenerator
{
    HeartbeatGenerator(const Config& config, const char* subject, GMSEC_U16 pubRate, const std::list<gmsec::api::Field*>& fields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> dl_fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
        {
            dl_fields.push_back(*it);
        }

        return new HeartbeatGenerator(config, subject, pubRate, dl_fields);
    }
}
