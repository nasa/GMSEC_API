/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <ConfigFile_Net.h>

#include <Config_Net.h>
#include <ConfigFileIterator_Net.h>
#include <GMSEC_Exception_Net.h>
#include <Message_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/Config.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>


using namespace GMSEC::API;
using namespace System;
using namespace System::Runtime::InteropServices;


ConfigFile::ConfigFile()
{
	m_impl = new gmsec::api::ConfigFile();
}


ConfigFile::~ConfigFile()
{
	this->!ConfigFile();
}


void ConfigFile::Load(String^ filePath)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(filePath, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "File path is null");

	char* filePathStr = nullptr;

	try
	{
		filePathStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(filePath).ToPointer());

		m_impl->load(filePathStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process filePath");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(filePathStr);
	}
}


void ConfigFile::Save()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");

	try
	{
		m_impl->save();
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConfigFile::Save(String^ filePath)
{
	this->Save(filePath, false);
}


void ConfigFile::Save(String^ filePath, bool compact)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(filePath, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "File path is null");

	char* filePathStr = nullptr;

	try
	{
		filePathStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(filePath).ToPointer());

		m_impl->save(filePathStr, compact);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process filePath");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(filePathStr);
	}
}


void ConfigFile::FromXML(String^ xml)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(xml, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "XML string is null");

	char* xmlStr = nullptr;

	try
	{
		xmlStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(xml).ToPointer());

		m_impl->fromXML(xmlStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}


String^ ConfigFile::ToXML()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");

	return gcnew String(m_impl->toXML());
}


bool ConfigFile::IsLoaded()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");

	return m_impl->isLoaded();
}


