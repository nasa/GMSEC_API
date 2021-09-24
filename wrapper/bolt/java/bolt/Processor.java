
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package bolt;

import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Logger;

interface Processor
{
	void enqueue(Message message);
}

class FakeProcessor
		implements Processor
{
	private static final Logger source = Log.createSource("FAKE_PROCESSOR");

	public void enqueue(Message message) {
		// no op
		message.dump("FakeProcessor.enqueue:", System.out);
	}
}

class RealProcessor
		extends Thread
		implements Processor
{
	private static final Logger source = Log.createSource("Processor");
	// private List<Message> queue = Collections.synchronizedList(new
	// LinkedList<Message>());
	private LinkedBlockingQueue<Message> queue = new LinkedBlockingQueue<Message>(Options.DEFAULT_PROCESSOR_QUEUE);
	private Distributor distributor;

	interface Handler
	{
		boolean handle(Message message);
	}
	private Map<MessageType, Handler> handlers = new HashMap<MessageType, Handler>();

	RealProcessor() {
		handlers.put(MessageType.ECHO, new Handler()
		{
			public boolean handle(Message message) {
				Message echo = new Message(message.getHeader(), message.getMeta(), null);
				message.getClient().output(echo);
				return true;
			}
		});

		handlers.put(MessageType.NEGOTIATE, new Handler()
		{
			public boolean handle(Message message) {
				return negotiate(message);
			}
		});

		handlers.put(MessageType.SUBSCRIBE, new Handler()
		{
			public boolean handle(Message message) {
				return message.getClient().subscribe(message);
			}
		});

		handlers.put(MessageType.UNSUBSCRIBE, new Handler()
		{
			public boolean handle(Message message) {
				message.getClient().unsubscribe(message.getMeta().getTopic());
				return true;
			}
		});

		Handler distribute = new Handler()
		{
			public boolean handle(Message message) {
				return distribute(message);
			}
		};

		handlers.put(MessageType.PUBLISH, distribute);
		handlers.put(MessageType.REQUEST, distribute);
		handlers.put(MessageType.REPLY, distribute);
	}

	void setDistributor(Distributor distributor) {
		this.distributor = distributor;
	}

	public void enqueue(Message message) {
		try {
			queue.put(message);
		}
		catch (InterruptedException e) {
			Log.exception(source, e);
		}
	}

	public void run() {
		double totalDt = 0;
		int count = 0;

		while (true) {
			try {
				Message message = queue.take();
				// TODO: remove- this is manual profiling
				if (message.getHeader().getType() == MessageType.PUBLISH) {
					message.procTime = System.nanoTime();
					++count;
					totalDt += message.procTime - message.rxTime;
					if ((count % Options.REPORT_n) == 0)
						System.out.format("procTime: after %d, mean procTime=%.2f [ms]\n", count,
								totalDt / count / 1e6);
				}
				dispatch(message);
			}
			catch (Exception e) {
				Log.exception(source, e);
			}
		}
	}

	private void dispatch(Message message) {

		MessageType type = message.getHeader().getType();

		Handler handler = handlers.get(type);
		if (handler == null)
			Log.warning(source, "no handler for type " + type);
		else
			handler.handle(message);
	}

	// handlers
	boolean negotiate(Message message) {
		Meta meta = message.getMeta();

		if (meta == null) {
			message.getClient().output(message.makeAck());
			return true;
		}

		Client.Options options = message.getClient().getOptions();

		for (Property p : meta.getProperties()) {
			String name = p.getName();
			if (name.equalsIgnoreCase("maxMessageSize"))
				options.maxMessageSize = p.asInteger();
			else if (name.equalsIgnoreCase("inputBufferSize"))
				options.inputBufferSize = p.asInteger();
			else if (name.equalsIgnoreCase("outputBufferSize"))
				options.outputBufferSize = p.asInteger();
			else
				Log.warning(source, "unhandled option " + p);
		}

		message.getClient().output(message.makeAck());

		return true;
	}

	boolean distribute(Message message) {
		Meta meta = message.getMeta();
		if (meta == null) {
			Log.warning(source, "distribute: null meta");
			return false;
		}
		String topic = meta.getTopic();
		if (topic == null) {
			Log.warning(source, "distribute: null topic");
			return false;
		}
		distributor.enqueue(message);
		return true;
	}
}
