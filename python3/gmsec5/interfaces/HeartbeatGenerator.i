/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module HeartbeatGenerator

%{
#include <gmsec5/HeartbeatGenerator.h>
using namespace gmsec::api5;
%}

/* Methods containing lists that will be redefined */
%ignore gmsec::api5::HeartbeatGenerator::HeartbeatGenerator(const Config&, GMSEC_U16, const gmsec::api5::util::List<gmsec::api5::Field*>&);

%rename("is_running") isRunning;
%rename("change_publish_rate") changePublishRate;
%rename("set_field") setField;

%include "dox/HeartbeatGenerator_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/HeartbeatGenerator.h>

%extend gmsec::api5::HeartbeatGenerator
{
    HeartbeatGenerator(const Config& config, GMSEC_U16 pubRate, const std::list<gmsec::api5::Field*>& fields)
    {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::list<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
        {
            list_fields.push_back(*it);
        }

        return new HeartbeatGenerator(config, pubRate, list_fields);
    }
}
