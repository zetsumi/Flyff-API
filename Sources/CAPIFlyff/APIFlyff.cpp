#include "stdafx.h"
#include "APIFlyff.hpp"

std::map<char*, cmdAPI>	cmds;


bool CAPIFlyff::SetProcessAdress(void)
{
	if (hDLL == nullptr)
		return false;

	api.initialize = (API_INIT_FLYFF)GetProcAddress(hDLL, "api_initialize");
	api.destroy = (API_DESTROY_FLYFF)GetProcAddress(hDLL, "api_destroy");
	api.authenticate = (API_AUTHENTICATE_FLYFF)GetProcAddress(hDLL, "api_authenticate");
	api.putChatMessage = (API_PUT_CHAT_MESSAGE_FLYFF)GetProcAddress(hDLL, "api_put_chat_message");
	api.putCommand = (API_PUT_COMMAND_FLYFF)GetProcAddress(hDLL, "api_put_command");
	api.send = (API_SEND)GetProcAddress(hDLL, "api_send");

	cmds["sys"] = &CAPIFlyff::PutCommandSys;

	return true;
}

CAPIFlyff& CAPIFlyff::GetInstance(void)
{
	static CAPIFlyff instance;
	return std::ref(instance);
}

bool	CAPIFlyff::Initialize(void)
{
	hDLL = LoadLibraryW(L"APIConnectord.dll");
	if (hDLL == nullptr)
		return false;
	if (SetProcessAdress() == false)
		return false;
	return true;
}

bool CAPIFlyff::Connect()
{
	if (api.initialize == nullptr)
		return false;
	if (api.initialize(27015, "127.0.0.1", &CAPIFlyff::onMsg) == false)
		return false;
	return true;
}

bool CAPIFlyff::SendBuffer(PacketBuilder& pb)
{
	if (api.send == nullptr)
		return false;
	unsigned int length = pb.getSize();
	const unsigned char* buffer = pb.getData();
	if (api.send(length, buffer) == false)
		return false;
	return true;
}

bool CAPIFlyff::Authenticate(unsigned int key, const char* login)
{
	if (api.authenticate == nullptr)
		return false;
	if (api.authenticate(key, (unsigned int)API_FLYFF_AUTHENTICATE::SERVER_FLYFF, login) == false)
		return false;
	return true;
}

void CAPIFlyff::Destroy()
{
	if (api.destroy == nullptr)
		return;
	api.destroy();
}

void CAPIFlyff::PutChatMessage(unsigned int typeChat, const char* playerName, const char* message)
{
	if (typeChat > 2)
		return;
	if (playerName == nullptr)
		return;
	if (message == nullptr)
		return;
	if (api.putChatMessage == nullptr)
		return;
	api.putChatMessage(typeChat, playerName, message);
}

void CAPIFlyff::PutCommand(const char* playerName, const char* command)
{
	if (playerName == nullptr)
		return;
	if (command == nullptr)
		return;
	if (api.putCommand == nullptr)
		return;
	api.putCommand(playerName, command);
}

