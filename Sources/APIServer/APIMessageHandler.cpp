#pragma warning( disable: 4251 )

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Logger.hpp"

#include "APIMessageHandler.hpp"
#include "ClientAPI.hpp"

extern ClientAPI	g_bots;
extern ClientAPI	g_servers;
extern ClientAPI	g_externs;

APIMessageHandler& APIMessageHandler::instance()
{
	static APIMessageHandler api;
	return std::ref(api);
}

void	APIMessageHandler::onMsg(SOCKET id, PacketStructure* ps)
{
	if (isInValid(ps))
		return;
	PacketBuilder pb;
	pb.setPacket(ps);
	while (pb.getOffset() < ps->size)
	{
		unsigned int size = pb.read<unsigned int>();
		API_PACKET_TYPE type = (API_PACKET_TYPE)pb.read<unsigned int>();
		LOGCONSOLE("size[%u] type[%u]", size, type);
		switch (type)
		{
		case API_PACKET_TYPE::AUTHENTICATE: onAuthenticate(id, pb); break;
		case API_PACKET_TYPE::PUT_CHAT_MESSAGE: onPutChatMessage(id, pb); break;
		case API_PACKET_TYPE::PUT_COMMAND: onPutCommand(id, pb); break;
		default: LOG("PacketType<%u> unknow", (unsigned int)type); return;
		}
	}
}

void APIMessageHandler::onAuthenticate(SOCKET id, PacketBuilder& pb)
{
	unsigned int key = pb.read<unsigned int>();
	API_FLYFF_AUTHENTICATE auth = (API_FLYFF_AUTHENTICATE)pb.read<unsigned int>();
	const char* login = pb.readString();

	LOGCONSOLE("key[%u] author[%u] login[%s]", key, auth, login);

	switch (auth)
	{
	case API_FLYFF_AUTHENTICATE::SERVER_FLYFF:
		LOGCONSOLE("new Server Flyff");
		g_servers.add(id);
		break;
	case API_FLYFF_AUTHENTICATE::BOT_DISCORD:
		LOGCONSOLE("new BOT DISCORD");
		g_bots.add(id);
		break;
	case API_FLYFF_AUTHENTICATE::CLIENT_EXTERN:
		LOGCONSOLE("new Client Extern");
		g_externs.add(id);
		break;
	default:
		break;
	}
}

void APIMessageHandler::onPutChatMessage(SOCKET id, PacketBuilder& pb)
{
	unsigned int chat = pb.read<unsigned int>();
	const char* playerName = pb.readString();
	const char* message = pb.readString();
	LOGCONSOLE("chat[%u] player[%s] message[%s]", chat, playerName, message);

	PacketBuilder pbb;
	pbb.write<unsigned int>((unsigned int)API_PACKET_TYPE::PUT_CHAT_MESSAGE);
	pbb.write<unsigned int>(chat);
	pbb.writeString(playerName);
	pbb.writeString(message);
	const unsigned char* buffer = pbb.getData();
	unsigned int length = pbb.getSize();

	if (g_bots.is(id) == false)
		g_bots.sendAll(length, buffer);
	if (g_servers.is(id) == false)
		g_servers.sendAll(length, buffer);
}

void APIMessageHandler::onPutCommand(SOCKET id, PacketBuilder& pb)
{
	const char* playerName = pb.readString();
	const char* command = pb.readString();
	LOGCONSOLE("player[%s] command[%s]", playerName, command);

	PacketBuilder pbb;
	pbb.write<unsigned int>((unsigned int)API_PACKET_TYPE::PUT_COMMAND);
	pbb.writeString(playerName);
	pbb.writeString(command);
	const unsigned char* buffer = pbb.getData();
	unsigned int length = pbb.getSize();

	// send bots
	if (g_bots.is(id) == false)
		g_bots.sendAll(length, buffer);

	// send server clients
	if (g_servers.is(id) == false)
		g_servers.sendAll(length, buffer);

	// send client externs
	if (g_externs.is(id) == false)
		g_externs.sendAll(length, buffer);
}
