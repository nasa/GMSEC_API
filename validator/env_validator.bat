@echo off

rem Copyright 2007-2016 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.





setlocal EnableDelayedExpansion
setlocal EnableExtensions

set version=

if exist ..\..\include\gmsec_version.h (

	findstr /L GMSEC_VERSION_NUMBER ..\..\include\gmsec_version.h > tempversion.txt

	for /f "tokens=3" %%i in (tempversion.txt) do set version=%%i

	set version=!version:~1,-1!

	del tempversion.txt
) else (
	if exist ..\version.inf (
		set /p version=<..\version.inf
	) else (
		set version=__API_VERSION__
	)
)

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "DEL=%%a"
)

REM Prepare a file "X" with only one dot
<NUL > X set /p ".=."


REM ##########################################################################
REM ##
REM ##  Validate input arguments, if any.
REM ##
REM ##########################################################################
echo.
SET mypath=%~dp0


if "%1" == "" (
	call:Error "Error: Middleware type missing."
	call:ShowUsage
	goto:eof
)


set system_setup=0
set check_jms=0

if not "%2" == "" (
	if /i "%2" == "check_jms" (
		set check_jms=1
	) else (
		if /i not "%2" == "check_jms" (
			call:Error "Error: Invalid string for check_JMS parameter; found '%2'"
			goto:ShowUsage
		)
	)
)


call:Header "Starting GMSEC System Environment Validator Tool (version %version%)..."


REM ##########################################################################
REM ##
REM ##  Adjust the given middleware type to include "gmsec_" (if needed).
REM ##
REM ##########################################################################
echo.%1 | findstr /C:"gmsec_" 1>NUL
if !errorlevel! == 1 (set mw_type=gmsec_%1) else (set mw_type=%1)



REM ##########################################################################
REM ##
REM ## Deduce the GMSEC API installation path by searching through PATH for
REM ## the gmsecapi.dll file.
REM ##
REM ##########################################################################
call:CheckGMSEC



REM ##########################################################################
REM ##
REM ## Check Java settings.
REM ##
REM ##########################################################################
echo.
call:CheckJava



REM ##########################################################################
REM ##
REM ## Check Perl settings.
REM ##
REM ##########################################################################
echo.
call:CheckPerl



REM ##########################################################################
REM ##
REM ## Find dependencies for the middleware.
REM ##
REM ##########################################################################
echo %mw_type% | findstr "gmsec_activemq" 1>NUL
if !errorlevel! == 0 (
	call:CheckActiveMQ
	goto endhere
)

echo %mw_type% | findstr "gmsec_amqp" 1>NUL
if !errorlevel! == 0 (
	call:CheckAMQP
	goto endhere
)


echo %mw_type% | findstr "gmsec_apollo" 1>NUL
if !errorlevel! == 0 (
	call:CheckApollo
	goto endhere
)

echo %mw_type% | findstr "gmsec_bolt" 1>NUL
if !errorlevel! == 0 (
	call:CheckBolt
	goto endhere
)

echo %mw_type% | findstr "gmsec_mb" 1>NUL
if !errorlevel! == 0 (
	call:CheckMessageBus
	goto endhere
)

echo %mw_type% | findstr "gmsec_ss" 1>NUL
if !errorlevel! == 0 (
	call:CheckSmartSockets
	goto endhere
)

echo %mw_type% | findstr "gmsec_weblogic" 1>NUL
if !errorlevel! == 0 (
	call:CheckWeblogic
	goto endhere
)

echo %mw_type% | findstr "gmsec_websphere" 1>NUL
if !errorlevel! == 0 (
	call:CheckWebsphere
	goto endhere
)

call:UnknownMiddleware

:endhere

REM ##########################################################################
REM ##
REM ## All done; show summary
REM ##
REM ##########################################################################
:Summary
echo.
echo.          -----------------------------------------------------
echo.
if %system_setup% == 0 (
	call:Ok "Congratulations!  Your system is properly configured."
	echo.
	call:Ok "However, if there are any WARNING(s) listed above, carefully consider"
	call:Ok "whether they need to be corrected for your operational needs."
) else (
	call:Error "Please correct the FAILURE(s) and optionally any WARNING(s) shown above"
	call:Error "before proceeding to use the GMSEC API."
	echo.
	call:Error "Refer to both the 'GMSEC API Installation Guide' and the 'Installation and"
	call:Error "Configuration Guide' (for GMSEC Middleware Installation) for assistance"
	call:Error "in resolving the issue(s)."
)

