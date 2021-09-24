 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmreq.pl
#
# A Perl example demonstration of GMSEC requestor functionality.



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

   print "\n\nusage: perl gmreq.pl connectiontype=<middleware> ";
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

   # Output information
   Util::report("Using subject '" . $subject . "'");

   # Request - Reply
   my $request = $connection->CreateMessage($subject, $GMSEC_MSG_REQUEST);
   Util::check('CreateMessage(request)', $connection->Status);

   $self->{REQUEST} = $request;

   my $status;

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
   my $reply = $connection->Request($request, $msg_timeout_ms);
   Util::check('Request', $connection->Status);

   $self->{REPLY} = $reply;

   # Dump Reply
   print "received reply =>\n", $reply->ToXML, "\n";

   # Destroy the reply and request
   my $temp_reply = $reply;
   $reply = $NULL;
   $self->{REPLY} = $reply;
   unless ($connection->DestroyMessage($temp_reply)) {
      Util::check('DestroyMessage(temp_reply)', $connection->Status);	
   }

   my $temp_request = $request;
   $request = $NULL;
   $self->{REQUEST} = $request;
   unless ( $connection->DestroyMessage($temp_request)) {
      Util::check('DestroyMessage(temp_request)', $connection->Status);	
   }
}

sub cleanup
{
   my ($self) = @_;

   my $request = $self->{REQUEST};
   my $reply = $self->{REPLY};
   my $connection = $self->{CONNECTION};

   # Destroy the Reply
   if ($reply)
   {
      print "destorying reply\n";
      unless ( $connection->DestroyMessage($reply) ) 
      {
         Util::check('DestroyMessage(reply)', $connection->Status);	
      }
   }

   # Destroy the Request
   if ($request)
   {
      print "destorying request\n";
      unless ($connection->DestroyMessage($request)) 
      {
         Util::check('DestroyMessage(reply)', $connection->Status);	
      }
   } 

   # Destroy the connection
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
