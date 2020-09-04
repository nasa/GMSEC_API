
// File: classlibgmsec__python_1_1MessageFieldIterator.xml


%feature("docstring") gmsec::api::MessageFieldIterator "

    The MessageFieldIterator supports iterating over the fields of a Message.

    Note: MessageFieldIterator is not thread safe.

    See Also
    --------
    Message::getFieldIterator()
";

%feature("docstring") gmsec::api::MessageFieldIterator::hasNext "

    hasNext(self) -> bool

    Provides information as to whether there are additional fields that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::MessageFieldIterator::next "

    next(self) -> Field

    Returns a reference to the next available Field object.

    Returns
    -------
    A reference to a Field object.

    Exceptions
    ----------
    Exception is thrown if the iterator has reached the end (i.e. there are no more fields).
";

%feature("docstring") gmsec::api::MessageFieldIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Field list that is maintained by the Message.
";