del X
exit /b %system_setup%


REM ##########################################################################
REM ##
REM ## Functions start below...
REM ##
REM ##########################################################################


REM ##########################################################################
REM ##
REM ## ShowUsage
REM ##
REM ##########################################################################
:ShowUsage
echo.
echo.Usage: env_validator.bat ^<middleware^> [check_JMS]
echo.
echo.where ^<middleware^> is one of the following:
echo.
echo.       activemq383
echo.		apollo383
echo.		amqp
echo.       bolt
echo.       mb
echo.       ss66, ss67, ss68, ss681, ss682 (for TIBCO Smart Sockets)
echo.       weblogic11
echo.       websphere71, websphere75
echo.
echo.Note: It is also acceptable to preface any of the above middleware types
echo.      with 'gmsec_'.  For example, 'gmsec_activemq383'.
echo.
echo.
echo.The [check_JMS] is an optional argument that can be used to check if the
echo.installed Java Runtime Environment is compatible with the GMSEC JMS wrapper
echo.and whether the appropriate JRE libraries have been found.
goto:eof


REM ##########################################################################
REM ##
REM ## FindInPath
REM ##
REM ##########################################################################
:FindInPath
set result=""
set temppath="%PATH:;=";"%"
for %%d in (%temppath%) do (
	if exist %%~d\%~1 (
		set result=%%~d
		goto:eof
	)
)
goto:eof


REM ##########################################################################
REM ##
REM ## FindInClassPath
REM ##
REM ##########################################################################
:FindInClassPath
set result=""
set temppath="%CLASSPATH:;=";"%"
for %%d in (%temppath%) do (
	echo.%%d | findstr /C:"%~1" 1>NUL
	if !errorlevel! == 0 (
		set result=%%d
		goto:eof
	) 
)
goto:eof


REM ##########################################################################
REM ##
REM ## Header
REM ##
REM ##########################################################################
:Header
call:Color 03 "%~1"
echo.
exit /b
goto:eof


REM ##########################################################################
REM ##
REM ## Ok
REM ##
REM ##########################################################################
:Ok
call:Color 02 "%~1"
echo.
exit /b
goto:eof


REM ##########################################################################
REM ##
REM ## Error
REM ##
REM ##########################################################################
:Error
call:Color 04 "%~1"
echo.
exit /b
goto:eof


REM ##########################################################################
REM ##
REM ## Success
REM ##
REM ##########################################################################
:Success
call:Color 02 "    +++ SUCCESS: %~1"
echo.
exit /b
goto:eof


REM ##########################################################################
REM ##
REM ## Failure
REM ##
REM ##########################################################################
:Failure
call:Color 04 "    --- FAILURE: %~1"
echo.
set system_setup=1
goto:eof


REM ##########################################################################
REM ##
REM ## Warning
REM ##
REM ##########################################################################
:Warning
call:Color 06 "    ~~~ WARNING: %~1"
echo.
goto:eof


REM ##########################################################################
REM ##
REM ## CheckGMSEC
REM ##
REM ##########################################################################
:CheckGMSEC
echo.
call:Header "Checking GMSEC API setup..."
call:FindInPath gmsecapi.dll
if %result% == "" (
	call:Failure "Core GMSEC library gmsecapi.dll not found while searching PATH"
	set system_setup=1
	goto:eof
) else (
	call:Success "Core GMSEC library gmsecapi.dll found using PATH"
	set GMSEC_BIN=%result%
)

call:CheckValidDLL %GMSEC_BIN%\gmsecapi.dll

for %%j in ( gmsecapi.jar ) do (

	call:FindInClassPath %%j
	if !result! == "" (
		call:Warning "%%j not found in CLASSPATH
	) else (
		call:Success "%%j in CLASSPATH"
	)
)

