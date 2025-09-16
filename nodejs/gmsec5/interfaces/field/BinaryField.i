/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module BinaryField

%{
#include <gmsec5/field/BinaryField.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::BinaryField(const char*, const GMSEC_U8*, size_t, bool);
%ignore gmsec::api5::BinaryField::getValue() const;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/BinaryField.h>

%include "std_vector.i"

namespace std
{
    %template(BlobArray) vector<GMSEC_U8>;
}

%extend gmsec::api5::BinaryField {

    BinaryField(const char* fieldName, const std::vector<GMSEC_U8>& blob, bool isHeader = false)
    {
        return new BinaryField(fieldName, blob.data(), blob.size(), isHeader);
    }

    std::vector<GMSEC_U8> CALL_TYPE getValue()
    {
        size_t length = self->getLength();
        GMSEC_U8 const* data = self->getValue();

        std::vector<GMSEC_U8> values;

        for (size_t i = 0; i < length; ++i) {
            values.push_back(data[i]);
        }

        return values;
    }
}
