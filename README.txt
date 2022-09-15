This GMSEC API README file contains technical notes and the change summary
for the current release and historical releases.

See the associated Version Description Document (VDD) for further details
of each release.

For technical support regarding this release, or any previous version, please
contact the API Team at gmsec-support@lists.nasa.gov.


==============================================================================
= GMSEC API 4.9.1 release notes (August 2022)
==============================================================================
o Minor bug fixes


--- Change summary -----------------------------------------------------------
* API-5587: Bolt middleware wrapper can deadlock
* API-5614: Fix logger so that carriage return is not literally interpreted
* API-5618: Very long log messages are truncated when using the C binding
* API-5692: C# binding should output log messages to standard-error
* API-5783: Fix errors in DBQUERY templates
* API-5785: Add RESPONSE field to REQ.DBQUERY message specification and template
* API-5787: Python binding should not be dependent on python3.lib
* API-5835: Heartbeat message custom field, regardless of type, is included a a STRING field
* API-5836: Fix logic used to handle multiple responses
* API-5837: Updated project files to function with Visual Studio 2019
* API-5838: Update Perl binding to build with Strawberry Perl 5.32


==============================================================================
= GMSEC API 4.9 release notes (March 2021)
==============================================================================
o Minor bug fixes
o Removed middleware wrappers for ActiveMQ 3.8.4/3.9.4 and WebSphere 8.0


--- Change summary -----------------------------------------------------------
* API-5419 : API needs to deallocate proxy ReplyCallback object used by ConnectionManager when asynchronous request is employed
* API-5420 : Add OpenDDS m/w wrapper support on RHEL/CentOS 8
* API-5446 : Bolt does not handle open-response when multiple connections are in use
* API-5448 : Removed loop condition that caused validation to prematurely stop
* API-5450 : Setting PUB-RATE in HeartbeatGenerator can lead to a seg-fault
* API-5453 : IBM MQ m/w wrapper subscriber thread does not report error when reconnect fails
* API-5454 : A 'make clean' does not remove all generated files
* API-5459 : Updated C# documentation to include reference to 2019 C2MS
* API-5461 : Header file climits not needed to build SWIG-generated bindings
* API-5463 : OpenDDS crashes (sometimes) when connection is disconnected
* API-5472 : AMQP wrapper is attempting to publish a message with an illegal subject
* API-5480 : Scan GMSEC API 4.9 using Fortify
* API-5481 : Connection getMWInfo() should always report middleware information
* API-5482 : A ReplyCallback cannot be used with Connection registerEventCallback() in the Java binding
* API-5483 : Translating C++ ConnectionEvent to comparable Java event can fail
* API-5489 : API should allow for custom specification version



==============================================================================
= GMSEC API 4.8 release notes (November 2020)
==============================================================================
o A Ruby binding for the GMSEC API is now offered

o Support for Apache Artemis is now offered

o Python3 binding is built using Python 3.8.5


