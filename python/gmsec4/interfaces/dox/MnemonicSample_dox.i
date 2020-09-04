
// File: classlibgmsec__python_1_1MnemonicSample.xml


%feature("docstring") gmsec::api::mist::MnemonicSample "

    This class is a lightweight container for holding information
    on a Mnemonic Sample, and used to generate GMSEC Mnemonic messages by the
    ConnectionManager and Device classes

    See Also
    --------
    Mnemonic
    ConnectionManager

    CONSTRUCTORS:

    MnemonicSample(self, timestamp, rawValue)

    Initializes the MnemonicSample object with a timestamp and value.

    Parameters
    ----------
    timestamp: The time at which the sampled value was taken.
    rawValue: The raw value of the parameter.

    MnemonicSample(self, other)

    Copy constructor - Initializes the MnemonicSample object from another MnemonicSample.

    Parameters
    ----------
    other: The other MnemonicSample to inherit values from.
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getQuality "

    getQuality(self) -> bool

    Accessor for the quality

    Returns
    -------
    The MnemonicSample quality

    Exceptions
    ----------
    An exception is thrown if the quality flag has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasQuality "

    hasQuality(self) -> bool

    Returns the availability of a MnemonicSample quality flag

    Returns
    -------
    True if MnemonicSample quality has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getLimit "

    getLimit(self) -> gmsec::api::mist::MnemonicSample::LimitFlag

    Accessor for the limit

    Returns
    -------
    The MnemonicSample limit

    Exceptions
    ----------
    An exception is thrown if the limit has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setQuality "

    setQuality(self, quality)

    Supplies a quality flag for the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setLimitEnableDisable "

    setLimitEnableDisable(self, value)

    Supplies a boolean flag for the MnemonicSample setLimitEnableDisable(bool value)
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setTextValue "

    setTextValue(self, value)

    Supplies a string for the MnemonicSample text value and makes a copy of the string
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasEUValue "

    hasEUValue(self) -> bool

    Returns the availability of a MnemonicSample value in
    engineering units

    Returns
    -------
    True if MnemonicSample EU value has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::__ne__ "

    __ne__(self, mnemonicSample) -> bool

    Parameters
    ----------
    mnemonicSample: gmsec::api::mist::MnemonicSample const &
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getTextValue "

    getTextValue(self) -> char const *

    Accessor for the text value

    Returns
    -------
    A pointer to the MnemonicSample text value field

    Exceptions
    ----------
    An exception is thrown if the text value field has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasFlags "

    hasFlags(self) -> bool

    Returns the availability of a MnemonicSample flags field

    Returns
    -------
    True if MnemonicSample flags value has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getEUValue "

    getEUValue(self) -> Field

    Accessor for the EU Value

    Returns
    -------
    A reference to the MnemonicSample EU value field

    Exceptions
    ----------
    An exception is thrown if the EU value field has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasTextValue "

    hasTextValue(self) -> bool

    Returns the availability of a MnemonicSample text value

    Returns
    -------
    True if MnemonicSample text value has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasStalenessStatus "

    hasStalenessStatus(self) -> bool

    Returns the availability of a MnemonicSample staleness status

    Returns
    -------
    True if MnemonicSample staleness has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasLimit "

    hasLimit(self) -> bool

    Returns the availability of a MnemonicSample limit

    Returns
    -------
    True if MnemonicSample limit has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getTimestamp "

    getTimestamp(self) -> char const *

    Retrieves the timestamp of the MnemonicSample

    Returns
    -------
    The timestamp of the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getFlags "

    getFlags(self) -> GMSEC_I32

    Accessor for the flags

    Returns
    -------
    A reference to the MnemonicSample flags field

    Exceptions
    ----------
    An exception is thrown if the flags field has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getStalenessStatus "

    getStalenessStatus(self) -> bool

    Accessor for the staleness

    Returns
    -------
    The MnemonicSample staleness

    Exceptions
    ----------
    An exception is thrown if the staleness flag has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setStalenessStatus "

    setStalenessStatus(self, staleness)

    Supplies a staleness flag for the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getLimitEnableDisable "

    getLimitEnableDisable(self) -> bool

    Accessor for the limit enable/disable flag

    Returns
    -------
    The MnemonicSample limit enable/disable flag

    Exceptions
    ----------
    An exception is thrown if the limit enable/disable flag has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasLimitEnableDisable "

    hasLimitEnableDisable(self) -> bool

    Returns the availability of a MnemonicSample limit enable/disable flag

    Returns
    -------
    True if MnemonicSample limit enable/disable flag has been set,
    false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setEUValue "

    setEUValue(self, value)

    Supplies a field for the MnemonicSample EU value and makes a copy of the field
";

%feature("docstring") gmsec::api::mist::MnemonicSample::__eq__ "

    __eq__(self, mnemonicSample) -> bool

    Parameters
    ----------
    mnemonicSample: gmsec::api::mist::MnemonicSample const &
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setFlags "

    setFlags(self, flags)

    Supplies a GMSEC_I32 for the MnemonicSample flags value
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getRawValue "

    getRawValue(self) -> Field

    Retrieves the value of the MnemonicSample

    Returns
    -------
    The value of the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setLimit "

    setLimit(self, arg2)

    Supplies a limit flag for the MnemonicSample limit
";
