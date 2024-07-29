@echo OFF

rem Copyright 2007-2024 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

rem This batch file builds the GMSEC API for Microsoft Windows

setlocal

set GMSEC_VS2019=1

if not defined "%SUPPORT_HOME%" (
	set SUPPORT_HOME=%~dp0..\GMSEC_SUPPORT
)

if not defined "%JDK_HOME%" (
	set JDK_HOME=C:\Program Files\Java\jdk-11.0.8
)

if not defined "%PERL5_HOME%" (
	set PERL5_HOME=C:\Strawberry
)

if not defined "%SWIG_HOME%" (
	set SWIG_HOME=%SUPPORT_HOME%\swig-4.0.2
)

if not defined "%PYTHON3_HOME%" (
	set PYTHON3_HOME=%SUPPORT_HOME%\python-3.8.5
)

echo.
echo.Building the GMSEC API with the following environment variables:
echo.
echo    GMSEC_VS2019    : %GMSEC_VS2019%
echo    SUPPORT_HOME    : %SUPPORT_HOME%
echo    JDK_HOME        : %JDK_HOME%
echo    PERL5_HOME      : %PERL5_HOME%
echo    SWIG_HOME       : %SWIG_HOME%
echo    PYTHON3_HOME    : %PYTHON3_HOME%
echo.

rem Finally, build the GMSEC API
build4windows.cmd

echo All done
