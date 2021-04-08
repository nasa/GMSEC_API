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
