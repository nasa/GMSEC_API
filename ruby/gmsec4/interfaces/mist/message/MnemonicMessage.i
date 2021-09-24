/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%rename(MnemonicMessage) gmsec::api::mist::message::MnemonicMessage;

%module MnemonicMessage

%{
#include <gmsec4/mist/message/MnemonicMessage.h>
using namespace gmsec::api::mist::message;
%}

%ignore gmsec::api::mist::message::MnemonicMessage::operator=(const MnemonicMessage&);

/* Ignore deprecated methods */
%ignore gmsec::api::mist::message::MnemonicMessage::MnemonicMessage(const char*);
%ignore gmsec::api::mist::message::MnemonicMessage::getMnemonic(size_t) const;

%rename("add_mnemonic") addMnemonic;
%rename("get_num_mnemonics") getNumMnemonics;
%rename("get_mnemonic_iterator") getMnemonicIterator;
%rename("convert_message") convertMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/MnemonicMessage.h>
