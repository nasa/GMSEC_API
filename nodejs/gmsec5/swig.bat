rem Copyright 2007-2023 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

REM This script is invoked as a Custom Build Tool for gmsec_nodejs.i
REM
REM To view how this script is called, right-click on gmsec_nodejs.i, select Custom Build Tool->General,
REM and then view the Command Line setting.
REM

if "%SWIG_HOME%" == "" (
	echo Error: SWIG_HOME has not been defined
	goto:eof
)

echo SWIG_HOME: %SWIG_HOME%

set _VisualStudioVersion=%~1
set _VCInstallDir=%~2
set _VCIncludeDir=%~3
set _InputFile=%~4

echo args:
echo 1 -- %_VisualStudioVersion%
echo 2 -- %_VCInstallDir%
echo 3 -- %_VCIncludeDir%
echo 4 -- %_InputFile%

setlocal enabledelayedexpansion

REM For Visual Studio 2017
REM Remove the semicolon from the end of the _VC_InstallDir macro for a valid path to use with SWIG
set INCLUDE=!_VCIncludeDir!
if "x!INCLUDE:~-1!"=="x;" (
	set INCLUDE=!INCLUDE:~0,-1!
)

echo "%SWIG_HOME%\swig.exe" -DGMSEC_S16IL32LL64=1 -DCHAR_BIT=8 -c++ -javascript -node -I../../framework/include -I"!_VCInstallDir!include" -I"!INCLUDE!" "!_InputFile!"
"%SWIG_HOME%\swig.exe" -DGMSEC_S16IL32LL64=1 -DCHAR_BIT=8 -c++ -javascript -node -I../../framework/include -I"!_VCInstallDir!include" -I"!INCLUDE!" "!_InputFile!"


echo "Patching swig generated code"
call patch.bat
REM call addWindowsMacros.bat


:eof
set _VisualStudioVersion=
set _VCInstallDir=
set _VCIncludeDir=
set _InputFile=
