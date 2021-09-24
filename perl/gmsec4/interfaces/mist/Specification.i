/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Specification

%{
#include <gmsec4/mist/Specification.h>
#include <vector>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Specification::getMessageSpecifications() const;

%ignore gmsec::api::mist::Specification::getTemplateXML(const char*, const char*);

/* Ignore C methods */
%ignore gmsec::api::mist::Specification::registerMessageValidator(GMSEC_MessageValidator*);

// Turn on director mapping for the Specification package
%feature("director") Specification;

%apply SWIGTYPE *DISOWN { Specification *spec };

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Specification.h>

%include <std_vector.i>

namespace std
{
    %template(MessageSpecificationList) vector<gmsec::api::mist::MessageSpecification*>;
}


%extend gmsec::api::mist::Specification {

    const std::vector<gmsec::api::mist::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::vector<gmsec::api::mist::MessageSpecification*> newMsgSpecs;

        for (gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
}


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Specification

	The Specification class loads a list of templates from a designated directory that are used to create and validate message schemas.  Each template has an associated ID that is used to identify the kind of message schema the template will be defining.

=head2 Public Member Subroutines


=head3 new

C<libgmsec_perl::Specification-E<gt>new()>

	Initializes the Specification with the default message specification (NASA/GMSEC Addendum)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception is thrown if an error occurs loading the message specification


C<libgmsec_perl::Specification-E<gt>new($config)>

	Initializes the Specification instance with a configuration

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$config The configuration file that Specification uses

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception is thrown if Specification cannot deduce the values passed to it by the Configuration
	Exception is thrown if an error occurs loading the message specification


=head3 DESTROY

C<libgmsec_perl::Specification-E<gt>DESTROY()>

    Destructor


=head3 validateMessage

C<libgmsec_perl::Specification-E<gt>validateMessage($message)>

	Looks up the message subject in the message registry to grab the approproate template (based on its assigned schema ID).  The contents of the message are then compared to the template to ensure the message complies with the ISD.  This function will also register the message with an appropriate schema ID if it is not found in the registry.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$message - the message to be validated.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if the message fails to pass validation.


=head3 getSchemaIDIterator

C<libgmsec_perl::Specification-E<gt>getSchemaIDIterator()>

	returns a SchemaIDIterator, which can be used to iterate over the IDs of the loaded templates.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>
	
	Only one SchemaIDIterator object is associated with a Specification object; multiple calls to getSchemaIDIterator() will return a reference to the same SchemaIDIterator object.  Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	a reference to a schemaIDIterator object


=head3 getVersion

C<libgmsec_perl::Specification-E<gt>getVersion()>

	Gets the version of the ISD being used

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	The version number of the ISD being used


=head3 getSchemaLevel

C<libgmsec_perl::Specification-E<gt>getSchemaLevel()>

	Returns the schema level of the message specification in use

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

	An enumerated value for the Schema Level


=head3 getMessageSpecifications

C<libgmsec_perl::Specification-E<gt>getMessageSpecifications()>
    Accessor that returns a vector of MessageSpecification object(s) associated with the Specification

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
    A vector of MessageSpecification objects(s)


=head3 registerMessageTemplate

C<libgmsec_perl::Specification-E<gt>registerMessageTemplate()>

    Registers the give MessageValidator to be used when message validation takes place.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$validator The custom MessageValidator to perform message validation

=cut
%}
