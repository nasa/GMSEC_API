
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

#include "Connection_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::Connection::!Connection()
{
	// Free the unmanaged implementation pointer, messages are created and destroyed instead by the connection factory
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::Connection::Connection()
{
	// Default constructor which assigns null to impl
	mImpl = nullptr;
}

Nasa::Gmsec::Net::Connection::~Connection()
{
	// Call the finalizer.
	this->!Connection();
}

Nasa::Gmsec::Net::Connection::Connection(gmsec::Connection * conn)
{
	mImpl = conn;
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Connect()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Connect();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Disconnect()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Disconnect();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

bool Nasa::Gmsec::Net::Connection::IsConnected()
{ 
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, false);

		// Call the unmanaged method.
		return(mImpl->IsConnected());
	}
	ENDMANAGEDCATCHRETURNVALUE(false);
}

String ^ Nasa::Gmsec::Net::Connection::GetLibraryRootName()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Call the unmanaged method.
		return(gcnew String(mImpl->GetLibraryRootName()));
	}
	ENDMANAGEDCATCHRETURNVALUE(nullptr)
}

String ^ Nasa::Gmsec::Net::Connection::GetLibraryVersion()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Call the unmanaged method.
		return(gcnew String(mImpl->GetLibraryVersion()));
	}
	ENDMANAGEDCATCHRETURNVALUE(nullptr)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::RegisterErrorCallback(const String ^ eventStr, Nasa::Gmsec::Net::ErrorCallback ^ ecb)
{
	char *eventStrStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CONFIG, eventStr, "eventStr is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, ecb, "ecb is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, ecb->GetUnmanImpl(), "ecb impl is null");
		
		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::ErrorCallback* ecb_impl = ecb->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		eventStrStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) eventStr).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->RegisterErrorCallback(eventStrStr, ecb_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(eventStrStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Subscribe(const String ^ subject)
{
	char *subjectStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");
		
		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) subject).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->Subscribe(subjectStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Subscribe( 
							   const String ^ subject,
							   Nasa::Gmsec::Net::Callback ^ cb)
 {
	char *subjectStr = nullptr;

 	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, cb, "cb is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, cb->GetUnmanImpl(), "cb impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Callback* cb_impl = cb->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) subject).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->Subscribe(subjectStr, cb_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::UnSubscribe(const String ^ subject)
{
	char *subjectStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) subject).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->UnSubscribe(subjectStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

Nasa::Gmsec::Net::Status ^  Nasa::Gmsec::Net::Connection::UnSubscribe( 
								 const String ^ subject, 
								 Nasa::Gmsec::Net::Callback ^ cb)
{
	char *subjectStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, cb, "cb is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, cb->GetUnmanImpl(), "cb impl is null");
		
		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Callback* cb_impl = cb->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) subject).ToPointer());

		// Call the unmanaged method.
		*status_impl = mImpl->UnSubscribe(subjectStr, cb_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::CreateMessage( 
								   [Runtime::InteropServices::Out] 
										Nasa::Gmsec::Net::Message ^ %msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		
		gmsec::Message *msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->CreateMessage(msgNative);

		if (msg == nullptr)
		{
			msg = gcnew Nasa::Gmsec::Net::Message(msgNative);
		}
		else
		{
			msg->SetUnmanImpl(msgNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

 Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::CreateMessage( 
									const String ^ subject, 
									GMSECMsgKindDefs msgKind,
									[Runtime::InteropServices::Out] 
										Nasa::Gmsec::Net::Message ^ %msg)
 {
	char *subjectStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
	
		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^) subject).ToPointer());

		gmsec::Message *msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->CreateMessage(subjectStr, (UInt16)msgKind, msgNative);

		if (msg == nullptr)
		{
			msg = gcnew Nasa::Gmsec::Net::Message(msgNative);
		}
		else
		{
			msg->SetUnmanImpl(msgNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

 Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::CreateMessage( 
									const String ^ subject,	
									GMSECMsgKindDefs msgKind, 
									[Runtime::InteropServices::Out] 
									Nasa::Gmsec::Net::Message ^ %msg, 
									Nasa::Gmsec::Net::Config ^ config)
{
	char *subjectStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_SUBJECT_NAME, subject, "subject is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CONFIG, config, "config is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CONFIG, config->GetUnmanImpl(), "config impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Config* config_impl = config->GetUnmanImpl();

		// Convert input parameter from Managed to Unmanaged
		subjectStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^)subject).ToPointer());

		gmsec::Message *msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->CreateMessage(subjectStr, (UInt16)msgKind, msgNative, config_impl);

		if (msg == nullptr)
		{
			msg = gcnew Nasa::Gmsec::Net::Message(msgNative);
		}
		else
		{
			msg->SetUnmanImpl(msgNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::ConvertMessage(
									Nasa::Gmsec::Net::Message ^ in_msg,
									[Runtime::InteropServices::Out] 
									Nasa::Gmsec::Net::Message ^ %out_msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, in_msg, "in_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, in_msg->GetUnmanImpl(), "in_msg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* in_msg_impl = in_msg->GetUnmanImpl();

		gmsec::Message *out_msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->ConvertMessage(in_msg_impl, 
			out_msgNative);

		if (out_msg == nullptr)
		{
			out_msg = gcnew Nasa::Gmsec::Net::Message(out_msgNative);
		}
		else
		{
			out_msg->SetUnmanImpl(out_msgNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::ConvertCleanup(
									Nasa::Gmsec::Net::Message ^ in_msg, 
									Nasa::Gmsec::Net::Message ^ out_msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, in_msg, "in_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, in_msg->GetUnmanImpl(), "in_msg impl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, out_msg, "out_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, out_msg->GetUnmanImpl(), "out_msg impl is null");
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* in_msg_impl = in_msg->GetUnmanImpl();
		gmsec::Message* out_msg_impl = out_msg->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->ConvertCleanup(in_msg_impl, 
			out_msg_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::CloneMessage( 
								  Nasa::Gmsec::Net::Message ^ in_msg, 
 								  [Runtime::InteropServices::Out] 
								  Nasa::Gmsec::Net::Message ^ %out_msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, in_msg, "in_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, in_msg->GetUnmanImpl(), "in_msg impl is null");

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* in_msg_impl = in_msg->GetUnmanImpl();

		gmsec::Message *out_msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->CloneMessage(in_msg_impl, out_msgNative);

		if (out_msg == nullptr)
		{
			out_msg = gcnew Nasa::Gmsec::Net::Message(out_msgNative);
		}
		else
		{
			out_msg->SetUnmanImpl(out_msgNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::DestroyMessage(
									Nasa::Gmsec::Net::Message ^ msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, msg, "msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, msg->GetUnmanImpl(), "msg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* msgImplPin = msg->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->DestroyMessage(msgImplPin);

		// Clean-up.
		msg->ResetUnmanImpl();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Publish( 
									Nasa::Gmsec::Net::Message ^ msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, msg, "msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, msg->GetUnmanImpl(), "msg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* msgImplPin = msg->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Publish(msgImplPin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Request( 
							 Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
							 Nasa::Gmsec::Net::ReplyCallback ^ rcb)
{
	return Request(request_msg, timeout, rcb, 0);
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Request(
							 Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
							 [Runtime::InteropServices::Out] 
								Nasa::Gmsec::Net::Message ^ %reply_msg)
{
	return Request(request_msg, timeout, reply_msg, 0);
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Request( 
							 Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
							 Nasa::Gmsec::Net::ReplyCallback ^ rcb, Int32 republish_ms)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, request_msg, "request_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, request_msg->GetUnmanImpl(), "request_msg impl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, rcb, "rcb is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_CALLBACK, rcb->GetUnmanImpl(), "rcb impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* request_msg_impl = request_msg->GetUnmanImpl();
		gmsec::ReplyCallback* rcb_impl = rcb->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Request(request_msg_impl, timeout,
			rcb_impl, republish_ms);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Request(
							 Nasa::Gmsec::Net::Message ^ request_msg, Int32 timeout, 
							 [Runtime::InteropServices::Out] 
								Nasa::Gmsec::Net::Message ^ %reply_msg, Int32 republish_ms)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, request_msg, "request_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, request_msg->GetUnmanImpl(), "request_msg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* request_msg_impl = request_msg->GetUnmanImpl();

		gmsec::Message *reply_msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->Request(request_msg_impl, timeout, 
			reply_msgNative, republish_ms);

		if (reply_msg == nullptr)
		{
			reply_msg = gcnew Nasa::Gmsec::Net::Message(reply_msgNative);
		}
		else
		{
			reply_msg->SetUnmanImpl(reply_msgNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::Reply( 
						   Nasa::Gmsec::Net::Message ^ request_msg,
						   Nasa::Gmsec::Net::Message ^ reply_msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, request_msg, "request_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, request_msg->GetUnmanImpl(), "request_msg impl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, reply_msg, "reply_msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, reply_msg->GetUnmanImpl(), "reply_msg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Reply(request_msg->GetUnmanImpl(), 
			reply_msg->GetUnmanImpl());

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::StartAutoDispatch()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->StartAutoDispatch();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::StopAutoDispatch()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->StopAutoDispatch();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::StopAutoDispatch(bool waitForComplete)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->StopAutoDispatch();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::GetNextMsg(
								[Runtime::InteropServices::Out]
								Nasa::Gmsec::Net::Message ^ %msg, Int32 timeout)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		gmsec::Message *msgNative = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetNextMsg(msgNative, timeout);

		if (msgNative != nullptr)
		{
			if (msg == nullptr)
			{
				msg = gcnew Nasa::Gmsec::Net::Message(msgNative);
			}
			else
			{
				msg->SetUnmanImpl(msgNative);
			}
		}
		else
		{
			if (msg != nullptr)
			{
				// free unmanaged
				msg->ResetUnmanImpl();

				// return null
				msg = nullptr;
			}
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::DispatchMsg( 
								 Nasa::Gmsec::Net::Message ^ msg)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, msg, "msg is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::INVALID_MESSAGE, msg->GetUnmanImpl(), "msg impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Message* msgImplPin = msg->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->DispatchMsg(msgImplPin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Connection::GetLastDispatcherStatus()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->GetLastDispatcherStatus();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_CONNECTION_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

const String ^ Nasa::Gmsec::Net::Connection::GetName()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Call the unmanaged method.
		return(gcnew String(mImpl->GetName()));
	}
	ENDMANAGEDCATCHRETURNVALUE(nullptr)
}

void Nasa::Gmsec::Net::Connection::SetName(const String ^ name)
{
	char *nameStr;
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((String ^)name).ToPointer());

		// Call the unmanaged method.
		mImpl->SetName(nameStr);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

String ^ Nasa::Gmsec::Net::Connection::GetMWINFO()
{
	char *infoBufferStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Convert input parameter from Managed to Unmanaged
		infoBufferStr = new char[gmsec::Connection::MWINFO_BUFFER];

		// Call the unmanaged method.
		String ^ retString = gcnew String(mImpl->GetMWINFO(infoBufferStr));
	
		return(retString);
	}
	ENDMANAGEDCATCHRETURNVALUE(nullptr)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		if (infoBufferStr != nullptr)
			delete [] infoBufferStr;
	}
}

gmsec::Connection * Nasa::Gmsec::Net::Connection::GetUnmanImpl()
{
	return(mImpl);
}

void Nasa::Gmsec::Net::Connection::ResetUnmanImpl()
{
	// Reset impl pointer to null
	// Is deleted by ConnectionFactory, not here
	mImpl = nullptr;
}

void Nasa::Gmsec::Net::Connection::SetUnmanImpl(gmsec::Connection* connection)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = connection;
}
