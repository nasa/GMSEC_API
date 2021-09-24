/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_EXCEPTION_NET_H
#define GMSEC_API_EXCEPTION_NET_H

// Managed
#include <Errors_Net.h>

// C++ API native
#include <gmsec4/Exception.h>


namespace GMSEC
{
namespace API
{
ref class Status;


public ref class GMSEC_Exception : public System::Exception
{
public:
	/// <summary>Constructor</summary>
	///
	/// <param name="errorClass">The error class for the exception</param>
	/// <param name="errorCode">The error code for the exception</param>
	/// <param name="errorMsg">The error message for the exception</param>
	///
	/// <seealso cref="StatusClass"/>
	/// <seealso cref="StatusCode"/>
	GMSEC_Exception(StatusClass errorClass, StatusCode errorCode, System::String^ errorMsg);


	/// <summary>Constructor</summary>
	///
	/// <param name="errorClass">The error class for the exception</param>
	/// <param name="errorCode">The error code for the exception</param>
	/// <param name="customCode">The custom code for the exception</param>
	/// <param name="errorMsg">The error message for the exception</param>
	///
	/// <seealso cref="StatusClass"/>
	/// <seealso cref="StatusCode"/>
	GMSEC_Exception(StatusClass errorClass, StatusCode errorCode, int customCode, System::String^ errorMsg);


	/// <summary>Copy Constructor</summary>
	///
	/// <param name="other">The other GMSEC_Exception object to copy</param>
	GMSEC_Exception(GMSEC_Exception^ %other);


	/// <summary>Constructor that translates the given Status object into a GMSEC_Exception object</summary>
	///
	/// <param name="status">The Status object to use for the translation</param>
	GMSEC_Exception(Status^ %status);


	/// <summary>Destructor</summary>
	~GMSEC_Exception();


	/// <summary>This will return the error status class</summary>
	///
	/// <seealso cref="StatusClass"/>
	///
    /// <returns>Error status class</returns>
    StatusClass GetErrorClass();


	/// <summary>This will return the error status code</summary>
	///
	/// <seealso cref="StatusCode"/>
	///
    /// <returns>Error status code</returns>
	StatusCode GetErrorCode();


	/// <summary>This will return the custom error code</summary>
	///
    /// <returns>Custom error code</returns>
	int GetCustomCode();


	/// <summary>This will return the error message string</summary>
	///
    /// <returns>Error message string</returns>
	System::String^ GetErrorMessage();


	/// <summary>This will return a string with the format
	/// "[errorClass,errorCode,customCode]: errorMessage"</summary>
	///
	/// <seealso cref="StatusClass"/>
	/// <seealso cref="StatusCode"/>
	///
    /// <returns>Error message string</returns>
	virtual System::String^ ToString() override;


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!GMSEC_Exception();


internal:
	/// <summary>Get unmanaged implementation</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::Exception* GetUnmanagedImplementation();


	/// <summary>If present, delete old implementation, and set unmanaged implementation to a new value</summary>
	///
	/// <param name="exception">Unmanaged implementation to associate with the Status object</param>
	void SetUnmanagedImplementation(gmsec::api::Exception* exception);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Constructor for creating association of this object with an existing unmanaged object</summary>
	///
	/// <param name="exception">Handle to the unmanaged implementation object</param>
	GMSEC_Exception(gmsec::api::Exception& exception);


private:
	gmsec::api::Exception* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
