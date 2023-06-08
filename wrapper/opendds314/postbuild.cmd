rem Copyright 2007-2023 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

@echo OFF

set BINDIR=..\..\bin

echo Post-Build Cleanup of OpenDDS wrapper

del /Q %BINDIR%\gmsec_opendds314.exp
del /Q %BINDIR%\gmsec_opendds314.iobj
del /Q %BINDIR%\gmsec_opendds314.ipdb
del /Q %BINDIR%\gmsec_opendds314.lib
del /Q %BINDIR%\gmsec_opendds314.pdb

rmdir /S /Q %BINDIR%\opendds

mkdir %BINDIR%\opendds

copy ..\opendds\dds_tcp_conf.ini %BINDIR%\opendds
