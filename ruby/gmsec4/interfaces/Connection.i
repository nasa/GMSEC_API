/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Connection

%{
#include <gmsec4/Connection.h>
using namespace gmsec::api;
%}


/* SWIG doesn't play well with these function headers.
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api::Connection::subscribe(const char*, const Config&, Callback*);
%ignore gmsec::api::Connection::unsubscribe(SubscriptionInfo*&);
%ignore gmsec::api::Connection::publish(const Message&, const Config&);
%ignore gmsec::api::Connection::release(Message*&);
%ignore gmsec::api::Connection::destroy(Connection*&);


/* Ignore C-related methods.
 */
%ignore gmsec::api::Connection::registerEventCallback(ConnectionEvent, GMSEC_EventCallback*);
%ignore gmsec::api::Connection::subscribe(const char*, const Config&, GMSEC_Callback*);
%ignore gmsec::api::Connection::request(const Message&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);
%ignore gmsec::api::Connection::cancelRequest(GMSEC_ReplyCallback*);


/* Crashes occur when used with ActiveMQ; thus do not provide.
 */
%ignore gmsec::api::Connection::shutdownAllMiddlewares();
%ignore gmsec::api::Connection::shutdownMiddleware(const char*);


/* These will not work with Ruby; attempting to call callback
 * from separate thread will smash the stack.
 */
%ignore gmsec::api::Connection::request(const Message&, GMSEC_I32, ReplyCallback*, GMSEC_I32);
%ignore gmsec::api::Connection::cancelRequest(ReplyCallback*);
%ignore gmsec::api::Connection::getPublishQueueMessageCount() const;
%ignore gmsec::api::Connection::startAutoDispatch();
%ignore gmsec::api::Connection::stopAutoDispatch(bool);


%rename("get_api_version") getAPIVersion;
%rename("create") create;
%rename("destroy") destroy;
%rename("connect") connect;
%rename("disconnect") disconnect;
%rename("get_state") getState;
%rename("get_library_root_name") getLibraryRootName;
%rename("get_library_version") getLibraryVersion;
%rename("register_event_callback") registerEventCallback;
%rename("subscribe") subscribe;
%rename("unsubscribe") unsubscribe;
%rename("publish") publish;
%rename("request") request;
%rename("reply") reply;
%rename("dispatch") dispatch;
%rename("receive") receive;
%rename("release") release;
%rename("exclude_subject") excludeSubject;
%rename("remove_excluded_subject") removeExcludedSubject;
%rename("get_name") getName;
%rename("set_name") setName;
%rename("get_id") getID;
%rename("get_mw_info") getMWInfo;
%rename("get_connection_endpoint") getConnectionEndpoint;
/*
%rename("cancel_request") cancelRequest;
%rename("get_publish_queue_message_count") getPublishQueueMessageCount;
%rename("start_auto_dispatch") startAutoDispatch;
%rename("stop_auto_dispatch") stopAutoDispatch;
%rename("shutdown_all_middlewares") shutdownAllMiddlewares;
%rename("shutdown_middleware") shutdownMiddleware;
*/


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Connection.h>


/* Reimplement methods that SWIG cannot handle.
 */
%extend gmsec::api::Connection
{
    gmsec::api::SubscriptionInfo* CALL_TYPE subscribe(const char* topic, Callback* cb = NULL, const Config* config = NULL)
    {
        if (config == NULL)
        {
            return self->subscribe(topic, cb);
        }

        return self->subscribe(topic, *config, cb);
    }

    void CALL_TYPE unsubscribe(SubscriptionInfo* info)
    {
        self->unsubscribe(info);
    }

    void CALL_TYPE publish(const Message& msg, const Config* config = NULL)
    {
        if (config == NULL)
        {
            self->publish(msg);
        }
        else
        {
            self->publish(msg, *config);
        }
    }

    void CALL_TYPE release(Message* message)
    {
        self->release(message);
    }

    static void CALL_TYPE destroy(Connection* conn)
    {
        Connection::destroy(conn);
    }
};

