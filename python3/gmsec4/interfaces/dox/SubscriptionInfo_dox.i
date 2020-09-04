
%feature("docstring") gmsec::api::SubscriptionInfo "

    Structure that contains the information the user has supplied when
    setting up a subscription.

    See Also
    --------
    Connection::subscribe
    Connection::unsubscribe
";

%feature("docstring") gmsec::api::SubscriptionInfo::getCallback "

    getCallback(self) -> Callback

    Returns the Callback object, if any, used when setting up the
    subscription.

    Returns
    -------
    Pointer to Callback object.
";

%feature("docstring") gmsec::api::SubscriptionInfo::getSubject "

    getSubject(self) -> char const *

    Returns the subject string used when setting up the subscription.

    Returns
    -------
    Subject string.
";
