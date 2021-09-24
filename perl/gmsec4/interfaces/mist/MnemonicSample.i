/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MnemonicSample
%{
#include <gmsec4/mist/MnemonicSample.h>
using namespace gmsec::api::mist;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MnemonicSample.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::MnemonicSample

=head2 Public Types and Constants

=head3 LimitFlag

=over

=item libgmsec_perl::MnemonicSample::UNSPECIFIED

No limit Specified

=item libgmsec_perl::MnemonicSample::NO_LIMIT_VIOLATION

No violation

=item libgmsec_perl::MnemonicSample::RED_LOW

Lower red-line violation

=item libgmsec_perl::MnemonicSample::YELLOW_LOW

Lower yellow-line ciolation

=item libgmsec_perl::MnemonicSample::YELLOW-HIGH

Upper yellow line violation

=item libgmsec_perl::MnemonicSample::RED_HIGH

Upper red-line violation

=back

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::MnemonicSample-E<gt>new($timestamp, $rawValue)>

	Initializes the MnemonicSample object with a timestamp and value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$timestamp - A string representation of the time at which the sampled value was taken
	$rawValue - A Field object containing the raw value of the parameter

=head3 DESTROY

C<libgmsec_perl::MnemonicSample-E<gt>DESTROY()>

    Destructor

=head3 getTimestamp

C<libgmsec_perl::Device-E<gt>getTimestamp()>

    Retrieves the timestamp of the MnemonicSample

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The timestamp of the MnemonicSample as a string

=head3 getRawValue

C<libgmsec_perl::Device-E<gt>getRawValue()>

    Retrieves the value of the MnemonicSample

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A field object containing the value of the MnemonicSample

=head3 hasEUValue

C<libgmsec_perl::MnemonicSample-E<gt>hasEUValue()>

    Checks the availability of the MnemonicSample value in engineering units

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the EU value has been set, 0 otherwise

=head3 getEUValue

C<libgmsec_perl::MnemonicSample-E<gt>getEUValue()>

    Accessor for the MnemonicSample EU Value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    a Field object containing the MnemonicSample EU value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the EU value field has not been set

=head3 setEUValue

C<libgmsec_perl::MnemonicSample-E<gt>setEUValue($value)>

    Supplies a field for the MnemonicSample EU value and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $value - a Field object containing the EU value

=head3 hasTextValue

C<libgmsec_perl::MnemonicSample-E<gt>hasTextValue()>

    Checks the availability of the MnemonicSample text value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the text value has been set, 0 otherwise

=head3 getTextValue

C<libgmsec_perl::MnemonicSample-E<gt>getTextValue()>

    Accessor for the MnemonicSample Text Value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    a Field object containing the MnemonicSample Text value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the Text value field has not been set

=head3 setTextValue

C<libgmsec_perl::MnemonicSample-E<gt>setTextValue($value)>

    Supplies a string for the MnemonicSample text value and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $value - a text string

=head3 hasFlags

C<libgmsec_perl::MnemonicSample-E<gt>hasFlags()>

    Checks the availability of the MnemonicSample flags field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the flags value has been set, 0 otherwise

=head3 getFlags

C<libgmsec_perl::MnemonicSample-E<gt>getFlags()>

    Accessor for the MnemonicSample flags Value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    the MnemonicSample flags value

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the EU value field has not been set

=head3 setFlags

C<libgmsec_perl::MnemonicSample-E<gt>setFlags($value)>

    Supplies a number for the MnemonicSample flags and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $value - the flag value for the MnemonicSample

=head3 hasLimitEnableDisable

C<libgmsec_perl::MnemonicSample-E<gt>hasLimitEnableDisable()>

    Checks the availability of the MnemonicSample limit enable/disable flag

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the limit enable/disable flag field is available, 0 otherwise

=head3 getLimitEnableDisable

C<libgmsec_perl::MnemonicSample-E<gt>getLimitEnableDisable()>

    Accessor for the MnemonicSample limit enable/disable flag

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if MnemonicSample limits are enabled, 0 otherwise

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the limit enable/disable flag field is not available

=head3 setLimitEnableDisable

C<libgmsec_perl::MnemonicSample-E<gt>setLimitEnableDisable($value)>

    Supplies a boolean flag for the MnemonicSample limit enable/disable flag

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $value - a boolean value of 1 or 0 to enable/disable limits for the MnemonicSample

=head3 hasLimit

C<libgmsec_perl::MnemonicSample-E<gt>hasLimit()>

    Checks the availability of the MnemonicSample limit

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the limit has been set, 0 otherwise

=head3 getLimit

C<libgmsec_perl::MnemonicSample-E<gt>getEUValue()>

    Accessor for the MnemonicSample Limit

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    a LimitFlag enumeration of the MnemonicSample limit

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the limit has not been set

=head3 setLimit

C<libgmsec_perl::MnemonicSample-E<gt>setlimit($limitFlag)>

    Supplies a field for the MnemonicSample limit and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $limitFlag - a LimitFlag enumeration of the MnemonicSample limit

=head3 hasStalenessStatus

C<libgmsec_perl::MnemonicSample-E<gt>hasStalenessStatus()>

    Checks the availability of the MnemonicSample staleness status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the staleness field is available, 0 otherwise

=head3 getStalenessStatus

C<libgmsec_perl::MnemonicSample-E<gt>getStalenessStatus()>

    Accessor for the MnemonicSample staleness status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the MnemonicSample staleness flag has been set, 0 otherwise

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the staleness field is not available

=head3 setStalenessStatus

C<libgmsec_perl::MnemonicSample-E<gt>setStalenessStatus($value)>

    Supplies a boolean value for the MnemonicSample staleness status and makes a copy of the field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $value - a boolean value of 1 or 0 to set/unset the MnemonicSample staleness status

=head3 hasQuality

C<libgmsec_perl::MnemonicSample-E<gt>hasQuality()>

    Checks the availability of the MnemonicSample quality flag field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the quality flag field is available has been set, 0 otherwise

=head3 getQuality

C<libgmsec_perl::MnemonicSample-E<gt>getQuality()>

    Accessor for the MnemonicSample quality flag

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the quality flag is set, 0 otherwise

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the quality flag field is not available

=head3 setEUValue

C<libgmsec_perl::MnemonicSample-E<gt>setQuality($quality)>

    Supplies a boolean value for the MnemonicSample quality flag

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $value - a boolean value of 1 or 0 to set/unset the quality flag

=cut
%}
