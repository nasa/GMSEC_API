 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








##############################################################################
# Connection.pm
#
# Description: Base class for middleware connections made with the
#    ConnectionFactory class
#
##############################################################################

package GMSECAPI::Connection;

use strict;
use base qw(DynaLoader);
use GMSECAPI::Util;

use threads;
use threads::shared;

my $AUTO_DISPATCH_RUNNING :shared;

bootstrap GMSECAPI::Connection;


sub Status
{
    my ($self) = @_;

    return GMSECAPI::Util::GetStatus($self);
}



sub StartPerlDispatchThread
{
	my ($self) = @_;
	if (not $AUTO_DISPATCH_RUNNING) {
		my $thread = threads->create(sub {

			$self->SetThreadedDispatchContext;

			{
				lock($AUTO_DISPATCH_RUNNING);
				$AUTO_DISPATCH_RUNNING = 1;
				cond_broadcast($AUTO_DISPATCH_RUNNING);
			}

			# wait forever
			my $untouchable :shared;
			{
			 	lock($untouchable);
			 	cond_wait($untouchable);
			}
		});
		$thread->detach;
	}
	else {
		# perl AutoDispatch thread already running...
	}

	# ensure dispatch thread is running
	if (not $AUTO_DISPATCH_RUNNING) {
        	# if it is not set within 10 seconds, something is seriously wrong
                my $start = time();
                my $limit = $start + 10;
                lock($AUTO_DISPATCH_RUNNING);
                while (not $AUTO_DISPATCH_RUNNING) {
			last if not cond_timedwait($AUTO_DISPATCH_RUNNING, $limit);
                }
                if (not $AUTO_DISPATCH_RUNNING) {
                        die('the perl auto-dispatch thread was not successfully
started');
                }
        }

}

sub Connect
{
	my ($self) = @_;
	$self->StartPerlDispatchThread;
	$self->SetDispatchContext;
	return $self->ConnectXS;
}

sub StartAutoDispatch
{
	my ($self) = @_;
	$self->ChooseThreadedDispatchContext;
	return $self->StartAutoDispatchXS;
}

sub StopAutoDispatch
{
	my ($self) = @_;
	$self->SetDispatchContext;
	# could try to stop the perl thread if noone is using it...
	return $self->StopAutoDispatchXS;
}


1;

__END__


=head1 NAME

GMSECAPI::Connection - Connection class for the Perl GMSEC API

=head1 SYNOPSIS

    use GMSECAPI;

    my $cfg = GMSECAPI::Config->new([ @ARGV ]);

    my $conn = GMSECAPI::ConnectionFactory::Create($cfg);
    my $status = $conn->Status;
    $conn->Connect;

    $conn->Subscribe("gmsec.*.publish");
    my $msg = $conn->GetNextMsg($GMSEC_WAIT_FOREVER);
    $conn->DestroyMessage($msg);

    $conn->Disconnect;
    GMSECAPI::ConnectionFactory::Destroy($conn);

    my $msg1 = $conn->CreateMessage("gmsec.test1", $GMSEC_MSG_PUBLISH);
    my $msg2 = $conn->CreateMessage("gmsec.test2", $GMSEC_MSG_PUBLISH, $cfg);

=head1 DESCRIPTION

The C<GMSECAPI::Connection> class is the interface for all interactions with the GMSEC middleware connection.  It is created by the GMSECAPI::ConnectionFactory module.  Please view the documentation for the C++ version of the GMSEC API to read a functionality overview of all methods listed below.  All method names are the same and arguments match up in most cases.  The main difference is that the status is stored in the Perl API and is returned in the C++ API.

=head1 METHODS

=over 8

=item $status = $conn->Status

The method returns a GMSECAPI::Status object for the status of the last method call in the GMSECAPI::Connection class.

=item $conn->Connect

=item $conn->Disconnect

=item $libname = $conn->GetLibraryRootName

=item $version = $conn->GetLibraryVersion

=item $conn->Subscribe($subject)

=item $conn->Subscribe($subject, $callbackObject)

=item $conn->Subscribe($subject, $callbackObject, $objectPackage)

=item $conn->Subscribe($subject, $callbackObject, $objectPackage, $objectMethod)

Above are four ways of subscribing to a particular message subject.  The last three versions are used for registering callbacks.  The second version uses the GMSECAPI::Callback module as the default callback object package.  It simply prints out the received message in an XML format.  The third version allows you to specify your own callback package to use.  The OnMessage method will be called with by default.  The fourth version of Subscribe allows you to specify the name of the method you want called.

=item $conn->UnSubscribe($subject)

=item $msg = $conn->CreateMessage($subject, $msgKind)

=item $msg = $conn->CreateMessage($subject, $msgKind, $cfg)

=item $outMsg = $conn->ConvertMessage($inMsg)

=item $conn->ConvertCleanup($inMsg, $outMsg)

=item $outMsg = $conn->CloneMessage($inMsg)

=item $conn->DestroyMessage($msg)

=item $conn->Publish($msg)

=item $reply = $conn->Request($request, $timeout)

=item $conn->Request($request, $timeout, $callbackObject)

=item $conn->Request($request, $timeout, $callbackObject, $objectPackage)

=item $conn->Request($request, $timeout, $callbackObject, $objectPackage, $objectMethod)

Above are four ways of requesting a message with a particular subject.  The last three versions are used for registering callbacks.  The second version uses the GMSECAPI::Callback module as the default callback object package.  It simply prints out the received message in an XML format.  The third version allows you to specify your own callback package to use.  The OnMessage method will be called with by default.  The fourth version of Request allows you to specify the name of the method you want called.

=item $conn->Reply($request, $reply)

=item $conn->StartAutoDispatch()

=item $conn->StopAutoDispatch()

=item $msg = $conn->GetNextMsg($timeout)

=item $msg = $conn->DispatchMsg($msg)

=item $status = $conn->GetLastDispatcherStatus()

=back

=head1 AUTHOR

 Timothy Esposito <timothy.esposito@nasa.gov>
 NASA Goddard Space Flight Center

=head1 SEE ALSO

L<GMSECAPI>, L<GMSECAPI::Status>, L<GMSECAPI::Field>, L<GMSECAPI::Message>, L<GMSECAPI::Config>, L<GMSECAPI::ConnectionFactory>, L<GMSECAPI::Callback>

=cut
