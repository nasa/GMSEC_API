#!/usr/bin/env node

const os = require('os');
const fs = require('fs');
const process = require('process');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class TestError extends Error
{
	constructor(s) {
		super(s);
		this.name = "TestError";
	}
}


class TestCase
{
	constructor()
	{
		this.nOk = 0;
		this.nBad = 0;
		this.directory = null;
		this.prefix = null;
		this.signal = null;
		this.reason = null; 
		this.exitReason = null;
		this.description = null;
		this.config = null;
		this.standardFields = null;
	}

	testCase()
	{
		this.check("this is not a complete test case", false);
	}

	run(argv)
	{
		this.initialize(argv);

		try {
			this.testCase();
		}
		catch (e) {
			this.check("exception: " + e, false);
		}
		finally {
			if (this.exitReason != null) {
				gmsec.Log.info("Forcing exit due to: " + exitReason);
			}

			this.finish();
		}
	}

	initialize(argv)
	{
		var cfgArgs = "";
		argv.forEach((arg) => {
			if (arg.charAt(0) == '-' && arg.startsWith("-signal=")) {
				this.signal = arg.substring(8);
			}
			else if (arg.includes("=")) {
				cfgArgs += (arg + " ");
			}
		});

		if (cfgArgs != "") {
			this.config = new gmsec.Config(cfgArgs, gmsec.DataType_KEY_VALUE_DATA);
		}
		else {
			this.config = new gmsec.Config();
		}
		this.config.addValue("loglevel", this.config.getValue("loglevel", "info"));
		this.config.addValue("logfile", this.config.getValue("logfile", "stderr"));

		var testNumber = this.constructor.name.substring(0, this.constructor.name.indexOf('_'));
		var sb = "NIGHTRUN_JAVASCRIPT_" + process.pid + "_" + os.hostname() + "_" + testNumber;
		this.prefix = sb.replace("-", "_").toUpperCase();

		this.directory = process.cwd();

		this.initStandardFields();
	}

	finish()
	{
		var result = "FAIL";
		if (this.nBad == 0 && this.nOk > 0) {
			result = "PASS";
		}
		if (this.reason != null) {
			result = "EXCUSE";
		}

		var summary = "RESULT=" + result +
		              "\nDESCRIPTION=" + this.getDescription() +
		              "\nN_OK=" + this.nOk +
		              "\nN_BAD=" + this.nBad +
		              "\nPREFIX=" + this.prefix;

		if (this.reason != null) {
			summary += "\nEXCUSE=" + this.reason;
		}

		gmsec.Log.info("Summary:\n" + summary);

		if (this.signal != null) {
			fs.writeFile(this.signal, summary, (err) => {
				if (err) throw err;
			});
		}
	}

	setDescription(desc)
	{
		this.description = desc;
	}

	getDescription()
	{
		return this.description;
	}

	getConfig()
	{
		return this.config;
	}

	initStandardFields()
	{
		this.standardFields = new gmsec.FieldArray();
		this.standardFields.add( new gmsec.StringField("MISSION-ID", "MY-MISSION", true) );
		this.standardFields.add( new gmsec.StringField("CONSTELLATION-ID", "MY-CNST-ID", true) );
		this.standardFields.add( new gmsec.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true) );
		this.standardFields.add( new gmsec.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true) );
		this.standardFields.add( new gmsec.StringField("DOMAIN1", "MY-DOMAIN-1", true) );
		this.standardFields.add( new gmsec.StringField("DOMAIN2", "MY-DOMAIN-2", true) );
		this.standardFields.add( new gmsec.StringField("FACILITY", "MY-FACILITY", true) );
		this.standardFields.add( new gmsec.StringField("COMPONENT", "JS-TESTCASE", true) );
	}

	getStandardFields()
	{
		// Initialize the standard fields each time this method is called.
		// The rationale: Previously created 'anonymous' fields could be garbage collected.
		this.initStandardFields();
		return this.standardFields;
	}

	setStandardFields(msgFactory)
	{
		// Initialize the standard fields each time this method is called.
		// The rationale: Previously created 'anonymous' fields could be garbage collected.
		this.initStandardFields();
		msgFactory.setStandardFields(this.standardFields);
	}

	verifyStandardFields(msg)
	{
		for (var i = 0; i < this.standardFields.size(); i++) {
			var field = this.standardFields.get(i);
			this.check("Message is missing standard field", msg.hasField(field.getName()));
			this.check("Message unexpected standard field type", msg.getField(field.getName()).getType() == field.getType());
			this.check("Message unexpected standard field value", msg.getField(field.getName()).getStringValue() === field.getStringValue());
			this.check("Message not a header field", msg.getField(field.getName()).isHeader() == true);
		}
	}

	getMW()
	{
		return this.getConfig().getValue("mw-id", "unknown");
	}

	getSubject(tag=null)
	{
		var subject;
		if (tag == null) {
			subject = this.prefix;
		}
		else {
			subject = this.prefix + "." + tag;
		}
		return subject.toUpperCase();
	}

	getUniqueComponent()
	{
		var component = this.prefix;
		return component.replace(".", "-");
	}

	getDataFile(name)
	{
		var dir = null;

		var addonDir = this.getConfig().getValue("addons");
		if (addonDir == null) {
			dir = this.getDirectory() + "/addons";
		}
		else {
			dir = addonDir;
		}

		return dir + "/" + name;
	}

	getDirectory()
	{
		return this.directory;
	}

	excuse(str)
	{
		this.reason = str;
	}

	comment(str)
	{
		gmsec.Log.info("[comment] " + str);
	}

	check(what, flag)
	{
		this._check(what, flag);
	}

	require(what, flag)
	{
		if (!this._check(what, flag)) {
			throw new TestError(what);
		}
	}

	_check(what, flag)
	{
		if (flag) {
			++this.nOk;
		}
		else {
			++this.nBad;
			gmsec.Log.warning("[error] " + what);
		}
		return flag;
	}
}

module.exports = TestError
module.exports = TestCase
