rem @echo OFF

rem Copyright 2007-2020 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


rem This batch file builds everything for Microsoft Windows

rem Build the main part of the API

set BUILD=gmsecapi gmsec_java gmsec_jni generic_jms bolt mb MBServer dotnet libgmsec_csharp libgmsec_perl libgmsec_python3 libgmsec_ruby gmhelp

IF DEFINED WRAPPERS (
	set BUILD=%BUILD% %WRAPPERS%
) 

IF DEFINED USER_WRAPPERS (
	set BUILD=%BUILD% %USER_WRAPPERS%
)

IF DEFINED GMSEC_x64 (
	set MCD=x64
) ELSE (
	set MCD=Win32
)

set TMP=
set TEMP=

REM IF DEFINED GMSEC_VC6 (
REM	call "C:\tools\VC98\Bin\VCVARS32.BAT"
REM	MSBuild.exe gmsecapi_allvendors.sln /t:Rebuild /p:Configuration=Release;Platform=Win32 /p:"VCBuildAdditionalOptions= /useenv"
REM	GOTO Perl
REM )

REM echo GMSEC_VS2010 is defined as %GMSEC_VS2010%
REM echo GMSEC_VS2013 is defined as %GMSEC_VS2013%
REM echo GMSEC_VS2017 is defined as %GMSEC_VS2017%

set _startPath=%~dp0
IF DEFINED GMSEC_VS2017 (
	echo Calling Microsoft Visual Studio 17.0 vcvarsall script for %MCD% architecture
	IF "%MCD%" == "Win32" (
		call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
	) ELSE (
		call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" %MCD%
	)
)
REM echo "Current Directory  : %cd%"
REM echo "Change Directory to: %_startPath%"
cd %_startPath%


REM IF DEFINED GMSEC_VS2013 (
REM	rmdir C:\Users\%USERNAME%\AppData\Local\Microsoft\VisualStudio\12.0 /s /q
REM	rmdir C:\Users\%USERNAME%\AppData\Roaming\Microsoft\VisualStudio\12.0 /s /q
REM	echo Calling Microsoft Visual Studio 12.0 vcvarsall script for %MCD% architecture
REM	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" %MCD%
REM )

REM IF DEFINED GMSEC_VS2010 (
REM	echo Upgrading project files
REM	devenv "gmsecapi_allvendors.sln" /upgrade
REM	cd tools/utilities
REM	devenv "utilities.sln" /upgrade
REM	cd ../..
REM )

rem Always start off with a clean slate
MSBuild.exe gmsecapi_allvendors.sln /t:Clean /p:Configuration=Release;Platform=%MCD%
MSBuild.exe gmsecapi_allvendors.sln /t:Clean /p:Configuration=Release-SNK;Platform=%MCD%

echo OFF

rem Build each portion of the solution, in the desired order.
FOR %%i IN (%BUILD%) DO (
	echo.
	echo.
	echo ###########################################################
	echo #
	echo #  Building %%i
	echo #
	echo ###########################################################
	IF '%%i'=='dotnet' (
		MSBuild.exe gmsecapi_allvendors.sln /t:%%i /p:Configuration=Release-SNK;Platform=%MCD%
	) ELSE (
		MSBuild.exe gmsecapi_allvendors.sln /t:%%i /p:Configuration=Release;Platform=%MCD%
	)
)

rem Build the utilities
cd tools/utilities
IF DEFINED GMSEC_VC6 (
	call "C:\tools\VC98\Bin\VCVARS32.BAT"
	MSBuild.exe utilities.sln /t:Rebuild /p:Configuration=Release /p:"VCBuildAdditionalOptions= /useenv"
) ELSE (
	MSBuild.exe utilities.sln /t:Rebuild /p:Configuration=Release;Platform=%MCD%
)
cd %_startPath%

:Perl
echo.
echo.
echo ###########################################################
echo #
echo #  Building Perl binding for API 3.x
echo #
echo ###########################################################
cd perl\gmsec
perl -Iextra Makefile.PL PREFIX=../../bin CC=cl LD=link OPTIMIZE=/O2
call fixMakefiles.bat
nmake
nmake install
cd %_startPath%

echo ON

rem Copy validator scripts
mkdir bin\validator
copy validator\env_validator.bat bin\validator
copy validator\get_arch.pl bin\validator
copy validator\perl_ver.pl bin\validator
copy validator\*.env bin\validator

