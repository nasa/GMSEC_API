/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Managed
#include <mist/SubscriptionInfo_Net.h>
#include <mist/ConnectionManagerCallback_Net.h>


using namespace GMSEC::API::MIST;



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


ConnectionManagerCallback^ SubscriptionInfo::GetCallback()
{
	if (m_impl != nullptr)
	{
		return m_callback;
	}

	return nullptr;
}


SubscriptionInfo::!SubscriptionInfo()
{
	ResetUnmanagedImplementation();
}


SubscriptionInfo::SubscriptionInfo(gmsec::api::mist::SubscriptionInfo* info)
{
	m_impl     = info;
	m_callback = nullptr;
}


SubscriptionInfo::SubscriptionInfo(gmsec::api::mist::SubscriptionInfo* info, ConnectionManagerCallback^ callback)
{
	m_impl     = info;
	m_callback = callback;
}


gmsec::api::mist::SubscriptionInfo* SubscriptionInfo::GetUnmanagedImplementation()
{
	return m_impl;
}


void SubscriptionInfo::ResetUnmanagedImplementation()
{
	m_impl     = nullptr;
	m_callback = nullptr;
}


void SubscriptionInfo::SetUnmanagedImplementation(gmsec::api::mist::SubscriptionInfo* info)
{
	m_impl = info;
}
