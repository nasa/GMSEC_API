/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>
/// A specialized MistMessage class that is capable of storing Device objects.
/// </summary>
///
/// <seealso cref="MistMessage"/>
/// <seealso cref="Config"/>
/// <seealso cref="Device"/>
/// <seealso cref="DeviceIterator"/>
/// <seealso cref="Specification"/>

public class DeviceMessage : MistMessage {

  /// <summary>
  /// Initializes the message instance and automatically builds the appropriate
  /// schemaID based on the version of Specification used.
  /// </summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public DeviceMessage(string subject, Specification spec) {
  }

  /// <summary>
  /// Initializes the message instance and automatically builds the appropriate
  /// schemaID based on the version of Specification used.
  /// </summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="config">A configuration to associate with the message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public DeviceMessage(string subject, Config config, Specification spec) {
  }

  /// <summary>Copy constructor - initializes the message instance using the other given DeviceMessage.<summary>
  ///
  /// <param name="other">The DeviceMessage to be copied.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given DeviceMessage is null.</exception>
  public DeviceMessage(DeviceMessage other) {
  }

  /// <summary>Initializes a DeviceMessage using an XML or JSON string.</summary>
  ///
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GmsecException">
  ///  Thrown if the data is null or cannot be interpreted as valid XML or JSON representation of a DeviceMessage.
  /// </exception>
  ///
  /// <remarks>This method has been deprecated; use DeviceMessage(Specification, string) instead.</remarks>
  public DeviceMessage(string data) {
  }

  /// <summary>Constructs a DeviceMessage using an XML or JSON string, and ensures the message adheres to the provided specification</summary>
  ///
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GMSEC_Exception">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
  /// </exception>
  public DeviceMessage(Specification spec, string data) {
  }


  /// <summary>Add a Device to the message</summary>
  ///
  /// <param name="device">The Device to add</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given Device is null.</exception>
  public void AddDevice(Device device) {
  }

  /// <summary>
  /// Method that allows the callee to get the DeviceIterator associated with the DeviceMessage.
  /// <p>
  /// This iterator will allow for applications to iterate over the Device objects stored within the %DeviceMessage.
  /// The iterator is reset each time getDeviceIterator() is called. The iterator itself is destroyed when the
  /// DeviceMessage object is destroyed.
  /// </summary>
  ///
  /// <remarks>
  ///  Note: Only one DeviceIterator object is associated with a %DeviceMessage object; multiple calls to
  /// getDeviceIterator() will return a reference to the same DeviceIterator object. Each call will reset the iterator.
  /// </remarks>
  public DeviceIterator GetDeviceIterator() {
  }

  /// <summary>Constructs and returns a DeviceMessage from an ordinary message.</summary>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the given Message object is null or if it cannot be interpreted as a valid %Device %Message.
  /// </exception>
  public static DeviceMessage ConvertMessage(Message message) {
  }

}

}
