 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmsub_disp_rr.pl
#
# A Perl example demonstraton of GMSEC subscriber and requestor using the 
# dispatcher functionality.  The callback is registered with the Connection.
# A seperate thread, transmits a request and waits for a reply.  The
# Dispatcher is then started, which launches in its own thread.  We're
# looking for contention issues with the message queue, you should be able
# to recieve messages while still waiting for a reply.


use lib '../../bin/lib';
use strict;
use GMSECAPI;
use PublishCallback;
use RequestCallback;
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

   print "\n\nusage: perl gmsub_disp_rr.pl connectiontype=<middleware> ";
   print "[ <parameter>=<value> ]";
   print "\n\n\tNote that the parameter 'connectiontype' is required.";
   print "\n\tThe rest of other parameters are optional.";
   print "\n\n\tserver=<server name>";
   print " (required if middleware is not bolt/MB locally)";
   print "\n\tsubject=<subject name>";
   print "\n\tsubject.<N>=<sub-subject name>";
   print "\n\t\tNote: N must be 1 or greater";
   print "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B";
   print "\n\tmsg_timeout_ms=<timeout perios (milliseconds)>";
   print "\n\tprog_timeout_s=<timeout period (seconds)>";
   print "\n\tcfgfile=<config_filename.xml>";
   print "\n\nFor more information, see API User's Guide\n\n";

   return 1;
}


sub run
{
   my ($self, $config) = @_;

   $config->AddValue("isThreaded", "true");

   my @subjects = Util::getSubjects($config);
   my $msg_timeout_ms = Util::getInteger($config, 'MSG_TIMEOUT_MS', 
                                         $GMSEC_WAIT_FOREVER);
   my $prog_timeout_s = Util::getInteger($config, 'PROG_TIMEOUT_S',
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

   # Output middleware version
   Util::report("Middleware version = " . $connection->GetLibraryVersion());

   # Register the callback for Publish / Subscribe
   for my $subject (@subjects) {
     Util::report("Subscribing to Publisher (" . $subject . ")");
     my $cb = PublishCallback->new;
     unless ( $connection->Subscribe($subject, $cb, 'PublishCallback') ) {
        Util::check('Subscribe', $connection->Status);
     }
   }

   # Launch the auto dispatcher in a seperate thread
   Util::report("Starting Auto Dispatch\n");
   unless ($connection->StartAutoDispatch) {
      Util::check('StartAutoDispatch', $connection->Status);
   }

   # Create a request/reply callback, and make a request
   Util::report("Creating a request message\n");
   my $rcb = RequestCallback->new;

   my $request = $connection->CreateMessage("GMSEC.TEST.REQUEST",
                                            $GMSEC_MSG_REQUEST);
   Util::check('Creating Request Message(request)', $connection->Status);

   # Add Fields
   my $field = GMSECAPI::Field->new($GMSEC_TYPE_STRING, "QUESTION",
                "Does the request/reply functionality still work?");
   unless ($request->AddField($field)) {
      Util::check('AddField(QUESTION)', $connection->Status);
   }

   Util::report("Sending the request\n");
   unless ($connection->Request($request, $msg_timeout_ms,
                                $rcb, 'RequestCallback') ) {
      Util::check('Request using RequestCallback', $connection->Status);
   }
   
   if($prog_timeout_s == $GMSEC_WAIT_FOREVER)
   {
      # Block main thread until keyboard hit
      Util::report("Press the enter key to exit\n");
      my ($kbhit);
      read(STDIN, $kbhit, 1);
   }
   else
   {
      Util::millisleep($prog_timeout_s * 1000);
	  Util::report("Program Timeout Reached!");
   }

   Util::report("Stopping Auto Dispatch\n");
   unless ($connection->StopAutoDispatch) {
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
