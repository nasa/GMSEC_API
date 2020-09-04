
%feature("docstring") list<gmsec::api::mist::FieldSpecification*> "

    A container that models the C++ STL list. This container holds
    FieldSpecification objects.

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
