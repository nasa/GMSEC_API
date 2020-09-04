#include <zmq.h>

#include <gmsec4/Config.h>

#include <gmsec4/ConfigOptions.h>

#include <iostream>


int main (int argc, char* argv[])
{
	gmsec::api::Config config(argc, argv);

	const char* pubEndpoint = config.getValue(gmsec::api::ZEROMQ_PUB_ENDPOINT);
	const char* subEndpoint = config.getValue(gmsec::api::ZEROMQ_SUB_ENDPOINT);

	if (pubEndpoint == 0 && subEndpoint == 0)
	{
		std::cerr << "Invalid or missing publish-endpoint and subscribe-endpoint values" << std::endl;
		std::cerr << "Usage: " << gmsec::api::BROKER_USAGE << std::endl;
		return -1;
	}
	else if (pubEndpoint == 0)
	{
		std::cerr << "Invalid or missing publish-endpoint value" << std::endl;
		std::cerr << "Usage: " << gmsec::api::BROKER_USAGE << std::endl;
		return -1;
	}
	else if (subEndpoint == 0)
	{
		std::cerr << "Invalid or missing subscribe-endpoint value" << std::endl;
		std::cerr << "Usage: " << gmsec::api::BROKER_USAGE << std::endl;
		return -1;
	}

	std::cout << "Starting up the ZeroMQ Proxy Broker..." << "\n"
	          << "Publishers should connect to : " << pubEndpoint << "\n"
	          << "Subscribers should connect to: " << subEndpoint << "\n"
	          << std::endl;

	void* context = zmq_ctx_new();

	// Set up the XSUB socket (Connect Publishers here)
	void* frontend  = zmq_socket(context, ZMQ_XSUB);
	int   zmqResult = zmq_bind(frontend, pubEndpoint); 

	if (zmqResult == -1)
	{	
		std::cerr << "ZeroMQ Socket bind operation failed, POSIX errno code: " << zmq_errno() << std::endl;
		return zmqResult;
	}

	// Set up the XPUB socket (Connect Subscribers here)
	void* backend = zmq_socket(context, ZMQ_XPUB);
	zmqResult = zmq_bind(backend, subEndpoint); 

	if (zmqResult == -1)
	{	
		std::cerr << "ZeroMQ Socket bind operation failed, POSIX errno code: " << zmq_errno() << std::endl;
		return zmqResult;
	}

	// Set up the proxy to forward subscriptions/publications
	zmq_proxy(frontend, backend, NULL);
}
