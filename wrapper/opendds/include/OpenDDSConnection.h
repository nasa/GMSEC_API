/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Holds the declaration of OpenDDS subclass of gmsec::api::internal::ConnectionInterface.

#ifndef OPENDDS_CONNECTION_H
#define OPENDDS_CONNECTION_H

#include <gmsec_opendds.h>
#include <Queue.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/UniqueFilter.h>
#include <gmsec4/internal/Encoder.h>

#include <gmsec4/Config.h>
#include <gmsec4/Message.h>
#include <gmsec4/Status.h>

// OpenDDS required header files
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsSubscriptionC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>

#include <dds/DCPS/StaticIncludes.h>

#include <OpenDDSMessageListener.h>
#include <OpenDDSMessageTypeSupportImpl.h>

#include <string>


namespace gmsec_opendds
{

// OpenDDS implementation of the GMSEC ConnectionInterface.
class GMSEC_OPENDDS_API OpenDDSConnection : public gmsec::api::internal::ConnectionInterface
{
public:
	// Create an OpenDDS Connection with configuration.
	OpenDDSConnection(const gmsec::api::Config& config);
	
	// Destroy an OpenDDS Connection.
	virtual ~OpenDDSConnection();

	// Determines the middleware wrapper tag.
	virtual const char* CALL_TYPE getLibraryRootName()
	{
		return OPENDDS_LIBROOTNAME;
	}
	
	// Determines the middleware version string.
	virtual const char* CALL_TYPE getLibraryVersion();
	
	// This function returns the middleware information
	virtual const char* CALL_TYPE getMWInfo();

	// This function does nothing for OpenDDS.
	virtual void CALL_TYPE mwConnect();

	// Disconnect from the middleware server.
	virtual void CALL_TYPE mwDisconnect();

	// Subscribe to subject (OpenDDS software wrapper currently does not support wildcards) using the configuration supplied in the config object.
	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config &config);

	// Unsubscribe from subject (OpenDDS software wrapper currently does not support wildcards).
	virtual void CALL_TYPE mwUnsubscribe(const char* subject);

	// Publish message msg using configuration.
	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& msg, const gmsec::api::Config& config);

	// Send request message request filling in and returning its reply id.
	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& request, std::string& uniqueID);

	// Send reply in response to a request.
	virtual void CALL_TYPE mwReply(const gmsec::api::Message& request, const gmsec::api::Message& reply);

	// Wait up to @p timeout milliseconds for next message.
	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& msg, GMSEC_I32 timeout);
	
private:
	std::string                        m_brokerService;
	std::string                        m_openddsConfig;
	std::string                        m_dcpsInfoRepo;
	std::string                        m_mwInfo;
	Queue                              m_queue;
	GMSEC_U32                          m_requestCounter;
	gmsec::api::internal::RequestSpecs m_requestSpecs;
	bool                               m_selfSubscribed;

	struct OpenDDSSubscriber
	{
		DDS::Subscriber_var 		subscriber;
		DDS::DataReaderListener_var listener;
		DDS::DataReader_var 		reader;
	};

	struct OpenDDSWriter
	{
		DDS::Publisher_var  publisher;
		DDS::DataWriter_var writer;
	};

	typedef std::map<std::string, OpenDDSSubscriber*> OpenDDSSubscriptions;  // key is subject
	typedef std::map<std::string, OpenDDSWriter*>     OpenDDSWriters;        // key is subject

	OpenDDSSubscriptions               m_subscriptions;
	OpenDDSWriters                     m_writers;
	DDS::DomainParticipantFactory_var  m_dpf;
	DDS::DomainParticipant_var         m_participant;
	DDS::DomainId_t                    m_domainId;


	// Helper methods
	OpenDDSWriter* createWriter(const std::string& subject, const gmsec::api::Config& config);

	OpenDDSMessage::Message prepareMsg(const gmsec::api::Message& msg);

	std::string generateUniqueId(GMSEC_U32 id);

	// Encode the meta data associated with the message
	gmsec::api::Status storeProperties(const gmsec::api::internal::ValueMap& meta, gmsec::api::util::DataBuffer& out);


	// Determine the size of the meta blob
	void findLength(const gmsec::api::internal::ValueMap& meta, size_t& length);

	// Encode the gmsec::internal::Value into the meta blob
	gmsec::api::Status storeProperty(const char* valueId, const gmsec::api::internal::Value& value, char** currentBuffer);

	std::string getConnectionEndpoint();
};

}  // end namespace gmsec_opendds

#endif
