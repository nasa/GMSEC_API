
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

import java.util.*;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.logging.Logger;


interface Distributor
{
	String subscribe (Client client, String topic);
	void unsubscribe (Client client, String topic);

	void enqueue (Message message);
	void prune (Client client);
}


class FakeDistributor
	implements Distributor
{
	public String subscribe (Client client, String topic) {
		return null;
	}

	public void unsubscribe (Client client, String topic) {
	}

	public void enqueue (Message message) {
	}

	public void prune (Client client) {
	}
}


class RealDistributor
	implements Distributor
{

	static final String STAR = "*";
	static final String ANGLE = ">";
	static final String PLUS = "+";

	static private class Group
	{
		Client client;
		List<Client> clients;
		Map<String, Group> subgroups;
	}

	static private Logger source = Log.createSource("Distributor");

	private LinkedBlockingQueue<Message> queue = new LinkedBlockingQueue<Message>(
			Options.DEFAULT_DISTRIBUTOR_QUEUE);
	private Group distribution = new Group();

	public synchronized String subscribe (Client client, String topic) {
		
		String[] elements = Topic.getElements(topic);
		if (!Topic.areValidElements(elements, true))
			return "invalid topic: " + topic;

		Group group = getGroup(elements, true);
		boolean duplicate = false;

		if (group.clients != null) {
			if (group.clients.contains(client))
				duplicate = true;
			else
				group.clients.add(client);
		}
		else if (group.client == null) {
			// this is the first/only subscriber in this level
			group.client = client;
		}
		else if (group.client.equals(client))
			duplicate = true;
		else {
			// switch from single client to list of clients
			group.clients = new ArrayList<Client>(4);
			group.clients.add(group.client);
			group.clients.add(client);
			group.client = null;
		}

		if (duplicate)
			return "duplicate subscription " + topic;
		
		return null;
	}

	public synchronized void unsubscribe (Client client, String topic) {
		String[] elements = Topic.getElements(topic);
		Group group = getGroup(elements, false);

		boolean missing = false;

		if (group == null)
			missing = true;
		else if (group.clients != null) {
			if (!group.clients.remove(client))
				missing = true;
		}
		else if (group.client.equals(client))
			group.client = null;
		else
			missing = true;

		if (missing)
			Log.warning(source, "unsubscribe: client not subscribed to " + topic);
		else
			Log.detail(source, "unsubscribe: client unsubscribed from " + topic);
	}

	private Group getGroup (String[] elements, boolean create) {

		Group group = distribution;

		for (int i = 0; i < elements.length; ++i) {
			String element = elements[i];

			if (group.subgroups == null)
				if (create)
					group.subgroups = new HashMap<String, Group>(4);
				else
					return null;

			Group tmp = group.subgroups.get(element);
			if (tmp == null)
				if (create)
					group.subgroups.put(element, tmp = new Group());
				else
					return null;

			group = tmp;
		}

		return group;
	}

	/**
	 * @brief Put message in queue for distribution to subscribers.
	 */
	private int count = 0;
	private double totalDt = 0;

	public synchronized void enqueue (Message message) {
		// assert message.getHeader().getType().isDistributable();

		if (message.getHeader().getType() == MessageType.PUBLISH) {
			message.distTime = System.nanoTime();
			++count;
			totalDt += message.distTime - message.procTime;
			if ((count % Options.REPORT_n) == 0)
				System.out.format("distTime: after %d, mean distTime=%.2f [ms]\n", count,
						totalDt / count / 1e6);
		}

		distribute(message);
	}

	private void addClients (Set<Client> clients, Group group) {
		if (group.clients != null) {
			for (Client client : group.clients)
				clients.add(client);
		}
		else if (group.client != null) {
			// this is the first/only subscriber in this level
			clients.add(group.client);
		}
		else
			; // no clients
	}

	private void addClientsAux (Set<Client> clients, String[] elements, Group group, int index) {

		if (index == elements.length - 1)
		{
			addClients(clients, group);

			// 0 case
			if (group.subgroups != null)
			{
				Group plus = group.subgroups.get(PLUS);
				if (plus != null)
				{
					 addClients(clients, plus);
				}
			}
		}
		else if (group.subgroups != null)
			getClients(clients, elements, group, index + 1);
		else
			; // group does not have subgroups to include
	}

	private void getClients (Set<Client> clients, String[] elements, Group group, int index) {

		String element = elements[index];

		Map<String, Group> subgroups = group.subgroups;

		if (subgroups == null)
			return;

		// 1 or more case, essentially the angle case
		Group plus = subgroups.get(PLUS);
		if (plus != null)
			addClients(clients, plus);
		
		Group angle = subgroups.get(ANGLE);
		if (angle != null)
			addClients(clients, angle);

		Group star = subgroups.get(STAR);
		if (star != null)
			addClientsAux(clients, elements, star, index);

		Group named = subgroups.get(element);
		if (named != null)
			addClientsAux(clients, elements, named, index);

		/**
		 * @code too(?) clever implementation of previous code + addClientsAux
		 * 
		 * <pre>
		 *       
		 *		for (String s : new String[] { ANGLE, STAR, element }) {
		 * 			Group subgroup = group.subgroups.get(s);
		 * 			if (subgroup != null) {
		 * 				if (index == elements.length - 1)
		 * 					addClients(clients, subgroup);
		 *      		else if (subgroup.subgroups != null)
		 *       			getClients(clients, elements, subgroup, index + 1);
		 *       		else
		 *       			; // subgroup does not have subgroups to include
		 *			}
		 *		}
		 * </pre>
		 */

	}

	private void distribute (Message message) {
		// assert message.getHeader().getType().isDistributable();

		String topic = message.getMeta().getTopic();
		String[] elements = Topic.getElements(topic);
		if (!Topic.areValidElements(elements, false)) {
			Log.detail(source, "distribute: invalid topic " + topic);
			return;
		}

		Set<Client> clients = new HashSet<Client>();

		getClients(clients, elements, distribution, 0);
		if (clients.size() == 0)
			Log.detail(source, "distribute: no clients for " + topic);

		for (Client client : clients)
			client.output(message);
	}

	public synchronized void prune (Client client) {
		// TODO: if Client.cleanup properly unsubscribes, this will not find any
		// remaining references, but we can take the opportunity to prune
		// unneeded nodes from the distribution tree
		Log.detail(source, "purge: not implemented");
	}

}
