#include "stdafx.h"
#include "./GLPVPWoeAgent.h"
#include "./GLAgentServer.h"
#include "./GLAGLandMan.h"
#include "./GLGaeaServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPVPWoeAgent& GLPVPWoeAgent::GetInstance()
{
	static GLPVPWoeAgent cInstance;
	return cInstance;
}

void GLPVPWoeAgent::LoadEvent( std::string strFile )
{
	ResetData();

	bool bok = Load( strFile );
	if ( bok )
	{
		FindScheduleFirst();
	}
}

bool GLPVPWoeAgent::SetMapState ()
{
	if( m_sMap.IsValidNativeID() )
	{
		GLAGLandMan *plandman = GLAgentServer::GetInstance().GetByMapID ( m_sMap );
		if ( plandman )
			plandman->m_bPVPWoeMap = true;
	}
	
	return true;
}

bool GLPVPWoeAgent::FrameMove ( float fElaps )
{
	if ( GLCONST_CHAR::bPKLESS )	return false;
	if ( m_vecSchedule.empty() )	return false;

	if ( IsEnded() )
	{
		if ( m_sScheduleNext.dwIndex != WOE_SCHEDULE_NONE )
		{
			CTime ctimecur = CTime::GetCurrentTime();
			if ( m_sScheduleNext.sStartSched.wStartDay == (WORD)ctimecur.GetDayOfWeek() &&
				m_sScheduleNext.sStartSched.wStartHour == (WORD)ctimecur.GetHour() &&
				m_sScheduleNext.sStartSched.wStartMinute == (WORD)ctimecur.GetMinute() )
			{
				SetEventState( WOE_STATE_REGISTER );
			}
		}
	}
	

	if ( IsRegister() )
	{
		m_fTimer += fElaps;
		m_fRemain -= fElaps;

		NotifyTimeToStart( m_fRemain );	

		if ( m_fTimer >= (float)m_wRegisterTime ){
			SetEventState( WOE_STATE_BATTLE );
		}
	}

	if ( IsBattle() )
	{
		m_fTimer += fElaps;
		m_fRemain -= fElaps;

		if ( m_fTimer >= (float)m_wBattleTime ){
			SetEventState( WOE_STATE_REWARD );
		}
	}

	if ( IsReward() )
	{
		m_fTimer += fElaps;
		m_fRemain -= fElaps;

		if ( m_fTimer >= (float)m_wRewardTime ){
			SetEventState( WOE_STATE_ENDED );
		}
	}

	if ( m_bMoveIn && IsBattle() )
	{
		m_fMoveTimer += fElaps;
		m_fMoveTimerTotal += fElaps;

		if( m_fMoveTimer >= 0.2f )
		{
			MapMoveInPlayer();
			m_fMoveTimer = 0.0f;
		}

		if ( m_fMoveTimerTotal >= 300.0f )
		{
			m_bMoveIn = false;
			m_fMoveTimer = 0.0f;
			m_fMoveTimerTotal = 0.0f;
		}

		if ( m_vecPlayerMove.empty() )
		{
			m_bMoveIn = false;
			m_fMoveTimer = 0.0f;
			m_fMoveTimerTotal = 0.0f;
		}

	}

	if ( m_bMoveOut && IsEnded() )
	{
		m_fMoveTimer += fElaps;
		m_fMoveTimerTotal += fElaps;

		if( m_fMoveTimer >= 0.2f )
		{
			MapMoveOutPlayer();
			m_fMoveTimer = 0.0f;
		}

		if ( m_fMoveTimerTotal >= 300.0f )
		{
			m_bMoveOut = false;
			m_fMoveTimer = 0.0f;
			m_fMoveTimerTotal = 0.0f;
		}

		if ( m_vecPlayerMove.empty() )
		{
			m_bMoveOut = false;
			m_fMoveTimer = 0.0f;
			m_fMoveTimerTotal = 0.0f;
		}
	}

	return true;
}

void GLPVPWoeAgent::SetEventState( WOE_STATE emState )
{
	std::string streventstate[WOE_STATE_SIZE] = 
	{
		"Event End",
		"Register Start",
		"Battle Start",
		"Award Start"
	};

	switch( emState )
	{
	case WOE_STATE_ENDED:
		{
			DoStateEnd();
		}break;

	case WOE_STATE_REGISTER:
		{
			DoStateRegister();
		}break;

	case WOE_STATE_BATTLE:
		{
			DoStateBattle();
		}break;

	case WOE_STATE_REWARD:
		{
			DoStateReward();
			DeclareEmperiumWinner();
		}break;
	};

	m_emState = emState;
}

