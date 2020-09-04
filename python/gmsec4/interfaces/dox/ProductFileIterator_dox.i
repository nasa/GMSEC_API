
// File: classlibgmsec__python_1_1ProductFileIterator.xml


%feature("docstring") gmsec::api::mist::ProductFileIterator "

    The ProductFileIterator supports iterating over the ProductFile objects of a ProductFileMessage.

    Note: ProductFileIterator is not thread safe.

    See Also
    --------
    ProductFileMessage::getProductFileIterator()
";

%feature("docstring") gmsec::api::mist::ProductFileIterator::reset "

    reset(self)

    Resets the interator to the beginning of the ProductFile list that is maintained by the ProductFileMessage.
";

%feature("docstring") gmsec::api::mist::ProductFileIterator::next "

    next(self) -> ProductFile

    Returns a reference to the next available ProductFile object.

    Returns
    -------
    A reference to a ProductFile object.

    Exceptions
    ----------
    Exception is thrown if the iterator has reached the end (i.e. there are no more ProductFiles).
";

%feature("docstring") gmsec::api::mist::ProductFileIterator::hasNext "

    hasNext(self) -> bool

    Provides information as to whether there are additional ProductFile objects that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";
