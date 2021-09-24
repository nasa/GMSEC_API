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

%include <std_string.i>
%include <arrays_csharp.i>

%apply unsigned char INOUT[] {unsigned char* pData}
%apply int {size_t}

%ignore gmsec::api::mist::ProductFile::getContents(GMSEC_BIN&) const;

%ignore gmsec::api::mist::ProductFile::operator=(const ProductFile&);
%ignore gmsec::api::mist::ProductFile::operator==(const ProductFile&) const;
%ignore gmsec::api::mist::ProductFile::operator!=(const ProductFile&) const;

%rename("GetName") getName;
%rename("GetDescription") getDescription;
%rename("GetVersion") getVersion;
%rename("GetFormat") getFormat;
%rename("URIAvailable") uriAvailable;
%rename("GetURI") getURI;
%rename("ContentsAvailable") contentsAvailable;
%rename("GetContents") getContents;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFile.h>

%extend gmsec::api::mist::ProductFile {

    GMSEC_BinaryData CALL_TYPE getContents()
    {
        GMSEC_BIN contents = NULL;

        size_t length = self->getContents(contents);

        return GMSEC_BinaryData(contents, (int) length);
    }

}
