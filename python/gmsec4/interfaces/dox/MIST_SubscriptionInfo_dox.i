
// File: classlibgmsec__python_1_1SubscriptionInfo.xml


%feature("docstring") gmsec::api::mist::SubscriptionInfo "

    Structure that contains the information the user has supplied when setting up a subscription.

    See Also
    --------
    Connection::subscribe
    Connection::unsubscribe

";

%feature("docstring") gmsec::api::mist::SubscriptionInfo::getCallback "

    getCallback(self) -> Callback

    Returns the Callback object, if any, used when setting up the subscription.

    Returns
    -------
    Pointer to Callback object.
";

%feature("docstring") gmsec::api::mist::SubscriptionInfo::getSubject "

    getSubject(self) -> char const *

    Returns the subject string used when setting up the subscription.

    Returns
    -------
    Subject string.
";

%feature("docstring") gmsec::api::mist::SubscriptionInfo::getCallbackFunction "

    getCallbackFunction(self) -> void *

    Returns
    -------
    Returns pointer to callback function that is used by C subscribers.
";

