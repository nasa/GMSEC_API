
// File: classlibgmsec__python_1_1MnemonicMessage.xml


%feature("docstring") gmsec::api::mist::message::MnemonicMessage "

    A MistMessage object that is capable of storing Mnemonic objects
    The following message schema IDs and their templates are supported: @n
    MSG.MVAL @n
    REQ.MVAL @n
    RESP.MVAL

    See Also
    --------
    Message
    MistMessage
    Config
    Specification
    Field
    MsgFieldIterator

    CONSTRUCTORS:

    MnemonicMessage(self, subject, version)

    Initializes the message instance.

    Parameters
    ----------
    subject: The subject string for the message.
    version: The version of the GMSEC message specification to be used.

    MnemonicMessage(self, subject, config, version)

    Initializes the message instance.

    Parameters
    ----------
    subject: The subject string for the message.
    config: A configuration to associate with the message.
    version: The version of the GMSEC message specification to be used.

    MnemonicMessage(self, other)

    Copy constructor - initializes the message instance using the other given MnemonicMessage.

    Parameters
    ----------
    other: The other MnemonicMessage object to copy.

    MnemonicMessage(self, data)

    Initializes a MnemonicMessage from an XML or JSON string.

    Parameters
    ----------
    data: XML or JSON string used to initialize MnemonicMessage.
";


%feature("docstring") gmsec::api::mist::message::MnemonicMessage::getMnemonic "

    getMnemonic(self, index) -> Mnemonic

    Get mnemonic numbered at index. Note that the index which will be
    retrieved does not correspond to the GMSEC ISD and starts from 0 instead of 1.
    For example, getMnemonic(0) would return the Mnemonic corresponding to
    MNEMONIC.1.

    Returns
    -------
    Mnemonic at specified index inside of MnemonicMessage.

    Exceptions
    ----------
    Throws an exception if the index specified is not in the range of Mnemonics in this message.

    Note: This function has been deprecated, use MnemonicIterator instead.
";


%feature("docstring") gmsec::api::mist::message::MnemonicMessage::getMnemonicIterator "

    getMnemonicIterator(self) -> MnemonicIterator

    Method that allows the callee to get the MnemonicIterator associated with the MnemonicMessage.
    This iterator will allow for applications to iterate over the Mnemonic objects stored within the MnemonicMessage.
    The iterator is reset each time getMnemonicIterator() is called.  The iterator itself is destroyed when the
    MnemonicMessage object is destroyed.

    Note: Only one MnemonicIterator object is associated with a MnemonicMessage object; multiple calls to
        getMnemonicIterator() will return a reference to the same MnemonicIterator object.  Each call will reset the iterator.

    Returns
    -------
    A reference to a MnemonicIterator object.
";

%feature("docstring") gmsec::api::mist::message::MnemonicMessage::addMnemonic "

    addMnemonic(self, mnemonic)

    Add a mnemonic to the message.
";


%feature("docstring") gmsec::api::mist::message::MnemonicMessage::convertMessage "

    convertMessage(message) -> MnemonicMessage

    Constructs and returns a MnemonicMessage from an an ordinary message.
";


%feature("docstring") gmsec::api::mist::message::MnemonicMessage::getNumMnemonics "

    getNumMnemonics(self) -> size_t

    Get the number of mnemonics in this message.

    Returns
    -------
    Return the number of mnemonics in the message.
";

