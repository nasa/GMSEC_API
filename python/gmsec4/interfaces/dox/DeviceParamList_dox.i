
// File: classlibgmsec__python_1_1DeviceParamList.xml


%feature("docstring") list<gmsec::api::mist::DeviceParam*> "

    A container that models the STL list. This container holds DeviceParam objects.

    Example usage:
    myDeviceParams = libgmsec_python.DeviceParamList()

    paramval1 = libgmsec_python.I32Field(\"Param1\", 1)
    paramval2 = libgmsec_python.I32Field(\"Param1\", 2)
    paramval3 = libgmsec_python.I32Field(\"Param1\", 3)

    param1 = libgmsec_python.DeviceParam(\"param1\", \"timestamp\", paramval1)
    param2 = libgmsec_python.DeviceParam(\"param2\", \"timestamp\", paramval2)
    param3 = libgmsec_python.DeviceParam(\"param3\", \"timestamp\", paramval3)

    myDeviceParams.push_back(param1)
    myDeviceParams.push_back(param2)
    myDeviceParams.push_back(param3)

    for param in myDeviceParams:
        print param.getValue().toXML()

    For documentation see - http://en.cppreference.com/w/cpp/container/list
";
