
// File: classlibgmsec__python_1_1SubscriptionEntry.xml


%feature("docstring") SubscriptionEntry "

    This class provides access to the attributes associated with a ConfigFile's Subscription entry.
";

%feature("docstring") SubscriptionEntry::getName "

    getName(self) -> char const *

    Returns the name associated with the Subscription entry

";

%feature("docstring") SubscriptionEntry::getPattern "

    getPattern(self) -> char const *

    Returns the subscription subject pattern
";

%feature("docstring") SubscriptionEntry::hasNextExcludedPattern "

    hasNextExcludedPattern(self) -> bool

    Returns whether there is a next excluded pattern associated with the Subscription entry
";

%feature("docstring") SubscriptionEntry::nextExcludedPattern "

    nextExcludedPattern(self) -> char const *

    Returns the next excluded pattern associated with the Subscription entry
";
