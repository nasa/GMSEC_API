/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Connection

%{
#include <gmsec5/Connection.h>
using namespace gmsec::api5;
%}

/* We'll tell SWIG to ignore them and define our own implementation using %extend */
%ignore gmsec::api5::Connection::Connection(const Config&);
%ignore gmsec::api5::Connection::Connection(const Config&, MessageFactory&);
%ignore gmsec::api5::Connection::unsubscribe(SubscriptionInfo*&);

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Connection.h>

%extend gmsec::api5::Connection {
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

    void CALL_TYPE unsubscribe(SubscriptionInfo* info)
    {
        self->unsubscribe(info);
    }
};

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Connection

=head1 DESCRIPTION

This is the public interface for middleware connections. Each middleware connection implements a class derived from Connection to abstract middleware specific connection details. The application will never access the middleware connection directly, but only through the Connection "interface".

The connection class provides services available on an implemented GMSEC connection. An application can create multiple connection objects with different parameters and different middleware. Creating multiple connections to the same middleware is not prevented but may not be supported by all middleware implementations.

Connection objects make use of a gmsec::util::Mutex by default and are therefore considered thread-safe.

=head2 Public Types and Constants

=head3 Event

=over

=item libgmsec_perl::Connection::DISPATCHER_ERROR_EVENT

Error occurred while attempting to dispatch message.

=item libgmsec_perl::Connection::REQUEST_TIMEOUT_EVENT

A timeout occurred while attempting to receive a reply for a pending request.

=item libgmsec_perl::Connection::CONNECTION_SUCCESSFUL_EVENT

Successfully connected to the middleware server.

=item libgmsec_perl::Connection::CONNECTION_BROKEN_EVENT

Connection to middleware server has been broken.

=item libgmsec_perl::Connection::CONNECTION_RECONNECT_EVENT

An attempt is being made to reconnect to the middleware server.

=item libgmsec_perl::Connection::WSMQ_ASYNC_STATUS_CHECK_EVENT

WebSphere MQ Asynchronous Put status check.

=item libgmsec_perl::Connection::MSG_PUBLISH_FAILURE_EVENT

Failure occured while attempting to publish aggregated message.

=item libgmsec_perl::Connection::INVALID_MESSAGE_EVENT

Message failed validation.

=item libgmsec_perl::Connection::ALL_EVENTS

Monitor all events.

=back

=head2 Public Static Subroutines

=head3 create

C<libgmsec_perl::Connection::create($config, $msgFactory = undef)>

    Creates a Connection instance using the given configuration. If a MessageFactory is provided,
    then it will be associated with the Connection instance.

    When an application requires more than one Connection instance it will be advantageous to rely
    on a single MessageFactory object. This will assist in reducing the memory footprint of the
    application. Do NOT destroy the MessageFactory object while the Connection object(s) are in use!

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config     - a Config object containing middleware ID and connection information.
        $msgFactory - Optional MessageFactory object to apply to the Connection object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Connection object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    $libgmsec_perl::GmsecException if the configuration information in the Config object cannot be used to deduce the middleware type.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#destroy">destroy()</a><br>


=head3 destroy

C<libgmsec_perl::Connection::destroy($conn)>

    Destroys the given Connection instance.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $conn - a Connection object.


=head3 getAPIVersion

C<libgmsec_perl::Connection::getAPIVersion()>

        This function identifies the version of the API.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        String containing API version information.


=head3 shutdownAllMiddlewares

C<libgmsec_perl::Connection::shutdownAllMiddlewares()>

        Calls shutdown routines for each middleware that has a shutdown routine registered.
        This method should only be called once, typically just before an application terminates. 
        Note the calling of this method is optional.


=head3 shutdownMiddleware

C<libgmsec_perl::Connection::shutdownMiddleware($name)>

        Calls the shutdown routine for the middleware with the given name.
        This method should only be called once, typically just before an application terminates. 
        Note the calling of this method is optional.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - A string representing the library name of the GMSEC wrapper for the middleware to shutdown (e.g. gmsec_mb)


=head2 Public Member Subroutines

=head3 connect

C<libgmsec_perl::Connection-E<gt>connect()>

    Establishes this connection to the middleware.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    $libgmsec_perl::GmsecException if an anomaly occurs while connecting to the GMSEC Bus.


