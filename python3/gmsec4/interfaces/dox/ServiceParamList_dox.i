
%feature("docstring") list<gmsec::api::mist::ServiceParam*> "

    A container that models the STL list. This container holds
    MnemonicSample objects.

    Example usage:
    param1 = ServiceParam(\"param1\", I32Field(\"param1\", 1))
    param2 = ServiceParam(\"param2\", I32Field(\"param2\", 2))
    param3 = ServiceParam(\"param3\", I32Field(\"param3\", 3))

    myparams= ServiceParamList()

    myparams.push_back(param1)
    myparams.push_back(param2)
    myparams.push_back(param3)

    for sample in myparams:
        print(sample.get_value().to_XML())
";
