#include "pch_tcp_network.h"
#include "Transaction.hpp"

bool Transaction::setSocket(Socket* s)
{
	if (s == nullptr)
		return false;
	_socket = s;
	return true;
}

void Transaction::setMode(MODE_TRANSACTION modeTransaction)
{
	mode = modeTransaction;
}

bool Transaction::isMode(MODE_TRANSACTION modeTransaction)
{
	return mode == modeTransaction;
}

bool Transaction::run(callbackOnMessage callback)
{
	if (_socket == nullptr)
		return false;
	if (_socket->getSocket() == INVALID_SOCKET)
		return false;
	if (mode == MODE_TRANSACTION::MODE_UNKNOW)
		return false;
	this->onPacketMessage = callback;
	if (mode == MODE_TRANSACTION::MODE_SERVER)
		return runServer();
	if (mode == MODE_TRANSACTION::MODE_CLIENT)
		return runClient();
	return true;
}
