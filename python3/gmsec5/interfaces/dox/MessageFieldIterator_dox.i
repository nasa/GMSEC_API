/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::MessageFieldIterator "

    The MessageFieldIterator supports iterating over the fields of a Message.

    Note: MessageFieldIterator is not thread safe.

    See Also
    --------
    Message::get_field_iterator()
";

%feature("docstring") gmsec::api5::MessageFieldIterator::hasNext "

    has_next(self) -> bool

    Provides information as to whether there are additional fields that can be referenced using next().

    Returns
    -------
    True if additional fields are available, False otherwise.
";

%feature("docstring") gmsec::api5::MessageFieldIterator::next "

    next(self) -> Field

    Returns a reference to the next available Field.

    Returns
    -------
    A reference to a Field.

    Exceptions
    ----------
    A GmsecError is thrown if the iterator has reached the end (i.e.  there are no more fields).
";

%feature("docstring") gmsec::api5::MessageFieldIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Field list that is maintained by the Message.
";
