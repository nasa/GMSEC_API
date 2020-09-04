 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







# file Replier.pl
#
#	A example demonstration of various GMSEC API functionality.
#
#	Contained within this example:
#
#	- Loading an XML configuration file
#	- Populating a Config object using the XML file
#	- Creating a connection using the Config object
#	- Connecting to the network
#	- Using the XML file to store and set various other program configurations
#	- Subscribing to specific subjects to receieve messages through a callback
#	- Creating a heartbeat message from an XML definition
#	- Starting the automatic message dispatching thread
#	- Publishing the heartbeat message using the Publish call
#	- Programatically altering a field in the heartbeat message
#	* Creating a response message from an XML definition upon receiving a request
#	* Programatically altering a field in the response message
#	* Using the Reply call to send a reply message
#	- Creating a log message from an XML definition
#	- Publishing the log message using the Publish call
#	- Disconnecting from the network
#	- Message memory cleanup
#	- Connection cleanup
#
# Any item with a * next to it is specific to this file in the overall example.
#


use lib '../../bin/lib';
use strict;
use threads;
use threads::shared;

use GMSECAPI;
use ReplierCallback;
use Util;


###########
## Main ###
###########
{
   my $task = bless({});
   Util::driver($task, @ARGV);
}


sub printUsage
{
   my ($self, $config) = @_;
   my $configFile = Util::getString($config, 'XML_FILE', '');

   if (length($configFile) == 0) {
      print "\n\nusage: perl Replier.pl xml_file=<XML config file>\n\n";
      return 1;
   }
   return 0;
}


sub run
{
   my ($self, $connectionConfig) = @_;

   # Buffer autoflushing
   $| = 1;

   my $xml_file = Util::getString($connectionConfig, 'XML_FILE',
                                  'sampleAppsCommon.xml');

   # Create new ConfigFile object and associate the config filename
   my $configFile = GMSECAPI::ConfigFile->new($xml_file);

   $self->{CONFIG_FILE} = $configFile;

   # Call the Load method to load-up the config file's contents
   unless ($configFile->Load()) {
      Util::check('configFile->Load', $configFile->Status);
   }

   # Look up the config file that contains the confiuration for the middleware
   unless ($configFile->LookupConfig("connection-config", $connectionConfig)) {
      Util::check('LookupConfig(connection-config)', $configFile->Status);
   }

   # Create the Connection of the configuration
   my $connection = GMSECAPI::ConnectionFactory::Create($connectionConfig);
   Util::check('ConnectionFactory.Create',
               GMSECAPI::ConnectionFactory::Status());

   $self->{CONNECTION} = $connection;


   # Connect to the network
   unless ($connection->Connect) {
      Util::check('Connect', $connection->Status);
   }

   # Add the progam configuration
   my $programConfig = GMSECAPI::Config->new();

   # Look up additional program configutation information
   unless ($configFile->LookupConfig("program-config", $programConfig)) {
      Util::check('LookupConfig(program-config)', $configFile->Status);
   }

   my $updateRate = 0;
   my $loopCountdown = 0;

   # Set program config values
   unless ($updateRate = $programConfig->GetValue("update-rate")) {
      Util::check('GetValue(update-rate)', $programConfig->Status);
   }
   unless ($loopCountdown = $programConfig->GetValue("loop-time")) {
      Util::check('GetValue(loop-time)', $programConfig->Status);
   }

   my $cb = ReplierCallback->new($configFile);

   # Create subscriptions from subscription templates in the config file
   # using callbacks
   my $tempSubject;
   unless ($configFile->LookupSubscription("DIRECTIVE-REQUEST", $tempSubject))
   {
      Util::check('LookupSubscription(DIRECTIVE-REQUEST)',
                  $configFile->Status);
   }

   unless ($connection->Subscribe($tempSubject, $cb, 'ReplierCallback')) {
      Util::check('Subscribe', $connection->Status);
   }

   # Create Heartbeat Message
   $self->create_heartbeatMessage();

   # Main publishing loop
   $self->publish($updateRate, $loopCountdown);

   return 1;
}


