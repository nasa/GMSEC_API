/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <mist/Device_Net.h>

#include <mist/DeviceParam_Net.h>
#include <field/Field_Net.h>
#include <field/FieldConverter_Net.h>

#include <GMSEC_Net.h>


// C++ API native
#include <gmsec4/mist/DeviceParam.h>
#include <gmsec4/field/Field.h>
#include <gmsec4/util/DataList.h>
#include <gmsec4/Exception.h>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


Device::Device(String^ name, DeviceStatus status)
{
	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		m_impl  = new gmsec::api::mist::Device(nameStr, static_cast<gmsec::api::mist::Device::DeviceStatus>(status));
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Device::Device(String^ name, DeviceStatus status, List<DeviceParam^>^ params)
{
	THROW_EXCEPTION_IF_NULLPTR(params, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceParam list is null");

	char* nameStr = nullptr;

	try
	{
		nameStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(name).ToPointer());

		gmsec::api::util::DataList<gmsec::api::mist::DeviceParam*> deviceParamList;

		for (int i = 0; i < params->Count; ++i)
		{
			DeviceParam^ param = params[i];

			deviceParamList.push_back(param->GetUnmanagedImplementation());
		}

		m_impl  = new gmsec::api::mist::Device(nameStr, static_cast<gmsec::api::mist::Device::DeviceStatus>(status), deviceParamList);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process name string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(nameStr);
	}
}


Device::Device(Device^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		gmsec::api::mist::Device* nativeOther = ((Device^) other)->GetUnmanagedImplementation();

		m_impl  = new gmsec::api::mist::Device(*nativeOther);
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Device::~Device()
{
	this->!Device();
}


String^ Device::GetName()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return gcnew String(m_impl->getName());
}


Device::DeviceStatus Device::GetStatus()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return static_cast<Device::DeviceStatus>(m_impl->getStatus());
}


void Device::SetStatus(Device::DeviceStatus status)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	m_impl->setStatus(static_cast<gmsec::api::mist::Device::DeviceStatus>(status));
}


bool Device::NumberAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->numberAvailable();
}


Field^ Device::GetNumber()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		const gmsec::api::Field& nativeNumber = m_impl->getNumber();

		return FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeNumber));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetNumber(Field^ number)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");
	THROW_EXCEPTION_IF_NULLPTR(number, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Number Field is null");

	try
	{
		gmsec::api::Field* nativeNumber = ((Field^) number)->GetChild();

		m_impl->setNumber(*nativeNumber);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


bool Device::ModelAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->modelAvailable();
}


String^ Device::GetModel()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		return gcnew String(m_impl->getModel());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetModel(String^ model)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	char* modelStr = nullptr;

	try
	{
		modelStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(model).ToPointer());

		m_impl->setModel(modelStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process model string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(modelStr);
	}
}


bool Device::SerialAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->serialAvailable();
}


String^ Device::GetSerial()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		return gcnew String(m_impl->getSerial());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetSerial(String^ serial)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	char* serialStr = nullptr;

	try
	{
		serialStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(serial).ToPointer());

		m_impl->setSerial(serialStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process serial string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(serialStr);
	}
}


bool Device::VersionAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->versionAvailable();
}


String^ Device::GetVersion()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		return gcnew String(m_impl->getVersion());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetVersion(String^ version)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	char* versionStr = nullptr;

	try
	{
		versionStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(version).ToPointer());

		m_impl->setVersion(versionStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process version string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(versionStr);
	}
}


bool Device::GroupAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->groupAvailable();
}


String^ Device::GetGroup()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		return gcnew String(m_impl->getGroup());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetGroup(String^ group)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	char* groupStr = nullptr;

	try
	{
		groupStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(group).ToPointer());

		m_impl->setGroup(groupStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process group string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(groupStr);
	}
}


bool Device::RoleAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->roleAvailable();
}


String^ Device::GetRole()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		return gcnew String(m_impl->getRole());
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetRole(String^ role)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	char* roleStr = nullptr;

	try
	{
		roleStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(role).ToPointer());

		m_impl->setRole(roleStr);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	catch (...)
	{
		throw gcnew GMSEC_Exception(StatusClass::MIST_ERROR, StatusCode::OUT_OF_MEMORY, "Unable to process role string");
	}
	finally
	{
		 FREE_HGLOBAL_IF_NOT_NULLPTR(roleStr);
	}
}


bool Device::InfoAvailable()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return m_impl->infoAvailable();
}


Field^ Device::GetInfo()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		const gmsec::api::Field& nativeInfo = m_impl->getInfo();

		return FieldConverter::CreateFieldReference(const_cast<gmsec::api::Field*>(&nativeInfo));
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::SetInfo(Field^ info)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");
	THROW_EXCEPTION_IF_NULLPTR(info, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Info Field is null");

	try
	{
		gmsec::api::Field* nativeInfo = ((Field^) info)->GetChild();

		m_impl->setInfo(*nativeInfo);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


long Device::GetParamCount()
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	return (long) m_impl->getParamCount();
}


DeviceParam^ Device::GetParam(long index)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");

	try
	{
		const gmsec::api::mist::DeviceParam& nativeParam = m_impl->getParam(index);

		return gcnew DeviceParam(const_cast<gmsec::api::mist::DeviceParam*>(&nativeParam), false);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


void Device::AddParam(DeviceParam^ param)
{
	THROW_EXCEPTION_IF_NULLPTR(m_impl, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Device is null");
	THROW_EXCEPTION_IF_NULLPTR(param, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "DeviceParam is null");

	try
	{
		gmsec::api::mist::DeviceParam* nativeParam = ((DeviceParam^) param)->GetUnmanagedImplementation();

		m_impl->addParam(*nativeParam);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Device::!Device()
{
	ResetUnmanagedImplementation();
}


Device::Device(gmsec::api::mist::Device* device, bool owned)
{
	SetUnmanagedImplementation(device, owned);
}


gmsec::api::mist::Device* Device::GetUnmanagedImplementation()
{
	return m_impl;
}


void Device::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void Device::SetUnmanagedImplementation(gmsec::api::mist::Device* device, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = device;
	m_owned = owned;
}
