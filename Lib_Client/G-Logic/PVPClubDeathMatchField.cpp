#include "stdafx.h"
#include "./PVPClubDeathMatchField.h"

#include "./PVPClubDeathMatchControlMsg.h"
#include "./GLGaeaServer.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


PVPClubDeathMatchField& PVPClubDeathMatchField::GetInstance()
{
	static PVPClubDeathMatchField cInstance;
	return cInstance;
}

PVPClubDeathMatchField::PVPClubDeathMatchField()
	: m_bValidInstance(false)
	, m_fRankUpdateTimer(0.0f)
	, m_bDoublePoint(FALSE)
	, m_fDoublePointTime(0.0f)
{

}

PVPClubDeathMatchField::~PVPClubDeathMatchField()
{

}

void PVPClubDeathMatchField::LoadEvent( std::string strFile )
{
	if ( !RPARAM::bEventClubDeathMatch )	return;

	ResetEvent();

	bool bok = DataLoad( strFile );
	if ( bok )
	{
		FindScheduleFirst();
	}
}

void PVPClubDeathMatchField::ResetEvent()
{
	m_bValidInstance = false;
	m_fRankUpdateTimer = 0.0f;

	m_bDoublePoint = FALSE;
	m_fDoublePointTime = 0.0f;

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	ProcessClubCleanUp();
}

bool PVPClubDeathMatchField::SetMapState ()
{
	m_bValidInstance = false;

	bool bValidBattleMap = false;
	bool bValidLobbyMap = false;

	if( m_sBattleMap.IsValidNativeID() )
	{
		SMAPNODE *pMapNode = GLGaeaServer::GetInstance().FindMapNode ( m_sBattleMap );
		if ( pMapNode )
			pMapNode->bPVPClubDeathMatchBattleZone = true;

		GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID ( m_sBattleMap );
		if ( pLandMan )
			pLandMan->m_bPVPClubDeathMatchBattleMap = true;	

		if ( pMapNode && pLandMan )
			bValidBattleMap = true;
	}

	if( m_sLobbyMap.IsValidNativeID() )
	{
		SMAPNODE *pMapNode = GLGaeaServer::GetInstance().FindMapNode ( m_sLobbyMap );
		if ( pMapNode )
			pMapNode->bPVPClubDeathMatchLobbyZone = true;

		GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID ( m_sLobbyMap );
		if ( pLandMan )
			pLandMan->m_bPVPClubDeathMatchLobbyMap = true;	

		if ( pMapNode && pLandMan )
			bValidLobbyMap = true;
	}

	if ( bValidBattleMap && bValidLobbyMap )
	{
		m_bValidInstance = true;
	}

	return true;
}

bool PVPClubDeathMatchField::FrameMove ( float fElaps )
{
	if ( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
	{
		m_fRankUpdateTimer += fElaps;
		if ( m_fRankUpdateTimer >= m_fRankUpdateTime )
		{
			m_fRankUpdateTimer = 0.0f;

			if ( m_bValidInstance )
			{
				RankingUpdate();
				RankingSend( FALSE );
			}
		}

		if ( m_bDoublePoint )
		{
			m_fDoublePointTime -= fElaps;
			if ( m_fDoublePointTime <= 0.0f )
			{
				m_fDoublePointTime = m_fDoublePointRechargeTime;
				m_bDoublePoint = FALSE;

				BroadCastDoublePoint();
			}
		}
		else
		{
			m_fDoublePointTime -= fElaps;
			if ( m_fDoublePointTime <= 0.0f )
			{
				m_fDoublePointTime = m_fDoublePointConsumeTime;
				m_bDoublePoint = TRUE;

				BroadCastDoublePoint();
			}
		}
	}

	return true;
}

void PVPClubDeathMatchField::DoStateRegister()
{
	ProcessClubCleanUp();
	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	m_fRankUpdateTimer = 0.0f;

	m_emState = PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
}

void PVPClubDeathMatchField::DoStateBattle()
{
	m_fRankUpdateTimer = 0.0f;

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();


	m_bDoublePoint = FALSE;
	m_fDoublePointTime = m_fDoublePointRechargeTime;

	//send initial scores to agent
	if ( m_bValidInstance )
	{
		RankingUpdate();
		RankingSend( FALSE );
	}

	m_emState = PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
}

void PVPClubDeathMatchField::DoStateReward()
{
	if ( m_bValidInstance )
	{
		//send final scores
		RankingUpdate();
		RankingSend( TRUE );

		SendRewards();
	}

	m_emState = PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
}

void PVPClubDeathMatchField::DoStateEnd()
{
	ProcessClubCleanUp();

	m_fRankUpdateTimer = 0.0f;

	m_bDoublePoint = FALSE;
	m_fDoublePointTime = m_fDoublePointRechargeTime;

	m_emState = PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField Change State to %s", COMMENT::PVP_CLUB_DEATH_MATCH_EVENT_STATE[m_emState].c_str() );
}

void PVPClubDeathMatchField::RequestInfoPC( DWORD dwCharID, BOOL bLobbyMap, BOOL bBattleMap )
{
	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNET_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC netmsg2agent;
	netmsg2agent.dwCharID = dwCharID;
	netmsg2agent.bLobbyMap = bLobbyMap;
	netmsg2agent.bBattleMap = bBattleMap;
	GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg2agent );
}

