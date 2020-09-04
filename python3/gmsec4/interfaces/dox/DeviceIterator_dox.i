
%feature("docstring") gmsec::api::mist::DeviceIterator "

    The DeviceIterator supports iterating over the Devices of a
    DeviceMessage.

    Note: DeviceIterator is not thread safe.

    See Also
    --------
    DeviceMessage.get_device_iterator()
";

%feature("docstring") gmsec::api::mist::DeviceIterator::hasNext "

    has_next(self) -> bool

    Provides information as to whether there are additional Devices
    that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::mist::DeviceIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Device list that is
    maintained by the DeviceMessage.
";

%feature("docstring") gmsec::api::mist::DeviceIterator::next "

    next(self) -> Device

    Returns a reference to the next available Device.

    Returns
    -------
    A reference to a Device.

    Exceptions
    ----------
    A GmsecError is thrown if the iterator has reached the end (i.e. there are no more Devices).
";
