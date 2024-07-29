@echo off

rem Copyright 2007-2024 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

rem Additional handling for ensuring that the globally-defined XEntry classes
rem (from ConfigFile) are used. Need to include the comma at the end of the
rem replacement string in order to not overwrite the SWIG-generated definition
rem for the (unused) ConfigFile::XEntry classes.

perl -pi.bak -e "s/SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g" interfaces\gmsec_nodejs_wrap.cxx
perl -pi.bak -e "s/SWIGTYPE_p_gmsec__api5__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g" interfaces\gmsec_nodejs_wrap.cxx
perl -pi.bak -e "s/SWIGTYPE_p_gmsec__api5__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g" interfaces\gmsec_nodejs_wrap.cxx

perl -pi.bak -e "s/long longVal = \(long\) SWIGV8_NUMBER_VALUE\(obj\);/long long longVal = \(long long\) SWIGV8_NUMBER_VALUE\(obj\);/g" interfaces\gmsec_nodejs_wrap.cxx

@echo on
