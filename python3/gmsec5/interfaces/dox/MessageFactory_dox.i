/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::MessageFactory "

    This class provides the public interface for the MessageFactory.

    CONSTRUCTORS

    MessageFactory()

    Basic constructor that initializes the MessageFactory to produce messages based on the latest available message specification.



    MessageFactory(cfg: Config)

    Constructor that initializes the MessageFactory to produce messages based on the information contained within the configuration object (e.g. gmsec-message-specification=201900).

    Parameter
    ---------
    cfg : A Config object of which will be used to construct a Specification object that the MessageFactory will rely on to generate messages.

    Exceptions
    ----------
    A GmsecError is thrown if the configuration cannot be used to create a Specification object.
";


%feature("docstring") gmsec::api5::MessageFactory::setStandardFields "

    set_standard_fields(self, standardFields: FieldList)

    Allows for the setting of common (standard) fields that should be applied to all messages created by the MessageFactory.

    Parameter
    ---------
    standardFields : The list of common/standard fields to apply to all messages.
";


%feature("docstring") gmsec::api5::MessageFactory::clearStandardFields "

    clear_standard_fields(self)

    Removes any common (standard) fields that have previously been associated with the MessageFactory.
";


%feature("docstring") gmsec::api5::MessageFactory::setMessageConfig "

    set_message_config(self, msgConfig: Config)

    Allows for the setting of a common message configuration that will be applied to all messages created by the MessageFactory.

    Parameter
    ---------
    msgConfig : A message configuration object.
";


%feature("docstring") gmsec::api5::MessageFactory::createMessage "

    create_message(self) -> Message

    Creates and returns a Message object. The Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and custom message validator).

    Returns
    -------
    A Message object.



    create_message(self, schemaID: str) -> Message

    Creates and returns a Message object that is populated with the fields derived from the working message specification and schema ID. In addtion, the Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and custom message validator).

    Parameter
    ---------
    schemaID : The string identifying which message to create (e.g. HB).

    Returns
    -------
    A Message object.

    Exception
    ---------
    A GmsecError is thrown if the schemaID string is NULL, contains an empty string, or references an unknown/illegal schema ID.
";


%feature("docstring") gmsec::api5::MessageFactory::fromData "

    from_data(self, data: str, type: int) -> Message

    Creates and returns a Message object based on the given XML or JSON data.

    Parameter
    ---------
    data : An XML or JSON data string that represents a GMSEC-style message.
    type : The type of data being provided

    Returns
    -------
    A Message object.

    Exception
    ---------
    A GmsecError is thrown if the given data string is NULL, or does not represent a GMSEC-style message in either XML or JSON format.
";


%feature("docstring") gmsec::api5::MessageFactory::getSpecification "

    get_specification(self) -> Specification

    Returns a reference to the Specification object associated with the MessageFactory.

    Returns
    -------
    A Specification object.
";


%feature("docstring") gmsec::api5::MessageFactory::registerMessageValidator "

    register_message_validator(self, validator: MessageValidator)

    Registers the given custom MessageValidator with each message created using the MessageFactory.

    Parameter
    ---------
    validator : A custom MessageValidator object.
";
