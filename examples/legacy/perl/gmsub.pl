 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmsub.pl
#
# A example demonstration of GMSEC subscriber functionality.



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

   print "\n\nusage: perl gmsub.pl connectiontype=<middleware> ";
   print "[ <parameter>=<value> ]";
   print "\n\n\tNote that the parameter 'connectiontype' is required.";
   print "\n\tThe rest of other parameters are optional.";
   print "\n\n\tserver=<server name>";
   print " (required if middleware is not bolt/MB locally)";
   print "\n\tsubject=<subject name>";
   print "\n\titerations=<iterations>";
   print "\n\tsubject.<N>=<sub-subject name>";
   print "\n\t\tNote: N must be 1 or greater";
   print "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B";
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

   # Get the subjects 
   my @subjects = Util::getSubjects($config);
   my $iterations = Util::getInteger($config, 'ITERATIONS', 0);
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

   if ($iterations > 0)
   {
      Util::report("waiting for up to " . $iterations . " messages");
   }

   # Create the Connection
   my $connection = GMSECAPI::ConnectionFactory::Create($config);
   Util::check('ConnectionFactory.Create',
       GMSECAPI::ConnectionFactory::Status());
   $self->{CONNECTION} = $connection;

   # Connect
   unless ($connection->Connect) {
     Util::check('Connect', $connection->Status);
   }

   # Output middleware version
   Util::report("Middleware version = " . $connection->GetLibraryVersion());

   # Subscribe
   for my $subject (@subjects) {
     Util::report("subscribing to " . $subject);
     unless ($connection->Subscribe($subject)) {
 	     Util::check('Subscribe', $connection->Status);
     }
   }

   my $count = 0;
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
      
      if ($status->GetCode() == $GMSEC_TIMEOUT_OCCURRED) 
      { 
         Util::checkNoThrow('GetNextMsg(' . $msg_timeout_ms . ')', $status);
         next; 
      }
      else
      {
         Util::check('GetNextMsg(' . $msg_timeout_ms . ')', $status);
      }

      if ($message->isValid())
      {
         $count++;
         $done = 1 if ($iterations > 0 && $count >= $iterations);

         # Dump the message
         print "received =>\n", $message->ToXML, "\n";

         my $temp_subject = $message->GetSubject;
         Util::check('GetSubject()', $message->Status);

         $done = 1 if ($temp_subject eq "GMSEC.TERMINATE");

         my $temp_msg = $message;
         $message = $NULL;
         $self->{MESSAGE} = $message;
         # Destroy the Message
         unless ( $connection->DestroyMessage($temp_msg) ) {
            Util::check('DestroyMessage', $connection->Status);
         }
      }
   }
   return 1;
}


sub cleanup
{
   my ($self) = @_;
   

   my $message = $self->{MESSAGE};
   my $connection = $self->{CONNECTION};
	
   # Destroy the message
   if ($message) 
   {
      print "destroying message\n";
      unless ($connection->DestroyMessage($message)) {
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
