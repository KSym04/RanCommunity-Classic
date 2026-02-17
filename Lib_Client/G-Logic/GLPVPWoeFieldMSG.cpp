#include "stdafx.h"
#include "GLPVPWoeField.h"
#include "GLGaeaServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GLPVPWoeField::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !nmg ){
		return E_FAIL;
	}
	switch ( nmg->nType )
	{
	case NET_MSG_WOE_A2F_DECLARE_WINNER:
	{
		GLMSG::SNET_MSG_WOE_A2F_DECLARE_WINNER* netmsg = (GLMSG::SNET_MSG_WOE_A2F_DECLARE_WINNER*)nmg;
		DeclareEmperiumWinner(netmsg->szName);
	}
	break;
	case NET_MSG_WOE_CAPTURE_FLD:
	{
		GLMSG::SNET_MSG_WOE_CAPTURE_FLD* netmsg = (GLMSG::SNET_MSG_WOE_CAPTURE_FLD*)nmg;
	}
	break;
	case NET_MSG_GCTRL_WOE_A2FC_STATE_REGISTER:
		{
			GLMSG::SNET_WOE_A2FC_STATE_REGISTER* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_REGISTER* ) nmg;
			DoStateRegister();
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_STATE_BATTLE:
		{
			GLMSG::SNET_WOE_A2FC_STATE_BATTLE* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_BATTLE* ) nmg;
			DoStateBattle();
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_STATE_REWARD:
		{
			GLMSG::SNET_WOE_A2FC_STATE_REWARD* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_REWARD* ) nmg;
			DoStateReward();
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_STATE_ENDED:
		{
			GLMSG::SNET_WOE_A2FC_STATE_ENDED* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_ENDED* ) nmg;
			DoStateEnd();
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_NEXTSCHED:
		{
			GLMSG::SNET_WOE_A2FC_NEXTSCHED* pnetmsg = ( GLMSG::SNET_WOE_A2FC_NEXTSCHED* ) nmg;
			m_sScheduleNext = pnetmsg->sScheduleNext;
		}break;

	case NET_MSG_GCTRL_WOE_A2F_PLAYER_DATA:
		{
			GLMSG::SNETPC_WOE_A2F_PLAYER_DATA* pnetmsg = ( GLMSG::SNETPC_WOE_A2F_PLAYER_DATA* ) nmg;

			for( int i=0; i<pnetmsg->wRankNum; ++i )
				InsertPlayerData( &pnetmsg->sPlayerData[i] );
		}break;

	case NET_MSG_GCTRL_WOE_A2F_TOWER_CAPTURE:
		{
			GLMSG::SNETPC_WOE_A2F_TOWER_CAPTURE* pnetmsg = ( GLMSG::SNETPC_WOE_A2F_TOWER_CAPTURE* ) nmg;
			
			GLMSG::SNETPC_WOE_F2C_TOWER_CAPTURE netmsgclient;
			netmsgclient.dwClubID = pnetmsg->dwClubID;
			netmsgclient.dwMarkVer = pnetmsg->dwMarkVer;
			StringCchCopy(netmsgclient.szName, CLUB_NAME + 1, pnetmsg->szName);
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sMap.dwID, &netmsgclient );

		}break;

	case NET_MSG_GCTRL_WOE_A2F_TOWER_WINNER:
		{
			/*GLMSG::SNETPC_WOE_A2F_TOWER_WINNER* pnetmsg = ( GLMSG::SNETPC_WOE_A2F_TOWER_WINNER* ) nmg;

			GLMSG::SNETPC_WOE_F2C_TOWER_WINNER netmsgclient;
			netmsgclient.nCapturedFB = pnetmsg->nCaptureFB;
			StringCchCopy(netmsgclient.szNamePrev, CLUB_NAME + 1, pnetmsg->szNamePrev);
			StringCchCopy(netmsgclient.szNameCur, CLUB_NAME + 1, pnetmsg->szNameCur);
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMap.dwID, &netmsgclient);*/

		}break;

	case NET_MSG_GCTRL_WOE_C2F_REVIVE_REQ:
		{
			GLMSG::SNETPC_WOE_C2F_REVIVE_REQ* pnetmsg = ( GLMSG::SNETPC_WOE_C2F_REVIVE_REQ* ) nmg;
			PlayerRevive( dwGaeaID, pnetmsg->dwCharID );
		}break;

	};

	return S_OK;
}