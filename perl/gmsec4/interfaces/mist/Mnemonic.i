/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Mnemonic
%{
#include <gmsec4/mist/Mnemonic.h>
#include <list>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Mnemonic::Mnemonic(const char*, const gmsec::api::util::DataList<gmsec::api::mist::MnemonicSample*>&);
%ignore gmsec::api::mist::Mnemonic::addSample(const MnemonicSample&);

%ignore gmsec::api::mist::Mnemonic::operator=(const Mnemonic&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Mnemonic.h>

%include <std_list.i>
namespace std
{
    %template(MnemonicSampleList) list<gmsec::api::mist::MnemonicSample*>;
}

%extend gmsec::api::mist::Mnemonic {

    Mnemonic(const char* name, const std::list<gmsec::api::mist::MnemonicSample*>& samples)
    {
        gmsec::api::util::DataList<gmsec::api::mist::MnemonicSample*> list;

        for(std::list<gmsec::api::mist::MnemonicSample*>::const_iterator it = samples.begin(); it != samples.end(); ++it)
            list.push_back(*it);
    
        return new Mnemonic(name, list);
    }
    
    void CALL_TYPE addSample(const gmsec::api::mist::MnemonicSample& sample)
    {
        self->addSample(sample);
    }
}

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Mnemonic

This class is a lightweight container for holding information on a Mnemonic, and is used to generate Mnemonic messages by the ConnectionManager Class

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Mnemonic-E<gt>new($name, $samples)>

	Initializes the Mnemonic object with a set of MnemonicSample information

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$name - the name of the Mnemonic
	$samples - a MnemonicSampleList object containing MnemonicSamples to add to the Mnemonic object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if a NULL or empty name parameter is provided

=head3 DESTROY

C<libgmsec_perl::Mnemonic-E<gt>DESTROY()>

    Destructor

=head3 getName

C<libgmsec_perl::Mnemonic-E<gt>getName()>

	Retrieves the name of the Mnemonic

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The name of the Mnemonic

=head3 statusAvailable

C<libgmsec_perl::Mnemonic-E<gt>statusAvailable()>

	Returns the availability of a Mnemonic status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	1 if a Mnemonic status has been set, 0 otherwise

=head3 getStatus

C<libgmsec_perl::Mnemonic-E<gt>getStatus()>

	Accessor for the Mnemonic status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A Field object containing the Mnemonic status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the status field has not been set

=head3 setStatus

C<libgmsec_perl::Mnemonic-E<gt>setStatus($status)>

	Supplies a status for the Mnemonic and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	A Field object containing the status for the Mnemonic

=head3 unitsAvailable

C<libgmsec_perl::Mnemonic-E<gt>unitsAvailable()>

	Returns the availability of a Mnemonic units

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	1 if a Mnemonic units has been set, 0 otherwise

=head3 getUnits

C<libgmsec_perl::Mnemonic-E<gt>getUnits()>

	Accessor for the Mnemonic units

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	the Mnemonic units as a string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the units field has not been set

=head3 setUnits

C<libgmsec_perl::Mnemonic-E<gt>setUnits($units)>

	Supplies a units string for the Mnemonic and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	A string value of the units for the Mnemonic

=head3 getSampleCount

C<libgmsec_perl::Mnemonic-E<gt>getSampleCount()>

	Returns the number of samples in this Mnemonic

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The number of samples contained in the Mnemonic

=head3 getSample

C<libgmsec_perl::Mnemonic-E<gt>getSample($idx)>

	Returns a reference to a MnemonicSample held within this object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$idx - the zero-based index of the MnemonicSample object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to the requested MnemonicSample object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the index supplied is out of bounds

=cut
%}
