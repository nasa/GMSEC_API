/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::HeartbeatGenerator "

    This class can be used to start a thread that will continuously publish Heartbeat (HB) messages.

    See Also
    --------
    Config

    CONSTRUCTORS:

    HeartbeatGenerator(self, config: Config, hbPubRate: int)

    Basic class constructor.

    Parameters
    ----------
    config    : The Config used to configure the Connection used to publish messages.
    hbPubRate : The message publish rate (in seconds).

    Exceptions
    ----------
    A GmsecError is thrown if a Connection object cannot be created.

    Notes
    ----------
    If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.

    See also
    ----------
    set_field()



    HeartbeatGenerator(self, config: Config, hbPubRate: int, fieldList: FieldList)

    Class constructor where a list of message fields can be provided.

    Parameters
    ----------
    config    : The Config used to configure the Connection used to publish messages.
    hbPubRate : The message publish rate (in seconds).
    fieldList : FieldList of fields that should be added to the heartbeat message.

    Exceptions
    ----------
    A GmsecError is thrown if a Connection object cannot be created.

    Note
    ----------
    If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.

    See also
    ----------
    set_field()
";


%feature("docstring") gmsec::api5::HeartbeatGenerator::start "

    start(self) -> bool

    Starts the heartbeat generator thread.

    Returns
    ----------
    True if the heartbeat generator thread has started; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if a connection cannot be established with the GMSEC Bus.
    A GmsecError is thrown if message validation is enabled and a valid HB message cannot be created.

    Notes
    ----------
    If the publish rate is set to 0 (zero), only one heartbeat message will be published,
    however the heartbeat generator thread will continue to run.

    See also
    ----------
    stop()
";


%feature("docstring") gmsec::api5::HeartbeatGenerator::stop "

    stop(self) -> bool

    Stops the heartbeat generator thread.

    Returns
    ----------
    True if the heartbeat generator thread has stopped; False otherwise.

    See also
    ----------
    start()
";


%feature("docstring") gmsec::api5::HeartbeatGenerator::isRunning "

    isRunning(self) -> bool

    Accessor that can be used to query whether the heartbeat generator thread is running.

    Returns
    ----------
    True if the heartbeat generator thread is running; False otherwise.
";


%feature("docstring") gmsec::api5::HeartbeatGenerator::changePublishRate "

    changePublishRate(self, pubRate: unsigned int) -> void

    Accessor for changing the publish rate of the heartbeat message generator.

    Parameters
    ----------
    pubRate : The new publish rate (in seconds)

    Note
    ----------
    If the publish rate is set to 0 (zero), only one additional heartbeat message will be published.
";


%feature("docstring") gmsec::api5::HeartbeatGenerator::setField "

    set_field(self, field: Field) -> bool

    Accessor that will add the given field to heartbeat message.

    Parameters
    ----------
    field : The Field to add to the heartbeat message.

    Returns
    ----------
    True if an existing field was overwritten; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if for whatever reason the field cannot be added to the message.

    Note
    ----------
    If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will
    be updated to publish at the (new) rate.



    set_field(self, fieldName: str, fieldValue: GMSEC_I64) -> bool

    Accessor that will smartly add a Field to heartbeat message.

    Parameters
    ----------
    fieldName  : The name of the field.
    fieldValue : The signed 64-bit value for the field.

    Returns
    ----------
    True if an existing field was overwritten; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is null or contains an empty string.
    A GmsecError is thrown if for whatever reason the field cannot be added to the message.

    Notes
    ----------
    1. This method relies on the usage of Message.set_value(str, GMSEC_I64).
    2. If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will
       be updated to publish at the (new) rate.



    set_field(self, fieldName: str, fieldValue: GMSEC_F64) -> bool

    Accessor that will smartly add a Field to heartbeat message.

    Parameters
    ----------
    fieldName  : The name of the field.
    fieldValue : The 64-bit floating point value for the field.

    Returns
    ----------
    True if an existing field was overwritten; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is null or contains an empty string.
    A GmsecError is thrown if for whatever reason the field cannot be added to the message.

    Notes
    ----------
    1. This method relies on the usage of Message.set_value(str, GMSEC_F64).
    2. If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will
       be updated to publish at the (new) rate.



    set_field(self, fieldName: str, fieldValue: str) -> bool

    Accessor that will smartly add a Field to heartbeat message.

    Parameters
    ----------
    fieldName  : The name of the field.
    fieldValue : The string value for the field.

    Returns
    ----------
    True if an existing field was overwritten; False otherwise.

    Exceptions
    ----------
    A GmsecError is thrown if the field name is null or contains an empty string.
    A GmsecError is thrown if the field value is null.
    A GmsecError is thrown if for whatever reason the field cannot be added to the message.

    Notes
    ----------
    1. This method relies on the usage of Message.set_value(str, str).
    2. If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will
       be updated to publish at the (new) rate.
";

