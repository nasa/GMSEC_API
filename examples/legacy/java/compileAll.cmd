 
@ECHO OFF
rem Copyright 2007-2018 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.







 
@ECHO OFF
echo Building java examples...
javac -classpath ..\..\bin\gmsecapi.jar;. *.java

