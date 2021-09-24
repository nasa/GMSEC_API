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
/// This class is a lightweight container for holding information on a %Device, and is
/// used to generate a %GMSEC DeviceMessage.
/// </summary>

public class Device : global::System.IDisposable {

  /// <summary>Simple constructor</summary>
  ///
  /// <param name="name">%Device name</param>
  /// <param name="status">%Device status</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  public Device(string name, Device.DeviceStatus status) {
  }

  /// <summary>Constructor that accepts list of %Device Parameters</summary>
  ///
  /// <param name="name">Device name</param>
  /// <param name="status">Device status</param>
  /// <param name="params">List of device parameters</param>
  ///
  /// <exception cref="GmsecException">Thrown if the name is null or contains an empty-string</exception>
  public Device(string name, Device.DeviceStatus status, DeviceParamList params) {
  }

  /// <summary>Copy-constructor for making deep-copy of other %Device object.</summary>
  ///
  /// <param name="other">The other %Device object to copy</param>
  public Device(Device other) {
  }

  /// <summary>Returns the name of the %Device</summary>
  public string GetName() {
  }

  /// <summary>Returns the availability of a %Device number.</summary>
  ///
  /// <seealso cref="Device.GetNumber"/>
  /// <seealso cref="Device.SetNumber"/>
  public bool NumberAvailable() {
  }

  /// <summary>Returns the %Device number.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device number is not available</exception>
  ///
  /// <seealso cref="Device.NumberAvailable"/>
  public Field GetNumber() {
  }

  /// <summary>Assigns a number to the %Device.</summary>
  ///
  /// <param name="number">The number to assign to the %Device</param>
  ///
  /// <seealso cref="Device.NumberAvailable"/>
  /// <seealso cref="Device.GetNumber"/>
  public void SetNumber(Field number) {
  }

  /// <summary>Returns the availability of a %Device model number.</summary>
  ///
  /// <seealso cref="Device.GetModel"/>
  /// <seealso cref="Device.SetModel"/>
  public bool ModelAvailable() {
  }

  /// <summary>Returns the %Device model number.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device model number is not available</exception>
  ///
  /// <seealso cref="Device.ModelAvailable"/>
  /// <seealso cref="Device.SetModel"/>
  public string GetModel() {
  }

  /// <summary>Assigns a model number to the %Device.</summary>
  ///
  /// <param name="model">The model number to assign to the %Device</param>
  ///
  /// <exception cref="GmsecException">Thrown if the model number is null or contains an empty-string</exception>
  ///
  /// <seealso cref="Device.ModelAvailable"/>
  /// <seealso cref="Device.GetModel"/>
  public void SetModel(string model) {
  }

  /// <summary>Returns the availability of a %Device serial number.</summary>
  ///
  /// <seealso cref="Device.GetSerial"/>
  /// <seealso cref="Device.SetSerial"/>
  public bool SerialAvailable() {
  }

  /// <summary>Returns the %Device serial number.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device serial number is not available</exception>
  ///
  /// <seealso cref="Device.SerialAvailable"/>
  /// <seealso cref="Device.SetSerial"/>
  public string GetSerial() {
  }

  /// <summary>Assigns a serial number to the %Device.</summary>
  ///
  /// <param name="serial">The serial number to assign to the %Device</param>
  ///
  /// <exception cref="GmsecException">Thrown if the serial number is null or contains an empty-string</exception>
  ///
  /// <seealso cref="Device.SerialAvailable"/>
  /// <seealso cref="Device.GetSerial"/>
  public void SetSerial(string serial) {
  }

  /// <summary>Returns the availability of a %Device version string.</summary>
  ///
  /// <seealso cref="Device.GetVersion"/>
  /// <seealso cref="Device.SetVersion"/>
  public bool VersionAvailable() {
  }

  /// <summary>Returns the %Device version string.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device version string is not available</exception>
  ///
  /// <seealso cref="Device.VersionAvailable"/>
  /// <seealso cref="Device.SetVersion"/>
  public string GetVersion() {
  }

  /// <summary>Assigns a version string to the %Device.</summary>
  ///
  /// <param name="version">The version string to assign to the %Device</param>
  ///
  /// <exception cref="GmsecException">Thrown if the version number is null or contains an empty-string</exception>
  ///
  /// <seealso cref="Device.VersionAvailable"/>
  /// <seealso cref="Device.GetVersion"/>
  public void SetVersion(string version) {
  }

  /// <summary>Returns the availability of a %Device group.</summary>
  ///
  /// <seealso cref="Device.GetGroup"/>
  /// <seealso cref="Device.SetGroup"/>
  public bool GroupAvailable() {
  }

  /// <summary>Returns the %Device group.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device group is not available</exception>
  ///
  /// <seealso cref="Device.GroupAvailable"/>
  /// <seealso cref="Device.SetGroup"/>
  public string GetGroup() {
  }

  /// <summary>Assigns a group string to the %Device.</summary>
  ///
  /// <param name="group">The group string to assign to the %Device</param>
  ///
  /// <exception cref="GmsecException">Thrown if the group string is null or contains an empty-string</exception>
  ///
  /// <seealso cref="Device.GroupAvailable"/>
  /// <seealso cref="Device.GetGroup"/>
  public void SetGroup(string group) {
  }

  /// <summary>Returns the availability of a %Device role.</summary>
  ///
  /// <seealso cref="Device.GetRole"/>
  /// <seealso cref="Device.SetRole"/>
  public bool RoleAvailable() {
  }

  /// <summary>Returns the %Device role.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device role is not available</exception>
  ///
  /// <seealso cref="Device.RoleAvailable"/>
  /// <seealso cref="Device.SetRole"/>
  public string GetRole() {
  }

  /// <summary>Assigns a role string to the %Device.</summary>
  ///
  /// <param name="role">The role string to assign to the %Device</param>
  ///
  /// <exception cref="GmsecException">Thrown if the role string is null or contains an empty-string</exception>
  ///
  /// <seealso cref="Device.RoleAvailable"/>
  /// <seealso cref="Device.GetRole"/>
  public void SetRole(string role) {
  }

  /// <summary>Returns the status of the %Device</summary>
  public Device.DeviceStatus GetStatus() {
  }

  /// <summary>Allows for the setting of the status of the %Device</summary>
  ///
  /// <param name="status">The status of the %Device</param>
  public void SetStatus(Device.DeviceStatus status) {
  }

  /// <summary>Returns the availability of a %Device information field.</summary>
  ///
  /// <seealso cref="Device.GetInfo"/>
  /// <seealso cref="Device.SetInfo"/>
  public bool InfoAvailable() {
  }

  /// <summary>Returns the %Device information field.</summary>
  ///
  /// <exception cref="GmsecException">Thrown if a %Device information field is not available</exception>
  ///
  /// <seealso cref="Device.InfoAvailable"/>
  /// <seealso cref="Device.SetInfo"/>
  public Field GetInfo() {
  }

  /// <summary>Assigns an information field to the %Device.</summary>
  ///
  /// <param name="info">The information field to assign to the %Device</param>
  ///
  /// <seealso cref="Device.InfoAvailable"/>
  /// <seealso cref="Device.GetInfo"/>
  public void SetInfo(Field info) {
  }

  /// <summary>Returns the number of DeviceParam objects assigned to the %Device.</summary>
  public int GetParamCount() {
  }

  /// <summary>Returns a reference to the DeviceParam object held in the %Device object.</summary>
  ///
  /// <param name="index">The zero-based index of the DeviceParam object to access</param>
  ///
  /// <seealso cref="Device.GetParamCount"/>
  public DeviceParam GetParam(int index) {
  }

  /// <summary>Assign a DeviceParam object to the list held by the %Device.</summary>
  ///
  /// <param name="param">The DeviceParam object to add to the %Device</param>
  public void AddParam(DeviceParam param) {
  }

  /// <summary>Enumeration of the possible %Device status</summary>
  public enum DeviceStatus {
    /// <summary>Debug status</summary>
    DEBUG = 0,

    /// <summary>Nominal status</summary>
    NOMINAL_GREEN = 1,

    /// <summary>Yellow status</summary>
    YELLOW = 2,

    /// <summary>Orange status</summary>
    ORANGE = 3,

    /// <summary>Red status</summary>
    RED = 4
  }

}

}
