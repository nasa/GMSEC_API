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
/// A specialized MistMessage object that is capable of storing ProductFile objects.
/// </summary>
///
/// <seealso cref="MistMessage"/>
/// <seealso cref="Config"/>
/// <seealso cref="ProductFile"/>
/// <seealso cref="ProductFileIterator"/>
/// <seealso cref="Specification"/>

public class ProductFileMessage : MistMessage {

  /// <summary>
  /// Initializes the message instance and automatically builds the appropriate
  /// schemaID based on the version of Specification used.
  /// </summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="status">RESPONSE-STATUS field to insert into a MSG PROD message.</param>
  /// <param name="kind">The kind of message to instantiate.</param>
  /// <param name="productType">PROD-TYPE field to insert into a MSG PROD message.</param>
  /// <param name="productSubtype">PROD-SUBTYPE field to insert into a MSG PROD message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if productType is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public ProductFileMessage(string subject, ResponseStatus.Response status, Message.MessageKind kind, string productType, string productSubtype, Specification spec) {
  }

  /// <summary>
  /// Initializes the message instance and automatically builds the appropriate
  /// schemaID based on the version of Specification used
  /// </summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="status">RESPONSE-STATUS field to insert into a MSG PROD message.</param>
  /// <param name="kind">The kind of message to instantiate.</param>
  /// <param name="productType">PROD-TYPE field to insert into a MSG PROD message.</param>
  /// <param name="productSubtype">PROD-SUBTYPE field to insert into a MSG PROD message.</param>
  /// <param name="config">A configuration to associate with the message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if productType is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public ProductFileMessage(string subject, ResponseStatus.Response status, Message.MessageKind kind, string productType, string productSubtype, Config config, Specification spec) {
  }

  /// <summary>Initializes the message with a given schema ID.</summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="status">RESPONSE-STATUS field to insert into a MSG PROD message.</param>
  /// <param name="schemaID">The string used to identify the message schema in C2MS or other message specification. The schema ID has the
  ///                        format of: messageKind.messageType.messageSubtype (e.g. MSG.PROD).</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public ProductFileMessage(string subject, ResponseStatus.Response status, string schemaID, Specification spec) {
  }

  /// <summary>Initializes the message with a given schema ID.</summary>
  ///
  /// <param name="subject">The subject string for the message.</param>
  /// <param name="responseStatus">RESPONSE-STATUS field to insert into a MSG PROD message.</param>
  /// <param name="schemaID">The string used to identify the message schema in C2MS or other message specification. The schema ID has the
  ///                        format of: messageKind.messageType.messageSubtype (e.g. MSG.PROD).</param>
  /// <param name="config">A configuration to associate with the message.</param>
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  ///
  /// <exception cref="GmsecException">Thrown if subject is null or contains an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if Specification is null.</exception>
  public ProductFileMessage(string subject, ResponseStatus.Response responseStatus, string schemaID, Config config, Specification spec) {
  }

  /// <summary>Copy constructor - initializes the message instance using the other given ProductFileMessage.<summary>
  ///
  /// <param name="other">The ProductFileMessage to be copied.</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given ProductFileMessage is null.</exception>
  public ProductFileMessage(ProductFileMessage other) {
  }

  /// <summary>Initializes a ProductFileMessage using an XML or JSON string.</summary>
  ///
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a %Product File %Message.
  /// </exception>
  ///
  /// <remarks>This method has been deprecated; use ProductFileMessage(Specification, string) instead.</remarks>
  public ProductFileMessage(string data) {
  }

  /// <summary>Constructs a ProductFileMessage using an XML or JSON string, and ensures the message adheres to the provided specification</summary>
  ///
  /// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
  /// <param name="data">XML or JSON string used to initialize the message.</param>
  ///
  /// <exception cref="GMSEC_Exception">
  /// Thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
  /// </exception>
  public ProductFileMessage(Specification spec, string data) {
  }

  /// <summary>Add a ProductFile to the message</summary>
  ///
  /// <param name="productFile">The ProductFile to add</param>
  ///
  /// <exception cref="GmsecException">Thrown if the given ProductFile is null.</exception>
  public void AddProductFile(ProductFile productFile) {
  }

  /// <summary>Returns the Response Status associated with this Product File Message.</summary>
  public ResponseStatus.Response GetResponseStatus() {
  }

  /// <summary>Returns the PROD-TYPE value associated with this Product File Message.</summary>
  public string GetProductType() {
  }

  /// <summary>Returns the PROD-SUBTYPE value associated with this Product File Message.</summary>
  public string GetProductSubtype() {
  }

  /// <summary>
  /// Method that allows the callee to get the ProductFileIterator associated with the ProductFileMessage.
  /// <p>
  /// This iterator will allow for applications to iterate over the ProductFile objects stored within the %ProductFileMessage.
  /// The iterator is reset each time getProductFileIterator() is called. The iterator itself is destroyed when the
  /// %ProductFileMessage object is destroyed.
  /// </summary>
  ///
  /// <remarks>
  /// Note: Only one ProductFileIterator object is associated with a %ProductFileMessage object; multiple calls to
  /// getProductFileIterator() will return a reference to the same ProductFileIterator object. Each call will reset the iterator.
  /// </remarks>
  ///
  public ProductFileIterator GetProductFileIterator() {
  }

  /// <summary>Constructs and returns a %ProductFileMessage from an ordinary message.</summary>
  ///
  /// <exception cref="GmsecException">
  /// Thrown if the given Message is null or if it cannot be interpreted as a Product File %Message.
  /// </exception>
  public static ProductFileMessage ConvertMessage(Message message) {
  }

}

}
