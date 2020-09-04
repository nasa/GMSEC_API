
// File: classlibgmsec__python_1_1DeviceMessage.xml


%feature("docstring") gmsec::api::mist::message::DeviceMessage "

    The Message object contains a GMSEC MSG C2CX DEV message.
    The following message schema IDs and their templates are supported: @n
    GMSEC.MSG.C2CX.DEV

    CONSTRUCTORS:

    DeviceMessage(self, subject, version)

    Initializes the message instance.

    Parameters
    ----------
    subject: The subject string for the message.
    version: The version of the GMSEC message specification to be used.

    DeviceMessage(self, subject, config, version)

    Initializes the message instance and associates a Configuration object with the message.

    Parameters
    ----------
    subject: The subject string for the message.
    config: A configuration to associate with the message.
    version: The version of the GMSEC message specification to be used.

    DeviceMessage(self, data)

    Initialize DeviceMessage from XML or JSON string.

    Parameters
    ----------
    data: XML or JSON string used to initialize the DeviceMessage.
";



%feature("docstring") gmsec::api::mist::message::DeviceMessage::getDevice "

    getDevice(self, index) -> Device

    Get device numbered at index.

    Returns
    -------
    Device at specified index inside of DeviceMessage.

    Exceptions
    ----------
    Throws an exception if the index specified is not in the
        range of Devices in the message.
";


%feature("docstring") gmsec::api::mist::message::DeviceMessage::getDeviceIterator "

    getDeviceIterator(self) -> DeviceIterator

    Method that allows the callee to get the DeviceIterator associated with the DeviceMessage.
    This iterator will allow for applications to iterate over the Device objects stored within the DeviceMessage.
    The iterator is reset each time getDeviceIterator() is called.  The iterator itself is destroyed when the
    DeviceMessage object is destroyed.

    Note: Only one DeviceIterator object is associated with a DeviceMessage object; multiple calls to
        getDeviceIterator() will return a reference to the same DeviceIterator object.  Each call will reset the iterator.

    Returns
    -------
    A reference to a DeviceIterator object.
";


%feature("docstring") gmsec::api::mist::message::DeviceMessage::getNumDevices "

    getNumDevices(self) -> size_t

    Get the number of devices in this message.

    Returns
    -------
    Return the number of devices in the message.
";


%feature("docstring") gmsec::api::mist::message::DeviceMessage::convertMessage "

    convertMessage(message) -> DeviceMessage

    Constructs and returns a DeviceMessage from an an ordinary message.
";

%feature("docstring") gmsec::api::mist::message::DeviceMessage::addDevice "

    addDevice(self, device)

    Add a device to the message.
";
