/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

%{
#include <gmsec4/Config.h>
using namespace gmsec::api;
%}


%ignore gmsec::api::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api::Config::getNext(const char*&, const char*&) const;

%ignore gmsec::api::Config::operator=(const Config&);

%rename("add_value") addValue;
%rename("clear_value") clearValue;
%rename("get_value") getValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_integer_value") getIntegerValue;
%rename("get_double_value") getDoubleValue;
%rename("to_XML") toXML;
%rename("from_XML") fromXML;
%rename("to_JSON") toJSON;


%include "dox/ConfigPair_dox.i"

%inline %{
class ConfigPair
{
public:
    inline const char* getName() const
    {
        return name;
    }
    inline const char* getValue() const
    {
        return value;
    }

    const char* name;
    const char* value;
};
%}


/* For support of Config(int argc, char* argv[]) */
%include <argcargv.i>
%apply (int ARGC, char **ARGV) { (int argc, char *argv[]) }


%include "dox/Config_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Config.h>


%extend gmsec::api::Config {

    bool CALL_TYPE getFirst(ConfigPair* cp) const
    {
        return self->getFirst(cp->name, cp->value);
    }

    bool CALL_TYPE getNext(ConfigPair* cp) const
    {
        return self->getNext(cp->name, cp->value);
    }
}
