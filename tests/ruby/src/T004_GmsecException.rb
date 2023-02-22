#!/usr/bin/env ruby

require 'libgmsec_ruby'

load 'Test.rb'


class Test_GmsecException < Test

	def initialize(args)
		super(args)
	end


    def test()
        test_constructor_1
        test_constructor_2
        test_constructor_3
        test_copy_constructor
        # Note: Accessors are tested in each test above.
	end


    def test_constructor_1()
        Libgmsec_ruby::Log::info("Test constructor (3 args)")

        emsg1 = "Something bad happened"

        e1 = Libgmsec_ruby::GmsecException.new(1, 2, emsg1)

        check("Unexpected error classification", e1.get_error_class() == 1)
        check("Unexpected error code", e1.get_error_code() == 2)
        check("Unexpected custom code", e1.get_custom_code() == 0)
        check("Unexpected error message", e1.get_error_message() == emsg1)
        check("Unexpected exception message", e1.message == "[1,2,0] : " + emsg1)
	end


    def test_constructor_2()
        Libgmsec_ruby::Log::info("Test constructor (4 args)")

        emsg1 = "Something bad happened"

        e1 = Libgmsec_ruby::GmsecException.new(1, 2, 3, emsg1)

        check("Unexpected error classification", e1.get_error_class() == 1)
        check("Unexpected error code", e1.get_error_code() == 2)
        check("Unexpected custom code", e1.get_custom_code() == 3)
        check("Unexpected error message", e1.get_error_message() == emsg1)
        check("Unexpected exception message", e1.message == "[1,2,3] : " + emsg1)
	end


    def test_constructor_3()
        Libgmsec_ruby::Log::info("Test constructor (Status arg)")

        reason = "Some worthy status"
        status = Libgmsec_ruby::Status.new(1, 2, reason, 3)

        e1 = Libgmsec_ruby::GmsecException.new(status)

        check("Unexpected error classification", e1.get_error_class() == 1)
        check("Unexpected error code", e1.get_error_code() == 2)
        check("Unexpected custom code", e1.get_custom_code() == 3)
        check("Unexpected error message", e1.get_error_message() == reason)
        check("Unexpected exception message", e1.message == "[1,2,3] : " + reason)
	end


    def test_copy_constructor()
        Libgmsec_ruby::Log::info("Test copy-constructor")

        emsg = "Some worthy exception"

        e1 = Libgmsec_ruby::GmsecException.new(1, 2, 3, emsg)
        e2 = Libgmsec_ruby::GmsecException.new(e1)

        check("Unexpected error classification", e1.get_error_class() == e2.get_error_class())
        check("Unexpected error code", e1.get_error_code() == e2.get_error_code())
        check("Unexpected custom code", e1.get_custom_code() == e2.get_custom_code())
        check("Unexpected error message", e1.get_error_message() == e2.get_error_message())
        check("Unexpected exception message", e1.message == e2.message)
	end
end


begin
    test = Test_GmsecException.new( ARGV )
    test.run
end
