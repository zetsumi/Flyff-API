#include "pch_api.h"
#include "TCPNetwork.hpp"
#include "APIConnector.hpp"

extern SFactoryClient* client;

extern "C"
{
	API_DECLSPEC bool __cdecl api_initialize(unsigned int port, const char* ip, callbackOnMessage callback)
	{
		client = createClient(port, ip, callback);
		if (client == nullptr)
			return false;
		if (client->trans == nullptr)
			return false;
		if (client->net == nullptr)
			return false;
		if (client->socket == nullptr)
			return false;
		return client->trans->run(callback);
	}

	API_DECLSPEC void __cdecl api_destroy(void)
	{
		destroyClient(&client);
	}
}