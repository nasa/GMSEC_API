
%feature("docstring") gmsec::api::Config "

    This class is a collection for managing connection configuration
    items.

    Encapsulates the parameters necessary for initialization
    Connections and setting middleware dependent values in Messages.
    Used by the Connection and Message, this class provides a generic
    means of supplying initialization data without being Connection
    or Message specific.

    See Also
    --------
    Connection
    Message

    CONSTRUCTORS:

    Config()

    Default constructor.



    Config(args: list)

    This constructor accepts a list of strings, generally like those
    passed from the command line.

    Example
    -------
    config = libgmsec_python3.Config(sys.argv)

    Parameters
    ----------
    args : List of strings



    Config(data: str)

    This constructor will create a Config by parsing key/value
    pairs from the given data string. The data string can contain
    either XML or JSON formatted configuration data, or contain raw
    data consisting of white-space separated key=value pairs.

    Parameters
    ----------
    data : String containing configuration information

    Exceptions
    ----------
    A GmsecError is thrown if the data string is None, or contains an empty string.
    A GmsecError is thrown if the data string is not parsable.



    Config(cfg: Config)

    Basic (and safe) copy constructor

    Parameters
    ----------
    cfg : Config to copy
";

%feature("docstring") gmsec::api::Config::addValue "

    add_value(self, name: str, value: str)

    This method adds a value to the Config by name

    Parameters
    ----------
    name  : Name of value to add
    value : Value to be added


    Exceptions
    ----------
    A GmsecError is thrown if either name or value are None, or are empty strings

    Example
    ----------
    import libgmsec_python3
    config = libgmsec_python3.Config()
    config.add_value('mw-id', 'bolt')
    config.add_value('loglevel', 'info')
";

%feature("docstring") gmsec::api::Config::clearValue "

    clear_value(self, name: str) -> bool

    This method clears a value by name

    Parameters
    ----------
    name : Name of value to clear

    Returns
    -------
    Boolean to indicate whether operation was successful

    Exceptions
    ----------
    A GmsecError is thrown if name is None.
";

%feature("docstring") gmsec::api::Config::getValue "

    get_value(self, name: str) -> str

    This method gets a value by name. The scope of the value is the
    life of the Config.

    Parameters
    ----------
    name : Name of value to get

    Returns
    -------
    If the key entry is found, then the associated value is returned;
    otherwise None is returned.


    get_value(self, name: str, defaultValue: str) -> str

    This method gets a value by name and if there is no value
    assigned to will return the defaultValue parameter. defaultValue
    can be None or empty. The scope of the value is the life of
    the Config.

    Parameters
    ----------
    name         : Name of value to get
    defaultValue : Default value to be returned if no value exist for name parameter

    Returns
    -------
    Value of named item, or the default value if item is not found.
";

%feature("docstring") gmsec::api::Config::getBooleanValue "

    get_boolean_value(self, name: str) -> bool

    This method attempts to get the boolean representation of
    configuration element. An exception is thrown if the element does
    not exist or there is no valid boolean interpretion of the value.

    Parameters
    ----------
    name : Name of value to get

    Returns
    -------
    Value of named item

    Exceptions
    ----------
    A GmsecError is thrown if name is None or contains an empty string.


    get_boolean_value(self, name: str, defaultValue: bool) -> bool

    This method attempts to get the boolean representation of
    configuration element. If the element does not exist or the element
    cannot be interpreted as a boolean, the default value is returned.

    Parameters
    ----------
    name         : Name of value to get
    defaultValue : Default value to be returned if no value exist for name parameter

    Returns
    -------
    Value of named item, or the default value if item is not found.
";

%feature("docstring") gmsec::api::Config::getIntegerValue "

    get_integer_value(self, name: str) -> int

    This method attempts to get the integer representation of
    configuration element. An exception is thrown if the element does
    not exist or there is no valid integer interpretion of the value.

    Parameters
    ----------
    name : Name of value to get

    Returns
    -------
    Integer value of named item

    Exceptions
    ----------
    A GmsecError is thrown if name is None or contains an empty string


    get_integer_value(self, name: str, defaultValue: int) -> int

    This method attempts to get the integer representation of
    configuration element. If the element does not exist or the element
    cannot be interpreted as a integer, the default value is returned.

    Parameters
    ----------
    name         : Name of value to get
    defaultValue : Default value to be returned if no value exists for name parameter

    Returns
    -------
    Value of named item, or the default value if item is not found.
