%module(directors="1") LogHandler
%{
#include <gmsec4/util/LogHandler.h>
using namespace gmsec::api;
%}

// Turn on director mapping for the Callback class
%feature("director") LogHandler;

%apply SWIGTYPE *DISOWN { LogHandler *cb };

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/LogHandler.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::LogHandler

=head1 DESCRIPTION

This class is the abstract base class for the logger handler callbacks.

Example LogHandler package:

        {
                package MyLogHandler;
                use base 'libgmsec_perl::LogHandler';

                sub onMessage
                {
                        my ($self, $entry) = @_;

                        # Check to see if the logging macro was called from the API, or from the SWIG wrapper code
                        my $isFromSwig = index($entry->swig_file_get, "libgmsec_perl_wrap") != -1;
                        my $file = ($isFromSwig) ? (caller(1))[1] : $entry->swig_file_get;
                        my $line = ($isFromSwig) ? (caller(1))[2] : $entry->swig_line_get;
                        my $timeString = "YYYY-DDD-HH:MM:SS.sss"; # Instantiate a character buffer that is as long as the default time format
                        libgmsec_perl::TimeUtil::formatTime($entry->swig_time_get, $timeString);

                        my $fileName = (caller(1))[1];
                        my $lineNumber = (caller(1))[2];

                        my $message = $timeString . " [" . libgmsec_perl::Log::toString($entry->swig_level_get) . "] "
                                        . "[" . $fileName . ":" . $lineNumber . "] "
                                        . $entry->swig_message_get . "\n";

                        my $find = "\n";
                        my $replace = "\n\t";
                        $message =~ s/$find/$replace/g;

                        $message .= "\n";

                        print $message;
                }
        }

Example LogHandler registration:

        my $logHandler = MyLogHandler->new();
        $logHandler->DISOWN();
        libgmsec_perl::Log::registerHandler($libgmsec_perl::logINFO, $logHandler); # Register handler for INFO logging level
        libgmsec_perl::Log::registerHandler($logHandler); # Register handler for all logging levels

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::LogHandler-E<gt>DESTROY()>

        Destructor.

=head3 onMessage

C<libgmsec_perl::LogHandler-E<gt>onMessage($entry)>

        This function is called by the API in response to a log message. A class derrived from libgmsec_perl::LogHandler needs to be registered with Log::registerHandler() in order to be called for a particular logging level.

        Because this class is user defined, it is not considered thread-safe unless made so by the user. The use of a libgmsec_perl::Mutex is suggested to encapsulate any sensitive code blocks.


=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $entry - the received log entry

=cut
%}
