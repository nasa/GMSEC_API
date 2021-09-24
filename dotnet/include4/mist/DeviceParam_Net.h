/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_DEVICE_PARAM_NET_H
#define GMSEC_API_DEVICE_PARAM_NET_H


// C++ API native
#include <gmsec4/mist/DeviceParam.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Field;

namespace MIST
{


/// <summary>
/// This class is a lightweight container for holding information on a DeviceParam,
/// and used to generate GMSEC Device messages by the ConnectionManager and Device classes.
/// </summary>


public ref class DeviceParam
{
public:
	/// <summary>Constructor for initializing the DeviceParam object with a sampled value.</summary>
	///
	/// <param name="name">The name of the device parameter</param>
	/// <param name="timestamp">The timestamp of when the sampled value was collected</param>
	/// <param name="value">The sampled value</param>
	DeviceParam(System::String^ name, System::String^ timestamp, Field^ value);


	/// <summary>Copy-constructor that can be used to make a deep-copy of another device parameter object.</summary>
	///
	/// <param name="other">The device parameter object to copy</param>
	DeviceParam(DeviceParam^ other);


	/// <summary>Destructor</summary>
	~DeviceParam();


	/// <summary>Returns the name of the device parameter.</summary>
	System::String^ GetName();


	/// <summary>Returns the timestamp of when the device parameter sample was taken.</summary>
	System::String^ GetTimestamp();


	/// <summary>Returns the sample value.</summary>
	Field^ GetValue();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!DeviceParam();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="param">unmanaged implementation to initialize with</param>
	DeviceParam(gmsec::api::mist::DeviceParam* param, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::DeviceParam* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::DeviceParam* param, bool owned);


private:
	gmsec::api::mist::DeviceParam* m_impl;
	bool                           m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
