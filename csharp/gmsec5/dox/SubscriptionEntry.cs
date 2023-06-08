/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API5 {

/// <summary>This class provides access to the attributes associated with a ConfigFile's subscription entry.</summary>

public class SubscriptionEntry : global::System.IDisposable {

  /// <summary>Basic constructor.</summary>
  /// <param name="name">The name to associate with the entry</param>
  /// <param name="pattern">The subscription topic to associate with the entry</param>
  /// <exception cref="GmsecException">Thrown if the name is null or is an empty string.</exception>
  /// <exception cref="GmsecException">Thrown if the pattern is null or is an empty string.</exception>
  public SubscriptionEntry(string name, string pattern) {
  }

  /// <summary>Copy constructor.</summary>
  /// <param name="other">The SubscriptionEntry object to copy</param>
  public SubscriptionEntry(SubscriptionEntry other) {
  }

  /// <summary>Returns the name associated with the entry.</summary>
  public string GetName() {
  }

  /// <summary>Returns the pattern associated with the subscription entry.</summary>
  public string GetPattern() {
  }

  /// <summary>Adds an excluded subject pattern/topic to the SubscriptionEntry.</summary>
  /// <param name="pattern">The subscription topic to exclude</param>
  /// <exception cref="GmsecException">Thrown if the pattern is null or is an empty string.</exception>
  public void AddExcludedPattern(string pattern) {
  }

  /// <summary>Returns whether there is a next excluded pattern associated with the subscription entry.</summary>
  public bool HasNextExcludedPattern() {
  }

  /// <summary>Returns the next excluded pattern associated with the subscription entry.</summary>
  public string NextExcludedPattern() {
  }

}

}
