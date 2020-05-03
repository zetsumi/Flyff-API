#include "pch_api.h"
#include "APIConnector.hpp"

api_flyff_function::api_flyff_function() :
	initialize(nullptr),
	destroy(nullptr),
	authenticate(nullptr),
	putChatMessage(nullptr),
	putCommand(nullptr),
	send(nullptr)
{
}

api_flyff_function::api_flyff_function(api_flyff_function* api) :
	initialize(api->initialize),
	destroy(api->destroy),
	authenticate(api->authenticate),
	putChatMessage(api->putChatMessage),
	putCommand(api->putCommand),
	send(api->send)
{
}

api_flyff_function::api_flyff_function(const api_flyff_function& api) :
	initialize(api.initialize),
	destroy(api.destroy),
	authenticate(api.authenticate),
	putChatMessage(api.putChatMessage),
	putCommand(api.putCommand),
	send(api.send)
{
}


api_flyff_function::~api_flyff_function()
{
	initialize = nullptr;
	destroy = nullptr;
	authenticate = nullptr;
	putChatMessage = nullptr;
	putCommand = nullptr;
	send = nullptr;
}

api_flyff_function& api_flyff_function::operator=(const api_flyff_function& api)
{
	initialize = api.initialize;
	destroy = api.destroy;
	authenticate = api.authenticate;
	putChatMessage = api.putChatMessage;
	putCommand = api.putCommand;
	send = api.send;
	return *this;
}
