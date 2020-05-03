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
#include "UserMng.h"
#include "DPCoreClient.h"

extern	CDPCoreClient		g_DPCoreClient;
extern	CUserMng			g_UserMng;


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
		OUTPUTDEBUGSTRING("%s: size<%u> type<%u>", __FUNCTION__, size, type);
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
	OUTPUTDEBUGSTRING("%s:", __FUNCTION__);
}

void CAPIFlyff::onPutChatMessage(SOCKET id, PacketBuilder& pb)
{
	unsigned int chat = pb.read<unsigned int>();
	const char* playerName = pb.readString();
	const char* message = pb.readString();
	OUTPUTDEBUGSTRING("%s: chat[%u] player[%s] message[%s]", __FUNCTION__, chat, playerName, message);

	g_UserMng.AddShoutFromDiscord(playerName, message);
}

void CAPIFlyff::onPutCommand(SOCKET id, PacketBuilder& pb)
{
	const char* playerName = pb.readString();
	const char* command = pb.readString();

	OUTPUTDEBUGSTRING("%s: name[%s] command[%s]", __FUNCTION__, playerName, command);

	CScanner scanner;
	scanner.SetProg((LPTSTR)command);
	scanner.dwValue = NULL_ID;

	scanner.GetToken(); // skip the '!'
	scanner.GetToken(); // take the cmd
	char cmd[200] = {0,};
	strcpy(cmd, scanner.token);

	char msg[1024] = {0,};
	unsigned int index = 0;
	while (scanner.tok != FINISHED) {
		scanner.GetToken();
		char* txt = scanner.token;
		::memcpy_s(msg + index, 1024, txt, strlen(txt));
		index += strlen(txt);
		if (scanner.tok != FINISHED) {
			msg[index] = ' ';
			++index;
		}
		msg[index] = '\0';
		OUTPUTDEBUGSTRING("%s: %s", __FUNCTION__, msg);
	}
	OUTPUTDEBUGSTRING("%s: cmd[%s] msg[%s]", __FUNCTION__, cmd, msg);

	g_DPCoreClient.SendSystem(msg);

}