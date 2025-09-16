/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Connection

%{
#include <gmsec5/Connection.h>
#include <gmsec5/Config.h>
#include <gmsec5/SubscriptionInfo.h>
using namespace gmsec::api5;
%}


/* SWIG doesn't play well with these function headers
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api5::Connection::Connection(const Config&);
%ignore gmsec::api5::Connection::Connection(const Config&, MessageFactory&);
%ignore gmsec::api5::Connection::unsubscribe(SubscriptionInfo*&);

%ignore gmsec::api5::Connection::shutdownAllMiddlewares();
%ignore gmsec::api5::Connection::shutdownMiddleware(const char*);

/* Ignore methods that are not supported or are pertinent to the C binding */
%ignore gmsec::api5::Connection::registerEventCallback(Connection::Event, EventCallback*);
%ignore gmsec::api5::Connection::subscribe;
%ignore gmsec::api5::Connection::subscribe(char const*, Callback*);
%ignore gmsec::api5::Connection::subscribe(char const*, Config const&, Callback*);
%ignore gmsec::api5::Connection::request(Message const&, GMSEC_I32, ReplyCallback*, GMSEC_I32);
%ignore gmsec::api5::Connection::registerEventCallback(Event, GMSEC_EventCallback*);
%ignore gmsec::api5::Connection::subscribe(char const*, Config const&, GMSEC_Callback*);
%ignore gmsec::api5::Connection::request(Message const&, GMSEC_I32, GMSEC_ReplyCallback*, GMSEC_EventCallback*, GMSEC_I32);
%rename ("$ignore", fullname=1) gmsec::api5::Connection::publish(const Message&);
%rename ("$ignore", fullname=1) gmsec::api5::Connection::publish(const Message&, const Config&);
%ignore gmsec::api5::Connection::receive(GMSEC_I32 timeout = -1);

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Connection.h>
%include <gmsec5/SubscriptionInfo.h>


%extend gmsec::api5::Connection
{

    Connection(const Config* config, MessageFactory* factory = NULL)
    {
        GMSEC_WARNING << "This constructor is deprecated; use gmsec.Connection.create() instead";
        if (factory == NULL) {
            return new Connection(*config);
        }
        return new Connection(*config, *factory);
    }

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

    SubscriptionInfo* CALL_TYPE setupSubscription(char const* subject)
    {
        return self->subscribe(subject);
    }

    SubscriptionInfo* CALL_TYPE setupSubscriptionWithConfig(char const* subject, Config const* config)
    {
        return self->subscribe(subject, *config);
    }

    void CALL_TYPE unsubscribe(SubscriptionInfo* info)
    {
        self->unsubscribe(info);
    }

    void CALL_TYPE publish(gmsec::api5::Message* msg)
    {
        self->publish(*msg);
    }

    void CALL_TYPE publishWithConfig(Message const* msg, Config const* config)
    {
        self->publish(*msg, *config);
    }

    Message* CALL_TYPE receive(long timeout)
    {
        return self->receive(static_cast<GMSEC_I32>(timeout));
    }
};

