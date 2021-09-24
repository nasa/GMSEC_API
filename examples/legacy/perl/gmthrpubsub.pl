 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# gmthrpubsub.pl
#
#
# A Perl example demonstration of GMSEC subscriber in the main thread subscribing to
# what a publisher in anoter thread publishes using the GMSEC standard Perl API
# and Perl threads.



use lib '../../bin/lib';
use strict;
use threads;
use GMSECAPI;
use Util;


# Number of messages to send/receive
my $nmsgs = 3; 

# Time to wait between messages
my $minterval = 2; 


{
   my $task = bless({});
   Util::driver($task, @ARGV);
}


sub create_config_from_file
{
   my $XMLConfigPath = shift;
   my $config_name = shift;

   # Create new ConfigFile object and associate the config filename
   #
   # NOTE:
   #    Thers's no status or return value in the C++ API,
   #    and therefore no way to check the success/failure here.
   #      $self{ConfigFile} doesn't need to be "freed".
   #
   my $configFile = GMSECAPI::ConfigFile->new($XMLConfigPath);

   #
   # Call the Load method to load-up the config file's contents
   #
   unless($configFile->Load()) {
      Util::check('configFile->Load', $configFile->Status);
   }
   #
   # Create a Config object to pass into the LookupConfig call
   #     $config doesn't need to be "freed".
   #
   my $config = GMSECAPI::Config->new();

   #
   # Look up the config file that contains the confiuration for the middleware
   #
   unless($configFile->LookupConfig($config_name, $config)) {
      Util::check('configFile->LookupConfig', $configFile->Status);
   }

   # Create the Connection
   my $conn = GMSECAPI::ConnectionFactory::Create($config);
   my $result = GMSECAPI::ConnectionFactory::Status();
   Util::check('ConnectionFactory::Create', $result);

   return $conn;
}


sub PublishTestMessage ($$) {

   my ($conn, $subject) = @_;

   my $i = 123;

   my $msg = $conn->CreateMessage($subject, $GMSEC_MSG_PUBLISH);

   if ($conn->Status->isError())
   {
      Util::checkNoThrow('PublishTestMessage:CreateMessage', $conn->Status);
      return 0;
   }

   # Add Field
   my $field = GMSECAPI::Field->new($GMSEC_TYPE_CHAR, "C", 'c');
   unless ($msg->AddField($field)) {
      Util::checkNoThrow('PublishTestMessage:AddField(<GMSEC_CHAR>)',
                         $msg->Status);
      return 0;
   }

   # Dump Message
   print "Sent a message =>\n", $msg->ToXML, "\n";

   # Publish Message
   unless ( $conn->Publish($msg) ) {
      Util::checkNoThrow('Publish', $conn->Status);
      return 0;
   }

   # Destroy the Message
   unless ($conn->DestroyMessage($msg)) {
      Util::checkNoThrow('DestroyMessage(msg)', $conn->Status);
      return 0;
    }

    return 1;
}


sub publisher
{
   my $conn = shift;

   # Connect
   unless ($conn->Connect) {
     Util::check('Connect from publisher', $conn->Status);
   }

   # Publish messages forever, sleeping for 2 seconds in between & at start
   for (my $i = 0; $i < $nmsgs; $i++)
   {
        sleep($minterval);
        PublishTestMessage($conn, "GMSEC.TEST.GMTHRPUBSUB" );
   }
   print "===> start disconnecting thread\n";

   
   if ($conn)
   {
      # Disconnect if necessary
      if ($conn->IsConnected)
      {
         print "disconnecting\n";
         unless ($conn->Disconnect) {
            Util::check('Disconnect', $conn->Status);
         }
      }

      # Destroy the Connection
      print "destroying connection\n";
      unless (GMSECAPI::ConnectionFactory::Destroy($conn)) {
          Util::check('ConnectionFactory.Destroy',
                      GMSECAPI::ConnectionFactory::Status());
      }
   }
}


sub printUsage
{
   my ($self, $config) = @_;
   my $configFile = Util::getString($config, 'XML_FILE', '');

   if (length($configFile) == 0) {
      print "\n\nusage: perl gmthrpubsub.pl xml_file=<XML config file>\n\n";
      return 1;
   }
   return 0;
}


sub run
{
   my ($self, $config) = @_;

   my $XMLConfigFile = Util::getString($config, 'XML_FILE', 'gmthrpubsub.xml');
   my $config_name = Util::getString($config, 'CONFIG_NAME', 'config_bolt');

   my $pub_thread = threads->create(
        "publisher",
        create_config_from_file($XMLConfigFile, $config_name));

   $self->{PUBLISH_THREAD} = $pub_thread;

   # create subscribe connection
   my $connection = create_config_from_file($XMLConfigFile, $config_name);
   $self->{CONNECTION} = $connection; 

   # Connect
   unless ( $connection->Connect ) {
      Util::check('Connect', $connection->Status);
   }

   # Subscribe
   unless ( $connection->Subscribe('GMSEC.TEST.GMTHRPUBSUB')) {
      Util::check('Subscribe', $connection->Status);
   }

   # listen
   for (my $i = 0; $i < $nmsgs; $i++)
   {
      my $message = $connection->GetNextMsg($GMSEC_WAIT_FOREVER);

      if ($connection->Status->isError())
      {
         Util::check('GetNextMsg', $connection->Status);
      }

      # Dump the message
      print "received a message =>\n", $message->ToXML, "\n";

      # Destroy the message
      unless ($connection->DestroyMessage($message)) {
         Util::check('DestroyMessage', $connection->Status);
      }
   };

   return 1;
}

sub cleanup
{
   my ($self) = @_;

   my $connection = $self->{CONNECTION};
   my $publish_thread = $self->{PUBLISH_THREAD};

   delete($self->{CONNECTION});
   delete($self->{PUBLISH_THREAD});

   # Destroy the connection
   if ($connection)
   {
      print "\n===> start disconnecting main thread\n";
      # disconnect if necessary
      if ($connection->IsConnected)
      {
         print "disconnecting\n";
         unless ($connection->Disconnect) {
            Util::check('Disconnect', $connection->Status);
         }
      }

      # destroy the connection
      print "destroying connection\n";
      unless (GMSECAPI::ConnectionFactory::Destroy($connection)) {
          Util::check('ConnectionFactory.Destroy',
                      GMSECAPI::ConnectionFactory::Status());
      }
   }

   $publish_thread->detach;

   # join with publisher
   $publish_thread->join if ($publish_thread->is_joinable());

   print "===> exit out\n";
}


1;

__END__
