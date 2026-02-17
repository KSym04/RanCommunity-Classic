#include "stdafx.h"
#include "./PVPPartyBattleGroundsField.h"

#include "./GLGaeaServer.h"
#include "./GLLandMan.h"

#include "./PVPPartyBattleGroundsMsg.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace PVPPBG
{
	ManagerField& ManagerField::GetInstance()
	{
		static ManagerField cInstance;
		return cInstance;
	}

	ManagerField::ManagerField()
		: Manager()
		, m_bValidInstance(false)
		, m_fScoreUpdate(0.0f)
	{

	}

	ManagerField::~ManagerField()
	{

	}

	void ManagerField::LoadEvent( std::string strFile )
	{
		if ( !RPARAM::bEventPartyBattleGrounds )
			return;

		ResetEvent();

		bool bok = DataLoad( strFile );
		if ( bok )
		{
			FindScheduleFirst();
		}
	}

	void ManagerField::ResetEvent()
	{
		m_bValidInstance = false;
		m_fScoreUpdate = 0.0f;

		m_mapPlayerData.clear();
	}

	bool ManagerField::SetMapState ()
	{
		m_bValidInstance = false;

		bool bValidBattleMap = false;
		bool bValidLobbyMap = false;

		if( m_sBattleMap.IsValidNativeID() )
		{
			SMAPNODE *pMapNode = GLGaeaServer::GetInstance().FindMapNode ( m_sBattleMap );
			if ( pMapNode )
				pMapNode->bPVPPBGBattleZone = true;

			GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID ( m_sBattleMap );
			if ( pLandMan )
				pLandMan->m_bPVPPBGBattleMap = true;	

			if ( pMapNode && pLandMan )
				bValidBattleMap = true;
		}

		if( m_sLobbyMap.IsValidNativeID() )
		{
			SMAPNODE *pMapNode = GLGaeaServer::GetInstance().FindMapNode ( m_sLobbyMap );
			if ( pMapNode )
				pMapNode->bPVPPBGLobbyZone = true;

			GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID ( m_sLobbyMap );
			if ( pLandMan )
				pLandMan->m_bPVPPBGLobbyMap = true;	

			if ( pMapNode && pLandMan )
				bValidLobbyMap = true;
		}

		if ( bValidBattleMap && bValidLobbyMap )
		{
			m_bValidInstance = true;
		}

		return true;
	}

	bool ManagerField::FrameMove ( float fElaps )
	{
		if ( m_emState == PVPPBG::EVENT_STATE_BATTLE )
		{
			m_fScoreUpdate += fElaps;
			if ( m_fScoreUpdate >= m_fScoreUpdateTime )
			{
				m_fScoreUpdate = 0.0f;

				if ( m_bValidInstance )
				{
					ScoreUpdate();
				}
			}
		}

		return true;
	}

	void ManagerField::DoStateRegister()
	{
		m_mapPlayerData.clear();

 		m_fScoreUpdate = 0.0f;

		m_emState = PVPPBG::EVENT_STATE_REGISTER;

		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
		RLOG::LogFile( "_PVPPBG.txt", "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
	}

	void ManagerField::DoStateBattle()
	{
		m_fScoreUpdate = 0.0f;

		m_emState = PVPPBG::EVENT_STATE_BATTLE;

		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
		RLOG::LogFile( "_PVPPBG.txt", "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
	}

	void ManagerField::DoStateReward()
	{
 		if ( m_bValidInstance )
		{
			ScoreCalculate();
			CalculateRanking();
			CalculateRewardPoint();
			ScoreUpdate();
			RankingSend();
 
 			SendRewards();
 		}

		m_emState = PVPPBG::EVENT_STATE_REWARD;

		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
		RLOG::LogFile( "_PVPPBG.txt", "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
	}

	void ManagerField::DoStateEnd()
	{
		m_mapPlayerData.clear();

		m_fScoreUpdate = 0.0f;

		m_emState = PVPPBG::EVENT_STATE_ENDED;

		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
		RLOG::LogFile( "_PVPPBG.txt", "PVPPBG::ManagerField Change State to %s", PVPPBG::STRING_EVENT_STATE[m_emState].c_str() );
	}

	
	PLAYER_DATA* ManagerField::PlayerDataGet( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )
			return NULL;

		PLAYER_DATA_MAP_ITER iterdata = m_mapPlayerData.find( dwCharID );
		if( iterdata != m_mapPlayerData.end() )
		{
			return &(*iterdata).second;
		}

		return NULL;
	};


	void ManagerField::InsertPlayerData( REGISTER_DATA* pPlayerData )
	{
		if ( !pPlayerData )			
			return;

		if ( pPlayerData->dwInfoCharID == PVP_PBG_CHAR_ID_NULL )
		{
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "ManagerField invalid ID:%d %s", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName );
			return;
		}

		PLAYER_DATA* pdata = PlayerDataGet( pPlayerData->dwInfoCharID );
		if ( pdata )
		{
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "ManagerField char:%d %s already exist", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName );
			return;
		}

		if ( pPlayerData->wInfoSchool >= PVPPBG::SCHOOL_SIZE )
		{
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "ManagerField char:%d %s invalid school %d", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName, pPlayerData->wInfoSchool );
			return;
		}

		if ( pPlayerData->wInfoClass >= GLCI_NUM_8CLASS )
		{
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "ManagerField char:%d %s invalid class %d", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName, pPlayerData->wInfoClass );
			return;
		}

		PLAYER_DATA sdata;
		sdata.dwInfoCharID	= pPlayerData->dwInfoCharID;
		sdata.wInfoClass	= pPlayerData->wInfoClass;
		sdata.wInfoSchool	= pPlayerData->wInfoSchool;
		StringCchCopy( sdata.szInfoCharName, PVP_PBG_CHAR_NAME_SIZE, pPlayerData->szInfoCharName );
		m_mapPlayerData.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );


	}


	void ManagerField::RequestInfoPC( DWORD dwCharID )
	{
		PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	
			return;

		GLMSG::SNET_PVP_PBG_F2A_BATTLEINFO_PC netmsg2agent;
		netmsg2agent.dwCharID = dwCharID;
		GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg2agent );
	}

	void ManagerField::ScoreKill( DWORD dwCharKilled, DWORD dwCharKiller )
	{
		if ( m_emState != PVPPBG::EVENT_STATE_BATTLE )	
			return;

		PLAYER_DATA* pplayerkilled = PlayerDataGet( dwCharKilled );
		PLAYER_DATA* pplayerkiller = PlayerDataGet( dwCharKiller );

		if ( pplayerkilled && pplayerkiller )
		{
			pplayerkilled->wScoreDeath ++;
			pplayerkiller->wScoreKill ++;
		}
	}

	void ManagerField::ScoreHeal( DWORD dwChar, DWORD dwHeal, DWORD dwCharHealer  )
	{
		if ( m_emState != PVPPBG::EVENT_STATE_BATTLE )	
			return;

		if ( isDisableScoreSelfHeal() && dwChar == dwCharHealer )
			return;

		PLAYER_DATA* pplayerdata = PlayerDataGet( dwChar );
		if ( pplayerdata )
		{
			pplayerdata->dwScoreHeal += dwHeal;
		}
	}

	void ManagerField::ScoreResu( DWORD dwChar )
	{
		if ( m_emState != PVPPBG::EVENT_STATE_BATTLE )	
			return;

		PLAYER_DATA* pplayerdata = PlayerDataGet( dwChar );
		if ( pplayerdata )
		{
			pplayerdata->wScoreResu ++;
		}
	}

	void ManagerField::ScoreUpdate()
	{
		PLAYER_DATA_MAP_ITER it = m_mapPlayerData.begin();
		for ( ; it != m_mapPlayerData.end(); ++it )
		{
			const PLAYER_DATA& PlayerData = (*it).second;

			PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( PlayerData.dwInfoCharID );
			if ( pchar )	
			{
				GLMSG::SNETPC_PVP_PBG_F2C_PLAYER_SCORE_UPDATE	NetMsgClient;
				NetMsgClient.PlayerData = PlayerData;
				GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &NetMsgClient );
			}
		}
	}

	void ManagerField::RequestScoreInfo( DWORD dwCharID )
	{
		PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	
			return;

		PLAYER_DATA* pPlayerData = PlayerDataGet( pchar->m_dwCharID );
		if ( pPlayerData )
		{
			GLMSG::SNETPC_PVP_PBG_F2C_PLAYER_SCORE_UPDATE	NetMsgClient;
			NetMsgClient.PlayerData = *pPlayerData;
			GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &NetMsgClient );
		}	
	}

	void ManagerField::ScoreCalculate()
	{
		PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			PLAYER_DATA& sPlayerData = (*iterplayer).second;

			//calculate score
			float fTotalScore = 0.0f;
			fTotalScore += float( sPlayerData.dwScoreHeal * m_fHealVar );
			fTotalScore += float( sPlayerData.wScoreKill * m_fKillVar );
			fTotalScore += float( sPlayerData.wScoreResu * m_fResuVar );
			sPlayerData.dwScoreTotal = (DWORD)fTotalScore;
		}
	}

	void ManagerField::CalculateRanking()
	{
		PLAYER_DATA_VEC vecdata;
		vecdata.reserve( m_mapPlayerData.size() );

		PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			const PLAYER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
		}

		std::sort( vecdata.begin(), vecdata.end() );	

		for( size_t i=0; i<vecdata.size(); ++i )
			vecdata[i].wRank = i+1;

		for( size_t i=0; i<vecdata.size(); ++i )
		{
			const PLAYER_DATA& splayer_data = vecdata[i];
			PLAYER_DATA* prank_player = PlayerDataGet( splayer_data.dwInfoCharID );

			if ( prank_player && prank_player->dwInfoCharID == splayer_data.dwInfoCharID )
			{
				prank_player->wRank = splayer_data.wRank;
			}
		}
	}

	void ManagerField::CalculateRewardPoint()
	{
		PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			PLAYER_DATA& sPlayerData = (*iterplayer).second;

			if (sPlayerData.wRank == 1)
			{
				sPlayerData.wRewardPoint = m_wPoint1;
			}
			else if ( sPlayerData.wRank >= 2 && sPlayerData.wRank <= 5 )
			{
				sPlayerData.wRewardPoint = m_wPoint2;
			}
			else
			{
				sPlayerData.wRewardPoint = m_wPoint3;
			}
		}
	}

	void ManagerField::RankingSend()
	{
		PLAYER_DATA_VEC vecdata;
		vecdata.reserve( m_mapPlayerData.size() );

		PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			const PLAYER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
		}

		GLMSG::SNETPC_PVP_PBG_F2C_RANKING_UPDATE netmsg_ranking;
		GLMSG::SNETPC_PVP_PBG_F2A_RANKINFO_PC netmsg_ranking_agent;

		WORD wNum = 0;
		for ( size_t i=0; i<vecdata.size(); ++i )
		{
			const PLAYER_DATA& splayer_data = vecdata[i];
			netmsg_ranking.ADDRANK( splayer_data );
			if ( netmsg_ranking.wRankNum == PVP_PBG_PACKET_RANK_NUM )
			{
				GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &netmsg_ranking );
				GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &netmsg_ranking );
				wNum += netmsg_ranking.wRankNum;
				netmsg_ranking.Reset();
			}

			if ( splayer_data.wRank > 0 && splayer_data.wRank <= PVP_PBG_MINI_RANKING_NUM )
				netmsg_ranking_agent.ADDRANK( splayer_data );
		}

		if ( netmsg_ranking.wRankNum != 0 )
		{
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &netmsg_ranking );
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &netmsg_ranking );
			wNum += netmsg_ranking.wRankNum;
			netmsg_ranking.Reset();
		}

		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPPBG::ManagerField Send Rank num: %u", wNum );
		CDebugSet::ToFileWithTime( "_PVPPBG.txt", "PVPPBG::ManagerField Send Rank num: %u", wNum );

		if ( netmsg_ranking_agent.wRankNum > 0 )	
		{
			GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg_ranking_agent );
			GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPPBG::ManagerField Send to agent rank num: %u", netmsg_ranking_agent.wRankNum );
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "PVPPBG::ManagerField Send to agent rank num: %u", netmsg_ranking_agent.wRankNum );
			netmsg_ranking_agent.wRankNum = 0;
		}

		GLMSG::SNETPC_PVP_PBG_F2C_RANKING_END netmsg_ranking_end;
		netmsg_ranking_end.wPlayerNum = (WORD)m_mapPlayerData.size();
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &netmsg_ranking_end );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &netmsg_ranking_end );
	}


	void ManagerField::SendRewards()
	{
		PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			const PLAYER_DATA& splayerdata = (*iterplayer).second;

			PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
			if ( pChar )
			{
				if ( splayerdata.wRewardPoint > 0 )
				{
					pChar->m_dwContributionPoint += splayerdata.wRewardPoint;

					GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netmsgclient;
					netmsgclient.bNotice = true;
					netmsgclient.dwPoint = pChar->m_dwContributionPoint;
					GLGaeaServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &netmsgclient );
				}
			}
		}
	}

	HRESULT ManagerField::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
	{
		if ( !nmg )	
			return E_FAIL;

		switch ( nmg->nType )
		{
		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REGISTER:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_REGISTER* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_REGISTER* ) nmg;
				DoStateRegister();
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_BATTLE:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_BATTLE* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_BATTLE* ) nmg;
				DoStateBattle();
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REWARD:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_REWARD* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_REWARD* ) nmg;
				DoStateReward();
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_ENDED:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_ENDED* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_ENDED* ) nmg;
				DoStateEnd();
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_NEXTSCHED:
			{
				GLMSG::SNET_PVP_PBG_A2FC_NEXTSCHED* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_NEXTSCHED* ) nmg;
				m_sScheduleNext = pnetmsg->sScheduleNext;
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2F_PLAYER_DATA:
			{
				GLMSG::SNETPC_PVP_PBG_A2F_PLAYER_DATA* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_A2F_PLAYER_DATA* ) nmg;

				for( int i=0; i<pnetmsg->wRankNum; ++i )
					InsertPlayerData( &pnetmsg->sPlayerData[i] );
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_C2F_PLAYER_SCORE_INFO_REQ:
			{
				GLMSG::SNETPC_PVP_PBG_C2F_SCORE_INFO_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_C2F_SCORE_INFO_REQ* ) nmg;
				RequestScoreInfo( pnetmsg->dwCharID );
			}break;

		};

		return S_OK;
	}

	

}