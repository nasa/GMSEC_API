import gov.nasa.gsfc.gmsec.api5.*;
import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.ArrayList;
import java.util.List;

class T043_Callback extends Callback
{
	private TestCase m_test;
	private List<Message.ResponseStatus> m_statusList;

	public T043_Callback(TestCase test)
	{
		m_test       = test;
		m_statusList = new ArrayList<Message.ResponseStatus>();
		m_statusList.add(Message.ResponseStatus.ACKNOWLEDGEMENT);
		m_statusList.add(Message.ResponseStatus.WORKING_KEEP_ALIVE);
		m_statusList.add(Message.ResponseStatus.WORKING_KEEP_ALIVE);
		m_statusList.add(Message.ResponseStatus.WORKING_KEEP_ALIVE);
		m_statusList.add(Message.ResponseStatus.SUCCESSFUL_COMPLETION);
	}

	@Override
	public void onMessage(Connection conn, Message message)
	{
		try
		{
			Message reply = conn.getMessageFactory().createMessage("RESP.DIR");
			reply.setSubject( m_test.getSubject("RESP.DIR") );

			reply.addField("COMPONENT", (message.hasField("DESTINATION-COMPONENT") ? message.getStringValue("DESTINATION-COMPONENT") : "RESPONDER"));
			reply.addField("DESTINATION-COMPONENT", (message.hasField("COMPONENT") ? message.getStringValue("COMPONENT") : "FILL"));

			if (message.hasField("REQUEST-ID"))
			{
				reply.addField(message.getField("REQUEST-ID"));
			}

			Log.verbose("Received Request:\n" + message.toXML());

			for (Message.ResponseStatus status : m_statusList)
			{
				reply.addField("RESPONSE-STATUS", (short) status.getValue());

				Log.verbose("Issuing Reply:\n" + reply.toXML());

				conn.reply(message, reply);
			}
		}
		catch (GmsecException e)
		{
			Log.error("GmsecException in T043_Callback: " + e.getMessage());
		}
	}

	public int expectedResponses()
	{
		return m_statusList.size();
	}
}


class T043_ReplyCallback extends ReplyCallback
{
	private int repliesReceived;

	public T043_ReplyCallback()
	{
		repliesReceived = 0;
	}

	@Override
	public void onReply(Connection conn, Message request, Message reply)
	{
		try
		{
			Log.info("Received Reply with RESPONSE-STATUS: " + reply.getI16Value("RESPONSE-STATUS"));
			++repliesReceived;
		}
		catch (GmsecException e)
		{
			Log.error("GmsecException in T043_ReplyCallback: " + e.getMessage());
		}
	}

	@Override
	public void onEvent(Connection conn, Status status, Connection.Event connEvent)
	{
		Log.warning("Received unexpected event: " + status.get());
	}

	public int numRepliesReceived()
	{
		return repliesReceived;
	}
}


public class T043_MultiResponse extends TestCase
{
	public static void main(String[] args)
	{
		T043_MultiResponse test = new T043_MultiResponse();
		test.setDescription("Multi-Response");
		test.run(args);
	}

	@Override
	public void testCase() throws Exception
	{
		Config config = new Config(getConfig());  // make a copy
		config.addValue("mw-multi-resp", "true");

		T043_Callback       cb = new T043_Callback(this);
		T043_ReplyCallback rcb = new T043_ReplyCallback();

		Connection conn = null;

		try
		{
			conn = Connection.create(config);

			conn.connect();

			setStandardFields(conn.getMessageFactory());

			conn.startAutoDispatch();

			Message request = conn.getMessageFactory().createMessage("REQ.DIR");
			request.setSubject( getSubject("REQ.DIR") );
			request.addField("DIRECTIVE-STRING", "Do something!");
			request.addField("DESTINATION-COMPONENT", "RESPONDER");
			request.addField("REQUEST-ID", new U16(0));

			// subscribe to receive the response message (hack!)
			conn.subscribe( getSubject("RESP.DIR.+") );

			// subscribe to process the request message
			conn.subscribe(request.getSubject(), cb);

			Log.verbose("Issuing Request:\n" + request.toXML());

			conn.request(request, 5000, rcb, Gmsec.REQUEST_REPUBLISH_NEVER);

			TimeUtil.millisleep(5000);   // allow for the response message(s) to be received

			conn.disconnect();

			check("Unexpected response message count", rcb.numRepliesReceived() == cb.expectedResponses());
		}
		catch (GmsecException e)
		{
			require(e.getMessage(), false);
		}

		if (conn != null)
		{
			Connection.destroy(conn);
			conn = null;
		}
	}
}
