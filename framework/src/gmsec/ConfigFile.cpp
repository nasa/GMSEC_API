
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file ConfigFile.cpp
 *
 *
*/

#include <gmsec/ConfigFile.h>
#include <gmsec/internal/BaseConfigFile.h>

using namespace gmsec;
using namespace gmsec::internal;


ConfigFile::ConfigFile()
		:
		ptr(new BaseConfigFile())
{
}


ConfigFile::ConfigFile(const char *filepath)
		:
		ptr(new BaseConfigFile(filepath))
{
}


ConfigFile::~ConfigFile()
{
	delete ptr;
	ptr = 0;
}


Status ConfigFile::Load()
{
	return ptr->Load();
}


Status ConfigFile::Load(const char* filepath)
{
	return ptr->Load(filepath);
}


Status ConfigFile::Save(const char * filePath, bool compact)
{
	return ptr->Save(filePath, compact);
}


bool ConfigFile::IsLoaded()
{
	return ptr->IsLoaded();
}


Status ConfigFile::FromXML(const char *xml)
{
	return ptr->FromXML(xml);
}


Status ConfigFile::ToXML(const char *&xml)
{
	return ptr->ToXML(xml);
}


Status ConfigFile::LookupConfig(const char *name, Config *cfg)
{
	return ptr->LookupConfig(name, cfg);
}


Status ConfigFile::LookupMessage(const char *name, gmsec::Message *msg)
{
	return ptr->LookupMessage(name, msg);
}


Status ConfigFile::LookupSubscription(const char *name, const char *&pattern)
{
	return ptr->LookupSubscription(name, pattern);
}


Status ConfigFile::Reset(ConfigFileIterator &iter)
{
	return ptr->Reset(iter);
}


Status ConfigFile::GetConfigNames(gmsec::util::List<std::string> &names)
{
	return ptr->GetConfigNames(names);
}


Status ConfigFile::GetConfigs(gmsec::util::List<Config*> &configList)
{
	return ptr->GetConfigs(configList);
}


Status ConfigFile::GetSubscriptionNames(gmsec::util::List<std::string> &names)
{
	return ptr->GetSubscriptionNames(names);
}


Status ConfigFile::GetSubscriptions(gmsec::util::List<std::string> &patterns)
{
	return ptr->GetSubscriptions(patterns);
}


Status ConfigFile::GetMessageNames(gmsec::util::List<std::string> &names)
{
	return ptr->GetMessageNames(names);
}


Status ConfigFile::GetMessages(gmsec::Connection *connection, gmsec::util::List<gmsec::Message*> &message)
{
	return ptr->GetMessages(connection, message);
}


Status ConfigFile::AddSubscription(const char * name, const char *subscription)
{
	return ptr->AddSubscription(name, subscription);
}


Status ConfigFile::AddConfig(const char *name, gmsec::Config& config)
{
	return ptr->AddConfig(name, config);
}


Status ConfigFile::AddCustomXML(const char * xml)
{
	return ptr->AddCustomXML(xml);
}


Status ConfigFile::AddMessage(const char *name, gmsec::Message& message)
{
	return ptr->AddMessage(name, message);
}


Status ConfigFile::RemoveSubscription(const char * name)
{
	return ptr->RemoveSubscription(name);
}


Status ConfigFile::RemoveConfig(const char * name)
{
	return ptr->RemoveConfig(name);
}


Status ConfigFile::RemoveMessage(const char * name)
{
	return ptr->RemoveMessage(name);
}


Status ConfigFile::RemoveCustomXML(const char * xml)
{
	return ptr->RemoveCustomXML(xml);
}
