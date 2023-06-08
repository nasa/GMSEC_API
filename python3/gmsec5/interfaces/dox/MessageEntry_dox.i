/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") MessageEntry "

    This class provides access to the attributes associated with a
    ConfigFile's message entry.
";

%feature("docstring") MessageEntry::getName "

    get_name(self) -> str

    Returns the name associated with the message entry.
";

%feature("docstring") MessageEntry::getMessage "

    get_message(self) -> Message

    Returns the Message associated with the message entry.
";

