
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








# gmpub.pl
#
# A perl example demonstration of GMSEC publisher functionality.


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

   print "\n\nusage: perl gmpub.pl connectiontype=<middleware> ";
   print "[ <parameter>=<value> ]";
   print "\n\n\tNote that the parameter 'connectiontype' is required.";
   print "\n\tThe rest of other parameters are optional.";
   print "\n\n\tserver=<server name>";
   print "(required if middleware is not bolt/MB locally)";
   print "\n\tsubject=<subject name>";
   print "\n\titerations=<iterations>";
   print "\n\tinterval_ms=<interval (milliseconds)>";
   print "\n\tcfgfile=<config_filename.xml>";
   print "\n\nFor more information, see API User's Guide\n\n";

   return 1;
}


sub run
{
   my ($self, $config) = @_;

   my $subject = Util::getString($config, 'SUBJECT', 'GMSEC.TEST.PUBLISH');
   my $iterations = Util::getInteger($config, 'ITERATIONS', 1);
   my $interval_ms = Util::getInteger($config, 'INTERVAL_MS', 1000);

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

   for (my $i = 0; $i < $iterations; ++$i)
   {
      $self->publish($config, $subject, $i);
      if ($i < $iterations -1)
      {
         Util::millisleep($interval_ms);
      }
   }

}


sub publish ($$)
{
   my ($self, $config, $subject, $iteration) = @_;

   my $connection = $self->{CONNECTION};

   my $value = $iteration + 1;

   my $message = $connection->CreateMessage($subject, $GMSEC_MSG_PUBLISH);
   Util::check('CreateMessage', $connection->Status);

   $self->{MESSAGE} = $message;

   my $status;

   # Add Fields
   my $field = GMSECAPI::Field->new;

   $field->SetType($GMSEC_TYPE_CHAR);
   $field->SetName('C');
   $field->SetValue('c');
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_CHAR>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_BOOL);
   $field->SetName('T');
   $field->SetValue($GMSEC_TRUE);
   $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_TRUE>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_BOOL);
   $field->SetName('F');
   $field->SetValue($GMSEC_FALSE);
   $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_FALSE>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_I16);
   $field->SetName('J');
   $field->SetValue($value);
   $status = $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_I16>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_U16);
   $field->SetName('K');
   $field->SetValue($value);
   $status = $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_U16>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_I32);
   $field->SetName('I');
   $field->SetValue($value);
   $status = $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_I32>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_I32);
   $field->SetName("COUNT");
   $field->SetValue($iteration);
   $status = $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_I32>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_U32);
   $field->SetName('U');
   $field->SetValue($value);
   $status = $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_U32>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_STRING);
   $field->SetName('S');
   $field->SetValue("This is a test");
   $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_STRING>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_F32);
   $field->SetName('E');
   $field->SetValue(1 + 1. / $value);
   $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_F32>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_F64);
   $field->SetName('D');
   $field->SetValue(1 + 1. / $value);
   $message->AddField($field);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_F64>', $message->Status);
   }

   $field->SetType($GMSEC_TYPE_BIN);
   $field->SetName('X');
   $field->SetValue("JLMNOPQ", 7);
   unless ($message->AddField($field)) {
      Util::check('AddField<GMSEC_BIN>', $message->Status);
   }

   my $config_encrypt_value = $config->GetValue("ENCRYPT");
 
   if ($config_encrypt_value eq 'true')
   {
       $field->SetType($GMSEC_TYPE_BOOL);
       $field->SetName('SEC-ENCRYPT');
       $field->SetValue('true');
       unless ($message->AddField($field)) {
           Util::check("AddField(SEC-ENCRYPT)", $message->Status);
       }
   }
 
   # Dump Message
   print "published =>\n", $message->ToXML, "\n";

   # Publish Message
   unless ($connection->Publish($message)) {
      Util::check('Publish', $connection->Status);
   }

   print "Published!\n";

   # Destroy the Message
   if ($connection->DestroyMessage($message)) 
   {
      delete($self->{MESSAGE});
   }
   else 
   {
      Util::check('DestroyMessage', $connection->Status);
   }

   return 1;
}


sub cleanup
{
   my ($self) = @_;

   my $message = $self->{MESSAGE};
   my $connection = $self->{CONNECTION};

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
