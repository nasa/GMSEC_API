Instructions for running Java example programs


1. Configure the running environment

   For Linux:

      export JDK_HOME=/path/to/java_jdk

      export GMSEC_API_HOME=/path/to/GMSEC_API

      export CLASSPATH=$GMSEC_API_HOME/bin/gmsecapi.jar:.

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin


   Hint: Place the export statements in ~/.bashrc.


   For Windows:

      set JDK_HOME=C:\path\to\java_jdk

      set GMSEC_API_HOME=C:\path\to\GMSEC_API

      set CLASSPATH=%GMSEC_API_HOME%\bin\gmsecapi.jar;.

      set PATH=%PATH%;%GMSEC_API_HOME%\bin

      set PATH=%PATH%;%JDK_HOME%\bin


   Hint: Define the variables within Environment Variables for Your Account.


   IMPORTANT:
   The GMSEC API's Java binding is built using OpenJDK 11. Use a similar
   or later version of Java to build and run the example programs. Set the
   environment variable JDK_HOME to reference the path where OpenJDK is
   installed.



2. Run example program using Bolt

   For Linux:

      cd $GMSEC_API_HOME/examples/java

      ./compileAll.sh

      $JDK_HOME/bin/java <example_name> mw-id=bolt mw-server=localhost


   For Windows:

      cd %GMSEC_API_HOME%\examples\java

      compileAll.cmd

      %JDK_HOME%\bin\java <example_name> mw-id=bolt mw-server=localhost


   For other middleware, refer to the GMSEC API User's Guide for additional
   information.



3. Contact info

   For any issues, contact the GMSEC API team at GMSEC-support@lists.nasa.gov

