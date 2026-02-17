#include "stdafx.h"
#include "./PVPClubDeathMatchField.h"

#include "./PVPClubDeathMatchControlMsg.h"
#include "./GLGaeaServer.h"

#include "../../Lib_Network/s_NetGlobal.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT PVPClubDeathMatchField::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !nmg )	return E_FAIL;

	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER* ) nmg;
			DoStateRegister();
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE* ) nmg;
			DoStateBattle();

			RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField Received Club Data Num:%d/%d", (DWORD)m_mapClubData.size(), pnetmsg->dwClubNum );
			GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField Received Club Data Num:%d/%d", (DWORD)m_mapClubData.size(),  pnetmsg->dwClubNum );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD* ) nmg;
			DoStateReward();
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED* ) nmg;
			DoStateEnd();
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED* ) nmg;
			m_sScheduleNext = pnetmsg->sScheduleNext;
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA* ) nmg;

			PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pnetmsg->dwClubID );
			if ( pClubData )
			{
				for( int i=0; i<pnetmsg->wRankNum; ++i )
				{
					pClubData->InsertRegister( pnetmsg->sPlayerData[i].dwCharID, pnetmsg->sPlayerData[i].szCharName, pnetmsg->sPlayerData[i].wCharClass, pnetmsg->sPlayerData[i].wCharSchool );
				}
			}
			else
			{
				PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubNew = CreateClubData( pnetmsg->dwClubID, pnetmsg->szClubName, pnetmsg->szMasterName, pnetmsg->dwClubMarkVer );
				if ( pClubNew )
				{
					for( int i=0; i<pnetmsg->wRankNum; ++i )
					{
						pClubNew->InsertRegister( pnetmsg->sPlayerData[i].dwCharID, pnetmsg->sPlayerData[i].szCharName, pnetmsg->sPlayerData[i].wCharClass, pnetmsg->sPlayerData[i].wCharSchool );
					}
				}
			}
			
			RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField::Received club:[%d]%s num: %u", pnetmsg->dwClubID, pnetmsg->szClubName, pnetmsg->wRankNum );
			GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField::Received club:[%d]%s num: %u", pnetmsg->dwClubID, pnetmsg->szClubName, pnetmsg->wRankNum );
		}break;

	
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ* ) nmg;
			RequestScoreInfo( pnetmsg->dwCharID );
		}break;
	};

	return S_OK;
}