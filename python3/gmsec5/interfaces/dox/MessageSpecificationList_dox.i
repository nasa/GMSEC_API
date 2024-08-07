/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%feature("docstring") list<gmsec::api5::MessageSpecification*> "

    A container that models the C++ STL list. This container holds
    MessageSpecification objects.

    Example usage:
    config = libgmsec_python3.Config()

    config.add_value(\"gmsec-specification-version\", libgmsec_python3.GMSEC_ISD_CURRENT)

    spec = libgmsec_python3.Specification(config)

    msgSpecs = spec.get_message_specifications()

    for msgSpec in msgSpecs:
        fieldSpecs = msgSpec.get_field_specifications()

        for fieldSpec in fieldSpecs:
            print(fieldSpec.get_name())
            print(fieldSpec.get_type())
            print(fieldSpec.get_mode())
            print(fieldSpec.get_classification())
            print(fieldSpec.get_value())
            print(fieldSpec.get_description())

";
