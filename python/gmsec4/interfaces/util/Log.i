%module Log
%{
#include <map>
#include <gmsec4/util/Log.h>
using namespace gmsec::api;
%}

%include "dox/Log_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Log.h>

%inline %{
        void logError(const char* entry)
        {
                GMSEC_ERROR << entry;
        }

        void logSecure(const char* entry)
        {
                GMSEC_SECURE << entry;
        }

        void logWarning(const char* entry)
        {
                GMSEC_WARNING << entry;
        }

        void logInfo(const char* entry)
        {
                GMSEC_INFO << entry;
        }

        void logVerbose(const char* entry)
        {
                GMSEC_VERBOSE << entry;
        }

        void logDebug(const char* entry)
        {
                GMSEC_DEBUG << entry;
        }
%}

/*
// TODO: Replace this with a LogHandler for Python
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
*/

