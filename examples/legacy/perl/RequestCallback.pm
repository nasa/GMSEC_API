 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# RequestCallback.pm
#
#
# A Perl example request callback class.


package RequestCallback;

use strict;
use GMSECAPI;


sub new ($) 
{ 
    return bless({ }, shift); 
}


sub OnMessage ($$$) 
{
   my ($self, $conn, $msg) = @_;

   print "[RequestCallback::OnMessage] inside message callback\n";

   if (ref($msg) eq 'GMSECAPI::Message')
   {
      print $msg->ToXML, "\n";
   } 
   else 
   {
      print "[RequestCallback::OnMessage] bad request\n";
      return;
   }

   # Get Subject
   #my $subject = $msg->GetSubject;

   # Construct Reply subject.
   my $msgField = $msg->GetField("COMPONENT");
   my $compVal = $msgField->GetValue();

   $msgField = $msg->GetField("MSG-ID");
   my $idVal = $msgField->GetValue();

   # Set Status Code.  See API Interface Specification for available status codes.
   my $statusCode = 1;

   my $reply_subject = "GMSEC.MISSION.SAT_ID.RESP.".$compVal.".".$idVal.".".$statusCode;

   # Create Reply
   my $reply = $conn->CreateMessage($reply_subject, $GMSEC_MSG_REPLY);
   if ($GMSEC_STATUS_NO_ERROR != $conn->Status->GetClass) 
   {
      print $conn->Status->Get, "\n";
      return;
   }

   # Add Fields
   my $fd = GMSECAPI::Field->new($GMSEC_TYPE_STRING, "ANSWER",
                                 "Sure looks like it!");
   $reply->AddField($fd);

   # Add the name of the component sending the Reply.
   $fd->SetName("COMPONENT");
   $fd->SetValue("GMRPL");
   $reply->AddField($fd);

   # Dump Reply
   print $reply->ToXML, "\n";

   # Send Reply
   unless ( $conn->Reply($msg, $reply) ) 
   {
      print $conn->Status->Get, "\n";
   }
}


1;
