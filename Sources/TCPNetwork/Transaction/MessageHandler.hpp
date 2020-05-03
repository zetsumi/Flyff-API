#pragma once

#include <winsock.h>
#include "PacketStructure.hpp"

typedef void	(*callbackOnMessage)(SOCKET id, PacketStructure* ps);
