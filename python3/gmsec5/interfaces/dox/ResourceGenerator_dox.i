/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::ResourceGenerator "

    This class can be used to start a thread that will continuously publish Resource (RSRC) messages.

    See Also
    --------
    Config

    CONSTRUCTORS:

    ResourceGenerator(self, config: Config, pubRate: int, sampleInterval: int, averageInterval: int)

    Basic class constructor.

    Parameters
    ----------
    config          : The Config used to configure the Connection used to publish messages.
    pubRate         : The message publish rate (in seconds).
    sampleInterval  : The time distance between sample collection (in seconds)
    averageInterval : The window over which to make a moving average of samples (in seconds)

    Exceptions
    ----------
    A GmsecError is thrown if a Connection object cannot be created.

    Notes
    ----------
    If the publish rate is set to 0 (zero), the resource generator will only publish one message.

    See also
    ----------
    set_field()



    ResourceGenerator(self, config: Config, pubRate: int, sampleInterval: int, averageInterval: int, fieldList: FieldList)

    Class constructor where a list of message fields can be provided.

    Parameters
    ----------
    config          : The Config used to configure the Connection used to publish messages.
    pubRate         : The message publish rate (in seconds).
    sampleInterval  : The time distance between sample collection (in seconds)
    averageInterval : The window over which to make a moving average of samples (in seconds)
    fieldList       : FieldList of fields that should be added to the resource message.

    Exceptions
    ----------
    A GmsecError is thrown if a Connection object cannot be created.

    Note
    ----------
    If the publish rate is set to 0 (zero), the resource generator will only publish one message.

    See also
    ----------
    set_field()
";


%feature("docstring") gmsec::api5::ResourceGenerator::start "

    start(self) -> bool

    Starts the resource generator thread.

    Returns
    ----------
    True if the resource generator thread has started; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if a connection cannot be established with the GMSEC Bus.
    A GmsecError is thrown if message validation is enabled and a valid RSRC message cannot be created.

    Notes
    ----------
    If the publish rate is set to 0 (zero), only one resource message will be published,
    however the resource generator thread will continue to run.

    See also
    ----------
    stop()
";


%feature("docstring") gmsec::api5::ResourceGenerator::stop "

    stop(self) -> bool

    Stops the resource generator thread.

    Returns
    ----------
    True if the resource generator thread has stopped; False otherwise.

    See also
    ----------
    start()
";


%feature("docstring") gmsec::api5::ResourceGenerator::isRunning "

    isRunning(self) -> bool

    Accessor that can be used to query whether the resource generator thread is running.

    Returns
    ----------
    True if the resource generator thread is running; False otherwise.
";


%feature("docstring") gmsec::api5::ResourceGenerator::setField "

    set_field(self, field: Field) -> bool

    Accessor that will add the given field to resource message.

    Parameters
    ----------
    field : The Field to add to the resource message.

    Returns
    ----------
    True if an existing field was overwritten; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if for whatever reason the field cannot be added to the message.

    Note
    ----------
    If the PUB-RATE field is provided, then the publish rate for the resource generator will
    be updated to publish at the (new) rate.
";



%feature("docstring") gmsec::api5::ResourceGenerator::createResourceMessage "

    create_resource_message(msgFactory: MessageFactory, sampleInterval: int, averageInterval: int) -> Message

    Creates/returns a Resource Message using the given MessageFactory.

    Parameters
    ----------
    msgFactory      : The MessageFactory to reference when creating the resource message.
    sampleInterval  : The time distance between sample collection (in seconds).
    averageInterval : Time window over which to make a moving average of samples (in seconds).

    Returns
    ----------
    A Message object representing a Resource Message.

    Exceptions
    ----------
    A GmsecError is thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval.
";
