This GMSEC API README file contains technical notes and the change summary
for the current release and historical releases.

See the associated Version Description Document (VDD) for further details
of each release.

For technical support regarding this release, or any previous version, please
contact the API Team at gmsec-support@lists.nasa.gov.



==============================================================================
= ADVISEMENT ALERT (for users of 32-bit RHEL 7 distro of the GMSEC API)
==============================================================================
The 32-bit RHEL 7 distro of the GMSEC API does not offer support for the
following items:

    * Middleware wrappers for activemq38 or websphere80
	* Perl binding for the GMSEC API 3.x interface
	* Python binding
	* C#/Mono binding



==============================================================================
= ADVISEMENT ALERT (for users of API 4.2.1 and later versions)
==============================================================================
Starting with API 4.2.1, support for RHEL 5 (both x86 and x86-64) has been
dropped.  It is recommended that users of RHEL 5 upgrade their systems to
RHEL 7 (or RHEL 6) if they wish to adopt the latest version of the GMSEC API.



==============================================================================
= ADVISEMENT ALERT (for users of API 4.1 and later versions)
==============================================================================
Version 4.1 of the GMSEC API introduces a new external interface binding for
Java, C, C++, and C#.  This new interface provides new functionality, including
an expanded range of capabilities for the Messaging Interface Standardization
Toolkit (MIST - please see the GMSEC API User's Guide).  This new interface 
has been optimized to require less code and result in safer operations.  While
the interface from version 3.7 and earlier is maintained, the GMSEC team 
recommends that new software make use of this new interface. 

It is also necessary for software products that use the 3.7 and earlier 
interface to upgrade to a 4.X series of the GMSEC API shared object
library, if those software products will be used on the same GMSEC bus as
clients which are using the new interface.  Since the new interface allows for
a broader range of data types, the library for the old interface has been
updated to handle the new data types.  If this library upgrade is not 
performed, existing software written with the GMSEC API will be unable to 
interpret messages with new data types. 



==============================================================================
= ADVISEMENT ALERT (for users of API 4.0)
==============================================================================
API 4.0 has been deprecated due to various issues, some of which involve the
refactoring of callback-related interfaces in the Java binding and changes to
an enumerated declaration that clashed with a C/C++ NO_ERROR macro definition
provided by Microsoft Visual Studio.

It is highly recommended that users stop using API 4.0, and instead develop
their applications using version 4.1 or greater.



==============================================================================
= GMSEC API 4.6 release notes (June 2019)
==============================================================================

--- Change summary -----------------------------------------------------------
5067    Allow for user to set custom value for COUNTER field that is used by
        Heartbeat Service
5102    Concatenate Specification's parse errors into string summary to be
        thrown
5107    Update InternalDeviceMessage::init to leverage message templates
5109    Update InternalMnemonicMessage::init to leverage message templates
5129    Simplify Message Validation's exception string
5130    MistMessage needs data constructor that takes in data + specification
5139    Define a public-facing HeartbeatGenerator class
5178    API should check tracking fields when performing message validation
5181    Replace usages of std::auto_ptr with StdUniquePtr
5183    Update API example Heartbeat Service and Resource Service programs
5185    Add logging arguments to Bolt
5186    Fix Perl 4.x documentation
5187    Add Config constructor to the C# binding that accepts a string array
5189    MistMessage setValue() methods do not indicate if Field is a header
        field
5190    Internal Product File Message consructors are not initializing class
        member data
5193    Fix documentation issues within the SWIG-generated C# binding.
5196    INSTALL.txt does not include instructions for installing libxml2 and
        libxslt under Ubuntu and macOS
5197    Include JavaDoc for Java binding within gmsecapi.jar file
5198    The JAVA_LIBS definition in GMSEC API Platform configuration files is
        incorrect for OpenJDK use
5199    Message Bus client closes stdin file descriptor when connection to
        server fails
5201    Add setName() and clone() methods to Field class
5203    Log messages are always displayed when using SWIG-generated C# binding,
        regardless of the logging level
5204    Fix documentation issues within the SWIG-generated Python3 binding
5205    Add Config() constructor to Python3 binding that accepts a string list
5206    TimeUtil.format_time() returns SwigPyObject instead of string in
        Python3 binding
5208    Update GMSEC API utility apps to use ConnectionManager
5209    Trim the directory path from filename used in log messages
5210    Display file name and line number within Python3 log messages
5211    Remove AutoMutex from Perl and Python bindings
5212    Use of the Python3 binding shutdown_all_middlewares() results in
        dead-lock
5213    Boolean Field not created correctly when XML content uses "1" for field
        value
5214    Python3 binding does not register custom log handler with Core API
5215    Message addField(String, String) in Java binding does not allow for
        empty string value
5216    C# binding does not register custom log handler with Core API
5217    Identify Config.Initialize() in C# binding as being obsolete
5218    ProductFile GetContents() can throw unmanaged exception if object has
        no data contents
5220    Resource Service should not increment COUNTER field with each sample
        taken
5221    Resource Message published by Resource Service should include PUB-RATE
        field
5223    Scan GMSEC API 4.6 using Fortify and correct/disposition any
        deficiencies found
5224    Message Bus Reader Thread does not properly destroy data buffer
5225    MSG-ID field not included within C2CX HB messages generated by the API
        when operating with the 2014 GMSEC ISD
5226    C++ log macros cannot be compiled in extended namespace
5227    Internal ValueMap does not handle GMSEC_U32 value type
5228    Java applications cannot run on macOS when SIP is enabled
5230    Use python-config and python3-config to get compiler and linker flags
5232    GMSEC API Project File improperly references certain source modules
5233    AMQP m/w wrapper fails to free resource
5234    Generic JMS m/w wrapper failes to free resources
5235    IBM MQ (formerly WebSphere) m/w wrapper does not always retry connecting
        to broker
5236    Certain example programs for the SWIG-generated language bindings do not
        demonstrate usage of release() for disposing of received messages
5237    MistMessage needs to identify header fields when applying standard
        fields to message
5238    Environment Validator does not fully check if Java is installed on macOS


==============================================================================
= GMSEC API 4.5.1 release notes (February 26 2019)
==============================================================================
--- Change summary -----------------------------------------------------------
5192    API 3.x Request Message Kind should not be altered when message is
        published
5202    TYPE attribute should be ignored if KIND attribute is provided with
        Message definition


==============================================================================
= GMSEC API 4.5 release notes (February 12 2019)
==============================================================================
o Support for Microsoft Visual Studio 2008, 2010, 2012, and 2013 has been
    dropped. Currently the API is built only with Visual Studio 2017.

o Support for Solaris 10 SPARC has been dropped.

o Support for Ubuntu 18.04 LTS is being provided.

o Java 8 (or later version) must be used to run Bolt and/or to use the Java
    binding of the GMSEC API.

o This release contains an additional interface for the C# language that can be
    used under Linux (using Mono) or under Windows.

    Please refer to the example programs in GMSEC_API/examples/csharp and the
    code documentation in GMSEC_API_DOCS/manual/csharp_4x to learn more about
    the use of the new binding.

    Note: In future releases, the separate and distinct C# .NET/CLI language
    binding will no longer be provided with the GMSEC API.

o This release contains an additional interface for the Python 3 language.
    This interface is based on the Python 3.6.5 interface.

    Please refer to the example programs in GMSEC_API/examples/python3 and the
    code documentation in GMSEC_API_DOCS/manual/python3_4x to learn more about
    the use of the new binding.

o An OpenDDS middleware wrapper is now available for Windows and RHEL7.

o An IBM MQ 9.0 middleware wrapper is now available.


--- Change summary -----------------------------------------------------------
4730    The API should be able to validate the values of fields as defined by
        the GMSEC ISD
4977    The C# LogEntry does not contain file name or line number information
        for the C# source file
5036    Provide support for IBM MQ (formerly WebSphere) 9.0
5043    Build and test the GMSEC API under Ubuntu 18.04 LTS
5052    Support message validation logic in the GMSEC API using XSD files
5075    Add SWIG-generated C# binding for Linux-based Mono projects
5076    Generate Javadoc JAR file for the Java binding of the GMSEC API
5080    Update API User's Guide to remove certain unsupported features
5085    Update API User's Guide to include all tracking field enable/disable
        options
5088    Tracking fields are not removed from C2CX Heartbeat messages
5089    The PROCESS-ID tracking field should not always be set to a U32 field
        type
5091    Upgrade Visual Studio 2008 Project/Solution files to the Visual Studio
        2017 format
5092    Messages with excluded subject patterns should not be aggregated when
        Message Binning is enabled
5094    The API does not support multi-layered schema addendums
5095    Newly defined C2CX HB message tracking fields should not be added if
        2014 or 2016 ISD is specified
5096    Allow for customizable time out period for receiving WebSphere messages
5097    API should not remove user-supplied tracking field(s) from messages
5098    StringUtil's getValue<T>() method does not always report errors when
        given bad input
5099    InternalMistMessage not converting data values correctly with setValue()
5100    Field GetType() in C#/CLI (dotnet) binding should be renamed to
        GetFieldType()
5101    MistMessage needs a constructor that can convert a Message object into
        a MistMessage object
5103    Specification (class) is doing unnecessary work when validate() is 
        called with NO_ENFORCEMENT configured
5105    Removed ConnectionManager's setStandardFields() exception specification
5110    Clarify API documentation to mention user needs to first add COUNTER
        field (to the Heartbeat Service) before the API will increment the field
5111    Bolt server displays incorrect port
5112    Remove REPLY-UNIQUE-ID from request and response messages
5113    Offer ability for users to acquire Message Specifications
5114    StringUtil::toJSON isn't escaping newline characters
5116    Fix Open-Response example programs
5118    Build the SWIG-generated C# binding of the API under Windows
5119    Refactor Specification copy-constructor
5120    Generate a 32-bit distro of the API using Visual Studio 2017
5121    Create a Python 3.x binding for the GMSEC API
5122    Rename SWIG-generated C# libraries to match GMSEC API convention
5123    Build the GMSEC API using JDK 8
5124    Setting JVM stack size proves fatal with use of Java 8
5125    Add support for IBM MQ m/w wrapper to Environment Validator and GMSEC
        Help utilities
5126    InternalConnectionManager does not need to check for viable Connection
        object
5127    The GMSEC API crashes when handling validation of a MSG.AMSG message
5133    Custom validation example programs for the GMSEC API need updating
5134    Scan GMSEC API software using HP Fortify
5135    Embellish error message when error occurs processing an XML or JSON
        field definition
5136    Add API method so a user can add a list of Field objects to a Message
5137    Implement middleware wrapper that supports OpenDDS
5138    Update API build framework to check if 3rd-party tools are installed
5140    Fix comments in Java 4.x binding to follow JavaDoc conventions
5142    InternalConnectionManager should create MistMessage messages
5143    Fix package names for JNI related classes
5144    Generate Doxygen style documentation for the SWIG-generated C# binding
5147    Connection unsubscribe() documentation should indicate that an exception
        can be thrown if error occurs
5148    Create Project and Solution files for C# example programs
5149    Update C# (dotnet) example programs to build using .NET Framework 4
5150    ActiveMQ middleware wrapper calls on unsafe ActiveMQ C++ library method
5151    Enhance gmreq utility program to allow for configurable request
        republish timeout period
5153    Exception is not handled by setHeartbeatServiceField()
5154    Update Environment Validator script for OpenDDS
5155    Update GMSEC Help utility application for OpenDDS
5156    Heartbeat Service should publish only one message when PUB-RATE is 0
        (zero)
5158    API should use CONNECTION-ENDPOINT tracking field for C2MS compliancy
5163    Fix example programs so that they compile and that they produce legal
        messages
5164    Late destruction of ConnectionManager affects Java applications relying
        on MistMessage setStandardFields()
5165    Memory leak occurs when a received message fails validation
5166    PUB-RATE cannot be changed when restarting Heartbeat Service
5167    Unable to run GMSEC API based Java application using Java 10
5168    ConfigFile should report more concise error messages
5169    A Field definition in XML representation that spans multiple lines
        cannot be parsed
5170    Drop support for IBM WebSphere 7.5
5171    Sorting fields in a Message leads to a core dump
5173    API is not identifying tracking fields as header fields
5174    Python bindings do not allow for bytearray to be used for representing
        raw binary data
5175    Provide Python applications with the means to convert a Field object
        into a more specialized field object
5179    Replace usage of javah with javac to build JNI header file
5182    Depending on middleware, API publish operations may not be thread-safe
5184    ActiveMQ m/w wrapper does not log error messages from client library


==============================================================================
= GMSEC API 4.4.2 release notes (2018 March 14)
==============================================================================
--- Change summary -----------------------------------------------------------
4933    Provide IPv6 support for Message Bus
5044    Address the sections of the API which are using the deprecated Mac
        system-level function OSAtomicAdd32()
5057    Add option to gmpub to allow it to publish forever
5070    GMSEC_TimeSpec overloaded operators should not be defined in a
        namespace
5073    Example programs should run when configured with the strictest level
        of message validation
5074    API Makefile should not attempt to build Perl and Python bindings if
        swig is unavailable
5077    The XML templates used by the API for message validation do not include
        tracking fields
5078    Product File Message class is not inserting properly named field
5081    Build 32-bit version of GMSEC API under RHEL (CentOS) 7
5082    Config object is not passed to internal API by MistMessage constructor
5083    JMSConnection constructor is not throwing an exception on error
5087    CONNECTION-ID tracking field should be a U32 field


==============================================================================
= GMSEC API 4.4.1 release notes (2018 February 6)
==============================================================================
--- Change summary -----------------------------------------------------------
4731    Add message validation to receiving operations and expand validation
        configuration options
4760    Support builds on Windows 10
4787    Provide utility app for publishing a canned GMSEC message supplied
        by the user
4798    Add support for certain Connection class methods within Connection
        Manager
4963    Add cautionary statements to API User's Guide and code documentation
        regarding Callbacks
5033    PROCESS-ID in information bus header needs to be U32 rather than I16
5039    MnemonicMessage should readily support MVAL request-type messages
5045    Report the key value in the error when attempting to retrieve a non-
        existing value from a Config object
5046    Connection state should not be CONNECTED if a connection authentication
        failure occurs
5047    Fix logic error with Bolt's TypeValidater
5048    Perl and Python bindings should not be built if SWIG_HOME is not defined
5049    Define PERL5_LIB and PYTHON_INC within API Linux configuration files
5051    Resource Info Generator should not be adding certain fields to Resource
        Message
5053    Use major.minor name convention for ActiveMQ CMS library wrapper file
5056    Provide an interface to set standard fields in MistMessage
5058    Further simplify API utility programs
5059    Exceptions should not be thrown from class destructors
5060    Allow for registration of Event Callback before initializing Connection
        Manager
5062    Certain string characters need to be escaped or converted when
        generating JSON and XML data
5063    Python example programs not handling error when user passes no args
5064    Define macro that contains hex representation of API version number
5065    Avoid assuming a synchronous request is done when a response has not
        been received following a timeout
5066    The default republish period when issuing request messages should be
        GMSEC_REQUEST_REPUBLISH_NEVER (-1)
5069    Define RPATH within all GMSEC libraries built for Linux systems
5072    ConfigFileIterator of the Java binding returns non-persistent references
        to objects


==============================================================================
= GMSEC API 4.4 release notes (2017 Dec 15)
==============================================================================
o This release contains an additional interface for the Python language.
    This interface is based on the Python 2.x interface and is built and tested
    with CPython. Please refer to the example programs in
    GMSEC_API/examples/python and the code documentation in
    GMSEC_API_DOCS/manual/python_4x to learn more about the use of the new
    binding.

--- Change summary -----------------------------------------------------------
4292    Add subscription exclusion filters to subscription definitions in
        ConfigFile
4446    Consolidate API string constants, especially config keys, into a single
        header file
4671    Execute middleware performance testing and generate a performance test
        report for GMSEC API 4.2
4740    Add a tracking field which contains a list of unique subscription
        subjects in a process
4741    Add a tracking field which shows the server to which a GMSEC Connection
        is currently connected
4758    Simplify the example programs
4791    Fix inconsistencies in the values of the FIELD_CLASS attributes in XML
        template files used for message validation
4792    Change the UNSET type attribute to VARIABLE in the XML template files
        used for message validation
4796    Fix various minor issues with the XML template files used for message
        validation
4837    Fix Magic Bus so that it handles requests for resource information
4869    Provide support for ActiveMQ CMS 3.9.4 and OpenSSL 1.0.2
4881    Update MistMessage constructors so that the Schema ID does not require
        the version number of the ISD
4898    Allow applications to register an EventCallback before connecting to
        the bus
4899    Setting the field storage container type after a message has been 
        created can truncate the message
4900    API Environment Validator script does not always report the version
        number of the API
4901    Allow users to set configuration options for messages that are received
        from the GMSEC Bus
4921    Field::getStringValue does not properly convert Binary, I8, or U8 Field
        values to strings
4934    Fix the message encoding and decoding logic which prevents Fields from
        being identified as a header
4938    Augment Field class to allow type of "BINARY" in addition to "BLOB"
        and "BIN"
4953    Rename Doxygen documentation directories for API 3.x
4955    Users shall be able to change the rate of publication of the Heartbeat
        Service
4964    JNI objects created within Callbacks need to be destroyed after use
4969    The JVM stack size should be specified by the Generic JMS wrapper
4973    The Java API's registerHandler should reset to the default Java handler
        when invoked with null
4997    Fix the memory leak in the Java Message.getXXField() functions
4999    Add a getStandardFields function to the ConnectionManager
5003    ConnectionManager::request(no callback) and reply() should be able to
        validate messages
5015    MessagePopulator class doesn't add MSG-ID field
5027    Fix the memory leak in the Java ConnectionManager requestSimpleService()
5028    Fix output from Message toXML() so that the message configuration, if
        any, is properly indented
5032    Fix the connectionManagerCreate() function in the C binding so that it
        does not enable message validation
5034    MIST ignoring lower level message templates when overriding addendum is
        applied
5035    Infinite loop occurring when addendum is added with no header listed in
        directory
5050    Mark all deprecated classes and functions of the API


==============================================================================
= GMSEC API 4.3 release notes (2017 May 1)
==============================================================================
o For Mac OS X users, this release may break binary compatibility with previous
    API 4.x releases.  This is due to a change in how the API is built on OS X.
    Previously, libstdc++ was being used; now libc++ is used.  (See item 4462
    below).  This change was made to prevent a sporadic crash from occurring
    when using various API functions.

o Beginning with API 4.3, strict validation of subjects used within Messages
    and for subscriptions will be performed to ensure that they are GMSEC-
    compliant, as defined by the GMSEC Interface Specification Document.
    Users wishing to continue using non-strict validation may employ the use
    of the "SEC-POLICY" configuration option and set it equal to the value
    "API3".

o Beginning with API 4.3, support for Solaris 10 x86 and HP-UX has been dropped.

o Users of the Java language binding should be aware that the
    Connection.disconnect() and ConnectionManager.cleanup() functions now
    throw a GMSEC_Exception.  This means that any code which calls these
    functions must be encapsulated by a try..catch statement which handles
    a GMSEC_Exception, or else the application will not be able to compile.
    Previously compiled software will still be able to run, just as it did
    prior to this change.  This change was made in response to DR #4788 
    (See below) so that exceptional states can be handled more gracefully.
    
o The Perl language binding of the API is being pared down to only include
    support for synchronous operations.  Features involving asynchronous
    operations (e.g. registration of callbacks, auto-dispatcher, etc.) have
    been removed from the binding.  Additionally, LogHandler subclassing has
    been removed.  The removal of these features is due to an instability
    discovered when executing subclass implementations in a multithreaded
    process, such as when using the AutoDispatcher or invoking a custom
    LogHandler from a non-main thread.
	
o Configurable message bins (aggregated messages) now support subscription-type
    wildcards (e.g. '>', '*', and '+')

o The ConnectionManager, MISTMessage, DeviceMessage, ProductFileMessage,
    MnemonicMessage, and various iterator classes (Collectively referred to
    as the Messaging Interface Standardization Toolkit (MIST)) have been
    added to the 4.X Perl interface.

o The Config class can now ingest and output JSON data, as well as ingest
    strings of whitespace-delimited "key=value" pairs, much like command-line
    arguments used when running example programs such as gmpub and gmsub.

--- Change summary -----------------------------------------------------------
4147    Refactor implementation of the ActiveMQ Transport Listener
4462    Build API on Mac OS X using libc++ (not libstdc++)
4512    Improve efficiency of InternalSpecification
4513    Move InternalField::getValue() to StringUtil
4514    Add setSubject function to Message class
4515    Add support for custom Message validation
4516    Expand 4.X Perl Unit Testing Coverage
4517    Add support for MIST into the 4.X Perl interface
4518    Add Device, Mnemonic and Product File Iterator support in C binding
4559    Config class should be able to ingest alternative types of String data
4561    Update gmhelp to provide bind/connect connection examples for ZeroMQ
4567    Expand OS support for 4.X Perl interface
4572    Embellish error information included with exceptions thrown from Bolt
4596    Prevent MagicBus middleware wrapper from attempting to communicate with
        MB server once it is known that the connection has been lost
4597    Cannot create a BinaryField object with the Perl interface
4598    Comparison made against configuration string value should be case
        insensitive
4600    In the C++ and C example programs, unsubscribe should only be called on
        active subscriptions
4601    Error checking should by default be performed on subscription subjects
        with wildcard operators
4610    Create a Python language binding
4615    Create example programs and unit tests for Python language binding
4616    Incorrect cast types are used in Java binding when constructing C++
        fields
4618    Users should be able to add or change Heartbeat Service fields
4619    Message template files do not use correct XML encoding syntax
4657    Field should be able to ingest JSON with numeric type syntax
4662    Implement a public toJSON() method for Config
4669    Config functions getFirst and getNext in Perl interface do not work
4694    Republish period for asynchronous request is not being properly handled
4699    Configurable Message Bin fails to handle message subject using wildcard
        element
4707    Replace use of deprecated OSAtomicAdd with alternative function for Mac
        OS X
4717    Remove references of API 3.x header files from API 4.x modules
4721    Address issues reported by HP Fortify
4729    Field.getString() should be Field.getStringValue() in the Java 4.X API
4743    Re-order field type and name when displaying Field XML information
4744    Issue Request-kind message when Connection Manager requestDirective()
        is called
4751    Make the decoding of a message packet more efficient
4768    Remove Field descriptions from validation error messages
4788    Update documentation and example programs to indicate that
        Connection::disconnect() can throw an exception in rare cases
4803    Non-ASCII Unicode characters cause String truncation when retrieving
        data from the Java language binding
4804    Improve error messages related to MnemonicMessage's dependent field
        RAW-VALUE
4807    The getField functions should report the name of the requested field
        when unable to retrieve it from a message
4809    GMSEC_Exception does not initialize base class (Exception)
4840    Fix MistMessage to allow a value of 0 (zero) for F32 fields
4853    Client applications crash when using a Magic Bus fail-over configuration
        and yet only one broker is running



==============================================================================
= GMSEC API 4.2.2 release notes (2016 December 16)
==============================================================================
o This is a patch release that includes a couple of bug fixes.

o (Fastpath) Improvements to Configurable Message Binning that allow users to
    configure which messages, if any, should be excluded from being binned.
    Users can also employ the use of wild-cards when indicating which messages
    to bin or exclude from binning.

--- Change summary -----------------------------------------------------------
4509    Add ability to exclude messages, using subject, from being added to
        Configurable Message Bins
4655    Fixed bug related to how message tracking field options are processed
        by the Connection class
4685    Resolve dead-lock issue when call to publish() is made within a callback
        that was summoned by dispatch()



==============================================================================
= GMSEC API 4.2.1 release notes (2016 November 9)
==============================================================================
o This is a patch release to correct a bug in a method that is used by the
	Time Utility functions.

--- Change summary -----------------------------------------------------------
4609    Fix StringUtil::getTimeFromString() so that it can process time stamps
        that do not include fractional time



==============================================================================
= GMSEC API 4.2 release notes (2016 October 31)
==============================================================================
o This release contains an additional interface for the Perl language.
	This new interface exists in parallel with the existing one, so all
	existing software that utilizes the GMSEC API will continue to function
	without interruption. However, new software development should utilize
	this new interface, as it is safer and leaner than the existing interface.
	
o User-defined message validation has been added to the Messaging Interface
	Standardization Toolkit (MIST) portion of the API.  The ConnectionManager
	and Specification classes can now be configured to validate user-defined
	messages, as would be defined in an addendum to the GMSEC ISD.  More
	information on this functionality is outlined in the GMSEC API User's Guide
	in the section named, "Addendums to the ISD."
	
o (Fastpath) The API's internal classes have been refactored to use Exceptions
	for error handling rather than checking on a Status object returned from
	each step of an operation.  This has in turn improved the efficiency of
	the API by reducing the number of instruction cycles the CPU must execute
	to perform operations such as publishing and receiving messages.
	
--- Change summary -----------------------------------------------------------
3844    Implement MIST Build 3
3963    Add source file name and line number to the Java LogEntry
4122    Add Perl interface for core (non-MIST) API 4.x
4156    Refactor Core API C++ Framework
4334    Fastpath: Streamline Connection publish() and receive() with seamless
        enhancements
4392    Add methods that allow users to easily get native value from a Field
        object
4396    Protect against concurrent access to the list of pending requests in
        RequestThread
4399    Prevent overwriting of EVENT-TIME field in MIST message field populator
4400    Java binding of API fails to catch exceptions thrown by the C++ layer
4435    Fastpath: Configurable Message Bins
4444    Catch exceptions in StdThread and remove include statements for API 3.x
        header files
4481    Stop Resource Message Service when ConnectionManager is destroyed
4490    Allow for XML or JSON data containing fields with an empty value to be
        parsed without error
4501    Add missing Time Utility functions to C binding of API
4506    Implement Mnemonic addSample() within JNI-layer of Java binding
4519    Increment COUNTER field in C2CX Heartbeat Messages
4537    Fix Java binding to return correct StatusClassification and StatusCode
4570    Prevent memory leak when Message class copy-constructor is called
4571    Bolt Server should report version information that correlates with the
        version of the API
4582    Fix Bolt middleware wrapper so that it does not throw an exception
        when a timeout occurs while attempting to receive a message
4591    Fix Message class to support U64 fields
4595    Make InternalConnection's insertTrackingFields() method thread-safe
4606    Report correct message kind after ingesting XML or JSON data
4607    Mitigate thread-safe issue within AMQP middleware wrapper.



==============================================================================
= GMSEC API 4.1 release notes (2016 July 25)
==============================================================================

o IMPORTANT: This version corrects a mild memory leak in the 4.X Java Callback
	handling system. All 4.X interface callbacks are now abstract classes,
	instead of interfaces. This requires all code using these classes to 
	change their reference to Callback, ReplyCallback, and EventCallback to
	use the keyword "extends" instead of "implements".
	This change was necessary to ensure that callback memory is properly cleaned
	up.
	Client programs previously built with version 4.0 or 4.0.1 of the API should
	be recompiled if the plan is to use API 4.1; otherwise undefined runtime
	behavior can be expected.

o Corrected several coding defects identified by static code analysis

--- Change summary -----------------------------------------------------------
4250    Address Fortify issues within 4.0.1 (see important note above)
4295    Incorporate memory management corrections in C# binding
4332    Config class allows merging of config objects
4353    Correct logic in autodispatcher shutdown timing
4357    API will now warn users if log file cannot be opened	
4358    C2 NSS wrapper now frees resources (Compat C2 version only)
4375    ZeroMQ wrapper now employes the zero copy concept
4385    ConnectionManager requestDirective() method was not setting appropriate
        value for RESPONSE field
4390    WebSphere middleware wrapper asynchronous status check fails to dispatch
        event
4391    MagicBus fails to deliver messages to subscribers when two brokers are
        being used



==============================================================================
= GMSEC API 4.0.1 release notes (2016 May 16)
==============================================================================

o Added new Connection and Publish-level Config options which expose the
	WebSphere MQ Asynchronous Put Response client option.  This new option
	allows users to toggle this functionality on or off and potentially
	achieve a higher throughput rate of message traffic when using
	the WebSphere MQ middleware.  For more information on this topic,
	please see the Installation and Configuration Guide Section 7.7
	as well as the current and legacy gmpub_wsmq_async example programs.

o Added the PublishWithConfig function to the 3.X C.v1 and C.v2 interfaces.

--- Change summary -----------------------------------------------------------
4274    Exposure of WebSphere MQ Asynchronous Put Response Functionality



==============================================================================
= GMSEC API 4.0 release notes (2016 April 04)
==============================================================================

o This release contains an additional interface for the Java, C, C++, and C#
 	languages. This new interface exists in parallel with the existing one, 
 	so all existing software that utilizes the GMSEC API will continue to 
 	function without interruption. However, new software development should 
 	utilize this new interface, as it is safer and leaner than the existing
 	interface.
 	
o This release also contains an expanded set of functionality for the 
	Messaging Interface Standardion Toolkit (MIST). Available for the new 
	interface only, this functionality includes validation of all GMSEC ISD
	described messages, automatic production of resource messages, and 
	simplified production of Mnemonic, Device, and Product messages. For more
	information, please see the GMSEC API User's Guide or the doxygen 
	for the ConnectionManager (available in GMSEC_API\docs\cpp_4x)

o New middleware supported with this release are Rabbit MQ via AMQP 1.0 and
	ZeroMQ. 
	
--- Change summary -----------------------------------------------------------
4171    ZeroMQ wrapper drops messages due to interrupted function calls
4146    ActiveMQ Durable Subscription functionality has been corrected to work
        with separate Connection sessions.
4136    Added ability for users to provide a Config object to a Java Subscribe
        call
4110    Add support for anonymous classes in new Java interface
4109    Add automatic flushing of cached WebSphereMQ topic handles
4069    Add ability to disable request/reply operations for a Connection
        object, to simplify topic management
4066    Correct memory leak in open response reply delivery
4047    Conform new C# interface functions to industry standard
4009    Add support for U64 field to new interface
4004    Add support for ZeroMQ middleware, see API User's Guide for details
3971    EventCallback reports the ConnectionEvent which resulted in the Callback
        invocation
3959    Repair validation error in InteralFieldDescriptor
3924    Add ability for JSON formatted GMSEC messages to be used as the 
        middleware payload
3905    Add asynchronous message publishing
3836    Add JSON support to the Message and Field interfaces
3811    Add configurable TCP buffer to Bolt middleware client
3777    Add "+" operator to allowed subscription wildcard set
3747    Allow configuration of backlog queue size for the listen-socket used by
        Bolt server
3737    Add support for ActiveMQ CMS 3.8.4
3639    Add ability for API to report asynchronous middleware events
3622    Add support for U8
3621    Correct issue with Message Bus error class reporting
3605    Normalize middleware disconnect behavior
3600    Add support for current Mac versions
3576    Add support for AMQP 1.0, tested with Rabbit MQ
3570    Correct bug in Connection::Unsubscribe(), where subscription count was
        not correctly updated
3550    Correct issue in CMSConnection::GetMWINFO() that could product a 
        segfault.
3548    Implement connection global unique identifier
3451    Correct issue with MIST loading templates on Linux
3443    Create new interface for Java, C, C++, C#
3421    Add new functional block for MIST
3411    Correct ConfigFile error handling
3377    Add exclusion filters to connection to allow simpler message filtering
2604    Add support for WebSphere MQ 8
2291    Add support for Component style configuration to ease XML configuration
        loading


[For change descriptions from prior releases of the GMSEC API, please send your
 request to gmsec-support@lists.nasa.gov]
