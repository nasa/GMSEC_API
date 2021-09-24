/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_PRODUCT_FILE_MESSAGE_NET_H
#define GMSEC_API_MIST_PRODUCT_FILE_MESSAGE_NET_H


#include <mist/message/MistMessage_Net.h>

#include <mist/MIST_Defs_Net.h>


// C++ API native
#include <gmsec4/mist/message/ProductFileMessage.h>


namespace GMSEC
{
namespace API
{
	// Forward declaration(s)
	ref class Config;

namespace MIST
{
	// Forward declaration(s)
	ref class ProductFile;
	ref class ProductFileIterator;
	ref class Specification;

namespace MESSAGE
{


/// <summary>
/// A MistMessage object that is capable of storing ProductFile objects
/// The following message schema IDs and their templates are supported, where <YYYY> is the year representing the ISD:
/// (e.g. 2014, 2016, etc).
///
/// <YYYY>.00.GMSEC.MSG.PROD.AAA
/// <YYYY>.00.GMSEC.MSG.PROD.AUTO
/// <YYYY>.00.GMSEC.MSG.PROD.FD
/// <YYYY>.00.GMSEC.MSG.PROD.MAS
/// <YYYY>.00.GMSEC.MSG.PROD.PAS
/// <YYYY>.00.GMSEC.MSG.PROD.SC
/// <YYYY>.00.GMSEC.MSG.PROD.TAC
/// <YYYY>.00.GMSEC.REQ.PROD.AAA
/// <YYYY>.00.GMSEC.REQ.PROD.AUTO
/// <YYYY>.00.GMSEC.REQ.PROD.FD
/// <YYYY>.00.GMSEC.REQ.PROD.MAS
/// <YYYY>.00.GMSEC.REQ.PROD.PAS
/// <YYYY>.00.GMSEC.REQ.PROD.SC
/// <YYYY>.00.GMSEC.REQ.PROD.TAC
/// <YYYY>.00.GMSEC.RESP.PROD.AAA
/// <YYYY>.00.GMSEC.RESP.PROD.AUTO
/// <YYYY>.00.GMSEC.RESP.PROD.FD
/// <YYYY>.00.GMSEC.RESP.PROD.MAS
/// <YYYY>.00.GMSEC.RESP.PROD.PAS
/// <YYYY>.00.GMSEC.RESP.PROD.SC
/// <YYYY>.00.GMSEC.RESP.PROD.TAC
/// </summary>
///
/// <seealso cref="MistMessage"/>
/// <seealso cref="Config"/>
/// <seealso cref="ProductFile"/>
/// <seealso cref="ProductFileIterator"/>
/// <seealso cref="Specification"/>

public ref class ProductFileMessage : public MistMessage
{
public:
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
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if subject is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	ProductFileMessage(System::String^ subject,
	                   ResponseStatus  status,
	                   MessageKind     kind,
	                   System::String^ productType,
	                   System::String^ productSubtype,
	                   Specification^  spec);


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
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if subject is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	ProductFileMessage(System::String^ subject,
	                   ResponseStatus  status,
	                   MessageKind     kind,
	                   System::String^ productType,
	                   System::String^ productSubtype,
	                   Config^         config,
	                   Specification^  spec);


	/// <summary>
	/// Initializes the message with a given schema ID
	/// </summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	/// <param name="status">RESPONSE-STATUS field to insert into a MSG PROD message.</param>
	/// <param name="schemaID">The string used to identify the message schema in the GMSEC ISD.  The schema ID has the
	///                        format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype
	///                        (e.g. 2016.00.GMSEC.MSG.PROD.AUTO).</param>
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if subject is null or contains an empty string.
	/// </exception>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if Specification is null or if a failure occurs when loading the template directory.
	/// </exception>
	ProductFileMessage(System::String^ subject,
	                   ResponseStatus  status,
	                   System::String^ schemaID,
	                   Specification^  spec);


	/// <summary>
	/// Initializes the message with a given schema ID
	/// </summary>
	///
	/// <param name="subject">The subject string for the message.</param>
	/// <param name="status">RESPONSE-STATUS field to insert into a MSG PROD message.</param>
	/// <param name="schemaID">The string used to identify the message schema in the GMSEC ISD.  The schema ID has the
	///                        format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype
	///                        (e.g. 2016.00.GMSEC.MSG.PROD.AUTO).</param>
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
	ProductFileMessage(System::String^ subject,
	                   ResponseStatus  status,
	                   System::String^ schemaID,
	                   Config^         config,
	                   Specification^  spec);


	/// <summary>Copy constructor - initializes the message instance using the other given ProductFileMessage.<summary>
	///
	/// <param name="other">The ProductFileMessage to be copied.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given ProductFileMessage is null.
	/// </exception>
	ProductFileMessage(ProductFileMessage^ other);


	/// <summary>Initializes a ProductFileMessage using an XML or JSON string.</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	///
	/// <remarks>This method has been deprecated; use ProductFileMessage(Specification^, System::String^) instead.
	ProductFileMessage(System::String^ data);


	/// <summary>Initializes a ProductFileMessage using an XML or JSON string, and ensures the message adheres to the provided specification.</summary>
	///
	/// <param name="spec">A reference to the specification this message's schema will adhere to.</param>
	/// <param name="data">XML or JSON string used to initialize the message.</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the data is null or cannot be interpreted as valid XML or JSON data that represents a message.
	/// </exception>
	ProductFileMessage(Specification^ spec, System::String^ data);


	/// <summary>Destructor</summary>
	~ProductFileMessage();


	/// <summary>Add a ProductFile to the message</summary>
	///
	/// <param name="productFile">The ProductFile to add</param>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given ProductFile is null.
	/// </exception>
    void AddProductFile(ProductFile^ productFile);


	/// <summary>Returns the number of ProductFile objects stored in this message.</summary>
    long GetNumProductFiles();


	/// <summary>Get the ProductFile at the given index.</summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if index specified is not in the range of ProductFiles in the message.
	/// </exception>
    ProductFile^ GetProductFile(long index);


	/// <summary Returns the Response Status associated with this Product File Message.</summary>
	ResponseStatus GetResponseStatus();


	/// <summary Returns the PROD-TYPE value associated with this Product File Message.</summary>
	System::String^ GetProductType();


	/// <summary Returns the PROD-SUBTYPE value associated with this Product File Message.</summary>
	System::String^ GetProductSubtype();


	/// <summary>
	/// Method that allows the callee to get the ProductFileIterator associated with the ProductFileMessage.
	/// This iterator will allow for applications to iterate over the ProductFile objects stored within the %ProductFileMessage.
	/// The iterator is reset each time getProductFileIterator() is called.  The iterator itself is destroyed when the
	/// ProductFileMessage object is destroyed.
	///
	/// Note: Only one ProductFileIterator object is associated with a %ProductFileMessage object; multiple calls to
	/// getProductFileIterator() will return a reference to the same ProductFileIterator object.  Each call will reset the iterator.
	///
    ProductFileIterator^ GetProductFileIterator();


	/// <summary>Constructs and returns a ProductFileMessage from an ordinary message.</summary>
	///
	/// <exception cref="GMSEC_Exception">
	/// An exception is thrown if the given Message is null.
	/// </exception>
    static ProductFileMessage^ ConvertMessage(Message^ message);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ProductFileMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="mistMsg">unmanaged implementation to initialize with</param>
	/// <param name="owned">indicates whether the unmanaged implemention is owned by this object</param>
	ProductFileMessage(gmsec::api::mist::message::ProductFileMessage* productFileMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::message::ProductFileMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::message::ProductFileMessage* productFileMsg, bool owned);
};

} // end namespace MESSAGE
} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
