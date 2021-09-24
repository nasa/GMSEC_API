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

%rename("get_time_stamp") getTimeStamp;
%rename("get_raw_value") getRawValue;
%rename("has_eu_value") hasEUValue;
%rename("get_eu_value") getEUValue;
%rename("set_eu_value") setEUValue;
%rename("has_text_value") hasTextValue;
%rename("get_text_value") getTextValue;
%rename("set_text_value") setTextValue;
%rename("has_flags") hasFlags;
%rename("get_flags") getFlags;
%rename("set_flags") setFlags;
%rename("has_limit_enable_disable") hasLimitEnableDisable;
%rename("get_limit_enable_disable") getLimitEnableDisable;
%rename("set_limit_enable_disable") setLimitEnableDisable;
%rename("has_limit") hasLimit;
%rename("get_limit") getLimit;
%rename("set_limit") setLimit;
%rename("has_staleness_status") hasStalenessStatus;
%rename("get_staleness_status") getStalenessStatus;
%rename("set_staleness_status") setStalenessStatus;
%rename("has_quality") hasQuality;
%rename("get_quality") getQuality;
%rename("set_quality") setQuality;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MnemonicSample.h>
