/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Device.h
 *
 *  @brief This file contains declarations for public interfaces to Device 
 *         containers.
**/

#ifndef GMSEC_API_DEVICE_H
#define GMSEC_API_DEVICE_H

#include <gmsec4/mist/DeviceParam.h>
#include <gmsec4/util/DataList.h>
#include <gmsec4_defs.h>

namespace gmsec
{
namespace api
{
namespace mist
{

namespace internal
{
class InternalDevice;
}

/**
 * @class Device
 *
 * @brief This class is a lightweight container for holding information
 * on a Device, and used to generate GMSEC Device messages by the 
 * ConnectionManager class
 *
 * @sa ConnectionManager @n
*/
class GMSEC_API Device
{
  public:

	/**
	 * @desc Status enumeration for the Device object
	 */
	enum DeviceStatus
	{
		DEBUG = 0,
		NOMINAL_GREEN = 1,
		YELLOW = 2,
		ORANGE = 3,
		RED = 4
	};


	/**
	 * @fn Device(const char* name, DeviceStatus status)
	 *
	 * @brief Constructor - Initializes the Device object with no
	 * Device parameter information
	 *
	 * @param name - The name of the Device
	 * GMSEC connection object
	 * @param status - The status of the Device
	 * @throw An exception is thrown if the name parameter is NULL or empty
	 */
	Device(const char* name, DeviceStatus status);


	/**
	 * @fn Device(const char* name, DeviceStatus status, const gmsec::api::util::DataList<DeviceParam>& params)
	 *
	 * @brief Constructor - Initializes the Device object with a set of
	 * Device parameter information
	 *
	 * @param name - The name of the Device
	 * GMSEC connection object
	 * @param status - The status of the Device
	 * @param params - The list of Device parameters
	 * @throw An exception is thrown if the name parameter is NULL or empty
	 */
	Device(const char* name, DeviceStatus status, const gmsec::api::util::DataList<DeviceParam*>& params);


	/**
	 * @fn Device(const Device& other)
	 *
	 * @brief Constructor - Initializes the Device object with the information
	 * from another device
	 *
	 * @param other - The other device
	 */
	Device(const Device& other);

	/**
	 * @fn Device& operator=(const Device& other)
	 *
	 * @brief Assignment operator - Initializes the Device object with the information
	 * from another device
	 *
	 * @param other - The other device
	 */
	Device& operator=(const Device& other);

	/**
	 * @fn ~Device()
	 *
	 * @brief Destructor
	 *
	 */
	~Device();


	/**
	 * @fn const char* getName() const
	 *
	 * @brief Retrieves the name of the device
	 *
	 * @return The name of the device
	 */
	const char* CALL_TYPE getName() const;


	/**
	 * @fn bool numberAvailable() const
	 *
	 * @brief Returns the availability of a Device Number
	 *
	 * @return True if a Device Number has been set, false otherwise
	 */
	bool CALL_TYPE numberAvailable() const;


	/**
	 * @fn const Field& getNumber() const
	 *
	 * @brief Accessor for the Device Number
	 *
	 * @return A reference to the Device Number field
	 *
	 * @throw An exception is thrown if the Number field has not been set
	 */
	const Field& CALL_TYPE getNumber() const;


	/**
	 * @fn void setNumber(const Field& number)
	 *
	 * @brief Supplies a Number for the Device and makes a copy of the field
	 *
	 */
	void CALL_TYPE setNumber(const Field& number);


	/**
	 * @fn bool modelAvailable() const
	 *
	 * @brief Returns the availability of a Device Model
	 *
	 * @return True if a Device Model has been set, false otherwise
	 */
	bool CALL_TYPE modelAvailable() const;


	/**
	 * @fn const char* getNumber() const
	 *
	 * @brief Accessor for the Device Model
	 *
	 * @return A pointer to the Device model string
	 *
	 * @throw An exception is thrown if the Model field has not been set
	 */
	const char* CALL_TYPE getModel() const;


	/**
	 * @fn void setModel(const char* model)
	 *
	 * @brief Supplies a Model for the Device and makes a copy of the string
	 *
	 */
	void CALL_TYPE setModel(const char* model);


	/**
	 * @fn bool serialAvailable() const
	 *
	 * @brief Returns the availability of a Device Serial Number
	 *
	 * @return True if a Device Serial Number has been set, false otherwise
	 */
	bool CALL_TYPE serialAvailable() const;


	/**
	 * @fn const char* getSerial() const
	 *
	 * @brief Accessor for the Device Serial Number
	 *
	 * @return A pointer to the Device Serial Number string
	 *
	 * @throw An exception is thrown if the Serial Number field has not been set
	 */
	const char* CALL_TYPE getSerial() const;


