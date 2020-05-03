# Flyff-API

Projet permetant de communiquer avec le serveur via des clients extern telque les `bots discord`. <br>
Le projet est constituer de solutions `TCPNetwork`, `APIConnector`, `APIServer`, `APIClient`. <br>


## TCPNetwork
Bibliotheque permettant d'effectuer une connexion et d'envoyer des messages via le protocol `TCP`. <br>
Language : `C++ 14`. <br>

## APIConnection
Bibliotheque dynamique, wrapper permettant de creer un client/server de l'API. <br>
Les fonction primaire sont defie dans cette bibliotheques tel que l'authentification... <br>

## APIServer
Executable repressentant le serveur Back-End. Il fait relier entre les serveur `Flyff`, `Bot Discord` et les `Clients Externes`. <br>

## APIClient
Executable permettant de tester les echanges entre les differentes partie --> SANDBOX.


# Utilisation

## Packet Structure

Les packets sont structuree de cette maniere :
| instruction   | octects   | type          |
| :--------:    | :-----:   | :----:        |
| LENGTH        | 4         | unsigned int  |
| PACKET TYPE   | 4         | unsigned int  |
| DATAS         | X         | X             |

## Server

### Initialisation
La creation d'un serveur `API` s'effectue via la function `createServer`, cette fonction retourne une structure `SFactoryServer`. <br>
Il faut indiquer a `createServer` le port.
La destruction du serveur `API` s'effectue via `destroyServer`, cette fonction permet de couper tous les connection TCP ouverte proprement. <br>
La gestion du pointeur serveur passe via les fonction `SFactory` fichier TFactory.hpp. <br>

### Informations
Un serveur peut donc communiquer avec des clients dont la liste est define dans `APIMessageHandler.hpp`.<br>
Lors que le serveur recoit un packet il renvoie l'informations au autres clients de type differents.
Exemple :
Si le serveur recoit un packet du client `BOT` lors il renvoie les informations aux autres clients differents de `BOTS`.

## Instruction
1) Creation d'un pointeur `SFactoryServer`.
2) Definir la `callback` pout la reception des paquets. 
3) Authentification

## Client

### Initialisation
La creation d'un client `API` s'effectue via la function `createClient`, cette fonction retourne une structure `SFactoryClient`. <br>
Le faut indiquer a `createClient` le port et l'ip du serveur.
La destruction du serveur `API` s'effectue via `destroyClient`, cette fonction permet de couper tous les connection TCP ouverte proprement. <br>
La gestion du pointeur serveur passe via les fonction `SFactory` fichier TFactory.hpp. <br>

## Information
Un client doit obligatoirement s'authentifier afin de definir sont `type` la liste est presente dans le fichier `APIMessageHandler.hpp`. <br>

## Instruction
1) Charger la DLL `APIConnector`
2) Creation d'un pointeur `SFactoryClient`.
3) Definie la `callback` pour de la reception des paquets.
3) Authentification


## Bot Discord

## Initialisation
Modifier les fichier `config.json` afin qu'il correspond a votre ID discord.


# Structure du projet
![](UML.png)


# Integration WorldServer
Dans le projet `WorldServer` ajouter les chemin d'inclusions :
```
Flyff-API\Sources\TCPNetwork\Packet\
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

Dans le fichier `ThreadMng.cpp` <bv>
Ajouter
```cpp
#if defined(__API)
#include "APIFlyff.hpp"
#endif //__API
```
Dans la fonction `BOOL CRunObject::Init(void)`
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
