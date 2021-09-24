/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_BOOLEANFIELD_NET_H
#define GMSEC_API_BOOLEANFIELD_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/BooleanField.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Specialized Field class that can be used to store a boolean value.
/// </summary>


public ref class BooleanField : public Field
{
public:
	/// <summary>Creates a BooleanField object with the given value</summary>
	///
	/// <param name="name">The name of the field</param>
	/// <param name="value">The value to store in the field</param>
	///
	/// <exception cref="GMSEC_Exception">If the name is null or contains an empty-string</exception>
	BooleanField(System::String^ name, bool value);


	/// <summary>Destructor</summary>
	~BooleanField();


	/// <summary>Fetches the value associated with the field</summary>
	///
	/// <returns>The value associated with the field</returns>
	bool GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!BooleanField();


internal:
	BooleanField(gmsec::api::Field* field);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::BooleanField* field);


private:
	gmsec::api::BooleanField* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
