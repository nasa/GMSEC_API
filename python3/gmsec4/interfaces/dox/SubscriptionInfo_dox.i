
%feature("docstring") gmsec::api::SubscriptionInfo "

    Structure that contains the information the user has supplied when
    setting up a subscription.

    See Also
    --------
    Connection::subscribe
    Connection::unsubscribe
";

%feature("docstring") gmsec::api::SubscriptionInfo::getCallback "

    get_callback(self) -> Callback

    Returns the Callback, if any, used when setting up the
    subscription.

    Returns
    -------
    Reference to Callback.
";

%feature("docstring") gmsec::api::SubscriptionInfo::getSubject "

    get_subject(self) -> str

    Returns the subject string used when setting up the subscription.

    Returns
    -------
    Subject string.
";
