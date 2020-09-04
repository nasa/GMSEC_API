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
 *
 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.Config;
import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;
import gov.nasa.gsfc.gmsec.api.Message;

import gov.nasa.gsfc.gmsec.api.util.Log;

import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIMistMessage;
import gov.nasa.gsfc.gmsec.api.jni.mist.message.JNIProductFileMessage;


/**
 * @class ProductFileMessage
 *
 * @brief The ProductFileMessage object is an extension of Message, and serves as a container for a GMSEC C2CX %ProductFile %Message.
 * The methods of this class allow for the construction and manipulation of the data within the message.
 *
 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.
 */
public class ProductFileMessage extends Message
{
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
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.
	 */
	public ProductFileMessage(String subject, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
		throws IllegalArgumentException
	{
		super((JNIProductFileMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.");

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

		registerChild(new JNIProductFileMessage(subject, status, Message.MessageKind.PUBLISH, productType, productSubtype, JNIMistMessage.buildSpecification(version)));
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
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.
	 */
	public ProductFileMessage(String subject, Config config, gmsecMIST.ResponseStatus status, String productType, String productSubtype, int version)
		throws IllegalArgumentException
	{
		super((JNIProductFileMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.");

		if (subject == null || subject.isEmpty())
		{
			throw new IllegalArgumentException("Subject cannot be null, nor contain an empty string");
		}
		if (config == null)
		{
			throw new IllegalArgumentException("Config cannot be null");
		}
		if (productType == null || productType.isEmpty())
		{
			throw new IllegalArgumentException("Product Type cannot be null, nor contain an empty string");
		}
		if (productSubtype == null || productSubtype.isEmpty())
		{
			throw new IllegalArgumentException("Product Subtype cannot be null, nor contain an empty string");
		}

		registerChild(new JNIProductFileMessage(subject, status, Message.MessageKind.PUBLISH, productType, productSubtype, config, JNIMistMessage.buildSpecification(version)));
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
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.
	 */
	public ProductFileMessage(String data) throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIProductFileMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.");

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
	 * @throws An IllegalArgumentException is thrown if the given ProductFileMessage is null.
	 *
	 * @note This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.
	 */
	public ProductFileMessage(ProductFileMessage other) throws IllegalArgumentException
	{
		super((JNIProductFileMessage) null);

		Log.warning("This class has been deprecated; use gov.nasa.gsfc.gmsec.api.mist.message.ProductFileMessage instead.");

		if (other == null)
		{
			throw new IllegalArgumentException("ProductFileMessage cannot be null");
		}

		registerChild(new JNIProductFileMessage((JNIProductFileMessage) getInternal(other)));
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

		((JNIProductFileMessage) getInternal(this)).addProductFile(ProductFile.getInternal(productFile));
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
		return ((JNIProductFileMessage) getInternal(this)).getProductFile(index);
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
		return ((JNIProductFileMessage) getInternal(this)).getNumProductFiles();
	}


	/**
	 * @fn gmsecMIST.ResponseStatus getResponseStatus()
	 *
	 * @return Returns the enumerated value contained within the message's RESPONSE-STATUS field.
	 */
	public gmsecMIST.ResponseStatus getResponseStatus()
	{
		return ((JNIProductFileMessage) getInternal(this)).getResponseStatus();
	}


	/**
	 * @fn String getProductType()
	 *
	 * @return Returns the product type string contained within the message's PROD-TYPE field.
	 */
	public String getProductType()
	{
		return ((JNIProductFileMessage) getInternal(this)).getProductType();
	}


	/**
	 * @fn String getProductSubtype()
	 *
	 * @return Returns the product subtype string contained within the message's PROD-SUBTYPE field.
	 */
	public String getProductSubtype()
	{
		return ((JNIProductFileMessage) getInternal(this)).getProductSubtype();
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
		return ((JNIProductFileMessage) getInternal(this)).getProductFileIterator();
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

        ProductFileMessage prodFileMsg = null;

        try
        {
            prodFileMsg = new ProductFileMessage(msg.toXML());
        }
        catch (GMSEC_Exception e)
        {
            throw new IllegalArgumentException(e.toString());
        }

        return prodFileMsg;
	}
}