sub create_heartbeatMessage
{
   my ($self) = @_;
   my $heartbeatMessage;
   my $tempField;
   my $heartbeatHolder = 0;

   my $configFile = $self->{CONFIG_FILE};
   my $connection = $self->{CONNECTION};

   # Create a generic message container for the heartbeat message
   unless ($heartbeatMessage = $connection->CreateMessage("TEMP.SUBJECT",
                                                 $GMSEC_MSG_PUBLISH)) 
   {
      Util::check('CreateMessage', $connection->Status);
   }
   $self->{HEARTBEAT_MESSAGE} = $heartbeatMessage;

   # Find and load the config file heartbeat message definition
   unless ($configFile->LookupMessage("C2CX-HEARTBEAT-REP", $heartbeatMessage)) {
      Util::check('LookupMessage(C2CX-HEARTBEAT-REP)', $configFile->Status);
   }

   # Obtain the publish rate field, by name, from the heartbeat message
   unless ($tempField = $heartbeatMessage->GetField("PUB-RATE")) {
      Util::check('GetField(PUB-RATE)', $heartbeatMessage->Status);
   }

   unless ($heartbeatHolder = $tempField->GetValue()) {
      $heartbeatHolder = 30;
   }

   $self->{HEARTBEAT_HOLDER} = $heartbeatHolder;
}

sub publish
{
   my ($self, $updateRate, $loopCountdown) = @_;

   my $connection = $self->{CONNECTION};
   my $heartbeatMessage= $self->{HEARTBEAT_MESSAGE};
   my $heartbeatHolder = $self->{HEARTBEAT_HOLDER};

   my $heartbeatCount = 1;
   my $heartbeatCountdown = 0;

   # Output some general program information
   print "Publishing for ", $loopCountdown, " seconds.\n";
   print "Publishing Heartbeat Message every ", $heartbeatHolder, 
         " seconds.\n";

   # Call to create the thread that will automatically retrieve messages 
   # off the message queue
   unless ($connection->StartAutoDispatch()) {
      Util::check('StartAutoDispatch', $connection->Status);
   }

   # Start time
   print "Start Time: ", $heartbeatMessage->GetTime(), "\n\n";

   # Publishing loop
   while ($loopCountdown > 0)
   {
      # When the countdown reaches 0, we publish a heartbeat message and
      # reset the counter
      if ($heartbeatCountdown < 1)
      {
         $heartbeatCount++;

         # Update the message counter
         my $tempField = GMSECAPI::Field->new($GMSEC_TYPE_SHORT, "COUNTER", 
                                              $heartbeatCount);
         $heartbeatMessage->AddField($tempField);

         # Publish the heartbeat message
         unless ($connection->Publish($heartbeatMessage)) {
            Util::check('Publish', $connection->Status);
         }

         # Output a heartbeat marker and reset the counter
         print "\nheartbeat\n";
         $heartbeatCountdown = $heartbeatHolder;
      }

      # Decrement the counters
      $heartbeatCountdown = $heartbeatCountdown - $updateRate;
      $loopCountdown--;
      sleep 1;
   }

   # End time
   print "\n\nEnd Time: ", $heartbeatMessage->GetTime(), "\n\n";	
}


sub cleanup
{
   my ($self) = @_;
   my $connection = $self->{CONNECTION};
   my $heartbeatMessage = $self->{HEARTBEAT_MESSAGE};

   # Stop auto dispatch
   print "Stop auto dispatch\n" if $connection;
   unless ($connection->StopAutoDispatch()) {
      Util::check('StopAutoDispatch', $connection->Status);
   }

   # Destroy the heartbeat message
   if ($heartbeatMessage)
   {
      print "destroying heartbeat message\n";
      unless ($connection->DestroyMessage($heartbeatMessage)) {
         Util::check('DestroyMessage(heartbeat)', $connection->Status);
      }
   }

   # Destroy the connection
   if ($connection)
   {
      if ($connection->IsConnected())
      {
          print "disconnecting\n";
          unless ($connection->Disconnect()) {
             Util::check('StopAutoDispatch', $connection->Status);
          }
      }

      print "destroying connection\n";
      unless (GMSECAPI::ConnectionFactory::Destroy($connection)) {
             Util::check('ConnectionFactory:Destroy',
                          GMSECAPI::ConnectionFactory::Status);
      }
   }
}

1;

__END__
