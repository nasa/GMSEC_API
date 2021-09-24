
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @class Server
 * @brief This is the main bolt server class.
 * The main thread accepts client connections and hands them off to IOManagers.
 */

package bolt;

import bolt.Client.Shared;

import java.net.*;

import java.nio.channels.*;

import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.logging.LogManager;

public class Server
{
	private Options options = new Options();

	private java.util.List<IOManager> ioManagers = new java.util.ArrayList<IOManager>(10);

	private RealDistributor distributor;
	private RealProcessor processor;
	private Statistics statistics = new Statistics();

	static final Logger source = Log.createSource("Server");
	final int BOLT_PRIORITY = Thread.NORM_PRIORITY + 2;

	private void execute () {
		ServerSocketChannel channel = null;
		try {
			channel = ServerSocketChannel.open();
			// open another channel for secure connections?
			// open another channel for peer server connections?

			distributor = new RealDistributor();

			processor = new RealProcessor();
			processor.setPriority(BOLT_PRIORITY);
			processor.setDistributor(distributor);
			processor.start();

			// what is IPv6 array?
			byte[] array = new byte[] { 0, 0, 0, 0 };
			SocketAddress address = new InetSocketAddress(InetAddress.getByAddress(array),
					options.port);

			channel.socket().bind(address, options.backlogSize);

			Log.info(source, Version.GMSEC_BOLT_VERSION);
			Log.info(source, "waiting for connections on " + channel.getLocalAddress());

			while (true)
				assign(channel.accept());
		}
		catch (java.io.IOException e) {
			Log.exception(source, e);
		}
		finally {
			if (channel != null) {
				try {
					channel.close();
				}
				catch (java.io.IOException e) {
					Log.exception(source, e);
				}
			}
		}
	}

	/**
	 * @brief assign the socket to an IOManager
	 * @param socket
	 */
	private void assign (SocketChannel channel) {

		try {
			channel.configureBlocking(false);
		}
		catch (java.io.IOException e) {
			Log.exception(source, e, true);
		}

		IOManager manager = null;

		for (IOManager m : ioManagers)
			if (m.isAvailable())
				manager = m;

		if (manager == null) {
			Log.info(source, "creating IOManager");
			manager = new IOManager();
			ioManagers.add(manager);
			manager.setOptions(options);
			manager.start();
		}

		Shared shared = new Shared();
		shared.manager = manager;
		shared.distributor = distributor;
		shared.processor = processor;
		shared.statistics = statistics;
		Client client = new Client(shared, channel);
		options.initialize(client.getOptions());

		manager.assign(client);
	}


	/**
	 * @brief Start a bolt server
	 * @param args
	 *            server options
	 */
	public static void main (String[] args) {
		Options options = new Options();
		if (options.parse(args)) {
			configureLogging(options.logLevel);
			Server server = new Server();
			server.options = options;
			server.execute();
		}
	}


	private static void configureLogging(Level level) {
		Logger rootLogger = LogManager.getLogManager().getLogger("");
		rootLogger.setLevel(level);
		for (Handler h : rootLogger.getHandlers()) {
			h.setLevel(level);
		}
	}
}
