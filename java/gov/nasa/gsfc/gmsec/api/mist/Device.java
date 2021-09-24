/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Device.java
 */

package gov.nasa.gsfc.gmsec.api.mist;

import gov.nasa.gsfc.gmsec.api.StatusClassification;
import gov.nasa.gsfc.gmsec.api.StatusCode;
import gov.nasa.gsfc.gmsec.api.GMSEC_Exception;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.DeviceParam;

import gov.nasa.gsfc.gmsec.api.jni.mist.JNIDevice;



/**
 * This class is a lightweight container for holding information
 * on a Device, and used to generate GMSEC Device messages by the 
 * ConnectionManager class.
*/
public class Device
{
	private JNIDevice m_jniDevice = null;


	/** 
	 * This method is for internal GMSEC API use only.
	 * @param obj Object to reference for acquiring internal JNIDevice.
	 * @return Internal JNIDevice object.
	 */
	public static JNIDevice getInternal(Device obj)
	{
		return (obj == null ? null : obj.m_jniDevice);
	}


	/**
	 * This constructor is for internal GMSEC API use only.
	 * @param jDevice Internal JNIDevice object.
	 */
	public Device(JNIDevice jDevice)
	{
		m_jniDevice = jDevice;
	}


	protected Device()
	{
	}


	/**
	 * Status enumeration of the Device object.
	 */
	public enum DeviceStatus
	{
		/** Debug status. */
		DEBUG,

		/** Nominal status. */
		NOMINAL_GREEN,

		/** Yellow status. */
		YELLOW,

		/** Orange status. */
		ORANGE,

		/** Red status. */
		RED
	};


	/**
	 * Constructor - Initializes the Device object with no DeviceParam information.
	 *
	 * @param name The name of the device GMSEC connection object.
	 * @param status The status of the device.
	 *
	 * @throws IllegalArgumentException Thrown if the device name is null, or contains an empty string.
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
	 * Constructor - Initializes the device object with a set of device parameter information.
	 *
	 * @param name The name of the device GMSEC connection object.
	 * @param status The status of the device.
	 * @param params The list of device parameters.
	 *
	 * @throws IllegalArgumentException Thrown if the device name is null, or contains an empty string.
	 * @throws IllegalArgumentException Thrown if the DeviceParam list is null.
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
	 * Copy-Constructor - Copies the information from the given Device object.
	 *
	 * @param other The Device object to copy.
	 *
	 * @throws IllegalArgumentException Thrown if the given Device object is null.
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
	 * Retrieves the name of the device.
	 *
	 * @return The name of the device.
	 */
	public String getName()
	{
		return m_jniDevice.getName();
	}


	/**
	 * Returns the availability of a device number field.
	 *
	 * @return True if a device number field has been set; false otherwise.
	 */
	public boolean numberAvailable()
	{
		return m_jniDevice.numberAvailable();
	}


	/**
	 * Accessor to get the device number field.
	 *
	 * @return A reference to the device number Field.
	 *
	 * @throws GMSEC_Exception Thrown if the device number Field has not been set.
	 */
	public Field getNumber() throws GMSEC_Exception
	{
		return m_jniDevice.getNumber();
	}


	/**
	 * Assigns the given device number Field to this object.
	 *
	 * @param number The device number Field.
	 *
	 * @throws IllegalArgumentException Thrown if the given number Field object is null.
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
	 * Returns the availability of a device model.
	 *
	 * @return True if a device model has been set, false otherwise.
	 */
	public boolean modelAvailable()
	{
		return m_jniDevice.modelAvailable();
	}


	/**
	 * Accessor to get the device model.
	 *
	 * @return A reference to the device model string.
	 *
	 * @throws GMSEC_Exception Thrown if the model has not been assigned to the device.
	 */
	public String getModel() throws GMSEC_Exception
	{
		return m_jniDevice.getModel();
	}


	/**
	 * Assigns the given string as the model number of the device.
	 *
	 * @param model The model number (as a string).
	 *
	 * @throws IllegalArgumentException Thrown if the model number string is null, or if it contains an empty-string.
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
	 * Returns the availability of a device serial number.
	 *
	 * @return True if a device serial number has been set, false otherwise.
	 */
	public boolean serialAvailable()
	{
		return m_jniDevice.serialAvailable();
	}


