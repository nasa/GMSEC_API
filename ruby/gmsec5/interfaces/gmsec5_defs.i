/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%{
#include <gmsec5_defs.h>
%}


%ignore LogLevel::logNLEVEL;

%rename("LogNONE") LogLevel::logNONE;
%rename("LogERROR") LogLevel::logERROR;
%rename("LogSECURE") LogLevel::logSECURE;
%rename("LogWARNING") LogLevel::logWARNING;
%rename("LogINFO") LogLevel::logINFO;
%rename("LogVERBOSE") LogLevel::logVERBOSE;
%rename("LogDEBUG") LogLevel::logDEBUG;


%include <gmsec5_defs.h>

