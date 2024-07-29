Instructions for running Ruby example programs


1. Configure the running environment

   For Linux:

      export GMSEC_API_HOME=/path/to/GMSEC_API

      export RUBYLIB=$GMSEC_API_HOME/bin

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin


   Hint: Place the export statements in ~/.bashrc.


   For Windows:

      set RUBY_HOME=C:\path\to\Ruby-3.2.2

      set GMSEC_API_HOME=C:\path\to\GMSEC_API

      set RUBYLIB=%GMSEC_API_HOME%\bin

      set PATH=%PATH%;%GMSEC_API_HOME%\bin

      set PATH=%PATH%;%RUBY_HOME%\bin


   Hint: Define the variables within Environment Variables for Your Account.



2. Run example program using Bolt

   For Linux:

      cd $GMSEC_API_HOME/examples/ruby

      ./<example_name>.rb mw-id=bolt mw-server=localhost


   For Windows:

      ruby <example_name>.rb mw-id=bolt mw-server=localhost

      Note: For Windows, use ruby 3.2.2 (2023-03-30 revision e51014f9c0) [x64-mswin64_140].


   For other middleware, refer to the GMSEC API User's Guide for additional
   information.



3. Contact info

   For any issues, contact the GMSEC API team at GMSEC-support@lists.nasa.gov