	/**
	 * Accessor to get the device serial number.
	 *
	 * @return The device serial number string.
	 *
	 * @throws GMSEC_Exception Thrown if the serial number string has not been set with this object.
	 */
	public String getSerial() throws GMSEC_Exception
	{
		return m_jniDevice.getSerial();
	}


	/**
	 * Assigns the given string as the serial number of the device.
	 *
	 * @param serial The serial number (as a string).
	 *
	 * @throws IllegalArgumentException Thrown if the serial string is null, or if it contains an empty-string.
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
	 * Returns the availability of a device version.
	 *
	 * @return True if a device version has been set, false otherwise.
	 */
	public boolean versionAvailable()
	{
		return m_jniDevice.versionAvailable();
	}


	/**
	 * Accessor to get the device version.
	 *
	 * @return The device version string.
	 *
	 * @throws GMSEC_Exception Thrown if the version string has not been assigned to the device.
	 */
	public String getVersion() throws GMSEC_Exception
	{
		return m_jniDevice.getVersion();
	}


	/**
	 * Assigns the given version number to the device.
	 *
	 * @param version The version number (as a string).
	 *
	 * @throws IllegalArgumentException Thrown if the version string is null, or if it contains an empty-string.
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
	 * Returns the availability of a device group.
	 *
	 * @return True if a device group has been set, false otherwise.
	 */
	public boolean groupAvailable()
	{
		return m_jniDevice.groupAvailable();
	}


	/**
	 * Accessor to get the device group.
	 *
	 * @return A reference to the device group string.
	 *
	 * @throws GMSEC_Exception Thrown if the group field has not been set.
	 */
	public String getGroup() throws GMSEC_Exception
	{
		return m_jniDevice.getGroup();
	}


	/**
	 * Assigns the given group to the device.
	 *
	 * @param group The name of the group.
	 *
	 * @throws IllegalArgumentException Thrown if the group string is null, or if it contains an empty-string.
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
	 * Returns the availability of a device role.
	 *
	 * @return True if a device role has been set, false otherwise.
	 */
	public boolean roleAvailable()
	{
		return m_jniDevice.roleAvailable();
	}


	/**
	 * Accessor to get the device role.
	 *
	 * @return A reference to the device role string.
	 *
	 * @throws GMSEC_Exception Thrown if the role field has not been set.
	 */
	public String getRole() throws GMSEC_Exception
	{
		return m_jniDevice.getRole();
	}


	/**
	 * Assigns the given role to the device.
	 *
	 * @param role The role the device supports.
	 *
	 * @throws IllegalArgumentException Thrown if the role string is null, or if it contains an empty-string.
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
	 * Accessor to get the device status.
	 *
	 * @return The device status.
	 */
	public DeviceStatus getStatus()
	{
		return m_jniDevice.getStatus();
	}


	/**
	 * Assigns the given status to the device.
	 *
	 * @param status The device status.
	 */
	public void setStatus(DeviceStatus status)
	{
		m_jniDevice.setStatus(status);
	}


	/**
	 * Returns the availability of a device info field.
	 *
	 * @return True if a device info field has been set, false otherwise.
	 */
	public boolean infoAvailable()
	{
		return m_jniDevice.infoAvailable();
	}


	/**
	 * Accessor for getting the device info Field.
	 *
	 * @return A reference to the device info field.
	 *
	 * @throws GMSEC_Exception Thrown if the info field has not been set.
	 */
	public Field getInfo() throws GMSEC_Exception
	{
		return m_jniDevice.getInfo();
	}


	/**
	 * Assigns the given info Field to the device.
	 *
	 * @param info The Field containing information about the device.
	 *
	 * @throws IllegalArgumentException Thrown if the info Field is null.
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
	 * Returns the number of parameters for this device.
	 *
	 * @return The number of parameters.
	 */
	public long getParamCount()
	{
		return m_jniDevice.getParamCount();
	}


	/**
	 * Returns a reference to a DeviceParam held in this object.
	 *
	 * @param index The zero-based index of the DeviceParam object to access.
	 *
	 * @return A reference to the requested DeviceParam object.
	 *
	 * @throws GMSEC_Exception Thrown if the index supplied is out of bounds.
	 */
	public DeviceParam getParam(long index) throws GMSEC_Exception
	{
		return m_jniDevice.getParam(index);
	}


	/**
	 * Adds the given DeviceParam to the device.
	 *
	 * @param param The new DeviceParam object.
	 *
	 * @throws IllegalArgumentException Thrown if the DeviceParam object is null.
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
