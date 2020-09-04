#
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.
#

# 
# logging.pl
# 
# This file contains a simple example of using the perl GMSEC API logging framework
# 


use strict;
use Util;

use libgmsec_perl;

*isa = \&UNIVERSAL::isa;

{
	package BaseHandler;

        sub onMessageAux
        {
                my ($source, $entry) = @_;

                # Check to see if the logging macro was called from the API, or from the SWIG wrapper code
                my $isFromSwig = index($entry->swig_file_get, "libgmsec_perl_wrap") != -1;
                my $file = ($isFromSwig) ? (caller(1))[1] : $entry->swig_file_get;
                my $line = ($isFromSwig) ? (caller(1))[2] : $entry->swig_line_get;
                my $timeString = "YYYY-DDD-HH:MM:SS.sss"; # Instantiate a character buffer that is as long as the default time format
                libgmsec_perl::TimeUtil::formatTime($entry->swig_time_get, $timeString);

		my $fileName = (caller(1))[1];
		my $lineNumber = (caller(1))[2];

                my $message = "$source: "
				. $timeString . " [" . libgmsec_perl::Log::toString($entry->swig_level_get) . "] "
				. "[" . $fileName . ":" . $lineNumber . "] "
				. $entry->swig_message_get . "\n";

                my $find = "\n";
                my $replace = "\n\t";
                $message =~ s/$find/$replace/g;

                $message .= "\n";

                print $message;
        }

	# ErrorHandler -- BaseHandler subclass
	{
		package ErrorHandler;
		use base 'libgmsec_perl::LogHandler';

		sub onMessage
		{
			my ($self, $entry) = @_;
			BaseHandler::onMessageAux(__PACKAGE__, $entry);
		}
	} # ErrorHandler

	# WarningHandler -- BaseHandler subclass
	{
		package WarningHandler;
		use base 'libgmsec_perl::LogHandler';

		sub onMessage
		{
			my ($self, $entry) = @_;
			BaseHandler::onMessageAux(__PACKAGE__, $entry);
		}
	} # WarningHandler

	# InfoHandler -- BaseHandler subclass
	{
		package InfoHandler;
		use base 'libgmsec_perl::LogHandler';

		sub onMessage
		{
			my ($self, $entry) = @_;
			BaseHandler::onMessageAux(__PACKAGE__, $entry);
		}
	} # InfoHandler

	# VerboseHandler -- BaseHandler subclass
	{
		package VerboseHandler;
		use base 'libgmsec_perl::LogHandler';

		sub onMessage
		{
			my ($self, $entry) = @_;
			BaseHandler::onMessageAux(__PACKAGE__, $entry);
		}
	} # VerboseHandler

	# DebugHandler -- BaseHandler subclass
	{
		package DebugHandler;
		use base 'libgmsec_perl::LogHandler';

		sub onMessage
		{
			my ($self, $entry) = @_;
			BaseHandler::onMessageAux(__PACKAGE__, $entry);
		}
	} # DebugHandler

	# AnyHandler -- BaseHandler subclass
	{
		package AnyHandler;
		use base 'libgmsec_perl::LogHandler';

		sub onMessage
		{
			my ($self, $entry) = @_;
			BaseHandler::onMessageAux(__PACKAGE__, $entry);
		}
	} # AnyHandler
}

{
	my $self = bless({});

	eval
	{
		$self->run();
	};
	if ($@)
	{
		print "caught: $@";
	}
}


