Instructions for running Python3 example programs


1. Configure the running environment

   For Linux:

      export GMSEC_API_HOME=/path/to/GMSEC_API

      export PYTHONPATH=$GMSEC_API_HOME/bin:$GMSEC_API_HOME/bin/lib/GMSECAPI5:.

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin


   Hint: Place the export statements in ~/.bashrc.


   For Windows:

      set PYTHON3_HOME=C:\path\to\Python310

      set GMSEC_API_HOME=C:\path\to\GMSEC_API

      set PYTHONPATH=%GMSEC_API_HOME%\bin;%GMSEC_API_HOME%\bin\lib\GMSECAPI5;.

      set PATH=%PATH%;%GMSEC_API_HOME%\bin

      set PATH=%PATH%;%PYTHON3_HOME%


   Hint: Define the variables within Environment Variables for Your Account.



2. Run example program using Bolt

   For Linux:

      cd $GMSEC_API_HOME/examples/python3

      ./<example_name>.py mw-id=bolt mw-server=localhost


   For Windows:

      python <example_name>.py mw-id=bolt mw-server=localhost

      Note: Use Python 3.10.7 or later.


   For other middleware, refer to the GMSEC API User's Guide for additional
   information.



3. Contact info

   For any issues, contact the GMSEC API team at GMSEC-support@lists.nasa.gov

