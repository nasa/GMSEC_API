
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

import java.nio.ByteBuffer;
import java.util.*;
import java.util.logging.Logger;
import java.util.logging.Level;

/**
 * @class MetaData
 * @brief Message meta-data
 */

public class Meta
{
	interface Parser
	{
		Property parse (Meta meta, ByteBuffer buffer);
	}

	static private Map<PropertyType, Parser> parsers;
	static {
		parsers = new HashMap<PropertyType, Parser>();

		parsers.put(PropertyType.STRING, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				String name = getPropertyName(buffer);
				String value = getPropertyString(buffer);
				return new StringProperty(name, value);
			}
		});

		parsers.put(PropertyType.I32, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				String name = getPropertyName(buffer);
				int value = buffer.getInt();
				return new IntegerProperty(name, value);
			}
		});

		parsers.put(PropertyType.F64, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				String name = getPropertyName(buffer);
				double value = buffer.getDouble();
				return new DoubleProperty(name, value);
			}
		});

		parsers.put(PropertyType.FLAG, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				String name = getPropertyName(buffer);
				boolean value = buffer.get() != 0 ? true : false;
				return new FlagProperty(name, value);
			}
		});

		parsers.put(PropertyType.ID, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				buffer.get();
				String value = getPropertyString(buffer);
				meta.id = value;
				return Property.createID(value);
			}
		});

		parsers.put(PropertyType.TOPIC, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				buffer.get();
				String value = getPropertyString(buffer);
				meta.topic = value;
				return Property.createTopic(value);
			}
		});

		parsers.put(PropertyType.CORR_ID, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				buffer.get();
				String value = getPropertyString(buffer);
				meta.corrID = value;
				return Property.createCorrID(value);
			}
		});

		parsers.put(PropertyType.REPLY_TO, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				buffer.get();
				String value = getPropertyString(buffer);
				meta.replyTo = value;
				return Property.createReplyTo(value);
			}
		});

		parsers.put(PropertyType.COMPRESS, new Parser() {
			public Property parse (Meta meta, ByteBuffer buffer) {
				buffer.get();
				meta.compress = buffer.get() != 0;
				return Property.createCompress(meta.compress);
			}
		});
	}

	// common properties are directly addressable
	private String id;
	private String topic;
	private String corrID;
	private String replyTo;
	private String selector;
	private boolean compress;

	// generic properties are in a hash
	private Map<String, Property> properties = new HashMap<String, Property>();

	private int encodedSize = 0;

	private static Logger source = Log.createSource("Meta");

	static final int MAX_ITEMS = 10000;

	boolean parse (java.nio.ByteBuffer buffer) {

		boolean ok = false;

		try {
			ok = parseAux(buffer);
		}
		catch (Exception e) {

			Log.error(source, "parseAux: " + e);
		}

		return ok;
	}

	boolean parseAux (java.nio.ByteBuffer buffer) {

		// there must be space for the size[int] and count[short]
		assert buffer.array().length >= 6;

		int size = buffer.getInt();
		if (size < 0) {
			Log.error(source, "invalid meta size " + size);
			return false;
		}

		int count = buffer.getShort();
		if (count < 0 || count > MAX_ITEMS) {
			Log.error(source, "invalid meta item count " + count);
			return false;
		}

		if (source.isLoggable(Level.FINER))
			Log.info(source, "parseAux: size=" + size + " count=" + count);

		for (int i = 0; i < count; ++i) {

			int enctype = buffer.get();
			int mode = enctype >> 6;
			int ptype = enctype & 0x3f;
			PropertyType type = PropertyType.find(ptype);

			int psize = -1;
			if (mode == Property.Mode.SMALL.value()) {
				psize = buffer.get();
				if (psize < 0)
					psize += 256;
			}
			else if (mode == Property.Mode.MEDIUM.value()) {
				psize = buffer.getShort();
				if (psize < 0)
					psize += 256*256;
			}
			else if (mode == Property.Mode.LARGE.value()) {
				int top = buffer.get();
				if (top < 0) top += 256;
				int bottom = buffer.getShort();
				if (bottom < 0)
					bottom += 256*256;
				psize = top*256*256 + bottom;
			}

			byte[] raw = new byte[psize];
			buffer.get(raw);
			ByteBuffer sub = ByteBuffer.wrap(raw);

			Property property = null;
			Parser parser = getParser(type);

			if (parser != null)
				property = parser.parse(this, sub);
			else
				Log.error(source, "invalid property type " + ptype);

			if (property != null) {
				property.setType(type);
				properties.put(property.getName(), property);
			}
		}

		return true;
	}

	private Parser getParser (PropertyType type) {
		Parser parser = null;
		if (type != null) {
			parser = parsers.get(type);
			if (parser == null)
				Log.error(source, "getParser: unregistered type " + type);
		}
		return parser;
	}

	static private String getPropertyName (ByteBuffer buffer) {
		int length = buffer.get();
		if (length < 0) length += 256;
		String name = null;
		if (length > 0) {
			byte[] data = new byte[length];
			buffer.get(data);
			name = new String(data);
		}
		return name;
	}


	static private String getPropertyString (ByteBuffer buffer) {
		int length = buffer.remaining();
		byte[] data = new byte[length];
		buffer.get(data);
		return new String(data);
	}


	public String getTopic () {
		return topic;
	}

	public String getID () {
		return id;
	}

	public boolean getCompress () {
		return compress;
	}

	public java.util.Collection<Property> getProperties () {
		return properties.values();
	}

	public Property getProperty (String name) {
		return properties.get(name);
	}

	public void dump (String tag, java.io.PrintStream out) {
		out.println(tag + " count=" + properties.size());
		for (Property p : getProperties())
			out.println(p.toString());
	}

	public void addProperty (Property p) {
		properties.put(p.getName(), p);
	}

	
	void put (ByteBuffer buffer) {
		assert encodedSize > 0;
		assert buffer.remaining() >= encodedSize;
		assert properties.size() <= MAX_ITEMS;

		buffer.putInt(encodedSize);
		buffer.putShort((short) properties.size());
		Property.Encoding encoding = new Property.Encoding();
		for (Property p : properties.values()) {

			PropertyType type = p.getType();

			p.getEncoding(encoding);

			int bits = (encoding.mode.value() << 6) | type.identifier();
			buffer.put((byte) bits);

			putEncLength(buffer, encoding.mode, encoding.count);

			// for generic properties, need to put name
			if (type.isNamed()) {
				buffer.put((byte) p.getName().length());
				buffer.put(p.getName().getBytes());
			}
			else
				buffer.put((byte) 0);

			if (!type.isNamed())
				buffer.put(p.asString().getBytes());
			else if (type == PropertyType.STRING)
				buffer.put(p.asString().getBytes());
			else if (type == PropertyType.I32)
				buffer.putInt(p.asInteger());
			else if (type == PropertyType.F64)
				buffer.putDouble(p.asDouble());
			else
				Log.error(source, "bug: put does not handle " + type);
		}

	}

	private void putEncLength(ByteBuffer buffer, Property.Mode mode, int length) {
		if (mode == Property.Mode.SMALL) {
			buffer.put((byte) length);
		}
		else if (mode == Property.Mode.MEDIUM) {
			buffer.putShort((short) length);
		}
		else if (mode == Property.Mode.LARGE) {
			int top = length >> 16;
			buffer.put((byte) top);
			buffer.putShort((short) (length & 0xffff));
		}
	}


	/** @brief determine and cache the encoded size of the meta data */
	int determineSize () {
		int size = 0;
		size += 4; // size
		size += 2; // count
		Property.Encoding encoding = new Property.Encoding();

		for (Property p : properties.values()) {
			p.getEncoding(encoding);
 // System.out.println("property: " + p.toString() + " prefix=" + encoding.prefix + " count=" + encoding.count);

			size += encoding.prefix;
			size += encoding.count;
		}

		// cache the encoded size
		encodedSize = size;

		return size;
	}
}