sub run
{
	my ($self) = @_;

	# Create and register handlers
	my $errorHandler = ErrorHandler->new();
	my $warningHandler = WarningHandler->new();
	my $infoHandler = InfoHandler->new();
	my $verboseHandler = VerboseHandler->new();
	my $debugHandler = DebugHandler->new();
	my $anyHandler = AnyHandler->new();

	$errorHandler->DISOWN();
	libgmsec_perl::Log::registerHandler($libgmsec_perl::logERROR, $errorHandler);
	$warningHandler->DISOWN();
	libgmsec_perl::Log::registerHandler($libgmsec_perl::logWARNING, $warningHandler);
	$infoHandler->DISOWN();
	libgmsec_perl::Log::registerHandler($libgmsec_perl::logINFO, $infoHandler);
	$verboseHandler->DISOWN();
	libgmsec_perl::Log::registerHandler($libgmsec_perl::logVERBOSE, $verboseHandler);
	$debugHandler->DISOWN();
	libgmsec_perl::Log::registerHandler($libgmsec_perl::logDEBUG, $debugHandler);

	# Output GMSEC API version
	libgmsec_perl::LogInfo(libgmsec_perl::Connection::getAPIVersion());

	# Set logging reporting level
	print "Setting log level to " . libgmsec_perl::Log::toString($libgmsec_perl::logVERBOSE) . "\n\n";

	libgmsec_perl::Log::setReportingLevel($libgmsec_perl::logVERBOSE);
	libgmsec_perl::LogInfo("The log reporting level is set to: " . libgmsec_perl::Log::getReportingLevel());

	libgmsec_perl::LogError("This is an example error message.");
	libgmsec_perl::LogWarning("This is an example warning message.");
	libgmsec_perl::LogVerbose("This is an example verbose message.");
	# The last message cannot be shown right now because the call
	# libgmsec_perl::Log::setReportingLevel(libgmsec_perl::logVERBOSE)
	# does not allow DEBUG messages to come out.
	libgmsec_perl::LogVerbose("This is an example debug message which should not show.");

	# Set logging reporting level to now allow DEBUG messages to be shown
	libgmsec_perl::Log::setReportingLevel($libgmsec_perl::logDEBUG);

	if (libgmsec_perl::Log::getReportingLevel() == $libgmsec_perl::logDEBUG)
	{
		libgmsec_perl::LogInfo("Changed reporting level to $libgmsec_perl::logDEBUG");
	}
	else
	{
		libgmsec_perl::LogError("Failed to change reporting level to $libgmsec_perl::logDEBUG");
	}
	libgmsec_perl::LogVerbose("This is an example debug message which should show.");

	libgmsec_perl::LogVerbose("NONE reporting level, numerically, is " . $libgmsec_perl::logNONE);
	libgmsec_perl::LogVerbose("ERROR reporting level, numerically, is " . $libgmsec_perl::logERROR);
	libgmsec_perl::LogVerbose("SECURE reporting level, numerically, is " . $libgmsec_perl::logSECURE);
	libgmsec_perl::LogVerbose("WARNING reporting level, numerically, is " . $libgmsec_perl::logWARNING);
	libgmsec_perl::LogVerbose("INFO reporting level, numerically, is " . $libgmsec_perl::logINFO);
	libgmsec_perl::LogVerbose("VERBOSE reporting level, numerically, is " . $libgmsec_perl::logVERBOSE);
	libgmsec_perl::LogVerbose("DEBUG reporting level, numerically, is " . $libgmsec_perl::logDEBUG);

	# Register general-purpose handler and test it
	$anyHandler->DISOWN();
	libgmsec_perl::Log::registerHandler($anyHandler);

	libgmsec_perl::LogError("NONE reporting level, numerically, is " . $libgmsec_perl::logNONE);
	libgmsec_perl::LogError("ERROR reporting level, numerically, is " . $libgmsec_perl::logERROR);
	libgmsec_perl::LogWarning("WARNING reporting level, numerically, is " . $libgmsec_perl::logWARNING);
	libgmsec_perl::LogInfo("INFO reporting level, numerically, is " . $libgmsec_perl::logINFO);
	libgmsec_perl::LogVerbose("VERBOSE reporting level, numerically, is " . $libgmsec_perl::logVERBOSE);
	libgmsec_perl::LogVerbose("DEBUG reporting level, numerically, is " . $libgmsec_perl::logDEBUG);
}


1;

__END__
