@echo OFF

set BINDIR=..\..\bin

echo Post-Build Cleanup of OpenDDS wrapper

del /Q %BINDIR%\gmsec_opendds317.exp
del /Q %BINDIR%\gmsec_opendds317.iobj
del /Q %BINDIR%\gmsec_opendds317.ipdb
del /Q %BINDIR%\gmsec_opendds317.lib
del /Q %BINDIR%\gmsec_opendds317.pdb

rmdir /S /Q %BINDIR%\opendds

mkdir %BINDIR%\opendds

copy ..\opendds\dds_tcp_conf.ini %BINDIR%\opendds
