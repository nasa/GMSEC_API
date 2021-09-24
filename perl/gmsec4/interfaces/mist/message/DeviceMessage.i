/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module DeviceMessage
%{
#include <gmsec4/mist/message/DeviceMessage.h>
using namespace gmsec::api::mist::message;
%}

%ignore gmsec::api::mist::message::DeviceMessage::operator=(const DeviceMessage&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/DeviceMessage.h>


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::DeviceMessage

	A MistMessage object that is capable of storing Device objects.  

	The Following message schema IDs and their templates are supported:
	MSG.DEV

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::DeviceMessage-E<gt>new($subject, $spec)>

	Initializes the message instance and automatically builds the appropriate schemaID based on the version of Specification used

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - The subject string of the message
	$spec - A reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID

C<libgmsec_perl::DeviceMessage-E<gt>new($subject, $config, $spec)>

	Initializes the message instance with an associated Configuration object and automatically builds the appropriate schemaID based on the version of Specification used

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - The subject string of the message
	$config - A Configuration to associate with the message
	$spec - A reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID


C<libgmsec_perl::DeviceMessage-E<gt>new($spec, $data)>

    Initializes a DeviceMessage using an XML or JSON string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $spec - A reference to the specification to which this message will adhere
    $data - XML or JSON string used to initialize the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the given data does not represent a valid XML or JSON statement.


=head3 DESTROY

C<libgmsec_perl::DeviceMessage-E<gt>DESTROY()>

    Destructor

=head3 addDevice

C<libgmsec_perl::DeviceMessage-E<gt>addDevice($device)>

	Adda a device to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$device - a Device object to attach to the message

=head3 getNumDevices

C<libgmsec_perl::DeviceMessage-E<gt>getNumDevices()>

	Gets the number of devices in the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The number of Device objects attached ot the message

=head3 getDevice

C<libgmsec_perl::DeviceMessage-E<gt>getDevice($index)>

	Returns a reference to a Device object attached to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$index - The zero-based index of the Device object to access

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A Device object at the specified index inside of DeviceMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the index specified is not in the range of Devices in the message

=head3 getDeviceIterator

C<libgmsec_perl::Devicemessage-E<gt>getDeviceIterator()>

	Method that allows the callee to get the DeviceIterator associated with the DeviceMessage.  This iterator will allow for applications to iterate over the Device objects stored within the DeviceMessage.  The Iterator is reset each time getDeviceIterator() is called.  The iterator itself is destroyed when the DeviceMessage object is destroyed.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

	Only one DeviceIterator object is associated with a DeviceMessage object; multiple calls to getDeviceIteraotr() will return a reference to the same DeviceIterator object.  Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to a DeviceIterator object

=head3 convertMessage

C<libgmsec_perl::DeviceMessage-E<gt>convertMessage($message)>

	Constructs and returns a DeviceMessage from an ordinary Message object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$message - the Message object to be converted to a DeviceMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A new DeviceMessage constructed using the data from the given Message object

=cut
%}
