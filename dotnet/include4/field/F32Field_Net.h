/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_F32FIELD_NET_H
#define GMSEC_API_F32FIELD_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/F32Field.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Specialized Field class that can be used to store a 32-bit floating point value.
/// </summary>


public ref class F32Field : public Field
{
public:
	/// <summary>Creates an F32field object with the given value</summary>
	///
	/// <param name="name">The name of the field</param>
	/// <param name="value">The value to store in the field</param>
	///
	/// <exception cref="GMSEC_Exception">If the name is null or contains an empty-string</exception>
	F32Field(System::String^ name, float value);


	/// <summary>Destructor</summary>
	~F32Field();


	/// <summary>Fetches the value associated with the field</summary>
	///
	/// <returns>The value associated with the field</returns>
	float GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!F32Field();


internal:
	F32Field(gmsec::api::Field* field);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::F32Field* field);


private:
	gmsec::api::F32Field* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
