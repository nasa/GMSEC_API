/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>
/// This class is a collection for managing connection configuration items.
/// Encapsulates the parameters necessary for initialization Connections and setting middleware dependent
/// values in Messages.  Used by the ConnectionFactory and Message, this class provides a generic means
/// of supplying initialization data without being Connection/Message specific.
/// </summary>

public class Config : global::System.IDisposable {

  /// <summary>
  /// Returns a Config object that has been initialized with the given string array of
  /// name=value pairs.
  /// </summary>
  /// <remarks>This method has been deprecated; use Config(string[]) instead.</remarks>
  public static Config Initialize(string[] args) {
  }

  /// <summary>Default Constructor</summary>
  public Config() {
  }

  /// <summary>
  /// Constructor that will parse command line parameters and add values for any param
  /// that is of the form name=value.
  /// </summary>
  ///
  /// <param name="args">The string array of command line parameters</param>
  public Config(string[] args) {
  }

  /// <summary>
  /// This constructor will create a Config object by parsing name/value pairs from
  /// the given data string.  The data string can contain either XML or JSON formatted
  /// configuration data, or contain raw data consisting of white-space separated name=value
  /// pairs.
  /// </summary>
  ///
  /// <param name="data">data string containing configuration information</param>
  ///
  /// <exception cref="GmsecException">Exception is thrown if the data string is not parsable.</exception>
  ///
  /// <seealso cref="ToXML()" />
  public Config(string data) {
  }

  /// <summary>Basic (and safe) deep copy constructor</summary>
  ///
  /// <param name="other">Config object to copy</param>
  public Config(Config other) {
  }

  /// <summary>This method adds a value to the config object by name</summary>
  ///
  /// <param name="name">name of value to add</param>
  /// <param name="value">value to be added</param>
  ///
  /// <exception cref="GmsecException">Null or empty name or value string given.</exception>
  public void AddValue(string name, string value) {
  }

  /// <summary>This method clears a value by name</summary>
  ///
  /// <param name="name">name of value to clear</param>
  ///
  /// <returns>Returns true if the operation was a success; false otherwise</returns>
  ///
  /// <exception cref="GmsecException">Null or empty name string given.</exception>
  public bool ClearValue(string name) {
  }

  /// <summary>This method gets a value by name. The scope of the value
  /// is the life of the Config object, or until the next %GetValue() call.</summary>
  ///
  /// <param name="name">name of value to get</param>
  ///
  /// <returns>The value associated with the name, or null if name does not exist.</returns>
  public string GetValue(string name) {
  }

  /// <summary>This method gets a value by name and value or will return a default 
  /// value if name does not exist. The scope of the value is the life of the Config
  /// object, or until the next %GetValue() call.</summary>
  ///
  /// <param name="name">name of value to get</param>
  /// <param name="defaultValue">return value if value does not exist</param>
  ///
  /// <returns>The value associated with the name, or defaultValue if name does not exist.</returns>
  public string GetValue(string name, string defaultValue) {
  }

  /// <summary>This method attempts to get a named value as a boolean. (true/false)
  /// If the field is not set or does not conform to "true" or "false", the an exception
  /// is thrown.</summary>
  ///
  /// <param name="name">name of value to get</param>
  ///
  /// <returns>value of named item</returns>
  ///
  /// <exception cref="GmsecException">Null or empty string given, name is not found, or value cannot be converted to a boolean.</exception>
  public bool GetBooleanValue(string name) {
  }

  /// <summary>This method attempts to get a named value as a boolean. (true/false)
  /// It will return the default value if the field is not set, or the value does
  /// not conform to "true" or "false".</summary>
  ///
  /// <param name="name">name of value to get</param>
  /// <param name="defaultValue">default value describe above</param>
  ///
  /// <returns>value of named item, or default value if necessary</returns>
  public bool GetBooleanValue(string name, bool defaultValue) {
  }

