
// File: classlibgmsec__python_1_1DeviceParam.xml


%feature("docstring") gmsec::api::mist::DeviceParam "

    This class is a lightweight container for holding information
    on a DeviceParam, and used to generate GMSEC Device messages by the
    ConnectionManager and Device classes

    CONSTRUCTORS:

    DeviceParam(self, name, timestamp, value)

    Initializes the DeviceParam object with a name, timestamp, and value.

    Parameters
    ----------
    name: The name of the DeviceParam.
    timestamp: The time at which the sampled value was taken.
    value: The value of the parameter.

    DeviceParam(self, deviceParam)

    Initializes the DeviceParam object from another DeviceParam.

    Parameters
    ----------
    deviceParam: The other DeviceParam to inherit values from.
";

%feature("docstring") gmsec::api::mist::DeviceParam::__eq__ "

    __eq__(self, deviceParam) -> bool

    Equality operator for DeviceParam
";

%feature("docstring") gmsec::api::mist::DeviceParam::__ne__ "

    __ne__(self, deviceParam) -> bool

    Non-equality operator for DeviceParam
";

%feature("docstring") gmsec::api::mist::DeviceParam::getTimestamp "

    getTimestamp(self) -> char const *

    Retrieves the timestamp of the DeviceParam

    Returns
    -------
    The timestamp of the DeviceParam
";

%feature("docstring") gmsec::api::mist::DeviceParam::getName "

    getName(self) -> char const *

    Retrieves the name of the DeviceParam

    Returns
    -------
    The name of the DeviceParam
";

%feature("docstring") gmsec::api::mist::DeviceParam::getValue "

    getValue(self) -> Field

    Retrieves the value of the DeviceParam

    Returns
    -------
    The value of the DeviceParam
";
