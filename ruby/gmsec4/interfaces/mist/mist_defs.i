/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Constants

%{
#include <gmsec4/mist/mist_defs.h>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::GMSEC_ISD_2014_00;
%ignore gmsec::api::mist::GMSEC_ISD_2016_00;
%ignore gmsec::api::mist::GMSEC_ISD_2018_00;
%ignore gmsec::api::mist::GMSEC_ISD_2019_00;
%ignore gmsec::api::mist::GMSEC_ISD_CURRENT;

%constant unsigned int GMSEC_ISD_2014_00 = gmsec::api::mist::GMSEC_ISD_2014_00;
%constant unsigned int GMSEC_ISD_2016_00 = gmsec::api::mist::GMSEC_ISD_2016_00;
%constant unsigned int GMSEC_ISD_2018_00 = gmsec::api::mist::GMSEC_ISD_2018_00;
%constant unsigned int GMSEC_ISD_2019_00 = gmsec::api::mist::GMSEC_ISD_2019_00;
%constant unsigned int GMSEC_ISD_CURRENT = gmsec::api::mist::GMSEC_ISD_CURRENT;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/mist_defs.h>
