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
/// This class is used to access elements of a configuration file (ConfigFile) object.
/// </summary>

public class ConfigFileIterator : global::System.IDisposable {

  /// <summary>Reports back whether additional configuration is available</summary>
  /// <returns>True if an additional configuration is available; false otherwise.</returns>
  public bool HasNextConfig() {
  }

  /// <summary>Reports back whether additional message is available</summary>
  /// <returns>True if an additional message is available; false otherwise.</returns>
  public bool HasNextMessage() {
  }

  /// <summary>Reports back whether additional subscription is available</summary>
  /// <returns>True if an additional subscriptions is available; false otherwise.</returns>
  public bool HasNextSubscription() {
  }

  /// <summary>Reports back whether additional custom XML element is available</summary>
  /// <returns>True if an additional custom XML element is available; false otherwise.</returns>
  public bool HasNextCustomElement() {
  }

  /// <summary>Reports back the next available custom XML element.</summary>
  /// <returns>The custom XML element string.</returns>
  /// <exception cref="GmsecException">Thrown if there is not an additional custom XML element.</exception>
  public string NextCustomElement() {
  }

  /// <summary>Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFile.</summary>
  public void Reset() {
  }

  /// <summary>Reports back the next available configuration.</summary>
  /// <returns>The next available ConfigEntry.</returns>
  /// <exception cref="GmsecException">Thrown if there is not an additional configuration entry.</exception>
  public ConfigEntry NextConfig() {
  }

  /// <summary>Reports back the next available message.</summary>
  /// <returns>The next available MessageEntry.</returns>
  /// <exception cref="GmsecException">Thrown if there is not an additional message entry.</exception>
  public MessageEntry NextMessage() {
  }

  /// <summary>Reports back the next available subscription.</summary>
  /// <returns>The next available SubscriptionEntry.</returns>
  /// <exception cref="GmsecException">Thrown if there is not an additional subscription entry.</exception>
  public SubscriptionEntry NextSubscription() {
  }

}

}
