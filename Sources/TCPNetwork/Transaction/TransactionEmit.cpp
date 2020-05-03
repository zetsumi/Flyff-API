#include "pch_tcp_network.h"
#include "Transaction.hpp"

bool Transaction::sender(SOCKET idSocket, unsigned int size, const char* data)
{
	if (_socket == nullptr)
		return false;
	int errorCode = ::send(idSocket, data, size, 0);
	if (errorCode == SOCKET_ERROR)
	{
		LOGCONSOLE("SOCKET ERROR with client<%u>", idSocket);
		return false;
	}
	return true;
}

bool Transaction::sender(SOCKET idSocket, PacketBuilder& packet)
{
	if (_socket == nullptr)
		return false;
	return sender(idSocket, packet.getSize(), reinterpret_cast<const char*>(packet.getData()));
}

PacketStructure* Transaction::receiver(SOCKET idSocket)
{
	return receiver(idSocket, DEFAULT_BUFFER_SIZE_RECEIVE);
}

PacketStructure* Transaction::receiver(SOCKET idSocket, unsigned int bufferSize)
{
	char* buffer = new char[bufferSize];
	ZeroMemory(buffer, sizeof(buffer));
	int octects = ::recv(idSocket, buffer, bufferSize, 0);
	if (octects <= 0)
	{
		if (isMode(MODE_TRANSACTION::MODE_SERVER) == true)
		{
			SocketServer* ss = dynamic_cast<SocketServer*>(_socket);
			if (ss != nullptr)
			{
				ss->removeClient(idSocket);
				LOGCONSOLE("number client<%u>", ss->getNumberClient());
			}
		}
		return nullptr;
	}
	if (octects > static_cast<int>(bufferSize))
		return nullptr;

	PacketStructure* ps = new PacketStructure();
	if (ps == nullptr)
	{
		return nullptr;
	}
	ps->data = (unsigned char*)buffer;
	ps->size = octects;
	return ps;
}

