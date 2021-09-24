 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmsub_cb.pl
#
#
# A Perl example demonstration of GMSEC callback subscriber functionality.



use lib '../../bin/lib';
use strict;
use GMSECAPI;
use PublishCallback;
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

   print "\n\nusage: perl gmsub_cb.pl connectiontype=<middleware> ";
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
   print "\n\tcfgfile=<config_filename.xml>";
   print "\n\nFor more information, see API User's Guide\n\n";

   return 1;
}


sub run
{
   my ($self, $config) = @_;

   my @subjects = Util::getSubjects($config);
   my $iterations = Util::getInteger($config, 'ITERATIONS', 0);

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

   my $cb = PublishCallback->new;

   # Subscribe via callback
   for my $subject (@subjects) {
     Util::report("Subscribing to " . $subject);
     unless ($connection->Subscribe($subject, $cb, 'PublishCallback')) {
       Util::check('Subscribe', $connection->Status);
     }
   }

   # Start autodispatcher to process callbacks	
   print "Start auto dispatch...\n";
   my $status = $connection->StartAutoDispatch;

   # Listen
   my $done = 0;
   while (not $done)
   {
      #Loop, checking to make sure connection has no problems
      $done = 1 if ($connection->Status->isError());
   } 

   print "Stop auto dispatch\n";
   $status= $connection->StopAutoDispatch;	
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
