
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

#include "Field_Net.h"
#include "Message_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::Field::!Field()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::Field::Field()
{
	// Initialize the unmanaged implementation pointer and object.
	mImpl = new gmsec::Field();
}

Nasa::Gmsec::Net::Field::Field(gmsec::Field* field)
{
	// Initialize the unmanaged implementation pointer and object.
	mImpl = field;
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, SByte value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (GMSEC_CHAR)value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, Boolean value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		GMSEC_BOOL b = (GMSEC_BOOL)value;
		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, b);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, Int16 value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (GMSEC_SHORT)value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, UInt16 value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (GMSEC_U16)value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, Int32 value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (GMSEC_LONG)value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, Int64 value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (GMSEC_LONGLONG)value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, UInt32 value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (GMSEC_ULONG)value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, Single value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, Double value)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, value);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, String ^ value)
{
	char *nameStr = nullptr;
	char *valueStr = nullptr;
	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());
		valueStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)value).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (const char *)valueStr);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, 
						array<Byte>^ value, UInt32 size)
{
	char *nameStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);
		RETURN_IF_NULLPTR_CHECK(value);

		// Size checks
		if ((UInt32)value->Length < size)
			return;

		if (size == 0)
			return;

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());

		// pin byte array
		pin_ptr<unsigned char> pinnedArray = &value[0];

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (const unsigned char *)pinnedArray, size);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Field::Field(const String ^ name, GMSECTypeDefs ftype, 
					 const String ^ value)
{
	char *nameStr = nullptr;
	char *valueStr = nullptr;

	try
	{
		mImpl = nullptr;

		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(name);

		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());
		valueStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)value).ToPointer());

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field((const char *)nameStr, (UInt16)ftype, (const char *)valueStr);
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}

Nasa::Gmsec::Net::Field::Field(Nasa::Gmsec::Net::Field ^ field)
{
	try
	{
		mImpl = nullptr;

		RETURN_IF_NULLPTR_CHECK(field);
		RETURN_IF_NULLPTR_CHECK(field->GetUnmanImpl());

		
		// Get native pointers
		gmsec::Field* field_impl = field->GetUnmanImpl();

		// Initialize the unmanaged implementation pointer and object.
		mImpl = new gmsec::Field(*(const_cast<Nasa::Gmsec::Net::Field ^> (field)->GetUnmanImpl()));
	}
	catch (Exception^)
	{
		ResetUnmanImpl();
	}
	catch (...)
	{		
		ResetUnmanImpl();
	}

}

Nasa::Gmsec::Net::Field::~Field()
{
	// Call the finalizer.
	this->!Field();
}

Nasa::Gmsec::Net::Field ^ Nasa::Gmsec::Net::Field::operator=(Nasa::Gmsec::Net::Field ^ field)
{
	mImpl = nullptr;

	try
	{
		// avoid != operator, but check to make sure does exist
		if ((dynamic_cast<Object^>(field) != nullptr))
		{
			// Get native pointers
			gmsec::Field* field_impl = field->GetUnmanImpl();

			// check if native pointer is null
			if (field_impl != nullptr)
			{
				mImpl = new gmsec::Field(*field_impl);
			}
		}
	}
	catch (Exception^)
	{
		// reset and return null if error
		ResetUnmanImpl();
		return nullptr;
	}
	catch (...)
	{	
		// reset and return null if error
		ResetUnmanImpl();
		return nullptr;
	}

	return(this);
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetName([Runtime::InteropServices::Out] const String ^ %name)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *nameStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetName((const char *&)(nameStr));

		name = gcnew String(nameStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetName(const String ^ name)
{
	char *nameStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
	
		// Convert input parameter from Managed to Unmanaged
		nameStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)name).ToPointer());
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetName(nameStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetType(GMSECTypeDefs ftype)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
	
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetType((UInt16)ftype);
	
		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetType([Runtime::InteropServices::Out] GMSECTypeDefs% ftype)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		UInt16 ftypeRetrieval;
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<UInt16> ftype_pin = &ftypeRetrieval;

		// Call the unmanaged method.
		*status_impl = mImpl->GetType(*ftype_pin);

		// If Enum has definition, else error
		if (Enum::IsDefined(GMSECTypeDefs::typeid, ftypeRetrieval))
		{
			ftype = (GMSECTypeDefs)ftypeRetrieval;
		}
		else
		{
			ftype = GMSECTypeDefs::UNSET;
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

bool Nasa::Gmsec::Net::Field::IsHeader()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, false);

		// Call the unmanaged method.
		return mImpl->isHeader();

	}  // Catch any exceptions thrown
	ENDMANAGEDCATCHRETURNVALUE(false)
}

