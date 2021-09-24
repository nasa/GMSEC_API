 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmrpl.pl
#
#
# A Perl example demonstratioin of GMSEC reply functionality.



use lib '../../bin/lib';
use strict;
use GMSECAPI;
use Util;


{
   my $task = bless({});
   Util::driver($task, @ARGV);
}


sub printUsage
{
   my ($self, $config) = @_;
   my $hasConnectionType = Util::hasConnectionType($config);

   return 0 if ($hasConnectionType);

   print "\n\nusage: perl gmrpl.pl connectiontype=<middleware> ";
   print "[ <parameter>=<value> ]";
   print "\n\n\tNote that the parameter 'connectiontype' is required.";
   print "\n\tThe rest of other parameters are optional.";
   print "\n\n\tserver=<server name>";
   print " (required if middleware is not bolt/MB locally)";
   print "\n\tsubject=<subject name>";
   print "\n\tmsg_timeout_ms=<timeout period (milliseconds)>";
   print "\n\tprog_timeout_s=<timeout period (seconds)>";
   print "\n\t\tNote: msg_timeout_ms must be specified for prog_timeout_s";
   print "\n\tcfgfile=<config_filename.xml>";
   print "\n\nFor more information, see API User's Guide\n\n";

   return 1;
}


sub run
{
   my ($self, $config) = @_;

   my $subject = Util::getString($config, 'SUBJECT', 'GMSEC.>'); 
   my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS', 
                                         $GMSEC_WAIT_FOREVER);
                                         
   my $prog_timeout_s = Util::getInteger($config, 'PROG_TIMEOUT_S',
   										 $GMSEC_WAIT_FOREVER);
   
   if($msg_timeout_ms == $GMSEC_WAIT_FOREVER && 
						$prog_timeout_s != $GMSEC_WAIT_FOREVER)
   {
		Util::report("Usage: To use prog_timeout_s, msg_timeout_ms must be specified!");
		return 1;
   }   

   # Output GMSEC API version
   Util::report(GMSECAPI::ConnectionFactory::GetAPIVersion());

   # Create the Connection
   my $connection = GMSECAPI::ConnectionFactory::Create($config);
   Util::check('ConnectionFactory.Create',
       GMSECAPI::ConnectionFactory::Status());
   $self->{CONNECTION} = $connection;

   # Connect
   unless ($connection->Connect) {
     Util::check('Connect', $connection->Status);
   }


   # output connection middleware version
   Util::report("Middleware version = " . $connection->GetLibraryVersion());

   # Subscribe
   Util::report("Subscribing to " . $subject);
   unless ($connection->Subscribe($subject)) {
     Util::check('Subscribe', $connection->Status);
   }

   Util::report("Subscribing to " . "GMSEC.TERMINATE" . "\n");
   unless ($connection->Subscribe('GMSEC.TERMINATE')) {
     Util::check('Subscription to GMSEC.TERMINATE failed', $connection->Status);
   }

   my $done = 0;
   my $prevTime = time();
   my $nextTime;
   my $elapsedTime = 0;

   # Listen
   while (not $done)
   {
   
	  if($elapsedTime >= $prog_timeout_s && $prog_timeout_s != $GMSEC_WAIT_FOREVER)
	  {
	     $done = 1;
	     next;
	  }
	  
      my $message = $connection->GetNextMsg($msg_timeout_ms);
      my $status = $connection->Status;
	  
	  if($prog_timeout_s != $GMSEC_WAIT_FOREVER)
	  {
	  	 $nextTime = time();
	     $elapsedTime += ($nextTime - $prevTime);
	     $prevTime = $nextTime;
	  }
	  
      $self->{MESSAGE} = $message;

      if ($status->isError())
      {
         if ($status->GetCode() == $GMSEC_TIMEOUT_OCCURRED)
         {
            Util::checkNoThrow('GetNextMsg(' . $msg_timeout_ms . ')', $status);
            next;
         }
         Util::checkThrow('GetNextMsg(' . $msg_timeout_ms . ')', $status);
      }

      my $temp_subject = $message->GetSubject;
      Util::check('GetSubject()', $message->Status);
      $done = 1 if ($temp_subject eq "GMSEC.TERMINATE");

      # Dump the message
      print "received =>\n", $message->ToXML, "\n";

      # check for request
      my $kind = $message->GetKind;

      if ($GMSEC_MSG_REQUEST == $kind ) 
      {
         my $temp_subject = $message->GetSubject;

         # Construct Reply subject.
         my $messageField = $message->GetField("COMPONENT");
         my $compVal = $messageField->GetValue();

         $messageField = $message->GetField("MSG-ID");
         my $idVal = $messageField->GetValue();

         # Set Status Code.  
         # See API Interface Specification for available status codes.
         my $statusCode = 1;

         my $reply_subject = "GMSEC.MISSION.SAT_ID.RESP.".$compVal.
                             ".".$idVal.".".$statusCode;

         # Create Reply if not done
         my $reply;
         if (not $done)
         {
            $reply = $connection->CreateMessage($reply_subject, 
                                                $GMSEC_MSG_REPLY);
         }
         else
         {
            $reply = $connection->CreateMessage($temp_subject, 
                                                $GMSEC_MSG_REPLY);
         }
         Util::check('CreateMessage(reply)', $connection->Status);
         $self->{REPLY} = $reply;

         # Add Fields
         my $field = GMSECAPI::Field->new($GMSEC_TYPE_STRING, "ANSWER",
                                          "Sure looks like it!");
         unless ($reply->AddField($field)) {
            Util::check('AddField(ANSWER)', $reply->Status);
         }

         $self->{FIELD} = $field;

         # Add the name of the component sending the Reply.
         $field->SetName("COMPONENT");
         $field->SetValue("GMRPL");
         unless ($reply->AddField($field)) {
            Util::check('AddField(COMPONENT)', $reply->Status);
         }

         # Dump Reply
         print "prepared reply =>\n", $reply->ToXML, "\n";

         # Send Reply
         unless ($connection->Reply($message, $reply)) {
            Util::check('Connection->Reply', $connection->Status);
         }

         print "\nsent reply =>\n";

         my $temp_msg = $message;
         $message = $NULL;
         $self->{MESSAGE} = $message;

         # Destroy inbound message
         unless ( $connection->DestroyMessage($temp_msg) ) {
            Util::check('DestroyMessage', $connection->Status);
         }
      }
   }

}


sub cleanup
{
   my ($self) = @_;

   my $reply = $self->{REPLY};
   my $message = $self->{MESSAGE};
   my $connection = $self->{CONNECTION};

   # Destroy the message
   if ($message && $message->isValid())
   {
      print "destroying message\n";
      unless ($connection->DestroyMessage($message)) {
         Util::check('DestroyMessage', $connection->Status);
      }
   }

   # Destroy the reply message
   if ($reply && $reply->isValid())
   {
      print "destroying reply message\n";
      unless ($connection->DestroyMessage($reply)) {
         Util::check('DestroyMessage', $connection->Status);
      }
   }

   if ($connection)
   {
      # Disconnect if necessary
      if ($connection->IsConnected)
      {
         print "disconnecting\n";
         unless ($connection->Disconnect) {
            Util::check('Disconnect', $connection->Status);
         }
      }

      # Destroy the Connection
      print "destroying connection\n";
      unless (GMSECAPI::ConnectionFactory::Destroy($connection)) {
         Util::check('ConnectionFactory.Destroy',
                     GMSECAPI::ConnectionFactory::Status());
      }
   }
}


1;

__END__

