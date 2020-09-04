
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
