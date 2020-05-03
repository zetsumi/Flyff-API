#pragma once

#include <Windows.h>
#include "APIConnector.hpp"
#include "PacketBuilder.hpp"

class CAPIFlyff
{
	HINSTANCE hDLL = nullptr;
	api_flyff_function	api = api_flyff_function();

	CAPIFlyff() = default;
	bool	SetProcessAdress(void);

public:
	~CAPIFlyff() = default;
	static CAPIFlyff& GetInstance(void);
	static void	onMsg(SOCKET id, PacketStructure* ps);
	static void	onAuthenticate(SOCKET id, PacketBuilder& pb);
	static void	onPutChatMessage(SOCKET id, PacketBuilder& pb);
	static void	onPutCommand(SOCKET id, PacketBuilder& pb);

	bool	Initialize(void);
	bool	Connect(void);
	bool	SendBuffer(PacketBuilder& pb);
	bool	Authenticate(unsigned int key, const char* login);
	void	Destroy(void);
	void	PutChatMessage(unsigned int typeChat, const char* playerName, const char* message);
	void	PutCommand(const char* playerName, const char* command);

	// Command
	static void	PutCommandSys(const char* playername, CScanner& scanner);
};

typedef void	(*cmdAPI)(const char* playername, CScanner& scanner);
