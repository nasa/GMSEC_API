/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>
/// This class is for the managment of standard configuration files.
/// </summary>

public class ConfigFile : global::System.IDisposable {

  /// <summary>
  /// Constructor that creates an empty configuration file object.
  /// The internally held MessageFactory will be configured with the default message specification used by the API.
  /// </summary>
  /// <exception cref="GmsecException">Thrown if the MessageFactory cannot be instantiated.</exception>
  public ConfigFile() {
  }

  /// <summary>
  /// Constructor that creates an empty configuration file object.
  /// The given configuration will be applied to the internally held MessageFactory.
  /// <p>
  /// Configuration options referenced are:
  /// <ul>
  ///    <li>gmsec-specification-version</li>
  ///    <li>gmsec-schema-level</li>
  ///    <li>gmsec-msg-content-validate</li>
  /// </ul>
  /// </summary>
  /// <param name="config">The configuration to apply to the internally held MessageFactory.</param>
  /// <exception cref="GmsecException">Thrown if the MessageFactory cannot be instantiated.</exception>
  public ConfigFile(Config config) {
  }

  /// <summary>Loads an XML configuration file.</summary>
  /// <param name="filePath">The file path where from where to load the configuration file</param>
  /// <exception cref="GmsecException">Thrown if the configuration file cannot be loaded or contains XML syntax issues.</exception>
  public void Load(string filePath) {
  }

  /// <summary>Saves configuration file to the specified file path. The file is stored compactly if so indicated.</summary>
  /// <param name="filePath">The file path where to store the configuration file</param>
  /// <param name="compact">Boolean flat to indicate whether the XML file should be stored compactly.</param>
  /// <exception cref="GmsecException">Thrown if the configuration file cannot be written.</exception>
  public void Save(string filePath, bool compact) {
  }

  /// <summary>Saves configuration file to the specified file path.</summary>
  /// <param name="filePath">The file path where to store the configuration file</param>
  /// <exception cref="GmsecException">Thrown if the configuration file cannot be written.</exception>
  public void Save(string filePath) {
  }

  /// <summary>Saves configuration file back to the file it was originally loaded from.</summary>
  /// <exception cref="GmsecException">Thrown if the configuration file cannot be written.</exception>
  public void Save() {
  }

  /// <summary>Initializes the configuration file using the given XML string.</summary>
  /// <param name="xml">The XML string representation of a configuration file.</param>
  /// <exception cref="GmsecException">Thrown if the XML string contains syntax issues.</exception>
  public void FromXML(string xml) {
  }

  /// <summary>Returns the XML string representation of the configuration file.</summary>
  public string ToXML() {
  }

  /// <summary>Returns whether the configuration file has been initialized (either loaded or via an XML string)</summary>
  public bool IsLoaded() {
  }

  /// <summary>Adds the Config object with the given name.</summary>
  /// <param name="name">The name of the Config object</param>
  /// <param name="config">The Config object</param>
  public void AddConfig(string name, Config config) {
  }

  /// <summary>Returns the Config object associated with the given name.</summary>
  /// <param name="name">The name of the Config object</param>
  /// <exception cref="GmsecException">Thrown if a Config object with the given name does not exist.</exception>
  public Config LookupConfig(string name) {
  }

  /// <summary>Removes the Config object associated with the given name.</summary>
  /// <param name="name">The name of the Config object to remove.</param>
  /// <returns>True if the operation is successful; false otherwise.
  public bool RemoveConfig(string name) {
  }

  /// <summary>Adds the Message object with the given name.</summary>
  /// <param name="name">The name of the Message object</param>
  /// <param name="message">The Message object</param>
  public void AddMessage(string name, Message message) {
  }

  /// <summary>Returns the Message object associated with the given name.</summary>
  /// <param name="name">The name of the Message object</param>
  /// <exception cref="GmsecException">Thrown if a Message object with the given name does not exist.</exception>
  public Message LookupMessage(string name) {
  }

  /// <summary>Removes the Message object associated with the given name.</summary>
  /// <param name="name">The name of the Message object to remove.</param>
  /// <returns>True if the operation is successful; false otherwise.
  public bool RemoveMessage(string name) {
  }

  /// <summary>Adds the SubscriptionEntry definition to the list of subscriptions.</summary>
  /// <param name="entry">A SubscriptionEntry object</param>
  public void AddSubscriptionEntry(SubscriptionEntry entry) {
  }

  /// <summary>Returns the Subscription Entry associated with the given name.</summary>
  /// <param name="name">The name of the subscription</param>
  /// <exception cref="GmsecException">Thrown if a Subscription Entry with the given name does not exist.</exception>
  public SubscriptionEntry LookupSubscriptionEntry(string name) {
  }

  /// <summary>Removes the SubscriptionEntry associated with the given name.</summary>
  /// <param name="name">The name of the subscription to remove.</param>
  /// <returns>True if the operation is successful; false otherwise.
  public bool RemoveSubscriptionEntry(string name) {
  }

  /// <summary>Returns the ConfigFileIterator that may be used to iterate over the entries of the ConfigFile.</summary>
  public ConfigFileIterator GetIterator() {
  }
}

}
