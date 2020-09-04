%module Log
%{
#include <map>
#include <gmsec4/util/Log.h>
using namespace gmsec::api;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Log.h>

%inline %{
        void LogError(const char* entry)
        {
                GMSEC_ERROR << entry;
        }

        void LogSecure(const char* entry)
        {
                GMSEC_SECURE << entry;
        }

        void LogWarning(const char* entry)
        {
                GMSEC_WARNING << entry;
        }

        void LogInfo(const char* entry)
        {
                GMSEC_INFO << entry;
        }

        void LogVerbose(const char* entry)
        {
                GMSEC_VERBOSE << entry;
        }

        void LogDebug(const char* entry)
        {
                GMSEC_DEBUG << entry;
        }
%}

%perlcode %{
# Define the default LogHandler for Perl
{
        package DefaultLogHandler;
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
                #my $message = $timeString . " [" . libgmsec_perl::Log::toString($entry->swig_level_get) . "] " . "[" . $entry->swig_file_get . ":" . $entry->swig_line_get . "] " . $entry->swig_message_get . "\n";
                my $message = $timeString . " [" . libgmsec_perl::Log::toString($entry->swig_level_get) . "] " . "[" . $file . ":" . $line . "] " . $entry->swig_message_get . "\n";

                my $find = "\n";
                my $replace = "\n\t";
                $message =~ s/$find/$replace/g;

                $message .= "\n";

                print $message;
        }
}

my $defaultHandler = DefaultLogHandler->new();
$defaultHandler->DISOWN();

# Register the Default Handler for all logging levels
libgmsec_perl::Log::setReportingLevel($libgmsec_perl::logNONE);
libgmsec_perl::Log::registerHandler($defaultHandler);

=pod

=head1 NAME

libgmsec_perl::Log

=head1 DESCRIPTION

This class defines the logging interface.

=head2 Public Member Subroutines

=head3 setReportingLevel

C<libgmsec_perl::Log-E<gt>setReportingLevel($logLevel)>

        This function sets the maximum logging level.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG

=head3 getReportingLevel

C<libgmsec_perl::Log-E<gt>getReportingLevel()>

        This function returns the maximum logging level set by the library or the maximum logging level set by the user.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG

=head3 registerHandler

C<libgmsec_perl::Log-E<gt>registerHandler($logHandler)>

        This function registers a LogHandler that will be called for all logging levels.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logHandler - Pointer to an instance of a package whose base package is LogHandler

C<libgmsec_perl::Log-E<gt>registerHandler($logLevel, $logHandler)>

        This function registers a LogHandler that will be called for a specific logging level.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG
        $logHandler - Pointer to an instance of a package whose base package is LogHandler

=head3 toString

C<libgmsec_perl::Log-E<gt>toString($logLevel)>

        This function converts the log level number to a string value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A string value with possible values of, "NONE," "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or "INVALID"

=head3 fromString

C<libgmsec_perl::Log-E<gt>fromString($logLevelStr)>

        This function converts the string value to the log level number.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevelStr - One of the following string values "NONE," "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or "INVALID"

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG

=cut
%}
