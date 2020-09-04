
%feature("docstring") gmsec::api::mist::MnemonicSample "

    This class is a lightweight container for holding information
    on a Mnemonic Sample, and used to generate GMSEC Mnemonic messages
    by the ConnectionManager and Device classes

    See Also
    --------
    Mnemonic
    ConnectionManager

    CONSTRUCTORS:

    MnemonicSample(timestamp: str, rawValue: Field)

    Initializes the MnemonicSample with a timestamp and value.

    Parameters
    ----------
    timestamp : The time at which the sampled value was taken.
    rawValue  : The Field containing the raw value of the parameter.



    MnemonicSample(other: MnemonicSample)

    Copy constructor - Initializes the MnemonicSample from another MnemonicSample.

    Parameters
    ----------
    other : The MnemonicSample to copy.
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getQuality "

    get_quality(self) -> bool

    Accessor for the quality

    Returns
    -------
    The MnemonicSample quality

    Exceptions
    ----------
    A GmsecError is thrown if the quality flag has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasQuality "

    has_quality(self) -> bool

    Returns the availability of a MnemonicSample quality flag

    Returns
    -------
    True if MnemonicSample quality has been set, false otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getLimit "

    get_limit(self) -> MnemonicSample::LimitFlag

    Accessor for the limit

    Returns
    -------
    The MnemonicSample limit

    Exceptions
    ----------
    A GmsecError is thrown if the limit has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setQuality "

    set_quality(self, quality: bool)

    Assigns a quality flag for the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setLimitEnableDisable "

    set_limit_enable_disable(self, value: bool)

    Assigns a limit enable/disable flag for the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setTextValue "

    set_text_value(self, value: str)

    Assigns a string for the MnemonicSample text value
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasEUValue "

    has_EU_value(self) -> bool

    Returns the availability of a MnemonicSample value in engineering units

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

    get_text_value(self) -> str

    Accessor for the text value

    Returns
    -------
    The MnemonicSample text value field

    Exceptions
    ----------
    A GmsecError is thrown if the text value field has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasFlags "

    has_flags(self) -> bool

    Returns the availability of a MnemonicSample flags field

    Returns
    -------
    True if MnemonicSample flags value has been set; False otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getEUValue "

    get_EU_value(self) -> Field

    Accessor for the EU Value

    Returns
    -------
    A reference to the MnemonicSample EU value field

    Exceptions
    ----------
    A GmsecError is thrown if the EU value field has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasTextValue "

    has_text_value(self) -> bool

    Returns the availability of a MnemonicSample text value

    Returns
    -------
    True if MnemonicSample text value has been set; False otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasStalenessStatus "

    has_staleness_status(self) -> bool

    Returns the availability of a MnemonicSample staleness status

    Returns
    -------
    True if MnemonicSample staleness has been set; False otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasLimit "

    has_limit(self) -> bool

    Returns the availability of a MnemonicSample limit

    Returns
    -------
    True if MnemonicSample limit has been set; False otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getTimestamp "

    get_timestamp(self) -> str

    Retrieves the timestamp of the MnemonicSample

    Returns
    -------
    The timestamp of the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getFlags "

    get_flags(self) -> int

    Accessor for the flags

    Returns
    -------
    The MnemonicSample flags field

    Exceptions
    ----------
    A GmsecError is thrown if the flags field has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getStalenessStatus "

    get_staleness_status(self) -> bool

    Accessor for the staleness

    Returns
    -------
    The MnemonicSample staleness

    Exceptions
    ----------
    A GmsecError is thrown if the staleness flag has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setStalenessStatus "

    set_staleness_status(self, staleness: bool)

    Assigns a staleness flag for the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getLimitEnableDisable "

    get_limit_enable_disable(self) -> bool

    Accessor for the limit enable/disable flag

    Returns
    -------
    The MnemonicSample limit enable/disable flag

    Exceptions
    ----------
    A GmsecError is thrown if the limit enable/disable flag has not been set
";

%feature("docstring") gmsec::api::mist::MnemonicSample::hasLimitEnableDisable "

    has_limit_enable_disable(self) -> bool

    Returns the availability of a MnemonicSample limit enable/disable flag

    Returns
    -------
    True if MnemonicSample limit enable/disable flag has been set; False otherwise
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setEUValue "

    set_EU_value(self, value: Field)

    Assigns a Field for the MnemonicSample EU value
";

%feature("docstring") gmsec::api::mist::MnemonicSample::__eq__ "

    __eq__(self, mnemonicSample) -> bool

    Parameters
    ----------
    mnemonicSample: gmsec::api::mist::MnemonicSample const &
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setFlags "

    set_flags(self, flags)

    Assigns a GMSEC_I32 for the MnemonicSample flags value
";

%feature("docstring") gmsec::api::mist::MnemonicSample::getRawValue "

    get_raw_value(self) -> Field

    Retrieves the value of the MnemonicSample

    Returns
    -------
    The value of the MnemonicSample
";

%feature("docstring") gmsec::api::mist::MnemonicSample::setLimit "

    set_limit(self, limit: MnemonicSample.LimitFlag)

    Assigns a limit flag for the MnemonicSample limit
";
