/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


%module HeartbeatGenerator

%include "std_vector.i"

%{
#include <gmsec5/HeartbeatGenerator.h>
#include <vector>
using namespace gmsec::api5;
%}

/* Ignore constructors; we will define a create() method */
%ignore gmsec::api5::HeartbeatGenerator::HeartbeatGenerator(const Config&, GMSEC_U16);
%ignore gmsec::api5::HeartbeatGenerator::HeartbeatGenerator(const Config&, GMSEC_U16, const gmsec::api5::util::List<gmsec::api5::Field*>&);

%rename("start") start;
%rename("stop") stop;
%rename("is_running") isRunning;
%rename("change_publish_rate") changePublishRate;
%rename("set_field") setField;

/* Track objects */
%trackobjects;

/* Specify custom mark function */
%freefunc HeartbeatGenerator "free_HeartbeatGenerator";

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/HeartbeatGenerator.h>

%extend gmsec::api5::HeartbeatGenerator
{
    static HeartbeatGenerator* CALL_TYPE create(const Config& config, GMSEC_U16 pubRate, std::vector<gmsec::api5::Field*>* fields = NULL)
    {
        HeartbeatGenerator* hbgen = 0;

        if (fields == NULL)
        {
            hbgen = new HeartbeatGenerator(config, pubRate);
        }
        else
        {
            gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

            for (std::vector<gmsec::api5::Field*>::iterator it = fields->begin(); it != fields->end(); ++it)
            {
                list_fields.push_back(*it);
            }

            hbgen = new HeartbeatGenerator(config, pubRate, list_fields);
        }

        return hbgen;
    }

    static void CALL_TYPE destroy(HeartbeatGenerator* hbgen)
    {
        delete hbgen;
    }
}

%header %{
    static void free_HeartbeatGenerator(void* ptr)
    {
        /* Do nothing; apps should use destroy() method. */
    }
%}
