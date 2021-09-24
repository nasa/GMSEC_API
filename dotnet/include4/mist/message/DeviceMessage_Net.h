/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_DEVICE_MESSAGE_NET_H
#define GMSEC_API_MIST_DEVICE_MESSAGE_NET_H


#include <mist/message/MistMessage_Net.h>


// C++ API native
#include <gmsec4/mist/message/DeviceMessage.h>


namespace GMSEC
{
namespace API
{
	// Forward declaration(s)
	ref class Config;

namespace MIST
{
	// Forward declaration(s)
	ref class Device;
	ref class DeviceIterator;
	ref class Specification;

namespace MESSAGE
{


/// <summary>
/// A MistMessage object that is capable of storing Device objects
/// The following message schema IDs and their templates are supported:
/// 2014.00.GMSEC.MSG.C2CX.DEV
/// 2016.00.GMSEC.MSG.C2CX.DEV
/// 2019.00.C2MS.MSG.DEV
/// MSG.DEV
/// </summary>
///
/// <seealso cref="MistMessage"/>
/// <seealso cref="Config"/>
/// <seealso cref="Device"/>
/// <seealso cref="DeviceIterator"/>
/// <seealso cref="Specification"/>

public ref class DeviceMessage : public MistMessage
{
public:
	/// <summary>
	/// Initializes the message instance and automatically builds the appropriate
	/// schemaID based on the version of Specification used
	/// </summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if subject is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	DeviceMessage(System::String^ subject, Specification^ spec);


	/// <summary>
	/// Initializes the message instance and automatically builds the appropriate
	/// schemaID based on the version of Specification used
	/// </summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	/// <param name="config">A configuration to associate with the message.</param>
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if subject is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	DeviceMessage(System::String^ subject, Config^ config, Specification^ spec);


	/// <summary>Copy constructor - initializes the message instance using the other given DeviceMessage.<summary>
	///
	/// <param name="other">The DeviceMessage to be copied.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given DeviceMessage is null.
	/// </exception>
	DeviceMessage(DeviceMessage^ other);


	/// <summary>Initializes a DeviceMessage using an XML or JSON string.</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	///
	/// <remarks>This method has been deprecated; use DeviceMessage(Specification^, System::String^) instead.
	DeviceMessage(System::String^ data);


	/// <summary>Initializes a DeviceMessage using an XML or JSON string, and ensures the message adheres to the provided specification.</summary>
	///
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	DeviceMessage(Specification^ spec, System::String^ data);


	/// <summary>Destructor</summary>
	~DeviceMessage();


	/// <summary>Add a Device to the message</summary>
	///
	/// <param name="device">The Device to add</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given Device is null.
	/// </exception>
    void AddDevice(Device^ device);


	/// <summary>Returns the number of devices in this message.</summary>
    long GetNumDevices();


	/// <summary>Get the Device at the given index.</summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if index specified is not in the range of Devices in the message.
	/// </exception>
    Device^ GetDevice(long index);


	/// <summary>
	/// Method that allows the callee to get the DeviceIterator associated with the DeviceMessage.
	/// This iterator will allow for applications to iterate over the Device objects stored within the %DeviceMessage.
	/// The iterator is reset each time getDeviceIterator() is called.  The iterator itself is destroyed when the
	/// DeviceMessage object is destroyed.
	///
	/// Note: Only one DeviceIterator object is associated with a %DeviceMessage object; multiple calls to
	/// getDeviceIterator() will return a reference to the same DeviceIterator object.  Each call will reset the iterator.
	///
    DeviceIterator^ GetDeviceIterator();


	/// <summary>Constructs and returns a DeviceMessage from an ordinary message.</summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given Message is null.
	/// </exception>
    static DeviceMessage^ ConvertMessage(Message^ message);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!DeviceMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="mistMsg">unmanaged implementation to initialize with</param>
	/// <param name="owned">indicates whether the unmanaged implemention is owned by this object</param>
	DeviceMessage(gmsec::api::mist::message::DeviceMessage* devMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::message::DeviceMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::message::DeviceMessage* devMsg, bool owned);
};

} // end namespace MESSAGE
} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
