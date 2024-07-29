This README file provides general information regarding the GMSEC API, its
supported middleware, and example programs.



IMPORTANT NOTE:
   The open-source distribution of the GMSEC API only includes C2MS message
   templates. All C2MS-PIPE (formerly known as C2MS-11b) and NASA/GMSEC Addendum
   to C2MS message templates have been removed from the open-source distribution
   of the GMSEC API, including within releases posted in the past.

   If attempting to use releases 4.5 through 4.9.1, use the connection config-
   uration option gmsec-schema-level=0 to load the C2MS message templates.

   If C2MS-PIPE or GMSEC message templates are required, please send a request
   to gmsec-support@lists.nasa.gov for further guidance on how to acquire these.



1. DISTRIBUTIONS

   The GMSEC API is available in two different distributions (Open-Source and
   Government version) for the following OSes:

      * Windows 10
      * Windows 11
      * RHEL 8
      * Ubuntu 20.04 LTS
      * Ubuntu 20.04 ARM/Aarch64 LTS
      * MacOS x86-64

   Binaries and source code for the Open-Source version is available on NASA
   GitHub. See here for further details: https://github.com/nasa/GMSEC_API

   Instructions for building the Open-Source code can be found in the
   INSTALL.txt file located at the root folder level where the GMSEC API project
   is installed.

   As for the Government version, it is only available in binary format (no
   source code), and more importantly it is only available by request from GMSEC
   by those supporting a US Government contract. This version contains the
   C2-NSS interface (for NSS message encrption); for users that do not
   require this feature, then it is recommended to use the Open-Source
   distribution.



2. DOCUMENTATION

   The GMSEC API includes documentation is the docs directory which is located
   in the base directory where the GMSEC API has been installed.

   There the following documents exist:

      * GMSEC API Installation and Configuration Guide
      * GMSEC API SRS
      * GMSEC API User's Guide
      * GMSEC API VDD
      * GMSEC API CompatC2 Installation and Configuration Guide [see note below]
      * NASA C2MS Addendum [see note below]

   In addition, Doxygen style documentation for the GMSEC API may be found in
   the docs/manual folder. Open index.html for the appropriate language binding 
   using a web browser.

   Note:
   These documents are *not* included with the Open-Source release of the GMSEC 
   API.



3. SOFTWARE INSTALLATION and SYSTEM CONFIGURATION

   The GMSEC API can be installed anywhere on a system, however it is
   recommended to choose a folder that does not contain whitespace. Special user
   accounts are not necessary to use the GMSEC API, nor are admin privileges.

   Once the software is installed, it is recommended to reference its location
   using the GMSEC_API_HOME environment variable, and then to use this to
   configure other environment variables. For example:

   On Linux:

      export GMSEC_API_HOME=/opt/GMSEC/GMSEC_API

      export LD_LIBRARY_PATH=$GMSEC_API_HOME/bin:$LD_LIBRARY_PATH

      export PATH=$PATH:$GMSEC_API_HOME/bin

      export CLASSPATH=$GMSEC_API_HOME/bin/gmsecapi.jar:$CLASSPATH


   On Windows:

      set GMSEC_API_HOME=C:\GMSEC\GMSEC_API

      set PATH=%PATH%;%GMSEC_API_HOME%\bin

      set CLASSPATH=%GMSEC_API_HOME%\bin\gmsecapi.jar;%CLASSPATH%



4. MIDDLEWARE BROKERS

   The GMSEC API supports various middleware brokers; these include:

      * Apache ActiveMQ
      * Apache Artemis
      * Bolt
      * IBM MQ
      * Message Bus
      * OpenDDS
      * RabbitMQ (using AMQP 1.0)
      * ZeroMQ

   Information on acquiring, installing, and running a middleware broker may be
   found in the GMSEC API Installation and Configuration Guide.



5. MIDDLEWARE CLIENT LIBRARIES

   To interact with a middleware broker, the GMSEC API will need to rely on
   vendor-specific client libraries. Typically these are downloaded from the
   respective vendor. The exception is with Bolt and Message Bus whose libraries
   are included with the GMSEC API distribution.

   The Apache ActiveMQ client libraries (which is also used for Artemis) can be
   obtained by request from the GMSEC team. Alternatively, the user can build
   these themselves from source code.

   Once installed, middleware client libraries should be referenced; for
   example:

   On Linux:

      export LD_LIBRARY_PATH=/path/to/client/lib:$LD_LIBRARY_PATH


   On Windows:

      set PATH=%PATH%;C:\path\to\client\lib



6. UTILITIES/TOOLS

   The GMSEC API provides a small suite of utility/tool applications. These
   include:

      * gmconfig_edit      # console app to assist creating a configuration file
      * gmhelp             # provides general middleware information
      * gmlog              # publishes a Log message
      * gmpub              # publishes a Heartbeat message
      * gmreq              # issues a Request Directive message
      * gmrpl              # issues a Response Directive message
      * gmsub              # subscribes to receive messages
      * msg_schema_ids     # lists available schema IDs
      * throughput_pub     # stress test that publishes messages
      * throughput_sub     # stress test that subscribes to receive messages



7. SUPPORTED PROGRAMMING LANGUAGE BINDINGS

   The GMSEC API provides support for multiple programming languages. These
   include:

      * C
      * C++
      * C#
      * Java
      * Node.js
      * Perl
      * Python3
      * Ruby

   Information with regards to developing applications using these bindings can
   be found in the respective sections of the GMSEC API User's Guide.

   Notes:

      1. Mono is required to use C# under Linux and MacOS
      2. Strawberry Perl 5.32.1 is required for Windows 10 and 11
      3. Python 3.10.7 (or later 3.10.x) is required for Windows 10 and 11
      4. Ruby 3.2.2 is required for Windows 10 and 11
      5. Node.js 18.15.0 (or later) is required for Windows 10 and 11



8. EXAMPLE PROGRAMS

   The GMSEC API provides a suite of example programs located under the examples
   directory located at the base directory where the GMSEC API distribution is
   installed. Each examples directory contains a README.txt file with
   information on how to configure one's environment to run the example
   programs.

   Those that are required to be compiled/linked will need for the working
   system to have development tools installed. For Linux, this will, at a
   minimum, require GCC and Make. For Windows, Visual Studio 2019 or later will
   be required.



9. SUPPORT

   For GMSEC API and for basic middleware support, feel free to send queries to
   gmsec-support@lists.nasa.gov.