	/**
	 * @fn void setSerial(const char* serial)
	 *
	 * @brief Supplies a Serial Number for the Device and makes a copy of the string
	 *
	 */
	void CALL_TYPE setSerial(const char* serial);


	/**
	 * @fn bool versionAvailable() const
	 *
	 * @brief Returns the availability of a Device Version
	 *
	 * @return True if a Device Version has been set, false otherwise
	 */
	bool CALL_TYPE versionAvailable() const;


	/**
	 * @fn const char* getVersion() const
	 *
	 * @brief Accessor for the Device Version
	 *
	 * @return A pointer to the Device Version string
	 *
	 * @throw An exception is thrown if the Version field has not been set
	 */
	const char* CALL_TYPE getVersion() const;


	/**
	 * @fn void setVersion(const char* version)
	 *
	 * @brief Supplies a Version for the Device and makes a copy of the string
	 *
	 */
	void CALL_TYPE setVersion(const char* version);


	/**
	 * @fn bool groupAvailable() const
	 *
	 * @brief Returns the availability of a Device Group
	 *
	 * @return True if a Device Group has been set, false otherwise
	 */
	bool CALL_TYPE groupAvailable() const;


	/**
	 * @fn const char* getGroup() const
	 *
	 * @brief Accessor for the Device Group
	 *
	 * @return A pointer to the Device Group string
	 *
	 * @throw An exception is thrown if the Group field has not been set
	 */
	const char* CALL_TYPE getGroup() const;


	/**
	 * @fn void setGroup(const char* group)
	 *
	 * @brief Supplies a Group for the Device and makes a copy of the string
	 *
	 */
	void CALL_TYPE setGroup(const char* group);


	/**
	 * @fn bool roleAvailable() const
	 *
	 * @brief Returns the availability of a Device Role
	 *
	 * @return True if a Device Role has been set, false otherwise
	 */
	bool CALL_TYPE roleAvailable() const;


	/**
	 * @fn const char* getRole() const
	 *
	 * @brief Accessor for the Device Role
	 *
	 * @return A pointer to the Device Role string
	 *
	 * @throw An exception is thrown if the Role field has not been set
	 */
	const char* CALL_TYPE getRole() const;


	/**
	 * @fn void setRole(const char* role)
	 *
	 * @brief Supplies a Role for the Device and makes a copy of the string
	 *
	 */
	void CALL_TYPE setRole(const char* role);


	/**
	 * @fn DeviceStatus getStatus() const
	 *
	 * @brief Accessor for the Device Status
	 *
	 * @return The Device Status enumeration
	 *
	 */
	DeviceStatus CALL_TYPE getStatus() const;


	/**
	 * @fn void setStatus(DeviceStatus status)
	 *
	 * @brief Supplies a Status for the Device and makes a copy of the string
	 *
	 */
	void CALL_TYPE setStatus(DeviceStatus status);


	/**
	 * @fn bool infoAvailable() const
	 *
	 * @brief Returns the availability of a Device Info field
	 *
	 * @return True if a Device Info field has been set, false otherwise
	 */
	bool CALL_TYPE infoAvailable() const;


	/**
	 * @fn const Field& getInfo() const
	 *
	 * @brief Accessor for the Device Info
	 *
	 * @return A reference to the Device Info field
	 *
	 * @throw An exception is thrown if the Info field has not been set
	 */
	const Field& CALL_TYPE getInfo() const;


	/**
	 * @fn void setInfo(const Field& info)
	 *
	 * @brief Supplies a Info for the Device and makes a copy of the field
	 *
	 */
	void CALL_TYPE setInfo(const Field& info);


	/**
	 * @fn size_t getParamCount() const
	 *
	 * @brief Returns the number of parameters for this Device
	 *
	 * @return a size_t representation of the number of parameters
	 */
	size_t CALL_TYPE getParamCount() const; 


	/**
	 * @fn const DeviceParam& getParam(size_t idx) const
	 *
	 * @brief Returns a reference to a DeviceParam held in this object
	 *
	 * @param idx - the zero-based index of the DeviceParam object to access
	 *
	 * @return A reference to the requested DeviceParam object
	 *
	 * @throw An exception if the index supplied is out of bounds
	 */
	const DeviceParam& CALL_TYPE getParam(size_t idx) const;


	/**
	 * @fn void addParam(const DeviceParam& param)
	 *
	 * @brief Adds a single DeviceParam to the current list
	 *
	 * @param param - the new DeviceParam object
	 *
	 */
	void CALL_TYPE addParam(const DeviceParam& param);
	                                        

  private:

	Device(); // default constructor not used

	gmsec::api::mist::internal::InternalDevice* m_iDevice;

};

}
}
}

#endif