";

%feature("docstring") gmsec::api::Config::getDoubleValue "

    get_double_value(self, name: str) -> double

    This method attempts to get the double representation of
    configuration element. An exception is thrown if the element does
    not exist or there is no valid double interpretion of the value.

    Parameters
    ----------
    name : Name of value to get

    Returns
    -------
    Value of named item

    Exceptions
    ----------
    A GmsecError is thrown if name is None or contains an empty string


    get_double_value(self, name: str, defaultValue: double) -> double

    This method attempts to get the double representation of
    configuration element. If the element does not exist or the element
    cannot be interpreted as a double, the default value is returned.

    Parameters
    ----------
    name         : Name of value to get
    defaultValue : Default value to be returned if no value exist for name parameter

    Returns
    -------
    Value of named item, or the default value if item is not found.
";

%feature("docstring") gmsec::api::Config::clear "

    clear(self)

    This method clears all values associated with the Config.
";

%feature("docstring") gmsec::api::Config::getFirst "

    get_first(self, cp: ConfigPair) -> bool

    This method gets the first name and value for iteration. The scope
    of the name and value are the life of the Config, or
    until the next get_first() call.

    Parameters
    ----------
    cp : ConfigPair in which to store the Config entries name and value

    Returns
    -------
    Boolean status as to whether name/value pair are available

    Example
    -------
    import libgmsec_python3 as lp

    cfg = lp.Config('mw-id=bolt server=localhost')

    cp = lp.ConfigPair()

    available = cfg.get_first(cp)

    while available:
        print('name: ' + cp.get_name() + ', value: ' + cp.get_value())

        available = cfg.get_next(cp)
";

%feature("docstring") gmsec::api::Config::getNext "

    get_next(self, cp: ConfigPair) -> bool

    This method gets the next name and value for iteration. The scope
    of the name and value are the life of the Config, or
    until the next get_next() call.

    Parameters
    ----------
    cp : ConfigPair in which to store the Config entries name and value

    Returns
    -------
    Boolean status as to whether name/value pair are available

    Example
    -------
    import libgmsec_python3 as lp

    cfg = lp.Config('mw-id=bolt server=localhost')

    cp = lp.ConfigPair()

    available = cfg.get_first(cp)

    while available:
        print('name: ' + cp.get_name() + ', value: ' + cp.get_value())

        available = cfg.get_next(cp)
";

%feature("docstring") gmsec::api::Config::merge "

    merge(self, other: Config, overwriteExisting=True: bool)

    Merge the given Config with the current Config.

    Parameters
    ----------
    other             : The Config from which to read name/value pairs.
    overwriteExisting : Indicates whether to overwrite any existing
                        name/value pairs within the working Config;
                        default is True.
";

%feature("docstring") gmsec::api::Config::toXML "

    to_XML(self) -> str

    This method will dump the Config into XML format.
    The format is the same as that which is parsed by from_XML().

    Returns
    -------
    XML string

    See Also
    --------
    from_XML()

";

%feature("docstring") gmsec::api::Config::fromXML "

    from_XML(self, xml: str)

    This method will read a configuration from an XML string.
    The format is the same as that which is generated by to_XML().

    Example XML string:
    <CONFIG>
        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>
        <PARAMETER NAME=\"server\">127.0.0.1</PARAMETER>
        <PARAMETER NAME=\"tracking\">true</PARAMETER>
    </CONFIG>

    Parameters
    ----------
    xml : XML string

    Exceptions
    ----------
    A GmsecError is thrown if a malformed XML, or None, string is given

    See Also
    --------
    to_XML()
    Config(data)
";

%feature("docstring") gmsec::api::Config::toJSON "

    to_JSON(self) -> str

    This method will dump the Config into JSON format.
    The format is the same as that which is accepted by the
    Config(data) constructor.

    Returns
    -------
    JSON formatted string

    See Also
    --------
    Config(data)
";
