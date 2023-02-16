/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") list<gmsec::api5::Field*> "

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
        print(field.to_xml())
";
