#include "stdafx.h"

#if defined(__API)
#include "APIFlyff.hpp"

#include "User.h"
#include "UserMacro.h"
#include "DPCoreClient.h"
#include "dpdatabaseclient.h"
#include "dpSrvr.h"
#include "eveschool.h"
#include "WorldDialog.h"
#include "ItemUpgrade.h"
#include "DPCoreClient.h"

extern	CDPCoreClient		g_DPCoreClient;
extern	CUserMng			g_UserMng;

void CAPIFlyff::PutCommandSys(const char* playername, CScanner& scanner)
{
	char msg[1024] = { 0, };
	unsigned int index = 0;
	while (scanner.tok != FINISHED)
	{
		scanner.GetToken();
		char* txt = scanner.token;
		::memcpy_s(msg + index, 1024, txt, strlen(txt));
		index += strlen(txt);
		if (scanner.tok != FINISHED)
		{
			msg[index] = ' ';
			++index;
		}
		msg[index] = '\0';
	}

	g_DPCoreClient.SendSystem(msg);
}

#endif //__API