#include "stdafx.h"
#include "./PVPClubDeathMatchAgent.h"

#include "./PVPClubDeathMatchControlMsg.h"
#include "./GLAgentServer.h"
#include "./GLClubMan.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


PVPClubDeathMatchAgent& PVPClubDeathMatchAgent::GetInstance()
{
	static PVPClubDeathMatchAgent cInstance;
	return cInstance;
}

PVPClubDeathMatchAgent::PVPClubDeathMatchAgent()
	: m_fTimer(0.0f)
	, m_fRemain(0.0f)
	, m_fBrdTimer(0.0f)
	, m_wLastNotifyTime(0)
	, m_bMoveIn(false)
	, m_bMoveOut(false)
	, m_fMoveTimer(0.0f)
	, m_fMoveTimerTotal(0.0f)
{

}

PVPClubDeathMatchAgent::~PVPClubDeathMatchAgent()
{

}

void PVPClubDeathMatchAgent::LoadEvent( std::string strFile )
{
	if ( !RPARAM::bEventClubDeathMatch )	return;

	ResetEvent();

	bool bok = DataLoad( strFile );
	if ( bok )
	{
		FindScheduleFirst();
	}
}

void PVPClubDeathMatchAgent::ResetEvent()
{
	m_emState = PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED;
	m_sScheduleNext.Reset();
	m_vecSchedule.clear();

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
		m_sClubTop[i] = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	m_vecPlayerMove.clear();
	ProcessClubCleanUp();

	m_bMoveIn = false;
	m_bMoveOut = false;

	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	m_mapPlayerHash.clear();

}

bool PVPClubDeathMatchAgent::SetMapState ()
{
	if( m_sBattleMap.IsValidNativeID() )
	{
		SMAPNODE *pMapNode = GLAgentServer::GetInstance().GetMapList()->FindMapNode ( m_sBattleMap );
		if ( pMapNode )
			pMapNode->bPVPClubDeathMatchBattleZone = true;

		GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( m_sBattleMap );
		if ( pLandMan )
			pLandMan->m_bPVPClubDeathMatchBattleMap = true;
	}

	if( m_sLobbyMap.IsValidNativeID() )
	{
		SMAPNODE *pMapNode = GLAgentServer::GetInstance().GetMapList()->FindMapNode ( m_sLobbyMap );
		if ( pMapNode )
			pMapNode->bPVPClubDeathMatchLobbyZone = true;

		GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
		if ( pLandMan )
			pLandMan->m_bPVPClubDeathMatchLobbyMap = true;
	}


	return true;
}

bool PVPClubDeathMatchAgent::FrameMove ( float fElapsedTime )
{
	if ( GLCONST_CHAR::bPKLESS )	return false;
	if ( m_vecSchedule.empty() )	return false;

	if ( IsEnded() )
	{
		if ( m_sScheduleNext.dwIndex != PVP_CLUB_DEATH_MATCH_SCHEDULE_NONE )
		{
			CTime ctimecur = CTime::GetCurrentTime();
			if ( m_sScheduleNext.sStartSched.wStartDay == (WORD)ctimecur.GetDayOfWeek() &&
				m_sScheduleNext.sStartSched.wStartHour == (WORD)ctimecur.GetHour() &&
				m_sScheduleNext.sStartSched.wStartMinute == (WORD)ctimecur.GetMinute() )
			{
				SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER );
			}
		}
	}


	if ( IsRegister() )
	{
		m_fTimer += fElapsedTime;
		m_fRemain -= fElapsedTime;

		NotifyTimeToStart( m_fRemain );	

		m_fBrdTimer += fElapsedTime;
		if ( m_fBrdTimer >= PVP_CLUB_DEATH_MATCH_REG_NUM_BRD_TIME )
		{
			m_fBrdTimer = 0.0f;
			BroadCastPlayerNum();
		}

		if ( m_fTimer >= (float)m_wRegisterTime )
		{
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE );
		}
	}

	if ( IsBattle() )
	{
		m_fTimer += fElapsedTime;
		m_fRemain -= fElapsedTime;

		CheckForceMove( fElapsedTime );

		if ( m_fTimer >= (float)m_wBattleTime )
		{
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD );
		}
	}

	if ( IsReward() )
	{
		m_fTimer += fElapsedTime;
		m_fRemain -= fElapsedTime;

		if ( m_fTimer >= (float)m_wRewardTime )
		{
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED );
		}
	}

	if ( m_bMoveIn && IsBattle() )
	{
		m_fMoveTimer += fElapsedTime;
		m_fMoveTimerTotal += fElapsedTime;

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
		m_fMoveTimer += fElapsedTime;
		m_fMoveTimerTotal += fElapsedTime;

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

void PVPClubDeathMatchAgent::SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE emState )
{
	GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "PVPClubDeathMatchAgent::SetEventState '%s'", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[emState].c_str() );
	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::SetEventState '%s'", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[emState].c_str() );

	switch( emState )
	{
	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED:
		{
			DoStateEnd();
		}break;

	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER:
		{
			DoStateRegister();
		}break;

	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE:
		{
			DoStateBattle();
		}break;

	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD:
		{
			DoStateReward();
		}break;
	};

	m_emState = emState;
}

