/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::ConfigFile "

    This file contains a class for managing configuration files.

    A ConfigFile has a one-to-many mapping to connection
    configurations. If a user wishes to maintain only a single
    Connection, the use of Config is suggested.

    CONSTRUCTORS:

    ConfigFile()

    Constructor that creates an empty configuration file object.
    The internally held MessageFactory will be configured with the default message specification used by the API.

    Once the ConfigFile has been created, use load(filepath) to
    load a configuration file, or if the plan is to add elements to the
    empty ConfigFile, then use save(filepath) to save any
    changes.

    Returns
    -------
    A ConfigFile object containing no definitions.

    Exceptions
    ----------
    A GmsecError is thrown if the MessageFactory cannot be instantiated.


    ConfigFile(config: Config)

    Constructor that creates an empty configuration file object.
    The internally held MessageFactory will be configured with the given configuration.

    Parameters
    ----------
    config : The configuration to apply to the internally held MessageFactory.

    Configuration options referenced
    --------------------------------
        gmsec-specification-version
        gmsec-schema-level
        gmsec-msg-content-validate

    Returns
    -------
    A ConfigFile object containing no definitions.

    Exceptions
    ----------
    A GmsecError is thrown if the MessageFactory cannot be instantiated.
";


%feature("docstring") gmsec::api5::ConfigFile::load "

    load(self, filepath: str)

    This function opens the configuration file and parses the XML
    It will return errors explaining what, if anything, went wrong.

    Parameters
    ----------
    filepath : The location to which the file will be saved

    Exceptions
    ----------
    A GmsecError is thrown if file parsing error occurs
";


%feature("docstring") gmsec::api5::ConfigFile::save "

    save(self, filePath=None: str, compact=False: bool)

    This function saves the currently open configuration file to
    the location specified.
    It will return errors explaining what, if anything, went wrong.

    Parameters
    ----------
    filePath : The location to which the file will be saved;
               if no path is specified or is specified as NULL, then the path,
               if any, that was used with load(filepath) will be used.
    compact  : To indicate whether the XML output will be compacted or not; default is False

    Exceptions
    ----------
    A GmsecError is thrown if error occurs writing to the file
";


%feature("docstring") gmsec::api5::ConfigFile::fromXML "

    from_xml(self, xml: str)

    This function parses the XML in the passed in string.
    It will return errors explaining what, if anything, went wrong.

    Exceptions
    ----------
    A GmsecError is thrown if error occurs while parsing XML string
";


%feature("docstring") gmsec::api5::ConfigFile::toXML "

    to_xml(self) -> str

    This function produces an XML config file containing all known
    config file tags.

    Returns
    -------
    The XML string representation of the ConfigFile
";


%feature("docstring") gmsec::api5::ConfigFile::isLoaded "

    is_loaded(self) -> bool

    Returns
    -------
    True if file loaded, False otherwise.

";


%feature("docstring") gmsec::api5::ConfigFile::addConfig "

    add_config(self, name: str, config: Config)

    Adds a config definition to the list of configurations.
    The Config, though passed by reference, will not be modified.

    Programmatic XML element addition will use pre-defined hierarchical
    rules for determining the placement of elements within the document,
    if saved.

    When adding a config element, it will initially be inserted after
    the last <CONFIG> element in the document, if no other config
    definitions are present in the document, the new config definition
    will be added after the last <SUBSCRIPTION> element in the document,
    if there are no subscription definitions then it will be added as
    the very first child of the root element.

    Note: Adding a config with a name that currently exists will
    overwrite the current configuration definition.

    Note: Adding a config to a newly instantiated ConfigFile
    that has not loaded a document will create a new XML document.

    Parameters
    ----------
    name   : Name to be associated with the configuration
    config : Config to add

    Exceptions
    ----------
    A GmsecError is thrown if name is NULL or contains an empty string
";


