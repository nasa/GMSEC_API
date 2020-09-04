/*
 * Copyright 2007-2016 United States Government as represented by the
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


#include <gmsec4/internal/mist/InternalDeviceMessage.h>

#include <gmsec4/internal/mist/InternalDeviceIterator.h>
#include <gmsec4/internal/mist/MessageSubclassHelper.h>

#include <gmsec4/mist/Device.h>
#include <gmsec4/util/Log.h>

#include <gmsec4/Exception.h>

#include <gmsec4/internal/StringUtil.h>

#include <sstream>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;
using namespace gmsec::api::mist::internal;


static const char* HEADER_VERSION_STRING    = "HEADER-VERSION";
static const char* MESSAGE_TYPE_STRING      = "MESSAGE-TYPE";
static const char* MESSAGE_SUBTYPE_STRING   = "MESSAGE-SUBTYPE";
static const char* CONTENT_VERSION_STRING   = "CONTENT-VERSION";
static const char* MSG_ID_STRING            = "MSG-ID";
static const char* MSG_STRING               = "MSG";
static const char* C2CX_STRING              = "C2CX";
static const char* DEV_STRING               = "DEV";
static const char* NUM_OF_DEVICES_STRING    = "NUM-OF-DEVICES";
static const char* C2CX_SUBTYPE_STRING      = "C2CX-SUBTYPE";


InternalDeviceMessage::InternalDeviceMessage(const char* subject, unsigned int version)
	: InternalMessage(subject, Message::PUBLISH),
	  m_list(),
	  m_deviceIterator(*this)
{
	init(version);
}


InternalDeviceMessage::InternalDeviceMessage(const char* subject, const Config& config, unsigned int version)
	: InternalMessage(subject, Message::PUBLISH, config),
	  m_list(),
	  m_deviceIterator(*this)
{
	init(version);
}


InternalDeviceMessage::InternalDeviceMessage(const InternalDeviceMessage& other)
	: InternalMessage(other),
	  m_list(other.m_list),
	  m_deviceIterator(*this)
{
	init(other.m_specVersion);
}


InternalDeviceMessage::InternalDeviceMessage(const char* data)
	: InternalMessage(data),
	  m_list(),
	  m_deviceIterator(*this)
{
	MessageSubclassHelper::checkFloatField(HEADER_VERSION_STRING, "InternalDeviceMessage()",
		2010.0, *this);

	MessageSubclassHelper::checkStringField(MESSAGE_TYPE_STRING, "InternalDeviceMessage()",
		MSG_STRING, *this);

	MessageSubclassHelper::checkStringField(MESSAGE_SUBTYPE_STRING, "InternalDeviceMessage()",
		C2CX_STRING, *this);

	MessageSubclassHelper::checkStringField(C2CX_SUBTYPE_STRING, "InternalDeviceMessage()",
		DEV_STRING, *this);

	try
	{
		const F32Field& f32_field = getF32Field(CONTENT_VERSION_STRING);
		if(f32_field.getValue() == 2016.0)
		{
			m_specVersion = GMSEC_ISD_2016_00;
		}else{
			//If message is not from the 2016 (or other known revision), message version shall be oldest supported
			m_specVersion = GMSEC_ISD_2014_00;
		}
	}
	catch (const Exception& excep)
	{
		std::ostringstream oss;

		oss << "InternalDeviceMessage:  Error while fetching "
		    << CONTENT_VERSION_STRING << " from message; field not fetched."
			<< excep.what();

		throw Exception(MIST_ERROR, MISSING_REQUIRED_FIELD, oss.str().c_str());
	}

	GMSEC_U16 num_of_devices;
	if(m_specVersion == GMSEC_ISD_2016_00){
		num_of_devices = MessageSubclassHelper::extractU16Field(NUM_OF_DEVICES_STRING, 
							"InternalDeviceMessage::InternalDeviceMessage()", *this);
	}else{
		num_of_devices = MessageSubclassHelper::extractI16Field(NUM_OF_DEVICES_STRING, 
							"InternalDeviceMessage::InternalDeviceMessage()", *this);
	}

	for (GMSEC_I16 count = 0; count < num_of_devices; count++)
	{
		m_list.push_back(extractMessageDevice(count+1));
	}
}


InternalDeviceMessage::~InternalDeviceMessage()
{
}


void InternalDeviceMessage::addDevice(const Device& device)
{
	m_list.push_back(device);

	if(m_specVersion == GMSEC_ISD_2014_00){
		addField(NUM_OF_DEVICES_STRING, (GMSEC_I16)m_list.size());
	}else if(m_specVersion == GMSEC_ISD_2016_00){
		addField(NUM_OF_DEVICES_STRING, (GMSEC_U16)m_list.size());
	}else{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate"
			<< " Device Message number of devices";
	}
	

	std::ostringstream s;
	s << "DEVICE." << m_list.size();
	std::string strCount = s.str();

	std::string nameStr = strCount;
	nameStr.append(".NAME");
	addField(nameStr.c_str(), device.getName());

	std::string statusStrName = strCount;
	statusStrName.append(".STATUS");
	addField(statusStrName.c_str(), (GMSEC_I16) device.getStatus());

	std::string numParamsName = strCount;
	numParamsName.append(".NUM-OF-PARAMS");
	
	if(m_specVersion == GMSEC_ISD_2014_00){
		addField(numParamsName.c_str(), (GMSEC_I16) device.getParamCount());
	}else if(m_specVersion == GMSEC_ISD_2016_00){
		addField(numParamsName.c_str(), (GMSEC_U16) device.getParamCount());
	}else{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate"
			<< " Device Message number of params";
	}
	
	if (device.infoAvailable())
	{
		std::string infoCount = strCount;
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
		std::string numberCount = strCount;
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
		std::string modelCount = strCount;
		modelCount.append(".MODEL");
		addField(modelCount.c_str(), device.getModel());
	}

	if (device.serialAvailable())
	{
		std::string serialCount = strCount;
		serialCount.append(".SERIAL");
		addField(serialCount.c_str(), device.getSerial());
	}

	if (device.versionAvailable())
	{
		std::string versionCount = strCount;
		versionCount.append(".VERSION");
		addField(versionCount.c_str(), device.getVersion());
	}

	if (device.groupAvailable())
	{
		std::string groupCount = strCount;
		groupCount.append(".GROUP");
		addField(groupCount.c_str(), device.getGroup());
	}

	if (device.roleAvailable())
	{
		std::string roleCount = strCount;
		roleCount.append(".ROLE");
		addField(roleCount.c_str(), device.getRole());
	}

	for (size_t s_idx = 0; s_idx < device.getParamCount(); s_idx++)
	{
		std::ostringstream ss;
		ss << strCount << ".PARAM." << (s_idx + 1);
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
	if (index < m_list.size())
	{
		return m_list[index];
	}

	std::ostringstream oss;

	oss << "InternalDeviceMessage::getDevice():  index "
	    << index << "specified is out-of-range";

	throw Exception(MIST_ERROR, INDEX_OUT_OF_RANGE, oss.str().c_str());
}


size_t InternalDeviceMessage::getNumDevices() const
{
	return m_list.size();
}


DeviceIterator& InternalDeviceMessage::getDeviceIterator()
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


Device InternalDeviceMessage::extractMessageDevice(size_t index) const
{
	char   tmp_name[256];
	
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
	size_t device_num_of_params;
	if(m_specVersion == GMSEC_ISD_2016_00){
		device_num_of_params = MessageSubclassHelper::extractU16Field(tmp_name, 
							"InternalDeviceMessage::extractMessageDevice()", *this);
	}else{
		device_num_of_params = MessageSubclassHelper::extractI16Field(tmp_name, 
							"InternalDeviceMessage::extractMessageDevice()", *this);
	}
	
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

	size_t      param_idx = 1;
	std::string param_name;
	std::string param_timestamp;
	const Field* param_value_ptr = NULL;

	while (param_idx <= device_num_of_params)
	{
		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.PARAM.%u.NAME", index, param_idx);
		param_name = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalDeviceMessage::extractMessageDevice()",
													  *this);
		

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.PARAM.%u.TIME", index, param_idx);
		param_timestamp = MessageSubclassHelper::extractStringField(tmp_name, 
													  "InternalDeviceMessage::extractMessageDevice()",
													  *this);

		StringUtil::stringFormat(tmp_name, sizeof(tmp_name), "DEVICE.%u.PARAM.%u.VALUE", index, param_idx);

		param_value_ptr = MessageSubclassHelper::extractField(tmp_name, 
						   "InternalDeviceMessage::extractMessageDevice()", *this);
		
		DeviceParam device_param(param_name.c_str(), param_timestamp.c_str(), *param_value_ptr);

		device.addParam(device_param);

		param_idx++;
	}

	return device;
}


void InternalDeviceMessage::init(unsigned int version)
{
	m_specVersion = version;

	//Ignore version, placeholder for updates to ISD
	addField(HEADER_VERSION_STRING, (GMSEC_F32) 2010);
	addField(MESSAGE_TYPE_STRING, MSG_STRING);
	addField(MESSAGE_SUBTYPE_STRING, C2CX_STRING);

	addField(C2CX_SUBTYPE_STRING, DEV_STRING);
	
	if (m_specVersion == GMSEC_ISD_2014_00)
	{
		addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2013);
		addField(MSG_ID_STRING, getSubject()); //MSG-ID only needed pre-2016
		addField(NUM_OF_DEVICES_STRING, (GMSEC_I16) m_list.size());
	}
	else if(m_specVersion == GMSEC_ISD_2016_00)
	{
		addField(CONTENT_VERSION_STRING, (GMSEC_F32) 2016);
		addField(NUM_OF_DEVICES_STRING, (GMSEC_U16) m_list.size());
	}
	else
	{
		GMSEC_WARNING << "Specification version unknown: " << m_specVersion << ", unable to autopopulate"
			<< " Device Message header information";
	}
}
