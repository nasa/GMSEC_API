
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

#include "Message_Net.h"
#include "Connection_Net.h"
#include "ConfigFile_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::Message::!Message()
{
	// Free the unmanaged implementation pointer, messages are created and destroyed instead by the connection factory
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::Message::Message()
{
	// Initialize the unmanaged implementation pointer.
	mImpl = nullptr;
}

Nasa::Gmsec::Net::Message::Message(gmsec::Message * msg)
{
	// Initialize the unmanaged implementation pointer.
	mImpl = msg;
}

Nasa::Gmsec::Net::Message::~Message()
{
	// Call the finalizer.
	this->!Message();
}

bool Nasa::Gmsec::Net::Message::IsValid()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, false);

		// Call the unmanaged method.
		return(mImpl->isValid());
	}
	ENDMANAGEDCATCHRETURNVALUE(false);
}

/// <seealso cref="Connection::GetLibraryRootName()" />
String ^ Nasa::Gmsec::Net::Message::GetLibraryRootName()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Call the unmanaged method.
		return(gcnew String(mImpl->GetLibraryRootName()));
	} // Catch any exceptions thrown and return null
	ENDMANAGEDCATCHRETURNVALUE(nullptr);
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::SetConfig(Nasa::Gmsec::Net::Config ^ config)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::INVALID_CONFIG, config, "config is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::INVALID_CONFIG, config->GetUnmanImpl(), "config impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Config* config_impl = config->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetConfig(config_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::SetKind(GMSECMsgKindDefs kind)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetKind((UInt16)kind);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetKind([Runtime::InteropServices::Out] GMSECMsgKindDefs %kind)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();

		UInt16 kindRetrieval;
		pin_ptr<GMSEC_MSG_KIND> kindPin = &kindRetrieval;

		// Call the unmanaged method.
		*status_impl = mImpl->GetKind(*kindPin);

		if (Enum::IsDefined(GMSECMsgKindDefs::typeid, kindRetrieval))
		{
			kind = (GMSECMsgKindDefs)kindRetrieval;
		}
		else
		{
			kind = GMSECMsgKindDefs::UNSET;
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::SetSubject(const String ^ subject)
{
	char *subjectStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)subject).ToPointer());

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetSubject((const char *)subjectStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetSubject([Runtime::InteropServices::Out] const String ^ %subject)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *subjectStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetSubject((const char *&)subjectStr);

		subject = gcnew String(subjectStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::ClearFields()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->ClearFields();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::AddField(Nasa::Gmsec::Net::Field ^ field)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::INVALID_FIELD, field, "field is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::INVALID_FIELD, field->GetUnmanImpl(), "field impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Field* field_impl = field->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->AddField(*field_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::ClearField(const String ^ name)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::INVALID_FIELD_NAME, name, "name is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->ClearField(nameStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetField(const String ^ name, [Runtime::InteropServices::Out] Nasa::Gmsec::Net::Field ^% field)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// create out parameter
		if (field == nullptr)
		{
			field = gcnew Field();
		}
		else
		{
			field->SetUnmanImpl(new gmsec::Field());
		}

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Field* field_impl = field->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->GetField(nameStr, *field_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetFieldCount([Runtime::InteropServices::Out] Int32 %count)
{	
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<Int32> countPin = &count;

		// Call the unmanaged method.
		*status_impl = mImpl->GetFieldCount((GMSEC_I32&)*countPin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetFirstField([Runtime::InteropServices::Out] Nasa::Gmsec::Net::Field ^% field)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// create out parameter
		if (field == nullptr)
		{
			field = gcnew Field();
		}
		else
		{
			field->SetUnmanImpl(new gmsec::Field());
		}

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Field* field_impl = field->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->GetFirstField(*field_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetNextField([Runtime::InteropServices::Out] Nasa::Gmsec::Net::Field^% field)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// create out parameter
		if (field == nullptr)
		{
			field = gcnew Field();
		}
		else
		{
			field->SetUnmanImpl(new gmsec::Field());
		}

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Field* field_impl = field->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->GetNextField(*field_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::CopyFields(Nasa::Gmsec::Net::Message^ toMsg)
{
	char *toMsgStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, toMsg, "toMsg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, toMsg->GetUnmanImpl(), "toMsg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Convert input parameter from Managed to Unmanaged
		toMsgStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)toMsg).ToPointer());

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* to_msg_impl = toMsg->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->CopyFields(to_msg_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(toMsgStr);
	}
}

/// <seealso cref="ConfigFile::GetMessages" />
Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::ToXML([Runtime::InteropServices::Out] const String ^ %xml)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert the xml.
		char *xmlStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->ToXML((const char *&)(xmlStr));

		// Convert the xml back.
		xml = gcnew String(xmlStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

/// <seealso cref="ConfigFile::GetMessages" />
Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::FromXML(const String ^ xml)
{
	char *xmlStr = nullptr;
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Convert input parameter from Managed to Unmanaged
		xmlStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)xml).ToPointer());

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
	
		// Call the unmanaged method.
		*status_impl = mImpl->FromXML(xmlStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(xmlStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::GetMSGSize([Runtime::InteropServices::Out] UInt32 %size)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<UInt32> sizePin = &size;

		// Call the unmanaged method.
		*status_impl = mImpl->GetMSGSize((GMSEC_U32&)*sizePin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

GMSECMsgKindDefs Nasa::Gmsec::Net::Message::LookupKind(const String ^ kind)
{
	char *kindStr;
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, GMSECMsgKindDefs::UNSET);
		RETURN_VALUE_IF_NULLPTR_CHECK(kind, GMSECMsgKindDefs::UNSET);

		// Convert input parameter from Managed to Unmanaged
		kindStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)kind).ToPointer());

		// Call the unmanaged method.
		UInt16 kind = mImpl->LookupKind(kindStr);
	
		// If Enum has definition, else error
		if (Enum::IsDefined(GMSECMsgKindDefs::typeid, kind))
		{
			return (GMSECMsgKindDefs)kind;
		}
		else
		{
			return GMSECMsgKindDefs::UNSET;
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(GMSECMsgKindDefs::UNSET)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(kindStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Message::Reset(FieldIterator^ iter)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Reset(*iter->GetUnmanImpl());

		return status;
		
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_MESSAGE_ERROR, GMSECErrorCodes::OTHER_ERROR)
}


gmsec::Message * Nasa::Gmsec::Net::Message::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::Message::ResetUnmanImpl()
{
	// Reset the unmanaged implementation pointer.
	// Is destroyed by connection, not here
	mImpl = nullptr;
}

void Nasa::Gmsec::Net::Message::SetUnmanImpl(gmsec::Message* message)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = message;
}
