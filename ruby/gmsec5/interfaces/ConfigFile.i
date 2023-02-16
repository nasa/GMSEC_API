/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFile


/* SWIG does not handle nested classes well for certain output languages
 * Redefine nested classes in global scope in order for SWIG to generate
 * a proxy class. Only SWIG parses this definition.
 */
%rename("get_name") getName;
%rename("get_config") getConfig;
%rename("get_message") getMessage;
%rename("get_pattern") getPattern;
%rename("add_excluded_pattern") addExcludedPattern;
%rename("has_next_excluded_pattern") hasNextExcludedPattern;
%rename("next_excluded_pattern") nextExcludedPattern;

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
	~SubscriptionEntry();
	const char* CALL_TYPE getName() const;
	const char* CALL_TYPE getPattern() const;
    void CALL_TYPE addExcludedPattern(const char* pattern);
    bool CALL_TYPE hasNextExcludedPattern() const;
    const char* CALL_TYPE nextExcludedPattern() const;
private:
	SubscriptionEntry();
};



/* ignore ConfigFile's subclasses */
%ignore gmsec::api5::ConfigFile::ConfigEntry;
%ignore gmsec::api5::ConfigFile::MessageEntry;
%ignore gmsec::api5::ConfigFile::SubscriptionEntry;

/* ignore methods that do not translate to Ruby */
%ignore gmsec::api5::ConfigFile::ConfigEntry::operator=(const ConfigEntry&);
%ignore gmsec::api5::ConfigFile::MessageEntry::operator=(const MessageEntry&);
%ignore gmsec::api5::ConfigFile::SubscriptionEntry::operator=(const SubscriptionEntry&);


%rename("load") load;
%rename("save") save;
%rename("from_xml") fromXML;
%rename("to_xml") toXML;
%rename("is_loaded") isLoaded;
%rename("add_config") addConfig;
%rename("lookup_config") lookupConfig;
%rename("remove_config") removeConfig;
%rename("add_message") addMessage;
%rename("lookup_message") lookupMessage;
%rename("remove_message") removeMessage;
%rename("add_subscription_entry") addSubscriptionEntry;
%rename("lookup_subscription_entry") lookupSubscriptionEntry;
%rename("remove_subscription_entry") removeSubscriptionEntry;
%rename("get_iterator") getIterator;


%{
#include <gmsec5/ConfigFile.h>
using namespace gmsec::api5;
%}

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/ConfigFile.h>

%{
// SWIG thinks that Inner is a global class, so we need to trick the C++
// compiler into understanding this so called global type.

typedef gmsec::api5::ConfigFile::ConfigEntry       ConfigEntry;
typedef gmsec::api5::ConfigFile::MessageEntry      MessageEntry;
typedef gmsec::api5::ConfigFile::SubscriptionEntry SubscriptionEntry;
%}

