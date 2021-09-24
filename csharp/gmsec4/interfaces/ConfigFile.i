/*
 * Copyright 2007-2021 United States Government as represented by the
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
%rename("NextExcludedPattern") nextExcludedPattern;
%rename("HasNextExcludedPattern") hasNextExcludedPattern;

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


/* ignore sub-classes */
%ignore gmsec::api::ConfigFile::ConfigEntry;
%ignore gmsec::api::ConfigFile::MessageEntry;
%ignore gmsec::api::ConfigFile::SubscriptionEntry;

/* ignore methods that are not translated well by Swig */
/*
%ignore gmsec::api::ConfigFile::lookupSubscriptionEntry(const char*);
*/

/* Ignore deprecated methods */
%ignore gmsec::api::ConfigFile::getSubject() const;
%ignore gmsec::api::ConfigFile::lookupSubscription(const char*) const;
%ignore gmsec::api::ConfigFile::SubscriptionEntry::getSubject() const;

%rename("Load") load;
%rename("Save") save;
%rename("FromXML") fromXML;
%rename("ToXML") toXML;
%rename("IsLoaded") isLoaded;
%rename("LookupConfig") lookupConfig;
%rename("LookupMessage") lookupMessage;
%rename("LookupSubscriptionEntry") lookupSubscriptionEntry;
%rename("AddSubscription") addSubscription;
%rename("AddConfig") addConfig;
%rename("AddMessage") addMessage;
%rename("AddCustomXML") addCustomXML;
%rename("RemoveSubscription") removeSubscription;
%rename("RemoveConfig") removeConfig;
%rename("RemoveMessage") removeMessage;
%rename("RemoveCustomXML") removeCustomXML;
%rename("GetIterator") getIterator;

%{
#include <gmsec4/ConfigFile.h>
using namespace gmsec::api;
%}

%include <gmsec4/util/Deprecated.h>
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ConfigFile.h>


/* SWIG thinks that Inner is a global class, so we need to trick the C++
 * compiler into understanding this so called global type.
 */
%{
typedef gmsec::api::ConfigFile::ConfigEntry       ConfigEntry;
typedef gmsec::api::ConfigFile::MessageEntry	  MessageEntry;
typedef gmsec::api::ConfigFile::SubscriptionEntry SubscriptionEntry;
%}
