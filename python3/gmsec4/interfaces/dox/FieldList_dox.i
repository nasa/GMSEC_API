
%feature("docstring") list<gmsec::api::Field*> "

    A container that models the C++ STL list. This container holds Field objects.

    Example usage:
    field1 = libgmsec_python3.StringField(\"FIELD-1\", \"one\")
    field2 = libgmsec_python3.StringField(\"FIELD-2\", \"two\")
    field3 = libgmsec_python3.StringField(\"FIELD-3\", \"three\")

    myFields = libgmsec_python3.FieldList()

    myFields.push_back(field1)
    myFields.push_back(field2)
    myFields.push_back(field3)

    for field in myFields:
        print(field.to_XML())
";
