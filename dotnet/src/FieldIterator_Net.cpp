
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


#include "FieldIterator_Net.h"

using namespace Nasa::Gmsec::Net;

Nasa::Gmsec::Net::FieldIterator::!FieldIterator()
{
	// Free the unmanaged implementation pointer.
	ResetUnmanImpl();
}

Nasa::Gmsec::Net::FieldIterator::FieldIterator()
{
	// Initialize the unmanaged implementation pointer.
	mImpl = new gmsec::FieldIterator();
}

Nasa::Gmsec::Net::FieldIterator::FieldIterator(gmsec::FieldSelector fieldSelector)
{
	// Initialize the unmanaged implementation pointer and object.
	// Can only be called from C++ CLI (C# cannot pass native function)
	mImpl = new gmsec::FieldIterator(fieldSelector);
}

Nasa::Gmsec::Net::FieldIterator::FieldIterator(Nasa::Gmsec::Net::FieldSelectorChoice fieldSelectorChoice)
{
	// Based on field selector type initialize the unmanaged implementation
	if (fieldSelectorChoice == Nasa::Gmsec::Net::FieldSelectorChoice::SelectAllFields)
	{
		mImpl = new gmsec::FieldIterator(&gmsec::selectAllFields);
	}
	else if (fieldSelectorChoice == Nasa::Gmsec::Net::FieldSelectorChoice::SelectHeaderFields)
	{
		mImpl = new gmsec::FieldIterator(&gmsec::selectHeaderFields);
	}
	else if (fieldSelectorChoice == Nasa::Gmsec::Net::FieldSelectorChoice::SelectNonheaderFields)
	{
		mImpl = new gmsec::FieldIterator(&gmsec::selectNonheaderFields);
	}
}

Nasa::Gmsec::Net::FieldIterator::~FieldIterator()
{
	// Call the finalizer.
	this->!FieldIterator();
}

bool Nasa::Gmsec::Net::FieldIterator::HasNext()
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_VALUE_IF_NULLPTR_CHECK(mImpl, false);

		// Call the unmanaged method.
		return mImpl->HasNext();
	}
	ENDMANAGEDCATCHRETURNVALUE(false)
}

Nasa::Gmsec::Net::Status ^ Nasa::Gmsec::Net::FieldIterator::Next(Field^ field)
{
	STARTMANAGEDCATCH 
	{
		// Safety checks.
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_ITERATOR_ERROR, GMSECErrorCodes::OTHER_ERROR, mImpl, "mImpl is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_ITERATOR_ERROR, GMSECErrorCodes::INVALID_FIELD, field, "field is null");
		RETURN_STATUS_IF_NULLPTR_CHECK(GMSECErrorClasses::STATUS_ITERATOR_ERROR, GMSECErrorCodes::INVALID_FIELD, field->GetUnmanImpl(), "field impl is null");

		Nasa::Gmsec::Net::Status ^ status = gcnew Nasa::Gmsec::Net::Status();
		
		// Get native pointers
		gmsec::Status* status_impl = status->GetUnmanImpl();
		gmsec::Field* field_impl = field->GetUnmanImpl();

		// Call the unmanaged method.
		*status_impl = mImpl->Next(*field_impl);

		return status;
	}  // Catch any exceptions thrown and return a proper Status
	ENDMANAGEDCATCH(GMSECErrorClasses::STATUS_ITERATOR_ERROR, GMSECErrorCodes::OTHER_ERROR)
}

gmsec::FieldIterator * Nasa::Gmsec::Net::FieldIterator::GetUnmanImpl()
{
	// Return the unmanaged implementation pointer.
	return(mImpl);
}

void Nasa::Gmsec::Net::FieldIterator::ResetUnmanImpl()
{
	// Reset the unmanaged implementation pointer.
	if (mImpl != nullptr)
	{
		delete mImpl;
		mImpl = nullptr;
	}
}

void Nasa::Gmsec::Net::FieldIterator::SetUnmanImpl(gmsec::FieldIterator* fieldIterator)
{
	// Clean up
	ResetUnmanImpl();

	// New native impl
	mImpl = fieldIterator;
}
