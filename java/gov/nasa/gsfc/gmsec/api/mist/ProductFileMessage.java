/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileMessage.java
 *
 * @brief The ProductFileMessage object is an extension of Message, and serves as a
 * container for a GMSEC C2CX %ProductFile %Message.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.jni.JNIProductFileMessage;


/**
 * @class ProductFileMessage
 *
 * @brief The ProductFileMessage object is an extension of Message, and serves as a container for a GMSEC C2CX %ProductFile %Message.
 * The methods of this class allow for the construction and manipulation of the data within the message.
 */
public class ProductFileMessage extends Message
{
	private JNIProductFileMessage m_jniProductFileMessage = null;


	public static JNIProductFileMessage getInternal(ProductFileMessage msg)
	{
		return (msg == null ? null : msg.m_jniProductFileMessage);
	}


	public ProductFileMessage(JNIProductFileMessage jProdFileMsg)
	{
		super(jProdFileMsg);

		m_jniProductFileMessage = jProdFileMsg;
	}


	/**
	 * @fn ProductFileMessage(String subject, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
	 *
	 * @brief Specialized constructor where a GMSEC Interface Specification Document version number can be specified.
	 *
	 * @param subject        - the GMSEC subject to associate with the message.
	 * @param status         - the value for the RESPONSE-STATUS field of the Product File %Message.
	 * @param productType    - the string value for the PROD-TYPE field of the Product File %Message.
	 * @param productSubtype - the string value for the PROD-SUBTYPE field of the Product File %Message.
	 * @param version        - the version of the GMSEC ISD to be used.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given product type string is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given product subtype string is null, or contains an empty string.
	 */
	public ProductFileMessage(String subject, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
		throws IllegalArgumentException
	{
		super(subject, Message.MessageKind.PUBLISH);

		if (productType == null || productType.isEmpty())
		{
			throw new IllegalArgumentException("Product type is null or contains an empty string");
		}
		if (productSubtype == null || productSubtype.isEmpty())
		{
			throw new IllegalArgumentException("Product subtype is null or contains an empty string");
		}

		m_jniProductFileMessage = new JNIProductFileMessage(subject, status, productType, productSubtype, version);

		registerChild(m_jniProductFileMessage);
	}


	/**
	 * @fn ProductFileMessage(String subject, Config config, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
	 *
	 * @brief Specialized constructor that accepts a Config object, and where a GMSEC Interface Specification Document
	 * version number can be speficied.
	 *
	 * @param subject        - the GMSEC subject to associate with the message.
	 * @param config         - a configuration object that contains message-specific options.
	 * @param status         - the value for the RESPONSE-STATUS field of the Product File %Message.
	 * @param productType    - the string value for the PROD-TYPE field of the Product File %Message.
	 * @param productSubtype - the string value for the PROD-SUBTYPE field of the Product File %Message.
	 * @param version        - the version of the GMSEC ISD to be used.
	 *
	 * @throws An IllegalArgumentException is thrown if the given subject string is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given Config is null.
	 * @throws An IllegalArgumentException is thrown if the given product type string is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the given product subtype string is null, or contains an empty string.
	 */
	public ProductFileMessage(String subject, Config config, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
		throws IllegalArgumentException
	{
		super(subject, Message.MessageKind.PUBLISH);

		if (config == null)
		{
			throw new IllegalArgumentException("Config is null");
		}
		if (productType == null || productType.isEmpty())
		{
			throw new IllegalArgumentException("Product type is null or contains an empty string");
		}
		if (productSubtype == null || productSubtype.isEmpty())
		{
			throw new IllegalArgumentException("Product subtype is null or contains an empty string");
		}

		m_jniProductFileMessage = new JNIProductFileMessage(subject, Config.getInternal(config), status, productType, productSubtype, version);

		registerChild(m_jniProductFileMessage);
	}


	/**
	 * @fn ProductFileMessage(String data)
	 *
	 * @brief Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data - the XML or JSON string that represents a GMSEC message.
	 *
	 * @throws An IllegalArgumentException is thrown if the given data string is null or contains an empty string.
	 * @throws A GMSEC_Exception is thrown if the data string cannot be parsed.
	 */
	public ProductFileMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super(data);

		m_jniProductFileMessage = new JNIProductFileMessage(data);

		registerChild(m_jniProductFileMessage);
	}


	/**
	 * @fn ProductFileMessage(ProductFileMessage other)
	 *
	 * @brief Copy constructor that initializes the message instance using another ProductFileMessage object.
	 *
	 * @param other - the other ProductFileMessage object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given ProductFileMessage is null.
	 */
	public ProductFileMessage(ProductFileMessage other) throws IllegalArgumentException
	{
		super(other);

		m_jniProductFileMessage = new JNIProductFileMessage(ProductFileMessage.getInternal(other));

		registerChild(m_jniProductFileMessage);
	}


	/**
	 * @fn void addProductFile(ProductFile productFile)
	 *
	 * @brief Adds the given ProductFile to the list of ProductFiles associated with this message.
	 *
	 * @param productFile - the ProductFile object to add to the message.
	 *
	 * @throws An IllegalArgumentException is thrown if the given ProductFile is null.
	 */
	public void addProductFile(ProductFile productFile) throws IllegalArgumentException
	{
		if (productFile == null)
		{
			throw new IllegalArgumentException("ProductFile is null");
		}

		m_jniProductFileMessage.addProductFile(ProductFile.getInternal(productFile));
	}


	/**
	 * @fn ProductFile getProductFile(int index)
	 *
	 * @brief Returns ProductFile object at specified index.
	 *
	 * @return A ProductFile object.
	 *
	 * @throws A GMSEC_Exception is thrown if the specified index is outside the number of ProductFiles in the message.
	 *
	 * @sa getNumProductFiles()
	 */
	public ProductFile getProductFile(int index) throws GMSEC_Exception
	{
		return m_jniProductFileMessage.getProductFile(index);
	}


	/**
	 * @fn int getNumProductFiles()
	 *
	 * @brief Returns the number of ProductFile objects associated with this ProductFileMessage.
	 *
	 * @return The number of ProductFile objects.
	 *
	 * @sa getProductFile()
	 */
	public int getNumProductFiles()
	{
		return m_jniProductFileMessage.getNumProductFiles();
	}


	/**
	 * @fn gmsecMIST.ResponseStatus getResponseStatus()
	 *
	 * @return Returns the enumerated value contained within the message's RESPONSE-STATUS field.
	 */
	public gmsecMIST.ResponseStatus getResponseStatus()
	{
		return m_jniProductFileMessage.getResponseStatus();
	}


	/**
	 * @fn String getProductType()
	 *
	 * @return Returns the product type string contained within the message's PROD-TYPE field.
	 */
	public String getProductType()
	{
		return m_jniProductFileMessage.getProductType();
	}


	/**
	 * @fn String getProductSubtype()
	 *
	 * @return Returns the product subtype string contained within the message's PROD-SUBTYPE field.
	 */
	public String getProductSubtype()
	{
		return m_jniProductFileMessage.getProductSubtype();
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
		return m_jniProductFileMessage.getProductFileIterator();
	}


	/**
	 * @fn ProductFileMessage convertMessage(Message msg)
	 *
	 * @brief Constructs and returns a ProductFileMessage that is created using the given Message.
	 *
	 * @return A ProductFileMessage object.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Message is null.
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