--- Change summary -----------------------------------------------------------
* API-5275 : C# binding GmsecException missing constructor that accepts Status object
* API-5276 : C# binding Status missing constructor that accepts GmsecException object
* API-5277 : Java binding GMSEC_Exception missing constructor that accepts Status object
* API-5278 : Java binding Status missing constructor that accepts a GMSEC_Exception object
* API-5289 : Provide support for Apache Artemis
* API-5304 : Resource Info Generator is not adding certain fields related to network interfaces
* API-5321 : Add getConnectionEndpoint() method to Connection and ConnectionManager
* API-5323 : ConfigFile does not reset doc-loaded flag on error
* API-5324 : InternalMistMessage should not add fields while iterating over existing fields
* API-5325 : InternalMessage should avoid unnecessary copying of fields
* API-5326 : InternalMistMessage setValue() cannot convert value to BinaryField
* API-5327 : gmsub subscribing to C2MS.TERMINATE when given a custom subject
* API-5329 : Java binding's JNIFieldConverter.cloneField() does not copy header attribute
* API-5330 : Improve efficiency by capturing host name, user name, and process ID only once
* API-5331 : Need to check values/parameters in U16 and U32 classes of Java binding
* API-5334 : Improve efficiency by not forcing Field names to uppercase
* API-5344 : Memory leak in Java binding setStandardFields()
* API-5346 : API should be able to parse XML configuration file that contains lowercase Subcription, Config, and Message definitions
* API-5347 : Augment documentation for shutdownAllMiddlewares() and shutdownMiddleware()
* API-5351 : Create a Ruby binding for the GMSEC API
* API-5355 : StringUtil getValue() can return NaN on macOS when failing to parse a floating point number
* API-5356 : Config getValue() methods that accept default value should not generate error when name is NULL or empty string
* API-5358 : Provide ability for API utility apps to accept configuration file and connection config name
* API-5360 : Flatten NDM message templates
* API-5362 : Message Bus client that has lost its connection is not provided error when attempting to publish
* API-5363 : Improve efficiency of the Message addField() methods in the Java binding
* API-5364 : OpenDDS m/w wrapper is returning incorrect library root name
* API-5365 : ProductFile can throw bad_alloc when it is unable to reserve space for binary file contents
* API-5366 : Internal Heartbeat Generator should use thread-safe objects
* API-5368 : Java JNIHeartbeatGenerator constructor should not be setting MistMessage standard fields
* API-5370 : Config getDoubleValue() in Java binding is calling wrong method to acquire the value
* API-5371 : Config getNext() seg-faults when called before getFirst() when there are no entries to fetch
* API-5372 : Build the Python3 binding of the GMSEC API using Python 3.8.5 or later
* API-5373 : Fix documentation for ConnectionManager's createHeartbeatMessage and createLogMessage
* API-5374 : ConnectionManager setStandardFields() should not throw exception if list of fields is empty
* API-5396 : ZeroMQ m/w wrapper does not remove ZMQ-REPLY-ADDRESS field from request message
* API-5397 : Resource Info Generator should handle improper division when generating metrics
* API-5404 : Update Environment Validator script for Apache Artemis
* API-5406 : MistMessage setValue() should not throw exception when converting GMSEC_I64 to GMSEC_U64
* API-5409 : Scan GMSEC API 4.8 using Fortify and address any issues
* API-5411 : Create middleware wrapper for Apache Artemis
* API-5412 : Allow for authentication credentials to be used with Generic JMS
* API-5416 : API needs to handle exception that may be thrown by message decoder
* API-5417 : IBM MQ m/w wrapper should not attempt to reconnect if user indicates no retries
* API-5418 : The GMSEC API maximum encoded message size limit cannot be represented using native int type
* API-5422 : Incorrect function being referenced to get NSS error string
* API-5425 : Floating point dependency check is calling incorrect Message method to acquire value
* API-5428 : Javadoc generated for Java binding fails on class search



==============================================================================
= GMSEC API 4.7 release notes (May 2020)
==============================================================================
o Support for 32-bit distributions of the GMSEC API has been dropped.

o Java-related facets of the GMSEC API are built using OpenJDK 11. Users can continue to use Java 8 through 11 to run Java applications.

o Support for Python (2.7) has been dropped; users should use the Python3 binding instead.

o Perl binding under Windows is no longer built with Active State Perl; Strawberry Perl is being used instead.


--- Change summary -----------------------------------------------------------
* API-4766 : The API should be able to validate dependent fields as defined by C2MS and GMSEC Addendum
* API-5242 : API should follow soft-link when attempting to determine install directory
* API-5245 : Under Windows, build the Perl binding of the GMSEC API using Strawberry Perl
* API-5248 : OpenDDS m/w occassionally crashes on shutdown
* API-5249 : Retrieval of binary data is not thread-safe in C# binding
* API-5250 : Add example programs that demonstrate basic usage of C2/C2-NSS capabilities of the GMSEC API
* API-5251 : Message validation can fail when comparing supposedly equal F32 values
* API-5252 : Tracking field insertion for C2MS (2019) Heartbeat message
* API-5253 : API should use the 2019 C2MS as the default for XSD files
* API-5255 : API is not referencing C++ header files on MacOSX
* API-5256 : Optimize InternalSpecification to skip loading unused message template files
* API-5258 : Avoid the unnecessary throwing of exception within the GMSEC API
* API-5259 : Condition class reports conflicting status
* API-5260 : By default, gmreq should not be republishing request messages
* API-5261 : Typo in error message when attempting to publish message with a non-PUBLISH message kind
* API-5266 : AMQP m/w wrapper provides vague error message when it fails to initialize
* API-5268 : Validation produces malformed error message when array fields are involved
* API-5269 : Add support for GMSEC-style wildcard characters in OpenDDS m/w wrapper
* API-5270 : Allow users to register custom message validation function
* API-5271 : Functions to create Connection Manager in C binding are not handling exceptions
* API-5272 : Perl and Python3 bindings are not properly handling GMSEC API exceptions
* API-5274 : Add a hasField() method to Message class
* API-5284 : Develop string utils that can be used to validate header string, timestamp, and IP address values that go into message fields
* API-5285 : Algorithm to determine leap-year reports incorrect results
* API-5288 : The gmreq and gmrpl utilities do not create valid C2MS messages
* API-5290 : ResourceInfoGenerator should not give up if it cannot query stats on a disk
* API-5291 : Tracking fields are not cleared from message upon receiving error from m/w wrapper
* API-5292 : ConnectionManager requestDirective() methods do not add REQUEST-ID or MSG-ID
* API-5295 : Provide the means for users to use an alias in lieu of a schema ID when building a MistMessage
* API-5296 : Provide utility app to display all available message schema IDs and associated alias (if any)
* API-5297 : Fix documentation for publish() method that accepts a Config object
* API-5299 : ConnectionManager publishLog() does not include miscellaneous elements in message subject
* API-5301 : Validate Header String, IP Address, and Timestamp contents in Fields
* API-5302 : Add getSchemaLevel() method to Specification
* API-5306 : NODE and USER-NAME fields are not C2MS compliant
* API-5307 : Add C2MS-Pipeline message templates
* API-5308 : Add middleware wrapper for ActiveMQ CMS 3.9.5
* API-5309 : API does not create message templates for all schema levels
* API-5312 : Field getIntegerValue() and getUnsignedIntegerValue() report incorrect error when a value cannot be produced
* API-5313 : MistMessage does not properly convert legacy message into newer format
* API-5315 : Duplicate message in error string when validating fields in an array
* API-5316 : Validation incorrectly identifies name of field when comparing to field template in array
* API-5317 : API's JNI MistMessage should not set standard fields when field count is zero
* API-5318 : Bolt wrapper is not reporting connection endpoint when multiple connections are used
* API-5319 : Fix Python3 and C# documentation regarding ConnectionManager's setStandardFields()
* API-5332 : Message Bus includes homemade field in request message
* API-5333 : IBM MQ m/w wrapper includes homemade field in request messages
* API-5342 : API fails to handle subscription with callback with certain topics ending with '+'
* API-5345 : Do not associate a Specification object with an InternalMistMessage


