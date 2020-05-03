#pragma once

struct SFactoryServer
{
	Network* net;
	SocketServer* socket;
	Transaction* trans;

	SFactoryServer() :
		net(new Network()),
		socket(new SocketServer),
		trans(new Transaction)
	{};
	~SFactoryServer()
	{
		delete net;
		delete socket;
		delete trans;
		net = nullptr;
		socket = nullptr;
		trans = nullptr;
	};
};

struct SFactoryClient
{
	Network* net;
	SocketClient* socket;
	Transaction* trans;

	SFactoryClient() :
		net(new Network()),
		socket(new SocketClient()),
		trans(new Transaction())
	{};
	~SFactoryClient()
	{
		delete net;
		delete socket;
		delete trans;
		net = nullptr;
		socket = nullptr;
		trans = nullptr;
	};
};

inline SFactoryClient* createClient(unsigned int port, const char* ip, callbackOnMessage callback)
{
	SFactoryClient* factory = new SFactoryClient();
	if (factory == nullptr)
		return nullptr;

	if (factory->net->setIP(ip) == false)
		return nullptr;
	factory->net->setPort(port);
	if (factory->socket->connect(*factory->net) == false)
		return nullptr;
	factory->trans->setMode(MODE_TRANSACTION::MODE_CLIENT);
	if (factory->trans->setSocket(factory->socket) == false)
		return nullptr;
	if (factory->trans->run(callback) == false)
		return nullptr;
	return factory;
}

inline SFactoryServer* createServer(unsigned int port, callbackOnMessage callback)
{
	SFactoryServer* factory = new SFactoryServer();
	if (factory == nullptr)
		return nullptr;

	factory->net->setPort(port);
	factory->socket->initialize(*factory->net);
	factory->trans->setMode(MODE_TRANSACTION::MODE_SERVER);
	if (factory->trans->setSocket(factory->socket) == false)
		return nullptr;
	if (factory->trans->run(callback) == false)
		return nullptr;
	return factory;
}

inline void	destroyClient(SFactoryClient** client)
{
	(*client)->socket->shutdown();
	(*client)->socket->clean();
	delete* client;
	*client = nullptr;
}

inline void	destroyServer(SFactoryServer** server)
{
	(*server)->socket->shutdown();
	(*server)->socket->clean();
	delete* server;
	*server = nullptr;
}