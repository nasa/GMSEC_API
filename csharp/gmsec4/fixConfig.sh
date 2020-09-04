#!/usr/bin/env bash

# Copyright 2007-2018 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GENERATED_SRC="interfaces/Config.cs"

OLD_TEXT="public class Config : global::System.IDisposable {"

NEW_TEXT=""
NEW_TEXT+="public class Config : global::System.IDisposable {\n\n"
NEW_TEXT+="    public static Config Initialize(string\[\] args) {\n"
NEW_TEXT+="        Config config = new Config();\n"
NEW_TEXT+="        for (int i = 0; i < args.Length; ++i) {\n"
NEW_TEXT+="            string\[\] argVal = args\[i\].Split('=');\n"
NEW_TEXT+="            if (argVal.Length == 2) {\n"
NEW_TEXT+="                config.AddValue(argVal\[0\], argVal\[1\]);\n"
NEW_TEXT+="            }\n"
NEW_TEXT+="         }\n"
NEW_TEXT+="         return config;\n"
NEW_TEXT+="    }\n\n"

sed -i "s/$OLD_TEXT/$NEW_TEXT/g" $GENERATED_SRC

exit 0
