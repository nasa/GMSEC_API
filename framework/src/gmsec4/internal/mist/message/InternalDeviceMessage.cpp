/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalDeviceMessage.cpp
 *
 * @brief This file contains the internal implementation of Device messages.
 */


// Disable the Visual Studio warning C4355
//   ('this' : used in base member initializer list)
//   Found in the Connection(Server s, const Options &o) constructor
#ifdef WIN32
#pragma warning(disable: 4355)
#endif

#include <gmsec4/internal/mist/message/InternalDeviceMessage.h>

#include <gmsec4/internal/mist/InternalDeviceIterator.h>
#include <gmsec4/internal/mist/MessageSubclassHelper.h>
#include <gmsec4/internal/mist/MessageTemplate.h>

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/util/Log.h>

#include <gmsec4/Exception.h>

#include <gmsec4/internal/StringUtil.h>

#include <sstream>


using namespace gmsec::api::mist::internal;
using namespace gmsec::api::util;


static const char* const MSG_ID_STRING            = "MSG-ID";
static const char* const NUM_OF_DEVICES_STRING    = "NUM-OF-DEVICES";
static const char* const C2CX_SUBTYPE_STRING      = "C2CX-SUBTYPE";


namespace gmsec
{
namespace api
{
namespace mist
{
namespace message
{
namespace internal
{

InternalDeviceMessage::InternalDeviceMessage(const char* subject, const char* id, const Specification& spec)
	: InternalMistMessage(subject, Message::PUBLISH, id, spec),
	  m_list(),
	  m_deviceIterator(*this)
{
	init();
}


InternalDeviceMessage::InternalDeviceMessage(const char* subject, const char* id, const gmsec::api::Config& config, const Specification& spec)
	: InternalMistMessage(subject, Message::PUBLISH, id, config, spec),
	  m_list(),
	  m_deviceIterator(*this)
{
	init();
}


InternalDeviceMessage::InternalDeviceMessage(const InternalDeviceMessage& other)
	: InternalMistMessage(other),
	  m_list(other.m_list),
	  m_deviceIterator(*this)
{
	init();
}


InternalDeviceMessage::InternalDeviceMessage(const char* data)
	: InternalMistMessage(data),
	  m_list(),
	  m_deviceIterator(*this)
{
	GMSEC_U32 numOfDevices = (GMSEC_U32) this->getUnsignedIntegerValue(NUM_OF_DEVICES_STRING);

	for (GMSEC_U32 index = 1; index <= numOfDevices; ++index)
	{
		m_list.push_back(extractMessageDevice(index));
	}
}


InternalDeviceMessage::InternalDeviceMessage(const Specification& spec, const char* data)
	: InternalMistMessage(spec, data),
	  m_list(),
	  m_deviceIterator(*this)
{
	GMSEC_U32 numOfDevices = (GMSEC_U32) this->getUnsignedIntegerValue(NUM_OF_DEVICES_STRING);

	for (GMSEC_U32 index = 1; index <= numOfDevices; ++index)
	{
		m_list.push_back(extractMessageDevice(index));
	}
}


InternalDeviceMessage::~InternalDeviceMessage()
{
}


void InternalDeviceMessage::addDevice(const Device& device)
{
	m_list.push_back(device);

	this->setValue(NUM_OF_DEVICES_STRING, (GMSEC_I64) m_list.size());

	std::ostringstream s;
	s << "DEVICE." << m_list.size();

	std::string nameStr = s.str();
	nameStr.append(".NAME");
	addField(nameStr.c_str(), device.getName());

	std::string statusStrName = s.str();
	statusStrName.append(".STATUS");
	addField(statusStrName.c_str(), (GMSEC_I16) device.getStatus());

	std::string numParamsName = s.str();
	numParamsName.append(".NUM-OF-PARAMS");
	
	this->setValue(numParamsName.c_str(), (GMSEC_I64) device.getParamCount());
	
	if (device.infoAvailable())
	{
		std::string infoCount = s.str();
		infoCount.append(".INFO");

		const Field &iField = device.getInfo();
		if (iField.getType() != Field::I16_TYPE)
		{
			throw Exception(MIST_ERROR,
			                INCORRECT_FIELD_TYPE,
			                "InternalDeviceMessage::addDevice():  Expected I16Field and got something different, for .INFO.");
		}

		MessageSubclassHelper::addRenamedFieldToMessage(iField, *this, infoCount.c_str());
	}

	if (device.numberAvailable())
	{
		std::string numberCount = s.str();
		numberCount.append(".NUMBER");

		GMSEC_I16 i16_value = 0;

		try
		{
			i16_value = dynamic_cast<const I16Field&>(device.getNumber()).getValue();
		}
		catch (...)
		{
			throw Exception(MIST_ERROR,
			                INCORRECT_FIELD_TYPE,
			                "InternalDeviceMessage::addDevice():  Expected I16Field and got something different in a dynamic_cast, for .NUMBER.");
		}
		addField(numberCount.c_str(), i16_value);
	}
	
	if (device.modelAvailable())
	{
		std::string modelCount = s.str();
		modelCount.append(".MODEL");
		addField(modelCount.c_str(), device.getModel());
	}

	if (device.serialAvailable())
	{
		std::string serialCount = s.str();
		serialCount.append(".SERIAL");
		addField(serialCount.c_str(), device.getSerial());
	}

	if (device.versionAvailable())
	{
		std::string versionCount = s.str();
		versionCount.append(".VERSION");
		addField(versionCount.c_str(), device.getVersion());
	}

	if (device.groupAvailable())
	{
		std::string groupCount = s.str();
		groupCount.append(".GROUP");
		addField(groupCount.c_str(), device.getGroup());
	}

	if (device.roleAvailable())
	{
		std::string roleCount = s.str();
		roleCount.append(".ROLE");
		addField(roleCount.c_str(), device.getRole());
	}

	for (size_t s_idx = 0; s_idx < device.getParamCount(); ++s_idx)
	{
		std::ostringstream ss;
		ss << s.str() << ".PARAM." << (s_idx+1);
		std::string sampleCountStr = ss.str();

		DeviceParam current = device.getParam(s_idx);

		std::string tsCount = sampleCountStr;
		tsCount.append(".TIME");
		addField(tsCount.c_str(), current.getTimestamp());

		std::string nameCount = sampleCountStr;
		nameCount.append(".NAME");
		addField(nameCount.c_str(), current.getName());

		std::string valCount = sampleCountStr;
		valCount.append(".VALUE");
		const Field& valueField = current.getValue();

		MessageSubclassHelper::addRenamedFieldToMessage(valueField, *this, valCount.c_str());
	}
}


const Device& InternalDeviceMessage::getDevice(size_t index) const
{
	//Note: Index is zero-based

	if (index >= m_list.size())
	{
		std::ostringstream oss;

		oss << "InternalDeviceMessage::getDevice():  index "
		    << index << "specified is out-of-range";

		throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, oss.str().c_str());
	}

