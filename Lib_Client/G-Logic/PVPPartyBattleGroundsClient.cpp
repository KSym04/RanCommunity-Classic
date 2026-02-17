#include "stdafx.h"
#include "./PVPPartyBattleGroundsClient.h"

#include "./GLGaeaClient.h"
#include "./PVPPartyBattleGroundsData.h"
#include "./PVPPartyBattleGroundsMsg.h"

#include "../DxGlobalStage.h"


#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"

#include "../../Lib_Helper/RPCID/RPCID.h"
#include "../../Lib_Helper/RPCID/RPCIDSMBIOS.h"
#include "../../Lib_Helper/RPCID/RPCIDMAC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace PVPPBG
{
	ManagerClient& ManagerClient::GetInstance()
	{
		static ManagerClient cInstance;
		return cInstance;
	}

	ManagerClient::ManagerClient()
		: m_emState(PVPPBG::EVENT_STATE_ENDED)
		, m_fRemain(0.0f)

		, m_wLevelReq(1)
		, m_wPlayerLimit(1)
		, m_dwContriReq(1)

 		, m_wBattleDuration(1)

 		, m_bRegistered(false)
		, m_bQueued(FALSE)
		, m_wQueueNum(0)

		, m_bShowStartNotice(FALSE)
		
	{

	}

	ManagerClient::~ManagerClient()
	{

	}

	bool ManagerClient::FrameMove ( float fTime, float fElapsedTime )
	{
		if( m_emState == PVPPBG::EVENT_STATE_REGISTER ||
			m_emState == PVPPBG::EVENT_STATE_BATTLE ||
			m_emState == PVPPBG::EVENT_STATE_REWARD )
		{
			if ( m_fRemain > 0.0f )
			{
				m_fRemain -= fElapsedTime;
			}
		}

		if ( m_emState == PVPPBG::EVENT_STATE_BATTLE )
		{
		}

		return true;
	}

	void ManagerClient::SetEventState( PVPPBG::EVENT_STATE emState )
	{
		switch( emState )
		{
		case PVPPBG::EVENT_STATE_ENDED:
			DoStateEnd();
			break;

		case PVPPBG::EVENT_STATE_REGISTER:
			DoStateRegister();
			break;

		case PVPPBG::EVENT_STATE_BATTLE:
			DoStateBattle();
			break;

		case PVPPBG::EVENT_STATE_REWARD:
			DoStateReward();
			break;
		};

		m_emState = emState;
	}

	void ManagerClient::DoStateRegister()
	{
		CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_PBG_REG") );
		CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_PBG_REG") );
	}

	void ManagerClient::DoStateBattle()
	{
		CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_PBG_BATTLE") );
		CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_PBG_BATTLE") );

		m_PlayerData = PVPPBG::PLAYER_DATA();

		for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
			m_sLastTop[i] = PVPPBG::PLAYER_DATA();

		m_vecRanking.clear();
	}

	void ManagerClient::DoStateReward()
	{
		CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_PBG_REWARD") );
		CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_PBG_REWARD") );
	}

	void ManagerClient::DoStateEnd()
	{
		CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_PBG_ENDED") );
		CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_PBG_ENDED") );

		m_fRemain = 0.0f;
 		m_bRegistered = false;
		m_bQueued = false;
		m_wQueueNum = 0;

 		m_bShowStartNotice = FALSE;

		m_PlayerData = PVPPBG::PLAYER_DATA();

		m_vecRanking.clear();
	}
	
	void ManagerClient::NotifyTimeToStart( WORD wTime )
	{
		if ( wTime > 0 )
		{
			if ( wTime <= 1 )
			{
				CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_PBG_TOBATTLE2"), wTime );
				CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_PBG_TOBATTLE2"), wTime );
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_PBG_TOBATTLE"), wTime );
				CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_PBG_TOBATTLE"), wTime );
			}
		}
	}

	PLAYER_DATA* ManagerClient::GetOwnRanking()
	{
		if ( m_vecRanking.empty() )	
			return NULL;

		DWORD dwcharid = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;

		for( size_t i=0; i<m_vecRanking.size(); ++i )
		{
			PLAYER_DATA& sranking = m_vecRanking[i];
			if ( dwcharid == sranking.dwInfoCharID )
			{
				return &sranking;
				break;
			}
		}

		return NULL;
	}

	int ManagerClient::GetRankingSize()
	{
		return (int)m_vecRanking.size();
	}

	PLAYER_DATA_VEC& ManagerClient::GetRankingVector()
	{
		return m_vecRanking;
	}

	HRESULT ManagerClient::MsgProcess ( NET_MSG_GENERIC* nmg )
	{
		if ( !nmg )
			return E_FAIL;

		switch ( nmg->nType )
		{
		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REGISTER:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_REGISTER* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_REGISTER* ) nmg;
				SetEventState( PVPPBG::EVENT_STATE_REGISTER );
				m_fRemain = pnetmsg->fTime;

				CInnerInterface::GetInstance().PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_PVP_PBG_REGISTER );

			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_BATTLE:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_BATTLE* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_BATTLE* ) nmg;
				SetEventState( PVPPBG::EVENT_STATE_BATTLE );
				m_fRemain = pnetmsg->fTime;
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REWARD:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_REWARD* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_REWARD* ) nmg;
				SetEventState( PVPPBG::EVENT_STATE_REWARD );
				m_fRemain = pnetmsg->fTime;
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_ENDED:
			{
				GLMSG::SNET_PVP_PBG_A2FC_STATE_ENDED* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_STATE_ENDED* ) nmg;
				SetEventState( PVPPBG::EVENT_STATE_ENDED );
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_TOBATTLE_TIME:
			{
				GLMSG::SNET_PVP_PBG_A2C_TOBATTLE_TIME* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2C_TOBATTLE_TIME* ) nmg;
				NotifyTimeToStart( pnetmsg->wTime );
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2FC_NEXTSCHED:
			{
				GLMSG::SNET_PVP_PBG_A2FC_NEXTSCHED* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2FC_NEXTSCHED* ) nmg;
				m_sScheduleNext = pnetmsg->sScheduleNext;
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_BATTLEINFO_PC:
			{
				GLMSG::SNET_PVP_PBG_A2C_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_PVP_PBG_A2C_BATTLEINFO_PC* ) nmg;
				m_emState = pnetmsg->emState;
				m_fRemain = pnetmsg->fRemain;
				m_sScheduleNext = pnetmsg->sScheduleNext;
				m_wLevelReq = pnetmsg->wLevelReq;
				m_dwContriReq = pnetmsg->dwContriReq;
				m_wPlayerLimit = pnetmsg->wPlayerLimit;
				m_wBattleDuration = pnetmsg->wBattleTime;

				if ( m_emState == PVPPBG::EVENT_STATE_REGISTER )
				{
					CInnerInterface::GetInstance().PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_PVP_PBG_REGISTER );
				}

				if ( m_emState != PVPPBG::EVENT_STATE_REWARD )
				{
					m_vecRanking.clear();
				}

				m_bRegistered = pnetmsg->bRegistered;
				m_bQueued = pnetmsg->bQueued;
				m_wQueueNum = pnetmsg->wQueueNum;
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_REGISTER_FB:
			{
				GLMSG::SNETPC_PVP_PBG_A2C_REGISTER_FB* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_A2C_REGISTER_FB* ) nmg;

				switch( pnetmsg->emFB )
				{
				case GLMSG::PVP_PBG_REGISTER_FB_NOTPOSSIBLE:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_NOTPOSSIBLE") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_INVALID_CHANNEL:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_INVALID_CHANNEL") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_INVALID_CHARID:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_INVALID_CHARID") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_LEVEL_LOW:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_LEVEL_LOW") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_CONTRI_LOW:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_CONTRI_LOW") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_ALREADY_REG:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_ALREADY_REG") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_ALREADY_QUEUE:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_ALREADY_QUEUE") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_QUEUED:
					{
						CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_QUEUED") );
						m_bRegistered = TRUE;
						if ( pnetmsg->wQueueNum != 0 )
						{
							m_bQueued = TRUE;
							m_wQueueNum = pnetmsg->wQueueNum;
							CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_QUEUED_NUM"), m_wQueueNum );
						}
					}
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_REGISTERED:
					{
						CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_REGISTERED") );
						m_bRegistered = TRUE;
						m_bQueued = FALSE;
						m_wQueueNum = 0;
					}
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_CANCEL_REG:
					{
						CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_CANCEL_REG") );
						m_bRegistered = FALSE;
						m_bQueued = FALSE;
						m_wQueueNum = 0;
					}
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_CANCEL_FAIL:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_CANCEL_FAIL") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_DEVICE_INFO_ERROR:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_DEVICE_INFO_ERROR") );
					break;

				case GLMSG::PVP_PBG_REGISTER_FB_DEVICE_REGISTERED:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_DEVICE_REGISTERED") );
					break;

				};
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_REJOIN_FB:
			{
				GLMSG::SNETPC_PVP_PBG_A2C_REJOIN_FB* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_A2C_REJOIN_FB* ) nmg;
				switch( pnetmsg->emFB )
				{
				case GLMSG::PVP_PBG_REJOIN_FB_FAILED:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REJOIN_FB_FAILED") );
					break;

				case GLMSG::PVP_PBG_REJOIN_FB_NOTPOSSIBLE:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REJOIN_FB_NOTPOSSIBLE") );
					break;

				case GLMSG::PVP_PBG_REJOIN_FB_NOT_REGISTERED:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REJOIN_FB_NOT_REGISTERED") );
					break;

				case GLMSG::PVP_PBG_REJOIN_FB_INVALID_CHARID:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REJOIN_FB_INVALID_CHARID") );
					break;

				case GLMSG::PVP_PBG_REJOIN_FB_INBATTLE:
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REJOIN_FB_INBATTLE") );
					break;
				};
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_QUEUE_MOVED:
			{
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_REGISTERED") );
				m_bRegistered = TRUE;
				m_bQueued = FALSE;
				m_wQueueNum = 0;
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_QUEUE_UPDATE:
			{
				GLMSG::SNETPC_PVP_PBG_A2C_QUEUE_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_A2C_QUEUE_UPDATE* ) nmg;
				if ( pnetmsg->wQueueNum != 0 && m_bQueued && m_bRegistered )
				{
					m_wQueueNum = pnetmsg->wQueueNum;
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_QUEUED_NUM"), m_wQueueNum );
				}
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_F2C_PLAYER_SCORE_UPDATE:
			{
				GLMSG::SNETPC_PVP_PBG_F2C_PLAYER_SCORE_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_F2C_PLAYER_SCORE_UPDATE* ) nmg;
				m_PlayerData = pnetmsg->PlayerData;

			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_A2C_RANKINFO_PC:
			{
				GLMSG::SNETPC_PVP_PBG_A2C_RANKINFO_PC* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_A2C_RANKINFO_PC* ) nmg;
	
				for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
					m_sLastTop[i] = pnetmsg->sPlayerData[i];

			}break;


		case NET_MSG_GCTRL_PVP_PBG_F2C_RANKING_UPDATE:
			{
				GLMSG::SNETPC_PVP_PBG_F2C_RANKING_UPDATE* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_F2C_RANKING_UPDATE* ) nmg;
				for( int i=0; i<pnetmsg->wRankNum; ++i )
				{
					m_vecRanking.push_back( pnetmsg->sPlayerData[i] );
				}
			}break;

		case NET_MSG_GCTRL_PVP_PBG_F2C_RANKING_END:
			{
				GLMSG::SNETPC_PVP_PBG_F2C_RANKING_END* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_F2C_RANKING_END* ) nmg;
				std::sort( m_vecRanking.begin(), m_vecRanking.end() );

				CInnerInterface::GetInstance().PVPPBGShowRanking();
			}break;

		};


		return S_OK;
	}


	void ManagerClient::DoRegister()
	{
		if ( GLGaeaClient::GetInstance().GetCharacter()->isPetLastUseTime() )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("PET_LAST_USE_ACTIVE") );
			return;
		}

		if ( m_emState != PVPPBG::EVENT_STATE_REGISTER && m_emState != PVPPBG::EVENT_STATE_BATTLE )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_NOTPOSSIBLE") );
			return;
		}

		if( DxGlobalStage::GetInstance().GetChannel() != 0 )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_INVALID_CHANNEL") );
			return;
		}

		if ( m_emState == PVPPBG::EVENT_STATE_BATTLE && !m_bRegistered )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_PBG_REGISTER_FB_NOTPOSSIBLE") );
			return;
		}

		if ( m_emState == PVPPBG::EVENT_STATE_REGISTER )
		{
 			GLMSG::SNETPC_PVP_PBG_C2A_REGISTER_REQ netmsgregister;
 			netmsgregister.bRegister = !m_bRegistered;
 			netmsgregister.wLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;
 			netmsgregister.dwContri = GLGaeaClient::GetInstance().GetCharacter()->m_dwContributionPoint;
 			netmsgregister.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
 
 			/* PVPSchoolWars hash check, Juver, 2021/07/17 */
 			SPCID_SMBIOS sPCID_SMBIOS;
 			RPCID::getSMBIOS( sPCID_SMBIOS );
 			std::string strHashCombined("");
 			strHashCombined += RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str();
 			strHashCombined += RPCID::getHashWindowsMAC().c_str();
 			::StringCchCopy(netmsgregister.szHASH, PVP_PBG_PLAYER_HASH_SIZE+1, strHashCombined.c_str() );
 
 			NETSEND ( &netmsgregister );
		}

		if ( m_emState == PVPPBG::EVENT_STATE_BATTLE && m_bRegistered )
		{
			if ( GLGaeaClient::GetInstance().GetActiveMap()->m_bPVPPBGBattleMap ||
				GLGaeaClient::GetInstance().GetActiveMap()->m_bPVPPBGLobbyMap )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_PBG_REJOIN_FB_INBATTLE") );
				return;
			}

			GLMSG::SNETPC_PVP_PBG_C2A_REJOIN_REQ netmsgrejoin;
			netmsgrejoin.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &netmsgrejoin );
		}

		GLGaeaClient::GetInstance().GetCharacter()->resetPetBlockTime();


	}

	void ManagerClient::RequestScoreInfo()
	{
		PLANDMANCLIENT plandclient = GLGaeaClient::GetInstance().GetActiveMap();
		if ( !plandclient )	
			return;

		if ( !plandclient->m_bPVPPBGLobbyMap && !plandclient->m_bPVPPBGBattleMap )	
			return;

		GLMSG::SNETPC_PVP_PBG_C2F_SCORE_INFO_REQ netmsginforeq;
		netmsginforeq.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		NETSENDTOFIELD ( &netmsginforeq );
	}

	void ManagerClient::DoRevive()
	{
		if ( !GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
		{
			return;
		}

		if ( m_emState != PVPPBG::EVENT_STATE_BATTLE && m_emState != PVPPBG::EVENT_STATE_REWARD )
		{
			GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
			CInnerInterface::GetInstance().CloseAllWindow();
			return;
		}

		CInnerInterface::GetInstance().CloseAllWindow();
		GLGaeaClient::GetInstance().GetCharacter()->SetSTATE(EM_ACT_DIE);

		GLMSG::SNETPC_PVP_PBG_C2AF_REVIVE_REQ netmsgrevive;
		netmsgrevive.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		NETSEND ( &netmsgrevive );
	}

	void ManagerClient::RequestMapExit()
	{
		GLMSG::SNETPC_PVP_PBG_C2A_MAPEXIT_REQ netmsgexit;
		netmsgexit.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		NETSEND ( &netmsgexit );
	}
};