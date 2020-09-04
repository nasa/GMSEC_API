
%feature("docstring") list<gmsec::api::mist::MnemonicSample*> "

    A container that models the STL list. This container holds
    MnemonicSample objects.

    Example usage:
    msample1 = MnemonicSample(\"MS1\", I32Field(\"MS1\", 1))
    msample2 = MnemonicSample(\"MS2\", I32Field(\"MS2\", 2))
    msample3 = MnemonicSample(\"MS3\", I32Field(\"MS3\", 3))

    mysamples = libgmsec_python3.MnemonicSampleList()

    mysamples.push_back(msample1)
    mysamples.push_back(msample2)
    mysamples.push_back(msample3)

    for sample in mysamples:
        print(sample.get_raw_value().to_XML())
";