	return m_list[index];
}


size_t InternalDeviceMessage::getNumDevices() const
{
	return m_list.size();
}


gmsec::api::mist::DeviceIterator& InternalDeviceMessage::getDeviceIterator()
{
	m_deviceIterator.m_iter->reset();
	return m_deviceIterator;
}


void InternalDeviceMessage::resetDeviceIterator()
{
	m_listIter = m_list.begin();
}


bool InternalDeviceMessage::hasNextDevice() const
{
	return m_listIter != m_list.end();
}


const Device& InternalDeviceMessage::nextDevice()
{
	if (!hasNextDevice())
	{
		throw Exception(MIST_ERROR, ITER_INVALID_NEXT, "No more Devices available");
	}

	const Device& device = *m_listIter;

	++m_listIter;

	return device;
}


std::string InternalDeviceMessage::buildSchemaID(const Specification& spec)
{
	if (spec.getVersion() < GMSEC_ISD_2019_00)
	{
		return "MSG.C2CX.DEV";
	}

	return "MSG.DEV";
}


Device InternalDeviceMessage::extractMessageDevice(size_t index) const
{
	//Note: Index is one-based

	char tmp_name[256];
	
	GMSEC_I16   device_info;
	GMSEC_I16   device_number;
	std::string device_model;
	std::string device_serial;
	std::string device_version;
	std::string device_group;
	std::string device_role;
	

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.NAME", index);

	std::string device_name = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalDeviceMessage::extractMessageDevice()",
													  *this);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.STATUS", index);
	Device::DeviceStatus device_status = static_cast<Device::DeviceStatus>(MessageSubclassHelper::extractI16Field(tmp_name, 
							"InternalDeviceMessage::extractMessageDevice()", *this));
	
	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.NUM-OF-PARAMS", index);
	size_t device_num_of_params = (size_t) this->getUnsignedIntegerValue(tmp_name);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.INFO", index);

