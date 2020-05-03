#include "pch_tcp_network.h"
#include "Logger.hpp"
#include "PacketBuilder.hpp"


void	PacketBuilder::debug(void) const
{
	LOGCONSOLE("============");
	LOGCONSOLE("size[%u]", packet->size);
	for (unsigned int i = 0; i < packet->size; ++i)
	{
		LOGCONSOLE("data[%u] = [%d] = 0x{%#02x}",
			i, packet->data[i], packet->data[i]);
	}
	LOGCONSOLE("============");
}

const unsigned char* PacketBuilder::getData(bool builder) const
{
	if (packet == nullptr)
		return nullptr;
	if (packet->size == 0)
		return nullptr;
	if (builder == false)
		return packet->data;
	unsigned char* tmp = new unsigned char[packet->size + sizeof(unsigned int)]();
	::memcpy_s(tmp, sizeof(unsigned int), &packet->size, sizeof(unsigned int));
	::memcpy_s(tmp + sizeof(unsigned int), packet->size, packet->data, packet->size);
	delete packet->data;
	packet->data = nullptr;
	packet->size += sizeof(unsigned int);
	packet->data = tmp;
	return packet->data;
}

unsigned int PacketBuilder::getSize(void) const
{
	if (packet == nullptr)
		return 0;
	return packet->size;
}

void	PacketBuilder::setPacket(PacketStructure* ps)
{
	if (ps != nullptr)
		packet = ps;
}

void	PacketBuilder::writeString(const char* var)
{
	unsigned int length = ::strlen(var);
	writeString(var, length);
}

void	PacketBuilder::writeString(const char* var, unsigned int length)
{
	write<unsigned int>(length);
	packet->data = (unsigned char*)::realloc(packet->data, packet->size + length);
	if (packet->data == nullptr)
		return;
	::memcpy_s(packet->data + packet->size, length, var, length);
	packet->size += length;
}

const char* PacketBuilder::readString(void)
{
	unsigned int length = read<unsigned int>();
	unsigned char* cur = packet->data + offset;
	char* var = new char[length + 1]();
	::memcpy_s(var, length, cur, length);
	var[length] = '\0';
	offset += length;

	LOGCONSOLE("length:[%u] string[%s]", length, var);

	return var;
}

unsigned int	PacketBuilder::getOffset(void) const
{
	return offset;
}