/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

%{
#include <gmsec5/Config.h>
using namespace gmsec::api5;
%}

%rename("$ignore", fullname=1) gmsec::api5::Config::Config(int argc, char* argv[]);

%ignore gmsec::api5::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api5::Config::getNext(const char*&, const char*&) const;

%ignore gmsec::api5::Config::operator=(const Config&);;


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


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Config.h>

%include "std_string.i"
%include "std_vector.i"

namespace std
{
    %template(StringArray) vector<std::string>;
}


%extend gmsec::api5::Config {

    Config(const std::string& jsonStr)
    {
        /* Expecting input string to be something like:
         * 
         * ["name1=value1","name2=value2"...]
         */
        std::string tmp = jsonStr;

        size_t pos = tmp.find("[");
        if (pos != std::string::npos) {
            tmp = tmp.replace(pos, 1, "");
        }

        pos = tmp.find("]");
        if (pos != std::string::npos) {
            tmp = tmp.replace(pos, 1, "");
        }

        pos = tmp.find(",");
        while (pos != std::string::npos) {
            tmp = tmp.replace(pos, 1, " ");
            pos = tmp.find(",");
        }

        return new Config(tmp.c_str(), DataType::KEY_VALUE_DATA);
    }

    Config(std::vector<std::string>& args)
    {
        std::string kvData;
        for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
            kvData.append(*it).append(" ");
        }
        return new Config(kvData.c_str(), DataType::KEY_VALUE_DATA);
    }

    bool CALL_TYPE getFirst(ConfigPair* cp) const
    {
        return self->getFirst(cp->name, cp->value);
    }

    bool CALL_TYPE getNext(ConfigPair* cp) const
    {
        return self->getNext(cp->name, cp->value);
    }
}

