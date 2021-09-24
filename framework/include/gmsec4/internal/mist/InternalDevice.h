/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalDevice.h
 *
 *  @brief This file contains declarations for Device containers
**/

#ifndef GMSEC_API_INTERNAL_DEVICE_H
#define GMSEC_API_INTERNAL_DEVICE_H

#include <gmsec4/mist/DeviceParam.h>
#include <gmsec4/mist/Device.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{


class GMSEC_API InternalDevice
{
public:

	InternalDevice(const char* name, Device::DeviceStatus status);

	InternalDevice(const char* name, Device::DeviceStatus status, const gmsec::api::util::DataList<DeviceParam*>& params);

	InternalDevice(const InternalDevice& other);

	~InternalDevice();

	InternalDevice& operator=(const InternalDevice& other);

	const char* CALL_TYPE getName() const;

	bool CALL_TYPE numberAvailable() const;

	const Field& CALL_TYPE getNumber() const;

	void CALL_TYPE setNumber(const Field& number);

	bool CALL_TYPE modelAvailable() const;

	const char* CALL_TYPE getModel() const;

	void CALL_TYPE setModel(const char* model);

	bool CALL_TYPE serialAvailable() const;

	const char* CALL_TYPE getSerial() const;

	void CALL_TYPE setSerial(const char* serial);

	bool CALL_TYPE versionAvailable() const;

	const char* CALL_TYPE getVersion() const;

	void CALL_TYPE setVersion(const char* version);

	bool CALL_TYPE groupAvailable() const;

	const char* CALL_TYPE getGroup() const;

	void CALL_TYPE setGroup(const char* group);

	bool CALL_TYPE roleAvailable() const;

	const char* CALL_TYPE getRole() const;

	void CALL_TYPE setRole(const char* role);

	Device::DeviceStatus CALL_TYPE getStatus() const;

	void CALL_TYPE setStatus(Device::DeviceStatus status);

	bool CALL_TYPE infoAvailable() const;

	const Field& CALL_TYPE getInfo() const;

	void CALL_TYPE setInfo(const Field& info);

	size_t CALL_TYPE getParamCount() const;

	const DeviceParam& CALL_TYPE getParam(size_t idx) const;

	void CALL_TYPE addParam(const DeviceParam& param);

private:
	std::string  m_name;

	Field*       m_number;
	bool         m_hasNumber;

	std::string  m_model;
	bool         m_hasModel;

	std::string  m_serial;
	bool         m_hasSerial;

	std::string  m_version;
	bool         m_hasVersion;

	std::string  m_group;
	bool         m_hasGroup;

	std::string  m_role;
	bool         m_hasRole;

	Device::DeviceStatus m_status;

	Field*       m_info;
	bool         m_hasInfo;

	gmsec::api::util::DataList<DeviceParam*> m_paramList;

};

}
}
}
}

#endif
