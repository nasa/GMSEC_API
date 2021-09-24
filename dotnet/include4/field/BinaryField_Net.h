/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_BINARYFIELD_NET_H
#define GMSEC_API_BINARYFIELD_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/BinaryField.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Specialized Field class that can be used to store a raw data value.
/// </summary>


public ref class BinaryField : public Field
{
public:
	/// <summary>Creates a BinaryField object with the given value</summary>
	///
	/// <param name="name">The name of the field</param>
	/// <param name="data">Array of raw data</param>
	///
	/// <exception cref="GMSEC_Exception">If the name is null or contains an empty-string</exception>
	BinaryField(System::String^ name, array<System::Byte>^ data);


	/// <summary>Destructor</summary>
	~BinaryField();


	/// <summary>Fetches the value associated with the field</summary>
	///
	/// <returns>The data associated with the field</returns>
	array<System::Byte>^ GetValue();


	/// <summary>Fetches the length of the data associated with the field</summary>
	///
	/// <returns>The length of the data associated with the field</returns>
	System::Int64 GetLength();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!BinaryField();


internal:
	BinaryField(gmsec::api::Field* field);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::BinaryField* field);


private:
	gmsec::api::BinaryField* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
