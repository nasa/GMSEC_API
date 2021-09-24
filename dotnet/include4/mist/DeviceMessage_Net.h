/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_DEVICE_MESSAGE_NET_H
#define GMSEC_API_DEVICE_MESSAGE_NET_H


// C# managed
#include <Message_Net.h>

#include <mist/MIST_Defs_Net.h>


// C++ API native
#include <gmsec4/mist/DeviceMessage.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Config;

namespace MIST
{

// Forward declarations
ref class Device;
ref class DeviceIterator;


/// <summary>
/// This class is a lightweight container for holding information on a Device, and
/// used to generate GMSEC Device messages by the ConnectionManager class.
/// </summary>
///
/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.DeviceMessage instead.


public ref class DeviceMessage : public Message
{
public:
	/// <summary>Basic constructor</summary>
	///
	/// <param name="subject">Message subject</param>
	/// <param name="version">The GMSEC Interface Specification Document (ISD) version to be used</param>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.DeviceMessage instead.
	DeviceMessage(System::String^ subject, unsigned int version);


	/// <summary>Constructor that initializes the message with a specific configuration</summary>
	///
	/// <param name="subject">Message subject</param>
	/// <param name="config">A configuration to associate with the message</param>
	/// <param name="version">The GMSEC Interface Specification Document (ISD) version to be used</param>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.DeviceMessage instead.
	DeviceMessage(System::String^ subject, Config^ config, unsigned int version);


	/// <summary>Constructor that initializes the message using an XML or JSON string</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the contents of the message</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the data cannot be parsed</exception>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.DeviceMessage instead.
	DeviceMessage(System::String^ data);


	/// <summary>Copy-constructor</summary>
	///
	/// <param name="other">The DeviceMessage object to copy</param>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.DeviceMessage instead.
	DeviceMessage(DeviceMessage^ other);


	/// <summary>Destructor</summary>
	~DeviceMessage();


	/// <summary>Returns the number of Device objects stored within the message.</summary>
	long GetNumDevices();


	/// <summary>Returns a reference to the Device object referenced by the given index.</summary>
	///
	/// <seealso cref="DeviceMessage::getNumDevices"/>
	Device^ GetDevice(long index);


	/// <summary>Associate a Device object with the message.</summary>
	///
	/// <param name="device">The Device object</param>
	void AddDevice(Device^ device);


	/// <summary>
	/// Method that allows the callee to get the DeviceIterator associated with the %DeviceMessage.
	/// This iterator will allow for applications to iterate over the Device objects stored within the %DeviceMessage.
	/// The iterator is reset each time GetDeviceIterator() is called.  The iterator itself is destroyed when the
	/// %DeviceMessage object is destroyed.
	/// </summary>
	///
	/// <note>
	/// Only one DeviceIterator object is associated with a %DeviceMessage object; multiple calls to
	/// GetDeviceIterator() will return a reference to the same DeviceIterator object.  Each call will reset the iterator.
	/// </note>
	///
	/// <returns>A reference to a DeviceIterator object.</return>
	DeviceIterator^ GetDeviceIterator();


	/// <summary>Constructs and returns a DeviceMessage from an an ordinary message.</summary>
	static DeviceMessage^ ConvertMessage(Message^ message);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!DeviceMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="deviceMessage">unmanaged implementation to initialize with</param>
	DeviceMessage(gmsec::api::mist::DeviceMessage* deviceMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::DeviceMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::DeviceMessage* deviceMsg, bool owned);
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
