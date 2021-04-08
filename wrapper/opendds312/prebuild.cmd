@echo OFF

set SUPPORT=..\..\..\GMSEC_SUPPORT\opendds-3.12.1

set ACE_ROOT=%SUPPORT%
set DDS_ROOT=%ACE_ROOT%

set TAO_IDL=%SUPPORT%\ACE_wrappers\bin\tao_idl.exe

set OPENDDS_IDL=%SUPPORT%\bin\opendds_idl.exe

set SRCDIR=..\opendds\src\
set OBJDIR=%SRCDIR%

set GMSEC_IDL_FILE=%SRCDIR%\OpenDDSMessage.idl

set OPENDDS_IDL_FILE=%SRCDIR%\OpenDDSMessageTypeSupport.idl

set PATH=%SUPPORT%\bin;%SUPPORT%\lib;%SUPPORT%\ACE_wrappers\bin;%SUPPORT%\ACE_wrappers\lib;%PATH%


echo Pre-Build of OpenDDS wrapper

del /Q %SRCDIR%\OpenDDSMessageC.h
del /Q %SRCDIR%\OpenDDSMessageC.cpp
del /Q %SRCDIR%\OpenDDSMessageC.inl
del /Q %SRCDIR%\OpenDDSMessageS.h
del /Q %SRCDIR%\OpenDDSMessageS.cpp
del /Q %SRCDIR%\OpenDDSMessageTypeSupportImpl.h
del /Q %SRCDIR%\OpenDDSMessageTypeSupportImpl.cpp
del /Q %SRCDIR%\OpenDDSMessageTypeSupport.idl
del /Q %SRCDIR%\OpenDDSMessageTypeSupportC.cpp
del /Q %SRCDIR%\OpenDDSMessageTypeSupportC.h
del /Q %SRCDIR%\OpenDDSMessageTypeSupportC.inl
del /Q %SRCDIR%\OpenDDSMessageTypeSupportS.h
del /Q %SRCDIR%\OpenDDSMessageTypeSupportS.cpp

%TAO_IDL% %GMSEC_IDL_FILE% -o %OBJDIR%
%OPENDDS_IDL% %GMSEC_IDL_FILE% -o %OBJDIR%
%TAO_IDL% -I%SRCDIR% -I%SUPPORT% -I%SUPPORT%\ACE_wrappers\TAO %OPENDDS_IDL_FILE% -o %OBJDIR%
