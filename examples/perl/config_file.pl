#!/usr/bin/env perl

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

use strict;
use libgmsec_perl;

*isa = \&UNIVERSAL::isa;


#
# @file config_file.pl
#
# @brief An extended example of reading a ConfigFile XML file and
# retrieving element information.
#
# See config_from_file for a basic example.
#
package config_file;

my $configFileName;
my $configFile;

# 
# Constructor, provide the file name of interest. Open the test file. If an GMSECException results because of this operation
# the method prints out the error message.
# 
# @param configFilename the filename.  All filename checking is done by the API.
#
sub new
{
	my $class = shift;
	$configFileName = shift;
   
	$configFile = new libgmsec_perl::ConfigFile();
		
	# generally all the gmsec-api fail with exceptions
	# many programmers group their gmsec interactions within
	# an exception handler in its entirety.
	# however here, it is broken out in a step by step manner
    # so each XML element and corresponding method are together

	eval {
			$configFile->load($configFileName);
			print "Using config file --> " . $configFileName . "\n";
	};
    if (UNIVERSAL::isa($@, 'libgmsec_perl::GmsecException')) {
        my $error = $@;

        print "ConfigFile open failed [" . $configFileName . "] error -- " . $error->what() . "\n"
    }
		
	my $self = {};
	bless $self, $class;
	return $self;
}


#
# Print the named subscription entry contents or print that it does not exist, or there is an error.
# @param subscriptionName a String
#
sub showSubscription
{
	# the argument
	my $subscriptionName = $_[1];
			
	eval {
		my $sub = $configFile->lookupSubscriptionEntry($subscriptionName);
		print "Subscription <" . $subscriptionName . ">:";
			
		while ($sub->hasNextExcludedPattern()) {
			my $exclude = $sub->nextExcludedPattern();
			print "     -> exclude: " . $exclude;
		}

		print "\n";
	};
	if (UNIVERSAL::isa($@, 'libgmsec_perl::GmsecException')) {
		my $error = $@;
		print "Subscription [" . $subscriptionName . "] error -- " . $error->what() . "\n";
	}
}


#
# Print the named config contents or print that it does not exist, or there is an error.
# @param configName a String
#
sub showConfig()
{
	# the argument
	my $configName = $_[1];
			
	eval {
		my $config = $configFile->lookupConfig($configName);
		print "Config <" . $configName . ">:\n";
			
		# the way to get the guts of the config is using the first/next iterator
		my $configPair = new libgmsec_perl::ConfigPair();
		my $result = $config->getFirst($configPair);

		while ($result) {
			print "  -> key: " . $configPair->getName() . " -> value: " . $configPair->getValue() . "\n";
			$result = $config->getNext($configPair);
		}
		print "\n";

		# but individual values may be found also
		my $mwid = $config->getValue("mw-id");
		print "MWID? --> " . $mwid . "\n";
		if ($mwid eq "") {
			print "  -> find 'mw-id': " . $config->getValue("mw-id") . "\n"
		}
		else {
			print "  -> 'mw-id'is not defined in this config.\n";
		}
			
		# or the entire thing can be printed as XML (or JSON)
		print "\n";
		print $config->toXML();
		print "\n\n";
	};
	if (UNIVERSAL::isa($@, 'libgmsec_perl::GmsecException')) {
		my $error = $@;
		print "Subscription [" . $configName . "] error -- " . $error->what() + "\n";
	}
}


#
# Print the named message contents or print that it does not exist, or there is an error.
# @param configName a String
#
sub showMessage
{
	# the argument
	my $messageName = $_[1];
				
	eval {
		# the Message is a core gmsec type or class
		my $msg = $configFile->lookupMessage($messageName);
			
		print "Message <" . $messageName . ">:\n";
			
		# there are number getters such as this one
		print "  -> find 'STRING-FIELD': " . $msg->getStringField("STRING-FIELD")->getStringValue() . "\n";
		print "\n";
			
		# or the entire thing can be printed as XML (or JSON)
		print $msg->toXML();
		print "\n\n";
	};
	if (UNIVERSAL::isa($@, 'libgmsec_perl::GmsecException')) {
		my $error = $@;
		print "Subscription [" . $messageName . "] error -- " . $error->what() + "\n";
	}
}


sub main
{
	my $num_args = $#ARGV + 1;
	if ($num_args != 1) {
		print "Usage: Provide an example XML file of interest.\n";
		return 1;
	}

	my $demo = config_file->new($_[0]);
        
	$demo->showSubscription("all-messages");
	$demo->showSubscription("custom1");

	$demo->showConfig("config1");
	$demo->showConfig("config2");
		
	$demo->showMessage("msg1");
	$demo->showMessage("msg2");

 	return 0;
}


# Main entry point
#
exit( main( @ARGV ) );
