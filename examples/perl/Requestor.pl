 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# file Requestor.pl
#
#	A Perl example demonstration of various GMSEC API functionality.

#	Contained within this example:

#	- Loading an XML configuration file
#	- Populating a Config object using the XML file
#	- Creating a connection using the Config object
#	- Connecting to the network
#	- Using the XML file to store and set various other program configurations
#	- Creating a heartbeat message from an XML definition
#	- Publishing the heartbeat message using the Publish call
#	- Programatically altering a field in the heartbeat message
#	* Creating a directive request message from an XML definition
#	* Publishing the directive request message using the Request call
#	* Using a callback class object to receieve a response from the request call
#	- Creating a log message from an XML definition
#	- Programatically altering a field in the log message
#	- Publishing the log message using the Publish call
#	- Disconnecting from the network
#	- Message memory cleanup
#	- Connection cleanup

#	Any item with a * next to it is specific to this file in the overall example.

use strict;
use lib '../../bin/lib';
# Include the GMSECAPI and callback packages
use GMSECAPI;
use RequestorCallback;
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
      print "\n\nusage: perl Requestor.pl xml_file=<XML config file>\n\n";
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


   $self->{CONFIG_FILE} = $configFile;
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
   my $directiveHolder = 0;

   # Set program config values
   unless ($updateRate = $programConfig->GetValue("update-rate")) {
      Util::check('GetValue(update-rate)', $programConfig->Status);
   }
   unless ($loopCountdown = $programConfig->GetValue("loop-time")) {
      Util::check('GetValue(loop-time)', $programConfig->Status);
   }
   unless ($directiveHolder = $programConfig->GetValue("send-directive-rate")) 
   {
      Util::check('GetValue(send-directive-rate)', $programConfig->Status);
   }

   my $directiveCountdown = $directiveHolder;
   my $cb = RequestorCallback->new($xml_file);

   $connection->StartAutoDispatch;

   $self->{CALLBACK} = $cb;

   $self->create_heartbeatMessage($updateRate, $loopCountdown);

   $self->create_directiveRequestMessage();

   $self->publish($updateRate, $loopCountdown, $directiveHolder, 
                  $directiveCountdown);
   
   return 1;
}

sub create_heartbeatMessage
{
   my ($self, $updateRate, $loopCountdown) = @_;

   my $configFile = $self->{CONFIG_FILE};
   my $connection = $self->{CONNECTION};

   my $heartbeatHolder = 0;
   my $tempField;
   my $heartbeatMessage;

   # Create Heartbeat Message ####

   # Create a generic message container for the heartbeat message
   unless ($heartbeatMessage = $connection->CreateMessage("TEMP.SUBJECT", 
                                                          $GMSEC_MSG_PUBLISH))
   {
      Util::check('CreateMessage', $connection->Status);
   }
   $self->{HEARTBEAT_MESSAGE} = $heartbeatMessage;

   # Find and load the config file heartbeat message definition
   unless ($configFile->LookupMessage("C2CX-HEARTBEAT-REQ", $heartbeatMessage)) {
      Util::check('LookupMessage(C2CX-HEARTBEAT-REQ)', $configFile->Status);
   }

   # Obtain the publish rate field, by name, from the heartbeat message
   unless ($tempField = $heartbeatMessage->GetField("PUB-RATE")) {
      Util::check('GetField(PUB-RATE)', $heartbeatMessage->Status);
   }

   $self->{TEMP_FIELD} = $tempField;

   unless ($heartbeatHolder = $tempField->GetValue()) {
      $heartbeatHolder = 30;
   }

   $self->{HEARTBEAT_HOLDER} = $heartbeatHolder;
}


# Create Directive Request Message 
sub create_directiveRequestMessage
{
   my ($self) = @_;

   my $configFile = $self->{CONFIG_FILE};
   my $connection = $self->{CONNECTION};

   # Create a generic message container for the Directive Request Message
   my $directiveRequestMessage = 0;
   unless ($directiveRequestMessage = $connection->CreateMessage(
                                                           "TEMP.SUBJECT",
                                                           $GMSEC_MSG_REQUEST))
   {
      Util::check('CreateMessage(directiveRequest)', $configFile->Status);
   }

   $self->{DIRECTIVE_REQUEST} = $directiveRequestMessage;

   # Find and load the config file Directive Request Message definition
   unless ($configFile->LookupMessage("DIRECTIVE-REQUEST", 
                                      $directiveRequestMessage)) 
   {
      Util::check('LookupMessage(DIRECTIVE-REQUEST)', $configFile->Status);
   }

}


