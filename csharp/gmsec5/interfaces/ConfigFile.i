/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFile


/* SWIG does not handle nested classes well for certain output languages
 * Redefine nested classes in global scope in order for SWIG to generate
 * a proxy class. Only SWIG parses this definition.
 */
%rename("GetName") getName;
%rename("GetConfig") getConfig;
%rename("GetMessage") getMessage;
%rename("GetPattern") getPattern;
%rename("AddExcludedPattern") addExcludedPattern;
%rename("HasNextExcludedPattern") hasNextExcludedPattern;
%rename("NextExcludedPattern") nextExcludedPattern;

class GMSEC_API ConfigEntry
{
public:
	const char* CALL_TYPE getName() const;
	const gmsec::api5::Config& CALL_TYPE getConfig() const;
private:
	ConfigEntry();
	~ConfigEntry();
};

class GMSEC_API MessageEntry
{
public:
	const char* CALL_TYPE getName() const;
	const gmsec::api5::Message& CALL_TYPE getMessage() const;
private:
	MessageEntry();
	~MessageEntry();
};

class GMSEC_API SubscriptionEntry
{
public:
	SubscriptionEntry(const char* name, const char* pattern);
	SubscriptionEntry(const SubscriptionEntry& other);
	~SubscriptionEntry();
	const char* CALL_TYPE getName() const;
	const char* CALL_TYPE getPattern() const;
    void CALL_TYPE addExcludedPattern(const char* pattern);
    bool CALL_TYPE hasNextExcludedPattern() const;
    const char* CALL_TYPE nextExcludedPattern() const;
private:
	SubscriptionEntry();
};


/* ignore sub-classes */
%ignore gmsec::api5::ConfigFile::ConfigEntry;
%ignore gmsec::api5::ConfigFile::MessageEntry;
%ignore gmsec::api5::ConfigFile::SubscriptionEntry;


%rename("Load") load;
%rename("Save") save;
%rename("FromXML") fromXML;
%rename("ToXML") toXML;
%rename("IsLoaded") isLoaded;
%rename("AddConfig") addConfig;
%rename("LookupConfig") lookupConfig;
%rename("RemoveConfig") removeConfig;
%rename("AddMessage") addMessage;
%rename("LookupMessage") lookupMessage;
%rename("RemoveMessage") removeMessage;
%rename("AddSubscriptionEntry") addSubscriptionEntry;
%rename("LookupSubscriptionEntry") lookupSubscriptionEntry;
%rename("RemoveSubscriptionEntry") removeSubscriptionEntry;
%rename("GetIterator") getIterator;

%{
#include <gmsec5/ConfigFile.h>
using namespace gmsec::api5;
%}

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/ConfigFile.h>


/* SWIG thinks that Inner is a global class, so we need to trick the C++
 * compiler into understanding this so called global type.
 */
%{
typedef gmsec::api5::ConfigFile::ConfigEntry       ConfigEntry;
typedef gmsec::api5::ConfigFile::MessageEntry      MessageEntry;
typedef gmsec::api5::ConfigFile::SubscriptionEntry SubscriptionEntry;
%}
