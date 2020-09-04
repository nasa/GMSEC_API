
// File: classlibgmsec__python_1_1ConfigFile.xml


%feature("docstring") gmsec::api::ConfigFile "

    This file contains a class for managing configuration files.

    A ConfigFile object has a one-to-many mapping to connection configurations. If a user wishes to maintain only one connection per object, the use of a Config object is suggested.

    Please see the C++ documentation for example contents of a configuration file used for ingest.


    CONSTRUCTORS:

    ConfigFile(self)

    Basic constructor that creates an empty configuration file object.

    Once the ConfigFile object has been created, use Load(const char* filepath) to load a configuration file, or if the plan is to add elements to the empty ConfigFile object, then use Save(const char* filepath, bool compact) to save any changes.
";

%feature("docstring") gmsec::api::ConfigFile::removeSubscription "

    removeSubscription(self, name) -> bool

    Removes the Subscription definition which is mapped to the provided name.

    Parameters
    ----------
    name: name associated with the subscription

    Returns
    -------
    whether successful or not

";

%feature("docstring") gmsec::api::ConfigFile::addCustomXML "

    addCustomXML(self, xml)

    Adds a custom XML string to the ConfigFile.

    Programmatic XML element addition will use pre-defined hierarchical rules for
    determining the placement of elements within the document, if saved.
    When adding a custom element, it will initially be inserted after
    the last <MESSAGE> element in the document, if no message definitions
    are present in the document, it will be added after the last <CONFIG> element
    in the document, if no other definitions are present in the document, the new
    config definition will be added after the last <SUBSCRIPTION> element in the
    document, if there are no subscription definitions then it will be added as
    the very first child of the root element.

    Note: Adding a custom XML to a newly instantiated ConfigFile object that has not loaded a document
        will create a new XML document.

    Parameters
    ----------
    xml: a custom, user-created XML string

    Exceptions
    ----------
    Exception if XML string is NULL, contains an empty string, or if an XML parsing error occurs

";

%feature("docstring") gmsec::api::ConfigFile::removeMessage "

    removeMessage(self, name) -> bool

    Removes the Message definition which is mapped to the provided name.

    Parameters
    ----------
    name: name associated with the message definition

    Returns
    -------
    whether successful or not

";

