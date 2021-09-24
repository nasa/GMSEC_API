/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFileIterator
%{
#include <gmsec4/ConfigFileIterator.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::ConfigFileIterator::nextConfig() const;
%ignore gmsec::api::ConfigFileIterator::nextMessage() const;
%ignore gmsec::api::ConfigFileIterator::nextSubscription() const;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ConfigFileIterator.h>

// Note: %extend places new additions into public member data
%extend gmsec::api::ConfigFileIterator {
	const ConfigEntry& CALL_TYPE nextConfig()
	{
		return self->nextConfig();
	}

	const MessageEntry& CALL_TYPE nextMessage()
	{
		return self->nextMessage();
	}

	const SubscriptionEntry& CALL_TYPE nextSubscription()
	{
		return self->nextSubscription();
	}
};

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ConfigFileIterator

=head1 DESCRIPTION

This class is used to access elements of a configuration file (ConfigFile) object.

=head2 Public Member Subroutines

=head3 hasNextConfig

C<libgmsec_perl::ConfigFileIterator-E<gt>hasNextConfig()>

        Determines if there is a next Config in the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if there is a next value, 0 (false) otherwise

=head3 hasNextMessage

C<libgmsec_perl::ConfigFileIterator-E<gt>hasNextMessage()>

        Determines if there is a next Message in the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if there is a next value, 0 (false) otherwise

=head3 hasNextSubscription

C<libgmsec_perl::ConfigFileIterator-E<gt>hasNextSubscription()>

        Determines if there is a next Subscription in the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if there is a next value, 0 (false) otherwise

=head3 hasNextCustomElement

C<libgmsec_perl::ConfigFileIterator-E<gt>hasNextCustomElement()>

        Determines if there is another custom XML element in the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if there is a next value, 0 (false) otherwise

=head3 nextConfig

C<libgmsec_perl::ConfigFileIterator-E<gt>nextConfig()>

        Returns the next ConfigEntry.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The next ConfigEntry

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception is thrown if no more config entries are available.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConfigFileIterator.html#hasnextconfig">hasNextConfig()</a><br>

=head3 nextMessage

C<libgmsec_perl::ConfigFileIterator-E<gt>nextMessage()>

        Returns the next MessageEntry.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The next MessageEntry

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception is thrown if no more message entries are available.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConfigFileIterator.html#hasnextmessage">hasNextMessage()</a><br>

=head3 nextSubscription

C<libgmsec_perl::ConfigFileIterator-E<gt>nextSubscription()>

        Returns the next SubscriptionEntry.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The next SubscriptioEntry

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception is thrown if no more subscription entries are available.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConfigFileIterator.html#hasnextsubscription">hasNextSubscription()</a><br>

=head3 nextCustomElement

C<libgmsec_perl::ConfigFileIterator-E<gt>nextCustomElement()>

        Returns the next custom XML element.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        The returned string is volatile; the callee should consider making a copy of the returned string. For example:

        eval
        {
            my $configFile = libgmsec_perl::ConfigFile->new();

            $configFile.load("somefile.xml");

            my $iter = $configFile->getIterator();

            my $customElement = "";
            if ($iter.hasNextCustomElement()
            {
                $customElement = $iter.nextCustomElement(); # Creates a copy of custom element
            }

            if ($customElement ne "")
            {
                libgmsec_perl::LogInfo("Custom XML element: $customElement");
            }
        };
        if ($@)
        {
            # Handle error loading ConfigFile
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The next SubscriptioEntry

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception is thrown if no more subscription entries are available.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ConfigFileIterator.html#hasnextcustomelement">hasNextCustomElement()</a><br>

=head3 reset

C<libgmsec_perl::ConfigFileIterator-E<gt>reset()>

        Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFile.

=cut
%}
