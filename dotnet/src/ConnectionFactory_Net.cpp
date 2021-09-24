
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

#include "ConnectionFactory_Net.h"

using namespace Nasa::Gmsec::Net;

const String ^ Nasa::Gmsec::Net::ConnectionFactory::GetAPIVersion()
{
	// Call the unmanaged method.
	return(gcnew String(gmsec::ConnectionFactory::GetAPIVersion()));
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConnectionFactory::Create( 
								   Nasa::Gmsec::Net::Config ^ config, 
								   [Runtime::InteropServices::Out]
								   Nasa::Gmsec::Net::Connection ^ %conn)
{
	STARTMANAGEDCATCH 
	{
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FACTORY_ERROR, GMSECErrorCodes::INVALID_CONFIG, config, "config is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FACTORY_ERROR, GMSECErrorCodes::INVALID_CONFIG, config->GetUnmanImpl(), "config impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Config* config_impl = config->GetUnmanImpl();

		// Convert the connection.
		gmsec::Connection *connNative = nullptr;

		// Call the unmanaged method.
		*status_impl = gmsec::ConnectionFactory::Create(config_impl, connNative);

		// Convert the connection back.
		if (conn == nullptr)
		{
			conn = gcnew Nasa::Gmsec::Net::Connection(connNative);
		}
		else
		{
			conn->SetUnmanImpl(connNative);
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FACTORY_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::ConnectionFactory::Destroy( 
									Nasa::Gmsec::Net::Connection ^ conn)
{
	STARTMANAGEDCATCH 
	{
		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FACTORY_ERROR, GMSECErrorCodes::INVALID_CONNECTION, conn, "conn is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FACTORY_ERROR, GMSECErrorCodes::INVALID_CONNECTION, conn->GetUnmanImpl(), "conn impl is null");

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = gmsec::ConnectionFactory::Destroy(conn->GetUnmanImpl());

		// Clean-up.
		conn->ResetUnmanImpl();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FACTORY_ERROR, GMSECErrorCodes::OTHER_ERROR)
}
