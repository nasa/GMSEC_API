#!/usr/bin/env node

const gmsec = require('../../../../bin/gmsec_nodejs.node');


process.on('message', (cfgXML) => {
	setTimeout(() => {
		try {
			var config = new gmsec.Config(cfgXML, gmsec.DataType_XML_DATA);
			//console.log("Responder config is:\n" + config.toXML());

			var conn = new gmsec.Connection(config);
			conn.connect();

			var info = conn.setupSubscription(config.getValue("req-subject"));
			//console.log("Subscribed to: " + info.getSubjectPattern());

			var reqMsg = conn.receive(15000);

			if (reqMsg == null) {
				console.log("Responder failed to receive a request message");
			}
			else {
				var repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
				repMsg.setSubject(config.getValue("rep-subject"));

				var iter = reqMsg.getFieldIterator(gmsec.MessageFieldIterator.Selector_HEADER_FIELDS);
				while (iter.hasNext()) {
					var field = iter.next();
					if (field.getName() === "MESSAGE-TYPE") continue;
					repMsg.addField(field);
				}

				repMsg.addField(new gmsec.StringField("COMPONENT", "RESPONDER"));
				if (reqMsg.hasField("REQUEST-ID")) {
					repMsg.addField(reqMsg.getField("REQUEST-ID"));
				}

				if (reqMsg.hasField("COMPONENT")) {
					repMsg.addField(new gmsec.StringField("DESTINATION-COMPONENT", reqMsg.getStringValue("COMPONENT"), true));
				}

				repMsg.addField(new gmsec.I16Field("RESPONSE-STATUS", 1));

				conn.reply(reqMsg, repMsg);
			}

			conn.disconnect();
		}
		catch (e) {
			console.log("Failed to run T010_Responder: " + e.message);
		}

		process.send("Done");
	}, 1);
});
