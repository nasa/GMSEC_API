/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CHARFIELD_NET_H
#define GMSEC_API_CHARFIELD_NET_H


// Managed
#include <field/Field_Net.h>

// C++ API native
#include <gmsec4/field/CharField.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// Specialized Field class that can be used to store a char value.
/// </summary>


public ref class CharField : public Field
{
public:
	/// <summary>Creates a CharField object with the given value</summary>
	///
	/// <param name="name">The name of the field</param>
	/// <param name="value">The value to store in the field</param>
	///
	/// <exception cref="GMSEC_Exception">If the name is null or contains an empty-string</exception>
	CharField(System::String^ name, char value);


	/// <summary>Destructor</summary>
	~CharField();


	/// <summary>Fetches the value associated with the field</summary>
	///
	/// <returns>The value associated with the field</returns>
	char GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!CharField();


internal:
	CharField(gmsec::api::Field* field);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::CharField* field);


private:
	gmsec::api::CharField* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
