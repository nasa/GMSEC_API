
%feature("docstring") gmsec::api::mist::message::ProductFileMessage "

    A specialized MistMessage that is capable of storing ProductFiles.

    The following message schema IDs and their templates are supported:
    2019.00.C2MS.MSG.PROD
    MSG.PROD

    See Also
    --------
    Message
    MistMessage
    Config
    Specification
    Field
    MessageFieldIterator

    CONSTRUCTORS:

    ProductFileMessage(subject: str, responseStatus: ResponseStatus.Response, msgKind: Message.MessageKind,
                       productType: str, productSubtype: str, spec: Specification)

    Initializes the message instance and automatically builds a schema ID based on params given.

    Parameters
    ----------
    subject        : The subject string for the message.
    responseStatus : The RESPONSE-STATUS field to indert into a MSG PROD message.
    msgKind        : The kind of message to instantiate.
    productType    : The PROD-TYPE field to insert into a MSG PROD message.
    productSubtype : The PROD-SUBTYPE field to indert into a MSG PROD message.
    spec           : The specification this message's schema will adhere to.

    Exceptions
    ----------
    A GmsecError is thrown if Specification fails to load the template directory or if a valid schemaID
    cannot be created from the given params.



    ProductFileMessage(subject: str, responseStatus: ResponseStatus.Response, msgKind: Message.MessageKind,
                       productType: str, productSubtype: str, config: Config, spec: Specification)

    Initializes the message instance and associates a Config and automatically builds a schema ID based on
    params given.

    Parameters
    ----------
    subject        : The subject string for the message.
    responseStatus : The RESPONSE-STATUS field to indert into a MSG PROD message.
    msgKind        : The kind of message to instantiate.
    productType    : The PROD-TYPE field to insert into a MSG PROD message.
    productSubtype : The PROD-SUBTYPE field to indert into a MSG PROD message.
    config         : A Config to associate with the message.
    spec           : The specification this message's schema will adhere to.

    Exceptions
    ----------
    A GmsecError is thrown if Specification fails to load the template directory or if a valid schemaID
    cannot be created from the given params.



    ProductFileMessage(subject: str, responseStatus: ResponseStatus.Response, schemaID: str, spec: Specification)

    Initializes the message with a given schema ID

    Parameters
    ----------
    subject        : The subject string for the message.
    responseStatus : The RESPONSE-STATUS field to indert into a MSG PROD message.
    schemaID       : The string used to identify the message schema in C2MS or other message specification.
                     The schema ID has the MessageKind.MessageType.MessageSubtype (e.g. MSG.PROD).
    spec           : The Specification this message's schema will adhere to.

    Exceptions
    ----------
    A GmsecError is thrown if Specification fails to load the template directory or if schemaID
    is not a valid ID.



    ProductFileMessage(subject: str, responseStatus: ResponseStatus.Response, schemaID: str,
                       config: Config, spec: Specification)

    Initializes the message with a given schema ID

    Parameters
    ----------
    subject        : The subject string for the message.
    responseStatus : The RESPONSE-STATUS field to indert into a MSG PROD message.
    schemaID       : The string used to identify the message schema in C2MS or other message specification.
                     The schema ID has the MessageKind.MessageType.MessageSubtype (e.g. MSG.PROD).
    config         : A Config to associate with the message.
    spec           : The Specification this message's schema will adhere to.

    Exceptions
    ----------
    A GmsecError is thrown if Specification fails to load the template directory or if schemaID
    is not a valid ID.



    ProductFileMessage(self, other: ProductFileMessage)

    Copy constructor - initializes the message instance using the other given ProductFileMessage.

    Parameters
    ----------
    other : The ProductFileMessage to copy.



    ProductFileMessage(data: str)

    Initializes a ProductFileMessage using an XML or JSON string.

    Parameters
    ----------
    data : XML or JSON string used to initialize the ProductFileMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.

    Note
    ----------
    This constructor has been deprecated. Use ProductFileMessage(self, spec, data) instead.



    ProductFileMessage(spec: Specification, data: str)

    Initializes a ProductFileMessage using an XML or JSON string.

    Parameters
    ----------
    spec : The Specification this message's schema will adhere to.
    data : The XML or JSON string used to initialize the ProductFileMessage.

    Exceptions
    ----------
    A GmsecError is thrown if the given data does not represent a valid XML or JSON statement.
    A GmsecError is thrown if the given data does not represent a Product File Message.
";


%feature("docstring") gmsec::api::mist::message::ProductFileMessage::add_product_file "

    add_product_file(self, productFile: ProductFile)

    Add a product file to the message.

    Parameters
    ----------
    productFile : The ProductFile to add to the message.
";

%feature("docstring") gmsec::api::mist::message::ProductFileMessage::getProductSubtype "

    get_product_subtype(self) -> str

    Get the PROD-SUBTYPE field value of this ProductFile message.
";

%feature("docstring") gmsec::api::mist::message::ProductFileMessage::getProductType "

    get_product_type(self) -> str

    Get the PROD-TYPE field value of this ProductFile message.
";

%feature("docstring") gmsec::api::mist::message::ProductFileMessage::getResponseStatus "

    get_response_status(self) -> ResponseStatus.Response

    Get the response status of this ProductFile message.
";

%feature("docstring") gmsec::api::mist::message::ProductFileMessage::convertMessage "

    convert_message(message: Message) -> ProductFileMessage

    Constructs and returns a MnemonicMessage from an an ordinary Message.

    Exceptions
    ----------
    A GmsecError is thrown if the given message cannot be converted to a ProductFileMessage
";

%feature("docstring") gmsec::api::mist::message::ProductFileMessage::getProductFileIterator "

    get_product_file_iterator(self) -> ProductFileIterator

    Method that allows the callee to get the ProductFileIterator associated with the
    ProductFileMessage. This iterator will allow for applications to iterate over the
    ProductFiles stored within the ProductFileMessage. The iterator is reset each time
    get_product_file_iterator() is called. The iterator itself is destroyed when the
    ProductFileMessage is destroyed.

    Note
    ----------
    Only one ProductFileIterator is associated with a ProductFileMessage; multiple calls
    to get_product_file_iterator() will return a reference to the same ProductFileIterator.
    Each call will reset the iterator.

    Returns
    -------
    A reference to a ProductFileIterator.
";