==============================================================================
= GMSEC API 4.6 release notes (June 2019)
==============================================================================

--- Change summary -----------------------------------------------------------
* API-5067 : Allow for user to set custom value for COUNTER field that is used by Heartbeat Service
* API-5102 : Concatenate Specification's parse errors into string summary to be thrown
* API-5107 : Update InternalDeviceMessage::init to leverage message templates
* API-5109 : Update InternalMnemonicMessage::init to leverage message templates
* API-5129 : Simplify Message Validation's exception string
* API-5130 : MistMessage needs data constructor that takes in data + specification
* API-5139 : Define a public-facing HeartbeatGenerator class
* API-5178 : API should check tracking fields when performing message validation
* API-5181 : Replace usages of std::auto_ptr with StdUniquePtr
* API-5183 : Update API example Heartbeat Service and Resource Service programs
* API-5185 : Add logging arguments to Bolt
* API-5186 : Fix Perl 4.x documentation
* API-5187 : Add Config constructor to the C# binding that accepts a string array
* API-5189 : MistMessage setValue() methods do not indicate if Field is a header field
* API-5190 : Internal Product File Message consructors are not initializing class member data
* API-5193 : Fix documentation issues within the SWIG-generated C# binding.
* API-5196 : INSTALL.txt does not include instructions for installing libxml2 and libxslt under Ubuntu and macOS
* API-5197 : Include JavaDoc for Java binding within gmsecapi.jar file
* API-5198 : The JAVA_LIBS definition in GMSEC API Platform configuration files is incorrect for OpenJDK use
* API-5199 : Message Bus client closes stdin file descriptor when connection to server fails
* API-5201 : Add setName() and clone() methods to Field class
* API-5203 : Log messages are always displayed when using SWIG-generated C# binding, regardless of the logging level
* API-5204 : Fix documentation issues within the SWIG-generated Python3 binding
* API-5205 : Add Config() constructor to Python3 binding that accepts a string list
* API-5206 : TimeUtil.format_time() returns SwigPyObject instead of string in Python3 binding
* API-5208 : Update GMSEC API utility apps to use ConnectionManager
* API-5209 : Trim the directory path from filename used in log messages
* API-5210 : Display file name and line number within Python3 log messages
* API-5211 : Remove AutoMutex from Perl and Python bindings
* API-5212 : Use of the Python3 binding shutdown_all_middlewares() results in dead-lock
* API-5213 : Boolean Field not created correctly when XML content uses "1" for field value
* API-5214 : Python3 binding does not register custom log handler with Core API
* API-5215 : Message addField(String, String) in Java binding does not allow for empty string value
* API-5216 : C# binding does not register custom log handler with Core API
* API-5217 : Identify Config.Initialize() in C# binding as being obsolete
* API-5218 : ProductFile GetContents() can throw unmanaged exception if object has no data contents
* API-5220 : Resource Service should not increment COUNTER field with each sample taken
* API-5221 : Resource Message published by Resource Service should include PUB-RATE field
* API-5223 : Scan GMSEC API 4.6 using Fortify and correct/disposition any deficiencies found
* API-5224 : Message Bus Reader Thread does not properly destroy data buffer
* API-5225 : MSG-ID field not included within C2CX HB messages generated by the API when operating with the 2014 GMSEC ISD
* API-5226 : C++ log macros cannot be compiled in extended namespace
* API-5227 : Internal ValueMap does not handle GMSEC_U32 value type
* API-5228 : Java applications cannot run on macOS when SIP is enabled
* API-5230 : Use python-config and python3-config to get compiler and linker flags
* API-5232 : GMSEC API Project File improperly references certain source modules
* API-5233 : AMQP m/w wrapper fails to free resource
* API-5234 : Generic JMS m/w wrapper failes to free resources
* API-5235 : IBM MQ (formerly WebSphere) m/w wrapper does not always retry connecting to broker
* API-5236 : Certain example programs for the SWIG-generated language bindings do not demonstrate usage of release() for disposing of received messages
* API-5237 : MistMessage needs to identify header fields when applying standard fields to message
* API-5238 : Environment Validator does not fully check if Java is installed on macOS


