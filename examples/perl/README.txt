Instructions for running Perl example programs


1. Configure the running environment

   For Linux:

      export GMSEC_API_HOME=/path/to/GMSEC_API

      export PERL5LIB=$GMSEC_API_HOME/bin:$GMSEC_API_HOME/bin/lib/GMSECAPI5:.

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin


   Hint: Place the export statements in ~/.bashrc.


   For Windows:

      set PERL5_HOME=C:\path\to\Strawberry

      set GMSEC_API_HOME=C:\path\to\GMSEC_API

      set PERL5LIB=%GMSEC_API_HOME%\bin;%GMSEC_API_HOME%\bin\lib\GMSECAPI5;.

      set PATH=%PATH%;%GMSEC_API_HOME%\bin

      set PATH=%PATH%;%PERL5_HOME%\c\bin

      set PATH=%PATH%;%PERL5_HOME%\perl\site\bin

      set PATH=%PATH%;%PERL5_HOME%\perl\bin


   Hint: Define the variables within Environment Variables for Your Account.



2. Run example program using Bolt

   For Linux:

      cd $GMSEC_API_HOME/examples/perl

      ./<example_name>.pl mw-id=bolt mw-server=localhost


   For Windows:

      perl <example_name>.pl mw-id=bolt mw-server=localhost


   Note: Use Strawberry Perl 5.32.1 or later under Windows.


   For other middleware, refer to the GMSEC API User's Guide for additional
   information.



3. Contact info

   For any issues, contact the GMSEC API team at GMSEC-support@lists.nasa.gov

