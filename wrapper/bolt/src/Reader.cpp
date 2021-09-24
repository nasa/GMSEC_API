/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <bolt/Shared.h>
#include <bolt/Packet.h>
#include <bolt/Log.h>
#include <bolt/util.h>

#include <gmsec4/util/StdUniquePtr.h>
#include <gmsec4/util/TimeUtil.h>


static int DEBUG_READER = 0;

namespace bolt {


struct InputState
{
public:
	u8 raw[Header::SIZE];
	ByteBuffer buffer;
	bool isHeader;
	i32 position;

	void reset() {
		isHeader = true;
		position = 0;
		buffer.set(&raw[0], Header::SIZE, false);
	}
};


// split into Reader and Unpacker (as in Java client)

void runReader(counted_ptr<Shared> shared)
{
	const int timeout_ms = 5000;

	SharedPacket packet;
	State state;

	InputState input;

	string tag = shared->tag("runReader");

	bool &isHeader = input.isHeader;
	i32 &position = input.position;
	ByteBuffer &buffer = input.buffer;

	i32 ioTraps = 0;
	i32 ioLocal = shared->getIoCounter(ioTraps) - 1;
	i32 traps = 0;

	while ((state = shared->getState()) != STATE_FINISHED)
	{
		const char * problem = 0;
		bool boring = false;

		i32 ioShared = shared->getIoCounter(traps);
		if (ioLocal != ioShared)
		{
			ioLocal = ioShared;
			ioTraps = traps;
			input.reset();
			GMSEC_VERBOSE << "i/o revised- reset buffer";
		}
		else if (state == STATE_CONNECTED)
		{
			i32 want = buffer.size() - position;
			int read = shared->read(buffer.raw() + position, want);
if (DEBUG_READER)
{
	GMSEC_DEBUG << tag.c_str() << "isHeader=" << isHeader << " want=" << want << " read=" << read;
}
			if (read > 0)
			{
				if (read == want)
				{
					if (isHeader)
					{
						// got header, set up for body
						gmsec::api::util::StdUniquePtr<Header> header(Header::parse(buffer));
						if (!header.get())
						{
							problem = "unable to parse packet header";
						}
						else if (header->getSize() > 0)
						{
							// the header indicates there is content, so prepare for it
							buffer.resize(header->getSize());
							isHeader = false;
							packet.reset(new Packet(header.release()));
						}
						else
						{
							// the header indicates no content, so process the packet
							packet.reset(new Packet(header.release()));
							shared->handle(packet);
						}

						position = 0;
					}
					else
					{
						// got packet, set up for next header
						if (packet->getContent(buffer))
						{
							shared->handle(packet);
							input.reset();
						}
						else
							problem = "unable to parse packet";
					}
				}
				else
				{
					position += read;
if (DEBUG_READER)
{
	GMSEC_DEBUG << tag.c_str() << "partial read of " << read << "- wanted " << want;
}
				}
			}
			else if (read < 0)
			{
				shared->getIoCounter(traps);
				if (traps == ioTraps)
				{
					problem = "read error";
				}
				else
				{
					ioTraps = traps;
					GMSEC_VERBOSE << "reader detected external i/o change";
				}
			}
			else // StdThread::yield();
				gmsec::api::util::TimeUtil::millisleep(1);
		}
		else
		{
			problem = "not connected";
			boring = true;
		}

		if (problem)
		{
			if (!boring)
			{
				GMSEC_VERBOSE << tag.c_str() << "state=" << util::toString(state).c_str() << ' ' << problem;
			}

			if (shared->getState() == STATE_CONNECTED)
				shared->ioError(problem);

			TestConnected test(shared.get());
			shared->await(timeout_ms, test);

			input.reset();
		}
	}

	GMSEC_VERBOSE << tag.c_str() << "runReader finished";
}

} // namespace bolt

