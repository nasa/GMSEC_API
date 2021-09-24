/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFile


%include "dox/ConfigEntry_dox.i"
%include "dox/MessageEntry_dox.i"
%include "dox/SubscriptionEntry_dox.i"

/* SWIG does not handle nested classes well for certain output languages
 * Redefine nested classes in global scope in order for SWIG to generate
 * a proxy class. Only SWIG parses this definition.
 */
%rename("get_name") getName;
%rename("get_config") getConfig;
%rename("get_message") getMessage;
%rename("get_pattern") getPattern;
%rename("next_excluded_pattern") nextExcludedPattern;
%rename("has_next_excluded_pattern") hasNextExcludedPattern;

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
%ignore gmsec::api::ConfigFile::ConfigEntry::operator=(const ConfigEntry&);
%ignore gmsec::api::ConfigFile::MessageEntry;
%ignore gmsec::api::ConfigFile::MessageEntry::operator=(const MessageEntry&);
%ignore gmsec::api::ConfigFile::SubscriptionEntry;
%ignore gmsec::api::ConfigFile::SubscriptionEntry::operator=(const SubscriptionEntry&);

/* Ignore deprecated methods */
%ignore gmsec::api::ConfigFile::getSubject() const;
%ignore gmsec::api::ConfigFile::lookupSubscription(const char*) const;
%ignore gmsec::api::ConfigFile::SubscriptionEntry::getSubject() const;

%rename("load") load;
%rename("save") save;
%rename("from_XML") fromXML;
%rename("to_XML") toXML;
%rename("is_loaded") isLoaded;
%rename("lookup_config") lookupConfig;
%rename("lookup_message") lookupMessage;
%rename("lookup_subscription_entry") lookupSubscriptionEntry;
%rename("add_subscription") addSubscription;
%rename("add_config") addConfig;
%rename("add_message") addMessage;
%rename("add_custom_XML") addCustomXML;
%rename("remove_subscription") removeSubscription;
%rename("remove_config") removeConfig;
%rename("remove_message") removeMessage;
%rename("remove_custom_XML") removeCustomXML;
%rename("get_iterator") getIterator;


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

