#include <iostream>
#include "TCPNetwork.hpp"
#include "Utils/Logger.hpp"
#include "APIMessageHandler.hpp"
#include "ClientAPI.hpp"

ClientAPI	g_bots;
ClientAPI	g_servers;
ClientAPI	g_externs;

int main()
{
	LOG("starting server [World] port:<%u>", PORT_API);
	SFactoryServer* server = createServer(PORT_API, &APIMessageHandler::onMsg);
	if (server == nullptr)
		return 1;
	server->trans->wait();
	destroyServer(&server);
	return 0;
}
