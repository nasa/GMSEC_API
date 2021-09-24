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


%ignore gmsec::api::mist::ProductFile(const char*, const char*, const char*, const char*, GMSEC_BIN, size_t);
%ignore gmsec::api::mist::ProductFile::getContents(GMSEC_BIN&) const;

%ignore gmsec::api::mist::ProductFile::operator=(const ProductFile&);
%ignore gmsec::api::mist::ProductFile::operator==(const ProductFile&) const;
%ignore gmsec::api::mist::ProductFile::operator!=(const ProductFile&) const;


%rename("get_name") getName;
%rename("get_description") getDescription;
%rename("get_version") getVersion;
%rename("get_format") getFormat;
%rename("uri_available") uriAvailable;
%rename("get_uri") getURI;
%rename("contents_available") contentsAvailable;
%rename("get_contents") getContents;


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFile.h>

%extend gmsec::api::mist::ProductFile {

    ProductFile(char* name, char* description, char* version, char* format, VALUE data, size_t fileSize) {
        char* blob = StringValuePtr(data);

        return new ProductFile(name, description, version, format, (GMSEC_BIN) blob, fileSize);
    }

    VALUE CALL_TYPE getContents() {
        GMSEC_BIN contents;
        size_t length = self->getContents(contents);
        return rb_str_new((char*) contents, length);
    }
};

