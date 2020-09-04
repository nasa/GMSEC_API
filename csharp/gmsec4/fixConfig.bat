@ECHO OFF


REM Copyright 2007-2018 United States Government as represented by the
REM Administrator of The National Aeronautics and Space Administration.
REM No copyright is claimed in the United States under Title 17, U.S. Code.
REM All Rights Reserved.


set OLD_TEXT=public class Config : global::System.IDisposable {

set "NEW_TEXT=public class Config : global::System.IDisposable {\n\n"
set "NEW_TEXT=%NEW_TEXT% public static Config Initialize\(string[] args\) {\n"
set "NEW_TEXT=%NEW_TEXT%        Config config = new Config\(\);\n"
set "NEW_TEXT=%NEW_TEXT%        for \(int i = 0; i < args.Length; ++i\) {\n"
set "NEW_TEXT=%NEW_TEXT%            string[] argVal = args[i].Split\('='\);\n"
set "NEW_TEXT=%NEW_TEXT%            if \(argVal.Length == 2\) {\n"
set "NEW_TEXT=%NEW_TEXT%                config.AddValue\(argVal[0], argVal[1]\);\n"
set "NEW_TEXT=%NEW_TEXT%            }\n"
set "NEW_TEXT=%NEW_TEXT%         }\n"
set "NEW_TEXT=%NEW_TEXT%         return config;\n"
set "NEW_TEXT=%NEW_TEXT%    }\n\n"


perl -pi.bak -e "s/%OLD_TEXT%/%NEW_TEXT%/" interfaces/Config.cs


@ECHO ON
