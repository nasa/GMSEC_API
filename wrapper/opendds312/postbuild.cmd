rem Copyright 2007-2024 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

@echo OFF

set BINDIR=..\..\bin

echo Post-Build Cleanup of OpenDDS wrapper

del /Q %BINDIR%\gmsec_opendds312.exp
del /Q %BINDIR%\gmsec_opendds312.iobj
del /Q %BINDIR%\gmsec_opendds312.ipdb
del /Q %BINDIR%\gmsec_opendds312.lib
del /Q %BINDIR%\gmsec_opendds312.pdb

rmdir /S /Q %BINDIR%\opendds

mkdir %BINDIR%\opendds

copy ..\opendds\dds_tcp_conf.ini %BINDIR%\opendds
