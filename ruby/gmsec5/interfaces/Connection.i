/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Connection

%{
#include <gmsec5/Connection.h>
using namespace gmsec::api5;
%}


/* SWIG doesn't play well with these function headers.
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api5::Connection::Connection(const Config&);
%ignore gmsec::api5::Connection::Connection(const Config&, MessageFactory&);
%ignore gmsec::api5::Connection::subscribe(const char*, const Config&, Callback*);
%ignore gmsec::api5::Connection::unsubscribe(SubscriptionInfo*&);
%ignore gmsec::api5::Connection::publish(const Message&, const Config&);


/* Ignore C-related methods. */
%ignore gmsec::api5::Connection::registerEventCallback(Event, GMSEC_EventCallback*);
%ignore gmsec::api5::Connection::subscribe(const char*, const Config&, GMSEC_Callback*);
%ignore gmsec::api5::Connection::request(const Message&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);


/* Crashes occur when used with ActiveMQ; thus do not provide. */
%ignore gmsec::api5::Connection::shutdownAllMiddlewares();
%ignore gmsec::api5::Connection::shutdownMiddleware(const char*);


/* These will not work with Ruby; attempting to call callback
 * from separate thread will smash the stack.
 */
%ignore gmsec::api5::Connection::request(const Message&, GMSEC_I32, ReplyCallback*, GMSEC_I32);
%ignore gmsec::api5::Connection::getPublishQueueMessageCount() const;
%ignore gmsec::api5::Connection::startAutoDispatch();
%ignore gmsec::api5::Connection::stopAutoDispatch(bool);


%rename("get_api_version") getAPIVersion;
%rename("get_library_name") getLibraryName;
%rename("get_library_version") getLibraryVersion;
%rename("get_config") getConfig;
%rename("get_message_factory") getMessageFactory;
%rename("register_event_callback") registerEventCallback;
%rename("connect") connect;
%rename("disconnect") disconnect;
%rename("subscribe") subscribe;
%rename("unsubscribe") unsubscribe;
%rename("publish") publish;
%rename("request") request;
%rename("reply") reply;
%rename("dispatch") dispatch;
%rename("receive") receive;
%rename("exclude_subject") excludeSubject;
%rename("remove_excluded_subject") removeExcludedSubject;
%rename("get_name") getName;
%rename("set_name") setName;
%rename("get_id") getID;
%rename("get_mw_info") getMWInfo;
%rename("get_connection_endpoint") getConnectionEndpoint;

/* asynchronous operations not supported
%rename("start_auto_dispatch") startAutoDispatch;
%rename("stop_auto_dispatch") stopAutoDispatch;
%rename("get_publish_queue_message_count") getPublishQueueMessageCount;
%rename("shutdown_all_middlewares") shutdownAllMiddlewares;
%rename("shutdown_middleware") shutdownMiddleware;
*/


/* Track objects */
%trackobjects;

/* Specify custom mark function */
%freefunc Connection "free_Connection";


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Connection.h>


%extend gmsec::api5::Connection
{
    static Connection* CALL_TYPE create(const Config* config, MessageFactory* factory = NULL)
    {
        if (factory == NULL) {
            return new Connection(*config);
        }
        return new Connection(*config, *factory);
    }

    static void CALL_TYPE destroy(Connection* conn)
    {
        delete conn;
    }

    gmsec::api5::SubscriptionInfo* CALL_TYPE subscribe(const char* topic, Callback* cb = NULL, const Config* config = NULL)
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
};


%header %{
    static void free_Connection(void* ptr)
    {
        /* Do nothing; apps should use destroy() method. */
    }
%}
