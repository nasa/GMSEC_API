
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








##############################
## Callback Class for Log Comments ###
##############################
package RecorderCallback;


use strict;
use GMSECAPI;

sub new ($) { return bless({ }, shift); }

sub OnMessage ($$$) {
    my ($self, $conn, $msg) = @_;

    if (ref($msg) eq 'GMSECAPI::Message') 
    {
       print $msg->ToXML(), "\n";
    }
}


1;
