/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_U32FIELD_NET_H
#define GMSEC_API_U32FIELD_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/U32Field.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Specialized Field class that can be used to store an unsigned 32-bit value.
/// </summary>


public ref class U32Field : public Field
{
public:
	/// <summary>Creates an U32field object with the given value</summary>
	///
	/// <param name="name">The name of the field</param>
	/// <param name="value">The value to store in the field</param>
	///
	/// <exception cref="GMSEC_Exception">If the name is null or contains an empty-string</exception>
	U32Field(System::String^ name, System::UInt32 value);


	/// <summary>Destructor</summary>
	~U32Field();


	/// <summary>Fetches the value associated with the field</summary>
	///
	/// <returns>The value associated with the field</returns>
	System::UInt32 GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!U32Field();


internal:
	U32Field(gmsec::api::Field* field);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::U32Field* field);


private:
	gmsec::api::U32Field* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
