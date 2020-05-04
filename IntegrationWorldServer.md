# Integration
Suivez les etapes afin d'integrer la classe `CAPIFlyff` dans la solution `WorldServer`. <br>

## Solution
Dans le projet `WorldServer` ajouter les chemin d'inclusions :
```
Flyff-API\Sources\TCPNetwork\Transaction\
Flyff-API\Sources\TCPNetwork\
Flyff-API\Sources\CAPIFlyff\
Flyff-API\Sources\
```

Ajouter les fichiers a votre solutution:
* APIFlyffPacketType.hpp
* APIFlyff.hpp
* APIFlyff.cpp
* APIFlyffMessage.cpp
* APICommand.cpp

## Fichier

### VersionCommion.h
```cpp
#define		__API								//	Service API externe
```

### ThreadMng.cpp
```cpp
#if defined(__API)
#include "APIFlyff.hpp"
#endif //__API
```

Dans la fonction ___BOOL CRunObject::Init(void)___
```cpp
#if defined(__API)
	if (CAPIFlyff::GetInstance().Initialize() == false)
	{
		char szMsgError[300] = { 0, };
		sprintf(szMsgError, "can not initialize API");
		AfxMessageBox(szMsgError);
		return FALSE;
	}
	if (CAPIFlyff::GetInstance().Connect() == false || CAPIFlyff::GetInstance().Authenticate(1234, "admin") == false)
	{
		char szMsgError[300] = { 0, };
		sprintf(szMsgError, "can not connat at APIServer");
		AfxMessageBox(szMsgError);
		return FALSE;
	}
#endif // __API
```

### FuncTextCmd.cpp
```cpp
#if defined(__WORLDSERVER) && defined(__API)
#include "APIFlyff.hpp"
#endif //__WORLDSERVER
```

Dans la function ___int ParsingCommand( LPCTSTR lpszString, CMover* pMover, BOOL bItem )___
```cpp
			else
				g_DPlay.SendChat((LPCSTR)lpszString);
#else	// __CLIENT
			if (pTextCmdFunc->m_nServer == TCM_SERVER || pTextCmdFunc->m_nServer == TCM_BOTH)
				(*pTextCmdFunc->m_pFunc)(scanner);
#if defined(__API)
			if (strcmp(pTextCmdFunc->m_pCommand, "shout") == 0 || strcmp(pTextCmdFunc->m_pAbbreviation, "s") == 0)
				CAPIFlyff::GetInstance().PutChatMessage(1, pMover->GetName(), lpszString);
			else
				CAPIFlyff::GetInstance().PutCommand(pMover->GetName(), lpszString);
#endif //__API
#endif	// __CLIENT
			return TRUE;
```

### DPSrv.cpp
```cpp
#if defined(__API)
#include "APIFlyff.hpp"
#endif //__API
```
Dans la fonction ___void CDPSrvr::OnChat(CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)___
```cpp
		if (sChat[0] == '/' && ParsingCommand(strChat, (CMover*)pUser))
			return;
#if defined(__API)
		CAPIFlyff::GetInstance().PutChatMessage(0, pUser->GetName(), strChat.GetBuffer());
#endif //__API
```

### User.h
Ajouter dans la class `CUserMng` :
```cpp
#if defined(__API)
    void			AddShoutFromDiscord(const char* playerName, const char* message);
#endif //__API
```

### User.cpp
```cpp
#if defined(__API)
void CUserMng::AddShoutFromDiscord(const char* playerName, const char* message)
{
    CAr arBlock;
    arBlock << NULL_ID << SNAPSHOTTYPE_SHOUT;
    arBlock << 1;
    arBlock.WriteString(playerName);
    arBlock.WriteString(message);
    DWORD dwColor = 0xffff99cc;
    arBlock << dwColor;
    GETBLOCK(arBlock, lpBlock, uBlockSize);

    for (map<DWORD, CUser*>::iterator it = m_users.begin(); it != m_users.end(); ++it)
    {
        CUser* pUser = it->second;
        if (pUser->IsValid() == FALSE)
            continue;
        if (pUser->GetWorld() == NULL)
            continue;
        pUser->AddBlock(lpBlock, uBlockSize);
    }
}
#endif //__API
```
