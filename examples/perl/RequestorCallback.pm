 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
##############################
## Callback Class for Request-Reply ###
##############################
package RequestorCallback;

use GMSECAPI;
use Util;

sub new ($) { 
   my ($first, $xmlFile) = @_;
   my $class = ref($first) || $first;
   my $self = bless({ }, $class);
   $self->{CONFIG_FILE} = GMSECAPI::ConfigFile->new($xmlFile);

   unless($self->{CONFIG_FILE}->Load()) {
      Util::check('configFile->Load', $self->{CONFIG_FILE}->Status);
   }

   return $self;
}

sub OnMessage ($$$) {
  my ($self, $conn, $msg) = @_;

   my $tempString;
   my $tempField;
   my $tempStatus;
   my $logMessage;

   # We don't care about anything but reply messages
   my $msgKind = $msg->GetKind();
   if ($msgKind != $GMSEC_MSG_REPLY)
   {
      return;
   }

   # Get the status of the response (successful, failed, ect) and 
   # act accordingly
   unless ($tempField = $msg->GetField("RESPONSE-STATUS")) {
      Util::check('GetField(RESPONSE-STATUS)', $msg->Status);
   }
   unless ($tempStatus = $tempField->GetValue() ) {
      Util::check('GetValue(blank)', $msg->Status);
   }

   if ($tempStatus == 1)
   {
      print "\nAcknowledgement";
   }
   elsif($tempStatus == 2)
   {
      print "\nWorking/keep alive";
   }
   elsif($tempStatus == 3)
   {
      # Obtain the DATA field which was filled with what request this reply 
      # was generated from
      unless ($tempField = $msg->GetField("DATA") ) {
         Util::check('GetField(DATA)', $msg->Status);
      }

      unless ($tempString = $tempField->GetValue()) {
         Util::check('GetValue()', $tempField->Status);
      }

      # Output that we've received a reply
      print "\nReceived Reply (", $tempString, "): ", $msg->GetTime();
   }
   elsif ($tempStatus == 4)
   {
      print "\nFailed completion";
   }
   elsif($tempStatus == 5)
   {
      print "\nInvalid Request";
   }

   # Create a generic message container for the log message
   $logMessage = $conn->CreateMessage("temp.subject", $GMSEC_MESSAGE_PUBLISH);
   # Find and load the config file log message definition
   $self->{CONFIG_FILE}->LookupMessage("LOG-REQ", $logMessage);

   # Add Fields
   $tempField = GMSECAPI::Field->new;

   # Fill the log message time
   $tempField->SetType($GMSEC_TYPE_STRING);
   $tempField->SetName('EVENT-TIME');
   $tempField->SetValue($msg->GetTime());
   unless ($logMessage->AddField($tempField)) {
      Util::check('AddField(EVENT-TIME)', $logMessage->Status);
   }

   # Fill the log message message text
   $tempField->SetType($GMSEC_TYPE_STRING);
   $tempField->SetName('MSG-TEXT');
   $tempField->SetValue($tempString);
   unless ($logMessage->AddField($tempField)) {
      Util::check('AddField(MSG-TEXT)', $logMessage->Status);
   }

   # Publish the log message
   unless ($conn->Publish($logMessage)) {
      Util::check('Publish(logMessage)', $logMessage->Status);
   }

   # Clean up
   unless ($conn->DestroyMessage($logMessage)) {
      Util::check('DestroyMessage(logMessage)', $conn->Status);
   }
}


1;
