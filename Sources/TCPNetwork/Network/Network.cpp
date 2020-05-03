#include "pch_tcp_network.h"
#include "Network.hpp"

bool Network::isValid(void) const
{
	if (ip.empty() == true)
		return false;
	if (port == 0)
		return false;
	return true;
}

bool Network::setIP(const std::string& newIP)
{
	if (newIP.empty() == true)
		return false;
	ip.assign(newIP);
	return true;
}

bool Network::setIP(const char* newIP)
{
	if (newIP == nullptr)
		return false;
	ip = newIP;
	return true;
}

void	Network::setPort(unsigned int newPort)
{
	port = newPort;
}