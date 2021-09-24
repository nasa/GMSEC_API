
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

import java.util.logging.Logger;

import static bolt.MessageType.*;

public class Header
{

	final static int FLAG_META = 0x01;
	final static int FLAG_BODY = 0x02;

	/**
	 * @brief the number of bytes in a message header
	 */
	public final static int SIZE = 8;

	/**
	 * @brief a header byte value to verify that the message stream is in sync
	 */
	final static byte SYNC = (byte) 0xb5;

	private static Logger source = Log.createSource("Header");

	private int size;
	private MessageType type;
	private int sync;
	private int flags;

	Header () {
		this(VOID);
	}

	public Header (MessageType type) {
		this.size = -1;
		this.type = type;
		this.sync = SYNC;
		this.flags = 0;
	}

	public static Header parse (java.nio.ByteBuffer buffer) {

		assert buffer.capacity() == SIZE && buffer.position() == 0;

		Header header = new Header();

		header.size = buffer.getInt();
		if (header.size < 0) {
			Log.error(source, "invalid message size " + header.size);
			return null;
		}
		
		// Log.info(source, "parse: size=" + header.size);
		int type = buffer.get();
		header.type = MessageType.find(type);
		if (header.type == null) {
			Log.error(source, "invalid message type " + header.type);
			return null;
		}

		header.sync = buffer.get();
		if (header.sync != SYNC) {
			Log.error(source, "lost message sync " + header.sync);
			return null;
		}

		header.flags = buffer.get();

		return header;
	}

	void put (java.nio.ByteBuffer buffer) {
		assert buffer.capacity() >= SIZE && buffer.position() == 0;

		buffer.putInt(size);

		buffer.put((byte) type.identifier());
		buffer.put((byte) sync);
		buffer.put((byte) flags);
		buffer.put((byte) 0);
	}

	/**
	 * @return the number of bytes in the body of the associated message
	 */
	int getSize () {
		return size;
	}

	boolean hasMeta () {
		return (flags & FLAG_META) != 0;
	}

	void setSize (int size) {
		this.size = size;
	}

	void setFlags (int flags) {
		this.flags = flags;
	}

	/**
	 * @return the type of the associated message
	 */
	public MessageType getType () {
		return type;
	}

	public void dump (String tag, java.io.PrintStream out) {
		if (tag == null)
			tag = "header:";
		out.println(tag + " size=" + size + " " + type + " sync=" + sync);
	}

}