void PVPClubDeathMatchAgent::DoStateRegister()
{
	m_wLastNotifyTime = 0;
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wRegisterTime;
	m_fBrdTimer = 0.0f;

	m_vecPlayerMove.clear();
	ProcessClubCleanUp();

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	m_mapPlayerHash.clear();

	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER netmsgstate;
	netmsgstate.fTime = m_fRemain;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void PVPClubDeathMatchAgent::DoStateBattle()
{
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wBattleTime;
	m_fBrdTimer = 0.0f;

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
		m_sClubTop[i] = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	ProcessClubQualify();
	
	SendClubDataAll();
	QueueMoveIn();

	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE netmsgstate;
	netmsgstate.fTime = m_fRemain;
	netmsgstate.dwClubNum = (DWORD)m_mapClubData.size();
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void PVPClubDeathMatchAgent::DoStateReward()
{
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wRewardTime;
	m_fBrdTimer = 0.0f;

	
	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD netmsgstate;
	netmsgstate.fTime = m_fRemain;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void PVPClubDeathMatchAgent::DoStateEnd()
{
	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED netmsgstate;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );

	if ( FindScheduleNext() )
	{
		GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED netmsgschedule;
		netmsgschedule.sScheduleNext = m_sScheduleNext;
		GLAgentServer::GetInstance().SENDTOCHANNEL( &netmsgschedule, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgschedule );
	}

	QueueMoveOut();

	ProcessClubCleanUp();

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	m_mapPlayerHash.clear();
}

void PVPClubDeathMatchAgent::NotifyTimeToStart( float fTime )
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

	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME netmsgclient;
	netmsgclient.wTime = wremain / 60;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgclient );
}

void PVPClubDeathMatchAgent::BroadCastPlayerNum()
{
	GLClubMan& cClubMan = GLAgentServer::GetInstance().GetClubMan();

	CLUBS_ITER pos = cClubMan.m_mapCLUB.begin();
	CLUBS_ITER end = cClubMan.m_mapCLUB.end();
	for ( ; pos!=end; ++pos )
	{
		GLCLUB &cCLUB = (*pos).second;

		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( cCLUB.m_dwID );
		if ( pClubData )
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM NetMsgBrd;
			NetMsgBrd.wClubParticipants = pClubData->GetPlayerNum();

			CLUBMEMBERS_ITER pos = cCLUB.m_mapMembers.begin();
			CLUBMEMBERS_ITER end = cCLUB.m_mapMembers.end();
			for ( ; pos!=end; ++pos )
			{
				PGLCHARAG pMEMBER = GLAgentServer::GetInstance().GetCharID ( (*pos).first );
				if ( pMEMBER )
				{
					GLAgentServer::GetInstance().SENDTOCLIENT ( pMEMBER->m_dwClientID, &NetMsgBrd );
				}
			}
		}
	}
}

