#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_Status
{
	my ($test) = @_;

	$test->setDescription("Test Status");

	test_constructor_1($test);
	test_constructor_2($test);
	test_exception_constructor($test);
	test_copy_constructor($test);
	test_get($test);
	test_set($test);
	test_set_class($test);
	test_set_code($test);
	test_set_reason($test);
	test_set_custom_code($test);
	test_reset($test);
}


sub test_constructor_1
{
	libgmsec_perl::logInfo("Test constructor (no args)");

	my ($test) = @_;

	my $reason = "";

   	my $s1 = libgmsec_perl::Status->new();

	$test->check("Unexpected error", $s1->hasError() == 0);
	$test->check("Unexpected classification", $s1->getClass() == $libgmsec_perl::NO_ERROR_CLASS);
	$test->check("Unexpected code", $s1->getCode() == $libgmsec_perl::NO_ERROR_CODE);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq $reason);
}


sub test_constructor_2
{
	libgmsec_perl::logInfo("Test constructor (2-4 args)");

	my ($test) = @_;

	my $reason = "Some worthy status";

	my $s1 = libgmsec_perl::Status->new(1, 2);

	$test->check("Unexpected error", $s1->hasError());
	$test->check("Unexpected classification", $s1->getClass() == 1);
	$test->check("Unexpected code", $s1->getCode() == 2);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq "");

	my $s2 = libgmsec_perl::Status->new(3, 4, $reason);

	$test->check("Unexpected error", $s2->hasError());
	$test->check("Unexpected classification", $s2->getClass() == 3);
	$test->check("Unexpected code", $s2->getCode() == 4);
	$test->check("Unexpected custom code", $s2->getCustomCode() == 0);
	$test->check("Unexpected reason", $s2->getReason() eq $reason);

	my $s3 = libgmsec_perl::Status->new(6, 7, $reason, 8);

	$test->check("Unexpected error", $s3->hasError());
	$test->check("Unexpected classification", $s3->getClass() == 6);
	$test->check("Unexpected code", $s3->getCode() == 7);
	$test->check("Unexpected custom code", $s3->getCustomCode() == 8);
	$test->check("Unexpected reason", $s3->getReason() eq $reason);
}


sub test_exception_constructor
{
	libgmsec_perl::logInfo("Test constructor (GmsecError arg)");

	my ($test) = @_;

   	my $reason = "Some worthy exception";

	my $e1 = libgmsec_perl::GmsecException->new(1, 2, 3, $reason);

	my $s1 = libgmsec_perl::Status->new($e1);

	$test->check("Unexpected error", $s1->hasError());
	$test->check("Unexpected classification", $s1->getClass() == 1);
	$test->check("Unexpected code", $s1->getCode() == 2);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 3);
	$test->check("Unexpected reason", $s1->getReason() eq $reason);
}


sub test_copy_constructor
{
	libgmsec_perl::logInfo("Test copy-constructor");

	my ($test) = @_;

	my $reason = "Some worthy exception";

	my $s1 = libgmsec_perl::Status->new(1, 2, $reason, 3);
	my $s2 = libgmsec_perl::Status->new($s1);

	$test->check("Unexpected error", $s2->hasError());
	$test->check("Unexpected classification", $s1->getClass() == $s2->getClass());
	$test->check("Unexpected code", $s1->getCode() == $s2->getCode());
	$test->check("Unexpected custom code", $s1->getCustomCode() == $s2->getCustomCode());
	$test->check("Unexpected reason", $s2->getReason() eq $reason);
}


sub test_get
{
	libgmsec_perl::logInfo("Test get()");

	my ($test) = @_;

	my $reason = "Some worthy exception";

	my $s1 = libgmsec_perl::Status->new(1, 2, $reason, 3);

	$test->check("Unexpected get() string", $s1->get() == "[1,2,3] : " . $reason);
}


sub test_set
{
	libgmsec_perl::logInfo("Test set()");

	my ($test) = @_;

	my $reason = "Some worthy exception";

	my $s1 = libgmsec_perl::Status->new();
	$s1->set(1, 2, $reason);
	$test->check("Unexpected error", $s1->hasError());
	$test->check("Unexpected classification", $s1->getClass() == 1);
	$test->check("Unexpected code", $s1->getCode() == 2);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq $reason);

	my $s2 = libgmsec_perl::Status->new(1, 2, "foobar", 6);
	$s2->set(1, 2, $reason, 3);
	$test->check("Unexpected error", $s2->hasError());
	$test->check("Unexpected classification", $s2->getClass() == 1);
	$test->check("Unexpected code", $s2->getCode() == 2);
	$test->check("Unexpected custom code", $s2->getCustomCode() == 3);
	$test->check("Unexpected reason", $s2->getReason() eq $reason);
}


sub test_set_class
{
	libgmsec_perl::logInfo("Test set_class()");

	my ($test) = @_;

	my $s1 = libgmsec_perl::Status->new();
	$s1->setClass(0);
	$test->check("Unexpected error", $s1->hasError() == 0);
	$test->check("Unexpected classification", $s1->getClass() == 0);
	$test->check("Unexpected code", $s1->getCode() == 0);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq "");

	my $s2 = libgmsec_perl::Status->new();
	$s2->setClass(1);
	$test->check("Unexpected error", $s2->hasError());
	$test->check("Unexpected classification", $s2->getClass() == 1);
	$test->check("Unexpected code", $s2->getCode() == 0);
	$test->check("Unexpected custom code", $s2->getCustomCode() == 0);
	$test->check("Unexpected reason", $s2->getReason() eq "");
}


sub test_set_code
{
	libgmsec_perl::logInfo("Test set_code()");

	my ($test) = @_;

	my $s1 = libgmsec_perl::Status->new();
	$s1->setCode(1);
	$test->check("Unexpected error", $s1->hasError() == 0);
	$test->check("Unexpected classification", $s1->getClass() == 0);
	$test->check("Unexpected code", $s1->getCode() == 1);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq "");
}


sub test_set_reason
{
	libgmsec_perl::logInfo("Test set_reason()");

	my ($test) = @_;

	my $reason = "Some worthy exception";

	my $s1 = libgmsec_perl::Status->new();
	$s1->setReason($reason);
	$test->check("Unexpected error", $s1->hasError() == 0);
	$test->check("Unexpected classification", $s1->getClass() == 0);
	$test->check("Unexpected code", $s1->getCode() == 0);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq $reason);
}


sub test_set_custom_code
{
	libgmsec_perl::logInfo("Test set_custom_code()");

	my ($test) = @_;

	my $s1 = libgmsec_perl::Status->new();
	$s1->setCustomCode(1);
	$test->check("Unexpected error", $s1->hasError() == 0);
	$test->check("Unexpected classification", $s1->getClass() == 0);
	$test->check("Unexpected code", $s1->getCode() == 0);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 1);
	$test->check("Unexpected reason", $s1->getReason() eq "");
}


sub test_reset
{
	libgmsec_perl::logInfo("Test reset()");

	my ($test) = @_;

	my $s1 = libgmsec_perl::Status->new(1, 2, "foobar", 3);
	$s1->reset();
	$test->check("Unexpected error", $s1->hasError() == 0);
	$test->check("Unexpected classification", $s1->getClass() == 0);
	$test->check("Unexpected code", $s1->getCode() == 0);
	$test->check("Unexpected custom code", $s1->getCustomCode() == 0);
	$test->check("Unexpected reason", $s1->getReason() eq "");
}


Test::driver(\&Test_Status);
