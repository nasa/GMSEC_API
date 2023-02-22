#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


# Note: This module is named T004_GmsecException to ensure conformity with
#       unit tests from other language bindings.
#
#       The tests below verify GmsecError.
#


class Test_GmsecError(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_constructor_3()
        self.test_copy_constructor()
        # Note: Accessors are tested in each test above.


    def test_constructor_1(self):
        lp.log_info("Test constructor (3 args)")

        emsg1 = "Something bad happened"

        e1 = lp.GmsecError(1, 2, emsg1)

        self.check("Unexpected error classification", e1.get_error_class() == 1)
        self.check("Unexpected error code", e1.get_error_code() == 2)
        self.check("Unexpected custom code", e1.get_custom_code() == 0)
        self.check("Unexpected error message", e1.get_error_message() == emsg1)
        self.check("Unexpected exception message", str(e1) == "[1,2,0] : " + emsg1)


    def test_constructor_2(self):
        lp.log_info("Test constructor (4 args)")

        emsg1 = "Something bad happened"

        e1 = lp.GmsecError(1, 2, 3, emsg1)

        self.check("Unexpected error classification", e1.get_error_class() == 1)
        self.check("Unexpected error code", e1.get_error_code() == 2)
        self.check("Unexpected custom code", e1.get_custom_code() == 3)
        self.check("Unexpected error message", e1.get_error_message() == emsg1)
        self.check("Unexpected exception message", str(e1) == "[1,2,3] : " + emsg1)


    def test_constructor_3(self):
        lp.log_info("Test constructor (Status arg)")

        reason = "Some worthy status"
        status = lp.Status(1, 2, reason, 3)

        e1 = lp.GmsecError(status)

        self.check("Unexpected error classification", e1.get_error_class() == 1)
        self.check("Unexpected error code", e1.get_error_code() == 2)
        self.check("Unexpected custom code", e1.get_custom_code() == 3)
        self.check("Unexpected error message", e1.get_error_message() == reason)
        self.check("Unexpected exception message", str(e1) == "[1,2,3] : " + reason)


    def test_copy_constructor(self):
        lp.log_info("Test copy-constructor")

        emsg = "Some worthy exception"

        e1 = lp.GmsecError(1, 2, 3, emsg)
        e2 = lp.GmsecError(e1)

        self.check("Unexpected error classification", e1.get_error_class() == e2.get_error_class())
        self.check("Unexpected error code", e1.get_error_code() == e2.get_error_code())
        self.check("Unexpected custom code", e1.get_custom_code() == e2.get_custom_code())
        self.check("Unexpected error message", e1.get_error_message() == e2.get_error_message())
        self.check("Unexpected exception message", str(e1) == str(e2))


Test.run('Test GmsecError', Test_GmsecError())
