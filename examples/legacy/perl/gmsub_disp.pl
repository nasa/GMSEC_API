 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmsub_disp.pl
#
#
# A Perl example demonstration of GMSEC subscriber with auto-dispatching
# using dispatcher functionality.



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

   print "\n\nusage: perl gmsub_disp.pl connectiontype=<middleware> ";
   print "[ <parameter>=<value> ]";
   print "\n\n\tNote that the parameter 'connectiontype' is required.";
   print "\n\tThe rest of other parameters are optional.";
   print "\n\n\tserver=<server name>";
   print " (required if middleware is not bolt/MB locally)";
   print "\n\tsubject=<subject name>";
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

   # Output middleware version
   Util::report("Middleware version = " . $connection->GetLibraryVersion());

   # Subscribe 
   for my $subject (@subjects) {
     Util::report("Subscribing to Publisher (" . $subject . ")\n");
     my $cb = PublishCallback->new;
     unless ($connection->Subscribe($subject, $cb, 'PublishCallback') ) {
       Util::check('Subscribe', $connection->Status);
     }
   }
   # Auto Dispatch
   Util::report("Starting AutoDispatch");
   unless ($connection->StartAutoDispatch) {
     Util::check('StartAutoDispatch', $connection->Status);
   }

   Util::report("Press the enter key to exit\n");

   my ($kbhit);
   read(STDIN, $kbhit, 1);
   Util::report("Stopping AutoDispatch\n");
   unless ( $connection->StopAutoDispatch ) {
     Util::check('StopAutoDispatch', $connection->Status);
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
         Util::report("disconnecting\n");
         unless ($connection->Disconnect) {
            Util::check('Disconnect', $connection->Status);
         }
      }

      # Destroy the Connection
      Util::report("destroying connection\n");
      unless (GMSECAPI::ConnectionFactory::Destroy($connection)) {
          Util::check('ConnectionFactory.Destroy',
                      GMSECAPI::ConnectionFactory::Status());
      }
   }
}


1;

__END__
