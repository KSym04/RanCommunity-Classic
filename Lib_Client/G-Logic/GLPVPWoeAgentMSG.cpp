#include "stdafx.h"
#include "./GLPVPWoeAgent.h"
#include "./GLAgentServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GLPVPWoeAgent::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !nmg ){
		return E_FAIL;
	}

	switch( nmg->nType )
	{
	case NET_MSG_GCTRL_WOE_F2A_BATTLEINFO_PC:
		{
			GLMSG::SNET_WOE_F2A_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_WOE_F2A_BATTLEINFO_PC* ) nmg;
			RequestInfoPC( pnetmsg->dwCharID, pnetmsg->bWoeMap );
		}break;

	case NET_MSG_GCTRL_WOE_F2A_RANKINFO_PC:
		{
			GLMSG::SNETPC_WOE_A2C_RANKINFO_PC netmsgclient;

			//GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgclient );

		}break;

	case NET_MSG_GCTRL_WOE_C2A_REGISTER_REQ:
		{
			GLMSG::SNETPC_WOE_C2A_REGISTER_REQ* pnetmsg = ( GLMSG::SNETPC_WOE_C2A_REGISTER_REQ* ) nmg;
			RequestPlayerRegister( pnetmsg->dwCharID, pnetmsg->wLevel, pnetmsg->bRegister );
		}break;

	case NET_MSG_GCTRL_WOE_C2A_REJOIN_REQ:
		{
			GLMSG::SNETPC_WOE_C2A_REJOIN_REQ* pnetmsg = ( GLMSG::SNETPC_WOE_C2A_REJOIN_REQ* ) nmg;
			PlayerRejoinBattle( pnetmsg->dwCharID );
		}break;

	case NET_MSG_GCTRL_WOE_C2A_MAPEXIT_REQ:
		{
			GLMSG::SNETPC_WOE_C2A_MAPEXIT_REQ* pnetmsg = ( GLMSG::SNETPC_WOE_C2A_MAPEXIT_REQ* ) nmg;
			MapExit( pnetmsg->dwCharID );
		}break;

	case NET_MSG_GCTRL_WOE_F2A_TOWER_CAPTURE:
		{	
			GLMSG::SNETPC_WOE_F2A_TOWER_CAPTURE* pnetmsg = ( GLMSG::SNETPC_WOE_F2A_TOWER_CAPTURE* ) nmg;
			EmperiumCapture(pnetmsg->sidMob, pnetmsg->wClubID);
		}break;

	case NET_MSG_GCTRL_WOE_C2A_TOWER_INFO_REQ:
		{
			GLMSG::SNETPC_WOE_C2A_TOWER_OWNER_INFO_REQ* pnetmsg = ( GLMSG::SNETPC_WOE_C2A_TOWER_OWNER_INFO_REQ* ) nmg;
			RequestEmperiumInfo( pnetmsg->dwCharID );
		}break;
	};

	return S_OK;
}
