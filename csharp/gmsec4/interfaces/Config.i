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

%ignore gmsec::api::Config::Config(int argc, char* argv[]);

%ignore gmsec::api::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api::Config::getNext(const char*&, const char*&) const;

%ignore gmsec::api::Config::operator=(const Config&);;

%rename("AddValue") addValue;
%rename("ClearValue") clearValue;
%rename("GetValue") getValue;
%rename("GetBooleanValue") getBooleanValue;
%rename("GetIntegerValue") getIntegerValue;
%rename("GetDoubleValue") getDoubleValue;
%rename("Clear") clear;
%rename("GetFirst") getFirst;
%rename("GetNext") getNext;
%rename("Merge") merge;
%rename("ToXML") toXML;
%rename("FromXML") fromXML;
%rename("ToJSON") toJSON;


%include <arrays_csharp.i>

%typemap(ctype) char** "char**"
%typemap(imtype) char** "string[]"
%typemap(cstype) char** "string[]"
 
%typemap(csin) char** "$csinput"
%typemap(csout, excode=SWIGEXCODE) char**, const char**& {
    int ret = $imcall;$excode
    return ret;
  }
%typemap(csvarin, excode=SWIGEXCODE2) char** %{
    set {
      $imcall;$excode
    } %}
%typemap(csvarout, excode=SWIGEXCODE2) char** %{
    get {
      int ret = $imcall;$excode
      return ret;
    } %}
 
%typemap(in) char** %{ $1 = $input; %}
%typemap(out) char** %{ $result = $1; %}


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


%typemap(cscode) gmsec::api::Config %{
    [System.Obsolete("Use Config(string[] args) instead.")]
    public static Config Initialize(string[] args) {
        return new Config(args);
    }

    public Config(string[] args) : this(args.Length, args) {
    }

%}


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Config.h>


%extend gmsec::api::Config {

    Config(int argc, char** argv)
    {
        return new Config(argc, argv);
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

