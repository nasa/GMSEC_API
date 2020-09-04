
%feature("docstring") gmsec::api::mist::message::DeviceMessage "

    A Message that contains/defines a C2CX Device Message.

    The following message schema IDs and their templates are supported:
    MSG.DEV

    CONSTRUCTORS:

    DeviceMessage(self, subject: str, spec: Specification)

    Initializes the message instance.

    Parameters
    ----------
    subject : The subject string for the message.
    spec    : The Specification this message's schema will adhere to.



    DeviceMessage(self, subject: str, config: Config, spec: Specification)

    Initializes the message instance and associates a Config with the message.

    Parameters
    ----------
    subject : The subject string for the message.
    config  : A Config to associate with the message.
    spec    : The Specification this message's schema will adhere to.



    DeviceMessage(self, other: DeviceMessage)

    Copy constructor - initializes the message instance using the other given DeviceMessage

    Parameters
    ----------
    other : The other DeviceMessage to copy.



    DeviceMessage(self, data: str)

    Initialize DeviceMessage from XML or JSON string.

    Parameters
    ----------
    data : XML or JSON string used to initialize the DeviceMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.

    Note
    ----------
    This constructor has been deprecated. Use DeviceMessage(self, spec, data) instead.



    DeviceMessage(self, spec: Specification, data: str)

    Initializes a Device Message using the given XML or JSON data representation of the message,
    and ensures the message adheres to the provided specification.

    Parameters
    ----------
    spec : The Specification this message's schema will adhere to.
    data : XML or JSON string used to initialize the DeviceMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.
    A GmsecError is thrown if the given data does not represent a Device Message.
";



%feature("docstring") gmsec::api::mist::message::DeviceMessage::getDevice "

    get_device(self, index: int) -> Device

    Get device numbered at index.

    Returns
    -------
    Device at specified index inside of DeviceMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the index specified is not in the range of Devices in the message.
";


%feature("docstring") gmsec::api::mist::message::DeviceMessage::getDeviceIterator "

    get_device_iterator(self) -> DeviceIterator

    Method that allows the callee to get the DeviceIterator associated
    with the DeviceMessage. This iterator will allow for applications
    to iterate over the Devices stored within the DeviceMessage.
    The iterator is reset each time getDeviceIterator() is called.
    The iterator itself is destroyed when the DeviceMessage is
    destroyed.

    Note: Only one DeviceIterator is associated with a
        DeviceMessage; multiple calls to getDeviceIterator()
        will return a reference to the same DeviceIterator.
        Each call will reset the iterator.

    Returns
    -------
    A reference to a DeviceIterator.
";


%feature("docstring") gmsec::api::mist::message::DeviceMessage::getNumDevices "

    get_num_devices(self) -> int

    Get the number of devices in this message.

    Returns
    -------
    Return the number of devices in the message.
";


%feature("docstring") gmsec::api::mist::message::DeviceMessage::convertMessage "

    convert_message(message: Message) -> DeviceMessage

    Constructs and returns a DeviceMessage from an an ordinary message.
";

%feature("docstring") gmsec::api::mist::message::DeviceMessage::addDevice "

    add_device(self, device: Device)

    Add a Device to the message.
";
