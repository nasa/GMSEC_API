/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ProductFile
%{
#include <gmsec4/mist/ProductFile.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::ProductFile::getContents(GMSEC_BIN&) const;

%ignore gmsec::api::mist::ProductFile::operator=(const ProductFile&);
%ignore gmsec::api::mist::ProductFile::operator==(const ProductFile&) const;
%ignore gmsec::api::mist::ProductFile::operator!=(const ProductFile&) const;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFile.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ProductFile

This class is a lightweight container for holding information on a product file, and is used to generate GMSEC ProductFileMessages by the ConnectionManager class

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::ProductFile-E<gt>new($name, $description, $version, $format, $uri)>

	Initializes the ProductFile object with a URI to describe a commonly accessible resource

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$name - the name of the file
	$description - the description of the file
	$version - the version of the file
	$format - the descriptor of the file format
	$uri - the URI ffor the file

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if $name, $description, $version, or $format are NULL or emtpy strings

C<libgmsec_perl::ProductFile-E<gt>new($name, $description, $version, $format, $data, $fileSize)>

	Initializes the ProductFile object with a URI to describe a commonly accessible resource

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

	$name - the name of the file
	$description - the description of the file
	$version - the version of the file
	$format - the descriptor of the file format
	$data - the binary contents of the file
	$fileSize - the size of the binary contents of the file

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

	Exception if $name, $description, $version, or $format are NULL or emtpy strings

=head3 DESTROY

C<libgmsec_perl::ProductFile-E<gt>DESTROY()>

    Destructor

=head3 getName

C<libgmsec_perl::ProductFile-E<gt>getName()>

    Retrieves the name of the ProductFile

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The name of the ProductFile

=head3 getDesctiption

C<libgmsec_perl::ProductFile-E<gt>getDescription()>

    Retrieves the description of the ProductFile

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The description of the ProductFile

=head3 getVersion

C<libgmsec_perl::ProductFile-E<gt>getVersion()>

    Retrieves the version of the ProductFile

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The version of the ProductFile

=head3 getFormat

C<libgmsec_perl::ProductFile-E<gt>getFormat()>

    Retrieves the format of the ProductFile

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The format of the ProductFile

=head3 uriAvailable

C<libgmsec_perl::ProductFile-E<gt>uriAvailable()>

    Checks the availability of the ProductFile URI

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Productfile URI has been set, 0 otherwise

=head3 getURIName

C<libgmsec_perl::ProductFile-E<gt>getURI()>

    Retrieves the URI of the ProductFile

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    the ProductFile URI string, or NULL if the URI is not available

=head3 contentsAvailable

C<libgmsec_perl::ProductFile-E<gt>contentAvailable()>

    Checks the availability of the ProductFile file content

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Productfile content file has been set, 0 otherwise
%}
