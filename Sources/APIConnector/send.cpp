#include "pch_api.h"
#include "TCPNetwork.hpp"
#include "APIConnector.hpp"

extern SFactoryClient* client;

extern "C"
{
	API_DECLSPEC bool __cdecl api_send(unsigned int length, const unsigned char* buffer)
	{
		if (client == nullptr || client->socket == nullptr || client->trans == nullptr)
			return false;

		SOCKET s = client->socket->getSocket();
		if (client->trans->sender(s, length, (const char*)buffer) == false)
			return false;
		return true;
	}
}