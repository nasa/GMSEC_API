
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    

#include "Config_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::Config::!Config()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::Config::Config()
{
	// Initialize the unmanaged implementation pointer and object.
	mImpl = new gmsec::Config(); 
}

Nasa::Gmsec::Net::Config::Config(gmsec::Config* config)
{
	// Initialize the unmanaged implementation pointer from existing one
	mImpl = new gmsec::Config(*config);
}

void Nasa::Gmsec::Net::Config::GenerateDefaultConfig()
{
	// Just in case one was created
	ResetUnmanImpl();

	STARTMANAGEDCATCH
	{
		// Setup a ConfigFile with a null which 
		// lets the GMSEC API handle the error.
		mImpl = new gmsec::Config(nullptr);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
}


Nasa::Gmsec::Net::Config::Config(array<String ^> ^ argv)
{
	char ** strings = nullptr;
	Int32 argc = 0;

	try
	{
		// if none specified, error, do default
		if (argv == nullptr)
		{
			GenerateDefaultConfig();
			return;
		}

		// set argc, no need to pass in
		argc = argv->Length;	

		// define array of char* to pass to unmanaged
		strings = new char*[argc];

		for (Int32 i = 0; i < argc; i++)
		{   // Convert input parameters from Managed to Unmanaged
			if (argv[i] != nullptr)
			{
				strings[i] = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(argv[i]).ToPointer());
			}
			else
			{
				strings[i] = nullptr;
			}
		}

		// Call the unmanaged method.
		mImpl = new gmsec::Config(argc, strings);
	}
	catch (Exception^)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfig();
	}
	catch (...)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfig();
	}
	finally
	{
		if (strings != nullptr)
		{
			for (Int32 i = 0; i < argc; i++) 
			{
				// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
				FREE_HGLOBAL_IF_NOT_NULLPTR(strings[i]);
			}
			delete strings;
		}
	}
}

Nasa::Gmsec::Net::Config::Config(const String ^ xml)
{
	char *xmlStr = nullptr;

	try
	{   
		// if xml not specified, error, do default
		if (xml == nullptr)
		{
			GenerateDefaultConfig();
			return;
		}
		
		// Convert input parameter from Managed to Unmanaged
		xmlStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) xml).ToPointer());

		// Call the unmanaged method.
		mImpl = new gmsec::Config(xmlStr);
	}
	catch (Exception^)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfig();
	}
	catch (...)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfig();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}

Nasa::Gmsec::Net::Config::Config(const Nasa::Gmsec::Net::Config ^ config)
{
	try
	{
		// if config not specified, error, do default
		if (config == nullptr)
		{
			GenerateDefaultConfig();
			return;
		}
		
		// Make new object by grabbing unmanaged Implementation.
		gmsec::Config* config_impl = (const_cast<Nasa::Gmsec::Net::Config ^>(config))->GetUnmanImpl();
		mImpl = new gmsec::Config(*config_impl);
	}
	catch (Exception^)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfig();
	}
	catch (...)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfig();
	}
}

Nasa::Gmsec::Net::Config::~Config()
{
	// Call the finalizer.
	this->!Config();
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::AddValue( 
								 const String ^ name, const String ^ value)
{
	char *nameStr = nullptr;
	char *valueStr = nullptr;
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::INVALID_CONFIG, name, "name is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::INVALID_CONFIG, value, "value is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		String ^ tmpName = (String ^)name;
		String ^ tmpValue = (String ^)value;
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tmpName).ToPointer());
		valueStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tmpValue).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->AddValue(nameStr, valueStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}

}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::ClearValue( 
								   const String ^ name)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::INVALID_CONFIG, name, "name is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		String ^ tmpName = (String ^)name;

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tmpName).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->ClearValue(nameStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::GetValue(const String ^ name, 
								 [Runtime::InteropServices::Out] 
									const String ^ %value)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::INVALID_CONFIG, name, "name is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		String ^ tmpName = (String ^)name;

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tmpName).ToPointer());

		char *valueStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue(nameStr, (const char *&)valueStr);

		value = gcnew String(valueStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Free input parameter
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

bool Nasa::Gmsec::Net::Config::CheckBoolValue(const String ^ name, bool defaultValue)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, defaultValue);
		RETURN_VALUE_IF_NULLPTR_CHECK(name, defaultValue);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Call the unmanaged method.
		bool retCode = mImpl->CheckBoolValue((const char *)nameStr, defaultValue);

		return(retCode);
	} // There is nothing else to do here but return defaultValue
	ENDMANAGEDCATCHRETURNVALUE(defaultValue)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::Clear()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Clear();

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::GetFirst(
						  [Runtime::InteropServices::Out]const String ^ %name,
					      [Runtime::InteropServices::Out]const String ^ %value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *nameStr = nullptr;
		char *valueStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetFirst((const char *&)(nameStr), (const char *&)(valueStr));

		name = gcnew String(nameStr);
		value = gcnew String(valueStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::GetNext(
						 [Runtime::InteropServices::Out] const String ^ %name,
						 [Runtime::InteropServices::Out] const String ^ %value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *nameStr = nullptr;
		char *valueStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetNext((const char *&)(nameStr), (const char *&)(valueStr));

		name = gcnew String(nameStr);
		value = gcnew String(valueStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::ToXML(
					   [Runtime::InteropServices::Out] const String ^ %xml)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *xmlStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->ToXML((const char *&)(xmlStr));

		xml = gcnew String(xmlStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Config::FromXML(const String ^ xml)
{
	char *xmlStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::INVALID_CONFIG, xml, "xml is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		xmlStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)xml).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->FromXML(xmlStr);

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIG_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}

gmsec::Config * Nasa::Gmsec::Net::Config::GetUnmanImpl()
{
	return(mImpl);
}

void Nasa::Gmsec::Net::Config::ResetUnmanImpl()
{
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}

void Nasa::Gmsec::Net::Config::SetUnmanImpl(gmsec::Config* config)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = config;
}
