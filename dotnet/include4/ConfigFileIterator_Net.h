/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_CONFIG_FILE_ITERATOR_NET_H
#define GMSEC_API_CONFIG_FILE_ITERATOR_NET_H

#include <ConfigFile_Net.h>

// C++ API native
#include <gmsec4/ConfigFileIterator.h>


namespace GMSEC
{
namespace API
{

/// <summary>
/// This class is used to access elements of a configuration file (ConfigFile) object.
/// </summary>


public ref class ConfigFileIterator
{
public:
	~ConfigFileIterator();


	/// <summary>Reports back whether additional configuration is available</summary>
	/// <returns>True if an additional configuration is available; false otherwise.</returns>
	bool HasNextConfig();


	/// <summary>Reports back whether additional message is available</summary>
	/// <returns>True if an additional message is available; false otherwise.</returns>
	bool HasNextMessage();


	/// <summary>Reports back whether additional subscription is available</summary>
	/// <returns>True if an additional subscriptions is available; false otherwise.</returns>
	bool HasNextSubscription();


	/// <summary>Reports back whether additional custom XML element is available</summary>
	/// <returns>True if an additional custom XML element is available; false otherwise.</returns>
	bool HasNextCustomElement();


	/// <summary>Reports back the next available configuration.</summary>
	/// <returns>The next available ConfigEntry.</returns>
	/// <exception cref="GMSEC_Exception">Thrown if there is not an additional configuration entry.</exception>
	ConfigFile::ConfigEntry^ NextConfig();


	/// <summary>Reports back the next available message.</summary>
	/// <returns>The next available MessageEntry.</returns>
	/// <exception cref="GMSEC_Exception">Thrown if there is not an additional message entry.</exception>
	ConfigFile::MessageEntry^ NextMessage();


	/// <summary>Reports back the next available subscription.</summary>
	/// <returns>The next available SubscriptionEntry.</returns>
	/// <exception cref="GMSEC_Exception">Thrown if there is not an additional subscription entry.</exception>
	ConfigFile::SubscriptionEntry^ NextSubscription();


	/// <summary>Reports back the next available custom XML element.</summary>
	/// <returns>The custom XML element string.</returns>
	/// <exception cref="GMSEC_Exception">Thrown if there is not an additional custom XML element.</exception>
	System::String^ NextCustomElement();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ConfigFileIterator();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="iter">unmanaged implementation to initialize with</param>
	ConfigFileIterator(gmsec::api::ConfigFileIterator* iter);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::ConfigFileIterator* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::ConfigFileIterator* iter);


private:
	ConfigFileIterator();

	gmsec::api::ConfigFileIterator* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
