
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/**
 * @class Client is used in the server for client connections.
 */
package bolt;

import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Logger;
import java.nio.*;
import java.nio.channels.*;

public class Client
		implements Unpacker.Target
{
	// static private Logger shared = Log.createSource("Client");
	static private int counter;
	static private final boolean DEBUG_IO = false;
	static final boolean DUMP_MESSAGES = false;

	static class Options
	{
		int inputBufferSize = 100 * 1024;
		int maxMessageSize = 10 * 1024 * 1024;
		int outputBufferSize = 100 * 1024;
		int inactivityTimeout_ms = 10 * 1000;
	}

	static class Shared
	{
		IOManager manager;
		Distributor distributor;
		Statistics statistics;
		RealProcessor processor;
	}

	/*
	 * TODO: if IOManager performance indicates it would be appropriate,
	 * Unpacker.ingest() can run in separate thread
	 */
	interface Raw
	{
		void put(ByteBuffer buffer);

		SocketChannel getChannel();

		void close();

		int getBufferedInput();

		boolean hasOutput();

		void doWrite();

		void doRead(long timeout_ms);
	}

	class FakeRaw
			implements Raw
	{
		public void put(ByteBuffer buffer) {
		}

		public SocketChannel getChannel() {
			return null;
		}

		public void close() {
		}

		public int getBufferedInput() {
			return 0;
		}

		public boolean hasOutput() {
			return false;
		}

		public void doWrite() {
		}

		public void doRead(long timeout_ms) {
		}
	}

	class RealRaw
			implements Raw
	{
		private SocketChannel channel;
		private ByteBuffer activeOutput;
		private LinkedBlockingQueue<ByteBuffer> queuedOutput = new LinkedBlockingQueue<ByteBuffer>();
		private ByteBuffer activeInput;

		RealRaw(SocketChannel c) {
			channel = c;
		}

		public SocketChannel getChannel() {
			return channel;
		}

		public void close() {
			SocketChannel c = channel;
			channel = null;
			if (c != null) {
				try {
					c.close();
				}
				catch (Exception e) {
				}
			}
		}

		public int getBufferedInput() {
			if (activeInput == null)
				return 0;
			return activeInput.position();
		}

		public void doRead(long time_ms) {
			try {
				ByteBuffer buffer = activeInput;
				if (buffer == null) {
					buffer = ByteBuffer.allocate(options.inputBufferSize);
					activeInput = buffer;
				}

				int start = buffer.position();
				int count = channel.read(buffer);
				if (count > 0) {

					inactive = false;
					activity_ms = time_ms;

					if (DEBUG_IO)
						Log.info(source, "read " + count);
					shared.statistics.addBytesRead(count);

					if (!unpacker.ingest(buffer.array(), start, count))
						failed("bad data format");

					// if the buffer is mostly used up, just start a new one
					if (buffer.remaining() < options.inputBufferSize / 2)
						activeInput = null;
				}
				else {
					Log.warning(source, "doRead: count=" + count);
					if (count < 0)
						failed("read error");
				}

			}
			catch (java.io.IOException e) {
				Log.exception(source, e, false);
				failed("read exception");
			}
		}

		public void doWrite() {
			activeOutput = prepareOutput();
			if (activeOutput == null) {
				Log.warning(source, "no output");
				return;
			}
			else if (activeOutput.remaining() < 1) {
				Log.warning(source, "nothing to write");
				return;
			}

			try {
				if (DEBUG_IO)
					Log.info(source, "doWrite: " + Client.this + " activeOutput.remaining()=" + activeOutput.remaining());
				int wrote = channel.write(activeOutput);
				if (wrote > 0) {
					shared.statistics.addBytesWritten(wrote);
					if (activeOutput.remaining() == 0)
						activeOutput = queuedOutput.poll();
				}
				if (DEBUG_IO)
					Log.info(source, "wrote " + wrote);
			}
			catch (java.io.IOException e) {
				Log.exception(source, e);
				failed("write exception");
			}
		}

		synchronized ByteBuffer prepareOutput() {
			// figure out how many buffers can be combined
			int count = 0;
			if (activeOutput != null)
				count += activeOutput.remaining();
			for (ByteBuffer buffer : queuedOutput)
				if (count + buffer.remaining() > options.outputBufferSize && count > 0)
					break;
				else
					count += buffer.remaining();

			if (DEBUG_IO)
				Log.info(source, "prepareOutput: " + Client.this + " count=" + count);
			if (count == 0)
				return null;

			if (activeOutput != null && count == activeOutput.remaining())
				return activeOutput;

			ByteBuffer buffer = ByteBuffer.allocate(count);
			if (activeOutput != null)
				buffer.put(activeOutput);

			while (buffer.hasRemaining()) {
				try {
					ByteBuffer other = queuedOutput.take();
					buffer.put(other);
				}
				catch (InterruptedException e) {
				}
			}

			buffer.flip();

			return buffer;
		}

		public synchronized boolean hasOutput() {
			if (activeOutput != null && activeOutput.remaining() > 0)
				return true;
			if (queuedOutput.size() > 0)
				return true;
			return false;
		}

		public synchronized void put(ByteBuffer buffer) {
			try {
				if (buffer.remaining() > 0) {
					ByteBuffer copy = ByteBuffer.wrap(buffer.array());
					queuedOutput.put(copy);
				}
				else
					Log.warning(source, "ignoring empty buffer");
			}
			catch (InterruptedException e) {
			}
		}
	}

	class Cooked
	{
		void input(Message message) {
			message.decode();
			if (DUMP_MESSAGES)
				message.dump("Cooked: received " + Client.this, System.out);
			message.setClient(Client.this);
			shared.statistics.readMessage();
			shared.processor.enqueue(message);
		}
		private double inServerTime;
		private int publishCount;

		void output(Message message) {

			if (message.getHeader().getType() == MessageType.PUBLISH) {
				message.txTime = System.nanoTime();
				long dt = message.txTime - message.rxTime;
				inServerTime += dt;
				++publishCount;
				if ((publishCount % bolt.Options.REPORT_n) == 0)
					System.out.format("output(Message): after %d, mean inServerTime=%.2f [ms]\n",
							publishCount, inServerTime / publishCount / 1e6);
			}
			ByteBuffer buffer = message.getEncoded();
			if (DUMP_MESSAGES)
				message.dump("Cooked: sending " + Client.this, System.out);
			shared.statistics.sentMessage();
			rawView.put(buffer);
		}
	}
	private int id = ++counter;
	private Raw rawView;
	private Cooked messageView;
	private Shared shared;
	private Unpacker unpacker = new Unpacker(this);
	private Options options = new Options();
	private List<String> subscriptions = new LinkedList<String>();
	private boolean cleanedUp = false;
	private Logger source;
	private boolean inactive;
	private long activity_ms = System.currentTimeMillis();

	Client(Shared shared, SocketChannel c) {
		this.shared = shared;

		rawView = new RealRaw(c);

		messageView = new Cooked();
		source = Log.createSource(toString());
		Log.monitor(source, "created");
	}

	Options getOptions() {
		return options;
	}

	SocketChannel getChannel() {
		return rawView.getChannel();
	}

	boolean hasOutput() {
		return rawView.hasOutput();
	}

	int getBufferedInput() {
		return rawView.getBufferedInput();
	}

	void readPending(long time_ms) {
		rawView.doRead(time_ms);
	}

	void writePending() {
		rawView.doWrite();
	}

	void cleanup() {
		if (cleanedUp) {
			Log.detail(source, "already cleaned");
			return;
		}
		cleanedUp = true;

		Distributor distributor = shared.distributor;
		shared.distributor = new FakeDistributor();

		// TODO: empty buffers
		for (String s : subscriptions)
			distributor.unsubscribe(this, s);
		distributor.prune(this);
		subscriptions.clear();

		Raw tmp = rawView;
		rawView = new FakeRaw();
		tmp.close();

		messageView = null;
		shared.manager = null;
	}

	boolean subscribe(Message message) {
		String issue = _subscribe(message.getMeta().getTopic());
		for (int i = 2; issue == null; ++i) {
			Property p = message.getMeta().getProperty("TOPIC-" + i);
			if (p == null)
				break;
			issue = _subscribe(p.asString());
		}
		Message ack = message.makeAck();
		if (issue != null && ack.getMeta() != null)
			ack.getMeta().addProperty(Property.createError(issue));
		output(ack);
		return issue == null;
	}

	private String _subscribe(String topic) {
		String issue = shared.distributor.subscribe(this, topic);
		if (issue == null) {
			subscriptions.add(topic);
			Log.info(source, this + " subscribed to " + topic);
		}
		else
			Log.warning(source, "_subscribe(" + topic + ") => " + issue);
		return issue;
	}

	void unsubscribe(String topic) {
		// String issue = null;
		shared.distributor.unsubscribe(this, topic);
		if (!subscriptions.remove(topic))
			Log.warning(source, "unsubscribe: " + this + " invalid topic " + topic);
	}

	void output(Message message) {
		Cooked cooked = messageView;
		if (cooked != null) {
			cooked.output(message);
			IOManager tmp = shared.manager;
			if (tmp != null)
				tmp.tickle();
		}
	}

	void failed(String reason) {
		Log.warning(source, "failed: " + reason);
		IOManager tmp = shared.manager;
		cleanup();
		if (tmp != null)
			tmp.addDeader(this);
		else
			Log.warning(source, "failed but manager is null");
	}

	public String toString() {
		return "Client[id=" + id + ']';
	}

	public void consume(Message message) {
		message.decode();
		message.rxTime = System.nanoTime();
		Cooked cooked = messageView;
		if (cooked != null)
			cooked.input(message);
	}

	public boolean isAcceptable(Header header) {
		if (header.getSize() > options.maxMessageSize) {
			Log.error(source, "exceeded maxMessageSize: " + header.getSize() + " > "
					+ options.maxMessageSize);
			return false;
		}
		return true;
	}

	void checkActivity(long time_ms) {
		if (inactive) {
			long delta_ms = time_ms - activity_ms;
			Log.warning(source, "checkActivity: inactive for " + delta_ms + " [ms]");
			if (delta_ms > options.inactivityTimeout_ms)
				failed("inactivity timeout");
		}
		inactive = true;
	}
}
