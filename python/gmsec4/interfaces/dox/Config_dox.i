
// File: classlibgmsec__python_1_1Config.xml


%feature("docstring") gmsec::api::Config "

    This class is a collection for managing connection configuration items.

    Encapsulates the parameters necessary for initialization Connections and setting
    middleware dependent values in Messages.  Used by the Connection and Message,
    this class provides a generic means of supplying initialization data without
    being Connection/Message specific.

    See Also
    --------
    Connection
    Message

    CONSTRUCTORS:

    Config(argc, argv)

    This constructor will parse command line parameters and add values for any param that is of the form <name>=<value>This constructor will parse command line parameters and add values for any param that is of the form <name>=<value>

    e.g. gmsub.exe mw-id=bolt server=localhost

    Parameters
    ----------
    argc: number of parameters in argv (from main)
    argv: an array of string parameters (from main)

    libgmsec_python.Config(data)

    This constructor will create a Config object by parsing key/value pairs from the given data string. The data string can contain either XML or JSON formatted configuration data, or contain raw data consisting of white-space separated key=value pairs.

    Parameters
    ----------
    data: data string containing configuration information

    Exceptions
    ----------
    An Exception is thrown if the data string is None, or contains an empty string
    An exception is thrown if the data string is not parsable

    libgmsec_python.Config(cfg)

    Basic (and safe) deep copy constructor

    Parameters
    ----------
    cfg: config object to copy
";

%feature("docstring") gmsec::api::Config::clearValue "

    clearValue(name) -> bool

    This function clears a value by name

    Parameters
    ----------
    name: string
        name of value to clear

    Returns
    -------
    boolean to indicate whether operation was successful

    Exceptions
    ----------
    Exception if name is None

";

%feature("docstring") gmsec::api::Config::toJSON "

    toJSON() -> string

    This function will dump the Config into JSON format.
    The format is the same as that which is accepted by Config(const char* data).

    Returns
    -------
    JSON formatted string

    See Also
    --------
    Config(data)
";

%feature("docstring") gmsec::api::Config::fromXML "

    fromXML(xml)

    This function will read a configuration from an XML string.
    The format is the same as that which is generated by toXML().

    Example XML string:
    <CONFIG>
        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>
        <PARAMETER NAME=\"server\">127.0.0.1</PARAMETER>
        <PARAMETER NAME=\"tracking\">true</PARAMETER>
        <PARAMETER NAME=\"isthreaded\">true</PARAMETER>
    </CONFIG>

    Parameters
    ----------
    xml: string
        XML string

    Exceptions
    ----------
    Exception if a malformed XML, or None, string is given

    See Also
    --------
    toXML()
    Config(data)
";

%feature("docstring") gmsec::api::Config::getBooleanValue "

    getBooleanValue(name) -> bool

    This function attempts to get the boolean representation of configuration
    element. An exception is thrown if the element does not exist or there is no
    valid boolean interpretion of the value.

    Parameters
    ----------
    name: string
        name of value to get

    Returns
    -------
    value of named item

    Exceptions
    ----------
    Exception if name is None

    getBooleanValue(name, defaultValue) -> bool

    This function attempts to get the boolean representation of configuration
    element. If the element does not exist or the element cannot be interpreted as
    a boolean, the default value is returned.

    Parameters
    ----------
    name: string
        name of value to get
    defaultValue: default value to be returned if no value exist for name parameter

    Returns
    -------
    value of named item

    Exceptions
    ----------
    Exception if name is None
";

%feature("docstring") gmsec::api::Config::clear "

    clear()

    This function clears all values
";

%feature("docstring") gmsec::api::Config::toXML "

    toXML() -> string

    This function will dump the Config into XML format.
    The format is the same as that which is parsed by fromXML().

    Returns
    -------
    XML string

    See Also
    --------
    fromXML(xml)

";

