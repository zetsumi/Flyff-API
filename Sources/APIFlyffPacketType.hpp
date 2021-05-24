#pragma once

#if !defined(__WORLDSERVER) || defined(__API)
enum class API_FLYFF_AUTHENTICATE : unsigned int
{
	SERVER_FLYFF,
	BOT_DISCORD,
	CLIENT_EXTERN
};

enum class API_PACKET_TYPE : unsigned int
{
	SEND_BUFFER = 0x01,
	AUTHENTICATE,
	PUT_CHAT_MESSAGE,
	PUT_COMMAND,
};
#endif //__API