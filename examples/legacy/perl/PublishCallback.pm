 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.







 
# PublishCallback.pm
#
#
# A Perl example publish callback class.


package PublishCallback;

use strict;


sub new ($) { return bless({ }, shift); }


sub OnMessage ($$$) 
{
   my ($self, $conn, $msg) = @_;

   print "[PublishCallback::OnMessage] inside message callback\n";

   if (ref($msg) eq 'GMSECAPI::Message') 
   {
      print $msg->ToXML, "\n";
   }
}


1;
