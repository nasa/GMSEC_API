/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module HeartbeatGenerator

%{
#include <gmsec4/HeartbeatGenerator.h>
using namespace gmsec::api;
%}

/* Methods containing lists that will be redefined */
%ignore gmsec::api::HeartbeatGenerator::HeartbeatGenerator(const Config&, const char*, GMSEC_U16, const gmsec::api::util::DataList<gmsec::api::Field*>&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/HeartbeatGenerator.h>

%extend gmsec::api::HeartbeatGenerator
{
    HeartbeatGenerator(const Config& config, const char* subject, GMSEC_U16 pubRate, const std::list<gmsec::api::Field*>& fields)
    {
        gmsec::api::util::DataList<gmsec::api::Field*> dl_fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it)
        {
            dl_fields.push_back(*it);
        }

        return new HeartbeatGenerator(config, subject, pubRate, dl_fields);
    }
}


%perlcode%{
=pod

=head1 libgmsec_perl::HeartbeatGenerator

This class can be used to start a thread that will continuously publish C2CX HB messages.

The Heartbeat Generator creates a C2CX HB message using MistMessage. It then uses a
ConnectionManager to publish this message on a periodic basis.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html">Config</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="mist/ConnectionManager.html">ConnectionManager</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="mist/message/MistMessage.html">MistMessage</a><br>

=head2 Public Member Subroutines



=head3 new()

C<libgmsec_perl::HeartbeatGenerator-E<gt>new($config, $hbMsgSubject, $hbPubRate)>

        Basic class constructor.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - The Config object to configure the ConnectionManager that is used to publish messages
        $hbMsgSubject - The subject to include with the published heartbeat messages
        $hbPubRate - The message publish rate (in seconds)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A HeartbeatGenerator object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::Exception - Thrown if the message subject is null or contains an empty string.
        $libgmsec_perl::Exception - Thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus.
        $libgmsec_perl::Exception - Thrown if message validation is enabled and a valid C2CX HB message cannot be created.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Notes:</b><br>

        - If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.
        - Consider using libgmsec_perl::MistMessage::setStandardFields() to automatically include standard field(s) within the heartbeat message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="HeartbeatGenerator.html#setField">setField()</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="mist/message/MistMessage.html#setStandardFields">setStandardFields()</a><br>



=head3 new()

C<libgmsec_perl::HeartbeatGenerator-E<gt>new($config, $hbMsgSubject, $hbPubRate, $fields)>

        Class constructor where a list of message fields can be provided.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - The Config object to configure the ConnectionManager that is used to publish messages
        $hbMsgSubject - The subject to include with the published heartbeat messages
        $hbPubRate - The message publish rate (in seconds)
        $fields - The list of fields to add to the heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A HeartbeatGenerator object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::Exception - Thrown if the message subject is null or contains an empty string.
        $libgmsec_perl::Exception - Thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus.
        $libgmsec_perl::Exception - Thrown if message validation is enabled and a valid C2CX HB message cannot be created.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Notes:</b><br>

        - If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.
        - Consider using libgmsec_perl::MistMessage::setStandardFields() to automatically include standard field(s) within the heartbeat message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="HeartbeatGenerator.html#setField">setField()</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="mist/message/MistMessage.html#setStandardFields">setStandardFields()</a><br>



=head3 DESTROY()

C<libgmsec_perl::HeartbeatGenerator-E<gt>DESTROY()>

        Destructor.



=head3 start()

C<libgmsec_perl::HeartbeatGenerator-E<gt>start()>

        Starts the heartbeat generator thread.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the heartbeat generator thread has started; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        If the publish rate is set to 0 (zero), only one heartbeat message will be published, however the heartbeat generator thread will continue to run.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="HeartbeatGenerator.html#stop">stop()</a><br>



=head3 stop()

C<libgmsec_perl::HeartbeatGenerator-E<gt>stop()>

        Stops the heartbeat generator thread.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the heartbeat generator thread has stopped; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="HeartbeatGenerator.html#start">start()</a><br>



=head3 isRunning()

C<libgmsec_perl::HeartbeatGenerator-E<gt>isRunning()>

        Accessor that can be used to query whether the heartbeat generator thread is running.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the thread is running; false otherwise.



=head3 changePublishRate()

C<libgmsec_perl::HeartbeatGenerator-E<gt>changePublishRate($pubRate)>

        Accessor for changing the publish rate of the heartbeat message generator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $pubRate - The new publish rate (in seconds)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        If the publish rate is set to 0 (zero), only one additional heartbeat message will be published.



=head3 setField()

C<libgmsec_perl::HeartbeatGenerator-E<gt>setField($field)>

        Accessor that will add the given field to the heartbeat message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - The field to add to the heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if an existing field was overwritten; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::Exception - Thrown if for whatever reason the field cannot be added to the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated to publish at the (new) rate.



=head3 setField()

C<libgmsec_perl::HeartbeatGenerator-E<gt>setField($fieldName, $fieldValue)>

        Accessor that will smartly add a field to the heartbeat message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $fieldName - The name of the field
        $fieldValue - The value of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if an existing field was overwritten; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::Exception - Thrown if the field name is null or contains an empty string.
        $libgmsec_perl::Exception - Thrown if for whatever reason the field cannot be added to the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        - This method relies onf the usage of MistMessage::setValue().
        - If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated to publish at the (new) rate.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="mist/message/MistMessage.html#setValue">setValue()</a><br>



=cut
%}