%feature("docstring") gmsec::api5::ConfigFile::lookupConfig "

    lookup_config(self, name: str) -> Config

    This function will attempt to find a named Config in the
    config file and set the passed in Config with
    the loaded values.

    Example config file load and get config:
    try:
        cfgFile = ConfigFile()

        # load & parse configuration file
        cfgFile.load(\"/some/path/to/config.xml\")

        # attempt to get Config from ConfigFile
        try:
            cfg = cfgFile.lookup_config(\"MyConfig\")

            # do something with cfg

        except GmsecError as e:
            # handle error Config lookup error

    except GmsecError as e:
        # handle configuration file parsing error

    Parameters
    ----------
    name : Value in the name=\"\" attribute of the <CONFIG/> block to seed the Config

    Returns
    -------
    Config that correlates with the given name

    Exceptions
    ----------
    A GmsecError is thrown for cases where configuration for the given name does not exist
";


%feature("docstring") gmsec::api5::ConfigFile::removeConfig "

    remove_config(self, name: str) -> bool

    Removes the Config definition which is mapped to the provided name.

    Parameters
    ----------
    name : Name associated with the configuration definition

    Returns
    -------
    True if removal was successful, False if not
";


%feature("docstring") gmsec::api5::ConfigFile::addMessage "

    add_message(self, name: str, message: Message)

    Adds a message definition to the list of messages.
    The Message, though passed by reference, will not be modified.

    Programmatic XML element addition will use pre-defined hierarchical
    rules for determining the placement of elements within the document,
    if saved.

    When adding a message element, it will initially be inserted after
    the last <MESSAGE> element in the document, if no other message
    definitions are present in the document, it will be added after the
    last <CONFIG> element in the document. If no other definitions are
    present in the document, the new config definition will be added
    after the last <SUBSCRIPTION> element in the document. If there are
    no subscription definitions then it will be added as the very first
    child of the root element.

    Note: Adding a message with a name that currently exists will
    overwrite the current message definition.

    Note: Adding a message definition to a newly instantiated
    ConfigFile that has not loaded a document will create a new
    XML document.

    Parameters
    ----------
    name    : Name to be associated with the message definition
    message : Message to add

    Exceptions
    ----------
    A GmsecError is thrown if name is NULL or contains an empty string
";


%feature("docstring") gmsec::api5::ConfigFile::lookupMessage "

    lookup_message(self, name: str) -> Message

    This function will attempt to find a named message in the
    config file and create a new message from that xml
    the loaded fields and values.

    Example config file load and get message:
    try:
        cfgFile = ConfigFile()

        # load & parse configuration file
        cfgFile.load(\"/some/path/to/config.xml\")

        # attempt to get Message from ConfigFile
        try:
            msg = cfgFile.lookup_message(\"MyMessage\")

            # do something with msg
        except GmsecError as me:
            # handle error Message lookup error
    except GmsecError as e:
        # handle configuration file parsing error

    Parameters
    ----------
    name : Value in the name=\"\" attribute of the <MESSAGE/> block to seed the Message.

    Returns
    -------
    Message that correlates with the given name.

    Exceptions
    ----------
    A GmsecError is thrown for cases where message configuration for the given name does not exist.
";


%feature("docstring") gmsec::api5::ConfigFile::removeMessage "

    remove_message(self, name: str) -> bool

    Removes the Message definition which is mapped to the provided name.

    Parameters
    ----------
    name : Name associated with the message definition

    Returns
    -------
    True if removal was successful, False if not.
";


%feature("docstring") gmsec::api5::ConfigFile::addSubscriptionEntry "

    add_subscription_entry(self, name: str, entry: SubscriptionEntry)

    Adds a Subscription definition to the list of subscriptions.

    Note: Adding a subscription with a name that currently exists will
    overwrite the current subscription entry.

    Note: Adding a subscription definition to a newly instantiated
    ConfigFile that has not loaded a document will create a new
    XML document.

    Parameters
    ----------
    name  : Name to be associated with the subscription.
    entry : A SubscriptionEntry object.

    Exceptions
    ----------
    A GmsecError is thrown if either name is null or if it contains an empty string

    See Also
    --------
    SubscriptionEntry
";


%feature("docstring") gmsec::api5::ConfigFile::lookupSubscriptionEntry "

    lookup_subscription_entry(self, name: str) -> SubscriptionEntry

    This function will look up a subscription pattern defined in
    the config file. This is useful to allow easy modification of
    subject names without code changes.

    Example
    try:
        cfgFile = ConfigFile()

        # load & parse configuration file
        cfgFile.load(\"/some/path/to/config.xml\")

        # get subscription entry from file
        try:
            entry = cgfFile.lookup_subscription_entry(\"MySubscription\")

            name  = entry.get_name()
            topic = entry.get_pattern()

            while entry.has_next_excluded_pattern():
                excluded_topic = entry.next_excluded_pattern()

        except GmsecError as e:
            # handle error Config lookup error

    except GmsecError as e:
        # handle configuration file parsing error


    Parameters
    ----------
    name : Value in the name=\"\" attribute of the <SUBSCRIPTION/> block containing the subscription pattern

    Returns
    -------
    SubscriptionEntry associated with SUBSCRIPTION block

    Exceptions
    ---------
    A GmsecError is thrown if the SubcriptionEntry cannot be found.
";


%feature("docstring") gmsec::api5::ConfigFile::removeSubscriptionEntry "

    remove_subscription_entry(self, name: str) -> bool

    Removes the Subscription definition which is mapped to the provided name.

    Parameters
    ----------
    name : Name associated with the subscription.

    Returns
    -------
    True if removal was successful, False if not.
";


%feature("docstring") gmsec::api5::ConfigFile::getIterator "

    get_iterator(self) -> ConfigFileIterator

    Method that allows the callee to get the ConfigFileIterator
    associated with the ConfigFile. This iterator will allow for
    applications to iterate over the entries stored within the
    ConfigFile.  The iterator is reset each time get_iterator() is
    called.  The iterator itself is destroyed when the ConfigFile
    is destroyed.

    Note: Only one ConfigFileIterator is associated with a
    ConfigFile; multiple calls to get_iterator() will return a
    reference to the same ConfigFileIterator.  Each call will
    reset the iterator.

    Returns
    -------
    A reference to a ConfigFileIterator.
";
