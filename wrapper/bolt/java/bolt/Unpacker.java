
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

import java.nio.ByteBuffer;
import java.util.logging.Logger;
import java.util.logging.Level;

public class Unpacker
{
	private static Logger source = Log.createSource("Unpacker");
	private ByteBuffer headerBuffer = ByteBuffer.allocate(Header.SIZE);
	private ByteBuffer messageBuffer = null;
	private Message message = null;
	private Target target;
	
	
	public interface Target {
		void consume (Message message);
		boolean isAcceptable (Header header);
	}
	
	
	public Unpacker (Target t) {
		this.target = t;
	}

	public boolean ingest (byte[] data, int offset, int received) {

		int limit = offset + received;

		while (offset < limit) {

			int left = limit - offset;

			int count = 0;

			if (messageBuffer != null) {
				if (left < messageBuffer.remaining()) {
					count = left;
					messageBuffer.put(data, offset, count);
				}
				else {
					// finishing message buffer
					count = messageBuffer.remaining();
					messageBuffer.put(data, offset, count);
					messageBuffer.flip();
					headerBuffer = ByteBuffer.allocate(Header.SIZE);
					messageBuffer = null;
					if (source.isLoggable(Level.FINER))
						message.dump("Unpacker: received body, consuming", System.out);
					target.consume(message);
				}
			}
			else if (left < headerBuffer.remaining()) {
				// ended up with partial header
				count = left;
				headerBuffer.put(data, offset, count);
			}
			else {
				// finishing header buffer
				count = headerBuffer.remaining();
				headerBuffer.put(data, offset, count);

				// parse the Header and set up a Message if successful
				headerBuffer.flip();
				Header header = Header.parse(headerBuffer);
				if (header == null)
					return false;
				if (!target.isAcceptable(header))
					return false;
				message = Message.create(header, headerBuffer);

				if (header.getSize() == 0) {
					if (source.isLoggable(Level.FINER))
						message.dump("Unpacker: received header and size=0 so consuming", System.out);
					headerBuffer = ByteBuffer.allocate(Header.SIZE);
					target.consume(message);
				}
				else
					messageBuffer = message.getBuffer();
			}

			offset += count;
		}

		return true;
	}
}


