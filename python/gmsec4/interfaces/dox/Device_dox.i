
// File: classlibgmsec__python_1_1Device.xml


%feature("docstring") gmsec::api::mist::Device "

    This class is a lightweight container for holding information
    on a Device, and used to generate GMSEC Device messages by the
    ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTORS:

    Device(self, name, status, params)

    Initializes the Device object with Device parameter information.

    Parameters
    ----------
    name: The name of the Device
    status: The status of the Device (DeviceStatus object)
    params: The list of Device parameters (DeviceParamList object)

    Exceptions
    ----------
    An Exception is thrown if the name parameter is NULL or empty

    Device(self, name, status)

    Initializes the Device object with no Device parameter information.

    Parameters
    ----------
    name: The name of the Device
    status: The status of the Device (DeviceStatus object)

    Exceptions
    ----------
    An Exception is thrown if the name parameter is NULL or empty

    Device(other)

    Initializes the Device object with the information from another device.

    Parameters
    ----------
    other: The other device object
";

%feature("docstring") gmsec::api::mist::Device::getVersion "

    getVersion(self) -> char const *

    Accessor for the Device Version

    Returns
    -------
    A pointer to the Device Version string

    Exceptions
    ----------
    An exception is thrown if the Version field has not been set

";

%feature("docstring") gmsec::api::mist::Device::numberAvailable "

    numberAvailable(self) -> bool

    Returns the availability of a Device Number

    Returns
    -------
    True if a Device Number has been set, false otherwise

";

%feature("docstring") gmsec::api::mist::Device::setVersion "

    setVersion(self, version)

    Supplies a Version for the Device and makes a copy of the string

";

%feature("docstring") gmsec::api::mist::Device::getRole "

    getRole(self) -> char const *

    Accessor for the Device Role

    Returns
    -------
    A pointer to the Device Role string

    Exceptions
    ----------
    An exception is thrown if the Role field has not been set

";

%feature("docstring") gmsec::api::mist::Device::groupAvailable "

    groupAvailable(self) -> bool

    Returns the availability of a Device Group

    Returns
    -------
    True if a Device Group has been set, false otherwise

";

%feature("docstring") gmsec::api::mist::Device::setInfo "

    setInfo(self, info)

    Supplies a Info for the Device and makes a copy of the field
";

%feature("docstring") gmsec::api::mist::Device::setSerial "

    setSerial(self, serial)

    Supplies a Serial Number for the Device and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::Device::setStatus "

    setStatus(self, status)

    Supplies a Status for the Device and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::Device::getStatus "

    getStatus(self) -> gmsec::api::mist::Device::DeviceStatus

    Accessor for the Device Status

    Returns
    -------
    The Device Status enumeration
";

%feature("docstring") gmsec::api::mist::Device::infoAvailable "

    infoAvailable(self) -> bool

    Returns the availability of a Device Info field

    Returns
    -------
    True if a Device Info field has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::Device::getNumber "

    getNumber(self) -> Field

    Accessor for the Device Number

    Returns
    -------
    A reference to the Device Number field

    Exceptions
    ----------
    An exception is thrown if the Number field has not been set
";

%feature("docstring") gmsec::api::mist::Device::setModel "

    setModel(self, model)

    Supplies a Model for the Device and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::Device::serialAvailable "

    serialAvailable(self) -> bool

    Accessor for the Device Serial Number

    Returns
    -------
    A pointer to the Device Serial Number string

    Exceptions
    ----------
    An exception is thrown if the Serial Number field has not been set
";

%feature("docstring") gmsec::api::mist::Device::setRole "

    setRole(self, role)

    Supplies a Role for the Device and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::Device::setGroup "

    setGroup(self, group)

    Supplies a Group for the Device and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::Device::addParam "

    addParam(self, param)

    Adds a single DeviceParam to the current list

    Parameters
    ----------
    param: the new DeviceParam object

";

%feature("docstring") gmsec::api::mist::Device::getName "

    getName(self) -> char const *

    Retrieves the name of the device

    Returns
    -------
    The name of the device
";

%feature("docstring") gmsec::api::mist::Device::getGroup "

    getGroup(self) -> char const *

    Accessor for the Device Group

    Returns
    -------
    A pointer to the Device Group string

    Exceptions
    ----------
    An exception is thrown if the Group field has not been set
";

%feature("docstring") gmsec::api::mist::Device::getModel "

    getModel(self) -> char const *

    Accessor for the Device Model

    Returns
    -------
    A pointer to the Device model string

    Exceptions
    ----------
    An exception is thrown if the Model field has not been set
";

%feature("docstring") gmsec::api::mist::Device::setNumber "

    setNumber(self, number)

    Supplies a Number for the Device and makes a copy of the field

";

%feature("docstring") gmsec::api::mist::Device::roleAvailable "

    roleAvailable(self) -> bool

    Returns the availability of a Device Role

    Returns
    -------
    True if a Device Role has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::Device::versionAvailable "

    versionAvailable(self) -> bool

    Returns the availability of a Device Version

    Returns
    -------
    True if a Device Version has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::Device::modelAvailable "

    modelAvailable(self) -> bool

    Returns the availability of a Device Model

    Returns
    -------
    True if a Device Model has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::Device::getParam "

    getParam(self, idx) -> DeviceParam const &

    Returns a reference to a DeviceParam held in this object

    Parameters
    ----------
    idx: the zero-based index of the DeviceParam object to access

    Returns
    -------
    A reference to the requested DeviceParam object

    Exceptions
    ----------
    An exception if the index supplied is out of bounds

";

%feature("docstring") gmsec::api::mist::Device::getInfo "

    getInfo(self) -> Field

    Accessor for the Device Info

    Returns
    -------
    A reference to the Device Info field

    Exceptions
    ----------
    An exception is thrown if the Info field has not been set
";

%feature("docstring") gmsec::api::mist::Device::getSerial "

    getSerial(self) -> char const *

    Accessor for the Device Serial Number

    Returns
    -------
    A pointer to the Device Serial Number string

    Exceptions
    ----------
    An exception is thrown if the Serial Number field has not been set
";

%feature("docstring") gmsec::api::mist::Device::getParamCount "

    getParamCount(self) -> size_t

    Returns the number of parameters for this Device

    Returns
    -------
    a size_t representation of the number of parameters
";
