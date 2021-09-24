
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

#include "ConfigFile_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::ConfigFile::!ConfigFile()
{
	// Free the unmanaged implementation pointer.
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}


Nasa::Gmsec::Net::ConfigFile::ConfigFile(const String ^ filepath)
{
	char *filepathStr = nullptr;
	try
	{
		if (filepath == nullptr)
		{
			GenerateDefaultConfigFile();
			return;
		}
		// Convert input parameter from Managed to Unmanaged
		filepathStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)filepath).ToPointer());

		// Call the unmanaged method.
		mImpl = new gmsec::ConfigFile(filepathStr);
	}
	catch (Exception^)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfigFile();
	}
	catch (...)
	{
		// cleanup and do config with nullptr
		GenerateDefaultConfigFile();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(filepathStr);
	}
}

Nasa::Gmsec::Net::ConfigFile::~ConfigFile()
{
	// Call the finalizer.
	this->!ConfigFile();
}

Nasa::Gmsec::Net::Status ^  Nasa::Gmsec::Net::ConfigFile::Load()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Load();

		return status;

	} // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)

} 

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::FromXML(const String ^ xml)
{
	char *xmlStr = nullptr;

	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::INVALID_CONFIG, xml, "xml is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		xmlStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)xml).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->FromXML(xmlStr);

		return status;

	} // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
} 

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::ToXML([Runtime::InteropServices::Out]
		                                 const String ^ %xml)
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert the xml.
		char *xml_str = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->ToXML((const char *&)(xml_str));

		// Convert the xml back.
		xml = gcnew String(xml_str);

		return status;

	} // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
} 

