
// File: classlibgmsec__python_1_1ServiceParamList.xml


%feature("docstring") list<gmsec::api::mist::ServiceParam*> "

    A container that models the STL list. This container holds MnemonicSample objects.

    Example usage:
    myparams= libgmsec_python.ServiceParamList()

    param1 = libgmsec_python.ServiceParam(\"param1\", libgmsec_python.I32Field(\"param1\", 1))
    param2 = libgmsec_python.ServiceParam(\"param2\", libgmsec_python.I32Field(\"param2\", 2))
    param3 = libgmsec_python.ServiceParam(\"param3\", libgmsec_python.I32Field(\"param3\", 3))

    myparams.push_back(param1)
    myparams.push_back(param2)
    myparams.push_back(param3)

    for sample in myparams:
        print sample.getValue().toXML()

    For documentation see - http://en.cppreference.com/w/cpp/container/list
";
