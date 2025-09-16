@ECHO OFF
rem Copyright 2007-2025 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.


if "%JDK_HOME%" == "" (
	echo Error: JDK_HOME is not defined
	echo.
	echo Set JDK_HOME to reference the path where OpenJDK 11 or later version is installed
	echo.
	echo Aborting
	goto:eof
)

echo Building java examples...
%JDK_HOME%\bin\javac -classpath ..\..\bin\gmsecapi.jar;. -Xlint:deprecation *.java

