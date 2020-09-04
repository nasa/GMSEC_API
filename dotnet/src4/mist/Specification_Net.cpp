/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// C# managed
#include <mist/Specification_Net.h>

#include <mist/SchemaIDIterator_Net.h>

#include <Config_Net.h>
#include <Message_Net.h>
#include <GMSEC_Exception_Net.h>
#include <GMSEC_Net.h>

#include <vcclr.h>


// C++ API native
#include <gmsec4/Exception.h>
#include <iostream>


using namespace GMSEC::API;
using namespace GMSEC::API::MIST;
using namespace System;
using namespace System::Runtime::InteropServices;


namespace GMSEC
{
namespace API
{
namespace MIST
{

// delegate for the callback
delegate void ValidateMessageDelegate(const gmsec::api::Message& msg);


/// <summary>
/// Native Specification that is passed to the GMSEC API.
/// </summary>
class NativeSpecification : public gmsec::api::mist::Specification
{
private:
    typedef void (__stdcall *ValidateMessageDelegateNative)(const gmsec::api::Message& msg);


public:
    explicit NativeSpecification(const gmsec::api::Config& config, ValidateMessageDelegate^ delegate)
        : Specification(config),
		  validateMsgDelegate(delegate)
    {
        nativeValidateMsg = (ValidateMessageDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
    }


    explicit NativeSpecification(const gmsec::api::mist::Specification& other, ValidateMessageDelegate^ delegate)
        : Specification(other),
		  validateMsgDelegate(delegate)
    {
        nativeValidateMsg = (ValidateMessageDelegateNative) Marshal::GetFunctionPointerForDelegate(delegate).ToPointer();
    }

	virtual ~NativeSpecification()
	{
	}

    virtual void CALL_TYPE validateMessage(const gmsec::api::Message& msg)
    {
        nativeValidateMsg(msg);
    }

    // Set the number handles up by one to avoid garbage collection
    gcroot<ValidateMessageDelegate^> validateMsgDelegate;
    ValidateMessageDelegateNative    nativeValidateMsg;
};

}
}
}


Specification::Specification(Config^ config)
{
	THROW_EXCEPTION_IF_NULLPTR(config, StatusClass::MIST_ERROR, StatusCode::INVALID_CONFIG, "Config is null");

	try
	{
		gmsec::api::Config* nativeConfig = config->GetUnmanagedImplementation();

		m_impl  = new NativeSpecification(*nativeConfig, gcnew ValidateMessageDelegate(this, &Specification::ValidateMessageNative));
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Specification::Specification(Specification^ other)
{
	THROW_EXCEPTION_IF_NULLPTR(other, StatusClass::MIST_ERROR, StatusCode::UNINITIALIZED_OBJECT, "Specification is null");

	try
	{
		gmsec::api::mist::Specification* nativeSpec = other->GetUnmanagedImplementation();

		m_impl  = new NativeSpecification(*nativeSpec, gcnew ValidateMessageDelegate(this, &Specification::ValidateMessageNative));
		m_owned = true;
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


Specification::~Specification()
{
	this->!Specification();
}


void Specification::ValidateMessageNative(const gmsec::api::Message& msg)
{
    Message^ msgWrapped  = gcnew Message(const_cast<gmsec::api::Message*>(&msg), false);

    ValidateMessage(msgWrapped);
}


void Specification::ValidateMessage(Message^ msg)
{
	THROW_EXCEPTION_IF_NULLPTR(msg, StatusClass::MIST_ERROR, StatusCode::INVALID_MESSAGE, "Message is null");

	try
	{
		gmsec::api::Message* nativeMsg = msg->GetUnmanagedImplementation();

		m_impl->Specification::validateMessage(*nativeMsg);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
}


SchemaIDIterator^ Specification::GetSchemaIDIterator()
{
	gmsec::api::mist::SchemaIDIterator& iter = m_impl->getSchemaIDIterator();

	return gcnew SchemaIDIterator(&iter);
}


unsigned int Specification::GetVersion()
{
	return m_impl->getVersion();
}


String^ Specification::GetTemplateXML(String^ subject, String^ schemaID)
{
	char* subjectStr  = nullptr;
	char* schemaIDStr = nullptr;

	try
	{
		subjectStr  = static_cast<char*>(Marshal::StringToHGlobalAnsi(subject).ToPointer());
		schemaIDStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(schemaID).ToPointer());

		const char* xml = m_impl->getTemplateXML(subjectStr, schemaIDStr);

		return gcnew String(xml);
	}
	catch (gmsec::api::Exception& e)
	{
		throw gcnew GMSEC_Exception(e);
	}
	finally
	{
		FREE_HGLOBAL_IF_NOT_NULLPTR(subjectStr);
		FREE_HGLOBAL_IF_NOT_NULLPTR(schemaIDStr);
	}
}


Specification::!Specification()
{
	ResetUnmanagedImplementation();
}


Specification::Specification(gmsec::api::mist::Specification* spec, bool owned)
{
	SetUnmanagedImplementation(spec, owned);
}


gmsec::api::mist::Specification* Specification::GetUnmanagedImplementation()
{
	return m_impl;
}


void Specification::ResetUnmanagedImplementation()
{
	if (m_impl != nullptr && m_owned)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}


void Specification::SetUnmanagedImplementation(gmsec::api::mist::Specification* spec, bool owned)
{
	ResetUnmanagedImplementation();

	m_impl  = spec;
	m_owned = owned;
}
