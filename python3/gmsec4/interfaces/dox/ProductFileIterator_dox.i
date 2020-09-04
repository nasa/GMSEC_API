
%feature("docstring") gmsec::api::mist::ProductFileIterator "

    The ProductFileIterator supports iterating over the ProductFiles
    of a ProductFileMessage.

    Note: ProductFileIterator is not thread safe.

    See Also
    --------
    ProductFileMessage::get_product_file_iterator()
";

%feature("docstring") gmsec::api::mist::ProductFileIterator::reset "

    reset(self)

    Resets the interator to the beginning of the ProductFile list that
    is maintained by the ProductFileMessage.
";

%feature("docstring") gmsec::api::mist::ProductFileIterator::next "

    next(self) -> ProductFile

    Returns a reference to the next available ProductFile.

    Returns
    -------
    A reference to a ProductFile.

    Exceptions
    ----------
    A GmsecError is thrown if the iterator has reached the end (i.e.
    there are no more ProductFiles).
";

%feature("docstring") gmsec::api::mist::ProductFileIterator::hasNext "

    has_next(self) -> bool

    Provides information as to whether there are additional ProductFiles
    that can be referenced using next().

    Returns
    -------
    True if additional fields are available; False otherwise.
";