	bool device_info_exists = MessageSubclassHelper::getOptionalI16(tmp_name, *this, device_info);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.NUMBER", index);
	bool device_number_exists = MessageSubclassHelper::getOptionalI16(tmp_name, *this, device_number);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.MODEL", index);
	bool device_model_exists = MessageSubclassHelper::getOptionalString(tmp_name, *this, device_model);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.SERIAL", index);
	bool device_serial_exists = MessageSubclassHelper::getOptionalString(tmp_name, *this, device_serial);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.VERSION", index);
	bool device_version_exists = MessageSubclassHelper::getOptionalString(tmp_name, *this, device_version);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.GROUP", index);
	bool device_group_exists = MessageSubclassHelper::getOptionalString(tmp_name, *this, device_group);

	StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.ROLE", index);
	bool device_role_exists = MessageSubclassHelper::getOptionalString(tmp_name, *this, device_role);

	Device device(device_name.c_str(), device_status);

	if (device_model_exists)
	{
		device.setModel(device_model.c_str());
	}

	if (device_serial_exists)
	{
		device.setSerial(device_serial.c_str());
	}

	if (device_version_exists)
	{
		device.setVersion(device_version.c_str());
	}

	if (device_group_exists)
	{
		device.setGroup(device_group.c_str());
	}

	if (device_role_exists)
	{
		device.setRole(device_role.c_str());
	}

	if (device_number_exists)
	{
		I16Field device_number_field("Device Number", device_number);
		device.setNumber(device_number_field);
	}

	if (device_info_exists)
	{
		I16Field device_info_field("Device Information", device_info);
		device.setInfo(device_info_field);
	}

	std::string  param_name;
	std::string  param_timestamp;
	const Field* param_value = NULL;

	for (size_t param_idx = 1; param_idx <= device_num_of_params; ++param_idx)
	{
		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.PARAM.%u.NAME", index, param_idx);
		param_name = getStringValue(tmp_name);

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.PARAM.%u.TIME", index, param_idx);
		param_timestamp = getStringValue(tmp_name);

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.PARAM.%u.VALUE", index, param_idx);
		param_value = getField(tmp_name);

		DeviceParam device_param(param_name.c_str(), param_timestamp.c_str(), *param_value);

		device.addParam(device_param);
	}

	return device;
}


void InternalDeviceMessage::init()
{
	setValue(NUM_OF_DEVICES_STRING, (GMSEC_I64) m_list.size());

	if (getSpecVersion() <= GMSEC_ISD_2014_00 && getField(MSG_ID_STRING) == NULL)
	{
		addField(MSG_ID_STRING, getSubject()); //MSG-ID only needed pre-2016
	}
}

} // end namespace internal
} // end namespace message
} // end namespace mist
} // end namespace api
} // end namespace gmsec
