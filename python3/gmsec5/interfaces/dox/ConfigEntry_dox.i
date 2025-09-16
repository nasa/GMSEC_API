/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") ConfigEntry "

    This class provides access to the attributes associated with a
    ConfigFile's Config entry.
";


%feature("docstring") ConfigEntry::getConfig "

    get_config(self) -> Config

    Returns the Config object associated with the Config entry.

";


%feature("docstring") ConfigEntry::getName "

    get_name(self) -> str

    Returns the name associated with the Config entry.

";
