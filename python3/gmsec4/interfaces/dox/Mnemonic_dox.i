
%feature("docstring") gmsec::api::mist::Mnemonic "

    This class is a lightweight container for holding information
    on a Mnemonic, and used to generate GMSEC Mnemonic messages by the
    ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTORS:

    Mnemonic(name: str, samples: MnemonicSampleList)

    Initializes the Mnemonic with a name and list of mnemonic
    samples.

    Parameters
    ----------
    name    : Name of the Mnemonic
    samples : MnemonicSampleList of MnemonicSamples



    Mnemonic(other: Mnemonic)

    Initializes the Mnemonic with the information from another
    mnemonic.

    Parameters
    ----------
    other : The other Mnemonic to copy.
";

%feature("docstring") gmsec::api::mist::Mnemonic::getStatus "

    get_status(self) -> Field

    Accessor for the Mnemonic status

    Returns
    -------
    A reference to the Mnemonic status field

    Exceptions
    ----------
    A GmsecError is thrown if the status field has not been set
";

%feature("docstring") gmsec::api::mist::Mnemonic::getSampleCount "

    get_sample_count(self) -> int

    Returns the number of samples for this Mnemonic

    Returns
    -------
    The number of samples
";

%feature("docstring") gmsec::api::mist::Mnemonic::unitsAvailable "

    units_available(self) -> bool

    Returns the availability of Mnemonic units

    Returns
    -------
    True if Mnemonic units have been set, False otherwise
";

%feature("docstring") gmsec::api::mist::Mnemonic::getUnits "

    get_units(self) -> str

    Accessor for the Mnemonic units

    Returns
    -------
    A pointer to the Mnemonic units string

    Exceptions
    ----------
    A GmsecError is thrown if the units string has not been set
";

%feature("docstring") gmsec::api::mist::Mnemonic::setStatus "

    set_status(self, status: Field)

    Supplies a Status for the Mnemonic and makes a copy of the field

    Parameters
    ----------
    status : The Field representing the status for the Mnemonic
";

%feature("docstring") gmsec::api::mist::Mnemonic::getName "

    get_name(self) -> str

    Retrieves the name of the Mnemonic

    Returns
    -------
    The name of the Mnemonic
";

%feature("docstring") gmsec::api::mist::Mnemonic::addSample "

    add_sample(self, sample: MnemonicSample)

    Adds a single MnemonicSample to the current list

    Parameters
    ----------
    sample : The MnemonicSample to add

";

%feature("docstring") gmsec::api::mist::Mnemonic::setUnits "

    set_units(self, units: str)

    Supplies a units string for the Mnemonic and makes a copy of the string

    Parameters
    ----------
    units : The units string
";

%feature("docstring") gmsec::api::mist::Mnemonic::getSample "

    get_sample(self, index: int) -> MnemonicSample

    Returns a reference to a MnemonicSample

    Parameters
    ----------
    index : The zero-based index of the MnemonicSample to access

    Returns
    -------
    A reference to the requested MnemonicSample

    Exceptions
    ----------
    A GmsecError if the index supplied is out of bounds

";

%feature("docstring") gmsec::api::mist::Mnemonic::statusAvailable "

    status_available(self) -> bool

    Returns the availability of a Mnemonic status

    Returns
    -------
    True if a Mnemonic status has been set; False otherwise
";