=head3 disconnect

C<libgmsec_perl::Connection-E<gt>disconnect()>

    Terminates the connection to the middleware. It is automatically called by the destructor if necessary.


=head3 getLibraryName

C<libgmsec_perl::Connection-E<gt>getLibraryName()>

    Identifies the library name and therefore the connection type that this connection is associated with.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

         Library name


=head3 getLibraryVersion

C<libgmsec_perl::Connection-E<gt>getLibraryVersion()>

    Returns a string containing the version information of the associated middleware of the Connection object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    Library version


=head3 getConfig

C<libgmsec_perl::Connection-E<gt>getConfig()>

    Returns the config object associated with the connection

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

    The returned Config object is for reference purposes only. Changing the config object will not affect the Connection

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A handle to a Config object


=head3 subscribe

C<libgmsec_perl::Connection-E<gt>subscribe($subject, $callback = NULL)>

    Subscribes to a particular subject or pattern.

    Example subscription patterns:

    # this will match only messages with this exact subject 
    my subscriptionInfo = $conn->subscribe("GMSEC.MISSION.CONST.SAT.EVT.MSG");

    # this will match messages with any mission 
    my subscriptionInfo = $conn->subscribe("GMSEC.*.CONST.SAT.EVT.MSG");

    # this will match messages that have AT LEAST ONE MORE TAG 
    # (will not match "GMSEC.MISSION.CONST.SAT")
    my $subscriptionInfo = $conn->subscribe("GMSEC.MISSION.CONST.SAT.>");

    # this will match messages that have ZERO OR MORE TAG 
    # (will match "GMSEC.MISSION.CONST.SAT") 
    my $subscriptionInfo = $conn->subscribe("GMSEC.MISSION.CONST.SAT.+");

    # this will match any event message 
    my $subscriptionInfo = $conn->subscribe("GMSEC.*.*.*.EVT.>");

    # this will match any GMSEC-compliant message, and forward these messages to a callback 
    my $cb = MyCallback->new(); 
    my $subscriptionInfo = $conn->subscribe("GMSEC.>", $cb);

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Although subscription behavior is outlined as above, the actual behavior for a particular middleware implementation MAY slightly deviate from this behavior.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $subject - subject pattern to match received messages
        [optional] $callback - callback to be called when message is received

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        SubscriptionInfo - handle used to cancel or modify subscription. Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe() to free resource.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException on error establishing subscription.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#unsubscribe">unsubscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>

C<libgmsec_perl::Connection-E<gt>subscribe($subject, $config, $callback = NULL)>

    This function subscribes to a particular subject or pattern and uses the provided config object to enable or disable middleware-level subscription functionalities (e.g. ActiveMQ Durable Consumer).

    See libgmsec_perl::Connection::subscribe($subject, $callback) for an explanation of subscription patterns

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $subject - subject pattern to match received messages
        $config - config object to be used for subscription operation
        [optional] $callback - callback to be called when message is received

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        SubscriptionInfo - handle used to cancel or modify subscription. Connection maintains ownership of SubscriptionInfo; user should not delete but instead call unsubscribe() to free resource.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException on error establishing subscription.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#unsubscribe">unsubscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>


=head3 unsubscribe

C<libgmsec_perl::Connection-E<gt>unsubscribe($subscriptionInfo)>

    This function unsubscribes to a particular subject pattern, and will stop the reception of messages that match this pattern. It will also remove the registration of any callbacks with this subject pattern.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $subscriptionInfo - SubscriptionInfo handle returned by calling subscribe()

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException if SubscriptionInfo object originated from a different ConnectionManager
        $libgmsec_perl::GmsecException if error occurs at the middleware level

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>


=head3 publish

C<libgmsec_perl::Connection-E<gt>publish($message)>

        This function will publish a message to the middleware.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The actual Message published to the middleware will contain tracking fields; to disable this feature, create a Connection object with the tracking=off configuration option.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $message - message to be published

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException if error occurs while attempting to publish the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>

C<libgmsec_perl::Connection-E<gt>publish($message, $mwConfig)>

        Publishes the given message to the middleware using the given configuration to enable or disable certain middleware-level publish functionalities (e.g. ActiveMQ - Durable Producer).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The actual Message published to the middleware will contain tracking fields; to disable this feature, create a Connection object with the tracking=off configuration option.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $message - message to be published
        $mwConfig - config object for providing middleware configuration options

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException if error occurs while attempting to publish the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>


