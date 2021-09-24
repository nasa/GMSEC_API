 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# ReplyCallback.pm
#
#
# A Perl example demonstration of GMSEC reply callback class.


package ReplyCallback;

use strict;

sub new () { 
   my $replyCallbackFlag = 0;
   return bless({
                 replyCallbackFlag => $replyCallbackFlag,
			    }, shift); 
}


sub OnMessage ($$$) {
   my ($self, $conn, $msg) = @_;

   print "[ReplyCallback::OnMessage] inside message callback\n";

   if (ref($msg) eq 'GMSECAPI::Message' ) 
   {
      print $msg->ToXML, "\n";
   }
   $self->{replyCallbackFlag} = 1;
}

sub checkCallbackFlag() {
   my $self = shift;
   return $self->{replyCallbackFlag};
}


1;
