#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T001_Status extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor_1();
		this.testConstructor_2();
		this.testCopyConstructor();
		this.testGet();
		this.testSet();
		this.testSetClass();
		this.testSetCode();
		this.testSetReason();
		this.testSetCustomCode();
		this.testReset();
	}

	testConstructor_1()
	{
		gmsec.Log.info("Test constructor (no args)");

		var s1 = new gmsec.Status();
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == gmsec.NO_ERROR_CLASS);
		this.check("Unexpected code", s1.getCode() == gmsec.NO_ERROR_CODE);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "");
	}

	testConstructor_2()
	{
		gmsec.Log.info("Test constructor (2-4 args)");

		var s1 = new gmsec.Status(1, 2);
		this.check("Unexpected error", s1.hasError());
		this.check("Unexpected class", s1.getClass() == 1);
		this.check("Unexpected code", s1.getCode() == 2);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "");

		var s2 = new gmsec.Status(3, 4, "Some worthy status");
		this.check("Unexpected error", s2.hasError());
		this.check("Unexpected class", s2.getClass() == 3);
		this.check("Unexpected code", s2.getCode() == 4);
		this.check("Unexpected custom code", s2.getCustomCode() == 0);
		this.check("Unexpected reason", s2.getReason() == "Some worthy status");

		var s3 = new gmsec.Status(5, 6, "Some worthy status", 7);
		this.check("Unexpected error", s3.hasError());
		this.check("Unexpected class", s3.getClass() == 5);
		this.check("Unexpected code", s3.getCode() == 6);
		this.check("Unexpected custom code", s3.getCustomCode() == 7);
		this.check("Unexpected reason", s3.getReason() == "Some worthy status");
	}

	testCopyConstructor()
	{
		gmsec.Log.info("Test copy constructor");

		var s1 = new gmsec.Status(1, 2, "Some worthy status", 3);
		var s2 = new gmsec.Status(s1);

		this.check("Unexpected error", s1.hasError() == s2.hasError());
		this.check("Unexpected class", s1.getClass() == s2.getClass());
		this.check("Unexpected code", s1.getCode() == s2.getCode());
		this.check("Unexpected custom code", s1.getCustomCode() == s2.getCustomCode());
		this.check("Unexpected reason", s1.getReason() == s2.getReason());
	}

	testGet()
	{
		gmsec.Log.info("Test get()");

		var s1 = new gmsec.Status(1, 2, "Some worthy status", 3);

		this.check("Unexpected get string", s1.get() == "[1,2,3] : Some worthy status");
	}

	testSet()
	{
		gmsec.Log.info("Test set()");

		var s1 = new gmsec.Status();
		s1.set(0, 1);
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == 0);
		this.check("Unexpected code", s1.getCode() == 1);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "");

		var s2 = new gmsec.Status();
		s2.set(1, 2, "Some worthy status");
		this.check("Unexpected error", s2.hasError());
		this.check("Unexpected class", s2.getClass() == 1);
		this.check("Unexpected code", s2.getCode() == 2);
		this.check("Unexpected custom code", s2.getCustomCode() == 0);
		this.check("Unexpected reason", s2.getReason() == "Some worthy status");

		var s3 = new gmsec.Status(1, 2, "foobar", 6);
		s3.set(1, 2, "Some worthy status", 3);
		this.check("Unexpected error", s3.hasError());
		this.check("Unexpected class", s3.getClass() == 1);
		this.check("Unexpected code", s3.getCode() == 2);
		this.check("Unexpected custom code", s3.getCustomCode() == 3);
		this.check("Unexpected reason", s3.getReason() == "Some worthy status");
	}

	testSetClass()
	{
		gmsec.Log.info("Test setClass()");

		var s1 = new gmsec.Status();
		s1.setClass(0);
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == 0);
		this.check("Unexpected code", s1.getCode() == 0);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "");

		var s2 = new gmsec.Status();
		s2.setClass(1);
		this.check("Unexpected error", s2.hasError());
		this.check("Unexpected class", s2.getClass() == 1);
		this.check("Unexpected code", s2.getCode() == 0);
		this.check("Unexpected custom code", s2.getCustomCode() == 0);
		this.check("Unexpected reason", s2.getReason() == "");
	}

	testSetCode()
	{
		gmsec.Log.info("Test setCode()");

		var s1 = new gmsec.Status();
		s1.setCode(1);
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == 0);
		this.check("Unexpected code", s1.getCode() == 1);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "");
	}

	testSetReason()
	{
		gmsec.Log.info("Test setReason()");

		var s1 = new gmsec.Status();
		s1.setReason("Some worthy status");
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == 0);
		this.check("Unexpected code", s1.getCode() == 0);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "Some worthy status");
	}

	testSetCustomCode()
	{
		gmsec.Log.info("Test setCustomCode()");

		var s1 = new gmsec.Status();
		s1.setCustomCode(1);
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == 0);
		this.check("Unexpected code", s1.getCode() == 0);
		this.check("Unexpected custom code", s1.getCustomCode() == 1);
		this.check("Unexpected reason", s1.getReason() == "");
	}

	testReset()
	{
		gmsec.Log.info("Test reset()");

		var s1 = new gmsec.Status(1, 2, "foobar", 3);
		s1.reset();
		this.check("Unexpected error", s1.hasError() == false);
		this.check("Unexpected class", s1.getClass() == 0);
		this.check("Unexpected code", s1.getCode() == 0);
		this.check("Unexpected custom code", s1.getCustomCode() == 0);
		this.check("Unexpected reason", s1.getReason() == "");
	}
}

test = new T001_Status();
test.setDescription("Test Status");
test.run(process.argv);
