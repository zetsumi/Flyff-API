#pragma once

#include <list>
#include <vector>
#include <mutex>

#include "framework.hpp"
#include "Socket.hpp"

class API_DECLSPEC SocketServer : public Socket
{
	std::mutex	locker;
	std::vector<SOCKET>	clients;

public:
	SocketServer() = default;
	~SocketServer();

	void	clean(void) override;
	void	shutdown(void) override;
	bool	initialize(Network& network);
	bool	addClient(SOCKET client);
	void	removeClient(SOCKET client);
	SOCKET	getClient(unsigned int index);
	unsigned int	getNumberClient(void);
	const std::vector<SOCKET>& getClients(void) const;
};
