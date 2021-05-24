#pragma once

#if defined(__API)
#include <Windows.h>
#include "APIConnector.hpp"
#include "PacketBuilder.hpp"

class CAPIFlyff
{
	HINSTANCE hDLL = nullptr;
	api_flyff_function	api = api_flyff_function();
	bool	connected = false;

	CAPIFlyff() = default;
	bool	SetProcessAdress(void);

public:
	~CAPIFlyff() = default;
	static CAPIFlyff& GetInstance(void);
	static void	onMsg(SOCKET id, PacketStructure* ps);
	static void	onAuthenticate(SOCKET id, PacketBuilder& pb);
	static void	onPutChatMessage(SOCKET id, PacketBuilder& pb);
	static void	onPutCommand(SOCKET id, PacketBuilder& pb);

	bool	IsConnected(void);

	bool	Initialize(void);
	bool	Connect(void);
	bool	Authenticate(unsigned int key, const char* login);
	void	Destroy(void);
	void	PutChatMessage(unsigned int typeChat, const char* playerName, const char* message);
	void	PutCommand(const char* playerName, const char* command);
	bool	SendBuffer(PacketBuilder& pb);

	// Command
	static void	PutCommandSys(const char* playername, CScanner& scanner);
};

typedef void	(*cmdAPI)(const char* playername, CScanner& scanner);
#endif //__API
