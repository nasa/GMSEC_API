This GMSEC API README file contains technical notes and the change summary
for the current release and historical releases.

See the associated Version Description Document (VDD) for further details
of each release.



==============================================================================
= ADVISEMENT ALERT (for users of API 4.1 and later versions)
==============================================================================
Version 4.1 of the GMSEC API introduces a new external interface binding for
Java, C, C++, and C#. This new interface provides new functionality, including
an expanded range of capabilities for the Messaging Interface Standardization
Toolkit (MIST - please see the GMSEC API User's Guide). This new interface 
has been optimized to require less code and result in safer operations. While
the interface from version 3.7 and earlier is maintained, the GMSEC team 
recommends that new software make use of this new interface. 

It is also necessary for software products that use the 3.7 and earlier 
interface to upgrade the linked version of the GMSEC API shared object library
to 4.0, if those software products will be used on the same GMSEC bus as 
clients which are using the new interface. Since the new interface allows for
a broader range of data types, the library for the old interface has been
updated to handle the new data types. If this library upgrade is not 
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
= ADVISEMENT ALERT (for users of API 3.4 and earlier versions)
==============================================================================
For users that have systems configured with API 3.4 or earlier versions, and
are also using API 3.5 or later, there is a risk that subscriber clients may
encounter message loss.

The message loss could potentially occur if the underlying API used by the
subscriber client detects that a new message contains a UNIQUE-ID value which
is identical to a value that was previously received in another message.

With API 3.5 and later releases, duplicate message filtering is enabled by
default.  This feature can be disabled by setting MW-FILTER-DUPS=NO within
your connection configuration options.  Disabling duplicate messsage filtering
may mitigate potential message loss.  While this feature does not present a
concern to clients of version 3.5 and later, earlier versions use a less
specific UNIQUE-ID that presents a higher probability of collision.

For users that employ the same release of the API across all their systems,
the message loss issue is highly unlikely to occur.

For additional information regarding this issue, please contact the API Team
at gmsec-support@lists.nasa.gov.



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
3844	Implement MIST Build 3
3963	Add source file name and line number to the Java LogEntry
4122	Add Perl interface for core (non-MIST) API 4.x
4156	Refactor Core API C++ Framework
4334	Fastpath: Streamline Connection publish() and receive() with seamless
		enhancements
4392	Add methods that allow users to easily get native value from a Field
		object
4396	Protect against concurrent access to the list of pending requests in
		RequestThread
4399	Prevent overwriting of EVENT-TIME field in MIST message field populator
4400	Java binding of API fails to catch exceptions thrown by the C++ layer
4435	Fastpath: Configurable Message Bins
4444	Catch exceptions in StdThread and remove include statements for API 3.x
		header files
4481	Stop Resource Message Service when ConnectionManager is destroyed
4490	Allow for XML or JSON data containing fields with an empty value to be
		parsed without error
4501	Add missing Time Utility functions to C binding of API
4506	Implement Mnemonic addSample() within JNI-layer of Java binding
4519	Increment COUNTER field in C2CX Heartbeat Messages
4537	Fix Java binding to return correct StatusClassification and StatusCode
4570	Prevent memory leak when Message class copy-constructor is called
4571	Bolt Server should report version information that correlates with the
		version of the API
4582	Fix Bolt middleware wrapper so that it does not throw an exception
		when a timeout occurs while attempting to receive a message
4591	Fix Message class to support U64 fields
4595	Make InternalConnection's insertTrackingFields() method thread-safe
4606	Report correct message kind after ingesting XML or JSON data
4607	Mitigate thread-safe issue within AMQP middleware wrapper.



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
4250	Address Fortify issues within 4.0.1 (see important note above)
4295	Incorporate memory management corrections in C# binding
4332	Config class allows merging of config objects
4353	Correct logic in autodispatcher shutdown timing
4357	API will now warn users if log file cannot be opened	
4358	C2 NSS wrapper now frees resources (Compat C2 version only)
4375	ZeroMQ wrapper now employes the zero copy concept
4385	ConnectionManager requestDirective() method was not setting 
		appropriate value for RESPONSE field
4390	WebSphere middleware wrapper asynchronous status check fails
		to dispatch event
4391	MagicBus fails to deliver messages to subscribers when two
		brokers are being used



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
ID	    Description
4171    ZeroMQ wrapper drops messages due to interrupted function calls
4146    ActiveMQ Durable Subscription functionality has been corrected to work
		with separate Connection sessions.
4136    Added ability for users to provide a Config object to a Java
		Subscribe call
4110  	Add support for anonymous classes in new Java interface
4109	Add automatic flushing of cached WebSphereMQ topic handles
4069	Add ability to disable request/reply operations for a Connection
		object, to simplify topic management
4066	Correct memory leak in open response reply delivery
4047	Conform new C# interface functions to industry standard
4009	Add support for U64 field to new interface
4004 	Add support for ZeroMQ middleware, see API User's Guide for details
3971	EventCallback reports the ConnectionEvent which resulted in the
		the Callback invocation
3959	Repair validation error in InteralFieldDescriptor
3924	Add ability for JSON formatted GMSEC messages to be used as the 
		middleware payload
3905 	Add asynchronous message publishing
3836	Add JSON support to the Message and Field interfaces
3811	Add configurable TCP buffer to Bolt middleware client
3777	Add "+" operator to allowed subscription wildcard set
3747	Allow configuration of backlog queue size for the listen-socket
		used by Bolt server
3737	Add support for ActiveMQ CMS 3.8.4
3639	Add ability for API to report asynchronous middleware events
3622	Add support for U8
3621	Correct issue with Message Bus error class reporting
3605	Normalize middleware disconnect behavior
3600	Add support for current Mac versions
3576	Add support for AMQP 1.0, tested with Rabbit MQ
3570	Correct bug in Connection::Unsubscribe(), where subscription count
		was not correctly updated
3550	Correct issue in CMSConnection::GetMWINFO() that could product a 
		segfault.
3548	Implement connection global unique identifier
3451	Correct issue with MIST loading templates on Linux
3443	Create new interface for Java, C, C++, C#
3421	Add new functional block for MIST
3411	Correct ConfigFile error handling
3377	Add exclusion filters to connection to allow simpler message
		filtering
2604	Add support for WebSphere MQ 8
2291	Add support for Component style configuration to ease XML
		configuration loading
	
 

==============================================================================
= GMSEC API 3.7.2 release notes (2016 Jan 29)
==============================================================================

o Adds automatic topic handle management for the WebSphere MQ wrapper. The
	wrapper will cache topic handles up to a specified limit, which defaults
	to 100, and then flush the cache. Caching the topic handles streamlines
	the publishing by decreasing function execution time. At the default 
	value of 100, this flush operation should take no more than 50 
	milliseconds. If this introduces undesirable latency spikes, the flush
	limit can be specified with the Config parameter MW-MAX-TOPIC-HANDLES.
	
o This release contains a Beta release of the refactored C#,
	C, Java, and C++ interfaces for the GMSEC API. These new interfaces
	are under active development, and are made available only for review
	and references purposes, as they are subject to change until the 
	Spring 2016 release of GMSEC API 4.0



==============================================================================
= GMSEC API 3.7.1 release notes (2015 Oct 30)
==============================================================================

o Added a new feature called Asynchronous Publish. This feature allows a 
	publishing program to give a message to the GMSEC connection for
	publishing, but then return before the message is encoded and sent to
	the middleware. The message is duplicated and the duplicate is placed
	in a queue for asynchronous publishing, leaving the user thread to
	return and modify or clean up the original message. 
	In lab testing, this has been shown to result in an
	increase of message throughput of approximately 30% on quad core 
	computer systems. The following Config parametersare used to configure
	this behavior:

	To enable the feature:
	GMSEC-ASYNC-PUBLISH			=	TRUE or FALSE

	To determine the number of messages that may be queued before the 
	user thread will block on publish
	GMSEC-ASYNC-PUBLISH-QUEUE-DEPTH		=	(default, 1000)

	To determine the number of milliseconds which will be allowed on 
	Connection::Disconnect for queued messages to be published:
	GMSEC-ASYNC-PUBLISH-TEARDOWN-WAIT	=	(default, 1000)
	
o This release contains an Alpha release of the refactored
	C, Java, and C++ interfaces for the GMSEC API. These new interfaces
	are under active development, and are made available only for review
	and references purposes, as they are subject to change until the 
	Spring 2016 release of GMSEC API 4.0



==============================================================================
= GMSEC API 3.7 release notes (2015 April 3)
==============================================================================

o Added new module called the Message Interface Standardization Toolkit, 
	Build 1. This new module added publish-time validation of messages and 
	services for publishing log and heartbeat messages. More information can 
	be found in the User's Guide.
	

--- Change summary -----------------------------------------------------------
ID	    Description
1284    Capability to write to the configuration xml file
1147    1147 & 1232 have been distilled into the Message Interface
1232	Standardization Toolkit, Build 1.
2523	Create Generic Authorization Rules Generator
1221	API performance has been tuned via reference counting
3347	Redesign of gmconfig example program
2502	API Environment Validation scripts should display API version number
1970	Resolve safety issues with GetMWInfo function
2387	Resolve Spring 2014 Fraunhoffer-identified defects
2503	The GMSEC Environment Validation script should check dependencies 
		of the GMSEC JNI library
2506	Add color-coded output for the Windows-version of the GMSEC 
		Environment Validation script.
2252	API error for DLL lookup failure was corrected
2279	API should handle 3rd-party errors when attempting connection to
		middlewares such as generic_jms
2520	Loading a message definition using FromXML can potentially set 
		the message kind twice
2276	Corrected error in GMSEC build framework
2217	Corrected improper function invocations in Bolt code
2227	Corrected disparity in WebSphere GetNextMsg decryption failure
		reporting
1845	API Environment Configuration Tool and gorun issues
2190	Perl DispatchMsg results in a seg fault
1139	Corrected error where WebSphere wrapper goes into tight loop
		if server shut down in a certain way
2606	Under certain circumstances, Perl callbacks could be lost		
2517	GMSEC API C++ example programs were not built properly when
		using Clang compiler.
2570	GMSEC-API C++ Debug Mode issue on Visdual Studio 2010
2655	GetNextMsg with 0 timeout returned different status with 
		websphere75 than with other wrappers
3337	Message Bus gives up too easily when attempting to send message size
		via TCP socket.
2693	BaseConnection Auto-dispatch called multiple times when being stopped
2540	Error using overlapping subscriptions with WebSphere and NSS library
3353	API was storing Process ID within GMSEC Messages as a GMSEC_U16 field, 
		when in fact per the GMSEC Spec, it should be a GMSEC_I16 field.
3360	GMSEC API ActiveMQ C++ adapter did not properly handle username and 
		password connection parameters for non-trivial broker URIs
1690	Bolt failed coarse performance testing because of read/write problems
3326	Generic Rule Converter does not Properly Translate Wildcards for 
		WebSphere
3362	Refactor the JMSConnection destructor such that Disconnect is called 
		(if it has not already been called)
3396	Java binding is missing certain Status class enumerated constants 
		that are available in the C++ binding
3397	Field copy-constructor in Java binding can lead to Null Exception if 
		object being copied is not fully initialized
3398	Field method UnSet() in Java binding does not reset the name of 
		the Field.
3407	Minor issues with API's Logging framework
3408	Java binding Log class should check to ensure string parameters 
		which are passed to methods are non-null
3428	BaseStatus setString() method should check to determine if the 
		string being given has already been set.
3433	Windows version of the Environment Validator fails to report that 
		directory referenced by JAVA_HOME does not exist.



==============================================================================
= GMSEC API 3.6.4 release notes (2014 October 13)
==============================================================================

o Added new Publish and Subscribe functions in the public interface which
    accept a Config object in order to provide access to ActiveMQ's Durable
    Producer and Consumer functions.
o Fixed process ID field type within GMSEC Message.


--- Change summary -----------------------------------------------------------
ID	    Description
3297    Add Support for Durable Consumers and Producers in ActiveMQ Wrapper
3353    API is storing process ID within GMSEC Messages as GMSEC_U16 field,
        when in fact per the GMSEC Spec, it should be a GMSEC_I16 field.



==============================================================================
= GMSEC API 3.6.3 release notes (2014 September 22)
==============================================================================

o GMSEC Message size is now properly computed to reflect actual message
  payload size.
o Support for ActiveMQ's CPP wrapper version 3.8.3 has been added.
o Fixed bug with ActiveMQ and WebSphere GMSEC wrapper interfaces.
o Environment Validation tool to verifies m/w library compatibility with
  system.


--- Change summary -----------------------------------------------------------
ID	    Description
2217    Illegal operations in Bolt code.
3299    Change GMSEC Message Size Calculation from XML size to actual message
        payload size.
3300    Add support for ActiveMQ CPP 3.8.3.
3316	Fix ActiveMQ and WebSphere GMSEC wrappers so that reply message
        listener is set up only once.
3324    API Environment Validation scripts should check to ensure that
        middleware libraries are compatible with the system.



==============================================================================
= GMSEC API 3.6.2 release notes (2014 August 14)
==============================================================================

o Patch release offers new Performance Logging and Audit support. Users may 
now use the configuration flag GMSEC-LOG-PERFORMANCE to specify a file which
will be created (or appended to, if the file exists) for the purpose of 
logging message latencies. For example:

gmpub.exe connectionType=gmsec_bolt GMSEC-LOG-PERFORMANCE=audit.log

The format for file will be as such:

(Message #1 Subject),(Message #1 latency),(Message #1 Publish Time),(Message 
	#1 Receive Time)
(Message #2 Subject),(Message #2 latency),(Message #2 Publish Time),(Message 
	#2 Receive Time)
	
Message latencies are calculated to the nearest millisecond, sub-millisecond
precision is not available. The system is also dependent on system clock 
synchronization through a common time service, and thus the calculated
latencies will be limited in accuracy by the degree to which the clocks
are synchronized.

For example, if System A sends a message to System B, which takes five
milliseconds to be received, but the system clock on System B is three
milliseconds ahead of the system clock on System A, the total transmission
latency will be reported as eight milliseconds.

--- Change summary -----------------------------------------------------------
ID	Description
2618	Add Capability for API to Log Message Latencies



==============================================================================
= GMSEC API 3.6.1 release notes (2014 August 1)
==============================================================================

o Patch release offers Open-Response support so that message subjects associ-
ated with Request/Reply messages are not auto-mapped to a system generated
string.  To enable the new Open-Response feature, client applications should
specify a connection configuration option of GMSEC-REQ-RESP=OPEN-RESP.  In
addition, the requester client MUST subscribe to the reply message subject
that the replier will issue.

--- Change summary -----------------------------------------------------------
ID	Description
1992	Provide Open-Response support so that message subjects are not mapped.



==============================================================================
= GMSEC API 3.6 release notes (2014 April 28)
==============================================================================

o Potential binary incompatibility for C++ Visual Studio applications.
The gmsec::util::String class introduced in release 3.5 mistakenly included the
const qualifier on the return type of the size, length, and empty methods. That
has been fixed, but the impact is that a C++ application compiled with Visual
Studio against the GMSEC API 3.5.* headers which uses one those methods will
not run with GMSEC API 3.6 libraries.

o Version 3.5.1 of the GMSEC API removed inadvertently the function FromXML(
org.w3c.dom.Node) from the Java binding's implementation of the Config class.
This function was not intended to be exposed in the external API, however it
 has been restored in version 3.6 for any users that may utilize it. The 
 function is now considered deprecated and users are advised to discontinue use.

--- Change summary -----------------------------------------------------------
ID	Description
1562	Provide middleware independent compression option
1170	Optimized compilation
1286	Provide for GMSEC API version compatibility check between clients 
		and servers
1149	Enhance API Environment Validator
1248	Seperate Read/Write thread safety
1185	Enhance Example programs
1175	Normalize C# Example Programs
1813	gmhelp - Embedded helper program
1837	Stand-Alone GMSEC API Environment Validator
1967	Absolute Path Specification for Linux
2071	Custom Code to Java, Perl, C
2222	Add Support for VS 2012
2216	Support Mac OS-X with Clang compiler
2296	OpenSSL Upgrade
1180	Xerces Redundant Code
2037	Compiler Warning Correction
1233	Class Copy Protection
1236	Latency Spike at beginning of PTU graphs
1151	C (v1 and v2) examples bug in example_millisleep
1152	Example Program Corrections
1153	WebSphere error codes not displayed in error messages
1155	Issue with ReplyCallbacks in C# Wrapper
1103	Misc Documentation Fixes
1140	AutoDispatch Cleanup
1239	PTU TCP/IP on Windows Failure
1168	C# Callback Problem
1159	Same message be returned through GetNextMsg more than 
		once when overlapping subscriptions are used with WebSphere
1158	Rebuild libxml and libz static library for HPUX
1222	Bolt reports that it is out of memory
1243	Issues Reported by .NET Development Team
1104	UnSubscribe mishandling multiple subscriptions to the same subject
1102	Bug in gmsec::util::Decoder::getString()  when count is 0
1183	gmsec::util::StdThread start/destructor timing
1775	Crash using RH5 x86 3.5 binaries when dynamic linking with libz
1909	Incorrect length of XML string being used in BaseConfigFile::ToXML
1914	Fraunhoffer Reported Defects
1982	PTU Mean Queue Period Calculation Incorrect
1962	JNI implementation of new Config(String[] args)
		incorrect when args.length == 0
1999	Original topic string stored for ActiveMQ subscriptions
2012	GMSEC Perl version tracking
2030	Defect in WebSphere MQ GetMWInfo implementation
1134	Status class toString implementation add Custom Code
1939	Address B1R Fortify Results
1193	ActiveMQ documentation
2056	Example Program Corrections (Batch 2)
1905	Example Program Organization
2052	Review and Repair of PTU Documents and Examples
2027	Problem in Subscribe -> Unsubscribe -> Subscribe
1224	Signature mismatch error reporting discrepancy between middleware
2046	C++ ConnectionFactory is using incorrect class code in Status.
2040	UnSubscribe(pattern, callback) does not validate the callback
2039	Perl Teardown with Generic JMS
2062	C++ / Java Bad Connection Error Code Discrepancy
2067	Replace ActiveMQ 3.7.1 with 3.8.1
2235	Bolt GetMWINFO() returns incorrect value
2275	Java UnSubscribe with Callback UnSubscribes all callback
2278	RegisterErrorCallback() implementation correction

 

==============================================================================
= GMSEC API 3.5.4 release notes (2013 November 20)
==============================================================================

o Patch 3.5.4 removes support for ActiveMQ's CPP wrapper version 3.7.1 (due to
  a memory leak in that product), and replaces it with version 3.8.1.
  
o This patch also corrects a problem in our Subscribe -> Unsubscribe -> 
  Subscribe sequence related to the de-registration of callbacks. The code 
  behavior now correctly conforms to the documentation which describes the
  function of the Unsubscribe calls.

--- Change summary -----------------------------------------------------------
ID	Description
2067	Replace ActiveMQ Client 3.7.1 with 3.8.1
2027	Problem in Subscribe -> Unsubscribe -> Subscribe sequence



==============================================================================
= GMSEC API 3.5.3 release notes (2013 September 25)
==============================================================================

o Patch 3.5.3 adds support for ActiveMQ's CMS wrapper version 3.7.1 in addition
to the existing support for version 3.4.0.

--- Change summary -----------------------------------------------------------
ID	Description
2013	ActiveMQ Client upgrade to version 3.7.1



==============================================================================
= GMSEC API 3.5.2 release notes (2013 September 5)
==============================================================================

o Patch 3.5.2 finishes addressing DR 1939. In this release, Category I and II
code issues identified by the Fortify static code analysis utility are
addressed.

--- Change summary -----------------------------------------------------------
ID	Description
1939	Address B1R Fortify Results
2030	Defect in WebSphere MQ GetMWInfo Implementation



==============================================================================
= GMSEC API 3.5.1.1 release notes (2013 August 9)
==============================================================================

o Patch 3.5.1.1 partially addresses DR 1939. In this release, Category I code
issues identified by the Fortify static code analysis utility are addressed.



==============================================================================
= GMSEC API 3.5.1 release notes (2013 May 17)
==============================================================================

o Patch 3.5.1 addresses high priority issues identified in the 3.5 binary
release.

The GMSEC API 3.5 dependencies on zlib and libxml2 have been removed.  In
addition, the GMSEC API's dependency on Xerces has also been removed.

--- Change summary -----------------------------------------------------------
ID	Description
1180	Remove Xerces dependency from GMSEC API
1183	gmsec::util::StdThread start/destructor timing
1775	Crash using RH5 x86 3.5 binaries when dynamically linking with libz



==============================================================================
= GMSEC API 3.5 release notes (2013 March 31)
==============================================================================

o Release 3.5 addresses several bugs and enhancements which are listed in
the change summary below.

o The support libraries needed to build from source are no longer included
within the GMSEC API source package. See GMSEC_API_OpenSource_Instructions.txt
for details.

o The GMSEC API now supports a .NET binding on Windows platforms.

o Request/response functionality has been enhanced to allow subscribing
to responses (by controlling the subject used to route responses) and to
allow multiple responses to a single request.  See the Users Guide for details
on the MW-REPLY-STRING, MW-EXPOSE-RESP, MW-MULTI-RESP parameters.


--- Change summary -----------------------------------------------------------
ID	Description
1961	Ensure consistency of return codes when GetNextMsg encounters timeout
1964	Generic JMS to WebLogic Adaptation & Testing
1969	Allow SimpleMessage field map to switch between tree and hash table
1973	Unsafe rm -rf in Makefile
1975	Java Config.GetValue always returns success
1978	Java binding improperly exposes internals in public package
1979	Allow control of which extensions are built with the API
1981	Possible invalid pointer use when processing reply (in
	BaseConnection::onReply)
1982	Address improper logging and log levels within API
1988	AutoDispatch not properly shutting down threads
1990	WebSphere wrapper does not propagate subscription authorization error
1992	Add Visual Studio 2010 Support for 64 Bit Windows
1993	Remove unnecessary compiler dependencies from user visible interfaces
1996	Using the Connection passed through the Java ReplyCallback.OnReply
	/OnError methods crashes the JVM
1998	Update documentation to indicate preference for revised C binding
2001	Fail gracefully if trying to build Perl binding with an inappropriate
	Perl (one without thread support)
2004	Doxygen config files need to have nomenclature fixed
2006	Clarify documentation regarding thread safety and concurrency
2007	BoundedQueue threading problem when multiple waiters
2008	Enforce the documented restriction that GetNextMsg is incompatible
	with AutoDispatch
2010	Update Logging API documentation and LOG* macro
2011	Field enhancements: add FromXML and template Set/Get methods
2012	C++ Connection Reply dereferences reply without checking for null
2016	Address missing items in C++ initialization lists
2018	Refactor BaseMessage and pull-up redundant code from derived classes
2021	Separate API support libraries from source and binary packages
2026	GMSEC List overhaul
2030	Introduce internal Status class to address binary compatibility of
	custom code and future enhancements
2034	Unable to disconnect from WebSphere after failing to subscribe to a
	topic requiring user name authorization
2035	SimpleMessage GetField and getField do not check for NULL field name
2051	The Default Log Handler (DefaultHandler) should allow for displaying
	long messages.
2053	GMSEC API Environment Validator
2054	Example program overhaul
2059	BaseMessage should not report error when processing unknown Message
	configuration options
2062	Use U16 instead of USHORT within API
2063	Intermittent crash when Disconnect or Destroy is called while
	auto-dispatch is running
2072	gmsec::util::StdThread start/destructor timing
2075	Open source builds depend on a populated SUPPORT directory parallel to
	GMSEC_API (see build instructions)
2076	Update middleware wrappers to use Status CustomCode
2079	Unable to reply to a request using a clone of request message
2087	Integrate C# binding
2092	Caller of Dispatcher::shutdown potentially holds shutdownLatch Mutex
	when the Dispatcher thread is destroyed
2094	Java JNICallback has the same problem as Java JNIReplyCallback (1996)
2095	Remove C2 policy dependency on NSS
2110	Reply destroyed prematurely in synchronous request/reply update
2113	Prepare self-extracting executables for Windows deliveries



==============================================================================
= GMSEC API 3.4 release notes (2012 May 31)
==============================================================================

o Release 3.4 addresses several bugs and enhancements which are listed in
the change summary below.

o Clients using the ActiveMQ middleware are prone to deadlock during failover.
This is a known issue (https://issues.apache.org/jura/browse/AMQCPP-376).

o Full support for Java-based components to use the WebLogic Fusion middleware
is waiting on a change from Oracle (SR 3-3682544871).

o Visual Studio 2010 (VS 2010) Express is not able to load projects which have
64 bit targets (this is not an issue with VS 2010 Professional).  To allow use
of VS 2010 Express, a utility is provided (GMSEC_API/config/VS2010e.java) which
can be used to strip references to 64 bit targets.


--- Change summary -----------------------------------------------------------
ID	Description
1891	PTU Multiple subscriptions per producer
1920	Generic JMS GMSEC API m/w wrapper
1784	NRL/Blossom Point customer reports warnings during build on Solaris
1881	Request tracking fields not populated under ActiveMQ
1883	Overlapping subscription inconsistency
1921	Deadlock in Dispatcher and BaseConnection
1922	Message.FromXML does not properly process empty BIN and STRING fields
1775	Fix errors identified by model based testing



==============================================================================
= GMSEC API 3.3 release notes (2011 Oct 31)
==============================================================================

o Release 3.3 addresses several bugs and enhancements which are listed in
the change summary below.

o Clients using the ActiveMQ middleware are prone to deadlock during failover.
This is a known issue (https://issues.apache.org/jura/browse/AMQCPP-376).

o Full support for Java-based components to use the WebLogic Fusion middleware
is waiting on a change from Oracle (SR 3-3682544871).

o Visual Studio 2010 (VS 2010) Express is not able to load projects which have
64 bit targets (this is not an issue with VS 2010 Professional).  To allow use
of VS 2010 Express, a utility is provided (GMSEC_API/config/VS2010e.java) which
can be used to strip references to 64 bit targets.

o There is a new version of the ActiveMQ middleware wrapper based on ActiveMQ
CPP 3.4.0.


--- Change summary -----------------------------------------------------------
ID	Description
1474	WebLogic Fusion middleware support (available for C, C++, Perl).
1563	Middleware should provide connection/server info in published messages.
1666	ActiveMQ (server) dropping messages (upgrade to ActiveMQ 5.5).
1767	Make thread-safe for auto-dispatch.
1769	Enhanced (C++) ConfigFile with methods for loading all elements
	(subscriptions, configs, messages).
1880	Corrected time zone of result of Message.GetTime in Java-binding.
1894	Address intermittent error with SmartSockets when Connect is called
	right after Disconnect.
1895	Prefer MW- prefix for middleware specific parameters.
1896	Support Windows 7 x86_64.
1898	Support RedHat Linux 6 x86_64.
1899	Fixed compilation on platforms where pthread_t is not a primitive type.
1900	Ported to Visual Studio 2010
1901	Reduced latency when using SmartSockets for message rates below 10/s.



==============================================================================
= GMSEC API 3.2.1 release notes (2011 Jun 17)
==============================================================================

o Patch release 3.2.1 addresses the bugs / problems listed below.

--- Change summary -----------------------------------------------------------
ID	Description
1781	Middleware subscription wildcard matching (the fix for SmartSockets
	and ActiveMQ was not included in 3.2).
1881	Request tracking fields not populated under ActiveMQ.
1885	Solaris 10 SPARC binaries (inadvertantly left out of 3.2).
1886	Modified shutdown of auto-dispatch thread.



==============================================================================
= GMSEC API 3.2 release notes (2011 Feb 28)
==============================================================================

o Release 3.2 addresses several bugs and enhancements which have been listed
below.

--- Change summary -----------------------------------------------------------
ID	Description
1540	Develop logging framework (Java)
1546	Use Conditions with Unit Test
1547	Remove all "sleep" from the code and use condition variable within the
	Core API
1552	Address static code analysis results
1553 	TCPSocketClientReconnector
1554	BoundedQueue
1555	Rework PTU spec definition
1556 	Spawners able to manage multiple clients (PTU)
1557	Custom message definitions (PTU)
1558	Warn of potential unintended return types and down casting
1559	Add support for hybrid clients in PTU
1560	"MBWire::Deserialize: data[0] != CMD_PUB" message
1562	WebSphere MQ failover fails
1564	GMSEC encoder/decoder doesn't exit gracefully
1565	Address C++ Exception in Perl XS
1566	Enhancement to Middleware documentation
1567	Provide (optional) coarse locks for Connection and Message Objects
1568	Correct Warnings in Perl XS/C++ code
1569	No indication if request with callback times out
1570	ToXML/FromXML lose precision on floating point types
1571 	Updated C-binding addressing name space, prototypes, const
1572	Provide Config file for standard GMSEC Message
1573	Clean up middleware cleanly (provide shut down function)
1574	External iterators for message fields
1575	WebSphere GUI problem with Windows
1576	Atomic field types/Concurrent structures
1581	Unhandled exception when publishing and network cable pulled
1582	ConnectionWriter main loop needs condition variable
1583	Connection::IsConnected() has erroneous connection state information
1584	Blank message being published by MBServer
1662	Middleware subject matching
1757	Address Fraunhoffer Institute issues from Summer 2010



==============================================================================
= GMSEC API 3.1.2 release notes (2010 Nov 19)
==============================================================================

o Release 3.1.2 adds support for ActiveMQ SSL connections and NSS
based encryption and digital signatures.



==============================================================================
= GMSEC API 3.1.1 release notes (2010 Sep 22)
==============================================================================

o Release 3.1.1 provides infrastructure for information assurance including
user authentication; subscription and publication authorization; data privacy
and integrity; subject validation.
This is implemented through policies so that environments can define their
own rules.  One policy provides backward compatibility.

--- Change summary -----------------------------------------------------------
ID	Description
1700	Secure API extension with information assurance controls



==============================================================================
= GMSEC API 3.1 release notes (2010 Mar 31)
==============================================================================

o Release 3.1 adds a logging framework which users can employ to receive API
messages and distribute their own messages.  See the Users Guide for more
information.

o This release adds support for the 64 bit Windows platform.

o Please note that, while WebSphereMQ now supports failover, IBM's current
implementation requires at least 90 seconds for failover to occur. WebSphereMQ
has only included failover since the latest release of the product, and thus
it should not be considered as reliable as alternative middleware products
offering failover.


--- Change summary -----------------------------------------------------------
ID	Description
1508	Performance Test Utility Crashes
1509	PTU sequence interruption
1510	Blank output from performance test utility
1544	White space in Config server value generates exception
1583	Replace strcmp with stringEquals
1597	MB wrapper TCPSocket::getLastSocketError() assumes errno reflects
	problem
1605	Add Doxygen generation process to API Maintenance & Build Document
1612	Erroneous subject with Request/Reply
1613	Use C++ implementation of Request/Reply for Java
1615	Investigation of create message with null subject for all Middleware
1640	API coding requirement and style standards
1641	Verify Unit Test tests API requirement
1642	Resolve compiler warnings: C4996, C4244, and C4018
1643	Clean up doxygen (reflect any recent changes)
1655	Nightly test 2.13 failures
1667	Update Request/Reply API Example code
1673	Java Request/Reply with Callback not Working
1674	Perl Request/Reply with Callback not Working
1678	Request transmission not guaranteed
1692	Add logging to API
1693	Develop Nightly/Unit Tests for testing Req/Reply subject equality
1697	POSIX implementation of mutex non-reentrant
1699	JVM crashes when running gmreq_cb
1701	Address bashrc redundancy
1702	Build without Cygwin 
1703	PTU provide a graph of the results at the end of each run
1706	Develop logging framework (C & C++)
1707	PTU provide support for point to point connection via TCP socket
1710	Upgrade ActiveMQ CMS from v.2.2.2 to v.3.1
1711	Support 64-bit Windows
1715	Refactoring WebSphere wrapper with MQI
1716	StartAutoDispatch should not return until the dispatch thread is
	running or it fails to start. 
1724	Add WebSphere failover support



==============================================================================
= GMSEC API 3.0.1.1 release notes (2010 Jan 22)
==============================================================================

o The GMSEC API 3.0.1 patch release dated 12/14/2009 did not include the VC6
libraries normally located under GMSEC_API/objects which are needed by
developers linking their own applications.
This distribution includes those files.



==============================================================================
= GMSEC API 3.0.1 release notes (2010 Dec 14)
==============================================================================

o Patch release 3.0.1 includes support for the Microsoft Visual C++ 6.0
compiler on Windows platforms.  In order to use VC6, developers must define
the macro (preprocessing symbol) GMSEC_VC6.


--- Change summary -----------------------------------------------------------
ID	Description
1625	Java application using SmartSockets connection initialization problem
1666	TRMM / Raytheon needs VC6 support on Windows 2000



==============================================================================
= GMSEC API 3.0 release notes (2009 Sep 30)
==============================================================================

o Middleware supported by this version of the API:
	- Message Bus 3.0
	- TIBCO Smartsockets versions 6.6, 6.7, 6.8, 6.8.1, and 6.8.2
	- ActiveMQ 5.2
	- WebSphere 7.0.0.2 with xms fix to allow messages greater than 4MB

o GMSEC API release 3.0 breaks binary compatibility.
C++ components will have to be recompiled to use this versions 3.0 or later of the API.
The primary reason we broke binary compatibility at this time was to reduce
the impact of future extensions to the interface.

We strove mightily to maintain source compatibility, but in some instances,
decided that the long-term benefit of a change outweighed its short-term cost.

	- C/C++ GMSEC_BOOL is now an enumerated type instead of a typedef to
	unsigned char.  This means, for example, that the following statement
	which used to be valid
		GMSEC_BOOL value = 0;
	must be updated to
		GMSEC_BOOL value = GMSEC_FALSE;

	- Some functions and types had leaked out of the API internals and
	become visible to clients.  Examples include the C++ Connection and
	Message destructors and methods for tracking flags.  This has been
	addressed so that users must access this functionality through the
	official interface, e.g., by using ConnectionFactory::DestroyConnection,
	Connection::DestroyMessage, and Config objects to set tracking flags.

	- C++ namespace reorganization
	The C++ namespace gmsec now only holds the classes which are part of
	the official API (e.g., Config, ConfigFile, Connection, Field,
	Message, Status, and *Callback*).
	Support / implementation items have be moved to the gmsec::util
	or gmsec::internal namespaces.  The gmsec::util namespace, like the
	gmsec namespace, is available for compiling against, but gmsec::internal
	is not.  Of course, the open source distribution contains all source
	code.  Our goal is to maintain source but not binary compatibility
	between major releases for gmsec::util.

	The GMSEC C++ Thread and Mutex classes are now in the gmsec::util
	namespace in addition to functions which may be of general use.

o Release 3.0 uses a single directory for binaries independent of platform
(GMSEC_API/bin).
A user who needs to match a single pre-3.0 platform name on a POSIX system
could use a symbolic link.  For multiple platforms, it would be necessary to
move the contents of bin to appopriately named sub-directories.

o See the Users Guide for information on the new Connection support for
republishing requests.

o The GMSEC API documentation for each language is available under
GMSEC_API/docs.

o Java components should activate signal chaining to avoid potential
conflicting signal handling in native code.  See
	http://java.sun.com/javase/6/webnotes/trouble/TSG-VM/html/signals.html
for details.

--- Change summary -----------------------------------------------------------
ID	Description
1008	Provide MW-INFO for prime and backup buses
1080	Cable Pull Failover for SS Halts on HP
1086	Java example gmreq_cb doesnâ€™t work due to Request w/Reply issues
1102	Update Callback examples to use separate connections for multiple
	threads
1121	Release building of API (Java & Perl) on Windows API 1121
1173	Omit unsupported configured Items from CM
1208	Inquiries about threads and C
1290	MBServer to publish resource data
1330	Client crashes connecting to non-existent Message Bus server
1348	Enhance MBServer to include usage arguments
1394	New Middleware Support
1408	Removal of Heartbeat code from MBServer
1424	Removal of API test directory
1435	GMSEC Components crashing under MBServer 2.6
1440	MB client will not start if there are spaces between server
	primary & backup ip addresses
1442	Intermittent crashes running 11.1 and 11.2 unit tests
1449	Verify values of fields
1450	Common Encoding/Decoding
1455	Nightly Test 11.2 failures
1473	Support connectiontype=hostname:port,hostname:port
1476	Remove all legacy .dsp files from API
1477	Support for TIBCO SmartSockets 6.8.2
1485	Fix the batch files that start GMSEC API java examples



==============================================================================
= GMSEC API 2.6 release notes (2009 Mar 25)
==============================================================================

--- Change summary -----------------------------------------------------------
ID	Description
780	GMSEC API or SmartSockets Aborting
813	Unique SubjectID in Use
1061	API or SmartSockets Randomly Aborting after call to MessageLookup()
1064	CPU Usage Increases when running CAT with the Message Bus
1076	Perl Regression Test Case 2.14 Failure
1077	AutoDispatch Not Functioning Correctly
1095	Modify Darwin and DarwinIntel Makefiles and references to Mac and
	MacIntel
1096	Add a multi-threaded example
1103	Update API Users Guide elaborating how to use separate connections for
	multiple threads
1110	Eclipse Disconnecting and Connecting on SmartSockets with API 2.3
1111	SmartSockets 6.8 DLL Error when running API 2.3
1204	TRMM Front End (TFE) stops publishing via API 2.3 after a few minutes
1209	Debugger (DBX) Inquires
1223	Support Perl 5.8.8 for HP
1232	API Memory Leak in SmartSockets
1233	API Memory Leak in Message Bus
1287	HPUX B. 32 64 bit Itanium
1296	GMSEC API File Permissions are Wrong in API 2.4
1308	User's Guide states some SS parameters are deprecated
1336	Java Thread Handle/Memory Leak
1337	Perl Memory Leak in GetField()
1338	Perl Memory Leak in GetNextField()
1341	Perl Memory Leak in GetValue()
1347	API Users Guide â€“ Appendix A â€“ Needs Clarification
1354	Performance Testing
1367	Disfunctional gmsub_cfg.java example code
1373	API Fails to Build on XP if Installed under directories with spaces.
1390	Perl for API 2.5 Not Working on Windows XP
1407	TRMM 380 GMSEC Delivery CD does not work with TFE
1439	Java / JNI code not robust



==============================================================================
= GMSEC API 2.5 release notes (2008 Sep 24)
==============================================================================

o All third party libraries have been removed from the GMSEC API.
User installation and configuration is conducted in a new process.  Specific
details can be found in the most updated GMSEC API Userâ€™s Guide which is part
of this delivery and can be downloaded on the GSFC Virtual Systems Design
Environment (VSDE) at: http://vsde.gsfc.nasa.gov.


--- Change summary -----------------------------------------------------------
ID	Description
1079	MB Servers "Segmentation Fault"
1097	Unit Test 9.11 (Large Message) fails for Message Bus
1129	Message Bus dumping core when compiled with debug option
1266	Remove all third party software from the GMSEC API
1267	TIBCO DLL issue on HP-UX and Solaris GCC
1289	Drop GMSEC API support for Solaris 8 CC



==============================================================================
= GMSEC API 2.4 release notes (2008 May 28)
==============================================================================

o Middleware supported by this version of the API are the Message Bus and
TIBCO Smartsockets versions 6.6, 6.7, 6.8 and 6.8.1.

o The GMSEC User's Guide has been updated to reflect the changes to supported
platforms and configuration options.


--- Change summary -----------------------------------------------------------
ID	Description

o mission requested enhancements
1113	Support for Linux RH 5

o internally requested enhancements
1105	Setup an auto generated spreadsheet for nightly test result reporting
1122	Capability to run all night tests concurrently

o mission reported discrepancy reports
814	Resolved GMD failure with publish/subscribe example code
870	Resolved API randomly disconnecting and reconnecting
1111	Resolved SS 6.8 dll error when running API v2.3
1168	Resolved GMSEC Perl API memory leak

o internally reported discrepancy reports
860	Resolved regression testing not working
898	Resolved Solaris 10 API Build Script not running to completion
899	Resolved Mac OSX (darwin) API Build Script not running to completion
1047	Resolved Windows API Build Script not running to completion
1049	Resolved Solaris 8 API Build Script not running to completion
1050	Resolved Mac OSX (darwin-intel) API Build Script not running to
	completion
1051	Resolved Linux (Fedora) API Build Script not running to completion
1052	Resolved HP-UX API Build Script not running to completion
1082	Resolved corrupt SS68 dll files for Windows in CVS
1084	Remove dlcompat-20030629.tar from Vendor Directory
1106	Completed API test cases failure collection and revision
1107	Completed analysis and categorization of failed test cases 
1109	Updated API test plan with test case and procedure changes
1118	Resolved Linux (RH 3) API Build Script not running to completion
1119	Resolved Linux (RH 4) API Build Script not running to completion
1121	Resolved release building of API (Java & Perl) on Windows
1142	Resolved API not running under Java 1.4
1154	Resolved gmconfig example failing to run
1169	Resolved corrupt TIBCO vendor library in CVS



==============================================================================
= GMSEC API 2.3 release notes (2007 Oct 3)
==============================================================================

o When creating multi-threaded connections, use a separate GMSEC objects
(e.g., connections) for each thread.
Do not use the same GMSEC object (e.g., connection) in multiple threads.

o The Release 2.3 version of the Message Bus (MB) is not backwards compatible
with past releases of the GMSEC API.
  
o The release 2.3 version of the MB will only properly function with GMSEC
API 2.3.

o TIBCO Smartsockets 6.5 is not supported by the API; versions supported
are 6.6, 6.7, and 6.8.

o AutoDispatch Callbacks are currently not working consistently.  We will be
looking to resolve this issue in the upcoming release.

o The GMSEC User's Guide has been updated to reflect the changes to supported
platforms and configuration options.


--- Change summary -----------------------------------------------------------
ID	Description
813	Resolved Unique subject ID error message from SmartSockets failover.
865	Resolved Linux.Cpp.Regression testing not working.
868	Resolved Linux.C.Regression testing not working.
975	Resolved Increased failover delay time.
990	Resolved CPU usage increase with SmartSockets GetNextMsg call.
1003	Resolved Memory leak in API (C++).
1009	Resolved CAT connection issue with API 2.2.
1010	Resolved API 2.2 RHE tar file permission.
1044	Resolved No error message when calling Connect if no connection
	(Perl API).
1063	Resolved API example code with DestroyField call.
882	Message Bus Failover/Fault-Tolerance capability.
883	Message Bus Performance Analysis.
964	Update SmartSockets with 6.7 Patch.
1005	Java Message Bus capability for Mac OSX.
1037	Update SmartSockets with 6.8 Patch.



==============================================================================
= GMSEC API 2.2.1 release notes (2007 Jul 20)
==============================================================================

o The GMSEC User's Guide has been updated to reflect the changes to supported
platforms and configuration options.  

o We have added support for HPUX 11.11 and 11.23.

o We have added a Perl GMSEC API for Solaris 10.

o We have added support for SmartSockets 6.8.

o We have stopped supporting Linux RedHat L 7.1.


--- Change summary -----------------------------------------------------------
ID	Description
990	Fixed SmartSockets GetNextMsg call causing CPU usage to go to 100%.
991	Resolved SmartSockets not receiving 5MB messages on HP-UX 11.23.



==============================================================================
= GMSEC API 2.2 release notes (2007 Jun 27)
==============================================================================

o The GMSEC User's Guide has been updated to reflect the changes to supported
platforms and configuration options.  

o We have added debugging capability for the SmartSockets and Message Bus.

o We have added message compression capability for the Message Bus C++ and
Java.

o We have added support for Linux Fedora Core 5.

o We have added a Perl GMSEC API for the Mac OS X platform.

o We have added support for Macintosh OS X on Intel, including the C/C++ GMSEC
Message Bus, and the Java native versions of Smartsockets and the GMSEC Message
Bus.


--- Change summary -----------------------------------------------------------
ID	Description

[enhancements]
820	Added new capability to support HP-UX 11.11 and 11.23.
821	Added new capability to support Perl on Solaris 10.
822	Added manual tests procedures for Failover and GMD in the GMSEC
	API Test Plan.
880	Added new capability to support SmartSockets 6.8.

[fixes]
644	Fixed the Message Bus from stopping to run after 40 hours.
645	Fixed the GMSEC_Char from returning a byte.
688	Fixed Null return vs. "0" for "PARAMETER NAME" tag.
801	Fixed SmartSockets Memory Leak.
813	Fixed Unique Subject ID in use.
817	Fixed Message Bus running out of connections.
818	Fixed Timeout on Message Bus.
839	Resolved to correctly set field type in c program.
859	Resolved crash when using DestroyField() in the GMSEC C API with
	SmartSockets.
867	Fixed AutoDispatcher Test (Unit Test # 7_15) not working on Linux
	[DestroyConnection].
877	Resolved questions on failover arguments.
878	Resolved questions on solaris compiler.
976	Fixed GMSEC/ANSR crash with core dump.



==============================================================================
= GMSEC API 2.1 release notes (2007 Mar 19)
==============================================================================

--- Change summary -----------------------------------------------------------
ID	Description

[enhancements]
782	Added a Perl version of the GMSEC API for the Mac OS X platform.
783	Changed the GMSEC API default settings to not automatically start the
	Message Bus by default, instead if connection type is not found, an error
	message is displayed.
784	Added a new debugging capability for the SmartSockets and Message Bus.
786	Added the check for Darwin (Mac OS X) to all Perl test case code and
	appropriate libraries loaded.
791	Added compression capabilities for the C++ and Java Message Bus.
794	Added SmartSockets configuration items.
796	Added new capability to support Linux Fedora Core 5.
797	Added new capability to support Mac OS X on Intel.
800	Added new test in the API Test Suite that verified that a GMSEC message
	is correct when it is sent and received.

[fixed discrepancies]
785	Removed the â€œgot hereâ€� debug line left in the Java connection code.
787	Fixed the test case timeout script not exiting upon process completion.
788	Fixed the java_gmesec_ss and java_gmsec_mb connections for RedHat Linux.
789	Fixed the GMSEC API aborting on lookup call.
790	Removed â€œ#â€� and â€œ.â€� from GMSEC messages.
792	Fixed the failover and GMD callback issues with the Java native
	SmartSockets.
798	Fixed the Message Bus network disconnection.
812	Fixed the SmartSockets failover delay.



==============================================================================
= GMSEC API 2.0 release notes (2006 Sep 1)
==============================================================================

o The GMSEC User's Guide has been updated to reflect the changes to supported
platforms and configuration options.  

o The GMSEC API has now been tested on Java 1.5, and it is fully supported on
all platforms.

o There is now a new middleware option for Linux called ICE, which is based on
an open source middleware.  We plan to support the other platforms shortly.

o We have added support for GMSEC Message Bus and Smartsockets on Solaris 10,
using the Sunpro C/C++ (5.8) compiler (CC).

o We have added support for Macintosh OSX, including the C/C++ GMSEC Message
Bus, and the Java native versions of Smartsockets and the GMSEC Message Bus.


--- Change summary -----------------------------------------------------------
ID	Description

115215	Added a pure Java version of SmartSockets for all supported versions
	(6.5.1, 6.6, 6.7).

115217	Added an automatic field called USER-NAME, which is the name
	of the user id that ran the process which created the message.

115245	Added a pure Java version of the GMSEC Message Bus.

106915	Added new SmartSockets config value, SERVER_READ_TIMEOUT, so it is
	easier to configure the value.

115448	Changed the stopAutoDispath function to have the thread wait until
	it is completely stopped before returning on a disconnect.

115484	Added IsConnected function to the C and Perl implemenations.

115485	Fixed the IsConnected function in Java to return the proper value.

115497	A bug was fixed which caused a threadlock if a connection was
	disconnected, before calling StopAutoDispatch.

115778	A bug was fixed that caused Request/Reply to fail under certain
	circumstances.

139086	A bug in the GMSEC Message bus that would not allow a Request call
	with a timeout to return even if the time had passed.

152082	A memory leak in the Request/Reply with dipatcher function was fixed.

152083	A bug in the Java version of the Clone message funtion was fixed.

152220	A bug in the Request with Callback (using WAIT_FOREVER) was fixed,
	so that the callbacks would be cleaned up when disconnecting.

152857	A bug in SmartSockets was fixed where a message with to many fields
	could crash the server.  This turned out to be a bug in the SS api,
	6.5.1-6.7, so we made the GMSEC_API will now return with an error if
	you try to put more fields in (about 5000 fields, but it depends on
	the length of the field names).

159903	A bug that would make the Java version throw an exception if an array
	of strings was sent to the Config object, with 1 or more elements empty.



==============================================================================
= GMSEC API 1.3.2 release notes (2006 Mar 28)
==============================================================================



==============================================================================
= GMSEC API 1.3.1 release notes (2006 Feb 6)
==============================================================================

--- Change summary -----------------------------------------------------------
- A Bug in Java that made toString() on a binary field was very slow to return
the value.

- The GMD callback functionality in Java was implemented.

- A bug in the SS implementation which would return an error when trying to
destroy a connection.



==============================================================================
= GMSEC API 1.3 release notes (2005 Dec 7)
==============================================================================

o Support was added for SmartSockets 6.6.  If you are using 6.6 change the
connectiontype to 'gmsec_ss66' instead of 'gmsec_ss' (which now references
SS 6.5.1 only).

o There have been a number of connection config and message config item
additions, please review the GMSEC API User's Guide (section 2.3.1) for the
details.

--- Change summary -----------------------------------------------------------

- Added support for SmartSockets 6.6

- Added support for Red Hat Enterprise 4 linux

- Added new GMD examples for C, C++, and Java

- Added FromXML() and ToXML() support in ConfigFile.

- Added support for ReplyCallback, which is a variation of the Callback class
that also handles request timeout errors. 

- Added config items to toggle the individual tracking fields on and off.

- Added MW-INFO automatic field support.



==============================================================================
= GMSEC API 1.2.1.a release notes
==============================================================================



==============================================================================
= GMSEC API 1.2.1 release notes (2005 Aug 9)
==============================================================================

o This release is mainly to fix some memory leaks in the API.

--- Change summary -----------------------------------------------------------
- Fixed memory leak in Message::toXML().

- Fixed memory leak in the SmartSockets implementation.  This was a problem in
the SS api, if a message was re-used it would leak some of the information.
The GMSEC API now cleans up the leak after each Publish/Request/Reply.

- Fixed memory leaks in the Perl API.

- GMSEC Message Bus has better error reporting (codes and messages).



==============================================================================
= GMSEC API 1.2 release notes (2005 Apr 12)
==============================================================================

o GMSEC Message Bus middleware implementation was added which is a development
middleware.  The needed files are included in the release.  Setup and
configuration information is in the GMSEC API User's Manual Appendix A. 

o ConfigFile() Class added to support standard XML configuration file support.
Please refer to the API documentation for examples and GMSEC API User's Manual
section 5.1.1.5 for more information.

--- Change summary -----------------------------------------------------------

- SmartSockets will handle failover without returning a [4,10] error.  Will
continue to try to reconnect on its own.  This functionality can be turned off
with config items.  Support for most commonly used SmartSockets config items
added to API.  New config items defined in GMSEC API User's Manual section 2.3.

- A Bug in the AutoDispatcher that returned a incorrect Error was fixed.

- ErrorCallback support added to Java API

- Perl Support for Windows, Linux (7.x-Ent), Solaris

- Red Hat Enterprise support added.

- ClearField(), ClearFields() methods added to Message for all languages

- XMLMessage Class added for C++ and Java.

- ICS middleware implementation no longer part of this release

- API documentation improvement.

- SmartSockets GMD Examples examples added for C++ on all platforms.



==============================================================================
= GMSEC API 1.1 release notes (2004 Sep 30)
==============================================================================

o This release requires code changes to any application that uses the C api.
A status handle must now be created by the calling application and passed to
each C api call.  For specific calls that are needed, please refer to the C
example programs.  This change was made to make the C api thread safe.

--- Change summary -----------------------------------------------------------

- A new helper method was added to the Status class called isError(), which
can be called as a simple way to check for returned errors.

- A new method called FromXML() which will allow a message to be initialized
by sending an XML formatted string to the Message.  The format is the same
format that ToXML() uses for output and there is no schema enforcement at
this time.

- A bug that did not allow the Status to reset properly was fixed, so it is
safe to reuse a Status class.

- A bug that returned a bad Status when calling Message:SetConfig() was fixed.

- A bug in the C api (mulitple functions) allowed bad pointers to be sent back
if the call failed.  NULL pointers will now be returned.

