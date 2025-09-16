/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ResourceGenerator

%{
#include <gmsec5/ResourceGenerator.h>
#include <vector>
using namespace gmsec::api5;
%}

/* Ingore constructors; we will define a create() method */
%ignore gmsec::api5::ResourceGenerator::ResourceGenerator(const Config&, GMSEC_U16, GMSEC_U16, GMSEC_U16);
%ignore gmsec::api5::ResourceGenerator::ResourceGenerator(const Config&, GMSEC_U16, GMSEC_U16, GMSEC_U16, const gmsec::api5::util::List<gmsec::api5::Field*>&);

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/ResourceGenerator.h>

%extend gmsec::api5::ResourceGenerator
{
    static ResourceGenerator* CALL_TYPE create(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval, const std::vector<gmsec::api5::Field*>* fields = NULL)
    {
        ResourceGenerator* rsrcgen = 0;

        if (fields == NULL)
        {
            rsrcgen = new ResourceGenerator(config, pubRate, sampleInterval, averageInterval);
        }
        else
        {
            gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

            for (std::vector<gmsec::api5::Field*>::const_iterator it = fields->begin(); it != fields->end(); ++it)
            {
                list_fields.push_back(*it);
            }

            rsrcgen = new ResourceGenerator(config, pubRate, sampleInterval, averageInterval, list_fields);
        }

        return rsrcgen;
    }

    static void CALL_TYPE destroy(ResourceGenerator* rsrcgen)
    {
        delete rsrcgen;
    }
}


%perlcode%{
=pod

=head1 libgmsec_perl::ResourceGenerator

This class can be used to start a thread that will continuously publish RSRC messages.

The Resource Generator creates a RSRC message. It uses a Connection to publish this message on a periodic basis.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html">Config</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Connection.html">Connection</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Message.html">Message</a><br>


=head2 Public Static Subroutines

=head3 createResourceMessage()

C<libgmsec_perl::ResourceGenerator::createResourceMessage($msgFactory, $sampleInterval, $averageInterval)>

        Creates/returns a Resource Message using the given MessageFactory.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $msgFactory      - The MessageFactory to reference when creating the resource message
        $sampleInterval  - The time distance between sample collection (in seconds)
        $averageInterval - Time window over which to make a moving average of samples (in seconds)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Message object representing a Resource Message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException - Thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval


=head3 create()

C<libgmsec_perl::ResourceGenerator::create($config, $pubRate, $sampleInterval, $averageInterval, $fields = None)>

        Creates an instance of a ResourceGenerator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config          - The Config object to configure the ConnectionManager that is used to publish messages
        $pubRate         - The message publish rate (in seconds)
        $sampleInterval  - The time distance between sample collection (in seconds)
        $averageInterval - The window over which to make a moving average of samples (in seconds)
        $fields          - Optional list of fields to add to the resource message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A ResourceGenerator object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException - Thrown if a Connection object cannot be created.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Notes:</b><br>

        If the publish rate is set to 0 (zero), then the resource generator will only publish one message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ResourceGenerator.html#destroy">destroy()</a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ResourceGenerator.html#setField">setField()</a><br>


=head3 destroy()

C<libgmsec_perl::ResourceGenerator-E<gt>destroy($rsrcgen)>

        Destroys the ResourceGenerator instance.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $rsrcgen - The ResourceGenerator object to destroy.


=head2 Public Member Subroutines

=head3 start()

C<libgmsec_perl::ResourceGenerator-E<gt>start()>

        Starts the resource generator thread.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the resource generator thread has started; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException - Thrown if a connection cannot be established with the GMSEC Bus.
        $libgmsec_perl::GmsecException - Thrown if message validation is enabled and a valid RSRC message cannot be created.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        If the publish rate is set to 0 (zero), only one resource message will be published, however the resource generator thread will continue to run.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ResourceGenerator.html#stop">stop()</a><br>



=head3 stop()

C<libgmsec_perl::ResourceGenerator-E<gt>stop()>

        Stops the resource generator thread.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the resource generator thread has stopped; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ResourceGenerator.html#start">start()</a><br>



=head3 isRunning()

C<libgmsec_perl::ResourceGenerator-E<gt>isRunning()>

        Accessor that can be used to query whether the resource generator thread is running.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if the thread is running; false otherwise.



=head3 setField()

C<libgmsec_perl::ResourceGenerator-E<gt>setField($field)>

        Accessor that will add the given field to the resource message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - The field to add to the resource message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        True if an existing field was overwritten; false otherwise.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException - Thrown if for whatever reason the field cannot be added to the message.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        If the PUB-RATE field is provided, then the publish rate for the resource generator will be updated to publish at the (new) rate.



=cut
%}
