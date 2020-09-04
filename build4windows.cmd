rem    @echo OFF
rem Copyright 2007-2016 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.





rem This batch file builds everything for Microsoft Windows

rem Build the main part of the API

set BUILD=gmsecapi gmsec_java gmsec_jni generic_jms bolt mb MBServer dotnet libgmsec_perl gmhelp

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

IF DEFINED GMSEC_VC6 (
	call "C:\tools\VC98\Bin\VCVARS32.BAT"
	MSBuild.exe gmsecapi_allvendors.sln /t:Rebuild /p:Configuration=Release;Platform=Win32 /p:"VCBuildAdditionalOptions= /useenv"
	GOTO Perl
)

IF DEFINED GMSEC_VS2013 (
	call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64
)

IF DEFINED GMSEC_VS2010 (
	devenv "gmsecapi_allvendors.sln" /upgrade
	cd examples/cpp
	devenv "cpp.sln" /upgrade
	cd ../c
	devenv "c.sln" /upgrade
	cd ../cs
	devenv "cs.sln" /upgrade
	cd ../..
)

rem Always start off with a clean slate
MSBuild.exe gmsecapi_allvendors.sln /t:Clean /p:Configuration=Release;Platform=%MCD%
MSBuild.exe gmsecapi_allvendors.sln /t:Clean /p:Configuration=Release-SNK;Platform=%MCD%

rem Build each portion of the solution, in the desired order.
FOR %%i IN (%BUILD%) DO (
   IF '%%i'=='dotnet' (
	MSBuild.exe gmsecapi_allvendors.sln /t:%%i /p:Configuration=Release-SNK;Platform=%MCD%
   ) ELSE (
   	MSBuild.exe gmsecapi_allvendors.sln /t:%%i /p:Configuration=Release;Platform=%MCD%
   )
)

:Perl
cd perl\gmsec
rem Build the Perl part of the API
perl -Iextra Makefile.PL PREFIX=../../bin
nmake
nmake install
cd ..\..


rem Build the C++ examples

cd examples/cpp
IF DEFINED GMSEC_VC6 (
	call "C:\tools\VC98\Bin\VCVARS32.BAT"
	MSBuild.exe cpp.sln /t:Rebuild /p:Configuration=Release /p:"VCBuildAdditionalOptions= /useenv"
) ELSE (
	MSBuild.exe cpp.sln /t:Rebuild /p:Configuration=Release;Platform=%MCD%
)
cd ..

rem Build the C examples

cd c
IF DEFINED GMSEC_VC6 (
	call "C:\tools\VC98\Bin\VCVARS32.BAT"
	MSBuild.exe c.sln /t:Rebuild /p:Configuration=Release /p:"VCBuildAdditionalOptions= /useenv"
) ELSE (
	MSBuild.exe c.sln /t:Rebuild /p:Configuration=Release;Platform=%MCD%
)
cd ..

rem Build the C# examples

cd cs
IF DEFINED GMSEC_VS2010 (	
	MSBuild.exe cs.sln /t:Rebuild /p:TargetFrameworkVersion=v4.0;Configuration=Release;Platform="Any CPU"
) ELSE (
	MSBuild.exe cs.sln /t:Rebuild /p:Configuration=Release;Platform="Any CPU"
)
cd ../..

rem Copy validator scripts
mkdir bin\validator
copy validator\env_validator.bat bin\validator
copy validator\get_arch.pl bin\validator
copy validator\perl_ver.pl bin\validator
copy validator\*.env bin\validator
