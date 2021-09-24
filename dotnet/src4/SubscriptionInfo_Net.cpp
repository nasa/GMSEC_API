/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Managed
#include <SubscriptionInfo_Net.h>
#include <Callback_Net.h>


using namespace GMSEC::API;



SubscriptionInfo::~SubscriptionInfo()
{
	this->!SubscriptionInfo();
}


System::String^ SubscriptionInfo::GetSubject()
{
	if (m_impl != nullptr)
	{
		return gcnew System::String(m_impl->getSubject());
	}

	return nullptr;
}


Callback^ SubscriptionInfo::GetCallback()
{
	if (m_impl != nullptr)
	{
		//TODO: Need lookup table to map C++ API native callback to C# managed callback
	}

	return nullptr;
}


SubscriptionInfo::!SubscriptionInfo()
{
	ResetUnmanagedImplementation();
}


SubscriptionInfo::SubscriptionInfo(gmsec::api::SubscriptionInfo* info)
{
	m_impl = info;
}


gmsec::api::SubscriptionInfo* SubscriptionInfo::GetUnmanagedImplementation()
{
	return m_impl;
}


void SubscriptionInfo::ResetUnmanagedImplementation()
{
	m_impl = nullptr;
}


void SubscriptionInfo::SetUnmanagedImplementation(gmsec::api::SubscriptionInfo* info)
{
	m_impl = info;
}
