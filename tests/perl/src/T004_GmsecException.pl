#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_GmsecException
{
	my ($test) = @_;

	$test->setDescription("Test GmsecException");

	test_constructor_1($test);
	test_constructor_2($test);
	test_constructor_3($test);
	test_copy_constructor($test);
	# Note: Accessors are tested in each test above.
}


sub test_constructor_1
{
	libgmsec_perl::logInfo("Test constructor (3 args)");

	my ($test) = @_;

	my $emsg1 = "Something bad happened";

	my $e1 = libgmsec_perl::GmsecException->new(1, 2, $emsg1);

	$test->check("Unexpected error classification", $e1->getErrorClass() == 1);
	$test->check("Unexpected error code", $e1->getErrorCode() == 2);
	$test->check("Unexpected custom code", $e1->getCustomCode() == 0);
	$test->check("Unexpected error message", $e1->getErrorMessage() eq $emsg1);
	$test->check("Unexpected exception message", $e1->what() eq "[1,2,0] : $emsg1");
}


sub test_constructor_2
{
	libgmsec_perl::logInfo("Test constructor (4 args)");

	my ($test) = @_;

	my $emsg1 = "Something bad happened";

	my $e1 = libgmsec_perl::GmsecException->new(1, 2, 3, $emsg1);

	$test->check("Unexpected error classification", $e1->getErrorClass() == 1);
	$test->check("Unexpected error code", $e1->getErrorCode() == 2);
	$test->check("Unexpected custom code", $e1->getCustomCode() == 3);
	$test->check("Unexpected error message", $e1->getErrorMessage() eq $emsg1);
	$test->check("Unexpected exception message", $e1->what() eq "[1,2,3] : $emsg1");
}


sub test_constructor_3
{
	libgmsec_perl::logInfo("Test constructor (Status arg)");

	my ($test) = @_;

	my $reason = "Some worthy status";
	my $status = libgmsec_perl::Status->new(1, 2, $reason, 3);

	my $e1 = libgmsec_perl::GmsecException->new($status);

	$test->check("Unexpected error classification", $e1->getErrorClass() == 1);
	$test->check("Unexpected error code", $e1->getErrorCode() == 2);
	$test->check("Unexpected custom code", $e1->getCustomCode() == 3);
	$test->check("Unexpected error message", $e1->getErrorMessage() eq $reason);
	$test->check("Unexpected exception message", $e1->what() eq "[1,2,3] : $reason");
}


sub test_copy_constructor
{
	libgmsec_perl::logInfo("Test copy-constructor");

	my ($test) = @_;

	my $emsg = "Some worthy exception";

	my $e1 = libgmsec_perl::GmsecException->new(1, 2, 3, $emsg);
	my $e2 = libgmsec_perl::GmsecException->new($e1);

	$test->check("Unexpected error classification", $e1->getErrorClass() == $e2->getErrorClass());
	$test->check("Unexpected error code", $e1->getErrorCode() == $e2->getErrorCode());
	$test->check("Unexpected custom code", $e1->getCustomCode() == $e2->getCustomCode());
	$test->check("Unexpected error message", $e1->getErrorMessage() eq $e2->getErrorMessage());
	$test->check("Unexpected exception message", $e1->what eq $e2->what());
}


Test::driver(\&Test_GmsecException);
