/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Config

// This tells SWIG to treat char ** as a special case
%typemap(in) char ** {
  AV *tempav;
  I32 len;
  int i;
  SV  **tv;
  if (!SvROK($input))
    croak("Argument $argnum is not a reference.");
  if (SvTYPE(SvRV($input)) != SVt_PVAV)
    croak("Argument $argnum is not an array.");
  tempav = (AV*)SvRV($input);
  len = av_len(tempav);
  $1 = (char **) malloc((len+2)*sizeof(char *));
  for (i = 0; i <= len; i++) {
    tv = av_fetch(tempav, i, 0);
    $1[i] = (char *) SvPV(*tv, PL_na);
  }
  $1[i] = NULL;
};

// This cleans up the char ** array after the function call
%typemap(freearg) char ** {
  free($1);
}

%{
#include <gmsec5/Config.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::Config::Config(int, char**);
%ignore gmsec::api5::Config::getFirst(const char*&, const char*&) const;
%ignore gmsec::api5::Config::getNext(const char*&, const char*&) const;

%ignore gmsec::api5::Config::operator=(const Config&);


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


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Config.h>


%extend gmsec::api5::Config {

    bool CALL_TYPE getFirst(ConfigPair* cp) const
    {
        return self->getFirst(cp->name, cp->value);
    }

    bool CALL_TYPE getNext(ConfigPair* cp) const
    {
        return self->getNext(cp->name, cp->value);
    }

    static Config* create(char** argv)
    {
        std::string pargs;
        int i = 0;
        while (argv[i]) {
            pargs += argv[i];
            pargs += " ";
            ++i;
        }
        return (pargs.empty() ? new Config() : new Config(pargs.c_str(), DataType::KEY_VALUE_DATA));
    }
}


%perlcode%{
=pod

=head1 libgmsec_perl::Config

This class is a collection for managing connection configuration items.

Encapsulates the parameters necessary for initialization Connections and setting middleware dependent values in Messages. Used by the Connection and Message, this class provides a generic means of supplying initialization data without being Connection/Message specific.

=head2 Public Static Subroutines

=head3 create()

C<libgmsec_perl::Config::create(@argv)>

        This static method will create a config and parse values from the given array of configuration values. The array is typically the ARGV array derived from the command line arguments which consist of key=value strings.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        @argv - array of key=value strings

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Config object populated with values loaded from the given array


=head2 Public Member Subroutines

=head3 new()

C<libgmsec_perl::Config-E<gt>new()>

        Default Constructor

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An empty Config object


C<libgmsec_perl::Config-E<gt>new($data, $type)>

        This constructor will create a config and parse values from the passed in given string. The xml format is the same as toXML() functions.
        This constructor will create a Config object by parsing name/value pairs from the given data string. The data string can contain either XML or JSON formatted configuration data, or contain raw data consisting of white-space separated key=value pairs.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $data - data string containing configuration information
        $type - the type of data being provided (DataType_XML_DATA, DataType_JSON_DATA, DataType_KEY_VALUE_DATA)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Config object populated with values loaded from the data string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if the data cannot be parsed

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Config.html#toxml">toXML()</a>


C<libgmsec_perl::Config-E<gt>new($config)>

        Basic (and safe) deep copy constructor.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $config - config object to copy

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An copy of the Config object provided


=head3 addValue()

C<libgmsec_perl::Config-E<gt>addValue($name, $value)>

        This function adds a value to the config object by name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to add
        $value - value to be added

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if either name or value are NULL, or are empty strings


=head3 clearValue()

C<libgmsec_perl::Config-E<gt>clearValue($name)>

        This function clears a value by name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name of value to clear

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        boolean value to indicate whether operation was successful.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if name is NULL

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

        $libgmsec_perl::GmsecException if name is NULL or contains empty string

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

        $libgmsec_perl::GmsecException if name is NULL or contains empty string

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

        $libgmsec_perl::GmsecException if name is NULL or contains empty string

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


=head3 getFromFile()

C<libgmsec_perl::Config::getFromFile()>

        Attempts to open the given file and use the contents to create a Config object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $filename   - The configuration file to open.
        $configName - The name of the configuration to load/process.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A Config object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        $libgmsec_perl::GmsecException if the file name is undefined, or consists of an empty-string.
        $libgmsec_perl::GmsecException if the file cannot be opened.
        $libgmsec_perl::GmsecException if the file does not contain a valid configuration construct.
        $libgmsec_perl::GmsecException if the file does not contain the named configuration construct.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>



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
