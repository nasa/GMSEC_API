#!/usr/bin/env python3

import libgmsec_python3 as lp
from Test import Test


class Test_Status(Test):

    def test(self):
        self.test_constructor_1()
        self.test_constructor_2()
        self.test_exception_constructor()
        self.test_copy_constructor()
        self.test_get()
        self.test_set()
        self.test_set_class()
        self.test_set_code()
        self.test_set_reason()
        self.test_set_custom_code()
        self.test_reset()


    def test_constructor_1(self):
        lp.log_info("Test constructor (no args)")

        reason1 = ""

        s1 = lp.Status()

        self.check("Unexpected error", s1.has_error() == False)
        self.check("Unexpected classification", s1.get_class() == lp.NO_ERROR_CLASS)
        self.check("Unexpected code", s1.get_code() == lp.NO_ERROR_CODE)
        self.check("Unexpected custom code", 0 == s1.get_custom_code())
        self.check("Unexpected reason", s1.get_reason() == reason1)


    def test_constructor_2(self):
        lp.log_info("Test constructor (2-4 args)")

        reason = "Some worthy status"

        s1 = lp.Status(1, 2)

        self.check("Unexpected error", s1.has_error())
        self.check("Unexpected classification", s1.get_class() == 1)
        self.check("Unexpected code", s1.get_code() == 2)
        self.check("Unexpected custom code", s1.get_custom_code() == 0)
        self.check("Unexpected reason", s1.get_reason() == "")

        s2 = lp.Status(3, 4, reason)

        self.check("Unexpected error", s2.has_error())
        self.check("Unexpected classification", s2.get_class() == 3)
        self.check("Unexpected code", s2.get_code() == 4)
        self.check("Unexpected custom code", s2.get_custom_code() == 0)
        self.check("Unexpected reason", s2.get_reason() == reason)

        s3 = lp.Status(6, 7, reason, 8)

        self.check("Unexpected error", s3.has_error())
        self.check("Unexpected classification", s3.get_class() == 6)
        self.check("Unexpected code", s3.get_code() == 7)
        self.check("Unexpected custom code", s3.get_custom_code() == 8)
        self.check("Unexpected reason", s3.get_reason() == reason)


    def test_exception_constructor(self):
        lp.log_info("Test constructor (GmsecError arg)")

        reason = "Some worthy exception"

        e1 = lp.GmsecError(1, 2, 3, reason)

        s1 = lp.Status(e1)

        self.check("Unexpected error", s1.has_error())
        self.check("Unexpected classification", s1.get_class() == 1)
        self.check("Unexpected code", s1.get_code() == 2)
        self.check("Unexpected custom code", s1.get_custom_code() == 3)
        self.check("Unexpected reason", s1.get_reason() == reason)


    def test_copy_constructor(self):
        lp.log_info("Test copy-constructor")

        reason = "Some worthy exception"

        s1 = lp.Status(1, 2, reason, 3)
        s2 = lp.Status(s1)

        self.check("Unexpected error", s2.has_error())
        self.check("Unexpected classification", s1.get_class() == s2.get_class())
        self.check("Unexpected code", s1.get_code() == s2.get_code())
        self.check("Unexpected custom code", s1.get_custom_code() == s2.get_custom_code())
        self.check("Unexpected reason", s2.get_reason() == reason)


    def test_get(self):
        lp.log_info("Test get()")

        reason = "Some worthy exception"

        s1 = lp.Status(1, 2, reason, 3)

        self.check("Unexpected get() string", s1.get() == "[1,2,3] : " + reason)


    def test_set(self):
        lp.log_info("Test set()")

        reason = "Some worthy exception"

        s1 = lp.Status()
        s1.set(1, 2, reason)
        self.check("Unexpected error", s1.has_error())
        self.check("Unexpected classification", s1.get_class() == 1)
        self.check("Unexpected code", s1.get_code() == 2)
        self.check("Unexpected custom code", s1.get_custom_code() == 0)
        self.check("Unexpected reason", s1.get_reason() == reason)

        s2 = lp.Status(1, 2, "foobar", 6)
        s2.set(1, 2, reason, 3)
        self.check("Unexpected error", s2.has_error())
        self.check("Unexpected classification", s2.get_class() == 1)
        self.check("Unexpected code", s2.get_code() == 2)
        self.check("Unexpected custom code", s2.get_custom_code() == 3)
        self.check("Unexpected reason", s2.get_reason() == reason)


    def test_set_class(self):
        lp.log_info("Test set_class()")

        s1 = lp.Status()
        s1.set_class(0)
        self.check("Unexpected error", s1.has_error() == False)
        self.check("Unexpected classification", s1.get_class() == 0)
        self.check("Unexpected code", s1.get_code() == 0)
        self.check("Unexpected custom code", s1.get_custom_code() == 0)
        self.check("Unexpected reason", s1.get_reason() == "")

        s2 = lp.Status()
        s2.set_class(1)
        self.check("Unexpected error", s2.has_error())
        self.check("Unexpected classification", s2.get_class() == 1)
        self.check("Unexpected code", s2.get_code() == 0)
        self.check("Unexpected custom code", s2.get_custom_code() == 0)
        self.check("Unexpected reason", s2.get_reason() == "")


    def test_set_code(self):
        lp.log_info("Test set_code()")

        s1 = lp.Status()
        s1.set_code(1)
        self.check("Unexpected error", s1.has_error() == False)
        self.check("Unexpected classification", s1.get_class() == 0)
        self.check("Unexpected code", s1.get_code() == 1)
        self.check("Unexpected custom code", s1.get_custom_code() == 0)
        self.check("Unexpected reason", s1.get_reason() == "")


    def test_set_reason(self):
        lp.log_info("Test set_reason()")

        reason = "Some worthy exception"

        s1 = lp.Status()
        s1.set_reason(reason)
        self.check("Unexpected error", s1.has_error() == False)
        self.check("Unexpected classification", s1.get_class() == 0)
        self.check("Unexpected code", s1.get_code() == 0)
        self.check("Unexpected custom code", s1.get_custom_code() == 0)
        self.check("Unexpected reason", s1.get_reason() == reason)


    def test_set_custom_code(self):
        lp.log_info("Test set_custom_code()")

        s1 = lp.Status()
        s1.set_custom_code(1)
        self.check("Unexpected error", s1.has_error() == False)
        self.check("Unexpected classification", s1.get_class() == 0)
        self.check("Unexpected code", s1.get_code() == 0)
        self.check("Unexpected custom code", s1.get_custom_code() == 1)
        self.check("Unexpected reason", s1.get_reason() == "")


    def test_reset(self):
        lp.log_info("Test reset()")

        s1 = lp.Status(1, 2, "foobar", 3)
        s1.reset()
        self.check("Unexpected error", s1.has_error() == False)
        self.check("Unexpected classification", s1.get_class() == 0)
        self.check("Unexpected code", s1.get_code() == 0)
        self.check("Unexpected custom code", s1.get_custom_code() == 0)
        self.check("Unexpected reason", s1.get_reason() == "")


Test.run('Test Status', Test_Status())