==============================================================================
= GMSEC API 4.5.1 release notes (February 26 2019)
==============================================================================
--- Change summary -----------------------------------------------------------
* API-5192 : API 3.x Request Message Kind should not be altered when message is published
* API-5202 : TYPE attribute should be ignored if KIND attribute is provided with Message definition


==============================================================================
= GMSEC API 4.5 release notes (February 12 2019)
==============================================================================
o Support for Microsoft Visual Studio 2008, 2010, 2012, and 2013 has been dropped. Currently the API is built only with Visual Studio 2017.

o Support for Solaris 10 SPARC has been dropped.

o Support for Ubuntu 18.04 LTS is being provided.

o Java 8 (or later version) must be used to run Bolt and/or to use the Java binding of the GMSEC API.

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
* API-4730 : The API should be able to validate the values of fields as defined by the GMSEC ISD
* API-4977 : The C# LogEntry does not contain file name or line number information for the C# source file
* API-5036 : Provide support for IBM MQ (formerly WebSphere) 9.0
* API-5043 : Build and test the GMSEC API under Ubuntu 18.04 LTS
* API-5052 : Support message validation logic in the GMSEC API using XSD files
* API-5075 : Add SWIG-generated C# binding for Linux-based Mono projects
* API-5076 : Generate Javadoc JAR file for the Java binding of the GMSEC API
* API-5080 : Update API User's Guide to remove certain unsupported features
* API-5085 : Update API User's Guide to include all tracking field enable/disable options
* API-5088 : Tracking fields are not removed from C2CX Heartbeat messages
* API-5089 : The PROCESS-ID tracking field should not always be set to a U32 field type
* API-5091 : Upgrade Visual Studio 2008 Project/Solution files to the Visual Studio 2017 format
* API-5092 : Messages with excluded subject patterns should not be aggregated when Message Binning is enabled
* API-5094 : The API does not support multi-layered schema addendums
* API-5095 : Newly defined C2CX HB message tracking fields should not be added if 2014 or 2016 ISD is specified
* API-5096 : Allow for customizable time out period for receiving WebSphere messages
* API-5097 : API should not remove user-supplied tracking field(s) from messages
* API-5098 : StringUtil's getValue<T>() method does not always report errors when given bad input
* API-5099 : InternalMistMessage not converting data values correctly with setValue()
* API-5100 : Field GetType() in C#/CLI (dotnet) binding should be renamed to GetFieldType()
* API-5101 : MistMessage needs a constructor that can convert a Message object into a MistMessage object
* API-5103 : Specification (class) is doing unnecessary work when validate() is called with NO_ENFORCEMENT configured
* API-5105 : Removed ConnectionManager's setStandardFields() exception specification
* API-5110 : Clarify API documentation to mention user needs to first add COUNTER field (to the Heartbeat Service) before the API will increment the field
* API-5111 : Bolt server displays incorrect port
* API-5112 : Remove REPLY-UNIQUE-ID from request and response messages
* API-5113 : Offer ability for users to acquire Message Specifications
* API-5114 : StringUtil::toJSON isn't escaping newline characters
* API-5116 : Fix Open-Response example programs
* API-5118 : Build the SWIG-generated C# binding of the API under Windows
* API-5119 : Refactor Specification copy-constructor
* API-5120 : Generate a 32-bit distro of the API using Visual Studio 2017
* API-5121 : Create a Python 3.x binding for the GMSEC API
* API-5122 : Rename SWIG-generated C# libraries to match GMSEC API convention
* API-5123 : Build the GMSEC API using JDK 8
* API-5124 : Setting JVM stack size proves fatal with use of Java 8
* API-5125 : Add support for IBM MQ m/w wrapper to Environment Validator and GMSEC Help utilities
* API-5126 : InternalConnectionManager does not need to check for viable Connection object
* API-5127 : The GMSEC API crashes when handling validation of a MSG.AMSG message
* API-5133 : Custom validation example programs for the GMSEC API need updating
* API-5134 : Scan GMSEC API software using HP Fortify
* API-5135 : Embellish error message when error occurs processing an XML or JSON field definition
* API-5136 : Add API method so a user can add a list of Field objects to a Message
* API-5137 : Implement middleware wrapper that supports OpenDDS
* API-5138 : Update API build framework to check if 3rd-party tools are installed
* API-5140 : Fix comments in Java 4.x binding to follow JavaDoc conventions
* API-5142 : InternalConnectionManager should create MistMessage messages
* API-5143 : Fix package names for JNI related classes
* API-5144 : Generate Doxygen style documentation for the SWIG-generated C# binding
* API-5147 : Connection unsubscribe() documentation should indicate that an exception can be thrown if error occurs
* API-5148 : Create Project and Solution files for C# example programs
* API-5149 : Update C# (dotnet) example programs to build using .NET Framework 4
* API-5150 : ActiveMQ middleware wrapper calls on unsafe ActiveMQ C++ library method
* API-5151 : Enhance gmreq utility program to allow for configurable request republish timeout period
* API-5153 : Exception is not handled by setHeartbeatServiceField()
* API-5154 : Update Environment Validator script for OpenDDS
* API-5155 : Update GMSEC Help utility application for OpenDDS
* API-5156 : Heartbeat Service should publish only one message when PUB-RATE is 0 (zero)
* API-5158 : API should use CONNECTION-ENDPOINT tracking field for C2MS compliancy
* API-5163 : Fix example programs so that they compile and that they produce legal messages
* API-5164 : Late destruction of ConnectionManager affects Java applications relying on MistMessage setStandardFields()
* API-5165 : Memory leak occurs when a received message fails validation
* API-5166 : PUB-RATE cannot be changed when restarting Heartbeat Service
* API-5167 : Unable to run GMSEC API based Java application using Java 10
* API-5168 : ConfigFile should report more concise error messages
* API-5169 : A Field definition in XML representation that spans multiple lines cannot be parsed
* API-5170 : Drop support for IBM WebSphere 7.5
* API-5171 : Sorting fields in a Message leads to a core dump
* API-5173 : API is not identifying tracking fields as header fields
* API-5174 : Python bindings do not allow for bytearray to be used for representing raw binary data
* API-5175 : Provide Python applications with the means to convert a Field object into a more specialized field object
* API-5179 : Replace usage of javah with javac to build JNI header file
* API-5182 : Depending on middleware, API publish operations may not be thread-safe
* API-5184 : ActiveMQ m/w wrapper does not log error messages from client library


