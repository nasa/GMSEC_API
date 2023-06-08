#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T004_GmsecException extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.excuse("Does this class need to be tested?");
	}
}

test = new T004_GmsecException();
test.setDescription("Test GmsecException");
test.run(process.argv);
