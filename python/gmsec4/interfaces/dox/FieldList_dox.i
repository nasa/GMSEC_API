
// File: classlibgmsec__python_1_1FieldList.xml


%feature("docstring") list<gmsec::api::Field*> "

    A container that models the STL list. This container holds Field objects.

    Example usage:
    myFields = libgmsec_python.FieldList()

    field1 = libgmsec_python.StringField(\"FIELD-1\", \"one\")
    field2 = libgmsec_python.StringField(\"FIELD-2\", \"two\")
    field3 = libgmsec_python.StringField(\"FIELD-3\", \"three\")

    myFields.push_back(field1)
    myFields.push_back(field2)
    myFields.push_back(field3)

    for field in myFields:
        print field.toXML()

    For documentation see - http://en.cppreference.com/w/cpp/container/list
";