void GLPVPWoeAgent::ResetPreviousWinner()
{
	m_sWoeTowerData.dwGuildOwner = 0;
	m_strLeader.clear();
	m_strName.clear();
	m_dwEmperiumGuild = CLUB_NULL;
	m_dwEmperiumMarkVer = 0;
	m_nCountCaptured = 0;
}

void GLPVPWoeAgent::ResetData()
{
	m_emState = WOE_STATE_ENDED;
	m_sScheduleNext = WOE_SCHED_NEXT();
	m_vecSchedule.clear();
	
	m_vecPlayerMove.clear();

	m_bMoveIn = false;
	m_bMoveOut = false;

	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;
}

void GLPVPWoeAgent::DoStateRegister()
{
	m_wLastNotifyTime = 0;
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wRegisterTime;

	m_vecPlayerMove.clear();

	GLMSG::SNET_WOE_A2FC_STATE_REGISTER netmsgstate;
	netmsgstate.fTime = m_fRemain;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void GLPVPWoeAgent::DoStateBattle()
{
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wBattleTime;

	SendPlayerData();
	QueueMoveIn();
	ResetPreviousWinner();

	GLMSG::SNET_WOE_A2FC_STATE_BATTLE netmsgstate;
	netmsgstate.fTime = m_fRemain;
	netmsgstate.wPlayerNum = (WORD)m_mapGuildData.size();
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void GLPVPWoeAgent::DoStateReward()
{
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wRewardTime;

	GLMSG::SNET_WOE_A2FC_STATE_REWARD netmsgstate;
	netmsgstate.fTime = m_fRemain;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void GLPVPWoeAgent::DoStateEnd()
{
	GLMSG::SNET_WOE_A2FC_STATE_ENDED netmsgstate;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );

	if ( FindScheduleNext() )
	{
		GLMSG::SNET_WOE_A2FC_NEXTSCHED netmsgschedule;
		netmsgschedule.sScheduleNext = m_sScheduleNext;
		GLAgentServer::GetInstance().SENDTOCHANNEL( &netmsgschedule, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgschedule );
	}

	QueueMoveOut();

	m_mapGuildData.clear();
}

void GLPVPWoeAgent::NotifyTimeToStart( float fTime )
{
	WORD wremain = (WORD)m_fRemain;
	if( m_wLastNotifyTime == wremain )	return;

	bool bnotify = false;
	if ( wremain > 300 && ( wremain%300 == 0 ) )	
		bnotify = true;
	else if ( wremain > 0 && wremain <= 300 && ( wremain%60 == 0 ) )
		bnotify = true;

	if ( !bnotify )	return;

	m_wLastNotifyTime = wremain;

	GLMSG::SNET_WOE_A2C_TOBATTLE_TIME netmsgclient;
	netmsgclient.wTime = wremain / 60;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgclient );
}
void GLPVPWoeAgent::RequestInfoPC( DWORD dwCharID, BOOL bWoeMap)
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNET_WOE_A2C_BATTLEINFO_PC netmsgclient;
	netmsgclient.emState = m_emState;
	netmsgclient.fRemain = m_fRemain;
	netmsgclient.sScheduleNext = m_sScheduleNext;
	netmsgclient.wLevelReq = m_wLevelReq;
	netmsgclient.wPlayerLimit = m_wPlayerLimit;
	
	WOE_REGISTER_DATA* pplayerdata = PlayerDataGet( dwCharID );
	if ( pplayerdata ){
		netmsgclient.bRegistered = true;
	}

	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgclient );

	GLCLUB* pclub = GLAgentServer::GetInstance().GetClubMan().GetClub(m_dwEmperiumGuild);
	if (!pclub)		return;

	GLMSG::SNETPC_WOE_A2C_RANKINFO_PC netclient;
	netclient.dwClubID = pclub->m_dwID;
	netclient.dwEmperiumMarkVer = pclub->m_dwMarkVER;
	StringCchCopy(netclient.szName, CLUB_NAME + 1, pclub->m_szName);
	StringCchCopy(netclient.szLeader, CLUB_NAME + 1, pclub->m_szMasterName);
	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netclient);
	GLAgentServer::GetInstance().SENDTOALLCLIENT(&netclient);

}
