
// File: classlibgmsec__python_1_1ServiceParam.xml


%feature("docstring") gmsec::api::mist::ServiceParam "

    This class is a lightweight container for holding information
    on a Simple Service parameter, and used to generate GMSEC Simple Service
    messages by the ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTORS:

    ServiceParam(self, dName, dValue)

    Initializes the ServiceParam object with a name and value.

    Parameters
    ----------
    dName: The name of the ServiceParam
    dValue: The value of the parameter

    ServiceParam(self, other)

    Copy constructor - Initializes the ServiceParam object from another ServiceParam.

    Parameters
    ----------
    other: The other ServiceParam to inherit avalues from
";

%feature("docstring") gmsec::api::mist::ServiceParam::getValue "

    getValue(self) -> Field

    Retrieves the value of the ServiceParam

    Returns
    -------
    The value of the ServiceParam
";

%feature("docstring") gmsec::api::mist::ServiceParam::__eq__ "

    __eq__(self, serviceParam) -> bool

    Parameters
    ----------
    serviceParam: gmsec::api::mist::mist::ServiceParam const &
";

%feature("docstring") gmsec::api::mist::ServiceParam::getName "

    getName(self) -> char const *

    Retrieves the name of the ServiceParam

    Returns
    -------
    The name of the ServiceParam
";

%feature("docstring") gmsec::api::mist::ServiceParam::__ne__ "

    __ne__(self, serviceParam) -> bool

    Parameters
    ----------
    serviceParam: gmsec::api::mist::mist::ServiceParam const &
";
