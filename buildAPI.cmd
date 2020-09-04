@echo OFF

rem Copyright 2007-2019 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

rem This batch file builds the GMSEC API for Microsoft Windows

setlocal

set GMSEC_x64=

if "%VSCMD_ARG_TGT_ARCH%" == "x64" (
	set GMSEC_x64=1
)

set GMSEC_VS2017=1

if not defined "%SUPPORT_HOME%" (
	set SUPPORT_HOME=%~dp0..\SUPPORT
)

if not defined "%LIBXML2_HOME%" (
	set LIBXML2_HOME=%SUPPORT_HOME%\libxml2
)

if not defined "%LIBXSLT_HOME%" (
	set LIBXSLT_HOME=%SUPPORT_HOME%\libxslt
)

if not defined "%JDK_HOME%" (
	if "%GMSEC_x64%" == "1" (
		set JDK_HOME=C:\Program Files\Java\jdk1.8.0_171
	) else (
		set JDK_HOME=C:\Program Files ^(x86^)\Java\jdk1.8.0_171
	)
)

if not defined "%PERL5_HOME%" (
	if "%GMSEC_x64%" == "1" (
		set PERL5_HOME=C:\Perl64
	) else (
		set PERL5_HOME=C:\Perl
	)
)

if not defined "%SWIG_HOME%" (
	set SWIG_HOME=%SUPPORT_HOME%\swig-3.0.10
)

if not defined "%PYTHON_HOME%" (
	if "%GMSEC_x64%" == "1" (
		set PYTHON_HOME=C:\Python27
	)
)

if not defined "%PYTHON3_HOME%" (
	set PYTHON3_HOME=%SUPPORT_HOME%\python-3.6.5
)

echo.
echo.Building the GMSEC API with the following environment variables:
echo.
echo    GMSEC_x64   : %GMSEC_x64%
echo    GMSEC_VS2017: %GMSEC_VS2017%
echo    SUPPORT_HOME: %SUPPORT_HOME%
echo    LIBXML2_HOME: %LIBXML2_HOME%
echo    LIBXSLT_HOME: %LIBXSLT_HOME%
echo    JDK_HOME    : %JDK_HOME%
echo    PERL5_HOME  : %PERL5_HOME%
echo    SWIG_HOME   : %SWIG_HOME%
echo    PYTHON_HOME : %PYTHON_HOME%
echo    PYTHON3_HOME: %PYTHON3_HOME%
echo.


rem Finally, built the GMSEC API
build4windows.cmd

echo All done