%feature("docstring") gmsec::api::ConfigFile::lookupMessage "

    lookupMessage(self, name) -> Message

    This function will attempt to find a named message in the
    config file and create a new message from that xml
    the loaded fields and values.

    Example config file load and get massage:
    try:
        cfgFile = ConfigFile()

        # load & parse configuration file
        cfgFile.load(\"/some/path/to/config.xml\")

        # attempt to get Message from ConfigFile
        try:
            msg = cfgFile.lookupMessage(\"MyMessage\")

            # do something with msg
        except Exception as me:
            # handle error Message lookup error
    except Exception as e:
        # handle configuration file parsing error

    Parameters
    ----------
    name: value in the name=\"\" attribute of the <MESSAGE/> block to seed the Message object

    Returns
    -------
    Message object that correlates with the given name

    Exceptions
    ----------
    Exception for cases where message configuration for the given name does not exist

";

%feature("docstring") gmsec::api::ConfigFile::addMessage "

    addMessage(self, name, message)

    Adds a message definition to the list of messages.
    The Message object, though passed by reference, will not be modified.

    Programmatic XML element addition will use pre-defined hierarchical rules for
    determining the placement of elements within the document, if saved.
    When adding a message element, it will initially be inserted after
    the last <MESSAGE> element in the document, if no other message definitions
    are present in the document, it will be added after the last <CONFIG> element
    in the document, if no other definitions are present in the document, the new
    config definition will be added after the last <SUBSCRIPTION> element in the
    document, if there are no subscription definitions then it will be added as
    the very first child of the root element.

    Note: Adding a message with a name that currently exists will overwrite the current message definition.
    Note: Adding a message definition to a newly instantiated ConfigFile object that has not loaded a document
        will create a new XML document.

    Parameters
    ----------
    name: name to be associated with the message definition
    message: gmsec message

    Exceptions
    ----------
    Exception if name is NULL or contains an empty string

";

%feature("docstring") gmsec::api::ConfigFile::toXML "

    toXML(self) -> char const *

    This function produces an XML config file containing all known config file tags.

    Returns
    -------
    The XML string representation of the ConfigFile object

";

%feature("docstring") gmsec::api::ConfigFile::addConfig "

    addConfig(self, name, config)

    Adds a config definition to the list of configurations.
    The Config object, though passed by reference, will not be modified.

    Programmatic XML element addition will use pre-defined hierarchical rules for
    determining the placement of elements within the document, if saved.
    When adding a config element, it will initially be inserted after
    the last <CONFIG> element in the document, if no other config definitions are
    present in the document, the new config definition will be added after the
    last <SUBSCRIPTION> element in the document, if there are no subscription
    definitions then it will be added as the very first child of the root element.

    Note: Adding a config with a name that currently exists will overwrite the current configuration definition.
    Note: Adding a config to a newly instantiated ConfigFile object that has not loaded a document
        will create a new XML document.

    Parameters
    ----------
    name: name to be associated with the configuration
    config: gmsec config

    Exceptions
    ----------
    Exception if name is NULL or contains an empty string
";

%feature("docstring") gmsec::api::ConfigFile::addSubscription "

    addSubscription(self, name, subscription)

    Adds a Subscription definition to the list of subscriptions.

    Programmatic XML element addition will use pre-defined hierarchical rules for
    determining the placement of elements within the document, if saved.
    When adding a subscription element, it will initially be inserted after
    the last <SUBSCRIPTION> element in the document, or as the very first
    child of the root element if there are no other subscription definitions.

    Note: Adding a subscription with a name that currently exists will overwrite the current subscription.
    Note: Adding a subscription definition to a newly instantiated ConfigFile object that has not loaded
        a document will create a new XML document.

    Parameters
    ----------
    name: name to be associated with the subscription
    subscription: XML string representation of a gmsec Subscription.

    Exceptions
    ----------
    Exception if either name or subscription parameters are NULL or if either contains an empty string

";

%feature("docstring") gmsec::api::ConfigFile::fromXML "

    fromXML(self, xml)

    This function parses the XML in the passed in string.
    It will return errors explaining what, if anything, went wrong.

    Exceptions
    ----------
    Exception if error occurs while parsing XML string

";

%feature("docstring") gmsec::api::ConfigFile::save "

    save(self, filePath=None, compact=False)

    This function saves the currently open configuration file to
    the location specified.
    It will return errors explaining what, if anything, went wrong.

    Parameters
    ----------
    filePath: The location to which the file will be saved;
        if no path is specified or is specified as NULL, then the path, if any, that
        was used with Load(const char* filepath) will be used.
    compact: determines whether the XML output will be compacted or not

    Exceptions
    ----------
    Exception if error occurs writing to the file

";

%feature("docstring") gmsec::api::ConfigFile::lookupConfig "

    lookupConfig(self, name) -> Config

    This function will attempt to find a named Config in the
    config file and set the passed in config object with
    the loaded values.

    Example config file load and get config:
    try:
        cfgFile = ConfigFile()

        # load & parse configuration file
        cfgFile.load(\"/some/path/to/config.xml\")

        # attempt to get Config from ConfigFile
        try:
            cfg = cfgFile.lookupConfig(\"MyConfig\")

            # do something with cfg
        except Exception as e:
            # handle error Config lookup error
    except Exception as e:
        # handle configuration file parsing error

    Parameters
    ----------
    name: value in the name=\"\" attribute of the <CONFIG/> block to seed the Config object

    Returns
    -------
    Config object that correlates with the given name

    Exceptions
    ----------
    Exception for cases where configuration for the given name does not exist
";

%feature("docstring") gmsec::api::ConfigFile::removeConfig "

    removeConfig(self, name) -> bool

    Removes the Config definition which is mapped to the provided name.

    Parameters
    ----------
    name: name associated with the configuration definition

    Returns
    -------
    whether successful or not

";

%feature("docstring") gmsec::api::ConfigFile::isLoaded "

    isLoaded(self) -> bool

    Parameters
    ----------
    self: This function will return true is the config file was successfully loaded

    Returns
    -------
    true if file loaded

";

%feature("docstring") gmsec::api::ConfigFile::load "

    load(self, filepath)

    This function opens the configuration file and parses the XML
    It will return errors explaining what, if anything, went wrong.

    Parameters
    ----------
    filepath: The location to which the file will be saved

    Exceptions
    ----------
    Exception if file parsing error occurs
";

%feature("docstring") gmsec::api::ConfigFile::removeCustomXML "

    removeCustomXML(self, xml) -> bool

    Removes the provided xml element.

    Parameters
    ----------
    xml: xml element string matching the custom xml definition

    Returns
    -------
    whether successful or not

    Exceptions
    ----------
    Exception if XML string is NULL, contains an empty string, or if an XML parsing error occurs

";

%feature("docstring") gmsec::api::ConfigFile::lookupSubscription "

    lookupSubscription(self, name) -> char const *

    This function will look up a subscription pattern defined in
    the config file. This is useful to allow easy modification of subject
    names without code changes.

    Example
    try:
        cfgFile = ConfigFile()
        pattern = ""

        # load & parse configuration file
        cfgFile.load(\"/some/path/to/config.xml\")

        # get Message from file
        pattern = cgfFile.lookupSubscription(\"MySubscription\")
        if pattern != None:
            # success
        else:
            # handle error Config lookup error
    except Exception as e:
        # handle configuration file parsing error

    Parameters
    ----------
    name: value in the name=\"\" attribute of the <SUBSCRIPTION/> block containing the subscription pattern

    Returns
    -------
    subject pattern topic associated with SUBSCRIPTION block, or NULL if not found

";

%feature("docstring") gmsec::api::ConfigFile::getIterator "

    getIterator(self) -> ConfigFileIterator

    Method that allows the callee to get the ConfigFileIterator associated with the ConfigFile.
    This iterator will allow for applications to iterate over the entries stored within the ConfigFile.
    The iterator is reset each time getIterator() is called.  The iterator itself is destroyed when the
    ConfigFile object is destroyed.

    Note: Only one ConfigFileIterator object is associated with a ConfigFile object; multiple calls to
        getIterator() will return a reference to the same ConfigFileIterator object.  Each call will reset the iterator.

    Returns
    -------
    A reference to a ConfigFileIterator object.
";
