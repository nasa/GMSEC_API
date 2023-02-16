Instructions for running C# example programs


1. Configure the running environment

   For Linux:

      export GMSEC_API_HOME=/path/to/GMSEC_API

      export GMSEC_PLATFORM=linux.x86_64

      export MONO_PATH=$GMSEC_API_HOME/bin

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin


   Hint: Place the export statements in ~/.bashrc.


   For Windows:

      set GMSEC_API_HOME=C:\path\to\GMSEC_API

      set PATH=%PATH%;%GMSEC_API_HOME%\bin


   Hint: Define the variables within Environment Variables for Your Account.



2. Run example program using Bolt

   For Linux:

      cd $GMSEC_API_HOME/examples/csharp

      make

      cd <example_name>

      mono ./<example_name> mw-id=bolt mw-server=localhost


   For Windows:

      Load %GMSEC_API_HOME%\examples\csharp\cs.sln using Visual Studio 2019;
      then build all example programs.

      cd %GMSEC_API_HOME%\examples\csharp\<example_name>

      ..\bin\<example_name>.exe mw-id=bolt mw-server=localhost


   For other middleware, refer to the GMSEC API User's Guide for additional
   information.



3. Contact info

   For any issues, contact the GMSEC API team at GMSEC-support@lists.nasa.gov

