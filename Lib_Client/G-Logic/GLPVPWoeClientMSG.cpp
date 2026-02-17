#include "stdafx.h"
#include "GLPVPWoeClient.h"
#include "GLGaeaClient.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "DxGlobalStage.h"
#include "GLAgentServer.h"

#include "../Lib_ClientUI/Interface/PVPWoeTowerCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GLPVPWoeClient::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	if ( !nmg ){
		return E_FAIL;
	}
	switch ( nmg->nType )
	{
	case NET_MSG_WOE_CAPTURE:
	{
		GLMSG::SNET_MSG_WOE_CAPTURE* netmsg = (GLMSG::SNET_MSG_WOE_CAPTURE*)nmg;
	}
	break;
	case NET_MSG_GCTRL_WOE_A2FC_STATE_REGISTER:
		{
			GLMSG::SNET_WOE_A2FC_STATE_REGISTER* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_REGISTER* ) nmg;
			SetEventState( WOE_STATE_REGISTER );
			m_fRemain = pnetmsg->fTime;

			CInnerInterface::GetInstance().PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_WOE_REGISTER );

		}break;

	case NET_MSG_GCTRL_WOE_A2FC_STATE_BATTLE:
		{
			GLMSG::SNET_WOE_A2FC_STATE_BATTLE* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_BATTLE* ) nmg;
			SetEventState( WOE_STATE_BATTLE );
			m_fRemain = pnetmsg->fTime;
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_STATE_REWARD:
		{
			GLMSG::SNET_WOE_A2FC_STATE_REWARD* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_REWARD* ) nmg;
			SetEventState( WOE_STATE_REWARD );
			m_fRemain = pnetmsg->fTime;
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_STATE_ENDED:
		{
			GLMSG::SNET_WOE_A2FC_STATE_ENDED* pnetmsg = ( GLMSG::SNET_WOE_A2FC_STATE_ENDED* ) nmg;
			SetEventState( WOE_STATE_ENDED );
		}break;

	case NET_MSG_GCTRL_WOE_A2C_TOBATTLE_TIME:
		{
			GLMSG::SNET_WOE_A2C_TOBATTLE_TIME* pnetmsg = ( GLMSG::SNET_WOE_A2C_TOBATTLE_TIME* ) nmg;
			NotifyTimeToStart( pnetmsg->wTime );
		}break;

	case NET_MSG_GCTRL_WOE_A2FC_NEXTSCHED:
		{
			GLMSG::SNET_WOE_A2FC_NEXTSCHED* pnetmsg = ( GLMSG::SNET_WOE_A2FC_NEXTSCHED* ) nmg;
			m_sScheduleNext = pnetmsg->sScheduleNext;
		}break;

	case NET_MSG_GCTRL_WOE_A2C_BATTLEINFO_PC:
		{
			GLMSG::SNET_WOE_A2C_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_WOE_A2C_BATTLEINFO_PC* ) nmg;
			m_emState = pnetmsg->emState;
			m_fRemain = pnetmsg->fRemain;
			m_sScheduleNext = pnetmsg->sScheduleNext;
			m_wLevelReq = pnetmsg->wLevelReq;
			m_wPlayerLimit = pnetmsg->wPlayerLimit;

			if ( m_emState == WOE_STATE_REGISTER ){
				CInnerInterface::GetInstance().PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_WOE_REGISTER );
			}

			m_bRegistered = pnetmsg->bRegistered;

		}break;

	case NET_MSG_GCTRL_WOE_A2C_REGISTER_FB:
		{
			GLMSG::SNETPC_WOE_A2C_REGISTER_FB* pnetmsg = ( GLMSG::SNETPC_WOE_A2C_REGISTER_FB* ) nmg;

			switch( pnetmsg->emFB )
			{
			case WOE_REGISTER_FB_NOTPOSSIBLE:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_NOTPOSSIBLE") );
				}break;

			case WOE_REGISTER_FB_INVALID_CHANNEL:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_INVALID_CHANNEL") );
				}break;

			case WOE_REGISTER_FB_INVALID_CHARID:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_INVALID_CHARID") );
				}break;

			case WOE_REGISTER_FB_INVALID_SCHOOL:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_INVALID_SCHOOL") );
				}break;

			case WOE_REGISTER_FB_LEVEL_LOW:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_LEVEL_LOW") );
				}break;

			case WOE_REGISTER_FB_ALREADY_REG:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_ALREADY_REG") );
				}break;

			case WOE_REGISTER_FB_REGISTERED:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("WOE_REGISTER_FB_REGISTERED") );
					m_bRegistered = TRUE;
				}break;
			
			case WOE_REGISTER_FB_CANCEL_REG:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_CANCEL_REG") );
					m_bRegistered = FALSE;
				}break;

			case WOE_REGISTER_FB_CANCEL_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_CANCEL_FAIL") );
				}break;

			};
		}break;

	case NET_MSG_GCTRL_WOE_A2C_REJOIN_FB:
		{
			GLMSG::SNETPC_WOE_A2C_REJOIN_FB* pnetmsg = ( GLMSG::SNETPC_WOE_A2C_REJOIN_FB* ) nmg;
			switch( pnetmsg->emFB )
			{
			case WOE_REJOIN_FB_FAILED:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REJOIN_FB_FAILED") );
				break;

			case WOE_REJOIN_FB_NOTPOSSIBLE:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REJOIN_FB_NOTPOSSIBLE") );
				break;

			case WOE_REJOIN_FB_NOT_REGISTERED:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REJOIN_FB_NOT_REGISTERED") );
				break;

			case WOE_REJOIN_FB_INVALID_CHARID:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REJOIN_FB_INVALID_CHARID") );
				break;

			case WOE_REJOIN_FB_INBATTLE:
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("WOE_REJOIN_FB_INBATTLE") );
				break;
			};
		}break;

	case NET_MSG_GCTRL_WOE_F2C_TOWER_CAPTURE:
		{
			GLMSG::SNETPC_WOE_F2C_TOWER_CAPTURE* pnetmsg = ( GLMSG::SNETPC_WOE_F2C_TOWER_CAPTURE* ) nmg;

			m_dwEmperiumGuildPrev = pnetmsg->dwClubID;
			m_dwEmperiumMarkVerPrev = pnetmsg->dwMarkVer;

			CInnerInterface::GetInstance().PVPCompetitionShowNotice(NS_UITEXTCOLOR::GREEN_WOW, "%s has been captured the Emperium Crystal", pnetmsg->szName);
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::GREEN_WOW, "%s has been captured the Emperium Crystal", pnetmsg->szName);

		}break;

	case NET_MSG_GCTRL_WOE_A2C_TOWER_INFO:
		{
			GLMSG::SNETPC_WOE_A2C_TOWER_OWNER_INFO* pnetmsg = ( GLMSG::SNETPC_WOE_A2C_TOWER_OWNER_INFO* ) nmg;
		}break;

	case NET_MSG_WOE_A2C_TOWER_WINNER:
		{
			GLMSG::SNET_MSG_WOE_A2C_TOWER_WINNER* pnetmsg = ( GLMSG::SNET_MSG_WOE_A2C_TOWER_WINNER* ) nmg;

			CInnerInterface::GetInstance().ShowGroupFocus(WOE_RANKING_WINDOW);
			CInnerInterface::GetInstance().RefreshWoeRanking();

			switch (pnetmsg->emFB)
			{
				case 0:
					CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::GREEN_WOW, "No guild winner");
				break;
				case 1:
					CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::GREEN_WOW, "Guild Winner: %s", pnetmsg->szNamePrev);
				break;
				case 2:
					CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::GREEN_WOW, "%s guild successfully obtain tower from %s", pnetmsg->szNameCur, pnetmsg->szNamePrev);
				break;
			}
		}break;

	case NET_MSG_GCTRL_WOE_A2C_RANKINFO_PC:
		{
			GLMSG::SNETPC_WOE_A2C_RANKINFO_PC* pnetmsg = ( GLMSG::SNETPC_WOE_A2C_RANKINFO_PC* ) nmg;

			m_dwEmperiumGuildPrev = pnetmsg->dwClubID;
			m_dwEmperiumMarkVerPrev = pnetmsg->dwEmperiumMarkVer;
			strcpy_s(strName, sizeof(strName), pnetmsg->szName);
			strcpy_s(strLeader, sizeof(strLeader), pnetmsg->szLeader);

		}break;
	};

	return S_OK;
}