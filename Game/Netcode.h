#pragma once

#include <winsock.h>
#include <windows.h>
#include <stdio.h>



#define DEFAULTPORT 2088
#define DEFAULTIP "62.3.225.150"

class CCore;

class CNetcode
{
public:
	CNetcode(void);
	~CNetcode(void);
	void WriteToLog(char *lpszText, ...);
	bool InitNetCode_Server(int sport);
	bool InitNetCode_Client(LPSTR sip, int sport);

	bool bGotPlayerList;

	float ServerGlobalTick;

	bool m_bIsServer;	
	bool m_bIsMultiplayer;

	CCore* pcore;

	SOCKET m_socket; // our socket
	SOCKADDR_IN m_ServerAddress;  // host 
	bool b_EnableLogging;
	unsigned int m_iNumClients; // number of clients currently connected

	void ServerReceive(void);
	void ClientReceive(void);
	void UpdateMe(float x, float y, float rot, float xvel, float yvel);
	void SendUDPMessage(char* message, const struct sockaddr FAR *to);
	void SetBulletTime(float bt);
	void SendUDPMessageToEveryone(char* message);
	int AddClient(char* joinmsg, SOCKADDR_IN remoteaddr);
	int Client_RequestPlayerList(void);
	bool m_OKToSendPlayerMovement;

	void UpdateMyWeapon(int wepid);
	void ShootWeapon(int wepid, int x, int y, float angle);

	HANDLE LogMutex;
	void Do(void);
	void SendQuit(void);
	bool InitNetCode_SinglePlayer(void);
};