echo.
call:Header "Checking GMSEC C2 and C2_NSS Extensions setup..."
if exist %GMSEC_BIN%\gmsec_c2.dll (
	call::Success "GMSEC C2 Extensions are available"
	call::CheckDependencies %GMSEC_BIN%\gmsec_c2.dll 0
) else (
	call::Warning "GMSEC C2 library not found"
)
if exist %GMSEC_BIN%\gmsec_c2_nss.dll (
	echo.
	call::Success "GMSEC C2_NSS Extensions are available"
	call::CheckDependencies %GMSEC_BIN%\gmsec_c2_nss.dll 0
) else (
	call::Warning "GMSEC C2_NSS library not found"
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckValidDLL
REM ##
REM ##########################################################################
:CheckValidDLL
call:FindInPath perl.exe
if not !result! == "" (
	perl %mypath%\get_arch.pl %1 > %mypath%\api_arch.txt
	set /p api_arch=< %mypath%\api_arch.txt
	del %mypath%\api_arch.txt

	if "!api_arch!" == "!PROCESSOR_ARCHITECTURE!" (
		call:Success "%%~nx1 is compatible with a Windows !PROCESSOR_ARCHITECTURE! system"
	) else (
		call:Failure "%%~nx1 is NOT compatible with a Windows !PROCESSOR_ARCHITECTURE! system"
		set system_setup=1
		goto:eof
	)
) else (
	call:Warning "Unable to determine if %%~nx1 is compatible with a Windows !PROCESSOR_ARCHITECTURE! system"
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckDependencies
REM ##
REM ##########################################################################
:CheckDependencies
set libname=%1
set critical=%2

for /F %%i in ("%libname%") do set basename=%%~nxi

if exist %libname% (

	for /F %%d in ('findstr -i "\.dll" "%libname%" ^| C:\WINDOWS\System32\more ^| findstr -i "\.dll" ^| C:\WINDOWS\System32\more') do (

		set ignore_dll=0

		echo "%%d" | findstr /ri "msvcp*"   1>NUL && ( set ignore_dll=1 )
		echo "%%d" | findstr /ri "msvcr*"   1>NUL && ( set ignore_dll=1 )
		echo "%%d" | findstr /ri "kernel*"  1>NUL && ( set ignore_dll=1 )
		echo "%%d" | findstr /ri "ws2*"     1>NUL && ( set ignore_dll=1 )
		echo "%%d" | findstr /ri "wsock*"   1>NUL && ( set ignore_dll=1 )
		echo "%%d" | findstr /ri "winsock*" 1>NUL && ( set ignore_dll=1 )

		if !ignore_dll! == 0 (
			call:FindInPath %%d
			if !result! == "" (
				if !critical! == 1 (
					call:Failure "DLL file %%d cannot be found when scanning PATH"
					set system_setup=1
				) else (
					call:Warning "DLL file %%d cannot be found when scanning PATH"
				)
			) else (
				call:Success "Found DLL file %%d"
				call:CheckValidDLL "!result!\%%d"
			)
		)
	)
) else (
	call:Failure "Unable to locate %basename%"
	set system_setup=1
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckJava
REM ##
REM ##########################################################################
:CheckJava
call:Header "Checking Java setup..."
set found_java_home=""
set found_java_area=""
set min_java_ver=1.6.0

if defined JAVA_HOME (
	if exist "%JAVA_HOME%" (
		call:Success "JAVA_HOME is defined"
		set found_java_home="%JAVA_HOME%"
		set found_java_area="JAVA_HOME"
		goto verifyJava
	) else (
		call:Warning "JAVA_HOME is defined, but the path does not exist"
	)
) else (
	call:Warning "JAVA_HOME is not defined"
)

if defined JRE_HOME (
	if exist "%JRE_HOME%" (
		call:Success "JRE_HOME is defined"
		set found_java_home="%JRE_HOME%"
		set found_java_area="JRE_HOME"
		goto verifyJava
	) else (
		call:Warning "JRE_HOME is define, but the path does not exist"
	)
) else (
	call:Warning "JRE_HOME is not defined"
)

if defined JDK_HOME (
	if exist "%JDK_HOME%" (
			call:Success "JDK_HOME is defined"
			set found_java_home="%JDK_HOME%"
			set found_java_area="JDK_HOME"
			goto verifyJava
	) else (
		call:Warning "JDK_HOME is defined, but the path does not exist"
	)
) else (
	call:Warning "JDK_HOME is not defined"
)

call:FindInPath java.exe

if !result! == "" (
	call:Warning "Java not found in PATH"
	goto javaNotFound
) else (
	call:Success "Found Java using PATH"
	set found_java_home=!result!
	set found_java_area=""
	goto verifyJava
)

:verifyJava
if not %found_java_home% == "" (
	if %found_java_area% == "" (
		%found_java_home%\java -version 2> tmp_java_version.txt
		set java=%found_java_home%\java
	) else (
		%found_java_home%\bin\java -version 2> tmp_java_version.txt
		set java=%found_java_home%\bin\java
	)

	for /f "tokens=3" %%g in (tmp_java_version.txt) do (
		set java_version=%%~g
		goto:have_version
	)

:have_version
	del tmp_java_version.txt

	call:Success "Detected Java !java_version!"

	REM Verify whether the Java version we found is valid for GMSEC
	REM
	set cp="%GMSEC_BIN%\gmsecapi.jar"
	set validator="gov.nasa.gsfc.gmsecapi.util.SystemValidator"

	%java% -cp %cp% %validator% 2>&1 | findstr /i "java.lang.UnsupportedClassVersionError" 1> NUL

	if !errorlevel! == 0 (
		call:Failure "GMSEC is not compatible with Java !java_version!"
		set system_setup=1
	) else (
		call:Success "GMSEC is compatible with Java !java_version!"
	)

	if %check_jms% == 1 (
		if !java_version! geq %min_java_ver% (
			call:Success "GMSEC JMS wrapper compatible with Java !java_version!"
		) else (
			call:Failure "GMSEC JMS wrapper requires Java %min_java_ver% or later"
			set system_setup=1
		)
	)
)

echo.
call:Header "Checking dependencies for GMSEC Java Native Interface (JNI) library..."
if exist %GMSEC_BIN%\gmsec_jni.dll (
	call:Success "Found gmsec_jni.dll"
	echo.
	call:Header "Checking dependencies for gmsec_jni.dll..."
	call:CheckDependencies %GMSEC_BIN%\gmsec_jni.dll 0
) else (
	call:Warning "Unable to locate %GMSEC_BIN%\gmsec_jni.dll"
)
goto:eof

javaNotFound:
goto:eof


REM ##########################################################################
REM ##
REM ## CheckJMSCompatibility
REM ##
REM ##########################################################################
:CheckJMSCompatibility
set jms_dll=gmsec_generic_jms.dll
if exist %GMSEC_BIN%\%jms_dll% (
	call:Success "Found reference to %jms_dll%"
	echo.
	call:Header "Checking dependencies for %jms_dll%..."
	call:CheckDependencies %GMSEC_BIN%\%jms_dll% 1
) else (
	call:Failure "Unable to reference %jms_dll%"
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckEnvironmentVariables
REM ##
REM ##########################################################################
:CheckEnvironmentVariables
set env_file=%1.env
if exist %env_file% (
	echo.
	call:Header "Checking for optional %1 environment variables..."
	FOR /F "eol=# tokens=*" %%l IN ( %env_file% ) DO (
		IF DEFINED %%l (
			call::Success "Environment variable %%l is set to '!%%l!'"
		) else (
			call::Warning "Environment variable %%l is undefined"
		)
	)
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckPerl
REM ##
REM ##########################################################################
:CheckPerl
call:Header "Checking Perl setup..."
call:FindInPath perl.exe
if !result! == "" (
	call:Warning "Perl was not found"
) else (
	call:Success "Perl is installed"

	set min_perl_ver=5.8.0

	perl perl_ver.pl 1>NUl 2>NUL

	if !errorlevel! == 0 (
		call:Success "Perl with version !min_perl_ver! (or later) found"
	) else (
		call:Failure "Perl with version !min_perl_ver! (or later) not found"
	)
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckActiveMQ
REM ##
REM ##########################################################################
:CheckActiveMQ
echo.
call:Header "Checking middleware dependencies for ActiveMQ..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1

) else (
	call:Failure "Unable to reference %mw_type%.dll"
)

if %check_jms% == 1 (
	echo.
	call:Header "Checking GMSEC JMS support for ActiveMQ..."
	call:CheckJMSCompatibility

	echo.
	call:Header "Checking for JMS ActiveMQ JAR file(s)..."
	for %%j in ( activemq-all camel-spring slf4j-simple ) do (

		call:FindInClassPath %%j

		if not !result! == "" (
			call:Success "Reference to %%j JAR file found in CLASSPATH"
		) else (
			call:Failure "Reference to %%j JAR file not found in CLASSPATH"
			set system_setup=1
		)
	)
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckAMQP
REM ##
REM ##########################################################################
:CheckAMQP
echo.
call:Header "Checking middleware dependencies for AMQP..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1
) else (
	call:Failure "Unable to reference %mw_type%.dll"
)
echo.
call:Header "Checking for existence of AMQP XML configuration file..."
if exist %GMSEC_BIN%\amqp\amqp.xml (
	call:Success "Found amqp.xml configuration file"
) else (
	call:Failure "Unable to reference amqp.xml configuration file"
)

if %check_jms% == 1 (
	echo.
	call:Header "GMSEC JMS support is not offered for AMQP."
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckApollo
REM ##
REM ##########################################################################
:CheckApollo
echo.
call:Header "Checking middleware dependencies for Apollo (which uses ActiveMQ libraries)..."

set tmp_str=%mw_type%
set alt_mw_type=!tmp_str:apollo=activemq!

if exist %GMSEC_BIN%\%alt_mw_type%.dll (
	call:Success "Found %alt_mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %alt_mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%alt_mw_type%.dll 1

) else (
	call:Failure "Unable to reference %alt_mw_type%.dll"
)

if %check_jms% == 1 (
	echo.
	call:Header "GMSEC JMS support is not offered for Apollo."
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckBolt
REM ##
REM ##########################################################################
:CheckBolt
echo.
call:Header "Checking middleware dependencies for Bolt..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1

) else (
	call:Failure "Unable to reference %mw_type%.dll"
)
if %check_jms% == 1 (
	echo.
	call:Header "Checking GMSEC JMS support for GMSEC Bolt..."
	call:Warning "GMSEC JMS support not available for GMSEC Bolt"
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckMessageBus
REM ##
REM ##########################################################################
:CheckMessageBus
echo.
call:Header "Checking middleware dependencies for Message Bus..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1

) else (
	call:Failure "Unable to reference %mw_type%.dll"
)
if %check_jms% == 1 (
	echo.
	call:Header "Checking GMSEC JMS support for GMSEC Message Bus..."
	call:Warning "GMSEC JMS support not available for GMSEC Message Bus"
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckSmartSockets
REM ##
REM ##########################################################################
:CheckSmartSockets
echo.
call:Header "Checking middleware dependencies for SmartSockets..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1

) else (
	call:Failure "Unable to reference %mw_type%.dll"
)
if %check_jms% == 1 (
	echo.
	call:Header "Checking GMSEC JMS support for Smart Sockets..."
	call:Warning "GMSEC JMS support not available for Smart Sockets"
)
goto:eof


REM ##########################################################################
REM ##
REM ## CheckWeblogic
REM ##
REM ##########################################################################
:CheckWeblogic
echo.
call:Header "Checking middleware dependencies for WebLogic..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1

) else (
	call:Failure "Unable to reference %mw_type%.dll"
)

for %%j in ( wlfullclient.jar ) do (

	call:FindInClassPath %%j

	if not !result! == "" (
		call:Success "Reference to %%j JAR file found in CLASSPATH"
	) else (
		call:Failure "Reference to %%j JAR file not found in CLASSPATH"
		set system_setup=1
	)
)

if %check_jms% == 1 (
	echo.
	call:Header "Checking GMSEC JMS support for WebLogic..."
	call:CheckJMSCompatibility
)
call:CheckEnvironmentVariables weblogic
goto:eof


REM ##########################################################################
REM ##
REM ## CheckWebsphere
REM ##
REM ##########################################################################
:CheckWebsphere
echo.
call:Header "Checking middleware dependencies for WebSphere..."

if exist %GMSEC_BIN%\%mw_type%.dll (
	call:Success "Found %mw_type%.dll"
	echo.
	call:Header "Checking dependencies for %mw_type%.dll..."
	call:CheckDependencies %GMSEC_BIN%\%mw_type%.dll 1

) else (
	call:Failure "Unable to reference %mw_type%.dll"
)

if %check_jms% == 1 (
	echo.
	call:Header "Checking GMSEC JMS support for WebSphere..."
	call:CheckJMSCompatibility

	for %%j in ( com.ibm.mqjms.jar ) do (

		call:FindInClassPath %%j

		if not !result! == "" (
			call:Success "Reference to %%j JAR file found in CLASSPATH"
		) else (
			call:Failure "Reference to %%j JAR file not found in CLASSPATH"
			set system_setup=1
		)
	)
)
call:CheckEnvironmentVariables websphere
goto:eof


REM ##########################################################################
REM ##
REM ## UnknownMiddleware
REM ##
REM #########################################################################
:UnknownMiddleware
echo.
call:Header "Checking dependencies for unknown middleware type..."
call:Failure "Unable to reference %mw_type%.dll"
goto:eof


REM ##########################################################################
REM ##
REM ## Color
REM ##
REM #########################################################################
:color
set "param=^%~2" !
set "param=!param:"=\"!"
findstr /p /A:%1 "." "!param!\..\X" NUL
<NUL set /p ".=%DEL%%DEL%%DEL%%DEL%%DEL%%DEL%%DEL%"
exit /b