void Nasa::Gmsec::Net::Field::SetHeader(bool isHeader)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_IF_NULLPTR_CHECK(mImpl);

		// Call the unmanaged method.
		mImpl->setHeader(isHeader);

	}	// Catch any exceptions thrown and return
	ENDMANAGEDCATCHNOOP
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] SByte% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<SByte> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_CHAR &)*type_pin);
	
		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] Boolean% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<bool> type_pin = &value;

		// set initial return and create native enumerated type
		value = false;
		GMSEC_BOOL booleanReturn;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue(booleanReturn);

		// change value to true if true
		if (booleanReturn == GMSEC_TRUE)
		{
			value = true;
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] Int16% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
	
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<GMSEC_SHORT> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_SHORT&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] UInt16% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<GMSEC_U16> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_U16&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] array<Byte>^% value, [Runtime::InteropServices::Out] UInt32% size)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<UInt32> type_pin = &size;

		unsigned char* dataRaw = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue(&dataRaw, (GMSEC_U32&)*type_pin);

		if (dataRaw != nullptr)
		{
			value = gcnew array<Byte>(size);
			System::Runtime::InteropServices::Marshal::Copy(IntPtr(dataRaw), value, 0, size);
		}
		else
		{
			value = nullptr;
		}

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] Int32% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<Int32> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_LONG&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] UInt32% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<UInt32> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_ULONG&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] Int64% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<Int64> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_LONGLONG&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] Single% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<Single> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_FLOAT&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] Double% value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Don't let the gc move our managed objects.
		gmsec::Status* status_impl = status->GetUnmanImpl();
		pin_ptr<Double> type_pin = &value;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue((GMSEC_DOUBLE&)*type_pin);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::GetValue([Runtime::InteropServices::Out] String ^ %value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *valueStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->GetValue(valueStr);

		value = gcnew String(valueStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}



Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(SByte value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((GMSEC_CHAR)value);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(Boolean value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		GMSEC_BOOL booleanEnum;
		if (value == true)
		{
			booleanEnum = GMSEC_TRUE;
		}
		else
		{
			booleanEnum = GMSEC_FALSE;
		}

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue(booleanEnum);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(Int16 value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((GMSEC_SHORT)value);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(UInt16 value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((GMSEC_U16)value);
		
		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(Int32 value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((GMSEC_LONG)value);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(UInt32 value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((GMSEC_ULONG)value);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(Int64 value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((GMSEC_LONGLONG)value);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(Single value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue(value);
		
		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}


Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(array<Byte>^ value, UInt32 size)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::INVALID_FIELD_VALUE, value, "value is null");

		if ((UInt32)value->Length < size)
		{
			Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::INVALID_FIELD_VALUE,  "size of array is less than length");
			return status;
		}

		if (size == 0)
		{
			Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::INVALID_FIELD_VALUE,  "size specified for array is == 0");
			return status;
		}

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// pin byte array
		pin_ptr<unsigned char> pinnedArray = &value[0];

		// is copied here
		// Call the unmanaged method.
		*status_impl = mImpl->SetValue(pinnedArray, size);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(Double value)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue(value);
		
		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}


Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(String ^ value)
{
	char *valueStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::INVALID_FIELD_VALUE, value, "value is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Convert input parameter from Managed to Unmanaged
		valueStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)value).ToPointer());
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue(valueStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}


Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::SetValue(GMSECTypeDefs ftype, const String ^ value)
{
	char *valueStr = nullptr;

	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::INVALID_FIELD_VALUE, value, "value is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Convert input parameter from Managed to Unmanaged
		valueStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)value).ToPointer());
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->SetValue((UInt16)ftype, valueStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(valueStr);
	}
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::UnSet()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();

		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->UnSet();

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

