
%feature("docstring") gmsec::api::mist::ServiceParam "

    This class is a lightweight container for holding information
    on a Simple Service parameter, and used to generate GMSEC Simple
    Service messages by the ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTORS:

    ServiceParam(name: str, value: Field)

    Initializes the ServiceParam with a name and value.

    Parameters
    ----------
    name  : The name of the ServiceParam
    value : The Field containing the value of the parameter



    ServiceParam(other: ServiceParam)

    Copy constructor - Initializes the ServiceParam from another ServiceParam.

    Parameters
    ----------
    other : The ServiceParam to copy
";

%feature("docstring") gmsec::api::mist::ServiceParam::getValue "

    get_value(self) -> Field

    Retrieves the value of the ServiceParam

    Returns
    -------
    The value Field of the ServiceParam
";

%feature("docstring") gmsec::api::mist::ServiceParam::__eq__ "

    __eq__(self, serviceParam: ServiceParam) -> bool

    Parameters
    ----------
    serviceParam : The ServiceParam to compare against
";

%feature("docstring") gmsec::api::mist::ServiceParam::getName "

    get_name(self) -> str

    Retrieves the name of the ServiceParam

    Returns
    -------
    The name of the ServiceParam
";

%feature("docstring") gmsec::api::mist::ServiceParam::__ne__ "

    __ne__(self, serviceParam: ServiceParam) -> bool

    Parameters
    ----------
    serviceParam : The ServiceParam to compare against
";
