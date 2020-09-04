
// File: classlibgmsec__python_1_1SchemaIDIterator.xml


%feature("docstring") gmsec::api::mist::SchemaIDIterator "

    Supports the iteration of Specification's loaded schema templates.

    See Also
    --------
    Specification::getSchemaIDIterator()
";

%feature("docstring") gmsec::api::mist::SchemaIDIterator::hasNext "

    hasNext(self) -> bool

    Provides information as to whether there are additional Schema IDs that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::mist::SchemaIDIterator::next "

    next(self) -> char const *

    Returns a reference to the next available Schema ID.

    Returns
    -------
    A Schema ID string.

    Exceptions
    ----------
    Exception is thrown if the iterator has reached the end (i.e. there are no more IDs).
";

%feature("docstring") gmsec::api::mist::SchemaIDIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Schema list that is maintained by Specification.
";
