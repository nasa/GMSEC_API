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
/// This class is a lightweight container for holding information on a %MnemonicSample,
/// and is used to generate a Mnemonic.
/// </summary>

public class MnemonicSample : global::System.IDisposable {

  /// <summary>Creates a %MnemonicSample instance with a timestamp and a raw value.</summary>
  ///
  /// <param name="timestamp">The time at which the sampled value was taken</param>
  /// <param name="rawValue">The Field containing the sampled raw value</param>
  ///
  /// <exception cref="GmsecException">Thrown if the timestamp is null or contains an empty-string</exception>
  public MnemonicSample(string timestamp, Field rawValue) {
  }

  /// <summary>Copy-constructor that can be used to create a copy of a %MnemonicSample instance.</summary>
  ///
  /// <param name="other">The other %MnemonicSample to copy</param>
  public MnemonicSample(MnemonicSample other) {
  }

  /// <summary>Returns the timestamp associated with the sample.</summary>
  public string GetTimestamp() {
  }

  /// <summary>Returns the field containing the raw value which is associated with the sample.</summary>
  public Field GetRawValue() {
  }

  /// <summary>Returns whether an EU value has been set within the sample.</summary>
  ///
  /// <seealso cref="MnemonicSample.GetEUValue"/>
  /// <seealso cref="MnemonicSample.SetEUValue"/>
  public bool HasEUValue() {
  }

  /// <summary>Returns the field containing the EU value associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if an EU value has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasEUValue"/>
  /// <seealso cref="MnemonicSample.SetEUValue"/>
  public Field GetEUValue() {
  }

  /// <summary>Allows for the setting of the EU value within the sample.</summary>
  ///
  /// <param name="value">The field containing the EU value</param>
  ///
  /// <seealso cref="MnemonicSample.HasEUValue"/>
  /// <seealso cref="MnemonicSample.GetEUValue"/>
  public void SetEUValue(Field value) {
  }

  /// <summary>Returns whether a text value has been set within the sample.</summary>
  ///
  /// <seealso cref="MnemonicSample.GetTextValue"/>
  /// <seealso cref="MnemonicSample.SetTextValue"/>
  public bool HasTextValue() {
  }

  /// <summary>Returns the text value associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a text value has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasTextValue"/>
  /// <seealso cref="MnemonicSample.SetTextValue"/>
  public string GetTextValue() {
  }

  /// <summary>Allows for the setting of the text value within the sample.</summary>
  ///
  /// <param name="value">The string containing the text value</param>
  ///
  /// <seealso cref="MnemonicSample.HasTextValue"/>
  /// <seealso cref="MnemonicSample.GetTextValue"/>
  public void SetTextValue(string value) {
  }

  /// <summary>Returns whether a flags value has been set within the sample.</summary>
  ///
  /// <seealso cref="MnemonicSample.GetFlags"/>
  /// <seealso cref="MnemonicSample.SetFlags"/>
  public bool HasFlags() {
  }

  /// <summary>Returns the flags value associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a flags value has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasFlags"/>
  /// <seealso cref="MnemonicSample.SetFlags"/>
  public int GetFlags() {
  }

  /// <summary>Allows for the setting of the flags value within the sample.</summary>
  ///
  /// <param name="flags">The 32-bit flags value</param>
  ///
  /// <seealso cref="MnemonicSample.HasFlags"/>
  /// <seealso cref="MnemonicSample.GetFlags"/>
  public void SetFlags(int flags) {
  }

  /// <summary>Returns whether a limit enable/disable value has been set within the sample.</summary>
  public bool HasLimitEnableDisable() {
  }

  /// <summary>Returns the limit enable/disable boolean value associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a limit enable/disablue value has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasLimitEnableDisable"/>
  /// <seealso cref="MnemonicSample.SetLimitEnableDisable"/>
  public bool GetLimitEnableDisable() {
  }

  /// <summary>Allows for the setting of the Limit enable/disable value within the sample.</summary>
  ///
  /// <param name="value">Boolean value to indicate whether the limit is enabled (true) or disabled (false)</param>
  public void SetLimitEnableDisable(bool value) {
  }

  /// <summary>Returns whether a LimitFlag value has been set within the sample.</summary>
  public bool HasLimit() {
  }

  /// <summary>Returns the LimitFlag associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a LimitFlag has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasLimit"/>
  /// <seealso cref="MnemonicSample.SetLimit"/>
  public MnemonicSample.LimitFlag GetLimit() {
  }

  /// <summary>Allows for the setting of the Limit flag within the sample.</summary>
  ///
  /// <param name="limit">Enumerated LimitFlag value</param>
  public void SetLimit(MnemonicSample.LimitFlag limit) {
  }

  /// <summary>Returns whether a Staleness Status value has been set within the sample.</summary>
  public bool HasStalenessStatus() {
  }

  /// <summary>Returns the Staleness Status boolean value associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a Staleness Status value has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasStalenessStatus"/>
  /// <seealso cref="MnemonicSample.SetStalenessStatus"/>
  public bool GetStalenessStatus() {
  }

  /// <summary>Allows for the setting of the Staleness Status boolean value within the sample.</summary>
  ///
  /// <param name="staleness">Staleness boolean value</param>
  public void SetStalenessStatus(bool staleness) {
  }

  /// <summary>Returns whether a Quality value has been set within the sample.</summary>
  public bool HasQuality() {
  }

  /// <summary>Returns the Quality boolean value associated with the sample.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a Quality value has not been associated with the sample</exception>
  ///
  /// <seealso cref="MnemonicSample.HasQuality"/>
  /// <seealso cref="MnemonicSample.SetQuality"/>
  public bool GetQuality() {
  }

  /// <summary>Allows for the setting of the Quality boolean value within the sample.</summary>
  ///
  /// <param name="quality">Quality boolean value</param>
  public void SetQuality(bool quality) {
  }

   /// <summary>Enumeration of Limit Flags that can be assigned to the MnemonicSample.</summary>
  public enum LimitFlag {
    /// <summary>No limit flag specified</summary>
    UNSPECIFIED = 0,

    /// <summary>No limit violation</summary>
    NO_LIMIT_VIOLATION = 1,

    /// <summary>Red-low limit violation</summary>
    RED_LOW = 2,

    /// <summary>Yellow-low limit violation</summary>
    YELLOW_LOW = 3,

    /// <summary>Yellow-high limit violation</summary>
    YELLOW_HIGH = 4,

    /// <summary>Red-high limit violation</summary>
    RED_HIGH = 5
  }

}

}
