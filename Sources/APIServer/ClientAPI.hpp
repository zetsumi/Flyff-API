#pragma once

#include <Windows.h>
#include <vector>
#include <mutex>
#include <Socket.hpp>
#include "PacketBuilder.hpp"

struct ClientAPI
{
private:
	std::mutex	locker;
	std::vector<SOCKET>	sockets = {};

public:
	ClientAPI() = default;
	~ClientAPI() = default;

	const std::vector<SOCKET>& getSockets(void) const;
	bool	add(SOCKET id);
	bool	remove(SOCKET id);
	void	sendAll(unsigned int length, const unsigned char* buffer);
	bool	is(SOCKET id);
};
