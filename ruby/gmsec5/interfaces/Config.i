/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

%include "std_string.i"
%include "std_vector.i"

%{
#include <gmsec5/Config.h>
#include <string>
#include <vector>
#include <cstring>
using namespace gmsec::api5;
%}

/* ignore methods that will be overwritten with new implementation */
%ignore gmsec::api5::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api5::Config::getNext(const char*&, const char*&) const;
%ignore gmsec::api5::Config::Config(int argc, char* argv[]);

/* ignore method that cannot be translated to Ruby */
%ignore gmsec::api5::Config::operator=(const Config&);

%rename("add_value") addValue;
%rename("clear_value") clearValue;
%rename("get_value") getValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_integer_value") getIntegerValue;
%rename("get_double_value") getDoubleValue;
%rename("get_first") getFirst;
%rename("get_next") getNext;
%rename("to_xml") toXML;
%rename("from_xml") fromXML;
%rename("get_from_file") getFromFile;
%rename("to_json") toJSON;

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


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Config.h>


%extend gmsec::api5::Config {

    Config(VALUE rb_array)
    {
        int argc = RARRAY_LEN(rb_array);
        Config* config;

        if (argc > 0)
        {
            std::vector<char*> argv;

            for (int i = 0; i < argc; ++i)
            {
                VALUE elem = rb_ary_entry(rb_array, i);

                argv.push_back( strdup(StringValueCStr(elem)) );
            }

            config = new Config(argc, &argv[0]);

            for (int i = 0; i < argc; ++i)
            {
                free((void*) argv[i]);
            }
        }
        else
        {
            config = new Config();
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

