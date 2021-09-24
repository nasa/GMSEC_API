
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





package bolt;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;
import java.util.logging.Logger;

/**
 * @class IOManager @brief Manage Input/Output for a collection of Clients
 * (socket connections to clients).
 */
public class IOManager
		extends Thread
{
	static final Logger source = Log.createSource("IOManager");
	private List<Client> clients = new ArrayList<Client>(Options.CLIENTS_PER_MANAGER / 4);
	private List<Client> deaders = new ArrayList<Client>(Options.CLIENTS_PER_MANAGER / 4);
	private Selector selector;
	private Options options;
	private Selector tickleable;
	private static final boolean DEBUG = false;

	void setOptions(Options options) {
		this.options = options;
	}

	void assign(Client client) {

		welcome(client);

		synchronized (clients) {
			clients.add(client);

			clients.notifyAll();

			tickle();
		}
	}

	private void welcome(Client client) {
		Header header = new Header(MessageType.WELCOME);
		Meta meta = new Meta();
		meta.addProperty(new StringProperty("VERSION", "1.0"));

		client.output(new Message(header, meta, null));
		Log.info(source, "welcome: added WELCOME to " + client);
	}

	boolean isAvailable() {
		return clients.size() < options.clientsPerManager;
	}

	public void run() {

		while (true) {
			try {
				if (clients.size() > 0)
					processIO();
				else
					waitForClient();
			}
			catch (Exception e) {
				Log.exception(source, e);
			}
		}
	}

	private void processIO() {

		// make a local copy of clients so it can be used
		List<Client> tmp;
		synchronized (clients) {
			tmp = new ArrayList<Client>(clients);
		}

		checkActivity(tmp);

		try {
			if (openSelector()) {
				registerClients(tmp);
				performSelection();
			}
		}
		finally {
			closeSelector();
		}

		buryDeaders();
	}

	private boolean openSelector() {
		assert selector == null;

		try {
			selector = Selector.open();
		}
		catch (java.io.IOException e) {
			Log.exception(source, e);
		}

		return selector != null;
	}

	private void registerClients(List<Client> tmp) {

		tickleable = selector;

		for (Client client : tmp) {

			if (client.getChannel() == null)
				continue;

			int ops = 0;

			// do not read more data from clients which have loads
			if (client.getBufferedInput() < options.inputBufferLimit)
				ops |= SelectionKey.OP_READ;

			if (client.hasOutput())
				ops |= SelectionKey.OP_WRITE;

			try {
				SelectionKey key = client.getChannel().register(selector, ops);
				key.attach(client);
			}
			catch (ClosedChannelException e) {
				addDeader(client);
				Log.exception(source, e);
			}
		}
	}

	private void performSelection() {
		try {
			selector.select(options.selectionTimeout_ms);
		}
		catch (java.io.IOException e) {
			Log.exception(source, e);
		}
		finally {
			tickleable = null;
		}

		Set<SelectionKey> selected = selector.selectedKeys();
		if (DEBUG)
			Log.debug(source, "performSelection: " + selected.size() + " keys ready");

		long time_ms = System.currentTimeMillis();
		for (SelectionKey key : selected)
			processKey(key, time_ms);
	}

	private void closeSelector() {
		if (selector != null) {
			try {
				selector.close();
			}
			catch (IOException e) {
				Log.exception(source, e);
			}
			finally {
				selector = null;
			}
		}
	}

	private void processKey(SelectionKey key, long time_ms) {

		Client client = null;
		if (Client.class.isInstance(key.attachment())) {
			client = (Client) key.attachment();
			if (DEBUG)
				Log.detail(source, "processKey: " + client + " is ready");
		}

		if (!key.isValid()) {
			Log.warning(source, "processKey: " + client + " not valid");
			if (client != null)
				addDeader(client);
		}
		else if (client != null) {

			if (key.isReadable())
				client.readPending(time_ms);

			// could no longer be valid...
			if (key.isValid() && key.isWritable())
				client.writePending();
		}
		else {
			if (DEBUG)
				Log.detail(source, "processKey: non client is ready");
		}
	}

	private void waitForClient() {

		try {
			synchronized (clients) {
				clients.wait();
			}
		}
		catch (InterruptedException e) {
			Log.warning(source, "waitForClient interrupted");
		}

		tickle();
	}

	private void buryDeaders() {
		// clean up remains of dead clients
		synchronized (deaders) {
			for (Client client : deaders) {
				client.cleanup();
				Log.monitor(source, "buryDeader " + client);
				synchronized (clients) {
					clients.remove(client);
				}
			}
			deaders.clear();
		}
	}

	void addDeader(Client client) {
		synchronized (deaders) {
			deaders.add(client);
		}
	}

	void tickle() {
		if (DEBUG)
			Log.debug(source, "tickle!");
		Selector tmp = tickleable;
		if (tmp != null)
			tmp.wakeup();
	}
	private long check_ms = System.currentTimeMillis();

	private void checkActivity(List<Client> clients) {
		long now_ms = System.currentTimeMillis();
		if (now_ms - check_ms > options.inactivityInterval_ms) {
			check_ms = now_ms;

			for (Client client : clients) {
				if (client.getChannel() == null)
					continue;
				client.checkActivity(now_ms);
			}
		}
	}
}
