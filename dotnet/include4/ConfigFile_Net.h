/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONFIG_FILE_NET_H
#define GMSEC_API_CONFIG_FILE_NET_H


// C++ API native
#include <gmsec4/ConfigFile.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Config;
ref class ConfigFileIterator;
ref class Message;


/// <summary>
/// This class is for the managment of standard configuration files.
/// </summary>


public ref class ConfigFile
{
public:
	/// <summary>This class provides access to the attributes associated with a ConfigFile's Config entry.</summary>
	ref class ConfigEntry
	{
	public:
		/// <summary>Returns the name associated with the Config entry.</summary>
		System::String^ GetName();

		/// <summary>Returns the Config object associated with the Config entry.</summary>
		Config^ GetConfig();

		~ConfigEntry();

	protected:
		/// <summary>Finalizer, free memory and set objects to null</summary>
		!ConfigEntry();

	internal:
		/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
		///
		/// <param name="iter">unmanaged implementation to initialize with</param>
		ConfigEntry(gmsec::api::ConfigFile::ConfigEntry* entry);


		/// <summary>Get unmanaged implementation version</summary>
		///
		/// <returns>Unmanaged implementation</returns>
		gmsec::api::ConfigFile::ConfigEntry* GetUnmanagedImplementation();


		/// <summary>Reset unmanaged implementation to null</summary>
		void ResetUnmanagedImplementation();


		/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
		void SetUnmanagedImplementation(gmsec::api::ConfigFile::ConfigEntry* entry);


	private:
		ConfigEntry();

		gmsec::api::ConfigFile::ConfigEntry* m_entry;
	};


	/// <summary>This class provides access to the attributes associated with a ConfigFile's Message entry.</summary>
	ref class MessageEntry
	{
	public:
		/// <summary>Returns the name associated with the Message entry.</summary>
		System::String^ GetName();

		/// <summary>Returns the Message object associated with the Message entry.</summary>
		Message^ GetMessage();

		~MessageEntry();

	protected:
		/// <summary>Finalizer, free memory and set objects to null</summary>
		!MessageEntry();

	internal:
		/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
		///
		/// <param name="iter">unmanaged implementation to initialize with</param>
		MessageEntry(gmsec::api::ConfigFile::MessageEntry* entry);


		/// <summary>Get unmanaged implementation version</summary>
		///
		/// <returns>Unmanaged implementation</returns>
		gmsec::api::ConfigFile::MessageEntry* GetUnmanagedImplementation();


		/// <summary>Reset unmanaged implementation to null</summary>
		void ResetUnmanagedImplementation();


		/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
		void SetUnmanagedImplementation(gmsec::api::ConfigFile::MessageEntry* entry);


	private:
		MessageEntry();

		gmsec::api::ConfigFile::MessageEntry* m_entry;
	};


	/// <summary>This class provides access to the attributes associated with a ConfigFile's Subscription entry.</summary>
	ref class SubscriptionEntry
	{
	public:
		/// <summary>Returns the name associated with the Subscription entry.</summary>
		System::String^ GetName();

		/// <summary>Returns the subject/topic associated with the Subscription entry.</summary>
		///<note>This function has been deprecated; use getPattern() instead.</note>
		System::String^ GetSubject();
		
		/// <summary>Returns the pattern associated with the Subscription entry.</summary>
		System::String^ GetPattern();
		
		/// <summary>Returns the next excluded pattern associated with the Subscription entry.</summary>
		System::String^ NextExcludedPattern();

		/// <summary>Returns whether there is a next excluded pattern associated with the Subscription entry.</summary>
		bool HasNextExcludedPattern();	

		~SubscriptionEntry();

	protected:
		/// <summary>Finalizer, free memory and set objects to null</summary>
		!SubscriptionEntry();

	internal:
		/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
		///
		/// <param name="iter">unmanaged implementation to initialize with</param>
		SubscriptionEntry(gmsec::api::ConfigFile::SubscriptionEntry* entry);


		/// <summary>Get unmanaged implementation version</summary>
		///
		/// <returns>Unmanaged implementation</returns>
		gmsec::api::ConfigFile::SubscriptionEntry* GetUnmanagedImplementation();


		/// <summary>Reset unmanaged implementation to null</summary>
		void ResetUnmanagedImplementation();


		/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
		void SetUnmanagedImplementation(gmsec::api::ConfigFile::SubscriptionEntry* entry);


	private:
		SubscriptionEntry();

		gmsec::api::ConfigFile::SubscriptionEntry* m_entry;
	};


	/// <summary>Constructor</summary>
	ConfigFile();


	/// <summary>Destructor</summary>
	~ConfigFile();


	/// <summary>Loads an XML configuration file.</summary>
	/// <param name="filePath">The file path where from where to load the configuration file</param>
	/// <exception cref="GMSEC_Exception">Thrown if the configuration file cannot be loaded or contains XML syntax issues.</exception>
	void Load(System::String^ filePath);


	/// <summary>Saves configuration file back to the file it was originally loaded from.</summary>
	/// <exception cref="GMSEC_Exception">Thrown if the configuration file cannot be written.</exception>
	void Save();


	/// <summary>Saves configuration file to the specified file path.</summary>
	/// <param name="filePath">The file path where to store the configuration file</param>
	/// <exception cref="GMSEC_Exception">Thrown if the configuration file cannot be written.</exception>
	void Save(System::String^ filePath);


	/// <summary>Saves configuration file to the specified file path.  The file is stored compactly if so indicated.</summary>
	/// <param name="filePath">The file path where to store the configuration file</param>
	/// <param name="compact">Boolean flat to indicate whether the XML file should be stored compactly.</param>
	/// <exception cref="GMSEC_Exception">Thrown if the configuration file cannot be written.</exception>
	void Save(System::String^ filePath, bool compact);


	/// <summary>Initializes the configuration file using the given XML string.</summary>
	/// <param name="xml">The XML string representation of a configuration file.</param>
	/// <exception cref="GMSEC_Exception">Thrown if the XML string contains syntax issues.</exception>
	void FromXML(System::String^ xml);


	/// <summary>Returns the XML string representation of the configuration file.</summary>
	System::String^ ToXML();


	/// <summary>Returns whether the configuration file has been initialized (either loaded or via an XML string)</summary>
	bool IsLoaded();


	/// <summary>Returns the Config object associated with the given name.</summary>
	/// <param name="name">The name of the Config object</param>
	/// <exception cref="GMSEC_Exception">Thrown if a Config object with the given name does not exist.</exception>
	Config^ LookupConfig(System::String^ name);


	/// <summary>Returns the Message object associated with the given name.</summary>
	/// <param name="name">The name of the Message object</param>
	/// <exception cref="GMSEC_Exception">Thrown if a Message object with the given name does not exist.</exception>
	Message^ LookupMessage(System::String^ name);


	/// <summary>Returns the subscription subject/topic associated with the given name.</summary>
	///<note>This function has been deprecated; use lookupsubscriptionEntry instead.</note>
	/// <param name="name">The name of the subscription</param>
	/// <exception cref="GMSEC_Exception">Thrown if a subscription with the given name does not exist.</exception>
	System::String^ LookupSubscription(System::String^ name);


	/// <summary>Returns the Subscription Entry associated with the given name.</summary>
	/// <param name="name">The name of the subscription</param>
	/// <exception cref="GMSEC_Exception">Thrown if a Subscription Entry with the given name does not exist.</exception>
	ConfigFile::SubscriptionEntry^ LookupSubscriptionEntry(System::String^ name);


	/// <summary>Adds the subscription subject/topic with the given name.</summary>
	/// <param name="name">The name of the subscription</param>
	/// <param name="subscription">The subscription subject/topic</param>
	void AddSubscription(System::String^ name, System::String^ subscription);


	/// <summary>Adds the Config object with the given name.</summary>
	/// <param name="name">The name of the Config object</param>
	/// <param name="config">The Config object</param>
	void AddConfig(System::String^ name, Config^ config);


	/// <summary>Adds the Message object with the given name.</summary>
	/// <param name="name">The name of the Message object</param>
	/// <param name="message">The Message object</param>
	void AddMessage(System::String^ name, Message^ message);


	/// <summary>Adds the custom XML element string.</summary>
	/// <param name="xml">The custom XML element string</param>
	/// <exception cref="GMSEC_Exception">Thrown if the XML string contains syntax issues.</exception>
	void AddCustomXML(System::String^ xml);


	/// <summary>Removes the subscription associated with the given name.</summary>
	/// <param name="name">The name of the subscription to remove.</param>
	/// <returns>True if the operation is successful; false otherwise.
	bool RemoveSubscription(System::String^ name);


	/// <summary>Removes the Config object associated with the given name.</summary>
	/// <param name="name">The name of the Config object to remove.</param>
	/// <returns>True if the operation is successful; false otherwise.
	bool RemoveConfig(System::String^ name);


	/// <summary>Removes the Message object associated with the given name.</summary>
	/// <param name="name">The name of the Message object to remove.</param>
	/// <returns>True if the operation is successful; false otherwise.
	bool RemoveMessage(System::String^ name);


	/// <summary>Removes the custom XML element.</summary>
	/// <param name="xml">The custom XML element to remove.</param>
	/// <returns>True if the operation is successful; false otherwise.
	bool RemoveCustomXML(System::String^ xml);


	/// <summary>Returns the ConfigFileIterator that may be used to iterate over the entries of the ConfigFile.</summary>
	ConfigFileIterator^ GetIterator();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ConfigFile();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="cfgFile">unmanaged implementation to initialize with</param>
	ConfigFile(gmsec::api::ConfigFile* cfgFile);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::ConfigFile* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::ConfigFile* cfgFile);


private:
	gmsec::api::ConfigFile* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