==============================================================================
= GMSEC API 4.4.2 release notes (2018 March 14)
==============================================================================
--- Change summary -----------------------------------------------------------
* API-4933 : Provide IPv6 support for Message Bus
* API-5044 : Address the sections of the API which are using the deprecated Mac system-level function OSAtomicAdd32()
* API-5057 : Add option to gmpub to allow it to publish forever
* API-5070 : GMSEC_TimeSpec overloaded operators should not be defined in a namespace
* API-5073 : Example programs should run when configured with the strictest level of message validation
* API-5074 : API Makefile should not attempt to build Perl and Python bindings if swig is unavailable
* API-5077 : The XML templates used by the API for message validation do not include tracking fields
* API-5078 : Product File Message class is not inserting properly named field
* API-5081 : Build 32-bit version of GMSEC API under RHEL (CentOS) 7
* API-5082 : Config object is not passed to internal API by MistMessage constructor
* API-5083 : JMSConnection constructor is not throwing an exception on error
* API-5087 : CONNECTION-ID tracking field should be a U32 field


==============================================================================
= GMSEC API 4.4.1 release notes (2018 February 6)
==============================================================================
--- Change summary -----------------------------------------------------------
* API-4731 : Add message validation to receiving operations and expand validation configuration options
* API-4760 : Support builds on Windows 10
* API-4787 : Provide utility app for publishing a canned GMSEC message supplied by the user
* API-4798 : Add support for certain Connection class methods within Connection Manager
* API-4963 : Add cautionary statements to API User's Guide and code documentation regarding Callbacks
* API-5033 : PROCESS-ID in information bus header needs to be U32 rather than I16
* API-5039 : MnemonicMessage should readily support MVAL request-type messages
* API-5045 : Report the key value in the error when attempting to retrieve a non-existing value from a Config object
* API-5046 : Connection state should not be CONNECTED if a connection authentication failure occurs
* API-5047 : Fix logic error with Bolt's TypeValidater
* API-5048 : Perl and Python bindings should not be built if SWIG_HOME is not defined
* API-5049 : Define PERL5_LIB and PYTHON_INC within API Linux configuration files
* API-5051 : Resource Info Generator should not be adding certain fields to Resource Message
* API-5053 : Use major.minor name convention for ActiveMQ CMS library wrapper file
* API-5056 : Provide an interface to set standard fields in MistMessage
* API-5058 : Further simplify API utility programs
* API-5059 : Exceptions should not be thrown from class destructors
* API-5060 : Allow for registration of Event Callback before initializing Connection Manager
* API-5062 : Certain string characters need to be escaped or converted when generating JSON and XML data
* API-5063 : Python example programs not handling error when user passes no args
* API-5064 : Define macro that contains hex representation of API version number
* API-5065 : Avoid assuming a synchronous request is done when a response has not been received following a timeout
* API-5066 : The default republish period when issuing request messages should be GMSEC_REQUEST_REPUBLISH_NEVER (-1)
* API-5069 : Define RPATH within all GMSEC libraries built for Linux systems
* API-5072 : ConfigFileIterator of the Java binding returns non-persistent references to objects


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
* API-4292 : Add subscription exclusion filters to subscription definitions in ConfigFile
* API-4446 : Consolidate API string constants, especially config keys, into a single header file
* API-4671 : Execute middleware performance testing and generate a performance test report for GMSEC API 4.2
* API-4740 : Add a tracking field which contains a list of unique subscription subjects in a process
* API-4741 : Add a tracking field which shows the server to which a GMSEC Connection is currently connected
* API-4758 : Simplify the example programs
* API-4791 : Fix inconsistencies in the values of the FIELD_CLASS attributes in XML template files used for message validation
* API-4792 : Change the UNSET type attribute to VARIABLE in the XML template files used for message validation
* API-4796 : Fix various minor issues with the XML template files used for message validation
* API-4837 : Fix Magic Bus so that it handles requests for resource information
* API-4869 : Provide support for ActiveMQ CMS 3.9.4 and OpenSSL 1.0.2
* API-4881 : Update MistMessage constructors so that the Schema ID does not require the version number of the ISD
* API-4898 : Allow applications to register an EventCallback before connecting to the bus
* API-4899 : Setting the field storage container type after a message has been created can truncate the message
* API-4900 : API Environment Validator script does not always report the version number of the API
* API-4901 : Allow users to set configuration options for messages that are received from the GMSEC Bus
* API-4921 : Field::getStringValue does not properly convert Binary, I8, or U8 Field values to strings
* API-4934 : Fix the message encoding and decoding logic which prevents Fields from being identified as a header
* API-4938 : Augment Field class to allow type of "BINARY" in addition to "BLOB" and "BIN"
* API-4953 : Rename Doxygen documentation directories for API 3.x
* API-4955 : Users shall be able to change the rate of publication of the Heartbeat Service
* API-4964 : JNI objects created within Callbacks need to be destroyed after use
* API-4969 : The JVM stack size should be specified by the Generic JMS wrapper
* API-4973 : The Java API's registerHandler should reset to the default Java handler when invoked with null
* API-4997 : Fix the memory leak in the Java Message.getXXField() functions
* API-4999 : Add a getStandardFields function to the ConnectionManager
* API-5003 : ConnectionManager::request(no callback) and reply() should be able to validate messages
* API-5015 : MessagePopulator class doesn't add MSG-ID field
* API-5027 : Fix the memory leak in the Java ConnectionManager requestSimpleService()
* API-5028 : Fix output from Message toXML() so that the message configuration, if any, is properly indented
* API-5032 : Fix the connectionManagerCreate() function in the C binding so that it does not enable message validation
* API-5034 : MIST ignoring lower level message templates when overriding addendum is applied
* API-5035 : Infinite loop occurring when addendum is added with no header listed in directory
* API-5050 : Mark all deprecated classes and functions of the API


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
	
