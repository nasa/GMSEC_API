
// File: classlibgmsec__python_1_1ConfigFileIterator.xml


%feature("docstring") gmsec::api::ConfigFileIterator "

    This class is used to access elements of a configuration file (ConfigFile) object.
";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextSubscription "

    hasNextSubscription(self) -> bool

    Determines if there is a next subscription in the iterator

    Returns
    -------
    whether there is a next

";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextMessage "

    hasNextMessage(self) -> bool

    Determines if there is a next Message in the iterator

    Returns
    -------
    whether there is a next

";

%feature("docstring") gmsec::api::ConfigFileIterator::nextCustomElement "

    nextCustomElement(self) -> char const *

    Returns the next custom XML element.

    Note: The returned string is volatile; the callee should consider
        making a copy of the returned string.  For example:

    try:
        cfgFile = ConfigFile()
        cfgFile.load(\"someFile.xml\")

        iter = cfgFile.getIterator()

        customElement = ""

        if iter.hasCustomElement():
            customElement = iter.nextCustomElement()

        if customElement:
            print customElement
    except Exception as e:
        //handle error loading ConfigFile

    Returns
    -------
    An XML string.

    Exceptions
    ----------
    Exception is thrown if no custom elements are available.

";

%feature("docstring") gmsec::api::ConfigFileIterator::nextConfig "

    nextConfig(self) -> ConfigEntry

    Returns the next ConfigEntry.

    Returns
    -------
    The next ConfigEntry.

    Exceptions
    ----------
    Exception is thrown if no more config entries are available.

    See Also
    --------
    hasNextConfig()

";

%feature("docstring") gmsec::api::ConfigFileIterator::nextSubscription "

    nextSubscription(self) -> SubscriptionEntry

    Returns the next SubscriptionEntry.

    Note: The returned structure contains a string that is volatile; the callee should consider
        making a copy of the returned string.

    Returns
    -------
    The next SubscriptionEntry.

    Exceptions
    ----------
    Exception is thrown if no more subscription entries are available.

    See Also
    --------
    hasNextSubscription()
";

%feature("docstring") gmsec::api::ConfigFileIterator::reset "

    reset(self)

    Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFile.
";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextConfig "

    hasNextConfig(self) -> bool

    Determines if there is a next Config in the iterator

    Returns
    -------
    whether there is a next

";

%feature("docstring") gmsec::api::ConfigFileIterator::nextMessage "

    nextMessage(self) -> MessageEntry

    Returns a copy of the next MessageEntry.

    Returns
    -------
    The next MessageEntry.

    Exceptions
    ----------
    Exception is thrown if no message entries are available.

    See Also
    --------
    hasNextMessage()

";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextCustomElement "

    hasNextCustomElement(self) -> bool

    Determines if there is another custom XML element in the iterator

    Returns
    -------
    whether there is a next

";