bool Nasa::Gmsec::Net::ConfigFile::IsLoaded()
{
	// Safety check
	RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, false);

	// Call the unmanaged method.
	return mImpl->IsLoaded();
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::LookupConfig(const String ^ name, 
										 [Runtime::InteropServices::Out] Nasa::Gmsec::Net::Config ^% config)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::INVALID_CONFIG, name, "name is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		if (config == nullptr)
		{
			config = gcnew Nasa::Gmsec::Net::Config();
		}
		else
		{
			 config->SetUnmanImpl(new gmsec::Config());
		}

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Config* config_impl = config->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());
		
		// Call the unmanaged method
		*status_impl = mImpl->LookupConfig(nameStr, config_impl);

		return status;
	
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{		
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
} 

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::LookupMessage(const String ^ name, 
										  Nasa::Gmsec::Net::Message ^ msg)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::INVALID_CONFIG, name, "name is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::INVALID_CONFIG, msg, "msg is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* msg_impl = msg->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Call the unmanaged method
		*status_impl = mImpl->LookupMessage(nameStr, msg_impl);

		return status;
	
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{		
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::LookupSubscription(const String ^ name, 
											   [Runtime::InteropServices::Out]
													const String ^ %pattern)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::INVALID_CONFIG, name, "name is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Intialize output parameter
		char *pattern_str = nullptr;

		// Call the unmanaged method
		*status_impl = mImpl->LookupSubscription(nameStr, (const char *&)(pattern_str));

		//  if there was no Error, populate Managed const String ^ %pattern
		if (!status->IsError())
		{
			pattern = gcnew String(pattern_str);
		}

		return status;
	
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::GetSubscriptionNames([Runtime::InteropServices::Out] List<String^>^% names)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		
		// initialize out parameter
		if (names == nullptr)
		{
			names = gcnew List<String^>();
		}
		else
		{
			names->Clear();
		}

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

	    // Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Create Unmanaged list for local use
		gmsec::util::List<std::string> namesNative;

		// Call the unmanaged method.
		*status_impl = mImpl->GetSubscriptionNames(namesNative);

		//  if there was no Error, populate Managed List<String^>^% names
		if (!status->IsError())
		{
			for(Int32 i = 0; i < namesNative.size(); i++)
			{
				std::string nativeStr;
				namesNative.get(i, nativeStr);
				names->Add(gcnew String(nativeStr.c_str()));
			}
		}

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::GetSubscriptions ([Runtime::InteropServices::Out] List<String^>^% patterns)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		
		// initialize out parameter
		if (patterns == nullptr)
		{
			patterns = gcnew List<String^>();
		}
		else
		{
			patterns->Clear();
		}

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		gmsec::util::List<std::string> patternsNative;

		// Call the unmanaged method.
		*status_impl = mImpl->GetSubscriptions(patternsNative);

		//  if there was no Error, populate Managed List<String^>^% names
		if (!status->IsError())
		{
			for(Int32 i = 0; i < patternsNative.size(); i++)
			{
				std::string nativeStr;
				patternsNative.get(i, nativeStr);
				patterns->Add(gcnew String(nativeStr.c_str()));
			}
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)

}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::GetConfigNames (List<String^>^% names)
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		
		// initialize out parameter
		names = gcnew List<String^>();

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Create Unmanaged list for local use
		gmsec::util::List<std::string> namesNative;

		// Call the unmanaged method.
		*status_impl = mImpl->GetConfigNames(namesNative);

		//  if there was no Error, populate Managed List<String^>^% names
		if (!status->IsError())
		{
			for(Int32 i = 0; i < namesNative.size(); i++)
			{
				std::string nativeStr;
				namesNative.get(i, nativeStr);
				names->Add(gcnew String(nativeStr.c_str()));
			}
		}

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::GetConfigs ([Runtime::InteropServices::Out] List<Config^>^% configList)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		
		// initialize out parameter
		if (configList == nullptr)
		{
			configList = gcnew List<Config^>();
		}
		else
		{
			configList->Clear();
		}

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		gmsec::util::List<gmsec::Config *> configListNative;

		// Call the unmanaged method.
		*status_impl = mImpl->GetConfigs(configListNative);

		//  if there was no Error, populate Managed List<gmsec::Config *> configListNative
		if (!status->IsError())
		{
			for(Int32 i = 0; i < configListNative.size(); i++)
			{
				gmsec::Config * nativeConfig;
				configListNative.get(i, nativeConfig);
				configList->Add(gcnew Nasa::Gmsec::Net::Config(nativeConfig));
			}
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::GetMessageNames ([Runtime::InteropServices::Out] List<String^>^% names)
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
	
		// initialize out parameter
		if (names == nullptr)
		{
			names = gcnew List<String^>();
		}
		else
		{
			names->Clear();
		}

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Create Unmanaged list for local use
		gmsec::util::List<std::string> namesNative;

		// Call the unmanaged method.
		*status_impl = mImpl->GetMessageNames(namesNative);

		//  if there was no Error, populate Managed List<String^>^% names
		if (!status->IsError())
		{
			for(Int32 i = 0; i < namesNative.size(); i++)
			{
				std::string nativeStr;
				namesNative.get(i, nativeStr);
				names->Add(gcnew String(nativeStr.c_str()));
			}
		}

		return status;

	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConfigFile::GetMessages (Nasa::Gmsec::Net::Connection^ connection, [Runtime::InteropServices::Out] List<Nasa::Gmsec::Net::Message^>^% message)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::INVALID_CONFIG, connection, "connection is null");

		// initialize out parameter
		if (message == nullptr)
		{
			message = gcnew List<Nasa::Gmsec::Net::Message^>();
		}
		else
		{
			message->Clear();
		}

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		gmsec::util::List<gmsec::Message *> messageListNative;

		// Call the unmanaged method.
		*status_impl = mImpl->GetMessages(connection->GetUnmanImpl(), messageListNative);

		//  if there was no Error, populate Managed List<Message^>^% message
		if (!status->IsError())
		{
			for(Int32 i = 0; i < messageListNative.size(); i++)
			{
				gmsec::Message * nativeMessage;
				messageListNative.get(i, nativeMessage);
				message->Add(gcnew Nasa::Gmsec::Net::Message(nativeMessage));
			}
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONFIGFILE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

void Nasa::Gmsec::Net::ConfigFile::GenerateDefaultConfigFile()
{
	ResetUnmanImpl();

	// Set ConfigFile with null passed in
	mImpl = new gmsec::ConfigFile(nullptr);
}

gmsec::ConfigFile * Nasa::Gmsec::Net::ConfigFile::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::ConfigFile::ResetUnmanImpl()
{
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}
