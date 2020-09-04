
// File: classlibgmsec__python_1_1MnemonicSampleList.xml


%feature("docstring") list<gmsec::api::mist::MnemonicSample*> "

    A container that models the STL list. This container holds MnemonicSample objects.

    Example usage:
    mysamples = libgmsec_python.MnemonicSampleList()

    msample1 = libgmsec_python.MnemonicSample(\"MS1\", libgmsec_python.I32Field(\"MS1\", 1))
    msample2 = libgmsec_python.MnemonicSample(\"MS2\", libgmsec_python.I32Field(\"MS2\", 2))
    msample3 = libgmsec_python.MnemonicSample(\"MS3\", libgmsec_python.I32Field(\"MS3\", 3))

    mysamples.push_back(msample1)
    mysamples.push_back(msample2)
    mysamples.push_back(msample3)

    for sample in mysamples:
        print sample.getRawValue().toXML()

    For documentation see - http://en.cppreference.com/w/cpp/container/list
";
