/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Log

%{
#include <map>
#include <gmsec5/util/Log.h>
using namespace gmsec::api5;
%}

// Excluding these due to issues with threading in Perl with SWIG-based objects
%ignore registerHandler;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/Log.h>

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

%perlcode %{

=head1 NAME

libgmsec_perl::Log

=head1 DESCRIPTION

This class defines the logging interface.

=head2 Public Static Subroutines

=begin UNSUPPORTED

=head3 registerHandler

C<libgmsec_perl::Log::registerHandler($logHandler)>

        This function registers a LogHandler that will be called for all logging levels.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logHandler - instance of custom log handler whose base class is LogHandler.


=head3 registerHandler

C<libgmsec_perl::Log::registerHandler($logLevel, $logHandler)>

        This function registers a LogHandler that will be called for the specified logging level.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG
        $logHandler - instance of custom log handler whose base class is LogHandler.

=end UNSUPPORTED


=head3 setReportingLevel

C<libgmsec_perl::Log::setReportingLevel($logLevel)>

        This function sets the maximum logging level.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG


=head3 getReportingLevel

C<libgmsec_perl::Log::getReportingLevel()>

        This function returns the maximum logging level set by the library or the maximum logging level set by the user.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG


=head3 toString

C<libgmsec_perl::Log::toString($logLevel)>

        This function converts the log level number to a string value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevel - logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A string value with possible values of, "NONE," "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or "INVALID"


=head3 fromString

C<libgmsec_perl::Log::fromString($logLevelStr)>

        This function converts the string value to the log level number.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $logLevelStr - One of the following string values "NONE," "ERROR," "SECURE," "WARNING," "INFO," "VERBOSE," "DEBUG," or "INVALID"

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        logging level with possible values of $libgmsec_perl::logNONE, $libgmsec_perl::logERROR, $libgmsec_perl::logSECURE, $libgmsec_perl::logWARNING, $libgmsec_perl::logINFO, $libgmsec_perl::logVERBOSE, $libgmsec_perl::logDEBUG

=cut
%}
