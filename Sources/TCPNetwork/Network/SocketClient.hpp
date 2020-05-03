#pragma once

#include "framework.hpp"
#include "Socket.hpp"

class API_DECLSPEC SocketClient : public Socket
{
public:
	SocketClient() = default;
	~SocketClient();

	bool	connect(const Network& network);
	void	clean(void) override;
	void	shutdown(void) override;
};
