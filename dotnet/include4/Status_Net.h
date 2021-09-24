/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_STATUS_NET_H
#define GMSEC_API_STATUS_NET_H

// Managed
#include <Errors_Net.h>

// C++ API native
#include <gmsec4/Status.h>


namespace GMSEC
{
namespace API
{

public ref class Status
{
public:
	/// <summary>Default constructor for a Status object</summary>
	Status();


	/// <summary>Create a status object with a specific error classification, error code, error reason and custom code (if any)</summary>
	///
	/// <param name="statusClass>status classification</param>
	/// <param name="statusCode>status code</param>
	/// <param name="reason">status reason</param>
	Status(StatusClass statusClass, StatusCode statusCode, System::String^ reason);


	/// <summary>Copy-constructor</summary>
	///
	/// <param name="other">The Status object to copy</param>
	Status(Status^ %other);


	/// <summary>Destructor</summary>
	~Status();


	/// <summary>Deep copy asignment operator.</summary>
	///
	/// <param name="other">The Status object to copy</param>
	///
	/// <returns>New copied status object</returns>
	Status^ operator=(Status^ %other);


	/// <summary>This will return TRUE if the status is indicating an error, FALSE otherwise</summary>
	///
	/// <returns>If error</returns>
	bool IsError();


	/// <summary>This function will return a verbose status string as follows:
	/// [<i>class</i>,<i>code</i>,<i>custom</i>]: <i>reason</i></summary>
	///
	/// <returns>Verbose status string</returns>
	System::String^ Get();


	/// <summary>This is a convience function that will set class, code, and error text.</summary>
	///
	/// <param name="statusClass">status class</param>
	/// <param name="statusCode">status code</param>
	/// <param name="reason">status reason</param>
	void Set(StatusClass statusClass, StatusCode statusCode, System::String^ reason);


	/// <summary>This function will return the status class.</summary>
	///
	/// <seealso cref="StatusClass" />
	///
	/// <returns>Status Class</returns>
	StatusClass GetClass();


	/// <summary>This will set the status class</summary>
	///
	/// <param name="statusClass">status class</param>
	void SetClass(StatusClass statusClass);


	/// <summary>This will return the status code.</summary>
	///
	/// <seealso cref="StatusCode" />
	///
	/// <returns>Status Code</returns>
	StatusCode GetCode();


	/// <summary>This will set the specific status code.</summary>
	///
	/// <param name="statusCode">status code</param>
	void SetCode(StatusCode statusCode);


	/// <summary>This function will retrieve the string detailed description.</summary>
	///
	/// <returns>Detailed Description String</returns>
	System::String^ GetReason();


	/// <summary>This will set the status description.</summary>
	///
	/// <param name="reason">reason text</param>
	void SetReason(System::String^ reason);


	/// <summary>This will return the custom code number for this status.</summary>
	///
	/// <returns>Custom Code</returns>
	System::Int32 GetCustomCode();


	/// <summary>This will set the custom code.</summary>
	///
	/// <param name="code">custom code</param>
	void SetCustomCode(System::Int32 code);


	/// <summary>This clears this Status to NO_ERROR.</summary>
	void Reset();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Status();


internal:
	/// <summary>Get unmanaged implementation</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::Status* GetUnmanagedImplementation();


	/// <summary>If present, delete old implementation, and set unmanaged implementation to a new value</summary>
	///
	/// <param name="status">Unmanaged implementation to associate with the Status object</param>
	void SetUnmanagedImplementation(gmsec::api::Status* status);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Constructor for creating association of this object with an existing unmanaged object</summary>
	///
	/// <param name="status">Handle to the unmanaged implementation object</param>
	Status(gmsec::api::Status* status);


private:
	gmsec::api::Status* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
