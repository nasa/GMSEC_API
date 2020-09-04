/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

/*
struct ConfigPair
{
  char* name;
  char* value;
};
*/

%{
#include <gmsec4/Config.h>
using namespace gmsec::api;
%}


%ignore gmsec::api::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api::Config::getNext(const char*&, const char*&) const;

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
