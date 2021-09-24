@echo off

rem Copyright 2007-2021 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

rem Additional handling for ensuring that the globally-defined XEntry classes
rem (from ConfigFile) are used. Need to include the comma at the end of the
rem replacement string in order to not overwrite the SWIG-generated definition
rem for the (unused) ConfigFile::XEntry classes.

perl -pi.bak -e "s/SWIGTYPE_p_gmsec__api__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g" interfaces\libgmsec_python_wrap.cxx
perl -pi.bak -e "s/SWIGTYPE_p_gmsec__api__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g" interfaces\libgmsec_python_wrap.cxx
perl -pi.bak -e "s/SWIGTYPE_p_gmsec__api__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g" interfaces\libgmsec_python_wrap.cxx

rem Depending on the version of SWIG, the generated code varies
perl -pi.bak -e "s/class GmsecError\(_object\)/class GmsecError\(Exception\)/g" interfaces\libgmsec_python3.py
perl -pi.bak -e "s/class GmsecError\(object\)/class GmsecError\(Exception\)/g" interfaces\libgmsec_python3.py

@echo on
