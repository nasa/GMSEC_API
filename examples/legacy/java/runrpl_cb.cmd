 
rem Copyright 2007-2018 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.







 
set PATH=..\..\bin;%PATH%
java -Djava.library.path=..\..\bin -cp ..\..\bin\gmsecapi.jar;. gmrpl_cb %*
