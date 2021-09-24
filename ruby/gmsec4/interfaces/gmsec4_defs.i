/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%{
#include <gmsec4_defs.h>
%}


%ignore logNLEVEL;

%rename("LogNONE") logNONE;
%rename("LogERROR") logERROR;
%rename("LogSECURE") logSECURE;
%rename("LogWARNING") logWARNING;
%rename("LogINFO") logINFO;
%rename("LogVERBOSE") logVERBOSE;
%rename("LogDEBUG") logDEBUG;
//%rename("LogNLEVEL") logNLEVEL;


%include <gmsec4_defs.h>

