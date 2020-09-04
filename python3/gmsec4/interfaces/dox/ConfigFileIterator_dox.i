
%feature("docstring") gmsec::api::ConfigFileIterator "

    This class is used to access elements of a configuration file
    (ConfigFile).
";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextSubscription "

    has_next_subscription(self) -> bool

    Determines if there is a next subscription in the iterator

    Returns
    -------
    True if there is a next subscription, False otherwise.
";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextMessage "

    has_next_message(self) -> bool

    Determines if there is a next Message in the iterator

    Returns
    -------
    True if there is a next Message, False otherwise.
";

%feature("docstring") gmsec::api::ConfigFileIterator::nextCustomElement "

    next_custom_element(self) -> str

    Returns the next custom XML element.

    Note: The returned string is volatile; the callee should consider
        making a copy of the returned string.  For example:

    try:
        cfgFile = libgmsec_python3.ConfigFile()

        cfgFile.load(\"someFile.xml\")

        iter = cfgFile.getIterator()

        if iter.hasCustomElement():
            print(iter.nextCustomElement())

    except GmsecError as e:
        //handle error loading ConfigFile

    Returns
    -------
    An XML string.

    Exceptions
    ----------
    A GmsecError is thrown if no custom elements are available.
";

%feature("docstring") gmsec::api::ConfigFileIterator::nextConfig "

    next_config(self) -> ConfigEntry

    Returns the next ConfigEntry.

    Returns
    -------
    The next ConfigEntry.

    Exceptions
    ----------
    A GmsecError is thrown if no more config entries are available.

    See Also
    --------
    has_next_config()
";

%feature("docstring") gmsec::api::ConfigFileIterator::nextSubscription "

    next_subscription(self) -> SubscriptionEntry

    Returns the next SubscriptionEntry.

    Note: The returned structure contains a string that is volatile;
    the callee should consider making a copy of the returned string.

    Returns
    -------
    The next SubscriptionEntry.

    Exceptions
    ----------
    A GmsecError is thrown if no more subscription entries are available.

    See Also
    --------
    has_next_subscription()
";

%feature("docstring") gmsec::api::ConfigFileIterator::reset "

    reset(self)

    Resets all iterators to the beginning of the respective lists that
    are maintained by the ConfigFile.
";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextConfig "

    has_next_config(self) -> bool

    Determines if there is a next Config in the iterator

    Returns
    -------
    Whether there is a next Config
";

%feature("docstring") gmsec::api::ConfigFileIterator::nextMessage "

    next_message(self) -> MessageEntry

    Returns a copy of the next MessageEntry.

    Returns
    -------
    The next MessageEntry.

    Exceptions
    ----------
    A GmsecError is thrown if no message entries are available.

    See Also
    --------
    has_next_message()
";

%feature("docstring") gmsec::api::ConfigFileIterator::hasNextCustomElement "

    has_next_custom_element(self) -> bool

    Determines if there is another custom XML element in the iterator

    Returns
    -------
    True if there is a next custom element, False otherwise.
";
