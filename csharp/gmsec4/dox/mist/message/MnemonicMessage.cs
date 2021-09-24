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
/// A specialized MistMessage class that is capable of storing Mnemonic objects.
/// </summary>
///
/// <seealso cref="MistMessage"/>
/// <seealso cref="Config"/>
/// <seealso cref="Mnemonic"/>
/// <seealso cref="MnemonicIterator"/>
/// <seealso cref="Specification"/>

public class MnemonicMessage : MistMessage {

  /// <summary>
  /// Initializes the message instance and automatically builds the appropriate
  /// schemaID based on the version of Specification used
  /// </summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="schemaID">The string used to identify the message schema in the GMSEC ISD.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if the schema ID is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public MnemonicMessage(string subject, string schemaID, Specification spec) {
  }

  /// <summary>
  /// Initializes the message instance and automatically builds the appropriate
  /// schemaID based on the version of Specification used
  /// </summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="schemaID">The string used to identify the message schema in the GMSEC ISD.</param>
  /// <param name="config">A configuration to associate with the message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if the schema ID is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public MnemonicMessage(string subject, string schemaID, Config config, Specification spec) {
  }

  /// <summary>Copy constructor - initializes the message instance using the other given MnemonicMessage.<summary>
  ///
  /// <param name="other">The MnemonicMessage to be copied.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given MnemonicMessage is null.</exception>
  public MnemonicMessage(MnemonicMessage other) {
  }

  /// <summary>Initializes a MnemonicMessage using an XML or JSON string.</summary>
  ///
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON representation of a %MnemonicMessage.
  /// </exception>
  ///
  /// <remarks>This method has been deprecated; use MnemonicMessage(Specification, string) instead.</remarks>
  public MnemonicMessage(string data) {
  }

  /// <summary>Constructs a MnemonicMessage using an XML or JSON string, and ensures the message adheres to the provided specification</summary>
  ///
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GMSEC_Exception">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
  /// </exception>
  public MnemonicMessage(Specification spec, string data) {
  }

  /// <summary>Add a Mnemonic to the message</summary>
  ///
  /// <param name="mnemonic">The Mnemonic to add</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given Mnemonic is null.</exception>
  public void AddMnemonic(Mnemonic mnemonic) {
  }


  /// <summary>
  /// Method that allows the callee to get the MnemonicIterator associated with the MnemonicMessage.
  /// <p>
  /// This iterator will allow for applications to iterate over the Mnemonic objects stored within the %MnemonicMessage.
  /// The iterator is reset each time getMnemonicIterator() is called. The iterator itself is destroyed when the
  /// MnemonicMessage object is destroyed.
  /// <summary>
  ///
  /// <remarks>
  /// Note: Only one MnemonicIterator object is associated with a %MnemonicMessage object; multiple calls to
  /// getMnemonicIterator() will return a reference to the same MnemonicIterator object. Each call will reset the iterator.
  /// </remarks>
  ///
  public MnemonicIterator GetMnemonicIterator() {
  }

  /// <summary>Constructs and returns a MnemonicMessage from an ordinary message.</summary>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the Message is null or cannot be interpreted as valid %Mnemonic %Message.
  /// </exception>
  public static MnemonicMessage ConvertMessage(Message message) {
  }

}

}
