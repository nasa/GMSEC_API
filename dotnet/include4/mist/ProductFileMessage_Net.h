/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_PRODUCT_FILE_MESSAGE_NET_H
#define GMSEC_API_PRODUCT_FILE_MESSAGE_NET_H


// C# managed
#include <Message_Net.h>

#include <mist/MIST_Defs_Net.h>


// C++ API native
#include <gmsec4/mist/ProductFileMessage.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Config;

namespace MIST
{

// Forward declarations
ref class ProductFile;
ref class ProductFileIterator;


/// <summary>
/// The class defines a GMSEC MSG PROD (Product File) message.
/// </summary>
///
/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.ProductFileMessage instead.


public ref class ProductFileMessage : public Message
{
public:
	/// <summary>Basic constructor</summary>
	///
	/// <param name="subject">Message subject</param>
	/// <param name="status">The value of the RESPONSE-STATUS field to insert into the message.</param>
	/// <param name="productType">The value of the PROD-TYPE field to insert into the message.</param>
	/// <param name="productSubtype">The value of the PROD-SUBTYPE field to insert into the message.</param>
	/// <param name="version">The version of the GMSEC Interface Specification Document (ISD) to be used for
	/// validating the message.</param>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.ProductFileMessage instead.
	ProductFileMessage(System::String^ subject, ResponseStatus status, System::String^ productType,
		               System::String^ productSubtype, unsigned int version);


	/// <summary>Basic constructor</summary>
	///
	/// <param name="subject">Message subject</param>
	/// <param name="config">A configuration to associate with the message</param>
	/// <param name="status">The value of the RESPONSE-STATUS field to insert into the message.</param>
	/// <param name="productType">The value of the PROD-TYPE field to insert into the message.</param>
	/// <param name="productSubtype">The value of the PROD-SUBTYPE field to insert into the message.</param>
	/// <param name="version">The version of the GMSEC Interface Specification Document (ISD) to be used for
	/// validating the message.</param>
	///
	/// <seealso cref="MIST_Defs_Net.h"/>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.ProductFileMessage instead.
	ProductFileMessage(System::String^ subject, Config^ config, ResponseStatus status,
		               System::String^ productType, System::String^ productSubtype, unsigned int version);


	/// <summary>Constructor that initializes the message using an XML or JSON string</summary>
	///
	/// <param name="data">XML or JSON string used to initialize the contents of the message</param>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the data cannot be parsed</exception>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.ProductFileMessage instead.
	ProductFileMessage(System::String^ data);


	/// <summary>Copy-constructor</summary>
	///
	/// <param name="other">The ProductFileMessage object to copy</param>
	///
	/// Note: This class has been deprecated; use GMSEC.API.MIST.MESSAGE.ProductFileMessage instead.
	ProductFileMessage(ProductFileMessage^ other);


	/// <summary>Destructor</summary>
	~ProductFileMessage();


	/// <summary>Returns the value of the RESPONSE-STATUS field associated with the message.</summary>
	ResponseStatus GetResponseStatus();


	/// <summary>Returns the value of the PROD-TYPE field associated with the message.</summary>
	System::String^ GetProductType();


	/// <summary>Returns the value of the PROD-SUBTYPE field associated with the message.</summary>
	System::String^ GetProductSubtype();


	/// <summary>Returns the number of ProductFile objects stored within the message.</summary>
	long GetNumProductFiles();


	/// <summary>Returns a reference to the ProductFile object referenced by the given index.</summary>
	///
	/// <seealso cref="ProductFileMessage::getNumProductFiles"/>
	ProductFile^ GetProductFile(long index);


	/// <summary>Associate a ProductFile object with the message.</summary>
	///
	/// <param name="productFile">The ProductFile object</param>
	void AddProductFile(ProductFile^ productFile);


	/// <summary>
	/// Method that allows the callee to get the ProductFileIterator associated with the %ProductFileMessage.
	/// This iterator will allow for applications to iterate over the ProductFile objects stored within the %ProductFileMessage.
	/// The iterator is reset each time GetProductFileIterator() is called.  The iterator itself is destroyed when the
	/// %ProductFileMessage object is destroyed.
	/// </summary>
	///
	/// <note>
	/// Only one ProductFileIterator object is associated with a %ProductFileMessage object; multiple calls to
	/// GetProductFileIterator() will return a reference to the same ProductFileIterator object.  Each call will reset the iterator.
	/// </note>
	///
	/// <returns>A reference to a ProductFileIterator object.</returns>
	ProductFileIterator^ GetProductFileIterator();


	/// <summary>Constructs and returns a ProductFileMessage from an an ordinary message.</summary>
	static ProductFileMessage^ ConvertMessage(Message^ message);


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ProductFileMessage();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="productFileMessage">unmanaged implementation to initialize with</param>
	ProductFileMessage(gmsec::api::mist::ProductFileMessage* productFileMsg, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::ProductFileMessage* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::ProductFileMessage* productFile, bool owned);
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
