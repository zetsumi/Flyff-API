#include "stdafx.h"
#include "APIFlyff.hpp"

#include "User.h"
#include "UserMacro.h"
#include "DPCoreClient.h"
#include "dpdatabaseclient.h"
#include "dpSrvr.h"
#include "eveschool.h"
#include "WorldDialog.h"
#include "ItemUpgrade.h"
#include "DPCoreClient.h"

extern	CDPCoreClient		g_DPCoreClient;
extern	CUserMng			g_UserMng;

extern std::map<char*, cmdAPI>	cmds;


void CAPIFlyff::onMsg(SOCKET id, PacketStructure* ps)
{
	if (isInValid(ps))
		return;
	PacketBuilder pb;
	pb.setPacket(ps);
	while (pb.getOffset() < ps->size)
	{
		unsigned int size = pb.read<unsigned int>();
		API_PACKET_TYPE type = (API_PACKET_TYPE)pb.read<unsigned int>();
		switch (type)
		{
		case API_PACKET_TYPE::AUTHENTICATE: onAuthenticate(id, pb); break;
		case API_PACKET_TYPE::PUT_CHAT_MESSAGE: onPutChatMessage(id, pb); break;
		case API_PACKET_TYPE::PUT_COMMAND: onPutCommand(id, pb); break;
		default: break; return;
		}
	}
}

void CAPIFlyff::onAuthenticate(SOCKET id, PacketBuilder& pb)
{
}

void CAPIFlyff::onPutChatMessage(SOCKET id, PacketBuilder& pb)
{
	unsigned int chat = pb.read<unsigned int>();
	const char* playerName = pb.readString();
	const char* message = pb.readString();

	g_UserMng.AddShoutFromDiscord(playerName, message);
}

void CAPIFlyff::onPutCommand(SOCKET id, PacketBuilder& pb)
{
	const char* playerName = pb.readString();
	const char* command = pb.readString();

	CScanner scanner;
	scanner.SetProg((LPTSTR)command);
	scanner.dwValue = NULL_ID;

	scanner.GetToken(); // skip the '!'
	scanner.GetToken(); // take the cmd

	char cmd[200] = {0,};
	strcpy(cmd, scanner.token);

	for (auto it : cmds)
	{
		if (::strcmp(it.first, cmd) == 0)
		{
			it.second(playerName, scanner);
		}
	}
}