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


/* SWIG doesn't play well with these function headers
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api::Connection::unsubscribe(SubscriptionInfo*&);
%ignore gmsec::api::Connection::release(Message*&);
%ignore gmsec::api::Connection::destroy(Connection*&);
%ignore gmsec::api::Connection::shutdownAllMiddlewares();
%ignore gmsec::api::Connection::shutdownMiddleware(const char*);

/* Ignore C-binding methods */
%ignore gmsec::api::Connection::registerEventCallback(ConnectionEvent, GMSEC_EventCallback*);
%ignore gmsec::api::Connection::subscribe(const char*, const Config&, GMSEC_Callback*);
%ignore gmsec::api::Connection::request(const Message&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);
%ignore gmsec::api::Connection::cancelRequest(GMSEC_ReplyCallback*);

%rename("get_API_version") getAPIVersion;
%rename("get_state") getState;
%rename("get_library_root_name") getLibraryRootName;
%rename("get_library_version") getLibraryVersion;
%rename("register_event_callback") registerEventCallback;
%rename("start_auto_dispatch") startAutoDispatch;
%rename("stop_auto_dispatch") stopAutoDispatch;
%rename("cancel_request") cancelRequest;
%rename("exclude_subject") excludeSubject;
%rename("remove_excluded_subject") removeExcludedSubject;
%rename("get_name") getName;
%rename("set_name") setName;
%rename("get_id") getID;
%rename("get_MW_info") getMWInfo;
%rename("get_connection_endpoint") getConnectionEndpoint;
%rename("get_publish_queue_message_count") getPublishQueueMessageCount;


%include "dox/Connection_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Connection.h>


// Note: %extend places new additions into public member data
%extend gmsec::api::Connection {
        void CALL_TYPE unsubscribe(SubscriptionInfo* info)
        {
                self->unsubscribe(info);
        }

        void CALL_TYPE release(Message* message)
        {
                self->release(message);
        }

        static void destroy(Connection* conn)
        {
                Connection::destroy(conn);
        }

%pythoncode %{
    @staticmethod
    def shutdown_all_middlewares():
        pass

    @staticmethod
    def shutdown_middleware(name):
        pass
%}

};
