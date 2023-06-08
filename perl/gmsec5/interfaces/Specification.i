/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Specification

%{
#include <gmsec5/Specification.h>
#include <vector>
#include <list>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::Specification::getMessageSpecifications() const;
%ignore gmsec::api5::Specification::getHeaderFieldNames() const;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>

%include <std_vector.i>

namespace std
{
    %template(MessageSpecificationList) vector<gmsec::api5::MessageSpecification*>;
    %template(HeaderList) vector<std::string>;
}


%extend gmsec::api5::Specification {

    const std::vector<gmsec::api5::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api5::util::List<gmsec::api5::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::vector<gmsec::api5::MessageSpecification*> newMsgSpecs;

        for (gmsec::api5::util::List<gmsec::api5::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
	
	const std::vector<std::string> CALL_TYPE getHeaderFieldNames()
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames("DEFAULT");
		
		std::vector<std::string> headerList;
		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerList.push_back(*it);
		}
		
		return headerList;
	}
	
	const std::vector<std::string> CALL_TYPE getHeaderFieldNames(std::string schemaName)
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames(schemaName.c_str());
		
		std::vector<std::string> headerList;
		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerList.push_back(*it);
		}
		
		return headerList;
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

        $libgmsec_perl::GmsecException is thrown if an error occurs loading the message specification


C<libgmsec_perl::Specification-E<gt>new($config)>

        Initializes the Specification instance with a configuration

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config The configuration file that Specification uses

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException is thrown if Specification cannot deduce the values passed to it by the Configuration
        $libgmsec_perl::GmsecException is thrown if an error occurs loading the message specification


=head3 DESTROY

C<libgmsec_perl::Specification-E<gt>DESTROY()>

    Destructor


=head3 getSchemaIDIterator

C<libgmsec_perl::Specification-E<gt>getSchemaIDIterator()>

        Returns a SchemaIDIterator, which can be used to iterate over the IDs of the loaded templates.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Only one SchemaIDIterator object is associated with a Specification object; multiple calls to getSchemaIDIterator() will return a reference to the same SchemaIDIterator object.  Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A reference to a schemaIDIterator object


=head3 getVersion

C<libgmsec_perl::Specification-E<gt>getVersion()>

        Gets the version of the message specification being used

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The version number of the message specification being used


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


=head3 getHeaderFieldNames

C<libgmsec_perl::Specification-E<gt>getHeaderFieldNames()>
        Returns a list of the field names for the default header (which is the topmost schema level currently configured)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
        A list that contains the names of the header fields.

C<libgmsec_perl::Specification-E<gt>getHeaderFieldNames($schemaName)>
        Returns a list of the field names for the specified header 

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $schemaName - The name of the specified schema level (e.g. C2MS, C2MS-PIPE, GMSEC, DEFAULT).  An empty string refers to DEFAULT.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
        A list that contains the names of the header fields.

=cut
%}
