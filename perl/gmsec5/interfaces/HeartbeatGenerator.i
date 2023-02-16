/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module HeartbeatGenerator

%{
#include <gmsec5/HeartbeatGenerator.h>
#include <vector>
using namespace gmsec::api5;
%}

/* Ignore constructors; we will define a create() method */
%ignore gmsec::api5::HeartbeatGenerator::HeartbeatGenerator(const Config&, GMSEC_U16);
%ignore gmsec::api5::HeartbeatGenerator::HeartbeatGenerator(const Config&, GMSEC_U16, const gmsec::api5::util::List<gmsec::api5::Field*>&);

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/HeartbeatGenerator.h>

%extend gmsec::api5::HeartbeatGenerator
{
    static HeartbeatGenerator* CALL_TYPE create(const Config& config, GMSEC_U16 pubRate, const std::vector<gmsec::api5::Field*>* fields = NULL)
    {
        HeartbeatGenerator* hbgen = 0;

        if (fields == NULL)
        {
            hbgen = new HeartbeatGenerator(config, pubRate);
        }
        else
        {
            gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

            for (std::vector<gmsec::api5::Field*>::const_iterator it = fields->begin(); it != fields->end(); ++it)
            {
                list_fields.push_back(*it);
            }

            hbgen = new HeartbeatGenerator(config, pubRate, list_fields);
        }

        return hbgen;
    }

    static void CALL_TYPE destroy(HeartbeatGenerator* hbgen)
    {
        delete hbgen;
    }
}


%perlcode%{
=pod

=head1 libgmsec_perl::HeartbeatGenerator

This class can be used to start a thread that will continuously publish HB messages.

The Heartbeat Generator creates a HB message. It uses a Connection to publish this message on a periodic basis.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html">Config</a><br>


=head2 Public Static Subroutines

=head3 create()

C<libgmsec_perl::HeartbeatGenerator::create($config, $hbPubRate, $fields = None)>

        Creates an instance of a HeartbeatGenerator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - The Config object to configure the Connection object that is used to publish messages
        $hbPubRate - The message publish rate (in seconds)
        $fields - Optional list of fields to add to the heartbeat message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A HeartbeatGenerator object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException - Thrown if a Connection object cannot be created.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Notes:</b><br>

        If the publish rate is set to 0 (zero), then the heartbeat generator will only publish one message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="HeartbeatGenerator.html#destroy">destroy()</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="HeartbeatGenerator.html#setField">setField()</a><br>


=head3 destroy()

C<libgmsec_perl::HeartbeatGenerator::destroy($hbgen)>

        Destroys a HeartbeatGenerator instance.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $hbgen - The HeartbeatGenerator object to destroy.



=head2 Public Member Subroutines

=head3 start()

C<libgmsec_perl::HeartbeatGenerator-E<gt>start()>

        Starts the heartbeat generator thread.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the heartbeat generator thread has started; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException - Thrown if a connection cannot be established with the GMSEC Bus.
        $libgmsec_perl::GmsecException - Thrown if message validation is enabled and a valid HB message cannot be created.

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

        $libgmsec_perl::GmsecException - Thrown if for whatever reason the field cannot be added to the message.

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

        $libgmsec_perl::GmsecException - Thrown if the field name is null or contains an empty string.
        $libgmsec_perl::GmsecException - Thrown if for whatever reason the field cannot be added to the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will be updated to publish at the (new) rate.

=cut
%}
