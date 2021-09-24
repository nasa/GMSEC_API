/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MessagePopulator.h
 *
**/


#ifndef GMSEC_INTERNAL_MESSAGEPOPULATOR_H
#define GMSEC_INTERNAL_MESSAGEPOPULATOR_H

#include <gmsec4/Message.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/Device.h>
#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ServiceParam.h>
#include <gmsec4/mist/mist_defs.h>
#include <gmsec4/util/DataList.h>

#include <list>
#include <string>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API MessagePopulator
{
public:	
	typedef gmsec::api::util::DataList<Field*> FieldList;

	MessagePopulator(unsigned int isdVersion);

	~MessagePopulator();

	void CALL_TYPE setStandardFields(const gmsec::api::util::DataList<Field*>& standardFields);

	const gmsec::api::util::DataList<Field*>& getStandardFields() const;

	void CALL_TYPE addStandardFields(Message& msg) const;

	void CALL_TYPE populateSimpleServiceMessage(Message &msg, const char * opName, const Field& opNumber, const gmsec::api::util::DataList<Field*>& fields, const gmsec::api::util::DataList<ServiceParam*>& sParams);

	void CALL_TYPE populateDirective(Message &msg, const Field& directiveString, const gmsec::api::util::DataList<Field*>& fields);

	void CALL_TYPE populateLogMessage(Message &msg, const gmsec::api::util::DataList<Field*>& logFields, FieldList &standardFieldList);

	void CALL_TYPE populateHeartbeatMessage(Message &msg, const gmsec::api::util::DataList<Field*>& hbFields, FieldList &standardFieldList);

	void CALL_TYPE populateSimpleServiceAck(Message &msg, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields);

	void CALL_TYPE populateDirectiveAck(Message &msg, ResponseStatus::Response ssResponse, const gmsec::api::util::DataList<Field*>& fields);

	void CALL_TYPE populateResourceStaticMembers(Message &msg, size_t counter);

	static bool CALL_TYPE addFieldToMsg(const Field& fld, Message &msg, const char * nameOverwrite = NULL);

	static void CALL_TYPE destroyFields(FieldList& flist);

private:

	std::string generateUniqueID();

	unsigned int m_specVersion;
	FieldList    m_standardFieldsAllMsgs;
};

}  //namespace internal
}  //namespace mist
}  //namespace api
}  //namespace gmsec


#endif
