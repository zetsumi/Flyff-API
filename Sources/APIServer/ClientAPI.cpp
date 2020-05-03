#pragma warning( disable: 4251 )

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Logger.hpp"

#include "ClientAPI.hpp"

const std::vector<SOCKET>& ClientAPI::getSockets(void) const
{
	return std::ref(sockets);
}

bool	ClientAPI::add(SOCKET id)
{
	locker.lock();
	bool r = false;
	auto it = std::find_if(sockets.begin(), sockets.end(),
		[id](SOCKET s) {return s == id; }
	);
	if (it != sockets.end())
		goto end;
	sockets.push_back(id);
	r = true;
end:
	locker.unlock();
	return r;
}

void	ClientAPI::sendAll(unsigned int length, const unsigned char* buffer)
{
	locker.lock();
	for (auto it : sockets)
	{
		if (it != INVALID_SOCKET)
		{
			if (::send(it, (char*)buffer, length, 0) == -1)
			{
				LOGCONSOLE("can not send to <%u>", it);
			}
		}
	}
	locker.unlock();
}

bool ClientAPI::is(SOCKET id)
{
	locker.lock();
	auto it = std::find_if(sockets.begin(), sockets.end(),
		[id](SOCKET s) { return s == id; }
	);
	locker.unlock();
	if (it == sockets.end())
		return false;
	return true;
}

bool	ClientAPI::remove(SOCKET id)
{
	locker.lock();
	bool r = false;
	auto it = std::find_if(sockets.begin(), sockets.end(),
		[id](SOCKET s) { return s == id; }
	);
	if (it != sockets.end())
	{
		sockets.erase(it);
		r = true;
	}
	locker.unlock();
	return r;
}
