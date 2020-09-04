
%feature("docstring") gmsec::api::mist::message::MnemonicMessage "

    A MistMessage that is capable of storing Mnemonics.

    The following message schema IDs and their templates are supported:
    MSG.MVAL
    REQ.MVAL
    RESP.MVAL

    See Also
    --------
    Message
    MistMessage
    Config
    Specification
    Field
    MessageFieldIterator

    CONSTRUCTORS:

    MnemonicMessage(subject: str, schemaID: str, spec: Specification)

    Initializes the message instance.

    Parameters
    ----------
    subject  : The subject string for the message.
    schemaID : The string used to identify the message schema in the GMSEC ISD.
               The schema ID has the format of: MessageKind.MessageType (e.g. MSG.MVAL).
    spec     : The Specification this message's schema will adhere to.



    MnemonicMessage(subject: str, schemaID: str, config: Config, spec: Specification)

    Initializes the message instance.

    Parameters
    ----------
    subject  : The subject string for the message.
    schemaID : The string used to identify the message schema in the GMSEC ISD.
               The schema ID has the format of: messageKind.messageType (e.g. MSG.MVAL).
    config   : A Config to associate with the message.
    spec     : The Specification this message's schema will adhere to.



    MnemonicMessage(other: MnemonicMessage)

    Copy constructor - initializes the message instance using the other given MnemonicMessage.

    Parameters
    ----------
    other : The MnemonicMessage to copy.



    MnemonicMessage(data: str)

    Initializes a MnemonicMessage from an XML or JSON string.

    Parameters
    ----------
    data : XML or JSON string used to initialize MnemonicMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.

    Note
    ----------
    This constructor has been deprecated. Use MnemonicMessage(self, spec, data) instead.



    MnemonicMessage(spec: Specification, data: str)

    Initializes a MnemonicMessage using the given XML or JSON data representation of the message, and ensures the message adheres to the provided specification.

    Parameters
    ----------
    spec : The Specification this message's schema will adhere to.
    data : XML of JSON string used to initialize the message.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.
    A GmsecError is thrown if the given data does not represent a Mnemonic Message.
";


%feature("docstring") gmsec::api::mist::message::MnemonicMessage::getMnemonicIterator "

    get_mnemonic_iterator(self) -> MnemonicIterator

    Method that allows the callee to get the MnemonicIterator associated
    with the MnemonicMessage.
    This iterator will allow for applications to iterate over the
    Mnemonics stored within the MnemonicMessage.
    The iterator is reset each time getMnemonicIterator() is called.
    The iterator itself is destroyed when the MnemonicMessage
    is destroyed.

    Note: Only one MnemonicIterator is associated with a
    MnemonicMessage; multiple calls to getMnemonicIterator()
    will return a reference to the same MnemonicIterator. Each
    call will reset the iterator.

    Returns
    -------
    A reference to a MnemonicIterator.
";

%feature("docstring") gmsec::api::mist::message::MnemonicMessage::addMnemonic "

    add_mnemonic(self, mnemonic: Mnemonic)

    Add a mnemonic to the message.

    Parameters
    ----------
    mnemonic : The Mnemonic to add to the message.
";


%feature("docstring") gmsec::api::mist::message::MnemonicMessage::convertMessage "

    convert_message(message: Message) -> MnemonicMessage

    Constructs and returns a MnemonicMessage from an an ordinary message.

    Parameters
    ----------
    message - The Message to convert, if possible, into a MnemonicMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the given Message cannot be converted to a MnemonicMessage.
";
