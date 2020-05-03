#pragma once

#include <thread>
#include <functional>

#include "framework.hpp"
#include "SocketClient.hpp"
#include "SocketServer.hpp"
#include "PacketBuilder.hpp"
#include "MessageHandler.hpp"

#define	DEFAULT_BUFFER_SIZE_RECEIVE 1000

enum class MODE_TRANSACTION : unsigned int
{
	MODE_UNKNOW,
	MODE_SERVER,
	MODE_CLIENT
};

class API_DECLSPEC Transaction
{
	std::thread	main;
	Socket* _socket = nullptr;
	MODE_TRANSACTION	mode = MODE_TRANSACTION::MODE_UNKNOW;
	callbackOnMessage	onPacketMessage = nullptr;

	// server variables
	std::mutex	queueLocker;
	std::thread	threadAccept;
	bool	runningServer = false;

	// server functions
	bool	runServer(void);
	void	loopAccept(void);
	void	loopReceive(SOCKET idClient);

	// client functions
	bool	runClient(void);

public:
	Transaction() = default;
	~Transaction() = default;

	// global
	bool	setSocket(Socket* s);
	void	setMode(MODE_TRANSACTION modeTransaction);
	bool	isMode(MODE_TRANSACTION modeTransaction);
	bool	run(callbackOnMessage callback);

	// emiter
	bool	sender(SOCKET idSocket, unsigned int size, const char* data);
	bool	sender(SOCKET idSocket, PacketBuilder& packet);
	PacketStructure*	receiver(SOCKET idSocket);
	PacketStructure*	receiver(SOCKET idSocket, unsigned int bufferSize);

	// server
	bool	accept(void);
	void	wait(void);
};