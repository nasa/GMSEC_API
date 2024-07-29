@echo OFF

rem Copyright 2007-2024 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


rem This batch file builds everything for Microsoft Windows

rem Build the main part of the API

set BUILD=gmsec_api gmsec_java gmsec_jni generic_jms bolt mb MBServer loopback libgmsec_csharp libgmsec_perl libgmsec_python3 libgmsec_ruby gmsec_nodejs gmhelp

IF DEFINED WRAPPERS (
	set BUILD=%BUILD% %WRAPPERS%
) 

IF DEFINED USER_WRAPPERS (
	set BUILD=%BUILD% %USER_WRAPPERS%
)


set _startPath=%~dp0
IF DEFINED GMSEC_VS2017 (
    echo Calling Microsoft Visual Studio 17.0 vcvarsall script for x64 architecture
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
) ELSE IF DEFINED GMSEC_VS2019 (
	echo Calling Microsoft Visual Studio 19.0 vcvarsall script for x64 architecture
	call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
) ELSE (
	echo Only Visual Studio 2017 and 2019 are supported
	echo Set GMSEC_VS2017=1 or GMSEC_VS2019=1 and repeat the build
	goto:eof
)


cd %_startPath%


rem Always start off with a clean slate
MSBuild.exe gmsec_api_allvendors.sln /t:Clean /p:Configuration=Release;Platform=x64

rem Build each portion of the solution, in the desired order.
FOR %%i IN (%BUILD%) DO (
	echo.
	echo.
	echo ###########################################################
	echo #
	echo #  Building %%i
	echo #
	echo ###########################################################
	MSBuild.exe gmsec_api_allvendors.sln /t:%%i /p:Configuration=Release;Platform=x64
)

rem Build the utilities
cd tools/utilities
MSBuild.exe utilities.sln /t:Rebuild /p:Configuration=Release;Platform=x64

cd %_startPath%

rem Copy validator scripts
mkdir bin\validator
copy validator\env_validator.bat bin\validator
copy validator\get_arch.pl bin\validator
copy validator\perl_ver.pl bin\validator
copy validator\python_ver.py bin\validator
copy validator\*.env bin\validator

@echo ON

:eof
