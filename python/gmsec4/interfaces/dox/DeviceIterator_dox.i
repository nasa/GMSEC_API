
// File: classlibgmsec__python_1_1DeviceIterator.xml


%feature("docstring") gmsec::api::mist::DeviceIterator "

    The DeviceIterator supports iterating over the Device objects of a DeviceMessage.

    Note: DeviceIterator is not thread safe.

    See Also
    --------
    DeviceMessage::getDeviceIterator()
";

%feature("docstring") gmsec::api::mist::DeviceIterator::hasNext "

    hasNext(self) -> bool

    Provides information as to whether there are additional Device objects that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::mist::DeviceIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Device list that is maintained by the DeviceMessage.
";

%feature("docstring") gmsec::api::mist::DeviceIterator::next "

    next(self) -> Device

    Returns a reference to the next available Device object.

    Returns
    -------
    A reference to a Device object.

    Exceptions
    ----------
    Exception is thrown if the iterator has reached the end (i.e. there are no more Devices).
";
