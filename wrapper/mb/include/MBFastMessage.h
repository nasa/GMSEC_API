/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_FAST_MESSAGE_H
#define MB_FAST_MESSAGE_H


#include <gmsec4/Message.h>


namespace gmsec_messagebus
{

/**
 * @brief This class implements the Message abstract base class to support "Magic Bus" middleware.
 * This file contains the declaration of the MBFastMessage class used by the
 * Magic Bus.  MBFastMessage is only used in the Message Bus server.  It
 * holds an encoded message, only parsing the subject.  It only implements
 * that portion of the framework Message interface which is needed for
 * distributing messages to the clients.
 */
class MBFastMessage
{
public:
	MBFastMessage(gmsec::api::Message* msg, void* creatorObject);


	~MBFastMessage();


	static bool destroy(MBFastMessage* mb);


	/**
	 * @fn release()
	 * Releases ownership of the stored GMSEC Message to the callee.
	 */
	gmsec::api::Message* release();


	/**
	 * @fn get()
	 * Provides a reference to the store GMSEC Message.
	 */
	gmsec::api::Message* get();


	/**
	 * @fn get() const
	 * Provides a reference to the store GMSEC Message.
	 */
	const gmsec::api::Message* get() const;


	/**
	 * @fn getSubject() const
	 * Returns the subject of the stored GMSEC message (if any)
	 */
	const char* getSubject() const;


	/**
	 * @fn getCreatorObject()
	 * this function returns the handle of the
	 * object that created this instance
	 * This is intended to be used for handling
	 * loopback filtering on messages comming
	 * from the same connMgr that creted it
	 */
	void* getCreatorObject();


	/**
	 * @fn getRefCount() const
	 * return the refference count for this message this can be used to see if the message is ready for deletion
	 */
	int getRefCount();


	/**
	 * @fn setRefCount(unsigned int refCount)
	 * set the reference count
	 */
	void setRefCount(int refCount);


private:
	// Defined, but not implemented
	MBFastMessage(const MBFastMessage&);
	MBFastMessage& operator=(const MBFastMessage&);

	gmsec::api::Message* m_msg;
	bool                 m_ownMsg;
	void*                m_creatorObject;
	int                  m_refCount;
};

}  // end namespace gmsec_messagebus

#endif	// MB_FAST_MESSAGE_H
