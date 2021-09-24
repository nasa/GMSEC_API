/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module ConfigFile

// SWIG does not handle nested classes well for certain output languages
// (Perl included)
// Redefine nested classes in global scope in order for SWIG to generate
// a proxy class. Only SWIG parses this definition.
class GMSEC_API ConfigEntry
{
public:
	const char* CALL_TYPE getName() const;
	const gmsec::api::Config& CALL_TYPE getConfig() const;
private:
	ConfigEntry();
	~ConfigEntry();
};

class GMSEC_API MessageEntry
{
public:
	const char* CALL_TYPE getName() const;
	const gmsec::api::Message& CALL_TYPE getMessage() const;
private:
	MessageEntry();
	~MessageEntry();
};

class GMSEC_API SubscriptionEntry
{
public:
	const char* CALL_TYPE getName() const;
	const char* CALL_TYPE getSubject() const;
        const char* CALL_TYPE getPattern() const;
        const char* CALL_TYPE nextExcludedPattern() const;
        bool CALL_TYPE hasNextExcludedPattern();
private:
	SubscriptionEntry();
	~SubscriptionEntry();
};

%ignore gmsec::api::ConfigFile::ConfigEntry;
%ignore gmsec::api::ConfigFile::MessageEntry;
%ignore gmsec::api::ConfigFile::SubscriptionEntry;

%ignore gmsec::api::ConfigFile::ConfigEntry::operator=(const ConfigEntry&);
%ignore gmsec::api::ConfigFile::MessageEntry::operator=(const MessageEntry&);
%ignore gmsec::api::ConfigFile::SubscriptionEntry::operator=(const SubscriptionEntry&);

%{
#include <gmsec4/ConfigFile.h>
using namespace gmsec::api;
%}

%include <gmsec4/util/Deprecated.h>
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/ConfigFile.h>

%{
// SWIG thinks that Inner is a global class, so we need to trick the C++
// compiler into understanding this so called global type.

typedef gmsec::api::ConfigFile::ConfigEntry	   ConfigEntry;
typedef gmsec::api::ConfigFile::MessageEntry	  MessageEntry;
typedef gmsec::api::ConfigFile::SubscriptionEntry SubscriptionEntry;
%}

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::ConfigFile

=head1 DESCRIPTION

This class is for the managment of standard configuration files.

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Config-E<gt>new()>

        Basic constructor that creates an empty configuration file object.

        Once the ConfigFile object has been created, use Load(const char* filepath) to load a configuration file, or if the plan is to add elements to the empty ConfigFile object, then use Save(const char* filepath, bool compact) to save any changes.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An empty ConfigFile object

=head3 DESTROY

C<libgmsec_perl::Config-E<gt>DESTROY()>

        destructor

=head3 load

C<libgmsec_perl::ConfigFile-E<gt>load($filepath)>

        This function opens the configuration file and parses the XML It will return errors explaining what, if anything, went wrong.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $filepath - The location of the file to load

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if file parsing error occurs

=head3 save

C<libgmsec_perl::ConfigFile-E<gt>save($filepath = NULL, compact = 0)>

        This function saves the currently open configuration file to the location specified. It will return errors explaining what, if anything, went wrong.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $filepath - The location to which the file will be saved; if no path is specified or is specified as NULL, then the path, if any, that was used with load($filepath) will be used.
        $compact - determines whether the XML output will be compacted or not

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if error occurs writing to the file

=head3 fromXML

C<libgmsec_perl::ConfigFile-E<gt>fromXML($xml)>

        This function parses the XML in the passed in string. It will return errors explaining what, if anything, went wrong.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $xml - the XML string to be loaded

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if error occurs while parsing XML string

=head3 toXML

C<libgmsec_perl::ConfigFile-E<gt>toXML()>

        This function produces an XML string representing a config file containing all known config file tags.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        The XML string representation of the ConfigFile object.

=head3 isLoaded

C<libgmsec_perl::ConfigFile-E<gt>isLoaded()>

        This function will return true if the ConfigFile was successfully loaded.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        true if file loaded

=head3 lookupConfig

