REM Build Bolt server for Windows

RMDIR /S /Q classes
MKDIR classes

"%JDK_HOME%"\bin\java -version
"%JDK_HOME%"\bin\javac -d classes java\bolt\*.java
"%JDK_HOME%"\bin\jar cvfm bolt.jar java\MANIFEST -C classes .

REM Copy to the bin directory
COPY /Y bolt.jar ..\..\bin\bolt.jar
