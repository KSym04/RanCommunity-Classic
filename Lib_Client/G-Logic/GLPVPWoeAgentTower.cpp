#include "stdafx.h"
#include "./GLPVPWoeAgent.h"
#include "./GLPVPWoeField.h"
#include "./GLAgentServer.h"
#include "./GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLPVPWoeAgent::RequestEmperiumInfo( DWORD dwCharID )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_WOE_A2C_TOWER_OWNER_INFO netmsgclient;
	GLAgentServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
}

void GLPVPWoeAgent::EmperiumCapture(SNATIVEID sidMob, WORD wGuild)
{
	if (wGuild == CLUB_NULL)			return;
	if (m_emState != WOE_STATE_BATTLE)	return;
	
	GLCLUB* pclub = GLAgentServer::GetInstance().GetClubMan().GetClub(wGuild);
	if (!pclub)	return;

	m_dwEmperiumGuild = pclub->m_dwID;
	m_dwEmperiumMarkVer = pclub->m_dwMarkVER;

	GLMSG::SNETPC_WOE_A2F_TOWER_CAPTURE netmsgfield;
	netmsgfield.dwClubID = m_dwEmperiumGuild;
	netmsgfield.dwMarkVer = m_dwEmperiumMarkVer;
	StringCchCopy(netmsgfield.szName, CLUB_NAME + 1, pclub->m_szName);
	GLAgentServer::GetInstance().SENDTOCHANNEL(&netmsgfield, 0);

	m_nCountCaptured++;

	if (m_nCountCaptured == 1)
	{
		StringCchCopy(szName, CLUB_NAME + 1, pclub->m_szName);
	}

	/*if (m_nCountCaptured >= 2 )
	{
		SetEventState(WOE_STATE_REWARD);
	}*/
}

void GLPVPWoeAgent::DeclareEmperiumWinner()
{
	GLCLUB* pclub = GLAgentServer::GetInstance().GetClubMan().GetClub(m_dwEmperiumGuild);
	if (!pclub)		return;

	if (m_nCountCaptured == 0)
	{
		GLMSG::SNET_MSG_WOE_A2C_TOWER_WINNER netmsgfield;
		netmsgfield.emFB = ENUM_WOE_A2C_TOWER_WINNER_NONE;
		GLAgentServer::GetInstance().SENDTOALLCLIENT(&netmsgfield);
		return;
	}
	else if (m_nCountCaptured == 1)
	{
		GLMSG::SNET_MSG_WOE_A2C_TOWER_WINNER netmsgfield;
		netmsgfield.emFB = ENUM_WOE_A2C_TOWER_WINNER_FIRST;
		StringCchCopy(netmsgfield.szNamePrev, CLUB_NAME + 1, szName);
		GLAgentServer::GetInstance().SENDTOALLCLIENT(&netmsgfield);
		return;
	}
	else if (m_nCountCaptured == 2)
	{
		GLMSG::SNET_MSG_WOE_A2C_TOWER_WINNER netmsgfield;
		netmsgfield.emFB = ENUM_WOE_A2C_TOWER_WINNER_SECOND;
		StringCchCopy(netmsgfield.szNamePrev, CLUB_NAME + 1, szName);
		StringCchCopy(netmsgfield.szNameCur, CLUB_NAME + 1, pclub->m_szName);
		GLAgentServer::GetInstance().SENDTOALLCLIENT(&netmsgfield);
	}
}
