
%feature("docstring") gmsec::api::mist::MnemonicIterator "

    The MnemonicIterator supports iterating over the Mnemonics
    of a MnemonicMessage.

    Note: MnemonicIterator is not thread safe.

    See Also
    --------
    MnemonicMessage::get_mnemonic_iterator()
";

%feature("docstring") gmsec::api::mist::MnemonicIterator::hasNext "

    has_next(self) -> bool

    Provides information as to whether there are additional Mnemonics
    that can be referenced using next().

    Returns
    -------
    True if additional fields are available, False otherwise.
";

%feature("docstring") gmsec::api::mist::MnemonicIterator::next "

    next(self) -> Mnemonic

    Returns a reference to the next available Mnemonic.

    Returns
    -------
    A reference to a Mnemonic.

    Exceptions
    ----------
    A GmsecError is thrown if the iterator has reached the end (i.e.
    there are no more Mnemonics).
";

%feature("docstring") gmsec::api::mist::MnemonicIterator::reset "

    reset(self)

    Resets the interator to the beginning of the Mnemonic list that is
    maintained by the MnemonicMessage.
";