Config^ ConfigFile::LookupConfig(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::Config nativeConfig = m_impl->lookupConfig(nameStr);

		return gcnew Config(new gmsec::api::Config(nativeConfig), true);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Message^ ConfigFile::LookupMessage(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::Message nativeMsg = m_impl->lookupMessage(nameStr);

		return gcnew Message(new gmsec::api::Message(nativeMsg), true);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


String^ ConfigFile::LookupSubscription(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return gcnew String(m_impl->lookupSubscription(nameStr));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


ConfigFile::SubscriptionEntry^ ConfigFile::LookupSubscriptionEntry(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::ConfigFile::SubscriptionEntry& entry =
			const_cast<gmsec::api::ConfigFile::SubscriptionEntry&>(m_impl->lookupSubscriptionEntry(nameStr));

		return gcnew ConfigFile::SubscriptionEntry(&entry);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void ConfigFile::AddSubscription(String^ name, String^ subscription)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");
	THROW_EXCEPTION_IF_NULLPTR(subscription, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Subscription string is null");

	char* nameStr = nullptr;
	char* subStr  = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());
		subStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subscription).ToPointer());

		m_impl->addSubscription(nameStr, subStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process string Parameter(s)");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(subStr);
	}
}


void ConfigFile::AddConfig(String^ name, Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::CONFIGFILE_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::Config* nativeConfig = ((Config^) config)->GetUnmanagedImplementation();

		m_impl->addConfig(nameStr, *nativeConfig);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


void ConfigFile::AddMessage(String^ name, Message^ message)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");
	THROW_EXCEPTION_IF_NULLPTR(message, StatusClass::CONFIGFILE_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::Message* nativeMessage = ((Message^) message)->GetUnmanagedImplementation();

		m_impl->addMessage(nameStr, *nativeMessage);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


void ConfigFile::AddCustomXML(String^ xml)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(xml, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "XML string is null");

	char* xmlStr = nullptr;

	try
	{
		xmlStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(xml).ToPointer());

		m_impl->addCustomXML(xmlStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}


bool ConfigFile::RemoveSubscription(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->removeSubscription(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


bool ConfigFile::RemoveConfig(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->removeConfig(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


bool ConfigFile::RemoveMessage(String^ name)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(name, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Name string is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->removeMessage(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


bool ConfigFile::RemoveCustomXML(String^ xml)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");
	THROW_EXCEPTION_IF_NULLPTR(xml, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "XML string is null");

	char* xmlStr = nullptr;

	try
	{
		xmlStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(xml).ToPointer());

		return m_impl->removeCustomXML(xmlStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
        throw gcnew GMSEC_Exception(StatusClass::CONFIGFILE_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}


ConfigFileIterator^ ConfigFile::GetIterator()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::CONFIGFILE_ERROR, StatusCode::UNINITIALIZED_OBJECT, "ConfigFile is null");

	try
	{
		gmsec::api::ConfigFileIterator& iter = m_impl->getIterator();

		return gcnew ConfigFileIterator(&iter);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


ConfigFile::!ConfigFile()
{
	ResetUnmanagedImplementation();
}


ConfigFile::ConfigFile(gmsec::api::ConfigFile* cfgFile)
{
	m_impl = cfgFile;
}


gmsec::api::ConfigFile* ConfigFile::GetUnmanagedImplementation()
{
	return m_impl;
}


void ConfigFile::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void ConfigFile::SetUnmanagedImplementation(gmsec::api::ConfigFile* cfgFile)
{
	ResetUnmanagedImplementation();

	m_impl = cfgFile;
}


// ConfigEntry
//
String^ ConfigFile::ConfigEntry::GetName()
{
	return gcnew String(m_entry->getName());
}


Config^ ConfigFile::ConfigEntry::GetConfig()
{
	gmsec::api::Config& cfg = const_cast<gmsec::api::Config&>(m_entry->getConfig());

	return gcnew Config(&cfg, false);
}


ConfigFile::ConfigEntry::ConfigEntry()
{
	// nothing to do here
}


ConfigFile::ConfigEntry::~ConfigEntry()
{
	this->!ConfigEntry();
}


ConfigFile::ConfigEntry::!ConfigEntry()
{
	ResetUnmanagedImplementation();
}


ConfigFile::ConfigEntry::ConfigEntry(gmsec::api::ConfigFile::ConfigEntry* entry)
{
	SetUnmanagedImplementation(entry);
}


gmsec::api::ConfigFile::ConfigEntry* ConfigFile::ConfigEntry::GetUnmanagedImplementation()
{
	return m_entry;
}


void ConfigFile::ConfigEntry::ResetUnmanagedImplementation()
{
	m_entry = nullptr;
}


void ConfigFile::ConfigEntry::SetUnmanagedImplementation(gmsec::api::ConfigFile::ConfigEntry* entry)
{
	ResetUnmanagedImplementation();

	m_entry = entry;
}


// MessageEntry
//
String^ ConfigFile::MessageEntry::GetName()
{
	return gcnew String(m_entry->getName());
}


Message^ ConfigFile::MessageEntry::GetMessage()
{
	gmsec::api::Message& msg = const_cast<gmsec::api::Message&>(m_entry->getMessage());

	return gcnew Message(&msg, false);
}


ConfigFile::MessageEntry::MessageEntry()
{
	// nothing to do here
}


ConfigFile::MessageEntry::~MessageEntry()
{
	this->!MessageEntry();
}


ConfigFile::MessageEntry::!MessageEntry()
{
	ResetUnmanagedImplementation();
}


ConfigFile::MessageEntry::MessageEntry(gmsec::api::ConfigFile::MessageEntry* entry)
{
	SetUnmanagedImplementation(entry);
}


gmsec::api::ConfigFile::MessageEntry* ConfigFile::MessageEntry::GetUnmanagedImplementation()
{
	return m_entry;
}


void ConfigFile::MessageEntry::ResetUnmanagedImplementation()
{
	m_entry = nullptr;
}


void ConfigFile::MessageEntry::SetUnmanagedImplementation(gmsec::api::ConfigFile::MessageEntry* entry)
{
	ResetUnmanagedImplementation();

	m_entry = entry;
}


// SubscriptionEntry
//
String^ ConfigFile::SubscriptionEntry::GetName()
{
	return gcnew String(m_entry->getName());
}


String^ ConfigFile::SubscriptionEntry::GetSubject()
{
	return gcnew String(m_entry->getPattern());
}


String^ ConfigFile::SubscriptionEntry::GetPattern()
{
    return gcnew String(m_entry->getPattern());
}


String^ ConfigFile::SubscriptionEntry::NextExcludedPattern()
{
	return gcnew String(m_entry->nextExcludedPattern());
}

bool ConfigFile::SubscriptionEntry::HasNextExcludedPattern()
{
	return m_entry->hasNextExcludedPattern();
}


ConfigFile::SubscriptionEntry::SubscriptionEntry()
{
	// nothing to do here
}


ConfigFile::SubscriptionEntry::~SubscriptionEntry()
{
	this->!SubscriptionEntry();
}


ConfigFile::SubscriptionEntry::!SubscriptionEntry()
{
	ResetUnmanagedImplementation();
}


ConfigFile::SubscriptionEntry::SubscriptionEntry(gmsec::api::ConfigFile::SubscriptionEntry* entry)
{
	SetUnmanagedImplementation(entry);
}


gmsec::api::ConfigFile::SubscriptionEntry* ConfigFile::SubscriptionEntry::GetUnmanagedImplementation()
{
	return m_entry;
}


void ConfigFile::SubscriptionEntry::ResetUnmanagedImplementation()
{
	if (m_entry != nullptr)
	{
		delete m_entry;
		m_entry = nullptr;
	}
}


void ConfigFile::SubscriptionEntry::SetUnmanagedImplementation(gmsec::api::ConfigFile::SubscriptionEntry* entry)
{
	ResetUnmanagedImplementation();

	m_entry = entry;
}
