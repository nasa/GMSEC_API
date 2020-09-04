rem Copyright 2007-2016 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.






rem
rem This batch file builds the open source API for Microsoft Windows
rem

rem Build the main part of the API

IF DEFINED GMSEC_x64 (
MSBuild.exe gmsecapi_opensource.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64
) ELSE (
   IF DEFINED GMSEC_VC6 (
      call "C:\tools\VC98\Bin\VCVARS32.BAT"
      MSBuild.exe gmsecapi_opensource.sln /t:Rebuild /p:Configuration=Release /p:"VCBuildAdditionalOptions= /useenv"
   ) ELSE (
      MSBuild.exe gmsecapi_opensource.sln /t:Rebuild /p:Configuration=Release
   )
)


cd perl
rem Build the Perl part of the API
perl -Iextra Makefile.PL PREFIX=../bin
nmake
nmake install
cd ..

rem Build the C++ examples

cd examples/cpp
IF DEFINED GMSEC_x64 (
MSBuild.exe cpp.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64
) ELSE (
   IF DEFINED GMSEC_VC6 (
      call "C:\tools\VC98\Bin\VCVARS32.BAT"
      MSBuild.exe cpp.sln /t:Rebuild /p:Configuration=Release /p:"VCBuildAdditionalOptions= /useenv"
   ) ELSE (
      MSBuild.exe cpp.sln /t:Rebuild /p:Configuration=Release
   )
)
cd ..

rem Build the C examples

cd c
IF DEFINED GMSEC_x64 (
MSBuild.exe c.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64
) ELSE (
   IF DEFINED GMSEC_VC6 (
      call "C:\tools\VC98\Bin\VCVARS32.BAT"
      MSBuild.exe c.sln /t:Rebuild /p:Configuration=Release /p:"VCBuildAdditionalOptions= /useenv"
   ) ELSE (
      MSBuild.exe c.sln /t:Rebuild /p:Configuration=Release
   )
)
cd ../..

rem Copy validator scripts
mkdir bin\validator
copy validator\env_validator.bat bin\validator
copy validator\get_arch.pl bin\validator
copy validator\perl_ver.pl bin\validator
copy validator\*.env bin\validator
