/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFile


/* SWIG does not handle nested classes well for certain output languages
 * Redefine nested classes in global scope in order for SWIG to generate
 * a proxy class. Only SWIG parses this definition.
 */

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


/* ignore methods that do not translate to JavaScript */
%ignore gmsec::api5::ConfigFile::ConfigEntry::operator=(const ConfigEntry&);
%ignore gmsec::api5::ConfigFile::MessageEntry::operator=(const MessageEntry&);
%ignore gmsec::api5::ConfigFile::SubscriptionEntry::operator=(const SubscriptionEntry&);


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

