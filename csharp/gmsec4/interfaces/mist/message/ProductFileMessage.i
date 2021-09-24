/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%rename(ProductFileMessage) gmsec::api::mist::message::ProductFileMessage;

%module ProductFileMessage

%{
#include <gmsec4/mist/message/ProductFileMessage.h>
using namespace gmsec::api::mist::message;
%}

%ignore gmsec::api::mist::message::ProductFileMessage::operator=(const ProductFileMessage&);

/* Ignore deprecated methods */
%ignore gmsec::api::mist::message::ProductFileMessage::getNumProductFiles() const;
%ignore gmsec::api::mist::message::ProductFileMessage::getProductFile(size_t) const;

%rename("AddProductFile") addProductFile;
%rename("GetResponseStatus") getResponseStatus;
%rename("GetProductType") getProductType;
%rename("GetProductSubtype") getProductSubtype;
%rename("GetProductFileIterator") getProductFileIterator;
%rename("ConvertMessage") convertMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/ProductFileMessage.h>
