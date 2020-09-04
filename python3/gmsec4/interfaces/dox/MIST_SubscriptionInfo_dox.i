
%feature("docstring") gmsec::api::mist::SubscriptionInfo "

    Structure that contains the information the user has supplied when
    setting up a subscription.

    See Also
    --------
    ConnectionManager::subscribe()
    ConnectionManager::unsubscribe()

";

%feature("docstring") gmsec::api::mist::SubscriptionInfo::getCallback "

    get_callback(self) -> ConnectionManagerCallback

    Returns the ConnectionManagerCallback object, if any, used when setting up the
    subscription.

    Returns
    -------
    Pointer to ConnectionManagerCallback object.
";

%feature("docstring") gmsec::api::mist::SubscriptionInfo::getSubject "

    get_subject(self) -> str

    Returns the subject string used when setting up the subscription.

    Returns
    -------
    Subject string.
";