//remove all club data
void PVPClubDeathMatchField::ProcessClubCleanUp()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;

		//call cleanup
		sData.CleanUp();
	}

	//erase all data
	m_mapClubData.clear();
}


PVP_CLUB_DEATH_MATCH_CLUB_DATA* PVPClubDeathMatchField::GetClubData( DWORD dwClubID )
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it = m_mapClubData.find( dwClubID );
	if ( it != m_mapClubData.end() )
	{
		return &(*it).second;
	}

	return NULL;
}

PVP_CLUB_DEATH_MATCH_CLUB_DATA* PVPClubDeathMatchField::CreateClubData( DWORD dwClubID, const TCHAR* szClubName, const TCHAR* szMasterName, DWORD dwClubMarkVer )
{
	if ( dwClubID == PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL ) return NULL;
	if ( !szClubName )		return NULL;
	if ( !szMasterName )	return NULL;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA sData;
	sData.dwClubID = dwClubID;
	StringCchCopy( sData.szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, szClubName );
	StringCchCopy( sData.szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, szMasterName );
	sData.dwCLUB_MARK_VER = dwClubMarkVer;

	//add default score
	sData.wPoints = m_wDefaultPoint;

	m_mapClubData.insert( std::make_pair( sData.dwClubID, sData ) );

	return GetClubData( dwClubID );
}

void PVPClubDeathMatchField::BroadCastDoublePoint()
{
	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE NetMsgEnd;
	NetMsgEnd.bDoublePoint = m_bDoublePoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &NetMsgEnd );
	GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &NetMsgEnd );
}

void PVPClubDeathMatchField::SendRewards()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();
	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sClubData = (*it_b).second;

		if ( sClubData.wRank > 0 && sClubData.wRank <= PVP_CLUB_DEATH_MATCH_RANK_TOP )
		{
			PVP_CLUB_DEATH_MATCH_REWARD_DATA& sRewardData = m_sReward[sClubData.wRank-1];

			PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it_b2 = sClubData.mapPlayerData.begin();
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it_e2 = sClubData.mapPlayerData.end();

			for ( ; it_b2 != it_e2; ++it_b2 )
			{
				PVP_CLUB_DEATH_MATCH_PLAYER_DATA& sPlayerData = (*it_b2).second;

				PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID( sPlayerData.dwCharID );
				if ( !pChar )	continue;

				SITEM* pItemReward = GLItemMan::GetInstance().GetItem( sRewardData.sItemID );
				if ( pItemReward )
				{
					SITEMCUSTOM sITEM_NEW;
					sITEM_NEW.sNativeID = sRewardData.sItemID;
					sITEM_NEW.tBORNTIME = CTime::GetCurrentTime().GetTime();
					sITEM_NEW.cGenType = EMGEN_PVP_EVENT_REWARD;
					sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
					sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
					sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

					/*item color, Juver, 2018/01/08 */
					SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( sITEM_NEW.sNativeID );
					if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
					{
						sITEM_NEW.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
						sITEM_NEW.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
					}

					CItemDrop cDropItem;
					cDropItem.sItemCustom = sITEM_NEW;
					if ( pChar->IsInsertToInvenEx ( &cDropItem ) )
					{
						pChar->InsertToInvenEx ( &cDropItem );
						GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CLUB, pChar->m_dwGuild, ID_CHAR, pChar->m_dwCharID, EMITEM_ROUTE_SYSTEM, sITEM_NEW.wTurnNum );
					}
				}

				if ( sRewardData.wContributionPoint > 0 )
				{
					pChar->m_dwContributionPoint += sRewardData.wContributionPoint;

					GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netmsgclient;
					netmsgclient.bNotice = true;
					netmsgclient.dwPoint = pChar->m_dwContributionPoint;
					GLGaeaServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &netmsgclient );
				}

			}
		}
	}
}