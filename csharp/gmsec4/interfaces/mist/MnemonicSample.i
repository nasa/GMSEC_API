/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MnemonicSample

%{
#include <gmsec4/mist/MnemonicSample.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::MnemonicSample::operator==(const MnemonicSample&) const;
%ignore gmsec::api::mist::MnemonicSample::operator!=(const MnemonicSample&) const;

%rename("GetTimeStamp") getTimeStamp;
%rename("GetRawValue") getRawValue;
%rename("HasEUValue") hasEUValue;
%rename("GetEUValue") getEUValue;
%rename("SetEUValue") setEUValue;
%rename("HasTextValue") hasTextValue;
%rename("GetTextValue") getTextValue;
%rename("SetTextValue") setTextValue;
%rename("HasFlags") hasFlags;
%rename("GetFlags") getFlags;
%rename("SetFlags") setFlags;
%rename("HasLimitEnableDisable") hasLimitEnableDisable;
%rename("GetLimitEnableDisable") getLimitEnableDisable;
%rename("SetLimitEnableDisable") setLimitEnableDisable;
%rename("HasLimit") hasLimit;
%rename("GetLimit") getLimit;
%rename("SetLimit") setLimit;
%rename("HasStalenessStatus") hasStalenessStatus;
%rename("GetStalenessStatus") getStalenessStatus;
%rename("SetStalenessStatus") setStalenessStatus;
%rename("HasQuality") hasQuality;
%rename("GetQuality") getQuality;
%rename("SetQuality") setQuality;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MnemonicSample.h>