C<libgmsec_perl::ConfigFile-E<gt>lookupConfig($name)>

        This function will attempt to find and return a named Config object in the config file.

        Example config file load and get config:

        eval
        {
            my $configFile = libgmsec_perl::ConfigFile->new();

            # Load and parse configuration file
            $configFile->load("/some/path/to/config.xml");

            # Attempt to get Config from ConfigFile
            eval
            {
                my $config = $configFile->lookupConfig("MyConfig");

                # Do something with $config
            };
            if ($@)
            {
                # Handle Config lookup error
            }
        };
        if($@)
        {
                # Handle configuration file parsing error
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - value in the name="" attribute of the <CONFIG> block to seed the Config object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Config object that correlates with the given name

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        Exception for cases where configuration for the given name does not exist

=head3 lookupMessage

C<libgmsec_perl::ConfigFile-E<gt>lookupMessage($name)>

        This function will attempt to find and return a named Message object in the config file.

        Example config file load and get message:

        eval
        {
            my $configFile = libgmsec_perl::ConfigFile->new();

            # Load and parse configuration file
            $configFile->load("/some/path/to/config.xml");

            # Attempt to get Message from ConfigFile
            eval
            {
                my $message = $configFile->lookupMessage("MyMessage");

                # Do something with $message
            };
            if ($@)
            {
                # Handle Message lookup error
            }
        };
        if($@)
        {
                # Handle configuration file parsing error
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - value in the name="" attribute of the <MESSAGE> block to seed the Message object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Message object that correlates with the given name

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        Exception for cases where message configuration for the given name does not exist

=head3 lookupSubscription

C<libgmsec_perl::ConfigFile-E<gt>lookupSubscription($name)>

        This function will attempt to find and return a named subscription pattern in the config file.  This is useful to allow easy modification of subject names without code changes.

        Note: This function has been deprecated; use lookupSubscriptionEntry instead

        Example config file load and get message:

        eval
        {
            my $configFile = libgmsec_perl::ConfigFile->new();

            # Load and parse configuration file
            $configFile->load("/some/path/to/config.xml");

            # Attempt to get subscription pattern from ConfigFile
            my $subjectPattern = $configFile->lookupSubscription("MySubscription");

            if ($subjectPattern)
            {
                # Success
            }
            else
            {
                # Handle subscription pattern lookup error
            }
        };
        if($@)
        {
                # Handle configuration file parsing error
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - value in the name="" attribute of the <SUBSCRIPTION> block containing the subscription pattern

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        subject pattern topic associated with SUBSCRIPTION block, or NULL if not not found

=head3 lookupSubscriptionEntry

C<libgmsec_perl::ConfigFile-E<gt>lookupSubscriptionEntry($name)>

        This function will attempt to find and return a named subscription pattern in the config file.  This is useful to allow easy modification of subject names without code changes.

        Example config file load and get message:

        eval
        {
            my $configFile = libgmsec_perl::ConfigFile->new();

            # Load and parse configuration file
            $configFile->load("/some/path/to/config.xml");

            # Attempt to get subscription pattern from ConfigFile
            my $subjectPattern = $configFile->lookupSubscriptionEntry("MySubscription");

            if ($subjectPattern)
            {
                # Success
            }
            else
            {
                # Handle subscription pattern lookup error
            }
        };
        if($@)
        {
                # Handle configuration file parsing error
        }

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - value in the name="" attribute of the <SUBSCRIPTION> block containing the subscription pattern

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        subject pattern topic associated with SUBSCRIPTION block, or NULL if not not found

=head3 addSubscription

C<libgmsec_perl::ConfigFile-E<gt>addSubscription($name, $pattern)>

        Adds a Subscription definition to the list of subscriptions.

        Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within the document, if saved. When adding a subscription element, it will initially be inserted after the last <SUBSCRIPTION> element in the document, or as the very first child of the root element if there are no other subscription definitions.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Adding a subscription with a name that currently exists will overwrite the current subscription.
        Adding a subscription definition to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name to be associated with the subscription
        $subscription - XML string representation of a gmsec subscription

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if either name or subscription parameters are NULL or if either contains an empty string

=head3 addConfig

C<libgmsec_perl::ConfigFile-E<gt>addConfig($name, $config)>

        Adds a Config definition to the list of configurations.  The Config object, though passed by reference, will not be modified.

        Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within the document, if saved. When adding a config element, it will initially be inserted after the last <CONFIG> element in the document, if no other config definitions are present in the document, the new config definition will be added after the last <SUBSCRIPTION> element in the document, if there are no subscription definitions then it will be added as the very first child of the root element.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Adding a config with a name that currently exists will overwrite the current configuration definition.
        Adding a config to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name to be associated with the subscription
        $config - GMSEC Config object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if name is NULL or contains an empty string

=head3 addMessage

C<libgmsec_perl::ConfigFile-E<gt>addMessage($name, $message)>

        Adds a message definition to the list of messages. The Message object, though passed by reference, will not be modified.

        Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within the document, if saved. When adding a message element, it will initially be inserted after the last <MESSAGE> element in the document, if no other message definitions are present in the document, it will be added after the last <CONFIG> element in the document, if no other definitions are present in the document, the new config definition will be added after the last <SUBSCRIPTION> element in the document, if there are no subscription definitions then it will be added as the very first child of the root element.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Adding a message with a name that currently exists will overwrite the current message definition.
        Adding a message definition to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name to be associated with the subscription
        $message - GMSEC Message object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if name is NULL or contains an empty string

=head3 addCustomXML

C<libgmsec_perl::ConfigFile-E<gt>addCustomXML($xml)>

        Adds a custom XML string to the ConfigFile.

        Programmatic XML element addition will use pre-defined hierarchical rules for determining the placement of elements within the document, if saved. When adding a custom element, it will initially be inserted after the last <MESSAGE> element in the document, if no message definitions are present in the document, it will be added after the last <CONFIG> element in the document, if no other definitions are present in the document, the new config definition will be added after the last <SUBSCRIPTION> element in the document, if there are no subscription definitions then it will be added as the very first child of the root element.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Adding a custom XML to a newly instantiated ConfigFile object that has not loaded a document will create a new XML document.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $xml - a custom, user-created XML string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        Exception if XML string is NULL, contains an empty string, or if an XML parsing error occurs

=head3 removeSubscription

C<libgmsec_perl::ConfigFile-E<gt>removeSubscription($name)>

        Removes the Subscription definition which is mapped to the provided name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name associated with the subscription definition

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if successful, otherwise 0 (false)

=head3 removeConfig

C<libgmsec_perl::ConfigFile-E<gt>removeConfig($name)>

        Removes the Config definition which is mapped to the provided name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name associated with the configuration definition

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if successful, otherwise 0 (false)

=head3 removeMessage

C<libgmsec_perl::ConfigFile-E<gt>removeMessage($name)>

        Removes the Message definition which is mapped to the provided name.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - name associated with the message definition

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if successful, otherwise 0 (false)

=head3 removeCustomXML

C<libgmsec_perl::ConfigFile-E<gt>removeCustomXML($xml)>

        Removes the provided xml element.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $xml - XML element string matching the custom XML definition

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        1 (true) if successful, otherwise 0 (false)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exception:</b><br>

        Exception if XML string is NULL, contains an empty string, or if an XML parsing error occurs

=head3 getIterator

C<libgmsec_perl::ConfigFile-E<gt>getIterator()>

        Method that allows the callee to get the ConfigFileIterator associated with the ConfigFile. This iterator will allow for applications to iterate over the entries stored within the ConfigFile. The iterator is reset each time getIterator() is called. The iterator itself is destroyed when the ConfigFile object is destroyed.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Note:</b><br>

        Only one ConfigFileIterator object is associated with a ConfigFile object; multiple calls to getIterator() will return a reference to the same ConfigFileIterator object. Each call will reset the iterator.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A reference to a ConfigFileIterator object.

=head1 NAME

libgmsec_perl::SubscriptionEntry

=head1 DESCRIPTION

<Description of the class>

=head2 Public Member Subroutines

=head3 getName

C<libgmsec_perl::SubscriptionEntry-E<gt>getName()>

        This function returns the name associated with a subscription entry 

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
        
        A name associated with a subscription entry

=head3 getPattern

C<libgmsec_perl::SubscriptionEntry-E<gt>getPattern()>

        This function returns the pattern associated with a subscription entry 

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A pattern associated with a subscription entry

=head3 hasNextExcludedPattern

C<libgmsec_perl::SubscriptionEntry-E<gt>hasNextExcludedPattern()>

        This functions is used to determine if there is a next excluded pattern in the subscription entry

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns true if there is a next excluded pattern or false if there are no more excluded patterns

=head3 nextExcludedPattern

C<libgmsec_perl::SubscriptionEntry-E<gt>nextExcludedPattern()>

        This function returns the next excluded pattern in a subscription entry

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>
       
        Returns the next excluded pattern in a subscription entry

=cut
%}
