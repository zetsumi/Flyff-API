#include "pch_api.h"
#include "TCPNetwork.hpp"
#include "APIConnector.hpp"

extern SFactoryClient* client;

extern "C"
{
	API_DECLSPEC bool __cdecl api_put_command(const char* playerName, const char* message)
	{
		if (client == nullptr || client->socket == nullptr || client->trans == nullptr)
			return false;

		PacketBuilder pb;
		pb.write<unsigned int>((unsigned int)API_PACKET_TYPE::PUT_COMMAND);
		pb.writeString(playerName);
		pb.writeString(message);

		SOCKET s = client->socket->getSocket();
		if (client->trans->sender(s, pb) == false)
			return false;
		return true;
	}

}