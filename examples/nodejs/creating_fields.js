#!/usr/bin/env node


/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * creating_fields.js
 *
 * This example program merely demonstrates how to create field objects.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class creating_fields
{
	run()
	{
		try
		{
			// Set up configuration options (if any) provided by the user
			const config = utility.createConfig(process.argv, false);

			// Initialize log level for output
			utility.initializeLogLevel(config);

			// Demonstrate how to create/use fields
			this.createBinaryField();
			this.createBooleanField();
			this.createCharField();
			this.createF32Field();
			this.createF64Field();
			this.createI16Field();
			this.createI32Field();
			this.createI64Field();
			this.createI8Field();
			this.createStringField();
			this.createU16Field();
			this.createU32Field();
			this.createU64Field();
			this.createU8Field();
		}
		catch (e)
		{
			gmsec.Log.error("Exception: " + e.message);
		}
	}

	createBinaryField()
	{
		// Define the blob data
		const blob = new gmsec.BlobArray();
		for (let i = 0; i < 4; ++i) {
			blob.add(i);
		}

		// Create the field
		const field = new gmsec.BinaryField("BINARY-FIELD", blob);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		const data = field.getValue();

		for (let i = 0; i < data.size(); ++i) {
			if (data.get(i) != blob.get(i)) {
				gmsec.Log.error("Invalid data at position: " + i.toString());
			}
		}
	}

	createBooleanField()
	{
		// Create the field
		const field = new gmsec.BooleanField("BOOLEAN-FIELD", true);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (!field.getValue()) {
			gmsec.Log.error("Invalid BooleanField value");
		}
	}

	createCharField()
	{
		// Create the field
		const field = new gmsec.CharField("CHAR-FIELD", 'c');

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 'c') {
			gmsec.Log.error("Invalid CharField value");
		}
	}

	createF32Field()
	{
		// Create the field
		const field = new gmsec.F32Field("F32-FIELD", 1.234);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data (note due to imprecision, Math.round() is used)
		if (Math.round(field.getValue()) != Math.round(1.234)) {
			gmsec.Log.error("Invalid F32Field value");
		}
	}

	createF64Field()
	{
		// Create the field
		const field = new gmsec.F64Field("F64-FIELD", 9.234);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 9.234) {
			gmsec.Log.error("Invalid F64Field value");
		}
	}

	createI16Field()
	{
		// Create the field
		const field = new gmsec.I16Field("I16-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid I16Field value");
		}
	}

	createI32Field()
	{
		// Create the field
		const field = new gmsec.I32Field("I32-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid I32Field value");
		}
	}

	createI64Field()
	{
		// Create the field
		const field = new gmsec.I64Field("I64-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid I64Field value");
		}
	}

	createI8Field()
	{
		// Create the field
		const field = new gmsec.I8Field("I8-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid I8Field value");
		}
	}

	createStringField()
	{
		// Create the field
		const field = new gmsec.StringField("STRING-FIELD", "GMSEC Rocks!");

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != "GMSEC Rocks!") {
			gmsec.Log.error("Invalid StringField value");
		}
	}

	createU16Field()
	{
		// Create the field
		const field = new gmsec.U16Field("U16-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid U16Field value");
		}
	}

	createU32Field()
	{
		// Create the field
		const field = new gmsec.U32Field("U32-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid U32Field value");
		}
	}

	createU64Field()
	{
		// Create the field
		const field = new gmsec.U64Field("U64-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid U64Field value");
		}
	}

	createU8Field()
	{
		// Create the field
		const field = new gmsec.U8Field("U8-FIELD", 5);

		// Display the field to the console
		gmsec.Log.info("Field: " + field.toXML());

		// Get/validate field data
		if (field.getValue() != 5) {
			gmsec.Log.error("Invalid U8Field value");
		}
	}
}


// Instantiate the creating_fields example, and run it.
const example = new creating_fields();
example.run();
