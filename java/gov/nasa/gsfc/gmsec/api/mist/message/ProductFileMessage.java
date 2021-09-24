/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileMessage.java
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
 * The ProductFileMessage object is an extension of Message, and is used to define a GMSEC MSG PROD message.
 *
 * @see Message
 * @see MistMessage
 * @see Config
 * @see Specification 
 * @see gov.nasa.gsfc.gmsec.api.field.Field
 * @see gov.nasa.gsfc.gmsec.api.MessageFieldIterator
 */
public class ProductFileMessage extends MistMessage
{
	public ProductFileMessage(JNIProductFileMessage jProdFileMsg)
	{
		super(jProdFileMsg);
	}


	/**
	 * Default constructor - Initializes the message instance
	 *
	 * @param subject        The subject string for the message.
	 * @param responseStatus RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param kind           The kind of message to instantiate.
	 * @param productType    PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype PROD-SUBTYPE field to insert into a MSG PROD message.
	 * @param spec           A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
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
     * Constructor - Initializes the message instance and associates a Configuration object
     *
	 * @param subject        The subject string for the message.
	 * @param responseStatus RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param kind           The kind of message to instantiate.
	 * @param productType    PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype PROD-SUBTYPE field to insert into a MSG PROD message.
     * @param config         A configuration to associate with the message.
	 * @param spec           A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Config object is null.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
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
	 * Constructor - Initializes the message with a given schema ID
	 *
	 * @param subject        The subject string for the message.
	 * @param responseStatus RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param schemaID       The string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.PROD)
	 * Users may also use the shorthand notation of messageKind.messageType.messageSubType (e.g. MSG.PROD)
	 * @param spec           A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the schema ID string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
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
	 * Constructor - Initializes the message with a given schema ID
	 *
	 * @param subject        The subject string for the message.
	 * @param responseStatus RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param schemaID       The string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.PROD)
	 * Users may also use the shorthand notation of messageKind.messageType.messageSubType (e.g. MSG.PROD)
     * @param config         A configuration to associate with the message.
	 * @param spec           A reference to the specification this message's schema will adhere to.
	 *
	 * @throws IllegalArgumentException Thrown if the subject string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the schema ID string is null or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the %Specification object is null.
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
	 * Specialized constructor that initializes the message instance using either XML or JSON text.
	 *
	 * @param data The XML or JSON string that represents a GMSEC message.
	 *
	 * @throws IllegalArgumentException Thrown if the data string is null or contains an empty string.
	 * @throws GMSEC_Exception Thrown if the data string cannot be parsed.
	 *
	 * @deprecated This constructor has been deprecated; use ProductFileMessage(Specification, String) instead.
	 */
	@Deprecated
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
	 * Initializes a ProductFileMessage using an XML or JSON string, and ensures the message adheres
	 * to the provided specification.
	 *
	 * @param spec The Specification this message's schema will adhere to.
	 * @param data XML or JSON string used to initialize the message
	 *
	 * @throws IllegalArgumentException Thrown if the Specification is null.
	 * @throws IllegalArgumentException Thrown if the data string is null.
	 * @throws GMSEC_Exception Thrown if data string contains invalid an XML or JSON statement.
	 *
	 * @see Message#toXML()
	 * @see Message#toJSON()
	 */
	public ProductFileMessage(Specification spec, String data)
		throws IllegalArgumentException, GMSEC_Exception
	{
		super((JNIMistMessage) null);

		if (spec == null)
		{
			throw new IllegalArgumentException("Specification cannot be null");
		}
		if (data == null || data.isEmpty())
		{
			throw new IllegalArgumentException("Data cannot be null, nor contain an empty string");
		}

		registerChild(new JNIProductFileMessage(Specification.getInternal(spec), data));
	}


	/**
	 * Copy constructor that initializes the message instance using another ProductFileMessage object.
	 *
	 * @param other The other ProductFileMessage object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the product file message object is null.
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
	 * Adds the given ProductFile object to the internal list of product files maintained by the message.
	 *
	 * @param productFile The ProductFile object to add to the message.
	 *
	 * @throws IllegalArgumentException Thrown if the given ProductFile object is null.
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
	 * Returns the number of ProductFile objects associated with this ProductFileMessage.
	 *
	 * @return The number of product files objects.
	 *
	 * @see ProductFileMessage#getProductFile(int)
	 */
	public int getNumProductFiles()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getNumProductFiles();
	}


	/**
	 * Returns ProductFile object at specified index.
	 *
	 * @param index The index of the product file to retrieve.
	 *
	 * @return A ProductFile object.
	 *
	 * @throws GMSEC_Exception Thrown if the specified index is outside the number of product files in the message.
	 *
	 * @see ProductFileMessage#getNumProductFiles()
	 *
	 * @deprecated This method has been deprecated; use ProductFileIterator instead.
	 */
	@Deprecated
	public ProductFile getProductFile(int index) throws GMSEC_Exception
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductFile(index);
	}


	/**
	 * Get the response status of this ProductFile message.
	 *
	 * @return A ResponseStatus.
	 */
	public gmsecMIST.ResponseStatus getResponseStatus()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getResponseStatus();
	}


	/**
	 * Get the PROD-TYPE of this ProductFile message.
	 *
	 * @return A product type string.
	 */
	public String getProductType()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductType();
	}


	/**
	 * Get the PROD-SUBTYPE of this ProductFile message.
	 *
	 * @return A product subtype string.
	 */
	public String getProductSubtype()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductSubtype();
	}


	/**
	 * Method that allows the callee to get the ProductFileIterator associated with the ProductFileMessage.
	 * This iterator will allow for applications to iterate over the ProductFile objects stored within the ProductFileMessage.
	 * The iterator is reset each time getProductFileIterator() is called. The iterator itself is destroyed when the
	 * ProductFileMessage object is destroyed.
	 * <p>
	 * Note: Only one ProductFileIterator object is associated with a ProductFileMessage object; multiple calls to
	 * getProductFileIterator() will return a reference to the same ProductFileIterator object. Each call will reset the iterator.
	 *
	 * @return A reference to a ProductFileIterator object.
	 */
	public ProductFileIterator getProductFileIterator()
	{
		return ((JNIProductFileMessage) MistMessage.getInternal(this)).getProductFileIterator();
	}


	/**
	 * Constructs and returns a ProductFileMessage that is created using the given Message.
	 *
	 * @param msg The Message to convert.
	 *
	 * @return A ProductFileMessage object.
	 *
	 * @throws IllegalArgumentException Thrown if the given Message object is null.
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
