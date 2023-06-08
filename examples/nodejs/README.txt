Instructions for running NodeJS example programs


1. Configure the running environment

   For Linux:

      export GMSEC_API_HOME=/path/to/GMSEC_API

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin

      export NODEJS_HOME=/path/to/node-18.12.1

      export PATH=$GMSEC_API_HOME/bin:$PATH

      export PATH=$NODEJS_HOME:$PATH


   Hint: Place the export statements in ~/.bashrc.


   For Windows:

      set GMSEC_API_HOME=C:\path\to\GMSEC_API

      set NODEJS_HOME=C:\path\to\node-18.12.1

      set PATH=%GMSEC_API_HOME%\bin;%PATH%

      set PATH=%NODEJS_HOME%;%PATH%


   Hint: Define the variables within Environment Variables for Your Account.



2. Run example program using Bolt

   For Linux:

      cd $GMSEC_API_HOME/examples/nodejs

      ./<example_name>.js mw-id=bolt mw-server=localhost


   For Windows:

      node <example_name>.js mw-id=bolt mw-server=localhost


   Note: Use NodeJS 18.12.1 or later.


   For other middleware, refer to the GMSEC API User's Guide for additional
   information.



3. Contact info

   For any issues, contact the GMSEC API team at GMSEC-support@lists.nasa.gov

