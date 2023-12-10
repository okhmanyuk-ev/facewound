#include "netcode.h"

#include "core.h"

CNetcode::CNetcode(void)
{
	m_OKToSendPlayerMovement=true;
	m_bIsMultiplayer=true;
}

CNetcode::~CNetcode(void)
{
}

bool CNetcode::InitNetCode_Server(int sport)
{
	LogMutex = CreateMutex(NULL, false, NULL);

	m_bIsServer = true;
	WriteToLog("<b>SREVER</b><br><br>");
	WSADATA wsda;
	WSAStartup(MAKEWORD(1,1), &wsda);  

	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_socket == SOCKET_ERROR) {WriteToLog("Failed to create socket!");exit(3);}
  
	if (sport==0)
		sport = DEFAULTPORT;
	

	// bind local socket
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_port = htons(sport);
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;

	// this adds us
	pcore->MyPlayerID = pcore->AddPlayer( m_ServerAddress );
	WriteToLog("(my player id is %i)<br><br>", pcore->MyPlayerID);
	pcore->players[(pcore->MyPlayerID)].player.bIsPlayer = true;
	pcore->players[(pcore->MyPlayerID)].player.SetPlayerName(pcore->player_Name);

    if(bind(m_socket, (struct sockaddr *) &m_ServerAddress, sizeof(m_ServerAddress)) == SOCKET_ERROR)
       {
		   WriteToLog("Failed to bind socket! error:(%d).<br>", WSAGetLastError());
		   exit(4);
	   }

   WriteToLog("<br><b>Server Started - Listening for connections</b><br><br>");
   
   // server thread
  // CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ServerReceive, this, 0, NULL );

   return true;
}

bool CNetcode::InitNetCode_Client(LPSTR sip, int sport)
{
	LogMutex = CreateMutex(NULL, false, NULL);

	m_bIsServer = false;
	WriteToLog("<b>CLIENT</b><br><br>");

	char szAddress[100];
	sprintf(szAddress,sip);



	if (sport==0)
	{
		sport = DEFAULTPORT;
	}


	WSADATA wsda;
	WSAStartup(MAKEWORD(1,1), &wsda);


	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_socket == SOCKET_ERROR) {WriteToLog("Failed to create socket! error:(%d).<br>", WSAGetLastError());MessageBox(NULL,"Couldnt Create scoket... I dunno why", "oh crap",0);exit(3);}
  
	// bind local socket
	WriteToLog("port: %i<br>address: %s<br><br>", sport, szAddress);
    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_port = htons(sport);
    m_ServerAddress.sin_addr.s_addr = inet_addr(szAddress);

  struct hostent *host;
  if(m_ServerAddress.sin_addr.s_addr == INADDR_NONE) // The address wasn't in numeric form
   { 
	   WriteToLog("Address isnt an IP, looking up.<br>");
      host = NULL;
      host = gethostbyname(szAddress); // Get the IP address of the server
      
	  if(host == NULL)
      {
        WriteToLog("Couldnt resolve the host! (%s)",szAddress);
         exit(6);
      }

      memcpy(&m_ServerAddress.sin_addr, host->h_addr_list[0], host->h_length);
      WriteToLog("Looked up server address fine!<br>");
   }

   SOCKADDR_IN remoteaddr;
   int iremoteaddrlen = sizeof(remoteaddr);
   char szMessage[512];
   sprintf(szMessage, "");
   int ret;
   int imessagelen = strlen(szMessage);

   char szName[200];
   char szModel[200];

   for (int i=0;i<strlen(pcore->player_Name)&&i<25;i++)
   {
	   if (pcore->player_Name[i]==' ')
		szName[i] = '_';
	   else 
		szName[i] = pcore->player_Name[i];

	   szName[i+1] = '\0';

   }

   for (int i=0;i<strlen(pcore->player_Model)&&i<25;i++)
   {
	   if (pcore->player_Model[i]==' ')
		szModel[i] = '_';
	   else 
		szModel[i] = pcore->player_Model[i];

	   szModel[i+1] = '\0';
   }

   // DONE, GET CONNECTED
   WriteToLog("Asking if I can join...<br>");
   sprintf(szMessage, "JN] %s %s", szName, szModel);
   SendUDPMessage(szMessage, (struct sockaddr *) &m_ServerAddress);
   WriteToLog("Waiting for reply...<br>"); // todo: make this time out
   ret = recvfrom(m_socket, szMessage, sizeof(szMessage), 0, (struct sockaddr *) &remoteaddr, &iremoteaddrlen);
	if(ret == SOCKET_ERROR)
	{
		MessageBox(NULL,"Couldnt Connect... I dunno why", "oh crap",0);
		WriteToLog("<font color=red>Receive failed!</font> DIDNT CONNECT! error:(%d).<br>", WSAGetLastError());
		exit(5);
	}
	else
	{
		WriteToLog("Received: <b>len[%i]</b> ", ret);
		szMessage[ret] = '\0';
		WriteToLog(" [%s]<br>",szMessage);
		// command
		if (szMessage[3] == ']')
		{
			if (szMessage[1] == 'W' && szMessage[2] == 'C') // welcome
			{
				int userid;
				sscanf(szMessage,"%*s %i ",&userid);
				WriteToLog("Setting my userid to [%i] <br>", userid);

				pcore->AddPlayer(m_ServerAddress, userid);
				pcore->MyPlayerID = userid;
				pcore->players[userid].player.bIsPlayer = true;
				pcore->players[userid].player.SetPlayerName(pcore->player_Name);

			//	bGotPlayerList=false;
				this->Client_RequestPlayerList();
			}
		}
		else
		{
			MessageBox(NULL,"Couldnt Connect... Wrong message Received..", "oh crap",0);
			WriteToLog("<font color=red>Connect failed!</font> DIDNT CONNECT!");
			exit(5);
		}
		


		
	}

