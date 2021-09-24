
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
#ifndef status_Net
#define status_Net

// managed
#include "GMSEC_Net.h"
#include "GMSECErrors_Net.h"

// native
#include "gmsec/Status.h"

using namespace gmsec;
using namespace System;

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

/// <summary>
/// Returned by the API function calls to provide status feedback.  Status provides an error
/// code and error message specific to the error type, but independent of error source.
/// </summary>
/// <seealso cref="GMSECErrorClasses" />
/// <seealso cref="GMSECErrorCodes" />
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class Status
{
private:
 	gmsec::Status * mImpl;

internal:
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::Status * GetUnmanImpl();

	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	/// <param name="status">unmanaged implementation to initialize with</param>
	Status(gmsec::Status * status);

	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::Status* status);

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Status();

public:
	/// <summary>Create a status set to STATUS_NO_ERROR</summary>
	Status();
	
	/// <summary>Create a status with specific class, code, and string.</summary>
	/// <param name="eclass">error class</param>
	/// <param name="code">error code</param>
	/// <param name="text">error string</param>
	Status(GMSECErrorClasses eclass, GMSECErrorCodes code, 
		const String ^ text);
	
	/// <summary>Deep copy constructor.</summary>
	/// <param name="status">status to copy</param>
	Status(Nasa::Gmsec::Net::Status ^ status);
	
	/// <summary>Destructor</summary>
	~Status();
	
	/// <summary>Deep copy asignment operator.</summary>
	/// <param name="status">status to copy</param>
	/// <returns>New copied status object</returns>
	Status ^ operator=(Nasa::Gmsec::Net::Status ^ status);
	
	/// <summary>This function will retrieve the string detailed description of this error.</summary>
	/// <returns>Detailed Description String of Error</returns>
	const String ^ GetString();
	
	/// <summary>This function will return a verbose error string as follows:
	/// [ <i>class</i>, <i>code</i> ]: <i>error text</i></summary>
	/// <returns>Verbose Error string</returns>
	const String ^ Get();
	
	/// <summary>This function will return the error class ID.</summary>
	/// <seealso cref="GMSECErrorClasses" />
	/// <returns>Error Class</returns>
	GMSECErrorClasses GetClass();
	
	/// <summary>This will return the error code number for this status for easy comparison.</summary>
	/// <seealso cref="GMSECErrorCodes" />
	/// <returns>Error Code</returns>
	GMSECErrorCodes GetCode();

	/// <summary>This will return the custom code number for this status.</summary>
	/// <returns>Custom Code</returns>
	const Int32 GetCustomCode();
	
	/// <summary>This will return TRUE if the status is indicating an error, FALSE otherwise</summary>
	/// <returns>If error</returns>
	bool IsError();
	
	/// <summary>This will set the error string.</summary>
	/// <param name="text">error text</param>
	void SetString(const String ^ text);
	
	/// <summary>This will set the error class ID</summary>
	/// <param name="eclass">error class</param>
	void SetClass(GMSECErrorClasses eclass);
	
	/// <summary>This will set the specific error code.</summary>
	/// <param name="code">error code</param>
	void SetCode(GMSECErrorCodes code);
	
	/// <summary>This will set the custom code.</summary>
	/// <param name="code">custom code</param>
	void SetCustomCode(Int32 code);

	/// <summary>This is a convience function that will set class, code, and error text.</summary>
	/// <param name="eclass">error class</param>
	/// <param name="code">error code</param>
	/// <param name="text">error text</param>
	void Set(GMSECErrorClasses eclass, GMSECErrorCodes code, 
		const String ^ text);
	
	/// <summary>This clears this Status to STATUS_NO_ERROR.</summary>
	void ReSet();

};

}
}
}

#endif  // status_Net