o Configurable message bins (aggregated messages) now support subscription-type wildcards (e.g. '>', '*', and '+')

o The ConnectionManager, MISTMessage, DeviceMessage, ProductFileMessage,
    MnemonicMessage, and various iterator classes (Collectively referred to
    as the Messaging Interface Standardization Toolkit (MIST)) have been
    added to the 4.X Perl interface.

o The Config class can now ingest and output JSON data, as well as ingest
    strings of whitespace-delimited "key=value" pairs, much like command-line
    arguments used when running example programs such as gmpub and gmsub.

--- Change summary -----------------------------------------------------------
* API-4147 : Refactor implementation of the ActiveMQ Transport Listener
* API-4462 : Build API on Mac OS X using libc++ (not libstdc++)
* API-4512 : Improve efficiency of InternalSpecification
* API-4513 : Move InternalField::getValue() to StringUtil
* API-4514 : Add setSubject function to Message class
* API-4515 : Add support for custom Message validation
* API-4516 : Expand 4.X Perl Unit Testing Coverage
* API-4517 : Add support for MIST into the 4.X Perl interface
* API-4518 : Add Device, Mnemonic and Product File Iterator support in C binding
* API-4559 : Config class should be able to ingest alternative types of String data
* API-4561 : Update gmhelp to provide bind/connect connection examples for ZeroMQ
* API-4567 : Expand OS support for 4.X Perl interface
* API-4572 : Embellish error information included with exceptions thrown from Bolt
* API-4596 : Prevent MagicBus middleware wrapper from attempting to communicate with MB server once it is known that the connection has been lost
* API-4597 : Cannot create a BinaryField object with the Perl interface
* API-4598 : Comparison made against configuration string value should be case insensitive
* API-4600 : In the C++ and C example programs, unsubscribe should only be called on active subscriptions
* API-4601 : Error checking should by default be performed on subscription subjects with wildcard operators
* API-4610 : Create a Python language binding
* API-4615 : Create example programs and unit tests for Python language binding
* API-4616 : Incorrect cast types are used in Java binding when constructing C++ fields
* API-4618 : Users should be able to add or change Heartbeat Service fields
* API-4619 : Message template files do not use correct XML encoding syntax
* API-4657 : Field should be able to ingest JSON with numeric type syntax
* API-4662 : Implement a public toJSON() method for Config
* API-4669 : Config functions getFirst and getNext in Perl interface do not work
* API-4694 : Republish period for asynchronous request is not being properly handled
* API-4699 : Configurable Message Bin fails to handle message subject using wildcard element
* API-4707 : Replace use of deprecated OSAtomicAdd with alternative function for Mac OS X
* API-4717 : Remove references of API 3.x header files from API 4.x modules
* API-4721 : Address issues reported by HP Fortify
* API-4729 : Field.getString() should be Field.getStringValue() in the Java 4.X API
* API-4743 : Re-order field type and name when displaying Field XML information
* API-4744 : Issue Request-kind message when Connection Manager requestDirective() is called
* API-4751 : Make the decoding of a message packet more efficient
* API-4768 : Remove Field descriptions from validation error messages
* API-4788 : Update documentation and example programs to indicate that Connection::disconnect() can throw an exception in rare cases
* API-4803 : Non-ASCII Unicode characters cause String truncation when retrieving data from the Java language binding
* API-4804 : Improve error messages related to MnemonicMessage's dependent field RAW-VALUE
* API-4807 : The getField functions should report the name of the requested field when unable to retrieve it from a message
* API-4809 : GMSEC_Exception does not initialize base class (Exception)
* API-4840 : Fix MistMessage to allow a value of 0 (zero) for F32 fields
* API-4853 : Client applications crash when using a Magic Bus fail-over configuration and yet only one broker is running



