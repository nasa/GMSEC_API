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
/// This class is a lightweight container for holding information on a %Mnemonic,
/// and used to generate a %GMSEC MnemonicMessage.
/// </summary>

public class Mnemonic : global::System.IDisposable {

  /// <summary>Constructor</summary>
  ///
  /// <param name="name">%Mnemonic name</param>
  /// <param name="samples">List of MnemonicSample objects</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  public Mnemonic(string name, MnemonicSampleList samples) {
  }

  /// <summary>Basic Constructor</summary>
  ///
  /// <param name="name">%Mnemonic name</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  public Mnemonic(string name) {
  }

  /// <summary>Copy-constructor for making deep-copy of other %Mnemonic object.</summary>
  ///
  /// <param name="other">The other %Mnemonic object to copy</param>
  public Mnemonic(Mnemonic other) {
  }

  /// <summary>Returns the name associated with the mnemonic.</summary>
  public string GetName() {
  }

  /// <summary>Returns whether there is a status Field associated with the mnemonic.</summary>
  ///
  /// <seealso cref="Mnemonic.GetStatus"/>
  /// <seealso cref="Mnemonic.SetStatus"/>
  public bool StatusAvailable() {
  }

  /// <summary>Returns a reference to the status Field</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a status Field has not been set within the mnemonic</exception>
  ///
  /// <seealso cref="Mnemonic.StatusAvailable"/>
  /// <seealso cref="Mnemonic.SetStatus"/>
  public Field GetStatus() {
  }

  /// <summary>Sets the status Field within the mnemonic.</summary>
  ///
  /// <param name="status">The Field object containing a status value</param>
  ///
  /// <seealso cref="Mnemonic.StatusAvailable"/>
  /// <seealso cref="Mnemonic.GetStatus"/>
  public void SetStatus(Field status) {
  }

  /// <summary>Returns whether there is a unit string associated with the mnemonic.</summary>
  ///
  /// <seealso cref="Mnemonic.GetUnits"/>
  /// <seealso cref="Mnemonic.SetUnits"/>
  public bool UnitsAvailable() {
  }

  /// <summary>Returns a reference to the units string</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a units string has not been set within the mnemonic</exception>
  ///
  /// <seealso cref="Mnemonic.UnitsAvailable"/>
  /// <seealso cref="Mnemonic.SetUnits"/>
  public string GetUnits() {
  }

  /// <summary>Sets the unit string withing the mnemonic.</summary>
  ///
  /// <param name="units">String containing units description</param>
  ///
  /// <seealso cref="Mnemonic.UnitsAvailable"/>
  /// <seealso cref="Mnemonic.GetUnits"/>
  public void SetUnits(string units) {
  }

  /// <summary>Returns the number of MnemonicSample objects stored within the mnemonic.</summary>
  public int GetSampleCount() {
  }

  /// <summary>Returns a reference to the MnemonicSample object stored within the %Mnemonic.</summary>
  ///
  /// <param name="index">The zero-based index of the MnemonicSample to access</param>
  ///
  /// <exception cref="GmsecException">Thrown if the index is out of bounds</exception>
  ///
  /// <seealso cref="Mnemonic.GetSampleCount"/>
  public MnemonicSample GetSample(int index) {
  }

  /// <summary>Adds the given MnemonicSample to the mnemonic</summary>
  ///
  /// <param name="sample">The MnemonicSample object</param>
  public void AddSample(MnemonicSample sample) {
  }

}

}
