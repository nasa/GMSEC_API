/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") ConfigPair "

    Class that wraps a name value pair in a Config

    See Also
    --------
    Config
";


%feature("docstring") ConfigPair::getName "

    get_name(self) -> str

    Returns the name associated with the ConfigPair
";


%feature("docstring") ConfigPair::getValue "

    get_value(self) -> str

    Returns the value associated with the ConfigPair
";
