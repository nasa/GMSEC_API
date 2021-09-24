/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConfigFileIterator.cpp
 *
 *  This file contains the implementation of the Field Iterator class.
 *
 */

#include <gmsec/ConfigFileIterator.h>
#include <gmsec/internal/SimpleConfigFileIterator.h>


namespace gmsec{

ConfigFileIterator::ConfigFileIterator()
	: ptr(new internal::SimpleConfigFileIterator())
{
}

ConfigFileIterator::~ConfigFileIterator()
{
	delete ptr;
	ptr = 0;
}

bool ConfigFileIterator::HasNextSubscriptionName()
{
	return ptr->HasNextSubscriptionName();
}

bool ConfigFileIterator::HasNextConfigName()
{
	return ptr->HasNextConfigName();
}

bool ConfigFileIterator::HasNextMessageName()
{
	return ptr->HasNextMessageName();
}

bool ConfigFileIterator::HasNextSubscription()
{
	return ptr->HasNextSubscription();
}

bool ConfigFileIterator::HasNextConfig()
{
	return ptr->HasNextConfig();
}

bool ConfigFileIterator::HasNextMessage()
{
	return ptr->HasNextMessage();
}

bool ConfigFileIterator::HasNextCustomElement()
{
	return ptr->HasNextCustomElement();
}
	
Status ConfigFileIterator::NextSubscriptionName(gmsec::util::String &gmstr)
{
	return ptr->NextSubscriptionName(gmstr);
}

Status ConfigFileIterator::NextConfigName(gmsec::util::String &gmstr)
{
	return ptr->NextConfigName(gmstr);
}

Status ConfigFileIterator::NextMessageName(gmsec::util::String &gmstr)
{
	return ptr->NextMessageName(gmstr);
}

Status ConfigFileIterator::NextSubscription(gmsec::util::String &gmstr)
{
	return ptr->NextSubscription(gmstr);
}

Status ConfigFileIterator::NextConfig(Config *config)
{
	return ptr->NextConfig(config);
}

Status ConfigFileIterator::NextMessage(Message *message)
{
	return ptr->NextMessage(message);
}

Status ConfigFileIterator::NextCustomElement(gmsec::util::String &gmstr)
{
	return ptr->NextCustomElement(gmstr);
}

}  //namespace gmsec

