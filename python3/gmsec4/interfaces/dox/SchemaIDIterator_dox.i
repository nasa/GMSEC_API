
%feature("docstring") gmsec::api::mist::SchemaIDIterator "

    Supports the iteration of Specification's loaded schema templates.

    See Also
    --------
    Specification::get_schema_id_iterator()
";

%feature("docstring") gmsec::api::mist::SchemaIDIterator::hasNext "

    has_next(self) -> bool

    Provides information as to whether there are additional Schema IDs
    that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::mist::SchemaIDIterator::next "

    next(self) -> str

    Returns a reference to the next available Schema ID.

    Returns
    -------
    A Schema ID string.

    Exceptions
    ----------
    A GmsecError is thrown if the iterator has reached the end (i.e.
    there are no more IDs).
";

%feature("docstring") gmsec::api::mist::SchemaIDIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Schema list that is
    maintained by Specification.
";
