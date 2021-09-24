 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmreq_cb.pl
#
#
# A Perl example demonstration of GMSEC callback requestor functionality.


use lib '../../bin/lib';
use strict;
use GMSECAPI;
use ReplyCallback;
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

   print "\n\nusage: perl gmreq_cb.pl connectiontype=<middleware> ";
   print "[ <parameter>=<value> ]";
   print "\n\n\tNote that the parameter 'connectiontype' is required.";
   print "\n\tThe rest of other parameters are optional.";
   print "\n\n\tserver=<server name>";
   print " (required if middleware is not bolt/MB locally)";
   print "\n\tsubject=<subject name>";
   print "\n\tmsg_timeout_ms=<timeout period (milliseconds)>";
   print "\n\tcfgfile=<config_filename.xml>";
   print "\n\nFor more information, see API User's Guide\n\n";

   return 1;
}


sub run
{
   my ($self, $config) = @_;

   my $subject = Util::getString($config, 'SUBJECT',
                                 'GMSEC.MISSION.SAT_ID.REQ.CMD.GMRPL');
   my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS',
                                          $GMSEC_WAIT_FOREVER);

   # Create the Connection
   my $connection = GMSECAPI::ConnectionFactory::Create($config);
   Util::check('ConnectionFactory.Create',
       GMSECAPI::ConnectionFactory::Status());
   $self->{CONNECTION} = $connection;

   # Connect
   unless ($connection->Connect) {
      Util::check('Connect', $connection->Status);
   }

   # Request - Reply
   my $request = $connection->CreateMessage($subject, $GMSEC_MSG_REQUEST);
   Util::check('CreateMessage(request)', $connection->Status);

   # Add Fields
   my $field = GMSECAPI::Field->new;

   $field->SetType($GMSEC_TYPE_STRING);
   $field->SetName("QUESTION");
   $field->SetValue("Does the request/reply functionality still work?");
   unless ($request->AddField($field)) {
      Util::check('AddField<QUESTION>', $request->Status);
   }

   # Add the name of the component making the Request.
   $field->SetName("COMPONENT");
   $field->SetValue("GMREQ");
   unless ($request->AddField($field)) {
      Util::check('AddField<COMPONENT>', $request->Status);
   }

   # Add the Message ID of this Request.  In theory the MSG-ID should be unique,
   # but here it is not for the sake of simplicity.  See API Interface Specification
   # for suggetions on creating a MSG-ID.
   $field->SetName("MSG-ID");
   $field->SetValue("GMREQ-MSG");
   unless ($request->AddField($field)) {
      Util::check('AddField<MSG-ID>', $request->Status);
   }

   # Dump Request
   print "built request =>\n", $request->ToXML, "\n";

   # Send Request Message
   my $cb = ReplyCallback->new();

   # Start the dispatcher which processes callbacks 	
   my $status = $connection->StartAutoDispatch;	

   unless ($connection->Request($request, $msg_timeout_ms, 
                                $cb, 'ReplyCallback')) {
      Util::check('Request via Callback', $request->Status);
   }

   print "Waiting for reponse...\n";
   my $done = 0;
   my $elapsedTime = 0;
   
   while($cb->checkCallbackFlag == 0 && $done == 0)
   {
      Util::millisleep(100);
	  if($msg_timeout_ms != $GMSEC_WAIT_FOREVER)
	  {
		 $elapsedTime += 100;
		 $done = 1 if($elapsedTime >= $msg_timeout_ms)
	  }
   }
   
   # Destroy the Request
   unless ($connection->DestroyMessage($request)) {
      Util::check('Request via Callback', $request->Status);
   }
 
   $status = $connection->StopAutoDispatch;
   
   if($cb->checkCallbackFlag == 0)
   {
      print "No Response Recieved\n";
   }
   else
   {
      print "Response Recieved\n";
   }	
}

sub cleanup
{
   my ($self) = @_;
   my $connection = $self->{CONNECTION};

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
