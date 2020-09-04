
%feature("docstring") gmsec::api::MessageFieldIterator "

    The MessageFieldIterator supports iterating over the fields of a
    Message.

    Note: MessageFieldIterator is not thread safe.

    See Also
    --------
    Message::get_field_iterator()
";

%feature("docstring") gmsec::api::MessageFieldIterator::hasNext "

    has_next(self) -> bool

    Provides information as to whether there are additional fields that
    can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::MessageFieldIterator::next "

    next(self) -> Field

    Returns a reference to the next available Field.

    Returns
    -------
    A reference to a Field.

    Exceptions
    ----------
    A GmsecError is thrown if the iterator has reached the end (i.e.
    there are no more fields).
";

%feature("docstring") gmsec::api::MessageFieldIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Field list that is
    maintained by the Message.
";