bool Nasa::Gmsec::Net::Field::operator==(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b)
{
	STARTMANAGEDCATCH
	{
		// check if nullptr (can't use == on original object because would cause infinite recursion
		if ((dynamic_cast<Object^>(a) == nullptr) && (dynamic_cast<Object^>(b) == nullptr))
		{
			return true;
		}
		else if ((dynamic_cast<Object^>(a) == nullptr) || (dynamic_cast<Object^>(b) == nullptr))
		{
			return false;
		}
		else
		{
			gmsec::Field* a_impl = a->GetUnmanImpl();
			gmsec::Field* b_impl = b->GetUnmanImpl();

			// check native null, does not call native operator
			if ((a_impl == NULL) && (b_impl == NULL))
			{
				return true;
			}
			else if ((a_impl == NULL) || (b_impl == NULL))
			{
				return false;
			}
			else
			{
				// directly call native operator
				return a_impl->operator==(*b_impl);
			}
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(false)
}

bool Nasa::Gmsec::Net::Field::operator!=(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b)
{
	STARTMANAGEDCATCH
	{
		// check if nullptr (can't use == on original object because would cause infinite recursion
		if ((dynamic_cast<Object^>(a) == nullptr) && (dynamic_cast<Object^>(b) == nullptr))
		{
			return false;
		}
		else if ((dynamic_cast<Object^>(a) == nullptr) || (dynamic_cast<Object^>(b) == nullptr))
		{
			return true;
		}
		else
		{
			gmsec::Field* a_impl = a->GetUnmanImpl();
			gmsec::Field* b_impl = b->GetUnmanImpl();

			// check native null, does not call native operator
			if ((a_impl == NULL) && (b_impl == NULL))
			{
				return false;
			}
			else if ((a_impl == NULL) || (b_impl == NULL))
			{
				return true;
			}
			else
			{
				// directly call native operator
				return a_impl->operator!=(*b_impl);
			}
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(false)
}

bool Nasa::Gmsec::Net::Field::operator>(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b)
{
	STARTMANAGEDCATCH
	{
		// check if nullptr (can't use == on original object because would cause infinite recursion
		if ((dynamic_cast<Object^>(a) == nullptr) || (dynamic_cast<Object^>(b) == nullptr))
		{
			return false;
		}
		else
		{
			gmsec::Field* a_impl = a->GetUnmanImpl();
			gmsec::Field* b_impl = b->GetUnmanImpl();

			// check native null, does not call native operator
			if ((a_impl == NULL) || (b_impl == NULL))
			{
				return false;
			}
			else
			{
				// directly call native operator
				return a_impl->operator>(*b_impl);
			}
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(false)
}

bool Nasa::Gmsec::Net::Field::operator<(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b)
{
	STARTMANAGEDCATCH
	{
		// check if nullptr (can't use == on original object because would cause infinite recursion
		if ((dynamic_cast<Object^>(a) == nullptr) || (dynamic_cast<Object^>(b) == nullptr))
		{
			return false;
		}
		else
		{
			gmsec::Field* a_impl = a->GetUnmanImpl();
			gmsec::Field* b_impl = b->GetUnmanImpl();

			// check native null, does not call native operator
			if ((a_impl == NULL) || (b_impl == NULL))
			{
				return false;
			}
			else
			{
				// directly call native operator
				return a_impl->operator<(*b_impl);
			}
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(false)

}

bool Nasa::Gmsec::Net::Field::operator>=(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b)
{
	STARTMANAGEDCATCH
	{
		// check if nullptr (can't use == on original object because would cause infinite recursion
		if ((dynamic_cast<Object^>(a) == nullptr) && (dynamic_cast<Object^>(b) == nullptr))
		{
			return true;
		}
		else if ((dynamic_cast<Object^>(a) == nullptr) || (dynamic_cast<Object^>(b) == nullptr))
		{
			return false;
		}
		else
		{
			gmsec::Field* a_impl = a->GetUnmanImpl();
			gmsec::Field* b_impl = b->GetUnmanImpl();

			// check native null, does not call native operator
			if ((a_impl == NULL) && (b_impl == NULL))
			{
				return true;
			}
			else if ((a_impl == NULL) || (b_impl == NULL))
			{
				return false;
			}
			else
			{
				// directly call native operator
				return a_impl->operator>=(*b_impl);
			}
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(false)
}

bool Nasa::Gmsec::Net::Field::operator<=(Nasa::Gmsec::Net::Field ^ a, Nasa::Gmsec::Net::Field ^ b)
{
	// check if nullptr (can't use == on original object because would cause infinite recursion
	if ((dynamic_cast<Object^>(a) == nullptr) && (dynamic_cast<Object^>(b) == nullptr))
	{
			return true;
	}
	else if ((dynamic_cast<Object^>(a) == nullptr) || (dynamic_cast<Object^>(b) == nullptr))
	{
		return false;
	}
	else
	{
		gmsec::Field* a_impl = a->GetUnmanImpl();
		gmsec::Field* b_impl = b->GetUnmanImpl();

		// check native null, does not call native operator
		if ((a_impl == NULL) && (b_impl == NULL))
		{
			return true;
		}
		else if ((a_impl == NULL) || (b_impl == NULL))
		{
			return false;
		}
		else
		{
			// directly call native operator
			return a_impl->operator<=(*b_impl);
		}
	}
}

GMSECTypeDefs Nasa::Gmsec::Net::Field::LookupType(const String ^ ftype)
{
	char *ftypeStr = nullptr;
	STARTMANAGEDCATCH
	{
		// Convert input parameter from Managed to Unmanaged
		ftypeStr = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi((System::String ^)ftype).ToPointer());
		
		// Call the unmanaged method.
		UInt16 gType = mImpl->LookupType(ftypeStr);

		// If Enum has definition, else error
		if (Enum::IsDefined(GMSECTypeDefs::typeid, gType))
		{
			return (GMSECTypeDefs)gType;
		}
		else
		{
			return GMSECTypeDefs::UNSET;
		}
	}
	ENDMANAGEDCATCHRETURNVALUE(GMSECTypeDefs::UNSET)
	finally
	{
		// Be sure to free any unmanaged char* that have been allocated assigned to avoid memory leaks
		FREE_HGLOBAL_IF_NOT_NULLPTR(ftypeStr);
	}
}

/// <seealso cref="Message::ToXML()" />
Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::Field::ToXML([Runtime::InteropServices::Out] const String ^ %xml)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();

		char *xmlStr = nullptr;

		// Call the unmanaged method.
		*status_impl = mImpl->ToXML((const char *&)(xmlStr));

		xml = gcnew String(xmlStr);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_FIELD_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

gmsec::Field * Nasa::Gmsec::Net::Field::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}


void Nasa::Gmsec::Net::Field::ResetUnmanImpl()
{
	// Reset the unmanaged implementation pointer.
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}

void Nasa::Gmsec::Net::Field::SetUnmanImpl(gmsec::Field* field)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = field;
}
