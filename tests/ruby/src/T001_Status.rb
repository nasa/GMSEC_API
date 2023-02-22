#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_Status < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_constructor_1
        test_constructor_2
        test_exception_constructor
        test_copy_constructor
        test_get
        test_set
        test_set_class
        test_set_code
        test_set_reason
        test_set_custom_code
        test_reset
	end


    def test_constructor_1()
        Libgmsec_ruby::Log::info("Test constructor (no args)")

        reason1 = ""

        s1 = Libgmsec_ruby::Status.new()

        check("Unexpected error", s1.has_error() == false)
        check("Unexpected classification", s1.get_class() == Libgmsec_ruby::NO_ERROR_CLASS)
        check("Unexpected code", s1.get_code() == Libgmsec_ruby::NO_ERROR_CODE)
        check("Unexpected custom code", 0 == s1.get_custom_code())
        check("Unexpected reason", s1.get_reason() == reason1)
	end


    def test_constructor_2()
        Libgmsec_ruby::Log::info("Test constructor (2-4 args)")

        reason = "Some worthy status"

        s1 = Libgmsec_ruby::Status.new(1, 2)

        check("Unexpected error", s1.has_error())
        check("Unexpected classification", s1.get_class() == 1)
        check("Unexpected code", s1.get_code() == 2)
        check("Unexpected custom code", s1.get_custom_code() == 0)
        check("Unexpected reason", s1.get_reason() == "")

        s2 = Libgmsec_ruby::Status.new(3, 4, reason)

        check("Unexpected error", s2.has_error())
        check("Unexpected classification", s2.get_class() == 3)
        check("Unexpected code", s2.get_code() == 4)
        check("Unexpected custom code", s2.get_custom_code() == 0)
        check("Unexpected reason", s2.get_reason() == reason)

        s3 = Libgmsec_ruby::Status.new(6, 7, reason, 8)

        check("Unexpected error", s3.has_error())
        check("Unexpected classification", s3.get_class() == 6)
        check("Unexpected code", s3.get_code() == 7)
        check("Unexpected custom code", s3.get_custom_code() == 8)
        check("Unexpected reason", s3.get_reason() == reason)
	end


    def test_exception_constructor()
        Libgmsec_ruby::Log::info("Test constructor (GmsecError arg)")

        reason = "Some worthy exception"

        e1 = Libgmsec_ruby::GmsecException.new(1, 2, 3, reason)

        s1 = Libgmsec_ruby::Status.new(e1)

        check("Unexpected error", s1.has_error())
        check("Unexpected classification", s1.get_class() == 1)
        check("Unexpected code", s1.get_code() == 2)
        check("Unexpected custom code", s1.get_custom_code() == 3)
        check("Unexpected reason", s1.get_reason() == reason)
	end


    def test_copy_constructor()
        Libgmsec_ruby::Log::info("Test copy-constructor")

        reason = "Some worthy exception"

        s1 = Libgmsec_ruby::Status.new(1, 2, reason, 3)
        s2 = Libgmsec_ruby::Status.new(s1)

        check("Unexpected error", s2.has_error())
        check("Unexpected classification", s1.get_class() == s2.get_class())
        check("Unexpected code", s1.get_code() == s2.get_code())
        check("Unexpected custom code", s1.get_custom_code() == s2.get_custom_code())
        check("Unexpected reason", s2.get_reason() == reason)
	end


    def test_get()
        Libgmsec_ruby::Log::info("Test get()")

        reason = "Some worthy exception"

        s1 = Libgmsec_ruby::Status.new(1, 2, reason, 3)

        check("Unexpected get() string", s1.get() == "[1,2,3] : " + reason)
	end


    def test_set()
        Libgmsec_ruby::Log::info("Test set()")

        reason = "Some worthy exception"

        s1 = Libgmsec_ruby::Status.new()
        s1.set(1, 2, reason)
        check("Unexpected error", s1.has_error())
        check("Unexpected classification", s1.get_class() == 1)
        check("Unexpected code", s1.get_code() == 2)
        check("Unexpected custom code", s1.get_custom_code() == 0)
        check("Unexpected reason", s1.get_reason() == reason)

        s2 = Libgmsec_ruby::Status.new(1, 2, "foobar", 6)
        s2.set(1, 2, reason, 3)
        check("Unexpected error", s2.has_error())
        check("Unexpected classification", s2.get_class() == 1)
        check("Unexpected code", s2.get_code() == 2)
        check("Unexpected custom code", s2.get_custom_code() == 3)
        check("Unexpected reason", s2.get_reason() == reason)
	end


    def test_set_class()
        Libgmsec_ruby::Log::info("Test set_class()")

        s1 = Libgmsec_ruby::Status.new()
        s1.set_class(0)
        check("Unexpected error", s1.has_error() == false)
        check("Unexpected classification", s1.get_class() == 0)
        check("Unexpected code", s1.get_code() == 0)
        check("Unexpected custom code", s1.get_custom_code() == 0)
        check("Unexpected reason", s1.get_reason() == "")

        s2 = Libgmsec_ruby::Status.new()
        s2.set_class(1)
        check("Unexpected error", s2.has_error())
        check("Unexpected classification", s2.get_class() == 1)
        check("Unexpected code", s2.get_code() == 0)
        check("Unexpected custom code", s2.get_custom_code() == 0)
        check("Unexpected reason", s2.get_reason() == "")
	end


    def test_set_code()
        Libgmsec_ruby::Log::info("Test set_code()")

        s1 = Libgmsec_ruby::Status.new()
        s1.set_code(1)
        check("Unexpected error", s1.has_error() == false)
        check("Unexpected classification", s1.get_class() == 0)
        check("Unexpected code", s1.get_code() == 1)
        check("Unexpected custom code", s1.get_custom_code() == 0)
        check("Unexpected reason", s1.get_reason() == "")
	end


    def test_set_reason()
        Libgmsec_ruby::Log::info("Test set_reason()")

        reason = "Some worthy exception"

        s1 = Libgmsec_ruby::Status.new()
        s1.set_reason(reason)
        check("Unexpected error", s1.has_error() == false)
        check("Unexpected classification", s1.get_class() == 0)
        check("Unexpected code", s1.get_code() == 0)
        check("Unexpected custom code", s1.get_custom_code() == 0)
        check("Unexpected reason", s1.get_reason() == reason)
	end


    def test_set_custom_code()
        Libgmsec_ruby::Log::info("Test set_custom_code()")

        s1 = Libgmsec_ruby::Status.new()
        s1.set_custom_code(1)
        check("Unexpected error", s1.has_error() == false)
        check("Unexpected classification", s1.get_class() == 0)
        check("Unexpected code", s1.get_code() == 0)
        check("Unexpected custom code", s1.get_custom_code() == 1)
        check("Unexpected reason", s1.get_reason() == "")
	end


    def test_reset()
        Libgmsec_ruby::Log::info("Test reset()")

        s1 = Libgmsec_ruby::Status.new(1, 2, "foobar", 3)
        s1.reset()
        check("Unexpected error", s1.has_error() == false)
        check("Unexpected classification", s1.get_class() == 0)
        check("Unexpected code", s1.get_code() == 0)
        check("Unexpected custom code", s1.get_custom_code() == 0)
        check("Unexpected reason", s1.get_reason() == "")
	end
end


begin
	test = Test_Status.new( ARGV )
	test.run
end
