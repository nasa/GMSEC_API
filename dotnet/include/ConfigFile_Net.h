
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#ifndef ConfigFile_Net
#define ConfigFile_Net

// managed
#include "Config_Net.h"
#include "Connection_Net.h"
#include "Message_Net.h"
#include "Status_Net.h"
#include "GMSEC_Net.h"
#include "GMSECErrors_Net.h"

// native
#include <exception>
#include "gmsec/ConfigFile.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

/// <summary>
/// This class is for the managment of standard configuration files.
/// </summary>
/// <example>
/// <code lang="xml" xml:space="preserve">
/// <DEFINITIONS>
///	<SUBSCRIPTION NAME="events" PATTERN="GMSEC.MISSION.CONST.SAT.EVT.MSG.&gt;"/>
///	<SUBSCRIPTION NAME="custom1" PATTERN="CUSTOM.MESSAGE.SUBJECTS.*"/>
/// 	<CONFIG NAME="config1">
/// 		<PARAMETER NAME="connectiontype">gmsec_icsswb</PARAMETER>
/// 		<PARAMETER NAME="hostname">localhost</PARAMETER>
/// 		<PARAMETER NAME="port">10005</PARAMETER>
/// 	</CONFIG>
/// 	<CONFIG NAME="config2">
/// 		<PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
/// 		<PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
/// 		<PARAMETER NAME="tracking">true</PARAMETER>
/// 		<PARAMETER NAME="isthreaded">true</PARAMETER>
/// 		<PARAMETER NAME="compress">true</PARAMETER>
/// 	</CONFIG>
///	<MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
/// 		<CONFIG NAME="msg_config">
/// 			<PARAMETER NAME="KIND">GMSEC_MSG_PUBLISH</PARAMETER>
/// 		</CONFIG>
/// 		<FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
/// 		<FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
/// 		<FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
/// 		<FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
/// 		<FIELD TYPE="LONG" NAME="long_field">123</FIELD>
/// 		<FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
/// 		<FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
/// 		<FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
/// 		<FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
/// 		<FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
/// 	</MESSAGE>
/// </DEFINITIONS>
/// </code>
/// </example>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class ConfigFile
{
private:
	gmsec::ConfigFile * mImpl;

	/// <summary>Generate an empty configfile in case of error.  </summary>
	inline void GenerateDefaultConfigFile();

	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

internal:
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::ConfigFile * GetUnmanImpl();

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ConfigFile();

public:
	/// <summary>This constructor creates the association of this object with a configuration file.</summary>
	/// <param name="filepath">full or relative path to configuration file</param>
	ConfigFile(const String ^ filepath);

	/// <summary>Destructor</summary>
	~ConfigFile();

	/// <summary>This function opens the configuration file and parses the XML
	/// It will return errors explaining what, if anything, went wrong.</summary>
	/// <returns>status of whether the load and parse was successful</returns>
	Nasa::Gmsec::Net::Status ^ Load();

	
	/// <summary>This function parses the XML in the passed in string.
	/// It will return errors explaining what, if anything, went wrong.</summary>
	/// <param name="xml">xml string</param>
	/// <returns>status of whether the parse and parse was successful</returns>
	Nasa::Gmsec::Net::Status ^ FromXML(const String ^ xml);

	/// <summary>This function produces an XML config file containing all
	/// known config file tags.</summary>
	/// <param name="xml">Out Paramater:xml string</param>
	/// <returns>status of whether the parse and parse was successful</returns>
	Nasa::Gmsec::Net::Status ^ ToXML( 
		[Runtime::InteropServices::Out] const String ^ %xml);

	/// <summary>This function will return true is the config file was successfully loaded</summary>
	/// <returns>true if file loaded</returns>
	bool IsLoaded();

	/// <summary>This function will attempt to find a named connection in the
	/// config file and set the passed in config object with the loaded values.
	/// Example config file load and get config in code section:</summary>
	/// <example>
	/// <code lang="C++">
	/// // create configfile object and attach to file C++ CLI
	/// ConfigFile^ cf = gcnew ConfigFile("path/to/file.xml");
	/// // load and parse
	/// Status^ result = cf->Load();
	/// if( result->IsError() ) { /* handle error */ }
	/// // get 2 configs from file
	/// Config^ c1;
	/// Config^ c2;
	/// cf->LookupConfig("config1", c1);
	/// cf->LookupConfig("config2", c2);
	/// </code>
	/// <code lang="C#">
	/// // create configfile object and attach to file C#
	///  ConfigFile cf = new ConfigFile("path/to/file.xml");
	///  // load and parse
	///  Status result = cf.Load();
	/// if (result.IsError()) { /* handle error */ }
	///  // get 2 configs from file
	///  Config c1;
	///  Config c2;
	///  cf.LookupConfig("config1", out c1);
	///  cf.LookupConfig("config2", out c2);
	/// </code>
	/// <code lang="VB.NET">
	/// ' create configfile object and attach to file VB
	/// Dim cf As New ConfigFile("path/to/file.xml")
	/// ' load and parse
	/// Dim result As Status
	/// result = cf.Load()
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// ' get 2 configs from file
	/// Dim c1 As New Config()
	/// Dim c2 As New Config()
	/// c1 = Nothing
	/// c2 = Nothing
	/// cf.LookupConfig("config1", c1)
	/// cf.LookupConfig("config2", c2)
	/// </code>
	/// <code lang="f#">
	/// // create configfile object and attach to file F#
	/// let cf = new ConfigFile("path/to/file.xml")
	/// // load and parse
	/// let result = cf.Load() 
	/// if( result.IsError() ) then () // Handle Error
	/// // get 2 configs from file
	/// let (result2, c1) = cf.LookupConfig("config1")
	/// if( result2.IsError() ) then ()  // Handle Error
	/// let (result3, c2) = cf.LookupConfig("config2")
	/// if( result3.IsError() ) then () // Handle Error
	/// </code>
	/// </example>
	/// <param name="name">value in the name="" attribute of the &lt;CONFIG/&gt; block to seed the gmsec::Config object</param>
	/// <param name="config">Config object to set</param>
	/// <returns>status of whether named CONFIG block was found</returns>
	Nasa::Gmsec::Net::Status ^ LookupConfig(const String ^ name, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Config ^% config);
	
	/// <summary>This function will attempt to find a named message in the
	/// config file and create a new message from that xml the loaded fields and values.
	/// Example config file load and get message in code section:</summary>
	/// <example>
	/// <code lang="C++">
	/// // create configfile object and attach to file C++ CLI
	/// ConfigFile^ cf = gcnew ConfigFile ("path/to/file.xml");
	/// // load and parse
	/// Status^ result = cf->Load();
	/// if( result->IsError() ) { /* handle error */ }
	/// // create message from config file entry
	/// Message^ msg;
	/// result = conn->CreateMessage(msg);
	/// if( result->IsError() ) { /* handle error */ }
	/// // load message subject, type, and fields from config file
	/// result = cf->LookupMessage("msg1", msg);
	/// if( result->IsError() ) { /* handle error */ }
	/// </code>
	/// <code lang="C#">
	/// // create configfile object and attach to file C#
	/// ConfigFile cf = new ConfigFile("path/to/file.xml");
	/// // load and parse
	/// Status result = cf.Load();
	/// if (result.IsError())
	/// {
	///     //handle error
	/// }
	/// // create message from config file entry
	/// Message msg;
	/// result = conn.CreateMessage(out msg);
	/// if (result.IsError())
	/// {
	///     //handle error
	/// }
	/// // load message subject, type, and fields from config file
	/// result = cf.LookupMessage("msg1", msg);
	/// if (result.IsError())
	/// {
	///     //handle error
	/// }
	/// </code>
	/// <code lang="VB.NET">
	/// ' create configfile object and attach to file VB
	/// Dim cf As New ConfigFile("path/to/file.xml")
	/// Dim result As Status
	/// ' load and parse
	/// result = cf.Load()
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// ' create message from config file entry
	/// Dim msg As Message
	/// msg = Nothing
	/// result = conn.CreateMessage(msg)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// ' load message subject, type, and fields from config file
	/// result = cf.LookupMessage("msg1", msg)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// </code>
	/// <code lang="f#">
	/// // create configfile object and attach to file F#
	/// let cf = new ConfigFile("path/to/file.xml");
	/// // load and parse
	/// let result = cf.Load()
	/// if( result.IsError() ) then () // Handle Error
	/// // create message from config file entry
	/// let (result2, msg) = conn.CreateMessage()
	/// if( result2.IsError() ) then () // Handle Error
	/// // load message subject, type, and fields from config file
	/// let result3 = cf.LookupMessage("msg1", msg)
	/// if( result3.IsError() ) then () // Handle Error
	/// </code>
	/// </example>
	/// <param name="name">value in the name="" attribute of the &lt;MESSAGE/&gt; block to seed the gmsec::Message object</param>
	/// <param name="msg">Message object to set</param>
	/// <returns>status of whether named MESSAGE block was found</returns>
	Nasa::Gmsec::Net::Status ^ LookupMessage(const String ^ name, 
		Nasa::Gmsec::Net::Message ^ msg);

	/// <summary>This function will look up a subscription pattern defined in
	/// the config file. This is usefull to allow easy modification of subject
	/// names without code changes.</summary>
	/// <example>
	/// <code lang="C++">
	/// C++ CLI
	/// String^ pattern;
	/// // lookup subject pattern by name
	/// Status^ result = cf->LookupSubscription( "events", pattern );
	/// if( result->IsError() ) { /* handle error */ }
	/// Console::WriteLine("Subscribe to: " + pattern);
	/// // subscribe on the connection
	/// result = conn->Subscribe(pattern);
	/// if( result->IsError() ) { /* handle error */ }
	/// </code>
	/// <code lang="C#">
	/// C#
	/// String pattern;
	/// // lookup subject pattern by name
	/// Status result = cf.LookupSubscription( "events", out pattern );
	/// if (result.IsError()) { /* handle error */ }
	/// Console.WriteLine("Subscribe to: " + pattern);
	/// // subscribe on the connection
	/// result = conn.Subscribe(pattern);
	/// if (result.IsError()) { /* handle error */ }
	/// </code>
	/// <code lang="VB.NET">
	/// 'VB
	/// Dim pattern As String
	/// pattern = Nothing
	/// ' lookup subject pattern by name
	/// Dim result As Status
	/// result = cf.LookupSubscription("events", pattern)
	/// If (result.IsError()) Then
	///     ' handle error
	/// End If
	/// Console.WriteLine("Subscribe to: " + pattern)
	/// ' subscribe on the connection
	/// result = conn.Subscribe(pattern)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// </code>
	/// <code lang="f#">
	/// // F#
	/// // lookup subject pattern by name
	/// let (result, pattern) = cf.LookupSubscription( "events");
	/// if( result.IsError() ) then () // Handle Error
	/// Console.WriteLine("Subscribe to: " + pattern);
	/// // subscribe on the connection
	/// let result2 = conn.Subscribe(pattern);
	/// if( result2.IsError() ) then () // Handle Error
	/// </code>
	/// </example>
	/// <param name="name">value in the name="" attribute of the &lt;SUBSCRIPTION/&gt; block containing the subscription pattern</param>
	/// <param name="pattern">out parameter, the pattern defined in the config file</param>
	/// <returns>status of whether named SUBSCRIPTION block was found</returns>
	Nasa::Gmsec::Net::Status ^ LookupSubscription( 
		const String ^ name, [Runtime::InteropServices::Out] 
	    const String ^ %pattern);

	/// <summary>Get a list of all subscriptions by name</summary>
	/// <param name="names">name list</param>
    /// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetSubscriptionNames([Runtime::InteropServices::Out] List<String^>^% names);

	/// <summary>Gets all Subscriptions</summary>
	/// <param name="patterns">name list</param>
    /// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetSubscriptions([Runtime::InteropServices::Out] List<String^>^% patterns);

	/// <summary>Get all Config names</summary>
	/// <param name="names">config name list</param>
    /// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetConfigNames([Runtime::InteropServices::Out] List<String^>^% names);

	/// <summary>Get all Configs</summary>
	/// <param name="configList">config list</param>
    /// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetConfigs([Runtime::InteropServices::Out] List<Config^>^% configList);

	/// <summary>Get all Config names</summary>
	/// <param name="names">Name of configs</param>
    /// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetMessageNames([Runtime::InteropServices::Out] List<String^>^% names);

	/// <summary>Get all Message Definitions</summary>
	/// <param name="connection">connection</param>
	/// <param name="message">message list</param>
    /// <returns>status - result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetMessages(Nasa::Gmsec::Net::Connection^ connection, [Runtime::InteropServices::Out] List<Nasa::Gmsec::Net::Message^>^% message);	
};

}
}
}
#endif  // ConfigFile_Net

