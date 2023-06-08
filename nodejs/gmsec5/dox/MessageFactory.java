/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFactory.java
 */

package gmsec;


/**
 * The MessageFactory is used to create Message objects that are based on C2MS message templates.
 */
public class MessageFactory
{
	/**
	 * Creates %MessageFactory instance using the latest/current message specification.
	 *
	 * @return A new MessageFactory object.
	 *
	 * @throws GmsecException Thrown if the latest message templates cannot be loaded.
	 */
	public MessageFactory();


	/**
	 * Creates %MessageFactory instance using the given configuration.
	 *
	 * @param config Config object to customize the %MessageFactory, typically for alternative message specification.
	 *
	 * @return A new MessageFactory object.
	 *
	 * @throws SWIG_Exception Thrown if the Config is null.
	 * @throws GmsecException Thrown if the message templates cannot be loaded.
	 */
	public MessageFactory(config);


	/**
	 * Sets the internal list of fields which are to be automatically placed in all
	 * Message objects that are created using the %MessageFactory. Internal copies of
	 * the Fields are made, and thus ownership of the fields that are provided are not
	 * retained by %MessageFactory.
	 *
	 * @param standardFields A FieldArray of Field objects which to include with all
	 * Message objects that are created by the factory.
	 */
	public setStandardFields(standardFields);


	/**
	 * Destroys the lists of standard fields that are included with Message objects
	 * created using the MessageFactory.
	 */
	public clearStandardFields();


	/**
	 * Allows for the setting of a common message configuration that should be
	 * applied to all messages created by the %MessageFactory.
	 * <p>
	 * Note: Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 *
	 * @param msgConfig The message Config object.
	 *
	 * @throws SWIG_Exception Thrown if the given Config object is null.
	 */
	public setMessageConfig(msgConfig);


	/**
	 * Creates and returns a %Message object. The %Message will include any user-supplied
	 * message attributes (such as a message configuration and standard fields).
	 *
	 * @return A Message object is returned.
	 *
	 * @see setMessageConfig
	 * @see setStandardFields
	 */
	public createMessage();


	/**
	 * Creates and returns a Message object that is populated with fields derived
	 * from the working message specification and schema ID. In addition, the
	 * %Message object will include any user-supplied message attributes (such as a
	 * message configuration and standard fields).
	 *
	 * @param schemaID The string identifying which message to create (e.g. HB).
	 *
	 * @return A Message object is returned.
	 *
	 * @throws SWIG_Exception Thrown if the schemaID is null.
	 * @throws GmsecException Thrown if the schemaID contains an empty string,
	 * or references an unknown/illegal schema ID.
	 *
	 * @see setMessageConfig
	 * @see setStandardFields
	 */
	public createMessage(schemaID);


	/**
	 * Creates and returns a Message object based on the given XML or JSON data.
	 *
	 * @param data     An XML or JSON data string that represents a GMSEC-style message.
	 * @param dataType The type of data being provided.
	 *
	 * @return A Message object that is populated using the given data is returned.
	 *
	 * @throws SWIG_Exception Thrown if the data string is null.
	 * @throws GmsecException Thrown if the given data string is not parsable as a
	 * GMSEC-style message in either XML or JSON format.
	 */
	public fromData(data, dataType);


	/**
	 * Returns the Specification object associated with the %MessageFactory.
	 *
	 * @return A Specification object.
	 */
	public getSpecification();
}
