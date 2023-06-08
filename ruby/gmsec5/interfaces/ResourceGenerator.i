/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ResourceGenerator

%{
#include <gmsec5/ResourceGenerator.h>
using namespace gmsec::api5;
%}

/* Ignore constructors; we will define a create() method */
%ignore gmsec::api5::ResourceGenerator::ResourceGenerator(const Config&, GMSEC_U16, GMSEC_U16, GMSEC_U16);
%ignore gmsec::api5::ResourceGenerator::ResourceGenerator(const Config&, GMSEC_U16, GMSEC_U16, GMSEC_U16, const gmsec::api5::util::List<gmsec::api5::Field*>&);

%rename("is_running") isRunning;
%rename("set_field") setField;
%rename("create_resource_message") createResourceMessage;

/* Track objects */
%trackobjects;

/* Specify custom mark function */
%freefunc ResourceGenerator "free_ResourceGenerator";

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/ResourceGenerator.h>

%extend gmsec::api5::ResourceGenerator
{
    static ResourceGenerator* CALL_TYPE create(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval, const std::vector<gmsec::api5::Field*>* fields = NULL)
    {
        ResourceGenerator* rsrcgen = 0;

        if (fields == NULL)
        {
            rsrcgen = new ResourceGenerator(config, pubRate, sampleInterval, averageInterval);
        }
        else
        {
            gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

            for (std::vector<gmsec::api5::Field*>::const_iterator it = fields->begin(); it != fields->end(); ++it)
            {
                list_fields.push_back(*it);
            }

            rsrcgen = new ResourceGenerator(config, pubRate, sampleInterval, averageInterval, list_fields);
        }

        return rsrcgen;
    }

    static void CALL_TYPE destroy(ResourceGenerator* rsrcgen)
    {
        delete rsrcgen;
    }
}

%header %{
    static void free_ResourceGenerator(void* ptr)
    {
        /* Do nothing; apps should use destroy() method. */
    }
%}
