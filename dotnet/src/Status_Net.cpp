
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

#include "status_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::Status::!Status()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::Status::Status(gmsec::Status * status)
{
	mImpl = new gmsec::Status(*status);
}

Nasa::Gmsec::Net::Status::Status()
{
	mImpl = nullptr;

	try {
		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Status();
	}
	catch(Exception^)
	{
		// clean up what's there for mImpl, and set to null if error during creation
		ResetUnmanImpl();
	}
	catch(...)
	{
		// clean up what's there for mImpl, and set to null if error during creation
		ResetUnmanImpl();
	}

}

Nasa::Gmsec::Net::Status::Status(GMSECErrorClasses eclass, GMSECErrorCodes code,
					   const String ^ text)
{
	char *textStr = nullptr;
	try
	{
		// Safety checks
		RETURN_IF_NULLPTR_CHECK(text);

		// Convert input parameter from Managed to Unmanaged
		textStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)text).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Status((UInt16)eclass, (UInt32)code, textStr);
	}
	catch(Exception^)
	{
		// clean up what's there for mImpl, and set to null if error during creation
		ResetUnmanImpl();
	}
	catch(...)
	{
		// clean up what's there for mImpl, and set to null if error during creation
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(textStr);
	}
}

Nasa::Gmsec::Net::Status::Status(Nasa::Gmsec::Net::Status ^ status)
{
	mImpl = nullptr;

	try
	{
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Status(*status_impl);
	}
	catch(Exception^)
	{
		// if created, delete since was error, then set mImpl to null
		ResetUnmanImpl();
	}
	catch(...)
	{
		// if created, delete since was error, then set mImpl to null
		ResetUnmanImpl();
	}
}

Nasa::Gmsec::Net::Status::~Status()
{
	// Call the finalizer.
	this->!Status();
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Status::operator=(Nasa::Gmsec::Net::Status ^ status)
{
	mImpl = nullptr;

	try
	{
		// avoid != operator, but check to make sure does exist
		if ((dynamic_cast<Object^>(status) != nullptr))
		{
			// Get native pointers
			gmsec::Status* status_impl = (status)->GetUnmanImpl();

			// check if native pointer is null
			if (status_impl != nullptr)
			{
				// Call the unmanaged method.
				this->mImpl = new gmsec::Status(*status_impl);
			}
		}
	}
	catch(Exception^)
	{
		// if created, delete since was error, then set mImpl to null
		ResetUnmanImpl();
		return nullptr;
	}
	catch(...)
	{
		// if created, delete since was error, then set mImpl to null
		ResetUnmanImpl();
		return nullptr;
	}

	return(this);
}

const String ^ Nasa::Gmsec::Net::Status::GetString()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Call the unmanaged method.
		String ^ textStr = gcnew String(mImpl->GetString());

		return(textStr);
	}
	ENDMANAGEDCATCHRETURNVALUE(nullptr)
}

const String ^ Nasa::Gmsec::Net::Status::Get()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, nullptr);

		// Call the unmanaged method.
		String ^ textStr = gcnew String(mImpl->Get());
		return textStr;
	}
	ENDMANAGEDCATCHRETURNVALUE(nullptr)

}

GMSECErrorClasses Nasa::Gmsec::Net::Status::GetClass()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, GMSECErrorClasses::STATUS_OTHER_ERROR);

		// Call the unmanaged method.
		UInt16 errorClass = mImpl->GetClass();

		// If Enum has definition, else error
		if (Enum::IsDefined(GMSECErrorClasses::typeid, errorClass))
		{
			return((GMSECErrorClasses)errorClass);
		}
		else
		{
			return GMSECErrorClasses::STATUS_OTHER_ERROR;
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(GMSECErrorClasses::STATUS_OTHER_ERROR)
}

GMSECErrorCodes Nasa::Gmsec::Net::Status::GetCode()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, GMSECErrorCodes::OTHER_ERROR);

		// Call the unmanaged method.
		UInt32 errorCode = mImpl->GetCode();

		// If Enum has definition, else error
		if (Enum::IsDefined(GMSECErrorCodes::typeid, errorCode))
		{
			return((GMSECErrorCodes)errorCode);
		}
		else
		{
			return GMSECErrorCodes::OTHER_ERROR;
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(GMSECErrorCodes::OTHER_ERROR)
}

const Int32 Nasa::Gmsec::Net::Status::GetCustomCode()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, 0);

		// Call the unmanaged method.
		return(mImpl->GetCustomCode());
	}
	ENDMANAGEDCATCHRETURNVALUE(0)
}

bool Nasa::Gmsec::Net::Status::IsError()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, true);

		// Call the unmanaged method.
		return(Convert::ToBoolean(mImpl->isError()));
	} // Catch any exceptions thrown and return is error in that case
	ENDMANAGEDCATCHRETURNVALUE(true);
}

void Nasa::Gmsec::Net::Status::SetString(const String ^ text)
{
	char * textStr = nullptr;
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Convert input parameter from Managed to Unmanaged
		textStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)text).ToPointer());

		// Call the unmanaged method.
		mImpl->SetString(textStr);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(textStr);
	}
}

void Nasa::Gmsec::Net::Status::SetClass(GMSECErrorClasses eclass)
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Call the unmanaged method.
		mImpl->SetClass((UInt16)eclass);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
}

void Nasa::Gmsec::Net::Status::SetCode(GMSECErrorCodes code)
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Call the unmanaged method.
		mImpl->SetCode((UInt32)code);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
}

void Nasa::Gmsec::Net::Status::SetCustomCode(Int32 code)
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Call the unmanaged method.
		mImpl->SetCustomCode(code);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
}

void Nasa::Gmsec::Net::Status::Set(GMSECErrorClasses eclass, GMSECErrorCodes code, 
					 const String ^ text)
{
	char * textStr = nullptr;

	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Convert input parameter from Managed to Unmanaged
		textStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)text).ToPointer());

		// Call the unmanaged method.
		mImpl->Set((UInt16)eclass, (UInt32)code, textStr);
	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(textStr);
	}
}

void Nasa::Gmsec::Net::Status::ReSet()
{
	STARTMANAGEDCATCH
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Call the unmanaged method.
		mImpl->ReSet();

	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
}

gmsec::Status * Nasa::Gmsec::Net::Status::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::Status::ResetUnmanImpl()
{
	// free unmanaged object
	if (mImpl != nullptr)
	{
		delete(mImpl);
		mImpl = nullptr;
	}
}

void Nasa::Gmsec::Net::Status::SetUnmanImpl(gmsec::Status* status)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = status;
}
