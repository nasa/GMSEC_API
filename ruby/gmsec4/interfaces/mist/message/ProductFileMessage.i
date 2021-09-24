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

/* Ignore unsupported methods */
%ignore gmsec::api::mist::message::ProductFileMessage::operator=(const ProductFileMessage&);

/* Ignore deprecated methods */
%ignore gmsec::api::mist::message::ProductFileMessage::ProductFileMessage(const char*, ResponseStatus::Response, Message::MessageKind, const char*, const char*, const Specification&);
%ignore gmsec::api::mist::message::ProductFileMessage::ProductFileMessage(const char*, ResponseStatus::Response, Message::MessageKind, const char*, const char*, const Config&, const Specification&);
%ignore gmsec::api::mist::message::ProductFileMessage::ProductFileMessage(const char*);
%ignore gmsec::api::mist::message::ProductFileMessage::getProductFile(size_t) const;

%rename("add_product_file") addProductFile;
%rename("get_num_product_files") getNumProductFiles;
%rename("get_response_status") getResponseStatus;
%rename("get_product_type") getProductType;
%rename("get_product_subtype") getProductSubtype;
%rename("get_product_file_iterator") getProductFileIterator;
%rename("convert_message") convertMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/ProductFileMessage.h>
