REM This script is invoked as a Custom Build Tool for libgmsec_python.i
REM
REM To view how this script is called, right-click on libgmsec_python.i, select Custom Build Tool->General,
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

if "!_VisualStudioVersion!" == "" goto:buildLegacy

if !_VisualStudioVersion! lss 15 goto:buildLegacy


REM For Visual Studio 2017
REM Remove the semicolon from the end of the _VC_InstallDir macro for a valid path to use with SWIG
set INCLUDE=!_VCIncludeDir!
if "x!INCLUDE:~-1!"=="x;" (
	set INCLUDE=!INCLUDE:~0,-1!
)

echo "%SWIG_HOME%\swig.exe" -DGMSEC_S16IL32LL64=1 -DCHAR_BIT=8 -c++ -python -py3 -threads -I..\..\framework\include -I"!_VCInstallDir!include" -I"!INCLUDE!" "!_InputFile!"
"%SWIG_HOME%\swig.exe" -DGMSEC_S16IL32LL64=1 -DCHAR_BIT=8 -c++ -python -py3 -threads -I..\..\framework\include -I"!_VCInstallDir!include" -I"!INCLUDE!" "!_InputFile!"
goto:patch


:buildLegacy
REM For Visual Studio 2008, 2010 or 2013
echo "%SWIG_HOME%\swig.exe" -DGMSEC_S16IL32LL64=1 -DCHAR_BIT=8 -c++ -python -py3 -threads -I..\..\framework\include -I"!_VCInstallDir!include" "!_InputFile!"
"%SWIG_HOME%\swig.exe" -DGMSEC_S16IL32LL64=1 -DCHAR_BIT=8 -c++ -python -py3 -threads -I..\..\framework\include -I"!_VCInstallDir!include" "!_InputFile!"


:patch
echo "Patching swig generated code"
call patch.bat
call addWindowsMacros.bat


:eof
set _VisualStudioVersion=
set _VCInstallDir=
set _VCIncludeDir=
set _InputFile=
