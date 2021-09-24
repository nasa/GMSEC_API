/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConnectionManager
%{
#include <gmsec4/mist/ConnectionManager.h>
#include <list>
using namespace gmsec::api::mist;
%}


/* Methods containing lists that will be redefined */
%ignore gmsec::api::mist::ConnectionManager::setStandardFields(const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::createHeartbeatMessage(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::startHeartbeatService(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::createLogMessage(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::setLoggingDefaults(const char*, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, int, gmsec::api::mist::ConnectionManagerReplyCallback*, int);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, int, int);
%ignore gmsec::api::mist::Connectionmanager::acknowledgeDirectiveRequest(const char*, const gmsec::api::Message&, gmsec::api::mist::ResponseStatus::Response, const gmsec::api::util::DataList<gmsec::api::Field*>&);
%ignore gmsec::api::mist::ConnectionManager::acknowledgeSimpleService(const char*, const gmsec::api::Mesage&, gmsec::api::mist::ResponseStatus::Response, const gmsec::api::util::DataList<Field*>& fields);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, const gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*>&);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, const gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*>&, int, gmsec::api::mist::ConnectionManagerReplyCallback*, int);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const gmsec::api::Field&, const gmsec::api::util::DataList<gmsec::api::Field*>&, const gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*>&, int, int);
%ignore gmsec::api::mist::ConnectionManager::getStandardFields() const;
%ignore gmsec::api::mist::ConnectionManager::shutdownAllMiddlewares();

/* Ignore deprecated methods */
%ignore gmsec::api::mist::ConnectionManager::changeComponentStatus(const Field&);
%ignore gmsec::api::mist::ConnectionManager::changeComponentInfo(const Field&);
%ignore gmsec::api::mist::ConnectionManager::changeCPUMemory(const Field&);
%ignore gmsec::api::mist::ConnectionManager::changeCPUUtil(const Field&);

/* Ignore C methods */
%ignore gmsec::api::mist::ConnectionManager::registerMessageValidator(GMSEC_MessageValidator*);
%ignore gmsec::api::mist::ConnectionManager::registerEventCallback(Connection::ConnectionEvent, GMSEC_ConnectionMgrEventCallback*);
%ignore gmsec::api::mist::ConnectionManager::subscribe(const char*, const Config&, GMSEC_ConnectionMgrCallback*);
%ignore gmsec::api::mist::ConnectionManager::request(const Message&, GMSEC_I32, GMSEC_ConnectionMgrReplyCallback*, GMSEC_ConnectionMgrEventCallback*, GMSEC_I32);
%ignore gmsec::api::mist::ConnectionManager::cancelRequest(GMSEC_ConnectionMgrReplyCallback*);
%ignore gmsec::api::mist::ConnectionManager::requestDirective(const char*, const Field&, const gmsec::api::util::DataList<Field*>&, GMSEC_I32, GMSEC_ConnectionMgrReplyCallback*, GMSEC_ConnectionMgrEventCallback*, GMSEC_I32);
%ignore gmsec::api::mist::ConnectionManager::requestSimpleService(const char*, const char*, const Field&, const gmsec::api::util::DataList<Field*>&, const gmsec::api::util::DataList<ServiceParam*>&, GMSEC_I32, GMSEC_ConnectionMgrReplyCallback*, GMSEC_ConnectionMgrEventCallback*, GMSEC_I32);

%rename("get_API_version") getAPIVersion;
%rename("get_state") getState;
%rename("get_library_root_name") getLibraryRootName;
%rename("get_library_version") getLibraryVersion;
%rename("get_specification") getSpecification;
%rename("set_standard_fields") setStandardFields;
%rename("get_standard_fields") getStandardFields;
%rename("add_standard_fields") addStandardFields;
%rename("register_message_validator") registerMessageValidator;
%rename("register_event_callback") registerEventCallback;
%rename("cancel_request") cancelRequest;
%rename("start_auto_dispatch") startAutoDispatch;
%rename("stop_auto_dispatch") stopAutoDispatch;
%rename("exclude_subject") excludeSubject;
%rename("remove_excluded_subject") removeExcludedSubject;
%rename("get_name") getName;
%rename("set_name") setName;
%rename("get_id") getID;
%rename("get_MW_info") getMWInfo;
%rename("get_connection_endpoint") getConnectionEndpoint;
%rename("get_publish_queue_message_count") getPublishQueueMessageCount;
%rename("create_heartbeat_message") createHeartbeatMessage;
%rename("start_heartbeat_service") startHeartbeatService;
%rename("stop_heartbeat_service") stopHeartbeatService;
%rename("set_heartbeat_service_field") setHeartbeatServiceField;
%rename("create_log_message") createLogMessage;
%rename("set_logging_defaults") setLoggingDefaults;
%rename("publish_log") publishLog;
%rename("request_directive") requestDirective;
%rename("acknowledge_request_directive") acknowledgeDirectiveRequest;
%rename("publish_resource_message") publishResourceMessage;
%rename("create_resource_message") createResourceMessage;
%rename("start_resource_message_service") startResourceMessageService;
%rename("stop_resource_message_service") stopResourceMessageService;
%rename("acknowledge_simple_service") acknowledgeSimpleService;
%rename("request_simple_service") requestSimpleService;