==============================================================================
= GMSEC API 4.2.2 release notes (2016 December 16)
==============================================================================
o This is a patch release that includes a couple of bug fixes.

o (Fastpath) Improvements to Configurable Message Binning that allow users to
    configure which messages, if any, should be excluded from being binned.
    Users can also employ the use of wild-cards when indicating which messages
    to bin or exclude from binning.

--- Change summary -----------------------------------------------------------
* API-4509 : Add ability to exclude messages, using subject, from being added to Configurable Message Bins
* API-4655 : Fixed bug related to how message tracking field options are processed by the Connection class
* API-4685 : Resolve dead-lock issue when call to publish() is made within a callback that was summoned by dispatch()



==============================================================================
= GMSEC API 4.2.1 release notes (2016 November 9)
==============================================================================
o This is a patch release to correct a bug in a method that is used by the Time Utility functions.

--- Change summary -----------------------------------------------------------
* API-4609 : Fix StringUtil::getTimeFromString() so that it can process time stamps that do not include fractional time



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
* API-3844 : Implement MIST Build 3
* API-3963 : Add source file name and line number to the Java LogEntry
* API-4122 : Add Perl interface for core (non-MIST) API 4.x
* API-4156 : Refactor Core API C++ Framework
* API-4334 : Fastpath: Streamline Connection publish() and receive() with seamless enhancements
* API-4392 : Add methods that allow users to easily get native value from a Field object
* API-4396 : Protect against concurrent access to the list of pending requests in RequestThread
* API-4399 : Prevent overwriting of EVENT-TIME field in MIST message field populator
* API-4400 : Java binding of API fails to catch exceptions thrown by the C++ layer
* API-4435 : Fastpath: Configurable Message Bins
* API-4444 : Catch exceptions in StdThread and remove include statements for API 3.x header files
* API-4481 : Stop Resource Message Service when ConnectionManager is destroyed
* API-4490 : Allow for XML or JSON data containing fields with an empty value to be parsed without error
* API-4501 : Add missing Time Utility functions to C binding of API
* API-4506 : Implement Mnemonic addSample() within JNI-layer of Java binding
* API-4519 : Increment COUNTER field in C2CX Heartbeat Messages
* API-4537 : Fix Java binding to return correct StatusClassification and StatusCode
* API-4570 : Prevent memory leak when Message class copy-constructor is called
* API-4571 : Bolt Server should report version information that correlates with the version of the API
* API-4582 : Fix Bolt middleware wrapper so that it does not throw an exception when a timeout occurs while attempting to receive a message
* API-4591 : Fix Message class to support U64 fields
* API-4595 : Make InternalConnection's insertTrackingFields() method thread-safe
* API-4606 : Report correct message kind after ingesting XML or JSON data
* API-4607 : Mitigate thread-safe issue within AMQP middleware wrapper.



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
* API-4250 : Address Fortify issues within 4.0.1 (see important note above)
* API-4295 : Incorporate memory management corrections in C# binding
* API-4332 : Config class allows merging of config objects
* API-4353 : Correct logic in autodispatcher shutdown timing
* API-4357 : API will now warn users if log file cannot be opened	
* API-4358 : C2 NSS wrapper now frees resources (Compat C2 version only)
* API-4375 : ZeroMQ wrapper now employes the zero copy concept
* API-4385 : ConnectionManager requestDirective() method was not setting appropriate value for RESPONSE field
* API-4390 : WebSphere middleware wrapper asynchronous status check fails to dispatch event
* API-4391 : MagicBus fails to deliver messages to subscribers when two brokers are being used



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
* API-4274 : Exposure of WebSphere MQ Asynchronous Put Response Functionality



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

