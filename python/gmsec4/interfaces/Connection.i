/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Connection

%{
#include <gmsec4/Connection.h>
using namespace gmsec::api;
%}


// SWIG doesn't play well with these function headers
// We'll tell SWIG to ignore them and define our own implementation using %extend
%ignore gmsec::api::Connection::unsubscribe(SubscriptionInfo*&);
%ignore gmsec::api::Connection::release(Message*&);
%ignore gmsec::api::Connection::destroy(Connection*&);

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

        static void CALL_TYPE destroy(Connection* conn)
        {
                Connection::destroy(conn);
        }
};

