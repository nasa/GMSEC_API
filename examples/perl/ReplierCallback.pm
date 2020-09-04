 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
##############################
## Callback Class for Request-Reply ###
##############################
package ReplierCallback;


use GMSECAPI;
use Util;

sub new ($$) {
   my ($first, $configFile) = @_;
   my $class = ref($first) || $first;
   my $self = bless({ }, $class);
   $self->{CONFIG_FILE_CB} = $configFile;

   return $self;
}

sub OnMessage ($$$) {
   my ($self, $conn, $msg) = @_;

   my $tempString;
   my $logMessage;
   my $replyMessage;
   my $tempField;

   # We don't care about anything but reply messages
   my $msgKind = $msg->GetKind();
   if ($msgKind != $GMSEC_MSG_REQUEST)
   {
      return;
   }

   # Obtain the DIRECTIVE-KEYWORD field from the message.  It holds the 
   # request count.
   unless ($tempField = $msg->GetField("DIRECTIVE-KEYWORD")) {
      Util::check('GetValue(blank)', $msg->Status);
   }

   unless ($tempString = $tempField->GetValue()) {
      Util::check('GetValue(blank)', $tempField->Status);
   }

   # Output the request number which has been placed in the 
   # DIRECTIVE-KEYWORD field	
   print "\n\nReceived Request (", $tempString, "): ", $msg->GetTime();

   # Create a generic message container for the directive response message
   unless ($replyMessage = $conn->CreateMessage("TEMP.SUBJECT", 
                                                $GMSEC_MESSAGE_PUBLISH)) 
   {
      Util::check('CreateMessage(reply)', $conn->Status);
   }

   # Find and load the config file directive response message definition
   $self->{CONFIG_FILE_CB}->LookupMessage("DIRECTIVE-RESPONSE", $replyMessage);

   # Replace the DATA field with the request number we are responding to
   $tempField->SetType($GMSEC_TYPE_STRING);
   $tempField->SetName("DATA");
   $tempField->SetValue($tempString);
   unless ($replyMessage->AddField($tempField)) {
      Util::check('AddField(DATA)', $replyMessage->Status);
   }

   # Replace the TIME-COMPLETE field with the correct time
   $tempField->SetType($GMSEC_TYPE_STRING);
   $tempField->SetName("TIME-COMPLETED");
   $tempField->SetValue($msg->GetTime());
   unless ($replyMessage->AddField($tempField)) {
      Util::check('AddField(TIME_COMPLETED)', $replyMessage->Status);
   }

   # Set the reply subject to the request message's subject
   unless ($subject = $msg->GetSubject()) {
      Util::check('GetSubject()', $msg->Status);
   }

   unless ($replyMessage->SetSubject($subject)) {
      Util::check('SetSubject(reply)', $replyMessage->Status);
   }

   # Send the reply
   unless ($conn->Reply($msg, $replyMessage)) {
      Util::check('Reply', $conn->Status);
   }

   print "\nReply Sent (", $tempString, "): ", $msg->GetTime();

   # Create a generic message container for the log message
   unless ($logMessage = $conn->CreateMessage("TEMP.SUBJECT", 
                                              $GMSEC_MESSAGE_PUBLISH)) 
   {
      Util::check('CreateMessage(log)', $conn->Status);
   }

   # Find and load the config file log message definition
   $self->{CONFIG_FILE_CB}->LookupMessage("LOG-REP", $logMessage);

   # Fill the log message time
   $tempField->SetType($GMSEC_TYPE_STRING);
   $tempField->SetName("EVENT-TIME");
   $tempField->SetValue($msg->GetTime());
   unless ($logMessage->AddField($tempField)) {
      Util::check('AddField(EVENT-TIME)', $logMessage->Status);
   }

   # Fill the log message message text
   $tempField->SetType($GMSEC_TYPE_STRING);
   $tempField->SetName("MSG-TEXT");
   $tempField->SetValue($tempString);
   unless ($logMessage->AddField($tempField)) {
      Util::check('AddField(MSG-TEXT)', $logMessage->Status);
   }

   # Publish the log message
   unless ($conn->Publish($logMessage)) {
      Util::check('Publish)', $conn->Status);
   }

   # Clean up
   unless ($conn->DestroyMessage($replyMessage)) {
      Util::check('DestoryMessage(reply)', $conn->Status);
   }
   unless ($conn->DestroyMessage($logMessage)) {
      Util::check('DestoryMessage(log)', $conn->Status);
   }
}


1;
