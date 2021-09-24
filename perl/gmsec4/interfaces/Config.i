/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

/*
struct ConfigPair
{
  char* name;
  char* value;
};
*/

%{
#include <gmsec4/Config.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api::Config::getNext(const char*&, const char*&) const;

%ignore gmsec::api::Config::operator=(const Config&);


%inline %{
class ConfigPair
{
public:
    inline const char* getName() const
    {
        return name;
    }
    inline const char* getValue() const
    {
        return value;
    }

    const char* name;
    const char* value;
};
%}


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Config.h>


%extend gmsec::api::Config {

    bool CALL_TYPE getFirst(ConfigPair* cp) const
    {
        return self->getFirst(cp->name, cp->value);
    }

    bool CALL_TYPE getNext(ConfigPair* cp) const
    {
        return self->getNext(cp->name, cp->value);
    }
}





%perlcode%{
=pod

=head1 libgmsec_perl::Config

This class is a collection for managing connection configuration items.

Encapsulates the parameters necessary for initialization Connections and setting middleware dependent values in Messages. Used by the Connection and Message, this class provides a generic means of supplying initialization data without being Connection/Message specific.

=head2 Public Member Subroutines

=head3 new()

C<libgmsec_perl::Config-E<gt>new()>

        Default Constructor

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An empty Config object

C<libgmsec_perl::Config-E<gt>new($xml)>

        This constructor will create a config and parse values from the passed in xml string. The xml format is the same as toXML() functions.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $xml - string in xml format (See toXML() for an example)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Config object populated with values loaded from the XML string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html#toxml">toXML()</a>

C<libgmsec_perl::Config-E<gt>new($config)>

        Basic (and safe) deep copy constructor.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - config object to copy

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An copy of the Config object provided

=head3 DESTROY()

C<libgmsec_perl::Config-E<gt>DESTROY()>

        destructor

=head3 addValue()

C<libgmsec_perl::Config-E<gt>addValue($name, $value)>

        This function adds a value to the config object by name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to add
        $value - value to be added

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if either name or value are NULL, or are empty strings

=head3 clearValue()

C<libgmsec_perl::Config-E<gt>clearValue($name)>

        This function clears a value by name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to clear

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        boolean value to indicate whether operation was successful.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if name is NULL

=head3 getValue()

C<libgmsec_perl::Config-E<gt>getValue($name)>

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise NULL is returned.

C<libgmsec_perl::Config-E<gt>getValue($name, $defaultValue)>

        This function gets a value by name and if there is no value assigned to will return the defaultValue parameter. defaultValue can be NULL or empty. The scope of the value is the life of the Config object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get
        $defaultValue - default value to be returned if no value exists for name parameter

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise $defaultValue is returned.

=head3 getBooleanValue()

C<libgmsec_perl::Config-E<gt>getBooleanValue($name)>

        This function attempts to get the boolean representation of configuration element. An exception is thrown if the element does not exist or there is no valid boolean interpretion of the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise NULL is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if name is NULL or contains empty string

C<libgmsec_perl::Config-E<gt>getBooleanValue($name, $defaultValue)>

        This function attempts to get the boolean representation of configuration element. If the element does not exist or the element cannot be interpreted as a boolean, the default value is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get
        $defaultValue - default value to be returned if no value exists for name parameter

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise $defaultValue is returned.

=head3 getIntegerValue()

C<libgmsec_perl::Config-E<gt>getIntegerValue($name)>

        This function attempts to get the integer representation of configuration element. An exception is thrown if the element does not exist or there is no valid integer interpretion of the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise NULL is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if name is NULL or contains empty string

C<libgmsec_perl::Config-E<gt>getIntegerValue($name, $defaultValue)>

        This function attempts to get the integer representation of configuration element. If the element does not exist or the element cannot be interpreted as a integer, the default value is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get
        $defaultValue - default value to be returned if no value exists for name parameter

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise $defaultValue is returned.

=head3 getDoubleValue()

C<libgmsec_perl::Config-E<gt>getDoubleValue($name)>

        This function attempts to get the double representation of configuration element. An exception is thrown if the element does not exist or there is no valid double interpretion of the value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise NULL is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if name is NULL or contains empty string

C<libgmsec_perl::Config-E<gt>getDoubleValue($name, $defaultValue)>

        This function attempts to get the double representation of configuration element. If the element does not exist or the element cannot be interpreted as a double, the default value is returned.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to get
        $defaultValue - default value to be returned if no value exists for name parameter

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        If the key entry is found, then the associated value is returned; otherwise $defaultValue is returned.

=head3 clear()

C<libgmsec_perl::Config-E<gt>clear()>

        This function clears all values from the Config object

=head3 getFirst()

C<libgmsec_perl::Config-E<gt>getFirst($configPair)>

        This function gets the first name and value pair for iteration. The scope of the name and value are the life of the Config object, or until the next getFirst() call.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $configPair - structure containing name/value pair

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        boolean status as to whether name/value pair are available

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html#libgmsec_perl::ConfigPair">ConfigPair</a>

=head3 getNext()

C<libgmsec_perl::Config-E<gt>getNext($configPair)>

        This function gets the next name & value pair for iteration. The scope of the name and value are the life of the Config object, or until the next getNext() call.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $configPair - structure containing name/value pair

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        boolean status as to whether name/value pair are available

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html#libgmsec_perl::ConfigPair">ConfigPair</a>

=head3 merge()

C<libgmsec_perl::Config-E<gt>merge($config, $overwriteExisting = true)>

        Merge the given Config object with the current Config object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - the Config object from which to read name/value pairs.
        [optional] $overwriteExisting - indicates whether to overwrite any existing name/value pairs within the working Config object. If flag is omitted, then the default is to overwrite existing name/value pairs.

=head3 toXML()

C<libgmsec_perl::Config-E<gt>toXML()>

        This function will dump the Config into XML format. The format is the same as that which is parsed by the constructor which takes in an XML string.

        Example XML string:

        <PARAMETER NAME="mw-id">bolt</PARAMETER>
            <PARAMETER NAME="server">127.0.0.1</PARAMETER>
            <PARAMETER NAME="tracking">true</PARAMETER>
            <PARAMETER NAME="isthreaded">true</PARAMETER>
        </CONFIG>

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        XML string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html#new">new($xml)</a>



=head1 libgmsec_perl::ConfigPair

This structure is used to reference a single name/value pair that is returned when calling Config-E<gt>getFirst() and Config-E<gt>getNext().

Example usage:

    my $config = libgmsec_perl::Config->new();

    ...

    my $pair = libgmsec_perl::ConfigPair->new();

    my $hasPair = $config>getFirst($pair);

    while ($hasPair)
    {
        print("Name: " . $pair>getName() . ", Value: " . $pair>getValue() . "\n");

        $hasPair = $config>getNext($pair);
    }

=head2 Public Member Subroutines

=head3 new()

C<libgmsec_perl::ConfigPair-E<gt>new()>

        Default Constructor

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An empty ConfigPair object



=head3 getName()

C<libgmsec_perl::ConfigPair-E<gt>getName()>

        Returns the name associated with the object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A string identifying the name of the configuration pair.


=head3 getValue()

C<libgmsec_perl::ConfigPair-E<gt>getValue()>

        Returns the value associated with the object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A string identifying the value of the configuration pair.

=cut
%}