  /// <summary>This method attempts to get a named value as an integer.
  /// If the field is not set or does not conform to a number, the an exception
  /// is thrown.</summary>
  ///
  /// <param name="name">name of value to get</param>
  ///
  /// <returns>value of named item</returns>
  ///
  /// <exception cref="GmsecException">Null or empty string given, name is not found, or value cannot be converted to an integer.</exception>
  public int GetIntegerValue(string name) {
  }

  /// <summary>This method attempts to get a named value as an integer.
  /// It will return the default value if the field is not set, or the value does not conform to a number.</summary>
  ///
  /// <param name="name">name of value to get</param>
  /// <param name="defaultValue">default value describe above</param>
  ///
  /// <returns>value of named item, or default value if necessary</returns>
  public int GetIntegerValue(string name, int defaultValue) {
  }

  /// <summary>This method attempts to get a named value as a 64-bit floating point number.</summary>
  ///
  /// <param name="name">name of value to get</param>
  ///
  /// <returns>value of named item</returns>
  ///
  /// <exception cref="GmsecException">Null or empty string given, name is not found, or value cannot be converted to a double.</exception>
  public double GetDoubleValue(string name) {
  }

  /// <summary>This method attempts to get a named value as a 64-bit floating point number.</summary>
  ///
  /// <param name="name">name of value to get</param>
  /// <param name="defaultValue">default value describe above</param>
  ///
  /// <returns>value of named item, or default value if necessary</returns>
  public double GetDoubleValue(string name, double defaultValue) {
  }

  /// <summary>This method clears all values</summary>
  public void Clear() {
  }

  /// <summary>Merges the given %Config with the existing %Config object.</summary>
  ///
  /// <param name="other">Configuration object from which to read name/value pairs.</param>
  /// <param name="overwriteExisting">Indicates whether to overwrite existing name/value
  /// pairs within the working %Config object.</param>
  public void Merge(Config other, bool overwriteExisting) {
  }

  /// <summary>Merges the given %Config with the existing %Config object.</summary>
  ///
  /// <param name="other">Configuration object from which to read name/value pairs.</param>
  public void Merge(Config other) {
  }

  /// <summary>This method will dump this config to XML.  The format is the same as that which is parsed by FromXML().</summary>
  ///
  /// <seealso cref="FromXML()" />
  ///
  /// <returns>An XML string representation of the config object.</returns>
  public string ToXML() {
  }

  /// <summary>This method will read a config from XML.
  /// The format is the same as that which is generated by ToXML().</summary>
  ///
  /// <seealso cref="ToXML()" />
  ///
  /// <param name="xml">XML string</param>
  ///
  /// <exception cref="GmsecException">If a malformed XML, or null, string is given.</exception>
  public void FromXML(string xml) {
  }

  /// <summary>This method will dump this config to JSON.
  ///
  /// <returns>A JSON string representation of the config object.</returns>
  public string ToJSON() {
  }

  /// <summary>
  /// This method gets the first name and value pair for iteration.
  /// The scope of the name and value are the life of the Config object, or until the next GetFirst() call.
  /// </summary>
  ///
  /// <param name="cp">Structure containing name/value pair</param>
  ///
  /// <returns>Status as to whether name/value pair are available.</returns>
  ///
  /// <seealso cref="ConfigPair"/>
  /// <seealso cref="GetNext"/>
  public bool GetFirst(ConfigPair cp) {
  }

  /// <summary>
  /// This method gets the next name and value pair for iteration.
  /// The scope of the name and value are the life of the Config object, or until the next GetNext() call.
  /// </summary>
  ///
  /// <param name="cp">Structure containing name/value pair</param>
  ///
  /// <returns>Status as to whether name/value pair are available.</returns>
  ///
  /// <seealso cref="ConfigPair"/>
  /// <seealso cref="GetFirst"/>
  public bool GetNext(ConfigPair cp) {
  }

}

}
