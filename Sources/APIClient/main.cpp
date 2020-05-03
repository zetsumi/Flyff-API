#include <iostream>
#include "APIConnector.hpp"
#include "Logger.hpp"
#include "PacketBuilder.hpp"
#include "APIFlyffPacketType.hpp"


void	onMsg(SOCKET id, PacketStructure* ps)
{
	PacketBuilder pb;
	pb.setPacket(ps);

	unsigned int size = pb.read<unsigned int>();
	API_PACKET_TYPE type = (API_PACKET_TYPE)pb.read<unsigned int>();
	LOGCONSOLE("size<%u> type<%u>", size, type);

	switch (type)
	{
	case API_PACKET_TYPE::SEND_BUFFER:
		break;
	case API_PACKET_TYPE::AUTHENTICATE:
	{
		unsigned int nb1 = pb.read<unsigned int>();
		unsigned int nb2 = pb.read<unsigned int>();
		const char* message = pb.readString();
		LOGCONSOLE("nb1<%u> nb2<%u> message<%s>", nb1, nb2, message);
	} break;
	case API_PACKET_TYPE::PUT_CHAT_MESSAGE:
		break;
	case API_PACKET_TYPE::PUT_COMMAND:
	{
		const char* name = pb.readString();
		const char* command = pb.readString();
		LOGCONSOLE("name[%s] command[%s]", name, command);
	}break;
	default:
		break;
	}


}

static bool	api_dll()
{
	bool r = false;
	HINSTANCE inst;

	inst = LoadLibraryW(TEXT(API_FLYFF_DLL_NAME));
	if (inst == nullptr)
		return false;
	api_flyff_function api;

	api.initialize = (API_INIT_FLYFF)GetProcAddress(inst, "api_initialize");
	api.destroy = (API_DESTROY_FLYFF)GetProcAddress(inst, "api_destroy");
	api.authenticate = (API_AUTHENTICATE_FLYFF)GetProcAddress(inst, "api_authenticate");
	api.putChatMessage = (API_PUT_CHAT_MESSAGE_FLYFF)GetProcAddress(inst, "api_chat_message");
	api.putCommand = (API_PUT_COMMAND_FLYFF)GetProcAddress(inst, "api_command");
	api.send = (API_SEND)GetProcAddress(inst, "api_send");

	r = api.initialize(27015, "127.0.0.1", onMsg);
	if (r == false)
		return r;
	r = api.authenticate(1234, (unsigned int)API_FLYFF_AUTHENTICATE::CLIENT_EXTERN, "admin");
	if (r == false)
		return r;
	//r = api.putChatMessage(0, "zetsumi", "blablabla");
	//if (r == false)
	//	return r;
	//r = api.putCommand("zetsumi", "/level blade 121 2");
	//if (r == false)
	//	return r;

	while (true);
	api.destroy();
	return r;
}

int main()
{
	bool r = false;

	r = api_dll();
	return 0;
}
