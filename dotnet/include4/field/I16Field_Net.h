/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_I16FIELD_NET_H
#define GMSEC_API_I16FIELD_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/I16Field.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Specialized Field class that can be used to store a signed 16-bit value.
/// </summary>


public ref class I16Field : public Field
{
public:
	/// <summary>Creates an I16field object with the given value</summary>
	///
	/// <param name="name">The name of the field</param>
	/// <param name="value">The value to store in the field</param>
	///
	/// <exception cref="GMSEC_Exception">If the name is null or contains an empty-string</exception>
	I16Field(System::String^ name, System::Int16 value);


	/// <summary>Destructor</summary>
	~I16Field();


	/// <summary>Fetches the value associated with the field</summary>
	///
	/// <returns>The value associated with the field</returns>
	System::Int16 GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!I16Field();


internal:
	I16Field(gmsec::api::Field* field);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::I16Field* field);


private:
	gmsec::api::I16Field* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
