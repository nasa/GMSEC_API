/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") gmsec::api5::SubscriptionInfo "

    Structure that contains the information the user has supplied when
    setting up a subscription.

    See Also
    --------
    Connection::subscribe
    Connection::unsubscribe
";

%feature("docstring") gmsec::api5::SubscriptionInfo::getSubjectPattern "

    get_subject_pattern(self) -> str

    Returns the subject pattern string used when setting up the subscription.

    Returns
    -------
    Subject string.
";
