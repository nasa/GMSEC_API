
// File: classlibgmsec__python_1_1MnemonicIterator.xml


%feature("docstring") gmsec::api::mist::MnemonicIterator "

    The MnemonicIterator supports iterating over the Mnemonic objects of a MnemonicMessage.

    Note: MnemonicIterator is not thread safe.

    See Also
    --------
    MnemonicMessage::getMnemonicIterator()
";

%feature("docstring") gmsec::api::mist::MnemonicIterator::hasNext "

    hasNext(self) -> bool

    Provides information as to whether there are additional Mnemonic objects that can be referenced using next().

    Returns
    -------
    True if additional fields are available, false otherwise.
";

%feature("docstring") gmsec::api::mist::MnemonicIterator::next "

    next(self) -> Mnemonic

    Returns a reference to the next available Mnemonic object.

    Returns
    -------
    A reference to a Mnemonic object.

    Exceptions
    ----------
    Exception is thrown if the iterator has reached the end (i.e. there are no more Mnemonics).
";

%feature("docstring") gmsec::api::mist::MnemonicIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Mnemonic list that is maintained by the MnemonicMessage.
";

