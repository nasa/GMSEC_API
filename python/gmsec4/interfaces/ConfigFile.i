/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFile



%include "dox/ConfigEntry_dox.i"
%include "dox/MessageEntry_dox.i"
%include "dox/SubscriptionEntry_dox.i"
// SWIG does not handle nested classes well for certain output languages
// Redefine nested classes in global scope in order for SWIG to generate
// a proxy class. Only SWIG parses this definition.
class GMSEC_API ConfigEntry
{
public:
	const char* CALL_TYPE getName() const;
	const gmsec::api::Config& CALL_TYPE getConfig() const;
private:
	ConfigEntry();
	~ConfigEntry();
};

class GMSEC_API MessageEntry
{
public:
	const char* CALL_TYPE getName() const;
	const gmsec::api::Message& CALL_TYPE getMessage() const;
private:
	MessageEntry();
	~MessageEntry();
};

class GMSEC_API SubscriptionEntry
{
public:
	const char* CALL_TYPE getName() const;
	const char* CALL_TYPE getPattern() const;
        const char* CALL_TYPE nextExcludedPattern() const;
        bool CALL_TYPE hasNextExcludedPattern() const;
private:
	SubscriptionEntry();
	~SubscriptionEntry();
};



%ignore gmsec::api::ConfigFile::ConfigEntry;
%ignore gmsec::api::ConfigFile::MessageEntry;
%ignore gmsec::api::ConfigFile::SubscriptionEntry;

%{
#include <gmsec4/ConfigFile.h>
using namespace gmsec::api;
%}

%include "dox/ConfigFile_dox.i"
%include <gmsec4/util/Deprecated.h>
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ConfigFile.h>

%{
// SWIG thinks that Inner is a global class, so we need to trick the C++
// compiler into understanding this so called global type.

typedef gmsec::api::ConfigFile::ConfigEntry       ConfigEntry;
typedef gmsec::api::ConfigFile::MessageEntry	  MessageEntry;
typedef gmsec::api::ConfigFile::SubscriptionEntry SubscriptionEntry;
%}

