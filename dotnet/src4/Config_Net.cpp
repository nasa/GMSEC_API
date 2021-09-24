/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#include <Config_Net.h>
#include <GMSEC_Exception_Net.h>

#include <GMSEC_Net.h>

#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace System;
using namespace System::Runtime::InteropServices;


Config::Config()
{
	m_impl  = new gmsec::api::Config();
	m_owned = true;
}


Config::Config(array<String^>^ argv)
{
	char** pairs = nullptr;
	int    argc  = 0;

	try
	{
		if (argv != nullptr && argv->Length > 0)
		{
			pairs = new char*[argv->Length];

			for (int i = 0; i < argv->Length; ++i)
			{
				if (argv[i] != nullptr)
				{
					pairs[i] = static_cast<char*>(Marshal::StringToHGlobalAnsi(argv[i]).ToPointer());
					++argc;
				}
			}

			m_impl  = new gmsec::api::Config(argc, pairs);
			m_owned = true;
		}
		else
		{
			m_impl  = new gmsec::api::Config();
			m_owned = true;
		}
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config constructor is unable to process command-line arguments");
	}
	finally
	{
		if (pairs != nullptr)
		{
			for (int i = 0; i < argc; ++i)
			{
				FREE_HGLOBAL_IF_NOT_NULLPTR(pairs[i]);
			}
			delete [] pairs;
		}
	}
}


Config::Config(String^ data)
{
	char* dataStr = nullptr;

	try
	{
		dataStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(data).ToPointer());

		m_impl  = new gmsec::api::Config(dataStr);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config constructor is unable to process data string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(dataStr);
	}
}


Config::Config(Config^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::CONFIG_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	gmsec::api::Config* native = ((Config^) other)->GetUnmanagedImplementation();

	m_impl  = new gmsec::api::Config(*native);
	m_owned = true;
}


Config::~Config()
{
	this->!Config();
}


Config^ Config::operator=(Config^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::CONFIG_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	ResetUnmanagedImplementation();

	gmsec::api::Config* native = ((Config^) other)->GetUnmanagedImplementation();

	m_impl  = new gmsec::api::Config(*native);
	m_owned = true;

	return this;
}


void Config::AddValue(String^ name, String^ value)
{
	char* nameStr  = nullptr;
	char* valueStr = nullptr;

	try
	{
		nameStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());
		valueStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(value).ToPointer());

		m_impl->addValue(nameStr, valueStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::addValue is unable to process name and/or value string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}


bool Config::ClearValue(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->clearValue(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::clearValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


String^ Config::GetValue(String^ name)
{
	return GetValue(name, nullptr);
}


String^ Config::GetValue(String^ name, String^ defaultValue)
{
	char* nameStr         = nullptr;
	char* defaultValueStr = nullptr;

	try
	{
		nameStr         = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());
		defaultValueStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(defaultValue).ToPointer());

		const char* value = m_impl->getValue(nameStr, defaultValueStr);

		if (value != nullptr)
		{
			return gcnew String(value);
		}

		return nullptr;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch(...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(defaultValueStr);
	}
}


bool Config::GetBooleanValue(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->getBooleanValue(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getBooleanValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


bool Config::GetBooleanValue(String^ name, bool defaultValue)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->getBooleanValue(nameStr, defaultValue);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getBooleanValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Int32 Config::GetIntegerValue(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->getIntegerValue(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getIntegerValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Int32 Config::GetIntegerValue(String^ name, Int32 defaultValue)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->getIntegerValue(nameStr, defaultValue);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getIntegerValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Double Config::GetDoubleValue(String^ name)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->getDoubleValue(nameStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getDoubleValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Double Config::GetDoubleValue(String^ name, Double defaultValue)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		return m_impl->getDoubleValue(nameStr, defaultValue);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getDoubleValue is unable to process name string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


void Config::Clear()
{
	return m_impl->clear();
}


bool Config::GetFirst([Out] String^ %name, [Out] String^ %value)
{
	try
	{
		bool result = false;

		const char* nameStr  = nullptr;
		const char* valueStr = nullptr;

		result = m_impl->getFirst(nameStr, valueStr);

		name  = gcnew String(nameStr);
		value = gcnew String(valueStr);

		return result;
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getFirst is unable to create name/value strings");
	}
}


bool Config::GetNext([Out] String^ %name, [Out] String^ %value)
{
	try
	{
		bool result = false;

		const char* nameStr  = nullptr;
		const char* valueStr = nullptr;

		result = m_impl->getNext(nameStr, valueStr);

		name  = gcnew String(nameStr);
		value = gcnew String(valueStr);

		return result;
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::getNext is unable to create name/value strings");
	}
}


void Config::Merge(Config^ other, bool overwriteExisting)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::CONFIGFILE_ERROR, StatusCode::INVALID_CONFIG, "Other Config is null");

	gmsec::api::Config* nativeOther = ((Config^) other)->GetUnmanagedImplementation();

	m_impl->merge(*nativeOther, overwriteExisting);
}


String^ Config::ToXML()
{
	try
	{
		const char* xmlStr = m_impl->toXML();

		return gcnew String(xmlStr);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::toXML is unable to create XML string");
	}
}


void Config::FromXML(String^ xml)
{
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
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::fromXML is unable to process XML string");
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}


String^ Config::ToJSON()
{
	try
	{
		const char* jsonStr = m_impl->toJSON();

		return gcnew String(jsonStr);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::CONFIG_ERROR, StatusCode::OUT_OF_MEMORY, "Config::toXML is unable to create JSON string");
	}
}


Config::!Config()
{
	ResetUnmanagedImplementation();
}


Config::Config(gmsec::api::Config* config, bool owned)
{
	SetUnmanagedImplementation(config, owned);
}


gmsec::api::Config* Config::GetUnmanagedImplementation()
{
	return m_impl;
}


void Config::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void Config::SetUnmanagedImplementation(gmsec::api::Config* config, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = config;
	m_owned = owned;
}
