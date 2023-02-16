#!/usr/bin/env perl

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#
# @file message_field_iterator.pl
# 
# This file contains a simple example demonstrating the use of the
# MessageFieldIterator class to iterate over the Fields of a Message.
#
# This example program is intended to be run prior to executing any example
# program which publishes a GMSEC message (e.g. The publish example).
#


use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


# Define custom callback
#
{
    package FieldIterationCallback;
    use base 'libgmsec_perl::Callback';

    sub new
    {
        my $class = shift;
        my $self = $class->SUPER::new();
        bless $self, $class;
        return $self;
    }

    sub onMessage
    {
        my ($self, $connection, $msg) = @_;

        libgmsec_perl::logInfo("Received a message with subject: " . $msg->getSubject());

        libgmsec_perl::logInfo("Field Name (Field Type): Field Value");

        my $iter = $msg->getFieldIterator();

        while ($iter->hasNext())
        {
            my $field = $iter->next();

            # Extract the Field Name, Type, and Value
            my $name  = $field->getName();
            my $type  = typeToString($field->getType());
            my $value = $field->getStringValue();

            # Field objects can also be converted to specific Field types prior
            # to retrieval of the value contained in the Field. This is useful
            # for ensuring that data types do not lose any level of precision.
            # 
            # See get_field_value() for an example of how a Field can be converted to a
            # specialized Field object.
            #my $value = getFieldValue($field);

            libgmsec_perl::logInfo("$name ($type): $value");
        }
    }

    sub typeToString
    {
        my ($fieldType) = @_;

        if    ($fieldType == $libgmsec_perl::Field::Type_BINARY)    { return "BIN"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_BOOL)      { return "BOOL"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_CHAR)      { return "CHAR"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_F32)       { return "F32"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_F64)       { return "F64"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_I16)       { return "I16"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_I32)       { return "I32"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_I64)       { return "I64"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_I8)        { return "I8"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_STRING)    { return "STRING"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_U16)       { return "U16"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_U32)       { return "U32"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_U64)       { return "U64"; }
        elsif ($fieldType == $libgmsec_perl::Field::Type_U8)        { return "U8"; }
        else                                                             { return "UNKNOWN"; }
    }

    # This method is provided as an example of how to retrieve type-specific
    # values from individual Field objects.
    #
    sub getFieldValue
    {
        my ($field) = @_;

        if    ($field->getType() == $libgmsec_perl::Field::Type_BINARY)   { return libgmsec_perl::to_BinaryField($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_BOOL)     { return libgmsec_perl::to_BooleanField($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_CHAR)     { return libgmsec_perl::to_CharField($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_F32)      { return libgmsec_perl::to_F32Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_F64)      { return libgmsec_perl::to_F64Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_I16)      { return libgmsec_perl::to_I16Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_I32)      { return libgmsec_perl::to_I32Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_I64)      { return libgmsec_perl::to_I64Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_I8)       { return libgmsec_perl::to_I8Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_STRING)   { return libgmsec_perl::to_StringField($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_U16)      { return libgmsec_perl::to_U16Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_U32)      { return libgmsec_perl::to_U32Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_U64)      { return libgmsec_perl::to_U64Field($field)->getValue(); }
        elsif ($field->getType() == $libgmsec_perl::Field::Type_U8)       { return libgmsec_perl::to_U8Field($field)->getValue() ;}
        else                                                                   { return "UNKNOWN"; }
    }
}


sub main
{
    # Load the command-line input into a GMSEC Config object
    my $config = undef;
    if ($#ARGV ge 0)
    {
        $config = libgmsec_perl::Config::create(\@ARGV);
    }
    else
    {
        $config = libgmsec_perl::Config->new();
    }

    # Initialize log level for output
    initializeLogLevel($config);

    # Deduce subscription topic for the example program
    my $subscriptionTopic = "";
    my $level = $config->getIntegerValue("gmsec-schema-level", $libgmsec_perl::Specification::SchemaLevel_LEVEL_0);

    if ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_0)
    {
        $subscriptionTopic = "C2MS.>";
    }
    elsif ($level == $libgmsec_perl::Specification::SchemaLevel_LEVEL_1)
    {
        $subscriptionTopic = "C2MS-PIPE.>";
    }
    else
    {
        $subscriptionTopic = "GMSEC.>";
    }

    eval
    {
        # Create connection instance.
        my $conn = libgmsec_perl::Connection::create( $config );

        # Establish connection to the GMSEC Bus.
        $conn->connect();

        # Output information regarding the API version and connection
        libgmsec_perl::logInfo(libgmsec_perl::Connection::getAPIVersion());
        libgmsec_perl::logInfo("Middleware version = " . $conn->getLibraryVersion());

        # Subscribe to receive messages using a callback
        my $cb = FieldIterationCallback->new();
        $conn->subscribe($subscriptionTopic, $cb);
        libgmsec_perl::logInfo("Subscribed to topic: $subscriptionTopic");

        # Attempt to receive a message
        my $rcvd = $conn->receive($libgmsec_perl::GMSEC_WAIT_FOREVER);

        libgmsec_perl::logInfo("Dispatching message");
        $conn->dispatch($rcvd);
        $rcvd->acknowledge();
        libgmsec_perl::Message::destroy($rcvd);

        # Clean up
        $conn->disconnect();

        # Destroy the connection instance
        libgmsec_perl::Connection::destroy($conn);
    };
    if (isa($@, 'libgmsec_perl::GmsecException'))
    {
        # If here, the most likely case is the middleware could not be
        # deduced, or a failure occurred when attempting to connect to
        # the GMSEC Bus.
        my $error = $@;
        libgmsec_perl::logError("Exception: " . $error->what());
        return -1;
    }

    return 0;
}


sub initializeLogLevel
{
    my ($config) = @_;
    my $level = libgmsec_perl::Log::fromString( $config->getValue("loglevel", "info") );
    libgmsec_perl::Log::setReportingLevel($level);
}


# Main entry point
#
exit( main() );
