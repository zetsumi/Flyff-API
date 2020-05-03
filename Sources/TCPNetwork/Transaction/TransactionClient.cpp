#include "pch_tcp_network.h"
#include "Transaction.hpp"

bool Transaction::runClient()
{
	try
	{
		auto fct = std::bind(&Transaction::loopReceive, this, this->_socket->getSocket());
		std::thread t(fct);
		t.detach();
		runningServer = true;
	}
	catch (const std::exception& e)
	{
		LOGCONSOLE("error<%s>", e.what());
		return false;
	}
	return true;
}
