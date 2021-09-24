/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_SERVICE_PARAM_NET_H
#define GMSEC_API_SERVICE_PARAM_NET_H


// C++ API native
#include <gmsec4/mist/ServiceParam.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Field;

namespace MIST
{


/// <summary>
/// This class is a lightweight container for holding information on a
/// Simple Service parameter, and used to generate GMSEC Simple Service
/// messages by the ConnectionManager class.
/// </summary>


public ref class ServiceParam
{
public:
	/// <summary>Constructor for initializing the ServiceParam object with a sampled value.</summary>
	///
	/// <param name="name">The name of the service parameter</param>
	/// <param name="value">The value of the parameter</param>
	ServiceParam(System::String^ name, Field^ value);


	/// <summary>Copy-constructor that can be used to make a deep-copy of another service parameter object.</summary>
	///
	/// <param name="other">The service parameter object to copy</param>
	ServiceParam(ServiceParam^ other);


	/// <summary>Destructor</summary>
	~ServiceParam();


	/// <summary>Returns the name of the service parameter.</summary>
	System::String^ GetName();


	/// <summary>Returns the parameter value.</summary>
	Field^ GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ServiceParam();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="param">unmanaged implementation to initialize with</param>
	ServiceParam(gmsec::api::mist::ServiceParam* param, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::ServiceParam* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::ServiceParam* param, bool owned);


private:
	gmsec::api::mist::ServiceParam* m_impl;
	bool                            m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
