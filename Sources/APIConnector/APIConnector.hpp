#pragma once

#include <Windows.h>
#include "APIFlyffPacketType.hpp"
#include "PacketStructure.hpp"
#include "MessageHandler.hpp"

#if defined(_DEBUG)
#define	API_FLYFF_DLL_NAME	"APIConnectord.dll"
#else
#define	API_FLYFF_DLL_NAME	"APIConnector.dll"
#endif //_DEBUG

typedef bool	(__cdecl* API_INIT_FLYFF)(unsigned int port, const char* ip, callbackOnMessage callback);
typedef void	(__cdecl* API_DESTROY_FLYFF)(void);
typedef bool	(__cdecl* API_AUTHENTICATE_FLYFF)(unsigned int key, unsigned int author, const char* login);
typedef bool	(__cdecl* API_PUT_CHAT_MESSAGE_FLYFF)(unsigned int typeChat, const char* playerName, const char* message);
typedef bool	(__cdecl* API_PUT_COMMAND_FLYFF)(const char* playerName, const char* command);
typedef bool	(__cdecl* API_SEND)(unsigned int length, const unsigned char* buffer);

#if defined(APICONNECTOR_EXPORTS)
# define API_FLYFF_DECLSPEC __declspec(dllexport)
#else
# define API_FLYFF_DECLSPEC __declspec(dllimport)
#endif //APICONNECTOR_EXPORTS

struct API_FLYFF_DECLSPEC api_flyff_function
{
	API_INIT_FLYFF				initialize;
	API_DESTROY_FLYFF			destroy;
	API_AUTHENTICATE_FLYFF		authenticate;
	API_PUT_CHAT_MESSAGE_FLYFF	putChatMessage;
	API_PUT_COMMAND_FLYFF		putCommand;
	API_SEND					send;

	api_flyff_function();
	api_flyff_function(api_flyff_function* api);
	api_flyff_function(const api_flyff_function& api);
	~api_flyff_function();
	api_flyff_function& operator=(const api_flyff_function& api);
};