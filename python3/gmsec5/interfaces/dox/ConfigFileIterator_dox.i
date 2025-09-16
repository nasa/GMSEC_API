/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::ConfigFileIterator "

    This class is used to access elements of a configuration file
    (ConfigFile).
";


%feature("docstring") gmsec::api5::ConfigFileIterator::hasNextConfig "

    has_next_config(self) -> bool

    Determines if there is a next Config in the iterator

    Returns
    -------
    Whether there is a next Config
";


%feature("docstring") gmsec::api5::ConfigFileIterator::hasNextMessage "

    has_next_message(self) -> bool

    Determines if there is a next Message in the iterator

    Returns
    -------
    True if there is a next Message, False otherwise.
";


%feature("docstring") gmsec::api5::ConfigFileIterator::hasNextSubscription "

    has_next_subscription(self) -> bool

    Determines if there is a next subscription in the iterator

    Returns
    -------
    True if there is a next subscription, False otherwise.
";


%feature("docstring") gmsec::api5::ConfigFileIterator::nextConfig "

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


%feature("docstring") gmsec::api5::ConfigFileIterator::nextMessage "

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


%feature("docstring") gmsec::api5::ConfigFileIterator::nextSubscription "

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


%feature("docstring") gmsec::api5::ConfigFileIterator::reset "

    reset(self)

    Resets all iterators to the beginning of the respective lists that
    are maintained by the ConfigFile.
";
