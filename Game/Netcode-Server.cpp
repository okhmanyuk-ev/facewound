#include "netcode.h"
#include "Core.h"

void CNetcode::ServerReceive()
{
	if (!m_bIsMultiplayer) return;

   ServerGlobalTick = ServerGlobalTick - pcore->gfx.RealTick;

   	if (ServerGlobalTick>0) return;


	UpdateMe(pcore->players[pcore->MyPlayerID].player.m_fX,
				 pcore->players[pcore->MyPlayerID].player.m_fY,
				 pcore->players[pcore->MyPlayerID].player.m_AimAngle, 
				 pcore->players[pcore->MyPlayerID].player.m_fXVel ,
				 pcore->players[pcore->MyPlayerID].player.m_fYVel);

	ServerGlobalTick=5;

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
   char sparestringa[500];
   char sparestringb[500];
   char sparestringc[500];


   while (status!=0)
   {
	FD_ZERO( &readMask );
	FD_SET( m_socket, &readMask );
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 1;

	 status = select(0,&readMask,NULL,NULL,&timeOut);

	if (status==SOCKET_ERROR)
	{
			WriteToLog("<font color=red>Receive failed!</font> (%d), ignoring.<br>", WSAGetLastError());
	}
	else if (status==0)
	{
		//	WriteToLog("Nothing Waiting<br>");
	}
	else
	{
			//WriteToLog("got %i packets Waiting<br>",status);

			ret = recvfrom(m_socket, szMessage, sizeof(szMessage), 0, (struct sockaddr *) &remoteaddr, &iremoteaddrlen);
			if(ret == SOCKET_ERROR)
			{
				WriteToLog("<font color=red>Receive failed!</font> (%d), ignoring.<br>", WSAGetLastError());
				//exit(5);
			}
			else
			{
				//WriteToLog("Received: len[%i] ", ret);
				szMessage[ret] = '\0';
				//WriteToLog(" [%s]<br>",szMessage);

				// proper 
				if (szMessage[2]==']')
				{
					// PLAYER JOIN
					if (szMessage[0]=='J' && szMessage[1]=='N')
					{
						// add a player
						WriteToLog("<b>Player Joined!</b><br>");
						pid = AddClient(szMessage, remoteaddr);
						//JN] ?N=%s ?M=%s
						sscanf (szMessage,"JN] %s %s", &sparestringa, &sparestringb);
						sprintf(pcore->players[pid].player.Player_Name,sparestringa);

						pcore->players[pid].player.ChangeModel(sparestringb);

						WriteToLog("Got player name: %s<br>", sparestringa);
						WriteToLog("Got player model: %s<br>", sparestringb);
						WriteToLog("Telling player their ID is %i<br><br>", pid);
						// tell them they can join and their id
						// todo: add server info here ie map, graviyt(!?)
						sprintf(szMessage,"CWC] %i ",pid);
						SendUDPMessage(szMessage, (struct sockaddr *) &remoteaddr);
						sprintf(szMessage,"CPJ] %i", pid);
						SendUDPMessageToEveryone(szMessage);
					}

	// PLAYER MOVEMENT
					else if (szMessage[0]=='P' && szMessage[1]=='M')
					{
						// todo: function to check playerid with connection info

						sscanf (szMessage,"%*s %i %i %i %f %f %f", &pid, &t_x, &t_y, &t_aim, &t_xvel, &t_yvel);

						if (pcore->players[pid].inuse)
						{
							pcore->players[pid].player.m_fX = (float)t_x;
							pcore->players[pid].player.m_fY = (float)t_y;
							pcore->players[pid].player.m_fXVel = (float)t_xvel;
							pcore->players[pid].player.m_fYVel = (float)t_yvel;
							pcore->players[pid].player.m_AimAngle = (float)t_aim;
						}

						// tell them that we received it ok!
					//	SendUDPMessage("COK] PM", (struct sockaddr *) &remoteaddr);

						// tell everyone about the movements
						sprintf(szMessage,"CPM] %i %i %i %+.5f %+.5f %+.5f ", pid, t_x, t_y, t_aim, t_xvel, t_yvel);
						SendUDPMessageToEveryone(szMessage);

					}
// req playerlist
					else if (szMessage[0]=='P' && szMessage[1]=='L')
					{
						// todo: function to check playerid with connection info

						sscanf (szMessage,"%*s %i ", &pid);

						for (int i=0;i<MAX_CLIENTS;i++)
						{
							if (pcore->players[i].inuse)
							{
								sprintf(szMessage, "CAP] %i %i %i %s %s", i, (int)pcore->players[i].player.m_fX, (int)pcore->players[i].player.m_fY, pcore->players[i].player.Player_Name, pcore->players[i].player.Player_Model);
								SendUDPMessage(szMessage, (struct sockaddr *) &remoteaddr);
							}
						}
					}
// change weapon
					else if (szMessage[0]=='C' && szMessage[1]=='W') 
					{
						// todo: function to check playerid with connection info

						int wepid;

						sscanf (szMessage,"%*s %i %i", &pid, &wepid);

						pcore->players[pid].player.m_iWeapon = wepid;

						// relay message
						char szbuffer[300];
						sprintf(szbuffer,"%s%s", "C", szMessage);
						SendUDPMessageToEveryone(szbuffer);
					}
// shoot weapon
					else if (szMessage[0]=='S' && szMessage[1]=='H') 
					{
						// todo: function to check playerid with connection info

						int wepid;
						sscanf (szMessage,"%*s %i %i %i %i %f", &pid, &wepid, &t_x, &t_y, &t_aim);
                        
						if (pcore->players[pid].inuse)
						{
							pcore->players[pid].player.m_iWeapon = wepid;
							pcore->players[pid].player.m_fX = (float)t_x;
							pcore->players[pid].player.m_fY = (float)t_y;
							pcore->players[pid].player.m_AimAngle = (float)t_aim;
							pcore->players[pid].player.Shoot();
						}

						// relay message
						char szbuffer[300];
						sprintf(szbuffer,"%s%s", "C", szMessage);
						//pcore->gfx.WriteToLog(szbuffer);
						SendUDPMessageToEveryone(szbuffer);
					}
// player quit
					else if (szMessage[0]=='P' && szMessage[1]=='Q') 
					{
						// todo: function to check playerid with connection info

						sscanf (szMessage,"%*s %i ", &pid);
						WriteToLog("REMOVING PLAYER %i", pid);
                        
						if (pcore->players[pid].inuse)
						{
							pcore->players[pid].player.Free();
							pcore->players[pid].inuse = false;
						}

						// relay message
						char szbuffer[300];
						sprintf(szbuffer,"%s%s", "C", szMessage);
						SendUDPMessageToEveryone(szbuffer);
					}
				}
			}

	}

   }// while
	


	

}

int CNetcode::AddClient(char* joinmsg, SOCKADDR_IN remoteaddr)
{

	WriteToLog("Adding player!<br>");
	return pcore->AddPlayer(remoteaddr); // return player ID
}
