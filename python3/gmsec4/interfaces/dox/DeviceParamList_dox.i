
%feature("docstring") list<gmsec::api::mist::DeviceParam*> "

    A container that models the C++ STL list. This container holds
    DeviceParam objects.

    Example usage:

    paramval1 = libgmsec_python3.I32Field(\"Param1\", 1)
    paramval2 = libgmsec_python3.I32Field(\"Param1\", 2)
    paramval3 = libgmsec_python3.I32Field(\"Param1\", 3)

    param1 = libgmsec_python3.DeviceParam(\"param1\", \"timestamp\", paramval1)
    param2 = libgmsec_python3.DeviceParam(\"param2\", \"timestamp\", paramval2)
    param3 = libgmsec_python3.DeviceParam(\"param3\", \"timestamp\", paramval3)

    myDeviceParams = libgmsec_python3.DeviceParamList()

    myDeviceParams.push_back(param1)
    myDeviceParams.push_back(param2)
    myDeviceParams.push_back(param3)

    for param in myDeviceParams:
        print(param.get_value().to_XML())
";