o New middleware supported with this release are Rabbit MQ via AMQP 1.0 and ZeroMQ. 
	
--- Change summary -----------------------------------------------------------
* API-4171 : ZeroMQ wrapper drops messages due to interrupted function calls
* API-4146 : ActiveMQ Durable Subscription functionality has been corrected to work with separate Connection sessions.
* API-4136 : Added ability for users to provide a Config object to a Java Subscribe call
* API-4110 : Add support for anonymous classes in new Java interface
* API-4109 : Add automatic flushing of cached WebSphereMQ topic handles
* API-4069 : Add ability to disable request/reply operations for a Connection object, to simplify topic management
* API-4066 : Correct memory leak in open response reply delivery
* API-4047 : Conform new C# interface functions to industry standard
* API-4009 : Add support for U64 field to new interface
* API-4004 : Add support for ZeroMQ middleware, see API User's Guide for details
* API-3971 : EventCallback reports the ConnectionEvent which resulted in the Callback invocation
* API-3959 : Repair validation error in InteralFieldDescriptor
* API-3924 : Add ability for JSON formatted GMSEC messages to be used as the middleware payload
* API-3905 : Add asynchronous message publishing
* API-3836 : Add JSON support to the Message and Field interfaces
* API-3811 : Add configurable TCP buffer to Bolt middleware client
* API-3777 : Add "+" operator to allowed subscription wildcard set
* API-3747 : Allow configuration of backlog queue size for the listen-socket used by Bolt server
* API-3737 : Add support for ActiveMQ CMS 3.8.4
* API-3639 : Add ability for API to report asynchronous middleware events
* API-3622 : Add support for U8
* API-3621 : Correct issue with Message Bus error class reporting
* API-3605 : Normalize middleware disconnect behavior
* API-3600 : Add support for current Mac versions
* API-3576 : Add support for AMQP 1.0, tested with Rabbit MQ
* API-3570 : Correct bug in Connection::Unsubscribe(), where subscription count was not correctly updated
* API-3550 : Correct issue in CMSConnection::GetMWINFO() that could product a segfault.
* API-3548 : Implement connection global unique identifier
* API-3451 : Correct issue with MIST loading templates on Linux
* API-3443 : Create new interface for Java, C, C++, C#
* API-3421 : Add new functional block for MIST
* API-3411 : Correct ConfigFile error handling
* API-3377 : Add exclusion filters to connection to allow simpler message filtering
* API-2604 : Add support for WebSphere MQ 8
* API-2291 : Add support for Component style configuration to ease XML configuration loading


[For change descriptions from prior releases of the GMSEC API, please send your request to gmsec-support@lists.nasa.gov]