%include "dox/ConnectionManager_dox.i"
%include "dox/FieldList_dox.i"
%include "dox/ServiceParamList_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ConnectionManager.h>

%include <std_list.i>

namespace std
{
    %template(FieldList) list<gmsec::api::Field*>;
    %template(ServiceParamList) list<gmsec::api::mist::ServiceParam*>;
}


%extend gmsec::api::mist::ConnectionManager {

    void CALL_TYPE setStandardFields(const std::list<gmsec::api::Field*>& standardFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
            fields.push_back(*it);


        self->setStandardFields(fields);
    }
    const std::list<gmsec::api::Field*> CALL_TYPE getStandardFields()
    {
        gmsec::api::util::DataList<gmsec::api::Field*> standardFields = self->getStandardFields();
        std::list<gmsec::api::Field*> newFields;

        for (gmsec::api::util::DataList<gmsec::api::Field*>::const_iterator it = standardFields.begin(); it != standardFields.end(); ++it)
            newFields.push_back(*it);

        return newFields;
    }
    gmsec::api::Message CALL_TYPE createHeartbeatMessage(const char* subject, const std::list<gmsec::api::Field*>& heartbeatFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;
        
        for(std::list<gmsec::api::Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
            fields.push_back(*it);

        return self->createHeartbeatMessage(subject, fields);
    }

    void CALL_TYPE startHeartbeatService(const char* subject, const std::list<gmsec::api::Field*>& heartbeatFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = heartbeatFields.begin(); it != heartbeatFields.end(); ++it)
            fields.push_back(*it);

        self->startHeartbeatService(subject, fields);
    }
    gmsec::api::Message CALL_TYPE createLogMessage(const char* subject, const std::list<gmsec::api::Field*>& logFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
            fields.push_back(*it);

        return self->createLogMessage(subject, fields);
    }

    void CALL_TYPE setLoggingDefaults(const char* subject, const std::list<gmsec::api::Field*> logFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = logFields.begin(); it != logFields.end(); ++it)
            fields.push_back(*it);

        self->setLoggingDefaults(subject, fields);
    }

    void CALL_TYPE requestDirective(const char* subject, const gmsec::api::Field& directiveString, const std::list<gmsec::api::Field*> dFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = dFields.begin(); it != dFields.end(); ++it)
            fields.push_back(*it);

        self->requestDirective(subject, directiveString, fields);
    }

    gmsec::api::Message* CALL_TYPE requestDirective(const char* subject, const gmsec::api::Field& directiveString, const std::list<gmsec::api::Field*> dFields, int timeout, int republish_ms)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = dFields.begin(); it != dFields.end(); ++it)
            fields.push_back(*it);
        return self->requestDirective(subject, directiveString, fields, timeout, republish_ms);
    }

    void CALL_TYPE acknowledgeDirectiveRequest(const char* subject, const gmsec::api::Message& request, gmsec::api::mist::ResponseStatus::Response ssResponse, const std::list<gmsec::api::Field*>& dFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = dFields.begin(); it != dFields.end(); ++it)
            fields.push_back(*it);

        self->acknowledgeDirectiveRequest(subject, request, ssResponse, fields);
    }

    void CALL_TYPE acknowledgeSimpleService(const char* subject, const gmsec::api::Message& request, gmsec::api::mist::ResponseStatus::Response ssResponse, const std::list<Field*>& sFields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;

        for(std::list<gmsec::api::Field*>::const_iterator it = sFields.begin(); it != sFields.end(); ++it)
            fields.push_back(*it);

        self->acknowledgeSimpleService(subject, request, ssResponse, fields);
    }

    void CALL_TYPE requestSimpleService(const char* subject, const char* opName, const gmsec::api::Field& opNumber, const std::list<gmsec::api::Field*>& sFields, const std::list<gmsec::api::mist::ServiceParam*>& sParams)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;
        gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> params;

        for(std::list<gmsec::api::Field*>::const_iterator it = sFields.begin(); it != sFields.end(); ++it)
            fields.push_back(*it);

        for(std::list<gmsec::api::mist::ServiceParam*>::const_iterator it = sParams.begin(); it != sParams.end(); ++it)
            params.push_back(*it);

        self->requestSimpleService(subject, opName, opNumber, fields, params);
    }

    gmsec::api::Message* CALL_TYPE requestSimpleService(const char* subject, const char* opName, const gmsec::api::Field& opNumber, const std::list<gmsec::api::Field*>& sFields, const std::list<gmsec::api::mist::ServiceParam*>& sParams, int timeout, int republish_ms)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> fields;
        gmsec::api::util::DataList<gmsec::api::mist::ServiceParam*> params;

        for(std::list<gmsec::api::Field*>::const_iterator it = sFields.begin(); it != sFields.end(); ++it)
            fields.push_back(*it);

        for(std::list<gmsec::api::mist::ServiceParam*>::const_iterator it = sParams.begin(); it != sParams.end(); ++it)
            params.push_back(*it);

        return self->requestSimpleService(subject, opName, opNumber, fields, params, timeout, republish_ms);
    }

    void CALL_TYPE unsubscribe(SubscriptionInfo* info)
    {
        self->unsubscribe(info);
    }

    void CALL_TYPE release(Message* message)
    {
        self->release(message);
    }


%pythoncode %{
    @staticmethod
    def shutdown_all_middlewares():
        pass
%}

}


