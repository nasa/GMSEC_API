
// File: classlibgmsec__python_1_1Mnemonic.xml


%feature("docstring") gmsec::api::mist::Mnemonic "

    This class is a lightweight container for holding information
    on a Mnemonic, and used to generate GMSEC Mnemonic messages by the
    ConnectionManager class

    See Also
    --------
    ConnectionManager

    CONSTRUCTORS:

    Mnemonic(self, name, samples)

    Initializes the Mnemonic object with a name and list of mnemonic samples.

    Parameters
    ----------
    name: Name of the Mnemonic.
    samples: List of Mnemonic samples (MnemonicSampleList object)

    Mnemonic(self, mnemonic)

    Initializes the Mnemonic object with the information from another mnemonic object.

    Parameters
    ----------
    mnemonic: The other Mnemonic object.
";

%feature("docstring") gmsec::api::mist::Mnemonic::getStatus "

    getStatus(self) -> Field

    Accessor for the Mnemonic status

    Returns
    -------
    A reference to the Mnemonic status field

    Exceptions
    ----------
    An exception is thrown if the status field has not been set
";

%feature("docstring") gmsec::api::mist::Mnemonic::getSampleCount "

    getSampleCount(self) -> size_t

    Returns the number of samples for this Mnemonic

    Returns
    -------
    a size_t representation of the number of samples
";

%feature("docstring") gmsec::api::mist::Mnemonic::unitsAvailable "

    unitsAvailable(self) -> bool

    Returns the availability of Mnemonic units

    Returns
    -------
    True if Mnemonic units have been set, false otherwise
";

%feature("docstring") gmsec::api::mist::Mnemonic::getUnits "

    getUnits(self) -> char const *

    Accessor for the Mnemonic units

    Returns
    -------
    A pointer to the Mnemonic units string

    Exceptions
    ----------
    An exception is thrown if the units string has not been set
";

%feature("docstring") gmsec::api::mist::Mnemonic::setStatus "

    setStatus(self, status)

    Supplies a Status for the Mnemonic and makes a copy of the field
";

%feature("docstring") gmsec::api::mist::Mnemonic::getName "

    getName(self) -> char const *

    Retrieves the name of the Mnemonic

    Returns
    -------
    The name of the Mnemonic
";

%feature("docstring") gmsec::api::mist::Mnemonic::addSample "

    addSample(self, sample)

    Adds a single MnemonicSample to the current list

    Parameters
    ----------
    sample: the new MnemonicSample object

";

%feature("docstring") gmsec::api::mist::Mnemonic::setUnits "

    setUnits(self, units)

    Supplies a units string for the Mnemonic and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::Mnemonic::getSample "

    getSample(self, idx) -> MnemonicSample const &

    Returns a reference to a MnemonicSample held in this object

    Parameters
    ----------
    idx: the zero-based index of the MnemonicSample object to access

    Returns
    -------
    A reference to the requested MnemonicSample object

    Exceptions
    ----------
    An exception if the index supplied is out of bounds

";

%feature("docstring") gmsec::api::mist::Mnemonic::statusAvailable "

    statusAvailable(self) -> bool

    Returns the availability of a Mnemonic status

    Returns
    -------
    True if a Mnemonic status has been set, false otherwise
";
