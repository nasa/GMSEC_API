rem Copyright 2007-2023 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

REM Build Bolt server for Windows

RMDIR /S /Q classes
MKDIR classes

"%JDK_HOME%"\bin\java -version
"%JDK_HOME%"\bin\javac -d classes --release 8 java\bolt\*.java
"%JDK_HOME%"\bin\jar cvfm bolt.jar java\MANIFEST -C classes .

REM Copy to the bin directory
COPY /Y bolt.jar ..\..\bin\bolt.jar
