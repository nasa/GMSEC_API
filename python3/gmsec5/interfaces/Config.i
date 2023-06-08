/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

%{
#include <gmsec5/Config.h>
using namespace gmsec::api5;
%}


/* ignore methods that will be overwritten with new implementation */
%ignore gmsec::api5::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api5::Config::getNext(const char*&, const char*&) const;

/* ignore method that cannot be translated to Python */
%ignore gmsec::api5::Config::operator=(const Config&);

%rename("add_value") addValue;
%rename("clear_value") clearValue;
%rename("get_value") getValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_integer_value") getIntegerValue;
%rename("get_double_value") getDoubleValue;
%rename("to_xml") toXML;
%rename("from_xml") fromXML;
%rename("get_from_file") getFromFile;
%rename("to_json") toJSON;


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
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Config.h>


%extend gmsec::api5::Config {

    bool CALL_TYPE getFirst(ConfigPair* cp) const
    {
        return self->getFirst(cp->name, cp->value);
    }

    bool CALL_TYPE getNext(ConfigPair* cp) const
    {
        return self->getNext(cp->name, cp->value);
    }
}