void PVPClubDeathMatchAgent::RequestInfoPC( DWORD dwCharID, BOOL bLobbyMap, BOOL bBattleMap )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	//event info
	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC netmsgclient;
	netmsgclient.sScheduleNext = m_sScheduleNext;
	netmsgclient.emState = m_emState;
	netmsgclient.fRemain = m_fRemain;
	

	netmsgclient.wLevelReq = m_wLevelReq;
	netmsgclient.wClubLevelLimit = m_wClubLevelLimit;
	netmsgclient.wClubPlayerMin = m_wClubPlayerMin;
	netmsgclient.wClubPlayerMax = m_wClubPlayerMax;
	netmsgclient.bClubPlayerReqCDMRights = m_bClubPlayerReqCDMRights;

	netmsgclient.wBattleTime = m_wBattleTime;

	//register info
	PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pchar->m_dwGuild );
	if ( pClubData )
	{
		netmsgclient.wClubParticipants = pClubData->GetPlayerNum();

		PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayerData = pClubData->PlayerDataGet( pchar->m_dwCharID );
		if ( pPlayerData )
		{
			netmsgclient.bRegistered = true;
		}
	}

	//reward info
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		netmsgclient.sReward[i] = m_sReward[i];
	}

	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgclient );

	//last rankinfo
	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO netmsglastrank;
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
		netmsglastrank.sData[i] = m_sClubTop[i];

	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsglastrank );
}

void PVPClubDeathMatchAgent::SendClubDataAll()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();
	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sClubData = (*it_b).second;
		SendClubData( sClubData );
	}
}

void PVPClubDeathMatchAgent::SendClubData( PVP_CLUB_DEATH_MATCH_CLUB_DATA& sClubData )
{
	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it_b = sClubData.mapPlayerData.begin();
	PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it_e = sClubData.mapPlayerData.end();

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA	NetMsgField;
	NetMsgField.dwClubID = sClubData.dwClubID;
	StringCchCopy( NetMsgField.szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, sClubData.szClubName );
	StringCchCopy( NetMsgField.szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, sClubData.szMasterName );
	NetMsgField.dwClubMarkVer = sClubData.dwCLUB_MARK_VER;

	DWORD dwPlayerNum = 0;
	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_PLAYER_DATA& sPlayerData = (*it_b).second;

		PVP_CLUB_DEATH_MATCH_REGISTER_DATA sRegisterData = sPlayerData;

		NetMsgField.ADDRANK( sRegisterData );
		if ( NetMsgField.wRankNum == PVP_CLUB_DEATH_MATCH_PACKET_PLAYER_DATA_NUM )
		{
			GLAgentServer::GetInstance().SENDTOCHANNEL( &NetMsgField, 0 );
			dwPlayerNum += NetMsgField.wRankNum;
			NetMsgField.RESETRANK();
		}	
	}

	if ( NetMsgField.wRankNum != 0 )
	{
		GLAgentServer::GetInstance().SENDTOCHANNEL( &NetMsgField, 0 );
		dwPlayerNum += NetMsgField.wRankNum;
		NetMsgField.RESETRANK();
	}

	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::SendClubData club:[%d]%s num: %u", NetMsgField.dwClubID, NetMsgField.szClubName, dwPlayerNum );
	GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "PVPClubDeathMatchAgent::SendClubData club:[%d]%s num: %u", NetMsgField.dwClubID, NetMsgField.szClubName, dwPlayerNum );
}

void PVPClubDeathMatchAgent::SendTop10Info()
{
	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO netmsglastrank;
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
		netmsglastrank.sData[i] = m_sClubTop[i];

	GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsglastrank );
}
