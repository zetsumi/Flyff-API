#pragma once

#include <vector>
#include "TCPNetwork.hpp"
#include "APIFlyffPacketType.hpp"

class APIMessageHandler
{
	APIMessageHandler() = default;

public:
	~APIMessageHandler() = default;
	static APIMessageHandler& instance(void);
	static void	onMsg(SOCKET id, PacketStructure* ps);
	static void	onAuthenticate(SOCKET id, PacketBuilder& pb);
	static void	onPutChatMessage(SOCKET id, PacketBuilder& pb);
	static void	onPutCommand(SOCKET id, PacketBuilder& pb);
};
