 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmrpl_cb.pl
#
#
# A Perl example demonstration of GMSEC callback reply functionality.



use lib '../../bin/lib';
use strict;
use GMSECAPI;
use Util;
use RequestCallback;

{
   my $task = bless({});
   Util::driver($task, @ARGV);
}


sub printUsage
{
   my ($self, $config) = @_;
   my $hasConnectionType = Util::hasConnectionType($config);

   return 0 if ($hasConnectionType);

   print "\n\nusage: perl gmrpl_cb.pl connectiontype=<middleware> ";
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

   my $subject = Util::getString($config, 'SUBJECT', 'GMSEC.>');
   my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS',
                                         $GMSEC_WAIT_FOREVER);

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

   # Subscribe
   my $cb = RequestCallback->new;
   Util::report("Subscribing to " . $subject);
   unless ($connection->Subscribe($subject, $cb, 'RequestCallback')) {
      Util::check('Subscribe', $connection->Status);
   }

   Util::report("Subscribing to " . "GMSEC.TERMINATE" . "\n");
   unless ($connection->Subscribe('GMSEC.TERMINATE', $cb, 'RequestCallback')) {
      Util::check('Subscribe', $connection->Status);
   }

   my $status = $connection->StartAutoDispatch;	
   my $done = 0;

   # Listen
   while (not $done)
   {
      #Loop indefinitely, checking to make sure connection hasn't encountered an error
      $done = 1 if ($connection->Status->isError());
   }

   $status = $connection->StopAutoDispatch;	
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
