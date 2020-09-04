rem Copyright 2007-2019 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

@echo off
rem Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
rem Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
perl -i.bak -pe "s/SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g" interfaces\libgmsec_python_wrap.cxx
perl -i.bak -pe "s/SWIGTYPE_p_gmsec__api__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g" interfaces\libgmsec_python_wrap.cxx
perl -i.bak -pe "s/SWIGTYPE_p_gmsec__api__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g" interfaces\libgmsec_python_wrap.cxx

@echo on
