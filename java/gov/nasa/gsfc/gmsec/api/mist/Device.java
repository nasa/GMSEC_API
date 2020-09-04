/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Device.java
 *
 *  @brief This file contains declarations for public interfaces to Device 
 *         containers.
**/

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.DeviceParam;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDevice;



/**
 * @class Device
 *
 * @brief This class is a lightweight container for holding information
 * on a Device, and used to generate GMSEC Device messages by the 
 * ConnectionManager class
 *
 * @sa DeviceMessage
 * @sa ConnectionManager
*/
public class Device
{
	private JNIDevice m_jniDevice = null;


	public static JNIDevice getInternal(Device obj)
	{
		return (obj == null ? null : obj.m_jniDevice);
	}


	public Device(JNIDevice jDevice)
	{
		m_jniDevice = jDevice;
	}


	protected Device()
	{
	}


	/**
	 * @enum DeviceStatus
	 *
	 * @desc Status enumeration for the Device object
	 */
	public enum DeviceStatus
	{
		DEBUG,           ///< DEBUG
		NOMINAL_GREEN,   ///< NOMINAL GREEN
		YELLOW,          ///< YELLOW
		ORANGE,          ///< ORANGE
		RED              ///< RED
	};


	/**
	 * @fn Device(String name, DeviceStatus status)
	 *
	 * @brief Constructor - Initializes the Device object with no DeviceParam information
	 *
	 * @param name - The name of the Device GMSEC connection object
	 * @param status - The status of the Device
	 *
	 * @throws An IllegalArgumentException is thrown if the device name is null, or contains an empty string.
	 */
	public Device(String name, DeviceStatus status) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Device name is null or contain an empty-string");
		}

		m_jniDevice = new JNIDevice(name, status);
	}


	/**
	 * @fn Device(String name, DeviceStatus status, java.util.List<DeviceParam> params)
	 *
	 * @brief Constructor - Initializes the Device object with a set of
	 * Device parameter information
	 *
	 * @param name - The name of the Device GMSEC connection object
	 * @param status - The status of the Device
	 * @param params - The list of Device parameters
	 *
	 * @throws An IllegalArgumentException is thrown if the device name is null, or contains an empty string.
	 * @throws An IllegalArgumentException is thrown if the DeviceParam list is null.
	 */
	public Device(String name, DeviceStatus status, java.util.List<DeviceParam> params) throws IllegalArgumentException
	{
		if (name == null || name.isEmpty())
		{
			throw new IllegalArgumentException("Device name is null or contain an empty-string");
		}
		if (params == null)
		{
			throw new IllegalArgumentException("DeviceParam list is null");
		}

		m_jniDevice = new JNIDevice(name, status, params);
	}


	/**
	 * @fn Device(Device other)
	 *
	 * @brief Copy-Constructor - Copies the information from the given Device object.
	 *
	 * @param other - The Device object to copy.
	 *
	 * @throws An IllegalArgumentException is thrown if the given Device object is null.
	 */
	public Device(Device other) throws IllegalArgumentException
	{
		if (other == null)
		{
			throw new IllegalArgumentException("Device is null");
		}

		m_jniDevice = new JNIDevice(Device.getInternal(other));
	}


	/**
	 * @fn String getName()
	 *
	 * @brief Retrieves the name of the device
	 *
	 * @return The name of the device
	 */
	public String getName()
	{
		return m_jniDevice.getName();
	}


	/**
	 * @fn boolean numberAvailable()
	 *
	 * @brief Returns the availability of a device number field.
	 *
	 * @return True if a device number field has been set; false otherwise
	 */
	public boolean numberAvailable()
	{
		return m_jniDevice.numberAvailable();
	}


	/**
	 * @fn Field getNumber()
	 *
	 * @brief Accessor for the device number field.
	 *
	 * @return A reference to the device number Field
	 *
	 * @throws A GMSEC_Exception is thrown if the device number Field has not been set
	 */
	public Field getNumber() throws GMSEC_Exception
	{
		return m_jniDevice.getNumber();
	}


	/**
	 * @fn void setNumber(Field number)
	 *
	 * @brief Assigns the given device number Field to this object.
	 *
	 * @param number - the device number Field.
	 *
	 * @throws An IllegalArgumentException is thrown if the given number Field object is null.
	 */
	public void setNumber(Field number) throws IllegalArgumentException
	{
		if (number == null)
		{
			throw new IllegalArgumentException("Device number field is null");
		}

		m_jniDevice.setNumber(number);
	}


	/**
	 * @fn boolean modelAvailable()
	 *
	 * @brief Returns the availability of a device model
	 *
	 * @return True if a device model has been set, false otherwise
	 */
	public boolean modelAvailable()
	{
		return m_jniDevice.modelAvailable();
	}


	/**
	 * @fn String getModel()
	 *
	 * @brief Accessor for the device model
	 *
	 * @return A reference to the device model string
	 *
	 * @throws A GMSEC_Exception is thrown if the model has not been assigned to the Device.
	 */
	public String getModel() throws GMSEC_Exception
	{
		return m_jniDevice.getModel();
	}


	/**
	 * @fn void setModel(String model)
	 *
	 * @brief Assigns the given string as the model for the Device.
	 *
	 * @throws An IllegalArgumentException is thrown if the model string is null, or if it contains an empty-string.
	 */
	public void setModel(String model) throws IllegalArgumentException
	{
		if (model == null || model.isEmpty())
		{
			throw new IllegalArgumentException("Device Model is null or contain an empty string");
		}

		m_jniDevice.setModel(model);
	}


	/**
	 * @fn boolean serialAvailable()
	 *
	 * @brief Returns the availability of a device serial number
	 *
	 * @return True if a device serial number has been set, false otherwise
	 */
	public boolean serialAvailable()
	{
		return m_jniDevice.serialAvailable();
	}


	/**
	 * @fn String getSerial()
	 *
	 * @brief Accessor for the device serial number
	 *
	 * @return A reference to the device serial number string
	 *
	 * @throws A GMSEC_Exception is thrown if the serial number string has not been set with this object.
	 */
	public String getSerial() throws GMSEC_Exception
	{
		return m_jniDevice.getSerial();
	}


	/**
	 * @fn void setSerial(String serial)
	 *
	 * @brief Assigns the given string as the serial number for the Device.
	 *
	 * @throws An IllegalArgumentException is thrown if the serial string is null, or if it contains an empty-string.
	 */
	public void setSerial(String serial) throws IllegalArgumentException
	{
		if (serial == null || serial.isEmpty())
		{
			throw new IllegalArgumentException("Device Serial is null or contain an empty string");
		}

		m_jniDevice.setSerial(serial);
	}


	/**
	 * @fn boolean versionAvailable()
	 *
	 * @brief Returns the availability of a device version
	 *
	 * @return True if a device version has been set, false otherwise
	 */
	public boolean versionAvailable()
	{
		return m_jniDevice.versionAvailable();
	}


	/**
	 * @fn String getVersion()
	 *
	 * @brief Accessor for the device version
	 *
	 * @return The device version string
	 *
	 * @throws A GMSEC_Exception is thrown if the version string has not been assigned to the Device.
	 */
	public String getVersion() throws GMSEC_Exception
	{
		return m_jniDevice.getVersion();
	}


	/**
	 * @fn void setVersion(String version)
	 *
	 * @brief Supplies a Version for the Device and makes a copy of the string
	 *
	 * @throws An IllegalArgumentException is thrown if the version string is null, or if it contains an empty-string.
	 */
	public void setVersion(String version) throws IllegalArgumentException
	{
		if (version == null || version.isEmpty())
		{
			throw new IllegalArgumentException("Device Version is null or contain an empty string");
		}

		m_jniDevice.setVersion(version);
	}


	/**
	 * @fn boolean groupAvailable()
	 *
	 * @brief Returns the availability of a Device Group
	 *
	 * @return True if a Device Group has been set, false otherwise
	 */
	public boolean groupAvailable()
	{
		return m_jniDevice.groupAvailable();
	}


	/**
	 * @fn String getGroup()
	 *
	 * @brief Accessor for the Device Group
	 *
	 * @return A reference to the Device Group string
	 *
	 * @throws A GMSEC_Exception is thrown if the Group field has not been set
	 */
	public String getGroup() throws GMSEC_Exception
	{
		return m_jniDevice.getGroup();
	}


	/**
	 * @fn void setGroup(String group)
	 *
	 * @brief Supplies a Group for the Device and makes a copy of the string
	 *
	 * @throws An IllegalArgumentException is thrown if the group string is null, or if it contains an empty-string.
	 */
	public void setGroup(String group) throws IllegalArgumentException
	{
		if (group == null || group.isEmpty())
		{
			throw new IllegalArgumentException("Device Group is null or contain an empty string");
		}

		m_jniDevice.setGroup(group);
	}


	/**
	 * @fn boolean roleAvailable()
	 *
	 * @brief Returns the availability of a Device Role
	 *
	 * @return True if a Device Role has been set, false otherwise
	 */
	public boolean roleAvailable()
	{
		return m_jniDevice.roleAvailable();
	}


	/**
	 * @fn String getRole()
	 *
	 * @brief Accessor for the Device Role
	 *
	 * @return A reference to the Device Role string
	 *
	 * @throws A GMSEC_Exception is thrown if the Role field has not been set
	 */
	public String getRole() throws GMSEC_Exception
	{
		return m_jniDevice.getRole();
	}


	/**
	 * @fn void setRole(String role)
	 *
	 * @brief Supplies a Role for the Device and makes a copy of the string
	 *
	 * @throws An IllegalArgumentException is thrown if the role string is null, or if it contains an empty-string.
	 */
	public void setRole(String role) throws IllegalArgumentException
	{
		if (role == null || role.isEmpty())
		{
			throw new IllegalArgumentException("Device Role is null or contain an empty string");
		}

		m_jniDevice.setRole(role);
	}


	/**
	 * @fn DeviceStatus getStatus()
	 *
	 * @brief Accessor for the Device Status
	 *
	 * @return The Device Status enumeration
	 */
	public DeviceStatus getStatus()
	{
		return m_jniDevice.getStatus();
	}


	/**
	 * @fn void setStatus(DeviceStatus status)
	 *
	 * @brief Supplies a Status for the Device and makes a copy of the string
	 */
	public void setStatus(DeviceStatus status)
	{
		m_jniDevice.setStatus(status);
	}


	/**
	 * @fn boolean infoAvailable()
	 *
	 * @brief Returns the availability of a Device Info field
	 *
	 * @return True if a Device Info field has been set, false otherwise
	 */
	public boolean infoAvailable()
	{
		return m_jniDevice.infoAvailable();
	}


	/**
	 * @fn Field getInfo()
	 *
	 * @brief Accessor for the Device Info
	 *
	 * @return A reference to the Device Info field
	 *
	 * @throws A GMSEC_Exception is thrown if the Info field has not been set
	 */
	public Field getInfo() throws GMSEC_Exception
	{
		return m_jniDevice.getInfo();
	}


	/**
	 * @fn void setInfo(Field info)
	 *
	 * @brief Supplies a Info for the Device and makes a copy of the field
	 *
	 * @throws An IllegalArgumentException is thrown if the info Field is null.
	 */
	public void setInfo(Field info) throws IllegalArgumentException
	{
		if (info == null)
		{
			throw new IllegalArgumentException("Device Info Field is null");
		}

		m_jniDevice.setInfo(info);
	}


	/**
	 * @fn long getParamCount()
	 *
	 * @brief Returns the number of parameters for this Device
	 *
	 * @return the number of parameters
	 */
	public long getParamCount()
	{
		return m_jniDevice.getParamCount();
	}


	/**
	 * @fn DeviceParam getParam(long index)
	 *
	 * @brief Returns a reference to a DeviceParam held in this object
	 *
	 * @param index - the zero-based index of the DeviceParam object to access
	 *
	 * @return A reference to the requested DeviceParam object
	 *
	 * @throws A GMSEC_Exception if the index supplied is out of bounds
	 */
	public DeviceParam getParam(long index) throws GMSEC_Exception
	{
		return m_jniDevice.getParam(index);
	}


	/**
	 * @fn void addParam(DeviceParam param)
	 *
	 * @brief Adds a single DeviceParam to the current list
	 *
	 * @param param - the new DeviceParam object
	 *
	 * @throws An IllegalArgumentException is thrown if the DeviceParam object is null.
	 */
	public void addParam(DeviceParam param) throws IllegalArgumentException
	{
		if (param == null)
		{
			throw new IllegalArgumentException("DeviceParam is null");
		}

		m_jniDevice.addParam(param);
	}
}
