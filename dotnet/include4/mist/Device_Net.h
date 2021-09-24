/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_DEVICE_NET_H
#define GMSEC_API_DEVICE_NET_H


// C++ API native
#include <gmsec4/mist/Device.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Field;

namespace MIST
{

// Forward declarations
ref class DeviceParam;


/// <summary>
/// This class is a lightweight container for holding information on a Device, and
/// used to generate GMSEC Device messages by the ConnectionManager class.
/// </summary>


public ref class Device
{
public:
	enum class DeviceStatus
	{
		DEBUG = 0,
		NOMINAL_GREEN = 1,
		YELLOW = 2,
		ORANGE = 3,
		RED = 4
	};


	/// <summary>Simple constructor</summary>
	///
	/// <param name="name">Device name</param>
	/// <param name="status">Device status</param>
	Device(System::String^ name, DeviceStatus status);


	/// <summary>Constructor that accepts list of Device Parameters</summary>
	///
	/// <param name="name">Device name</param>
	/// <param name="status">Device status</param>
	/// <param name="params">List of device parameters</param>
	Device(System::String^ name, DeviceStatus status, System::Collections::Generic::List<DeviceParam^>^ params);


	/// <summary>Copy-constructor for making deep-copy of other Device object.</summary>
	///
	/// <param name="other">The other DeviceParam object to copy</param>
	Device(Device^ other);


	/// <summary>Destructor</summary>
	~Device();


	/// <summary>Returns the name of the device</summary>
	System::String^ GetName();


	/// <summary>Returns the status of the device</summary>
	DeviceStatus GetStatus();


	/// <summary>Allows for the setting of the status of the Device</summary>
	///
	/// <param name="status">The status of the device</param>
	void SetStatus(DeviceStatus status);


	/// <summary>Returns the availability of a Device Number.</summary>
	bool NumberAvailable();


	/// <summary>Returns the Device Number.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device Number is not available</exception>
	///
	/// <seealso cref="Device::numberAvailable"/>
	Field^ GetNumber();


	/// <summary>Assigns a Number to the Device.</summary>
	///
	/// <param name="number">The Number to assign to the Device</param>
	void SetNumber(Field^ number);


	/// <summary>Returns the availability of a Device model number.</summary>
	bool ModelAvailable();


	/// <summary>Returns the Device model number.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device model number is not available</exception>
	///
	/// <seealso cref="Device::modelAvailable"/>
	System::String^ GetModel();


	/// <summary>Assigns a model number to the Device.</summary>
	///
	/// <param name="model">The model number to assign to the Device</param>
	void SetModel(System::String^ model);


	/// <summary>Returns the availability of a Device serial number.</summary>
	bool SerialAvailable();


	/// <summary>Returns the Device serial number.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device serial number is not available</exception>
	///
	/// <seealso cref="Device::serialAvailable"/>
	System::String^ GetSerial();
	

	/// <summary>Assigns a serial number to the Device.</summary>
	///
	/// <param name="serial">The serial number to assign to the Device</param>
	void SetSerial(System::String^ serial);


	/// <summary>Returns the availability of a Device version string.</summary>
	bool VersionAvailable();


	/// <summary>Returns the Device version string.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device version string is not available</exception>
	///
	/// <seealso cref="Device::versionAvailable"/>
	System::String^ GetVersion();
	

	/// <summary>Assigns a version string to the Device.</summary>
	///
	/// <param name="version">The version string to assign to the Device</param>
	void SetVersion(System::String^ version);


	/// <summary>Returns the availability of a Device group.</summary>
	bool GroupAvailable();


	/// <summary>Returns the Device group.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device group is not available</exception>
	///
	/// <seealso cref="Device::groupAvailable"/>
	System::String^ GetGroup();
	

	/// <summary>Assigns a group string to the Device.</summary>
	///
	/// <param name="version">The group string to assign to the Device</param>
	void SetGroup(System::String^ group);


	/// <summary>Returns the availability of a Device role.</summary>
	bool RoleAvailable();


	/// <summary>Returns the Device role.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device role is not available</exception>
	///
	/// <seealso cref="Device::roleAvailable"/>
	System::String^ GetRole();
	

	/// <summary>Assigns a role string to the Device.</summary>
	///
	/// <param name="version">The role string to assign to the Device</param>
	void SetRole(System::String^ role);


	/// <summary>Returns the availability of a Device information field.</summary>
	bool InfoAvailable();


	/// <summary>Returns the Device information field.</summary>
	///
	/// <exception cref="GMSEC_Exception">Thrown if a Device information field is not available</exception>
	///
	/// <seealso cref="Device::infoAvailable"/>
	Field^ GetInfo();
	

	/// <summary>Assigns a information Field to the Device.</summary>
	///
	/// <param name="info">The information Field to assign to the Device</param>
	void SetInfo(Field^ info);


	/// <summary>Returns the number of DeviceParam objects assigned to the Device.</summary>
	long GetParamCount();


	/// <summary>Returns a reference to the DeviceParam object held in the Device object.</summary>
	///
	/// <param name="index">The zero-based index of the DeviceParam object to access</param>
	///
	/// <seealso cref="Device::getParamCount"/>
	DeviceParam^ GetParam(long index);


	/// <summary>Assign a DeviceParam object to the list held by the Device.</summary>
	///
	/// <param name="param">The DeviceParam object to add to the Device</param>
	void AddParam(DeviceParam^ param);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Device();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="device">unmanaged implementation to initialize with</param>
	Device(gmsec::api::mist::Device* device, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::Device* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::Device* device, bool owned);


private:
	gmsec::api::mist::Device* m_impl;
	bool                      m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
