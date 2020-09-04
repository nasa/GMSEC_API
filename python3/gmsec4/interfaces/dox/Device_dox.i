
%feature("docstring") gmsec::api::mist::Device "

    This class is a lightweight container for holding information
    on a Device, and used to generate GMSEC Device messages by the
    ConnectionManager class

    CONSTRUCTORS:

    Device(name: str, status: Device.DeviceStatus)

    Initializes the Device with no Device parameter information.

    Parameters
    ----------
    name   : The name of the Device
    status : The status of the Device

    Exceptions
    ----------
    A GmsecError is thrown if the name parameter is None or is an empty string



    Device(name: str, status: Device.DeviceStatus, params: DeviceParamList)

    Initializes the Device with DeviceParam information.

    Parameters
    ----------
    name   : The name of the Device
    status : The status of the Device
    params : The DeviceParamList of one or more DeviceParam objects

    Exceptions
    ----------
    A GmsecError is thrown if the name parameter is None or is an empty string



    Device(other)

    Creates a copy of the given Device.

    Parameters
    ----------
    other : The Device to copy
";

%feature("docstring") gmsec::api::mist::Device::getVersion "

    get_version(self) -> str

    Accessor for the Device Version

    Returns
    -------
    The Device Version string

    Exceptions
    ----------
    A GmsecError is thrown if the Version field has not been set.
";

%feature("docstring") gmsec::api::mist::Device::numberAvailable "

    number_available(self) -> bool

    Returns the availability of a Device Number

    Returns
    -------
    True if a Device Number has been set; False otherwise.
";

%feature("docstring") gmsec::api::mist::Device::setVersion "

    set_version(self, version: str)

    Associates version information with the Device.

    Parameters
    ----------
    version : Version information as a string (e.g. \"v5.0\")
";

%feature("docstring") gmsec::api::mist::Device::getRole "

    get_role(self) -> str

    Accessor for the Device role

    Returns
    -------
    The Device role string

    Exceptions
    ----------
    A GmsecError is thrown if the role field has not been set.
";

%feature("docstring") gmsec::api::mist::Device::groupAvailable "

    group_available(self) -> bool

    Returns the availability of a Device Group

    Returns
    -------
    True if a Device Group has been set, False otherwise.
";

%feature("docstring") gmsec::api::mist::Device::setInfo "

    set_info(self, info: Field)

    Associates the given informational Field with the Device.

    Parameters
    ----------
    info : Field to provide Device information.
";

%feature("docstring") gmsec::api::mist::Device::setSerial "

    set_serial(self, serial: str)

    Associates the given serial number string with the Device.
";

%feature("docstring") gmsec::api::mist::Device::setStatus "

    set_status(self, status: Device::DeviceStatus)

    Associates the given status with the Device.

    Parameters
    ----------
    status : Device status
";

%feature("docstring") gmsec::api::mist::Device::getStatus "

    get_status(self) -> Device::DeviceStatus

    Accessor for the Device status

    Returns
    -------
    The Device status enumeration
";

%feature("docstring") gmsec::api::mist::Device::infoAvailable "

    info_available(self) -> bool

    Returns the availability of a Device info field

    Returns
    -------
    True if a Device info field has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::Device::getNumber "

    get_number(self) -> Field

    Accessor for the Device Number

    Returns
    -------
    A reference to the Device Number field

    Exceptions
    ----------
    A GmsecError is thrown if the Number field has not been set
";

%feature("docstring") gmsec::api::mist::Device::setModel "

    set_model(self, model: str)

    Associates model information for the Device.

    Parameters
    ----------
    mode : Model information regarding the Device
";

%feature("docstring") gmsec::api::mist::Device::serialAvailable "

    serial_available(self) -> bool

    Returns the availability of a Device Serial Number

    Returns
    -------
    True if a Device Serial Number has been set; False otherwise
";

%feature("docstring") gmsec::api::mist::Device::setRole "

    set_role(self, role: str)

    Associates role information for the Device.

    Parameters
    ----------
    role : Role information regarding the Device
";

%feature("docstring") gmsec::api::mist::Device::setGroup "

    set_group(self, group: str)

    Associates group information for the Device.

    Parameters
    ----------
    group : Group information regarding the Device
";

%feature("docstring") gmsec::api::mist::Device::addParam "

    add_param(self, param: DeviceParam)

    Adds a DeviceParam to the Device

    Parameters
    ----------
    param : The new DeviceParam
";

%feature("docstring") gmsec::api::mist::Device::getName "

    get_name(self) -> str

    Retrieves the name of the device

    Returns
    -------
    The name of the device
";

%feature("docstring") gmsec::api::mist::Device::getGroup "

    get_group(self) -> str

    Accessor for the Device Group

    Returns
    -------
    The Device Group string

    Exceptions
    ----------
    A GmsecError is thrown if the Group field has not been set
";

%feature("docstring") gmsec::api::mist::Device::getModel "

    get_model(self) -> str

    Accessor for the Device Model

    Returns
    -------
    The Device model string

    Exceptions
    ----------
    A GmsecError is thrown if the Model field has not been set
";

%feature("docstring") gmsec::api::mist::Device::setNumber "

    set_number(self, number: Field)

    Associates a number Field with the Device.

    Parameters
    ----------
    number : Number Field to associate with the Device.
";

%feature("docstring") gmsec::api::mist::Device::roleAvailable "

    role_available(self) -> bool

    Returns the availability of a Device Role

    Returns
    -------
    True if a Device Role has been set; False otherwise.
";

%feature("docstring") gmsec::api::mist::Device::versionAvailable "

    version_available(self) -> bool

    Returns the availability of a Device Version

    Returns
    -------
    True if a Device Version has been set; False otherwise.
";

%feature("docstring") gmsec::api::mist::Device::modelAvailable "

    model_available(self) -> bool

    Returns the availability of a Device Model

    Returns
    -------
    True if a Device Model has been set; False otherwise.
";

%feature("docstring") gmsec::api::mist::Device::getParam "

    get_param(self, index: int) -> DeviceParam

    Returns a reference to a DeviceParam held in this Device

    Parameters
    ----------
    index : The zero-based index of the DeviceParam to access

    Returns
    -------
    A reference to the requested DeviceParam

    Exceptions
    ----------
    A GmsecError is thrown if the index supplied is out of bounds
";

%feature("docstring") gmsec::api::mist::Device::getInfo "

    get_info(self) -> Field

    Accessor for the Device Info

    Returns
    -------
    The Device Info Field

    Exceptions
    ----------
    A GmsecError is thrown if the Info field has not been set
";

%feature("docstring") gmsec::api::mist::Device::getSerial "

    get_serial(self) -> str

    Accessor for the Device Serial Number

    Returns
    -------
    The Device Serial Number string

    Exceptions
    ----------
    A GmsecError is thrown if the Serial Number field has not been set
";

%feature("docstring") gmsec::api::mist::Device::getParamCount "

    get_param_count(self) -> int

    Returns the number of DeviceParam objects associated with the Device

    Returns
    -------
    The number of DeviceParam objects
";