%feature("docstring") gmsec::api::Config::getIntegerValue "

    getIntegerValue(name) -> int

    This function attempts to get the integer representation of configuration
    element. An exception is thrown if the element does not exist or there is no
    valid integer interpretion of the value.

    Parameters
    ----------
    name: string
        name of value to get

    Returns
    -------
    int value of named item

    Exceptions
    ----------
    Exception if name is None

    getIntegerValue(name, defaultValue) -> int

    This function attempts to get the integer representation of configuration
    element. If the element does not exist or the element cannot be interpreted as
    a integer, the default value is returned.

    Parameters
    ----------
    name: string
        name of value to get
    defaultValue: 
        default value to be returned if no value exists for name parameter

    Returns
    -------
    int value of named item

    Exceptions
    ----------
    Exception if name is None

";

%feature("docstring") gmsec::api::Config::addValue "

    addValue(name, value)

    This function adds a value to the config object by name

    Parameters
    ----------
    name: string
        name of value to add
    value: string
        value to be added


    Exceptions
    ----------
    Exception if either name or value are None, or are empty strings

    Example
    ----------
    import libgmsec_python
    config = libgmsec_python.Config()
    config.addValue('LOGLEVEL', 'INFO')
";

%feature("docstring") gmsec::api::Config::getFirst "

    getFirst(name, value) -> bool

    This function gets the first name & value for iteration. The scope
    of the name and value are the life of the Config object, or
    until the next getFirst() call.

    Parameters
    ----------
    name: string
        output parameter name of value 
    value: string
        output parameter value
    Returns
    -------
    boolean status as to whether name/value pair are available
";

%feature("docstring") gmsec::api::Config::getNext "

    getNext(cp) -> bool

    This function gets the next name & value for iteration. The scope
    of the name and value are the life of the Config object, or
    until the next getNext() call.

    Parameters
    ----------
    cp: ConfigPair
        GMSEC ConfigPair object

    Returns
    -------
    boolean status as to whether name/value pair are available

    Example
    -------
    import libgmsec_python as lp
    cfg = lp.Config('mw-id=bolt server=localhost')
    cp = lp.ConfigPair()
    status = lp.Status()
    status = cfg.getFirst(cp)
    while status:
        print 'name ' + 'lp.getName() + 'value'
        status = cfg.getNext(cp)
";

%feature("docstring") gmsec::api::Config::merge "

    merge(other, overwriteExisting=True)

    Merge the given Config object with the current Config object.

    Parameters
    ----------
    other: the Config object from which to read name/value pairs.
    overwriteExisting: [optional] overwriteExisting - indicates whether to overwrite any existing name/value
        pairs within the working %Config object.  If flag is omitted, then the default is
        to overwrite existing name/value pairs.

";

%feature("docstring") gmsec::api::Config::getValue "

    getValue(name) -> string

    This function gets a value by name. The scope of the value is the life of the Config object.

    Parameters
    ----------
    name: string
        name of value to get

    Returns
    -------
    If the key entry is found, then the associated value is returned; otherwise None is returned.

    Exceptions
    ----------
    Exception if name is None

    getValue(name, defaultValue) -> string

    This function gets a value by name and if there is no value assigned to will return
    the defaultValue parameter. defaultValue can be None or empty. The scope of the value is the life of
    the Config object.

    Parameters
    ----------
    name: string
        name of value to get
    
    defaultValue: default value to be returned if no value exist for name parameter

    Returns
    -------
    If the key entry is found, then the associated value is returned; otherwise defaultValue is returned.

    Exceptions
    ----------
    Exception if name is None

";

%feature("docstring") gmsec::api::Config::getDoubleValue "

    getDoubleValue(name) -> float

    This function attempts to get the double representation of configuration
    element. An exception is thrown if the element does not exist or there is no
    valid double interpretion of the value.

    Parameters
    ----------
    name: string
        name of value to get

    Returns
    -------
    float value of named item

    Exceptions
    ----------
    Exception if name is None

    getDoubleValue(self, name, defaultValue) -> float

    This function attempts to get the double representation of configuration
    element. If the element does not exist or the element cannot be interpreted as
    a double, the default value is returned.

    Parameters
    ----------
    name: string
        name of value to get
    defaultValue: float
        default value to be returned if no value exist for name parameter

    Returns
    -------
    float value of named item

    Exceptions
    ----------
    Exception if name is None

";
