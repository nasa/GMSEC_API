/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONFIG_NET_H
#define GMSEC_API_CONFIG_NET_H


// C++ API native
#include <gmsec4/Config.h>


namespace GMSEC
{
namespace API
{


/// <summary>
/// This class is a collection for managing connection configuration items.
/// Encapsulates the parameters necessary for initialization Connections and setting middleware dependent
/// values in Messages.  Used by the ConnectionFactory and Message, this class provides a generic means
/// of supplying initialization data without being Connection/Message specific.
/// </summary>


public ref class Config
{
public:
	/// <summary>Default Constructor</summary>
	Config();


	/// <summary>Constructor</summary>
	///
	/// <param name="argv">array of command-line arguments</param>
	Config(array<System::String^>^ argv);


	/// <summary>
	/// This constructor will create a Config object by parsing key/value pairs from
	/// the given data string.  The data string can contain either XML or JSON formatted
	/// configuration data, or contain raw data consisting of white-space separated key=value
	/// pairs.
	/// </summary>
	///
	/// <param name="data">data string containing configuration information</param>
	///
	/// <exception cref="GMSEC_Exception">Exception is thrown if the data string is not parsable.</exception>
	///
	/// <seealso cref="ToXML()" />
	Config(System::String^ data);


	/// <summary>Basic (and safe) deep copy constructor</summary>
	///
	/// <param name="other">Config object to copy</param>
	Config(Config^ other);


	/// <summary>Destructor</summary>
	~Config();


	/// <summary>Deep-copy assignment operator</summary>
	///
	/// <param name="other">Config object to copy</param>
	Config^ operator=(Config^ other);


	/// <summary>This function adds a value to the config object by name</summary>
	///
	/// <param name="name">name of value to add</param>
	/// <param name="value">value to be added</param>
	///
	/// <exception cref="GMSEC_Exception">Null or empty name or value string given.</exception>
	void AddValue(System::String^ name, System::String^ value);


	/// <summary>This function clears a value by name</summary>
	///
	/// <param name="name">name of value to clear</param>
	///
	/// <returns>Returns true if the operation was a success; false otherwise</returns>
	///
	/// <exception cref="GMSEC_Exception">Null or empty name string given.</exception>
	bool ClearValue(System::String^ name);


	/// <summary>This function gets a value by name. The scope of the value
	/// is the life of the Config object, or until the next %GetValue() call.</summary>
	///
	/// <param name="name">name of value to get</param>
	///
	/// <returns>The value associated with the name, or nullptr if name key does not exist.</returns>
	System::String^ GetValue(System::String^ name);
	
	/// <summary>This function gets a value by name and value or will return a default 
	/// value if key does not exist. The scope of the value is the life of the Config
	/// object, or until the next %GetValue() call.</summary>
	///
	/// <param name="name">name of value to get</param>
	/// <param name="defaultValue">return value if value does not exist</param>
	///
	/// <returns>The value associated with the name, or defaultValue if name key does not exist.</returns>
	System::String^ GetValue(System::String^ name,System::String^ defaultValue);


	/// <summary>This function attempts to get a named value as a boolean. (true/false)
	/// If the field is not set or does not conform to "true" or "false", the an exception
	/// is thrown.</summary>
	///
	/// <param name="name">name of value to get</param>
	///
	/// <returns>value of named item</returns>
	///
	/// <exception cref="GMSEC_Exception">Null or empty string given, name key is not found, or value cannot be converted to a boolean.</exception>
	bool GetBooleanValue(System::String^ name);


	/// <summary>This function attempts to get a named value as a boolean. (true/false)
	/// It will return the default value if the field is not set, or the value does
	/// not conform to "true" or "false".</summary>
	///
	/// <param name="name">name of value to get</param>
	/// <param name="defaultValue">default value describe above</param>
	///
	/// <returns>value of named item, or default value if necessary</returns>
	bool GetBooleanValue(System::String^ name, bool defaultValue);


	/// <summary>This function attempts to get a named value as an integer.
	/// If the field is not set or does not conform to a number, the an exception
	/// is thrown.</summary>
	///
	/// <param name="name">name of value to get</param>
	///
	/// <returns>value of named item</returns>
	///
	/// <exception cref="GMSEC_Exception">Null or empty string given, name key is not found, or value cannot be converted to an integer.</exception>
	System::Int32 GetIntegerValue(System::String^ name);


	/// <summary>This function attempts to get a named value as an integer.
	/// It will return the default value if the field is not set, or the value does not conform to a number.</summary>
	///
	/// <param name="name">name of value to get</param>
	/// <param name="defaultValue">default value describe above</param>
	///
	/// <returns>value of named item, or default value if necessary</returns>
	System::Int32 GetIntegerValue(System::String^ name, System::Int32 defaultValue);


	/// <summary>This function attempts to get a named value as a 64-bit floating point number.</summary>
	///
	/// <param name="name">name of value to get</param>
	///
	/// <returns>value of named item</returns>
	///
	/// <exception cref="GMSEC_Exception">Null or empty string given, name key is not found, or value cannot be converted to a double.</exception>
	System::Double GetDoubleValue(System::String^ name);


	/// <summary>This function attempts to get a named value as a 64-bit floating point number.</summary>
	///
	/// <param name="name">name of value to get</param>
	/// <param name="defaultValue">default value describe above</param>
	///
	/// <returns>value of named item, or default value if necessary</returns>
	System::Double GetDoubleValue(System::String^ name, System::Double defaultValue);


	/// <summary>This function clears all values</summary>
	void Clear();


	/// <summary>This function gets the first name and value for iteration. The scope
	/// of the name and value are the life of the Config object, or
	/// until the next GetFirst() call.</summary>
	///
	/// <param name="name">output parameter name of value</param>
	/// <param name="value">output parameter value</param>
	///
	/// <returns>boolean status as to whether the name/value pair are available</returns>
	bool GetFirst([System::Runtime::InteropServices::Out] System::String^ %name, [System::Runtime::InteropServices::Out] System::String^ %value);


	/// <summary>This function gets the next name and value for iteration. The scope
	/// of the name and value are the life of the Config object, or
	/// until the next GetNext() call.</summary>
	///
	/// <param name="name">output parameter name of value</param>
	/// <param name="value">output parameter value</param>
	///
	/// <returns>boolean status as to whether the name/value pair are available</returns>
	bool GetNext([System::Runtime::InteropServices::Out] System::String^ %name, [System::Runtime::InteropServices::Out] System::String^ %value);


	/// <summary>Merges the given %Config with the existing %Config object.</summary>
	///
	/// <param name="other">Configuration object from which to read name/value pairs.</param>
	/// <param name="overwriteExisting">Indicates whether to overwrite existing name/value
	/// pairs within the working %Config object.</param>
	void Merge(Config^ other, bool overwriteExisting);


	/// <summary>This function will dump this config to XML.  The format is the same as that which is parsed by FromXML().</summary>
	///
	/// <seealso cref="FromXML()" />
	///
	/// <returns>An XML string representation of the config object.</returns>
	System::String^ ToXML();


	/// <summary>This function will read a config from XML.
	/// The format is the same as that which is generated by ToXML().</summary>
	///
	/// <example>
	/// <code lang="xml" xml:space="preserve">
	/// <CONFIG>
	///    <PARAMETER NAME="connectiontype">gmsec_bolt</PARAMETER>
	///    <PARAMETER NAME="server">127.0.0.1</PARAMETER>
	///    <PARAMETER NAME="tracking">true</PARAMETER>
	/// </CONFIG>
	/// </code>
	/// </example>
	///
	/// <seealso cref="ToXML()" />
	///
	/// <param name="xml">XML string</param>
	///
	/// <exception cref="GMSEC_Exception">If a malformed XML, or null, string is given.</exception>
	void FromXML(System::String^ xml);


	/// <summary>This function will dump this config to JSON.
	///
	/// <returns>A JSON string representation of the config object.</returns>
	System::String^ ToJSON();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!Config();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="config">unmanaged implementation to initialize with</param>
	Config(gmsec::api::Config* config, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::Config* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::Config* config, bool owned);


private:
	gmsec::api::Config* m_impl;
	bool                m_owned;
};

} // end namespace API
} // end namespace GMSEC

#endif