=head3 request

C<libgmsec_perl::Connection-E<gt>request($message, $timeout, $callback, $republish_ms = 0)>

        This function will send a request asyncronously. The callback will be called for the reply if it is received within the specified timeout. This function will not block. The timeout value is expressed in milliseconds.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The actual Message published to the middleware will contain tracking fields; to disable this feature, create a Connection object with the tracking=off configuration option.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $message - request message to be sent
        $timeout - maximum time to wait for reply (in milliseconds)
        $callback - ReplyCallback to invoke when reply is received
        $republish_ms - request message resubmission interval (in milliseconds). If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0, the period will default to 60000ms, unless the user has provided an alternate time period via the Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException on error with generating async request, or if ReplyCallback is NULL.


C<libgmsec_perl::Connection-E<gt>request($message, $timeout, $republish_ms = 0)>

        This function will send a request asyncronously. The callback will be called for the reply if it is received within the specified timeout. This function will not block. The timeout value is expressed in milliseconds.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The actual Message published to the middleware will contain tracking fields; to disable this feature, create a Connection object with the tracking=off configuration option.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $message - request message to be sent
        $timeout - maximum time to wait for reply (in milliseconds)
        $republish_ms - request message resubmission interval (in milliseconds). If set to a negative value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0, the period will default to 60000ms, unless the user has provided an alternate time period via the Config object used to create the Connection object.  The minimum republish period allowed is 100ms.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Reply Message, ir NULL if no reply is received in time

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException on error transmitting request message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Message.html#destroy">Message::destroy()</a><br>


=head3 reply

C<libgmsec_perl::Connection-E<gt>reply($request, $reply)>

        Send a reply message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The actual Message published to the middleware will contain tracking fields; to disable this feature, create a Connection object with the tracking=off configuration option.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $request - the request message that was received, and to which a response is being sent
        $reply   - the reply message to be sent

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException on error transmitting message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#receive">receive()</a><br>


=head3 receive

C<libgmsec_perl::Connection-E<gt>receive($timeout = -1)>

        This function returns the next message received within the specified timeout. The received messages are determined by the message subscriptions set up with the subscribe() function(s).

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $timeout - the maximum time (in milliseconds) to block waiting for a message; if -1 is specified for the timeout, then the call will block indefinitely.  Defaults to a value of -1 if not specified.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A pointer to the next available Message, or NULL if a timeout occurs.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException on error receiving message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#subscribe">subscribe()</a><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Message.html#destroy">Message::destroy()</a><br>


=head3 excludeSubject

C<libgmsec_perl::Connection-E<gt>excludeSubject($subject)>

        Exclude any incoming messages with the specified subject.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $subject - the subject pattern to look for in incoming messages

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException if the given subject is non-compliant.


=head3 removeExcludedSubject

C<libgmsec_perl::Connection-E<gt>removeExcludedSubject($subject)>

        Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $subject - the subject pattern to remove

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if not connected to the GMSEC Bus.
        $libgmsec_perl::GmsecException if the given subject is non-compliant.


=head3 getName

C<libgmsec_perl::Connection-E<gt>getName()>

        Returns the name of this connection, automatically generated or user specified

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A string representing the name of this Connection

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html#setName">setName()</a><br>


=head3 setName

C<libgmsec_perl::Connection-E<gt>setName($name)>

        Set the logical name of this connection. This can be used for Identifying connections withing a client program. If a name is not given, one will be automatically generated.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name to give to this connection


=head3 getID

C<libgmsec_perl::Connection-E<gt>getID()>

        Get the string ID for this connection.


=head3 getMWInfo

C<libgmsec_perl::Connection-E<gt>getMWInfo()>

        Returns a string containing middleware information.


=head3 getConnectionEndpoint

C<libgmsec_perl::Connection-E<gt>getConnectionEndpoint()>

        Returns middleware broker connection information.


=head3 getPublishQueueMessageCount

C<libgmsec_perl::Connection-E<gt>getPublishQueueMessageCount()>

        Retrieves the number of messages queued for asynchronous publish operations.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The number of messages in the publish queue

=cut
%}
