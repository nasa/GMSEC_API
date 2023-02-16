/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") SubscriptionEntry "

    This class provides access to the attributes associated with a
    ConfigFile's Subscription entry.
";

%feature("docstring") SubscriptionEntry::getName "

    get_name(self) -> str

    Returns the name associated with the Subscription entry

";

%feature("docstring") SubscriptionEntry::getPattern "

    get_pattern(self) -> str

    Returns the subscription subject pattern
";

%feature("docstring") SubscriptionEntry::hasNextExcludedPattern "

    has_next_excluded_pattern(self) -> bool

    Returns whether there is a next excluded pattern associated with
    the Subscription entry
";

%feature("docstring") SubscriptionEntry::nextExcludedPattern "

    next_excluded_pattern(self) -> str

    Returns the next excluded pattern associated with the Subscription
    entry
";
