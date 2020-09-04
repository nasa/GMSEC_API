/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileMessage.java
 *
 * @brief The ProductFileMessage object is an extension of MistMessage.
 */

package gov.nasa.gsfc.gmsec.api.mist.message;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.mist.gmsecMIST;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.ProductFileIterator;
import gov.nasa.gsfc.gmsec.api.mist.Specification;

import gov.nasa.gsfc.gmsec.api.jni.JNIMessage;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIProductFile;
import gov.nasa.gsfc.gmsec.api.jni.mist.JNISpecification;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;
import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIProductFileMessage;


/**
 * @class ProductFileMessage
 *
 * @brief The ProductFileMessage object is an extension of Message, and is used to define a GMSEC MSG PROD message.
 *
 * @sa Message
 * @sa MistMessage
 * @sa Config
 * @sa Specification 
 * @sa Field
 * @sa MessageFieldIterator
 */
public class ProductFileMessage extends MistMessage
{
	public ProductFileMessage(JNIProductFileMessage jProdFileMsg)
	{
		super(jProdFileMsg);
	}


	/**
	 * @fn ProductFileMessage(String subject,
	 *                        gmsecMIST.ResponseStatus responseStatus,
	 *                        Message.MessageKind kind,
	 *                        String productType,
	 *                        String productSubtype,
	 *                        Specification spec) throws IllegalArgumentException
	 *
	 * @brief Default constructor - Initializes the message instance
	 *
	 * @param subject        - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param kind           - the kind of message to instantiate.
	 * @param productType    - PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype - PROD-SUBTYPE field to insert into a MSG PROD message.
	 * @param spec           - A reference to the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 */
	public ProductFileMessage(String subject,
	                          gmsecMIST.ResponseStatus responseStatus,
	                          Message.MessageKind kind,
	                          String productType,
	                          String productSubtype,
	                          Specification spec) throws IllegalArgumentException
	{
		super((JNIProductFileMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (productType == null || productType.isEmpty())
		{
			throw new IllegalArgumentException("Product Type cannot be null, nor contain an empty string");
		}
		if (productSubtype == null || productSubtype.isEmpty())
		{
			throw new IllegalArgumentException("Product Subtype cannot be null, nor contain an empty string");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIProductFileMessage(subject, responseStatus, kind, productType, productSubtype, spec));
	}


   /**
	 * @fn ProductFileMessage(String subject,
	 *                        gmsecMIST.ResponseStatus responseStatus,
	 *                        Message.MessageKind kind,
	 *                        String productType,
	 *                        String productSubtype,
	 *                        Config config,
	 *                        Specification spec) throws IllegalArgumentException
     *
     * @brief constructor - Initializes the message instance and associates a Configuration object
     *
	 * @param subject        - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param kind           - the kind of message to instantiate.
	 * @param productType    - PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype - PROD-SUBTYPE field to insert into a MSG PROD message.
     * @param config         - A configuration to associate with the message.
	 * @param spec           - A reference to the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the %Config object is null.
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 */
	public ProductFileMessage(String subject,
	                          gmsecMIST.ResponseStatus responseStatus,
	                          Message.MessageKind kind,
	                          String productType,
	                          String productSubtype,
	                          Config config,
	                          Specification spec) throws IllegalArgumentException
	{
		super((JNIProductFileMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (productType == null || productType.isEmpty())
		{
			throw new IllegalArgumentException("Product Type cannot be null, nor contain an empty string");
		}
		if (productSubtype == null || productSubtype.isEmpty())
		{
			throw new IllegalArgumentException("Product Subtype cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIProductFileMessage(subject, responseStatus, kind, productType, productSubtype, config, spec));
	}


	/**
	 * @fn ProductFileMessage(String subject,
	 *                        gmsecMIST.ResponseStatus responseStatus,
	 *                        String schemaID,
	 *                        Specification spec)
	 *
	 * @brief constructor - Initializes the message with a given schema ID
	 *
	 * @param subject        - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param schemaID       - the string used to identify the message schema in the GMSEC ISD.
	 * @param spec           - A reference to the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the schema ID string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 */
	public ProductFileMessage(String subject,
	                          gmsecMIST.ResponseStatus responseStatus,
	                          String schemaID,
	                          Specification spec)
	{
		super((JNIProductFileMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Schema ID cannot be null, nor contain an empty string");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIProductFileMessage(subject, responseStatus, schemaID, spec));
	}


	/**
	 * @fn ProductFileMessage(String subject,
	 *                        gmsecMIST.ResponseStatus responseStatus,
	 *                        String schemaID,
	 *                        Config config,
	 *                        Specification spec)
	 *
	 * @brief constructor - Initializes the message with a given schema ID
	 *
	 * @param subject        - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param schemaID       - the string used to identify the message schema in the GMSEC ISD.
     * @param config         - A configuration to associate with the message.
	 * @param spec           - A reference to the specification this message's schema will adhere to.
	 *
	 * @throws An IllegalArgumentException is thrown if the subject string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the schema ID string is null or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the %Specification object is null.
	 */
	public ProductFileMessage(String subject,
	                          gmsecMIST.ResponseStatus responseStatus,
	                          String schemaID,
	                          Config config,
	                          Specification spec)
	{
		super((JNIProductFileMessage) null);

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (schemaID == null || schemaID.isEmpty())
		{
			throw new IllegalArgumentException("Schema ID cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}
		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}

		registerChild(new JNIProductFileMessage(subject, responseStatus, schemaID, config, spec));
	}


	/**
	 * @fn ProductFileMessage(String data)
	 *
	 * @brief Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data - the XML or JSON string that represents a GMSEC message.
	 *
	 * @throws An IllegalArgumentException is thrown if the data string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the data string cannot be parsed.
	 */
	public ProductFileMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIProductFileMessage) null);

		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIProductFileMessage(data));
	}


	/**
	 * @fn ProductFileMessage(ProductFileMessage other)
	 *
	 * @brief Copy constructor that initializes the message instance using another ProductFileMessage object.
	 *
	 * @param other - the other ProductFileMessage object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the product file message object is null.
	 */
	public ProductFileMessage(ProductFileMessage other) throws IllegalArgumentException
	{
		super((JNIProductFileMessage) null);

		if (other == null)
		{
			throw new IllegalArgumentException("ProductFileMessage cannot be null");
		}

		registerChild(new JNIProductFileMessage((JNIProductFileMessage) MistMessage.getInternal(other)));
	}


	/**
	 * @fn void addProductFile(ProductFile productFile)
	 *
	 * @brief Adds the given ProductFile object to the internal list of product files maintained by the message.
	 *
	 * @param productFile - the ProductFile object to add to the message.
	 *
	 * @throws An IllegalArgumentException is thrown if the given ProductFile object is null.
	 */
	public void addProductFile(ProductFile productFile) throws IllegalArgumentException
	{
		if (productFile == null)
		{
			throw new IllegalArgumentException("ProductFile is null");
		}

		((JNIProductFileMessage) MistMessage.getInternal(this)).addProductFile(ProductFile.getInternal(productFile));
	}


	/**
	 * @fn int getNumProductFiles()
	 *
	 * @brief Returns the number of ProductFile objects associated with this ProductFileMessage.
	 *
	 * @return The number of product files objects.
	 *
	 * @sa getProductFile()
	 */
	public int getNumProductFiles()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getNumProductFiles();
	}


	/**
	 * @fn ProductFile getProductFile(int index)
	 *
	 * @brief Returns ProductFile object at specified index.
	 *
	 * @return A ProductFile object.
	 *
	 * @throws A GMSEC_Exception is thrown if the specified index is outside the number of product files in the message.
	 *
	 * @sa getNumProductFiles()
	 */
	public ProductFile getProductFile(int index) throws GMSEC_Exception
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductFile(index);
	}


	/**
	 * @fn gmsecMIST.ResponseStatus getResponseStatus()
	 *
	 * @brief Get the response status of this ProductFile message.
	 */
	public gmsecMIST.ResponseStatus getResponseStatus()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getResponseStatus();
	}


	/**
	 * @fn String getProductType()
	 *
	 * @brief Get the PROD-TYPE of this ProductFile message.
	 */
	public String getProductType()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductType();
	}


	/**
	 * @fn String getProductSubtype()
	 *
	 * @brief Get the PROD-SUBTYPE of this ProductFile message.
	 */
	public String getProductSubtype()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductSubtype();
	}


	/**
	 * @fn ProductFileIterator getProductFileIterator()
	 *
	 * @brief Method that allows the callee to get the ProductFileIterator associated with the %ProductFileMessage.
	 * This iterator will allow for applications to iterate over the ProductFile objects stored within the %ProductFileMessage.
	 * The iterator is reset each time getProductFileIterator() is called.  The iterator itself is destroyed when the
	 * %ProductFileMessage object is destroyed.
	 *
	 * @note Only one ProductFileIterator object is associated with a %ProductFileMessage object; multiple calls to
	 * getProductFileIterator() will return a reference to the same ProductFileIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a ProductFileIterator object.
	 */
	public ProductFileIterator getProductFileIterator()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductFileIterator();
	}


	/**
	 * @fn ProductFileMessage convertMessage(Message msg)
	 *
	 * @brief Constructs and returns a ProductFileMessage that is created using the given Message.
	 *
	 * @return A ProductFileMessage object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message object is null.
	 */
	public static ProductFileMessage convertMessage(Message msg) throws IllegalArgumentException
	{
		if (msg == null)
		{
			throw new IllegalArgumentException("Message is null");
		}

		return JNIProductFileMessage.convertMessage(Message.getInternal(msg));
	}
}
