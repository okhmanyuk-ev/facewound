#include "netcode.h"
#include "Core.h"

void CNetcode::UpdateMe(float x, float y, float rot, float xvel, float yvel)
{
	if (!m_bIsMultiplayer) return;

	char ostring[1024];

	if (m_bIsServer)
	{
		// tell everyone own position if active
		sprintf(ostring,"CPM] %i %i %i %+.5f %+.5f %+.5f ", pcore->MyPlayerID, (int)x,(int)y,rot, xvel, yvel);
		SendUDPMessageToEveryone(ostring);
	}
	else
	{
	//	if (m_OKToSendPlayerMovement)
	//	{
		//	m_OKToSendPlayerMovement=false;
			sprintf(ostring,"PM] %i %i %i %+.5f %+.5f %+.5f ", pcore->MyPlayerID, (int)x,(int)y,rot, xvel, yvel);
			SendUDPMessage(ostring, (struct sockaddr *) &m_ServerAddress);
	//	}
	}
}

void CNetcode::SetBulletTime(float bt)
{
	if (!m_bIsMultiplayer) return;

	char ostring[1024];

	if (m_bIsServer)
	{
		sprintf(ostring,"CBT] %f", bt);
		SendUDPMessageToEveryone(ostring);
	}

}

void CNetcode::UpdateMyWeapon(int wepid)
{
	if (!m_bIsMultiplayer) return;

	char ostring[1024];

	if (m_bIsServer)
	{
		// tell everyone own position if active
		sprintf(ostring,"CCW] %i %i ", pcore->MyPlayerID, (int)wepid);
		SendUDPMessageToEveryone(ostring);
	}
	else
	{
		sprintf(ostring,"CW] %i %i ", pcore->MyPlayerID, (int)wepid);
		SendUDPMessage(ostring, (struct sockaddr *) &m_ServerAddress);
	}
}

void CNetcode::ShootWeapon(int wepid, int x, int y, float angle)
{
	if (!m_bIsMultiplayer) return;

	char ostring[1024];

	if (m_bIsServer)
	{
		// tell everyone own position if active
		sprintf(ostring,"CSH] %i %i %i %i %f", pcore->MyPlayerID, (int)wepid, x, y, angle);
		SendUDPMessageToEveryone(ostring);
	}
	else
	{
		sprintf(ostring,"SH] %i %i %i %i %f", pcore->MyPlayerID, (int)wepid, x, y, angle);
		SendUDPMessage(ostring, (struct sockaddr *) &m_ServerAddress);
	}
}

// request a list of current players from the server
int CNetcode::Client_RequestPlayerList(void)
{
	if (!m_bIsMultiplayer) return 0;

	char str[50];
	sprintf(str, "PL] %i ",pcore->players[pcore->MyPlayerID].userid);
	SendUDPMessage("PL] ", (struct sockaddr *) &m_ServerAddress);

	return 0;
}

