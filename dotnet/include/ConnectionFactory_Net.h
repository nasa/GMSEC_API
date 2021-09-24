
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
#ifndef ConnectionFactory_Net
#define ConnectionFactory_Net

// managed
#include "Config_Net.h"
#include "Connection_Net.h"
#include "Status_Net.h"
#include "GMSEC_Net.h"
#include "GMSECDefs_Net.h"
#include "GMSECErrors_Net.h"

// native
#include "gmsec/ConnectionFactory.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{


/// <summary>
/// This class contains static methods for the creation/destruction of connection objects
/// This class is NOT an abstract base class, it will have complete implementation that
/// would call the appropriate Connection object constructor for the requested connection.
/// The create function returns a Connection object associated with the determined connection
/// type but does not actually establish the connection. The Connection object method <see cref="Connection::Connect()"/>
/// is required to establish the software bus connection.
/// </summary>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class ConnectionFactory
{
public:
	/// <summary>This function returns a string containing the description of the current API version</summary>
	/// <returns>the current GMSEC API version string</returns>
	static const String ^ GetAPIVersion();

	/// <summary>This function is used to create a connection, although the connection type
	/// is inside the Config as an item named "connectionType" and
	/// whose value is the root library name for a middleware.
	/// For example, the icsswb can be designated by the following (code section):</summary>
	/// <example>
	/// <code lang="C++">
	// create config
	/// Config^ cfg = gcnew Config();
	/// // add connectionType to config
	/// Status^ result = cfg->AddValue("connectionType","gmsec_icsswb");
	/// if( result->IsError() ) { /* handle error */ }
	/// Connection^ conn;
	/// // create connection
	/// result = ConnectionFactory::Create(cfg, conn);
	/// if( result->IsError() ) { /* handle error */ }
	/// // do something
	/// // destroy connection
	/// result = ConnectionFactory::Destroy(conn);
	/// if( result->IsError() ) { /* handle error */ }
	/// </code>
	/// <code lang="C#">
	/// // create config
	/// Config cfg = new Config();
	/// // add connectionType to config
	/// Status result = cfg.AddValue("connectionType","gmsec_icsswb");
	/// if (result.IsError()) { /* handle error */ }
	/// Connection conn;
	/// // create connection
	/// result = ConnectionFactory.Create(cfg, out conn);
	/// if (result.IsError()) { /* handle error */ }
	/// // do something
	/// // destroy connection
	/// result = ConnectionFactory.Destroy(conn);
	/// if (result.IsError()) { /* handle error */ }
	/// </code>
	/// <code lang="VB.NET">
	/// ' create config
	/// Dim cfg As New Config()
	/// ' add connectionType to config
	/// Dim result As Status
	/// result = cfg.AddValue("connectionType", "gmsec_icsswb")
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// Dim conn As Connection
	/// conn = Nothing
	/// ' create connection
	/// result = ConnectionFactory.Create(cfg, conn)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// ' do something
	/// ' destroy connection
	/// result = ConnectionFactory.Destroy(conn)
	/// If (result.IsError()) Then
	///     'handle error
	/// End If
	/// </code>
	/// <code lang="f#">
	/// // create config
	/// let cfg = new Config();
	/// // add connectionType to config
	/// let result = cfg.AddValue("connectionType","gmsec_icsswb");
	/// if( result.IsError() ) then () // Handle Error
	/// // create connection
	/// let (result2, conn) = ConnectionFactory.Create(cfg);
	/// if( result2.IsError() ) then () // Handle Error
	/// // do something
	/// // destroy connection
	/// let result3 = ConnectionFactory.Destroy(conn);
	/// if( result3.IsError() ) then () // Handle Error
	/// </code>
	/// </example>
	/// <param name="config">config object specifying connection parameters</param>
	/// <param name="conn">out parameter, created connection</param>
    /// <returns>result of the operation</returns>
	static Nasa::Gmsec::Net::Status ^ Create(Nasa::Gmsec::Net::Config ^ config, 
		[Runtime::InteropServices::Out] Nasa::Gmsec::Net::Connection ^ %conn);

	/// <summary>This function is used to destroy a connection</summary>
	/// <param name="conn">connection to destroy</param>
	/// <returns>status - result of the operation</returns>
	static Nasa::Gmsec::Net::Status ^ Destroy(Nasa::Gmsec::Net::Connection ^ conn);
};

}
}
}

#endif  // ConnectionFactory_Net
