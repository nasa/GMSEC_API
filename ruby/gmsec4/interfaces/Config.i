/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

%include "std_string.i"
%include "std_vector.i"

%{
#include <gmsec4/Config.h>
#include <string>
#include <vector>
#include <cstring>
using namespace gmsec::api;
%}

%rename("add_value") addValue;
%rename("clear_value") clearValue;
%rename("get_value") getValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_double_value") getDoubleValue;
%rename("get_integer_value") getIntegerValue;
%rename("get_first") getFirst;
%rename("get_next") getNext;
%rename("to_xml") toXML;
%rename("from_xml") fromXML;
%rename("to_json") toJSON;

%ignore gmsec::api::Config::Config(int argc, char* argv[]);

%ignore gmsec::api::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api::Config::getNext(const char*&, const char*&) const;

%ignore gmsec::api::Config::operator=(const Config&);

%inline %{
class ConfigPair
{
public:
    inline const char* get_name() const
    {
        return name.c_str();
    }
    inline const char* get_value() const
    {
        return value.c_str();
    }

    std::string name;
    std::string value;
};
%}


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Config.h>


%extend gmsec::api::Config {

    Config(VALUE rb_array)
    {
        int argc = RARRAY_LEN(rb_array);
        std::vector<char*> argv;

        for (int i = 0; i < argc; ++i)
        {
            VALUE elem = rb_ary_entry(rb_array, i);

            argv.push_back( strdup(StringValueCStr(elem)) );
        }

        Config* config = new Config(argc, &argv[0]);

        for (int i = 0; i < argc; ++i)
        {
            free((void*) argv[i]);
        }

        return config;
    }

    bool CALL_TYPE getFirst(ConfigPair* cp) const
    {
        const char* name;
        const char* value;

        bool result = self->getFirst(name, value);

        if (result)
        {
            cp->name  = name;
            cp->value = value;
        }

        return result;
    }

    bool CALL_TYPE getNext(ConfigPair* cp) const
    {
        const char* name;
        const char* value;

        bool result = self->getNext(name, value);

        if (result)
        {
            cp->name  = name;
            cp->value = value;
        }

        return result;
    }
}