void CNetcode::ClientReceive()
{
	if (!m_bIsMultiplayer) return;

   timeval timeOut;
   fd_set readMask;



   char szMessage[5012];
   SOCKADDR_IN remoteaddr;
   int iremoteaddrlen = sizeof(remoteaddr);

   int status=1;
   int ret;

   int t_x, t_y;
   float t_aim, t_xvel, t_yvel;
   int pid;
   char buffer[100];
   char buffera[100];
   char bufferb[100];
   char bufferc[100];

   bool receivedpacket=false;

   while (status!=0)
   {
	FD_ZERO( &readMask );
	FD_SET( m_socket, &readMask );
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 1;

	 status = select(0,&readMask,NULL,NULL,&timeOut);

	if (status==SOCKET_ERROR)
	{
			//WriteToLog("<font color=red>Receive failed!</font> (%d), ignoring.<br>", WSAGetLastError());
	}
	else if (status==0)
	{
		//	WriteToLog("Nothing Waiting<br>");
	}
	else
	{
		receivedpacket=true;
	//	WriteToLog("got %i packets Waiting<br>",status);
		ret = recvfrom(m_socket, szMessage, sizeof(szMessage), 0, (struct sockaddr *) &remoteaddr, &iremoteaddrlen);
		if(ret == SOCKET_ERROR)
		{
			WriteToLog("<font color=red>Receive failed!</font> (%d), ignoring.<br>", WSAGetLastError());
		}
		else
		{
		//	WriteToLog("Received: len[%i] ", ret);
			szMessage[ret] = '\0';
		//	WriteToLog(" [%s]<br>",szMessage);

			// proper 
			if (szMessage[3]==']')
			{
// ADD PLAYER (from get player list)
				if (szMessage[1]=='A' && szMessage[2]=='P')
				{
					sscanf(szMessage, "%*s %i %i %i %s %s",&pid, &t_x, &t_y, &buffera, &bufferb);
					WriteToLog("<b>Adding Player</b> p:%i y:%i y:%i <br>", pid, t_x, t_y);

					if (pid!=pcore->MyPlayerID)
					{
						pcore->AddPlayer(remoteaddr, pid);
						sprintf(pcore->players[pid].player.Player_Name, buffera);
						pcore->players[pid].player.m_fX = t_x;
						pcore->players[pid].player.m_fY = t_y;
						pcore->players[pid].player.ChangeModel(bufferb);
					}
				}
// PLAYER MOVE
				else if (szMessage[1]=='P' && szMessage[2]=='M')
				{
					// todo: function to check playerid with connection info
					sscanf (szMessage,"%*s %i %i %i %f %f %f", &pid, &t_x, &t_y, &t_aim, &t_xvel, &t_yvel);

					if (pid!=pcore->MyPlayerID &&
						pcore->players[pid].inuse)
					{
						
						//WriteToLog("Updated user %i's position<br>", pid);
						pcore->players[pid].player.m_fX = (float)t_x;
						pcore->players[pid].player.m_fY = (float)t_y;
						pcore->players[pid].player.m_fXVel = (float)t_xvel;
						pcore->players[pid].player.m_fYVel = (float)t_yvel;
						pcore->players[pid].player.m_AimAngle = (float)t_aim;
					}
				}
// GOT MESSAGE OK (un-needed)
				else if (szMessage[1]=='O' && szMessage[2]=='K') // TELLING US IT RECEIVED THE MESSAGE OK
				{
					// todo: function to check playerid with connection info
					sscanf (szMessage,"%*s %s", buffer);

					if (strcmp(buffer,"PM")==0)
					{
						m_OKToSendPlayerMovement=true;
					}
				}
// CHANGE WEAPON
				else if (szMessage[1]=='C' && szMessage[2]=='W') 
				{
					// todo: function to check playerid with connection info

					int wepid;

					sscanf (szMessage,"%*s %i %i", &pid, &wepid);
					if (pid!=pcore->MyPlayerID &&
						pcore->players[pid].inuse)
					{
						pcore->players[pid].player.m_iWeapon = wepid;
					}
				}
// shoot weapon
				else if (szMessage[1]=='S' && szMessage[2]=='H') 
				{
					// todo: function to check playerid with connection info

					int wepid;
					sscanf (szMessage,"%*s %i %i %i %i %f", &pid, &wepid, &t_x, &t_y, &t_aim);
                    
					if (pid!=pcore->MyPlayerID &&
						pcore->players[pid].inuse)
					{
						pcore->players[pid].player.m_iWeapon = wepid;
						pcore->players[pid].player.m_fX = (float)t_x;
						pcore->players[pid].player.m_fY = (float)t_y;
						pcore->players[pid].player.m_AimAngle = (float)t_aim;
						pcore->players[pid].player.Shoot();
					}
				}
// PLAYER JOINED 
				if (szMessage[1]=='P' && szMessage[2]=='J')
				{
					sscanf(szMessage, "%*s %i",&pid);
					WriteToLog("<b>Player joined:</b> %i<br>", pid);

					if (pid!=pcore->MyPlayerID)
					{
						pcore->AddPlayer(remoteaddr, pid);
						pcore->players[pid].player.m_fX = t_x;
						pcore->players[pid].player.m_fY = t_y;
					}
				}
// CHANGE BULLETTIME
				if (szMessage[1]=='B' && szMessage[2]=='T')
				{
					sscanf(szMessage, "%*s %f",&t_aim);
					if (pid!=pcore->MyPlayerID)
					{
						pcore->timemultiplier = t_aim;
					}
				}
// SERVER SHUTDOWN
				if (szMessage[1]=='0' && szMessage[2]=='0')
				{
					MessageBox(NULL,"The Server Has Shutdown","bye!",0);
					exit(0);
				}
// PLAYER LEAVE
				if (szMessage[1]=='P' && szMessage[2]=='Q')
				{
					sscanf (szMessage,"%*s %i ", &pid);
                        
					if (pcore->players[pid].inuse)
					{
						pcore->players[pid].player.Free();
						pcore->players[pid].inuse = false;
					}
				}
			}
			



			
		}

	}

   }// while

   	if (receivedpacket)
	{
		UpdateMe(pcore->players[pcore->MyPlayerID].player.m_fX,
				 pcore->players[pcore->MyPlayerID].player.m_fY,
				 pcore->players[pcore->MyPlayerID].player.m_AimAngle, 
				 pcore->players[pcore->MyPlayerID].player.m_fXVel ,
				 pcore->players[pcore->MyPlayerID].player.m_fYVel);
		//NetCode_PlayerMoveTimer=0;
	}
}