//	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ClientReceive, this, 0, NULL );
//	Sleep(100);

	return true;
}


void CNetcode::WriteToLog(char *lpszText, ...)
{
	WaitForSingleObject(LogMutex, INFINITE);
	if(b_EnableLogging)
	{
		va_list argList;
		FILE *pFile;
		va_start(argList, lpszText);
		pFile = fopen("logs/net.html", "a+");
		vfprintf(pFile, lpszText, argList);
		putc('\n', pFile);
		fclose(pFile);
		va_end(argList);
	}
	ReleaseMutex(LogMutex);
}

void CNetcode::SendUDPMessage(char* message, const struct sockaddr FAR *to)
{
	//if (m_bIsServer)
		//WriteToLog("m->'%s' (%i)<br>", message, strlen(message));
   int ret = sendto(m_socket, message, strlen(message), 0, to, sizeof(*to));
   if(ret == SOCKET_ERROR)
	{
		WriteToLog("<br><font color=red>Send failed! (%i), ignoring. </font><br>", WSAGetLastError());
		exit(5);
	}
}

void CNetcode::SendUDPMessageToEveryone(char* message)
{
	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if ((pcore->players[i].inuse) && (i!=pcore->MyPlayerID))
		{
			SendUDPMessage(message,(struct sockaddr *) &(pcore->players[i].addr));
		}
	}
}

void CNetcode::Do(void)
{
	if (m_bIsServer)
		this->ServerReceive();
	else
		this->ClientReceive();

}

void CNetcode::SendQuit(void)
{
	if (!m_bIsMultiplayer) return;

	if (m_bIsServer)
	{
		SendUDPMessageToEveryone("C00] "); // c00 is quit
	}
	else
	{
		char ostring[1024];
		sprintf(ostring,"PQ] %i ", pcore->MyPlayerID);
		SendUDPMessage(ostring, (struct sockaddr *) &m_ServerAddress);
	}


}

bool CNetcode::InitNetCode_SinglePlayer(void)
{
	LogMutex = CreateMutex(NULL, false, NULL);
	m_bIsServer = true;
	m_bIsMultiplayer = false;
	WriteToLog("<b>SinglePlayer</b><br><br>");	

    m_ServerAddress.sin_family = AF_INET;
    m_ServerAddress.sin_addr.s_addr = INADDR_ANY;

	// this adds us
	pcore->MyPlayerID = pcore->AddPlayer( m_ServerAddress );
	WriteToLog("(my player id is %i)<br><br>", pcore->MyPlayerID);
	pcore->players[(pcore->MyPlayerID)].player.bIsPlayer = true;
	pcore->players[(pcore->MyPlayerID)].player.SetPlayerName(pcore->player_Name);

    WriteToLog("<br><b>Finished setting up netcode (faked for singleplayer)</b><br><br>");

	return true;
}
