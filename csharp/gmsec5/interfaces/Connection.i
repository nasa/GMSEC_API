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

/* Ignore methods pertinent to the C binding */
%ignore gmsec::api5::Connection::registerEventCallback(Event, GMSEC_EventCallback*);
%ignore gmsec::api5::Connection::subscribe(const char*, const Config&, GMSEC_Callback*);
%ignore gmsec::api5::Connection::request(const Message&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);

%rename("GetAPIVersion") getAPIVersion;
%rename("Connect") connect;
%rename("Disconnect") disconnect;
%rename("GetLibraryName") getLibraryName;
%rename("GetLibraryVersion") getLibraryVersion;
%rename("GetMessageFactory") getMessageFactory;
%rename("RegisterEventCallback") registerEventCallback;
%rename("Subscribe") subscribe;
%rename("Unsubscribe") unsubscribe;
%rename("StartAutoDispatch") startAutoDispatch;
%rename("StopAutoDispatch") stopAutoDispatch;
%rename("Publish") publish;
%rename("Request") request;
%rename("Reply") reply;
%rename("Dispatch") dispatch;
%rename("Receive") receive;
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

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Connection.h>


/* Note: %extend places new additions into public member data */
%extend gmsec::api5::Connection {
    void CALL_TYPE unsubscribe(SubscriptionInfo* info)
    {
        self->unsubscribe(info);
    }
};