# Main Publishing Loop 
sub publish
{
   my ($self, $updateRate, $loopCountdown, 
       $directiveHolder, $directiveCountdown) = @_;

   my $connection = $self->{CONNECTION};
   my $cb = $self->{CALLBACK};
   my $heartbeatMessage= $self->{HEARTBEAT_MESSAGE};
   my $heartbeatHolder = $self->{HEARTBEAT_HOLDER};
   my $directiveRequestMessage= $self->{DIRECTIVE_REQUEST};
   my $tempField = $self->{TEMP_FIELD};

   my $heartbeatCount = 1;
   my $heartbeatCountdown = 0;
   my $requestCount = 0;

   # Output some general program information
   print "Publishing for ", $loopCountdown, " seconds.\n";
   print "Publishing Directive Request Messages every ", $directiveHolder,
         " seconds.\n";
   print "Publishing Heartbeat Messages every ", $heartbeatHolder, 
         " seconds.\n";

   # Start time
   print "Start Time: ", $heartbeatMessage->GetTime();

   # Publishing loop
   while ($loopCountdown > 0)
   {
      if ($directiveCountdown < 1)
      {
         $requestCount++;

         ## When the countdown reaches 0, we publish a directive message and
         ##  reset the counter
         $tempField->SetType($GMSEC_TYPE_STRING);
         $tempField->SetName("DIRECTIVE-KEYWORD");
         $tempField->SetValue($requestCount);
         $directiveRequestMessage->AddField($tempField);

         # Alter the directive request to a unique string
         $tempField->SetType($GMSEC_TYPE_STRING);
         $tempField->SetName("DIRECTIVE-STRING");
         $tempField->SetValue("DIRECTIVE REQUEST MESSAGE" + $requestCount);
         $directiveRequestMessage->AddField($tempField);

         # Publish Time
         print "\n\nPublishing Directive Request Message (", $requestCount, 
               "): ", $directiveRequestMessage->GetTime();

         # Send the request with the reply being send to the callback 
         # we created
         my $timeout = 3 * 1000; # 3 seconds
         unless ($connection->Request($directiveRequestMessage, 
                                      $timeout, $cb, 
                                      'RequestorCallback')) 
         {
            Util::check('Request(directiveRequest)', $connection->Status);
         }
         $directiveCountdown = $directiveHolder;
      }
      # When the countdown reaches 0, we publish a heartbeat message and
      # reset the counter		
      if ($heartbeatCountdown < 1)
      {
         $heartbeatCount++;

         # Update the message counter field
         my $tempField = GMSECAPI::Field->new($GMSEC_TYPE_I16, "COUNTER", 
                                              $heartbeatCount);
         $heartbeatMessage->AddField($tempField);

         # Publish the heartbeat message
         unless ($connection->Publish($heartbeatMessage)) {
            Util::check('Publish(heartbeat)', $connection->Status);
         }

         # Ouput a heartbeat marker and reset the counter
         print "\nheartbeat\n";
         $heartbeatCountdown = $heartbeatHolder;
      }

      $directiveCountdown = $directiveCountdown - $updateRate;
      $loopCountdown--;
      sleep 1;
   }

   # End time
   print "\n\nEnd Time: ", $heartbeatMessage->GetTime(), "\n\n";	

   return 1;
}


sub cleanup
{
   my ($self) = @_;

   my $connection = $self->{CONNECTION};
   my $heartbeatMessage = $self->{HEARTBEAT_MESSAGE};
   my $directiveRequestMessage = $self->{DIRECTIVE_REQUEST};

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

   # Destroy the directive request message
   if ($directiveRequestMessage)
   {
      print "destroying directive request message\n";
      unless ($connection->DestroyMessage($directiveRequestMessage)) {
         Util::check('DestroyMessage(directiveRequest)', $connection->Status);
      }
   }

   if ($connection)
   {
      if ($connection->IsConnected())
      {
          print "disconnecting\n";
          unless ($connection->Disconnect()) {
             Util::check('Disconnect', $connection->Status);
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
