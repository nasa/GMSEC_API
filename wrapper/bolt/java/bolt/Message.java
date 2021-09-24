
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

import java.nio.*;
import java.util.Arrays;

public class Message
{
	private Header header;
	private Meta meta;
	private ByteBuffer buffer;
	private byte[] body;
	private Client client;
	
	// TODO: remove- this is manual profiling
	long rxTime, txTime, procTime, distTime;

	Message () {
	}

	public Message (Header header, Meta meta, byte[] body) {
		this.header = header;
		this.meta = meta;
		this.body = body;
	}

	public Header getHeader () {
		return header;
	}

	public Meta getMeta () {
		return meta;
	}

	public ByteBuffer getBuffer () {
		return buffer;
	}

	void setClient (Client client) {
		this.client = client;
	}

	Client getClient () {
		return client;
	}

	public static Message create (Header header, ByteBuffer headerBuffer) {

		assert headerBuffer.capacity() == Header.SIZE;

		Message message = new Message();

		message.header = header;

		message.buffer = ByteBuffer.allocate(Header.SIZE + header.getSize());
		message.buffer.put(headerBuffer.array(), 0, Header.SIZE);

		return message;
	}

	public void dump (String label, java.io.PrintStream out) {
		out.println(label);

		if (header != null)
			header.dump("header:", out);
		else
			out.println("header: null");

		if (meta != null)
			meta.dump("meta:", out);
		else
			out.println("meta: null");

		if (body != null)
			out.println("body: size=" + body.length);
		else
			out.println("body: null");

		if (buffer != null)
			out.println("buffer: capacity=" + buffer.capacity() + " position=" + buffer.position()
					+ " remaining=" + buffer.remaining());
		else
			out.println("buffer: null");
	}

	public synchronized ByteBuffer getEncoded () {
		if (buffer == null)
			encode();
		return buffer;
	}


	private ByteBuffer encode () {

		// the size of the message does not include the header...
		int size = 0;
		int flags = 0;
		if (meta != null) {
			size += meta.determineSize();
			flags |= Header.FLAG_META;
		}

		if (body != null) {
			size += body.length;
			flags |= Header.FLAG_BODY;
		}

		header.setSize(size);
		header.setFlags(flags);

		buffer = ByteBuffer.allocate(Header.SIZE + size);

		header.put(buffer);
		if (meta != null)
			meta.put(buffer);
		if (body != null)
			buffer.put(body);

		buffer.flip();
// dump("Message.encode: end", System.out);

		return buffer;
	}

	/**
	* Added this function since it's not available in JDK 5
	*/
	private static byte[] copyOfRange (byte[] original, int from, int to) {
		int newLen = to -from;
		if (newLen < 0) throw new IllegalArgumentException(from + " > " + to);
		byte[] copy = new byte[newLen];
		System.arraycopy(original, from, copy, 0, Math.min(original.length - from, newLen));
		return copy;
	}

	public void decode () {
		assert buffer != null;
		assert header != null;
		assert meta == null;
		assert body == null;

		buffer.position(Header.SIZE);

		if (header.hasMeta()) {
			meta = new Meta();
			meta.parse(buffer);
		}

		if (buffer.remaining() > 0) {
			int start = buffer.position();
			int end = buffer.capacity();
			body = copyOfRange(buffer.array(), start, end);
		}
	}
	
	public Message makeAck() {
		Header header = new Header(MessageType.ACK);
		Meta ometa = null;
		if (meta != null) {
			String id = meta.getID();
			if (id != null) {
				ometa = new Meta();
				ometa.addProperty(Property.createID(id));
			}
		}
		Message ack = new Message(header, ometa, null);
		return ack;
	}

}
