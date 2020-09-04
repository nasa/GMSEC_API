
%feature("docstring") gmsec::api::mist::DeviceParam "

    This class is a lightweight container for holding information
    on a DeviceParam, and used to generate GMSEC Device messages by
    the ConnectionManager and Device classes

    CONSTRUCTORS:

    DeviceParam(self, name: str, timestamp: str, value: Field)

    Initializes the DeviceParam with a name, timestamp, and value.

    Parameters
    ----------
    name      : The name of the DeviceParam.
    timestamp : The time at which the sampled value was taken.
    value     : The Field value of the parameter.



    DeviceParam(self, deviceParam: DeviceParam)

    Copy-constructor that initializes the DeviceParam from another DeviceParam.

    Parameters
    ----------
    deviceParam : The other DeviceParam to copy.
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

    get_timestamp(self) -> str

    Retrieves the timestamp of the DeviceParam

    Returns
    -------
    The timestamp of the DeviceParam
";

%feature("docstring") gmsec::api::mist::DeviceParam::getName "

    get_name(self) -> str

    Retrieves the name of the DeviceParam

    Returns
    -------
    The name of the DeviceParam
";

%feature("docstring") gmsec::api::mist::DeviceParam::getValue "

    get_value(self) -> Field

    Retrieves the value of the DeviceParam

    Returns
    -------
    The Field value of the DeviceParam
";
