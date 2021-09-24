
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
#ifndef Config_Net
#define Config_Net

// managed
#include "status_Net.h"
#include "GMSEC_Net.h"
#include "GMSECErrors_Net.h"

// native
#include "gmsec/Config.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

// Forward references
ref class Connection;
ref class Message;

/// <summary>
/// This class is a collection for managing connection configuration items.
/// Encapsulates the parameters necessary for initialization Connections and setting middleware dependent
/// values in Messages.  Used by the ConnectionFactory and Message, this class provides a generic means
/// of supplying initialization data without being Connection/Message specific.
/// </summary>
/// <example>
/// <code lang="C++">
/// Config^ cfg = gcnew Config(args);	// create from main parameters
/// String^ name = nullptr;
/// String^ value = nullptr;
/// Status^ res = cfg->GetFirst(name, value);	// get first value
/// while(!res->IsError()) {
///		Console::WriteLine(name + "," + value);
///		res = cfg->GetNext(name, value);	// get next value
/// }
/// </code>
/// <code lang="C#">
/// Config cfg = new Config(args);	// create from main parameters
/// String name = null;
/// String value = null;
/// Status res = cfg.GetFirst(out name, out value);	// get first value
/// while(!res.IsError()) {
///		Console.WriteLine(name + "," + value);
///		res = cfg.GetNext(out name, out value);	// get next value
/// }
/// </code>
/// <code lang="VB.NET">
/// Dim cfg As New Config(args)
/// Dim name As String
/// Dim value As String
/// name = Nothing
/// value = Nothing
/// Dim result As Status
/// result = cfg.GetFirst(name, value)  ' get first value
/// While (Not result.IsError())
///     Console.WriteLine(name + "," + value)
///     result = cfg.GetNext(name, value)  ' get next value
/// End While
/// </code>
/// <code lang="f#">
/// let cfg = new Config(args)// create from main parameters
/// let (res, name, value) = cfg.GetFirst(); // get first value
/// let mutable IsError = res.IsError()
/// let mutable nameTop = name
/// let mutable valueTop = value
/// while not IsError do
///     Console.WriteLine(nameTop + "," + valueTop)
///     let (res2, name2, value2) = cfg.GetNext() // get next value
///     IsError &lt;- res2.IsError()
///     nameTop &lt;- name2
///     valueTop &lt;- value2
/// </code>
/// </example>
/// <seealso cref="Connection" />
/// <seealso cref="Message" />
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class Config
{
private:
	gmsec::Config * mImpl;
	
	/// <summary>Generates a default config, with no parameters</summary>
	inline void GenerateDefaultConfig();

internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	/// <param name="config">unmanaged implementation to initialize with</param>
	Config(gmsec::Config* config);

	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::Config * GetUnmanImpl();

	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::Config* config);

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Config();

public:
	/// <summary>Default Constructor</summary>
	Config();

	/// <summary>Constructor</summary>
	/// <param name="argv">argv</param>
	Config(array<String ^> ^  argv);

	/// <summary>This constructor will create a config and parse values from the passed in xml string.
	/// The xml format is the same as used by the FromXML() and ToXML() functions.</summary>
	/// <seealso cref="FromXML()" />
	/// <seealso cref="ToXML()" />
	/// <param name="xml">string in xml format (see FromXML)</param>
	Config(const String ^ xml);

	/// <summary>Basic (and safe) deep copy constructor</summary>
	/// <param name="config">config object to copy</param>
	Config(const Nasa::Gmsec::Net::Config ^ config);

	/// <summary>brief destructor</summary>
	~Config();

	/// <summary>This function adds a value to the config object by name</summary>
	/// <param name="name">name of value to add</param>
	/// <param name="value">value to be added</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ AddValue(const String ^ name, const String ^ value);

	/// <summary>This function clears a value by name</summary>
	/// <param name="name">name of value to clear</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ ClearValue(const String ^ name);

	/// <summary>This function gets a value by name. The scope of the value
	/// is the life of the Config object, or until the next %GetValue() call.</summary>
	/// <param name="name">name of value to get</param>
	/// <param name="value">output parameter value</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetValue(const String ^ name, 
		[Runtime::InteropServices::Out] const String ^ %value);

	/// <summary>This function attempts to get a named value as a boolean. (true/false)
	/// It will return the default value if the field is not set, or the value does
	/// not conform to "true" or "false".</summary>
	/// <param name="name">name of value to get</param>
	/// <param name="defaultValue">default value describe above</param>
	/// <returns>value of named item</returns>
	bool CheckBoolValue(const String ^ name, bool defaultValue);

	/// <summary>This function clears all values</summary>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ Clear();

	/// <summary>This function gets the first name and value for iteration. The scope
	/// of the name and value are the life of the Config object, or
	/// until the next GetFirst() call.</summary>
	/// <param name="name">output parameter name of value</param>
	/// <param name="value">output parameter value</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetFirst( 
		[Runtime::InteropServices::Out] const String ^ %name, 
		[Runtime::InteropServices::Out] const String ^ %value);

	/// <summary>This function gets the next name and value for iteration. The scope
	/// of the name and value are the life of the Config object, or
	/// until the next GetNext() call.</summary>
	/// <param name="name">output parameter name of value</param>
	/// <param name="value">output parameter value</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ GetNext( 
		[Runtime::InteropServices::Out] const String ^ %name, 
		[Runtime::InteropServices::Out] const String ^ %value);

	/// <summary>This function will dump this config to XML.The format is the same as that which is parsed by FromXML().</summary>
	/// <seealso cref="FromXML()" />
	/// <param name="xml">output parameter: xml string</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ ToXML( 
		[Runtime::InteropServices::Out] const String ^ %xml);

	/// <summary>This function will read a config from XML
	/// The format is the same as that which is generated by ToXML().</summary>
	/// <example>
	/// <code lang="xml" xml:space="preserve">
	/// <CONFIG>
	///    <PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
	///    <PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
	///    <PARAMETER NAME="tracking">true</PARAMETER>
	///    <PARAMETER NAME="isthreaded">true</PARAMETER>
	/// </CONFIG>
	/// </code>
	/// </example>
	/// <seealso cref="ToXML()" />
	/// <param name="xml">xml string</param>
	/// <returns>status object with the result of the operation</returns>
	Nasa::Gmsec::Net::Status ^ FromXML(const String ^ xml);
};

}
}
}

#endif  // Config_Net

