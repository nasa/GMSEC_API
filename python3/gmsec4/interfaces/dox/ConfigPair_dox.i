
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
