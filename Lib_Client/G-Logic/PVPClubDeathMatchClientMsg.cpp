#include "stdafx.h"
#include "PVPClubDeathMatchClient.h"

#include "./GLGaeaClient.h"
#include "../../Lib_Network/s_NetGlobal.h"

#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HRESULT PVPClubDeathMatchClient::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	if ( !nmg )	return E_FAIL;

	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER* ) nmg;
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER );
			m_fRemain = pnetmsg->fTime;

			CInnerInterface::GetInstance().PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_CLUB_DEATH_MATCH_REGISTER );

		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE* ) nmg;
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE );
			m_fRemain = pnetmsg->fTime;
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD* ) nmg;
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD );
			m_fRemain = pnetmsg->fTime;
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED* ) nmg;
			SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME* ) nmg;
			NotifyTimeToStart( pnetmsg->wTime );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED* ) nmg;
			m_sScheduleNext = pnetmsg->sScheduleNext;
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC* ) nmg;
			m_sScheduleNext = pnetmsg->sScheduleNext;
			m_emState = pnetmsg->emState;
			m_fRemain = pnetmsg->fRemain;
			
			m_wLevelReq			= pnetmsg->wLevelReq;
			m_wClubLevelLimit	= pnetmsg->wClubLevelLimit;
			m_wClubPlayerMin	= pnetmsg->wClubPlayerMin;
			m_wClubPlayerMax	= pnetmsg->wClubPlayerMax;
			m_bClubPlayerReqCDMRights = pnetmsg->bClubPlayerReqCDMRights;

			m_wBattleDuration	= pnetmsg->wBattleTime;

			if ( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )
			{
				CInnerInterface::GetInstance().PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_CLUB_DEATH_MATCH_REGISTER );
			}

			if ( m_emState != CAPTURE_THE_FLAG_STATE_REWARD )
			{
				m_vecRanking.clear();
			}

			m_bRegistered = pnetmsg->bRegistered;
			m_wClubParticipants = pnetmsg->wClubParticipants;

			//reward info
			for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
			{
				m_sReward[i] = pnetmsg->sReward[i];
			}

		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM* ) nmg;
			m_wClubParticipants = pnetmsg->wClubParticipants;
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB* ) nmg;

			switch( pnetmsg->emFB )
			{
			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_FAILED:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_FAILED") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHARID:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHARID") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_NOTPOSSIBLE:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_NOTPOSSIBLE") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHANNEL:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHANNEL") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_LEVEL_LOW:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_LEVEL_LOW") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CLUB:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CLUB") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_CLUB_LEVEL_LOW:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_CLUB_LEVEL_LOW") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_REQUIRE_RIGHTS:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_REQUIRE_RIGHTS") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_ALREADY_REG:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_ALREADY_REG") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_FULL:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_FULL") );
				break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_REGISTERED:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_REGISTERED") );
					m_bRegistered = TRUE;

					m_wClubParticipants = pnetmsg->wClubParticipants;
				}break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_FAIL") );
					m_wClubParticipants = pnetmsg->wClubParticipants;
				}break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_REG:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_REG") );
					m_bRegistered = FALSE;
					m_wClubParticipants = pnetmsg->wClubParticipants;
				}break;

			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_NOT_ENOUGH_MEMBERS:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_NOT_ENOUGH_MEMBERS") );
					m_wClubParticipants = pnetmsg->wClubParticipants;
				}break;

				/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_DEVICE_INFO_ERROR:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_DEVICE_INFO_ERROR") );
				break;

				/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
			case PVP_CLUB_DEATH_MATCH_REGISTER_FB_DEVICE_REGISTERED:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_DEVICE_REGISTERED") );
				break;
			}
		}break;


	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB* ) nmg;
			switch( pnetmsg->emFB )
			{
			case PVP_CLUB_DEATH_MATCH_REJOIN_FB_FAILED:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REJOIN_FB_FAILED") );
				break;

			case PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOTPOSSIBLE:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOTPOSSIBLE") );
				break;

			case PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOT_REGISTERED:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOT_REGISTERED") );
				break;

			case PVP_CLUB_DEATH_MATCH_REJOIN_FB_INBATTLE:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REJOIN_FB_INBATTLE") );
				break;
			};
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE* ) nmg;
			m_sFirstClubRank = pnetmsg->sFirstRank;
			m_sMyClubRank = pnetmsg->sMyRank;
			m_bDoublePoint = pnetmsg->bDoublePoint;

			if ( m_bDoublePoint )
				CInnerInterface::GetInstance().PVPClubDeathMatchShowDoublePoint( TRUE );
			else
				CInnerInterface::GetInstance().PVPClubDeathMatchShowDoublePoint( FALSE );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO* ) nmg;
			
			for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
				m_sClubTop[i] = pnetmsg->sData[i];
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE* ) nmg;

			DWORD dwGuildID = GLGaeaClient::GetInstance().GetCharacter()->m_dwGuild;

			for( int i=0; i<pnetmsg->wDataNum; ++i )
			{
				PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& sData = pnetmsg->sClubData[i];

				PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( sData.dwClubID );
				if ( pClubData )
				{
					pClubData->wRank = sData.wRank;
					pClubData->wPoints = sData.wPoints;
					pClubData->wKills = sData.wKills;
					pClubData->wDeaths = sData.wDeaths;
					pClubData->wResu = sData.wResu;
				}
				else
				{
					PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubNew = CreateClubData( sData.dwClubID, sData.szClubName, sData.szMasterName, sData.dwCLUB_MARK_VER );
					if ( pClubNew )
					{
						pClubNew->wRank = sData.wRank;
						pClubNew->wPoints = sData.wPoints;
						pClubNew->wKills = sData.wKills;
						pClubNew->wDeaths = sData.wDeaths;
						pClubNew->wResu = sData.wResu;
					}
				}
		
				if ( sData.wRank == 1 )
				{
					m_sFirstClubRank = sData;
				}

				if ( sData.dwClubID == dwGuildID )
				{
					m_sMyClubRank = sData;
				}
			}
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END* ) nmg;
			if( pnetmsg->dwRankNum != (DWORD)m_mapClubData.size() )
			{
				CDebugSet::ToFileWithTime( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchClient Receive Club Rank size error Field:%d Client:%d", 
					pnetmsg->dwRankNum, (DWORD)m_mapClubData.size() );
			}

			if ( pnetmsg->bFinal )
			{
				m_vecRanking.clear();

				PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
				PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

				for ( ; it_b != it_e; ++it_b )
				{
					PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;
					PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK sRank = sData;
					m_vecRanking.push_back( sRank );
				}

				std::sort( m_vecRanking.begin(), m_vecRanking.end() );

				CInnerInterface::GetInstance().PVPClubDeathMatchShowRanking();
				CInnerInterface::GetInstance().PVPClubDeathMatchShowEndNotice();
			}

		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE* ) nmg;
			m_bDoublePoint = pnetmsg->bDoublePoint;

			if ( m_bDoublePoint )
				CInnerInterface::GetInstance().PVPClubDeathMatchShowDoublePoint( TRUE );
			else
				CInnerInterface::GetInstance().PVPClubDeathMatchShowDoublePoint( FALSE );
		}break;
	};

	return S_OK;
}