/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "MBFastMessage.h"

#include <gmsec4/util/Mutex.h>


using namespace gmsec::api;
using namespace gmsec::api::util; // Mutex

using namespace gmsec_messagebus;


// the mutex for modifying MBFastMessage reference counts
static Mutex& getRefCountMutex()
{
	static Mutex mutex;
	return mutex;
}


MBFastMessage::MBFastMessage(Message* msg, void* creatorObject)
	:
	m_msg(msg),
	m_ownMsg(true),
	m_creatorObject(creatorObject),
	m_refCount(0)
{
}


MBFastMessage::~MBFastMessage()
{
	AutoMutex hold(getRefCountMutex());

	if (m_ownMsg && m_msg)
	{
		delete m_msg;

		m_msg    = NULL;
		m_ownMsg = false;
	}
}


Message* MBFastMessage::release()
{
	AutoMutex hold(getRefCountMutex());

	Message* msg = m_msg;

	m_msg    = NULL;
	m_ownMsg = false;

	return msg;
}


Message* MBFastMessage::get()
{
	AutoMutex hold(getRefCountMutex());
	return m_msg;
}


const Message* MBFastMessage::get() const
{
	AutoMutex hold(getRefCountMutex());
	return m_msg;
}


const char* MBFastMessage::getSubject() const
{
	AutoMutex hold(getRefCountMutex());

	if (m_msg != NULL)
	{
		return m_msg->getSubject();
	}

	return NULL;
}


int MBFastMessage::getRefCount()
{
	AutoMutex hold(getRefCountMutex());
	return m_refCount;
}


void MBFastMessage::setRefCount(int refCount)
{
	AutoMutex hold(getRefCountMutex());
	m_refCount = refCount;
}


void* MBFastMessage::getCreatorObject()
{
	return m_creatorObject;
}


bool MBFastMessage::destroy(MBFastMessage* mb)
{
	bool toast = false;

	AutoMutex hold(getRefCountMutex());

	if (--mb->m_refCount <= 0)
	{
		toast = true;
		delete mb;
	}

	return toast;
}
