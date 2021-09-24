/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalMessage.h
 *
 *  @brief This file contains the wrapper class for messages. Method
 *	calls pass through this class to the "real" message, to which a pointer is held
 */


#ifndef gmsec_InternalMessage_h
#define gmsec_InternalMessage_h


#include <gmsec_defs.h>

#include <gmsec/FieldIterator.h>
#include <gmsec/Status.h>

#include <gmsec/internal/MessageFieldMap.h>

#include <gmsec/util/wdllexp.h>


namespace gmsec
{
class Config;
class Field;
class FieldIterator;
class Message;

namespace api
{
class Message;
}

namespace internal
{

class GMSEC_API InternalMessage
{
public:
	InternalMessage();

	InternalMessage(const char* subject, GMSEC_MSG_KIND msgKind);

	InternalMessage(const char* subject, GMSEC_MSG_KIND msgKind, gmsec::Config* config);

	InternalMessage(gmsec::api::Message* msg4);

	~InternalMessage();

	// deprecated
	bool CALL_TYPE isValid();

	bool CALL_TYPE IsValid();

	const char* CALL_TYPE GetLibraryRootName();

	Status CALL_TYPE SetConfig(Config* config);

	Status CALL_TYPE SetKind(GMSEC_MSG_KIND kind);

	Status CALL_TYPE GetKind(GMSEC_MSG_KIND& kind);

	Status CALL_TYPE SetSubject(const char* subject);

	Status CALL_TYPE GetSubject(const char*& subject);

	Status CALL_TYPE ClearFields();

	Status CALL_TYPE AddField(Field& field);

	Status CALL_TYPE ClearField(const char* name);

	Status CALL_TYPE GetField(const char* name, Field& field);

	Status CALL_TYPE GetFieldCount(GMSEC_I32& count);

	Status CALL_TYPE GetFirstField(Field& field);

	Status CALL_TYPE GetNextField(Field& field);

	Status CALL_TYPE CopyFields(InternalMessage* toMsg);

	Status CALL_TYPE ToXML(const char*& xml);

	Status CALL_TYPE FromXML(const char* xml);

	const char* CALL_TYPE ToJSON();

	Status CALL_TYPE FromJSON(const char* json);

	Status CALL_TYPE GetMSGSize(GMSEC_U32& size);

	Status CALL_TYPE Reset(FieldIterator& iter);

	gmsec::api::Message* getAdapter();


private:
	// Declared, but not implemented.
	InternalMessage(const InternalMessage&);
	InternalMessage& operator=(const InternalMessage&);	

	gmsec::api::Message* m_adapter;
	MessageFieldMap      m_fields;
	FieldIterator        m_fieldIter;
	bool                 m_fieldIterWarned;
	bool                 m_kindUnset;
};


} //namespace internal
} //namespace gmsec

#endif
