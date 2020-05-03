#include "pch_tcp_network.h"
#include "SocketServer.hpp"

SocketServer::~SocketServer()
{
	shutdown();
	clean();
}

void SocketServer::clean(void)
{
}

void SocketServer::shutdown(void)
{
}

bool SocketServer::initialize(Network& network)
{
	WSADATA				wsa;
	struct addrinfo*	result = nullptr;
	struct addrinfo		hints;
	int errorCode = 0;

	errorCode = ::WSAStartup(MAKEWORD(2, 2), &wsa);
	if (errorCode != 0)
		return false;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::string stringPort = std::to_string(network.getPort());
	const char* port = stringPort.c_str();
	errorCode = ::getaddrinfo(NULL, port, &hints, &result);
	if (errorCode != 0)
		return false;

	_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_socket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	errorCode = ::bind(_socket, result->ai_addr, (int)result->ai_addrlen);
	if (errorCode != 0)
	{
		freeaddrinfo(result);
		closesocket(_socket);
		WSACleanup();
		return false;
	}
	freeaddrinfo(result);

	errorCode = ::listen(_socket, SOMAXCONN);
	if (errorCode != 0)
	{
		closesocket(_socket);
		WSACleanup();
		return false;
	}

	return true;
}

bool SocketServer::addClient(SOCKET client)
{
	auto it = std::find_if(clients.begin(), clients.end(),
		[client](auto s)
		{return client == s; }
	);
	if (it == clients.end())
	{
		locker.lock();
		clients.push_back(client);
		locker.unlock();
		LOGCONSOLE("New client: %u", client);
		return true;
	}
	return false;
}

void	SocketServer::removeClient(SOCKET idClient)
{
	locker.lock();
	auto it = std::find_if(clients.begin(), clients.end(),
		[idClient](SOCKET idSocket)
		{return idSocket == idClient; }
	);
	if (it != clients.end())
		clients.erase(it);
	LOGCONSOLE("size clients <%u>", clients.size());
	locker.unlock();
}


SOCKET SocketServer::getClient(unsigned int index)
{
	if (getNumberClient() <= index)
		return INVALID_SOCKET;
	locker.lock();
	SOCKET idClient = clients.at(index);
	locker.unlock();
	return idClient;
}

unsigned int SocketServer::getNumberClient(void)
{
	locker.lock();
	unsigned int size = clients.size();
	locker.unlock();
	return size;
}

const std::vector<SOCKET>& SocketServer::getClients(void) const
{
	return clients;
}
