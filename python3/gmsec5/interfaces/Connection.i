/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Connection

%{
#include <gmsec5/Connection.h>
using namespace gmsec::api5;
%}


/* SWIG doesn't play well with these function headers
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api5::Connection::unsubscribe(SubscriptionInfo*&);

%ignore gmsec::api5::Connection::shutdownAllMiddlewares();
%ignore gmsec::api5::Connection::shutdownMiddleware(const char*);

/* Ignore C-binding methods */
%ignore gmsec::api5::Connection::registerEventCallback(Event, GMSEC_EventCallback*);
%ignore gmsec::api5::Connection::subscribe(const char*, const Config&, GMSEC_Callback*);
%ignore gmsec::api5::Connection::request(const Message&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);

%rename("get_api_version") getAPIVersion;
%rename("get_library_name") getLibraryName;
%rename("get_library_version") getLibraryVersion;
%rename("get_message_factory") getMessageFactory;
%rename("register_event_callback") registerEventCallback;
%rename("start_auto_dispatch") startAutoDispatch;
%rename("stop_auto_dispatch") stopAutoDispatch;
%rename("exclude_subject") excludeSubject;
%rename("remove_excluded_subject") removeExcludedSubject;
%rename("get_name") getName;
%rename("set_name") setName;
%rename("get_id") getID;
%rename("get_mw_info") getMWInfo;
%rename("get_connection_endpoint") getConnectionEndpoint;
%rename("get_publish_queue_message_count") getPublishQueueMessageCount;


%include "dox/Connection_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Connection.h>


// Note: %extend places new additions into public member data
%extend gmsec::api5::Connection {
        void CALL_TYPE unsubscribe(SubscriptionInfo* info)
        {
                self->unsubscribe(info);
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
