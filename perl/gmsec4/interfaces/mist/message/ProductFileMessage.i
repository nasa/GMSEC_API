/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ProductFileMessage
%{
#include <gmsec4/mist/message/ProductFileMessage.h>
using namespace gmsec::api::mist::message;
%}

%ignore gmsec::api::mist::message::ProductFileMessage::operator=(const ProductFileMessage&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/ProductFileMessage.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ProductFileMessage

	A ProductFileMessage object that is capable of storing ProductFile objects.

	The following message schema IDs and their templates are supported:
	 2019.00.C2MS.MSG.PROD
	 MSG.PROD

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::ProductFileMessage-E<gt>new($subject, $responseStatus, $kind, $productType, $productSubtype, $spec)>

	Initializes the message instance and automatically builds a schemaID based on the params given

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - The subject string for the message
	$responseStatus - RESPONSE-STATUS field to insert into the message
	$kind - the kind of message to instantiate
	$productType - PROD-TYPE field to insert into the message
	$productSubtype - PROD-SUBTYPE field to insert into the message
	$spec - a reference to the specification this message's schema will adhere to

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if a valid schemaID cannot be created form the given params

C<libgmsec_perl::ProductFileMessage-E<gt>new($subject, $responseStatus, $kind, $productType, $productSubtype, $config, $spec)>

	Initializes the message instance and automatically builds a schemaID based on the params given

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - The subject string for the message
	$responseStatus - RESPONSE-STATUS field to insert into the message
	$kind - the kind of message to instantiate
	$productType - PROD-TYPE field to insert into the message
	$productSubtype - PROD-SUBTYPE field to insert into the message
	$config - A configuration to associate with the message
	$spec - a reference to the specification this message's schema will adhere to

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if a valid schemaID cannot be created form the given params


C<libgmsec_perl::ProductFileMessage-E<gt>new($subject, $responseStatus, $schemaID, $spec)>

	Default constructor - Initializes the message instance with a template determined by the ID and spec

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - the subject string for the message.
	$responseStatus - RESPONSE-STATUS field to insert into the message
	$schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.PROD). Users may also use shorthand notation of messageKind.messageType.messageSubtype (e.g. MSG.PROD) Note that not all schemaIDs contain a messageSubtype
	$spec - a reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID	


C<libgmsec_perl::ProductFileMessage-E<gt>new($subject, $responseStatus, $schemaID, $config, $spec)>

	Initializes the message instance with a template determined by the ID and spec and an associated Configuration object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$subject - the subject string for the message.
	$responseStatus - RESPONSE-STATUS field to insert into the message
	$schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.PROD). Users may also use shorthand notation of messageKind.messageType.messageSubtype (e.g. MSG.PROD) Note that not all schemaIDs contain a messageSubtype
	$config - A Configuration object to associate with the message
	$spec - a reference to the specification this message's schema will adhere to.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if Specification fails to load the template directory or if schemaID is not a valid ID


C<libgmsec_perl::ProductFileMessage-E<gt>new($spec, $data)>

    Initializes a ProductFileMessage using an XML or JSON string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $spec - A reference to the specification to which this message will adhere
    $data - XML or JSON string used to initialize the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the given data does not represent a valid XML or JSON statement.


=head3 DESTROY

C<libgmsec_perl::ProductFileMessage-E<gt>DESTROY()>

    Destructor

=head3 addProductFile

C<libgmsec_perl::ProductFileMessage-E<gt>addProductFile($productFile)>

	Adda a ProductFile to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$productFile - a ProductFile object to attach to the message

=head3 getNumProductFiles

C<libgmsec_perl::ProductFileMessage-E<gt>getNumProductFiles()>

	Gets the number of ProductFiles in the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The number of ProductFile objects attached to the message

=head3 getProductFile

C<libgmsec_perl::ProductFileMessage-E<gt>getProductFile($index)>

	Returns a reference to a ProductFile object attached to the message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$index - The zero-based index of the ProductFile object to access

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A ProductFile object at the specified index inside of ProductFileMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the index specified is not in the range of ProductFiles in the message

=head3 getResponseStatus

C<libgmsec_perl::ProductFileMessage-E<gt>getResponseStatus()>

	Get the response status of this ProductFile message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	a ResponseStatus enumeration given by the RESPONSE-STATUS field

=head3 getProductType

C<libgmsec_perl::ProductFileMessage-E<gt>getProductType()>

	Get the PROD-TYPE of this productFile message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	a string given by the PROD-TYPE field in the message

=head3 getProductSubype

C<libgmsec_perl::ProductFileMessage-E<gt>getProductSubype()>

	Get the PROD-SUBTYPE of this productFile message

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	a string given by the PROD-SUBTYPE field in the message

=head3 getProductFileIterator

C<libgmsec_perl::ProductFilemessage-E<gt>getProductFileIterator()>

	Method that allows the callee to get the ProductFileIterator associated with the ProductFileMessage.  This iterator will allow for applications to iterate over the ProductFile objects stored within the ProductFileMessage.  The Iterator is reset each time getProductFileIterator() is called.  The iterator itself is destroyed when the ProductFileMessage object is destroyed.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

	Only one ProductFileIterator object is associated with a ProductFileMessage object; multiple calls to getProductFileIteraotr() will return a reference to the same ProductFileIterator object.  Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A reference to a ProductFileIterator object

=head3 convertMessage

C<libgmsec_perl::ProductFileMessage-E<gt>convertMessage($message)>

	Constructs and returns a ProductFileMessage from an ordinary Message object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$message - the Message object to be converted to a ProductFileMessage

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	A new ProductFileMessage constructed using the data from the given Message object

=cut
%}
