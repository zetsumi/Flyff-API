#include "pch_api.h"
#include "TCPNetwork.hpp"
#include "APIConnector.hpp"

extern SFactoryClient* client;

extern "C"
{
	API_DECLSPEC bool __cdecl api_authenticate(unsigned int key, unsigned int author, const char* login)
	{
		if (client == nullptr || client->socket == nullptr || client->trans == nullptr)
			return false;

		PacketBuilder pb;
		pb.write<unsigned int>((unsigned int)API_PACKET_TYPE::AUTHENTICATE);
		pb.write<unsigned int>(key);
		pb.write<unsigned int>(author);
		pb.writeString(login);

		SOCKET s = client->socket->getSocket();
		if (client->trans->sender(s, pb) == false)
			return false;
		return true;
	}
}
