#include "stdafx.h"
#include "./PVPClubDeathMatchAgent.h"

#include "./PVPClubDeathMatchControlMsg.h"
#include "./GLAgentServer.h"

#include "../../Lib_Network/s_NetGlobal.h"

#include "../../Lib_Engine/Core/NSRLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT PVPClubDeathMatchAgent::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !nmg )	return E_FAIL;

	switch( nmg->nType )
	{
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC:
		{
			GLMSG::SNET_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC* ) nmg;
			RequestInfoPC( pnetmsg->dwCharID, pnetmsg->bLobbyMap, pnetmsg->bBattleMap );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ* ) nmg;
			RequestPlayerRegister( pnetmsg->dwCharID, pnetmsg->wLevel, pnetmsg->bRegister, pnetmsg->szHASH );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ* ) nmg;
			PlayerRejoinBattle( pnetmsg->dwCharID );
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE* ) nmg;

			WORD wValidNum = 0;
			for( int i=0; i<pnetmsg->wDataNum; ++i )
			{
				PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pnetmsg->sClubData[i].dwClubID );
				if ( pClubData )
				{
					pClubData->wRank = pnetmsg->sClubData[i].wRank;
					pClubData->wPoints = pnetmsg->sClubData[i].wPoints;
					pClubData->wKills = pnetmsg->sClubData[i].wKills;
					pClubData->wDeaths = pnetmsg->sClubData[i].wDeaths;
					pClubData->wResu = pnetmsg->sClubData[i].wResu;

					//update the top rankings in final rank update
					if ( pnetmsg->bFinal )
					{
						if ( pClubData->wRank > 0 && pClubData->wRank <= PVP_CLUB_DEATH_MATCH_RANK_TOP )
						{
							m_sClubTop[pClubData->wRank-1] = *pClubData;
						}
					}
					
					if ( pClubData->wRank == 1 )
					{
						m_sFirstClubRank = *pClubData;
					}


					wValidNum ++;
				}
			}
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END* ) nmg;

			if ( pnetmsg->bFinal )
				SendTop10Info();
		}break;

	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ:
		{
			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ* ) nmg;
			MapExit( pnetmsg->dwCharID );
		}break;
	};

	return S_OK;
}