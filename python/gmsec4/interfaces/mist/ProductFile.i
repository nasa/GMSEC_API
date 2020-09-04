/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ProductFile
%{
#include <gmsec4/mist/ProductFile.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::ProductFile::getContents(GMSEC_BIN&) const;

%include "dox/ProductFile_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/ProductFile.h>

%extend gmsec::api::mist::ProductFile {

    GMSEC_BIN CALL_TYPE getContents()
    {
        GMSEC_BIN contents = NULL;

        self->getContents(contents);

        return contents;
    }

}
