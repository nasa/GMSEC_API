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
%ignore gmsec::api::Connection::registerEventCallback(ConnectionEvent, GMSEC_EventCallback*);
%ignore gmsec::api::Connection::subscribe(const char*, const Config&, GMSEC_Callback*);
%ignore gmsec::api::Connection::request(const Message&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);
%ignore gmsec::api::Connection::cancelRequest(GMSEC_ReplyCallback*);

%ignore gmsec::api::Connection::shutdownAllMiddlewares();
%ignore gmsec::api::Connection::shutdownMiddleware(const char*);

%rename("GetAPIVersion") getAPIVersion;
%rename("Create") create;
%rename("Destroy") destroy;
%rename("Connect") connect;
%rename("Disconnect") disconnect;
%rename("GetState") getState;
%rename("GetLibraryRootName") getLibraryRootName;
%rename("GetLibraryVersion") getLibraryVersion;
%rename("RegisterEventCallback") registerEventCallback;
%rename("Subscribe") subscribe;
%rename("Unsubscribe") unsubscribe;
%rename("StartAutoDispatch") startAutoDispatch;
%rename("StopAutoDispatch") stopAutoDispatch;
%rename("Publish") publish;
%rename("Request") request;
%rename("CancelRequest") cancelRequest;
%rename("Reply") reply;
%rename("Dispatch") dispatch;
%rename("Receive") receive;
%rename("Release") release;
%rename("ExcludeSubject") excludeSubject;
%rename("RemoveExcludedSubject") removeExcludedSubject;
%rename("GetName") getName;
%rename("SetName") setName;
%rename("GetID") getID;
%rename("GetMWInfo") getMWInfo;
%rename("GetConnectionEndpoint") getConnectionEndpoint;
%rename("GetPublishQueueMessageCount") getPublishQueueMessageCount;
/*
%rename("ShutdownAllMiddlewares") shutdownAllMiddlewares;
%rename("ShutdownMiddleware") shutdownMiddleware;
*/

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Connection.h>


/* Note: %extend places new additions into public member data */
%extend gmsec::api::Connection {
        void CALL_TYPE unsubscribe(SubscriptionInfo* info)
        {
                self->unsubscribe(info);
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

