#include "stdafx.h"
#include "./GLFreePK.h"
#include "./GLGaeaServer.h"
#include "./GLAgentServer.h"
#include "./GLContrlServerMsg.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL _FreePKTSort(const SBATTLE_RANKING& L_Rank, const SBATTLE_RANKING& R_Rank)
{
	return L_Rank.wKill > R_Rank.wKill;
}

GLFreePK& GLFreePK::GetInstance ()
{
	static GLFreePK cInstance;
	return cInstance;
}

GLFreePK::~GLFreePK()
{
}

/*dmk14 freepk*/
void GLFreePK::StartFREEPKMODE( DWORD dwTIME, SNATIVEID sMapID, SNATIVEID sItemReward )
{
	m_fPKTIME      = static_cast<float>( dwTIME );
	m_fUPDATETIMER = 300.0f;
	m_sMapID = sMapID;
} // end of decl

void GLFreePK::EndFREEPKMODE()
{
	m_fTIMER	   = 0.0f;
	m_fPKTIME	   = 0.0f;
	m_fUPDATETIMER = 300.0f;
	m_sMapID = NATIVEID_NULL();
} // end of decl

bool GLFreePK::FrameMove( float fElapsedAppTime )
{
	if ( IsFREEPKMODE() )
	{
		m_fUPDATETIMER -= fElapsedAppTime;
		if ( m_fUPDATETIMER < 0.0f )
		{
			// 성향이벤트 관련 정보 모두에게 알림
			NET_CHAT NetMsg;	
			NetMsg.nmg.nType	= NET_MSG_CHAT_FB;
			NetMsg.emType		= CHAT_TYPE_GLOBAL;
			::StringCchCopyN ( NetMsg.szChatMsg, CHAT_MSG_SIZE, ID2SERVERTEXT("BRIGHT_EVENT_MSG", 0 ), CHAT_MSG_SIZE );
			GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );
			::StringCchCopyN ( NetMsg.szChatMsg, CHAT_MSG_SIZE, ID2SERVERTEXT("BRIGHT_EVENT_MSG", 1 ), CHAT_MSG_SIZE );
			GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );
			::StringCchCopyN ( NetMsg.szChatMsg, CHAT_MSG_SIZE, ID2SERVERTEXT("BRIGHT_EVENT_MSG", 2 ), CHAT_MSG_SIZE );
			GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

			m_fUPDATETIMER = 300.0f;
		}

		m_fPKTIME -= fElapsedAppTime;
		if ( m_fPKTIME <= 0.0f )
		{
			m_fPKTIME = 0.0f;

			//	Note : 성향 이벤트 종료알림
			GLMSG::SNET_GM_FREEPK NetMsgFld;
			NetMsgFld.dwPKTIME = 0;
			NetMsgFld.sMapID = NATIVEID_NULL();		/*dmk14 freepk*/
			GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld ); // 필드에 알리고

			GLMSG::SNET_GM_FREEPK_BRD NetMsgBrd;
			NetMsgBrd.dwPKTIME	   = 0;
			NetMsgBrd.bSTATEUPDATE = false;
			NetMsgBrd.sMapID = NATIVEID_NULL();		/*dmk14 freepk*/
			GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );  // 모두에게 알림
		}
		return true;
	}
	return false;
} // end of decl

HRESULT GLFreePK::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !nmg ){
		return E_FAIL;
	}

	switch( nmg->nType )
	{
	case NET_MSG_GCTRL_GM_FREEPK_F2A_BATTLEINFO_PC:
		{
			GLMSG::SNET_GM_FREEPK_F2A_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_GM_FREEPK_F2A_BATTLEINFO_PC* ) nmg;
			RequestInfoPC( pnetmsg->dwCharID );
		}break;


	};

	return S_OK;
}

void GLFreePK::RequestInfoPC( DWORD dwCharID )
{
	if ( !IsFREEPKMODE() )	return;

	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	//event info
	GLMSG::SNET_GM_FREEPK_A2C_BATTLEINFO_PC netmsgclient;
	netmsgclient.sFreePkID = m_sMapID;
	netmsgclient.dwPKTIME = static_cast<DWORD>(m_fPKTIME);
	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgclient );
}

/*dmk14 freepk*/
GLFreePKField::GLFreePKField()
: m_sItemReward( NATIVEID_NULL())
, m_fPKTIMEField(0.0f)
, m_nState(0)
{
	m_mapBattleRankingP.clear();
	m_vecBattleRankingP.clear();
	m_mapBattleRankingC.clear();
	m_vecBattleRankingC.clear();
}

GLFreePKField::~GLFreePKField()
{
}

void GLFreePKField::EndFREEPKMODE()
{
	m_fPKTIMEField	   = 0.0f;
}

void GLFreePKField::StartFREEPKMODE( DWORD dwTIME, SNATIVEID sMapID, SNATIVEID sItemReward )
{
	m_fPKTIMEField      = static_cast<float>( dwTIME );
	/*dmk14 freepk*/
	m_sMapID = sMapID;
	m_sItemReward = sItemReward;

	m_mapBattleRankingP.clear();
	m_vecBattleRankingP.clear();
	m_mapBattleRankingC.clear();
	m_vecBattleRankingC.clear();

	SetEventState( 1 );

	CONSOLEMSG_WRITE( "Start FPK Time:%2.2f Map:%d-%d Item:%d-%d", m_fPKTIMEField, m_sMapID.wMainID,m_sMapID.wSubID, m_sItemReward.wMainID, m_sItemReward.wSubID );
} 

void GLFreePKField::FrameMove( float fElapsedAppTime )
{
	if( m_nState == 0 )	return;
	//start event state
	if( m_nState == 1 )
	{
		m_fPKTIMEField -= fElapsedAppTime;
		if ( m_fPKTIMEField <= 0.0f )
		{
			SetEventState( 2 );
		}
	}
	//reward state
	else if( m_nState == 2 )
	{
		UpdateBattleRanking();
		SendBattleReward();

		m_fPKTIMEField = 0.0f;
		m_sMapID = NATIVEID_NULL();
		m_sItemReward = NATIVEID_NULL();
		m_mapBattleRankingP.clear();
		m_vecBattleRankingP.clear();
		m_mapBattleRankingC.clear();
		m_vecBattleRankingC.clear();
		SetEventState(0);

		EndFREEPKMODE();
	}
}

void GLFreePKField::AddBattleRanking(DWORD dwKillID, DWORD dwDeathID)
{
	MAP_BATTLE_RANKING_ITER iter_find; 
	iter_find = m_mapBattleRankingP.find(dwKillID);
	if (iter_find != m_mapBattleRankingP.end())
	{
		(*iter_find).second.wKill++;
	}
	else
	{
		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID(dwKillID);
		if (pChar)
		{
			SBATTLE_RANKING sRanking;
			sRanking.dwCharID = dwKillID;
			sRanking.wKill++;
			StringCchCopy(sRanking.szChaName, CHAR_SZNAME, pChar->m_szName);

			m_mapBattleRankingP.insert(std::make_pair(dwKillID, sRanking));
		}
	}

	iter_find = m_mapBattleRankingP.find(dwDeathID);
	if (iter_find != m_mapBattleRankingP.end())
	{
		(*iter_find).second.wDead++;
	}
	else
	{
		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID(dwDeathID);
		if (pChar)
		{
			SBATTLE_RANKING sRanking;
			sRanking.dwCharID	= dwDeathID;
			sRanking.wDead++;
			StringCchCopy(sRanking.szChaName, CHAR_SZNAME, pChar->m_szName);

			m_mapBattleRankingP.insert(std::make_pair(dwDeathID, sRanking));
		}
	}

	PGLCHAR pCharKillClub = GLGaeaServer::GetInstance().GetCharID(dwKillID);
	if (pCharKillClub && pCharKillClub->m_dwGuild != CLUB_NULL )
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( pCharKillClub->m_dwGuild );
		if ( pClub )
		{
			iter_find = m_mapBattleRankingC.find( pCharKillClub->m_dwGuild );
			if (iter_find != m_mapBattleRankingC.end())
			{
				(*iter_find).second.wKill++;
			}
			else
			{

				SBATTLE_RANKING sRanking;
				sRanking.dwCharID = pCharKillClub->m_dwGuild;
				sRanking.wKill++;
				StringCchCopy(sRanking.szChaName, CHAR_SZNAME, pClub->m_szName);

				m_mapBattleRankingC.insert(std::make_pair(pCharKillClub->m_dwGuild, sRanking));
			}
		}
	}

	PGLCHAR pCharDeathClub = GLGaeaServer::GetInstance().GetCharID(dwDeathID);
	if (pCharDeathClub && pCharDeathClub->m_dwGuild != CLUB_NULL)
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( pCharDeathClub->m_dwGuild );
		if ( pClub )
		{
			iter_find = m_mapBattleRankingC.find( pCharDeathClub->m_dwGuild );
			if (iter_find != m_mapBattleRankingC.end())
			{
				(*iter_find).second.wDead++;
			}
			else
			{
				SBATTLE_RANKING sRanking;
				sRanking.dwCharID = pCharDeathClub->m_dwGuild;
				sRanking.wDead++;
				StringCchCopy(sRanking.szChaName, CHAR_SZNAME, pClub->m_szName);

				m_mapBattleRankingC.insert(std::make_pair(pCharDeathClub->m_dwGuild, sRanking));
			}
		}
	}

	UpdateBattleRanking();
}

void GLFreePKField::UpdateBattleRanking()
{
	m_vecBattleRankingP.clear();

	MAP_BATTLE_RANKING_ITER iter_begin	= m_mapBattleRankingP.begin();
	MAP_BATTLE_RANKING_ITER iter_end	= m_mapBattleRankingP.end();

	for (; iter_begin != iter_end; iter_begin++)
		m_vecBattleRankingP.push_back((*iter_begin).second);

	if (!m_vecBattleRankingP.empty())
	{
		sort(m_vecBattleRankingP.begin(), m_vecBattleRankingP.end(), _FreePKTSort);

		for (size_t i = 0; i < m_vecBattleRankingP.size(); i++)
			m_vecBattleRankingP[i].nIndex = i;

		GLMSG::SNET_REQ_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMapID.dwID;

		if (m_vecBattleRankingP.size() < 10)
		{
			for (size_t i = 0; i < m_vecBattleRankingP.size(); i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingP[i];
		}
		else
		{
			for (int i = 0; i < 10; i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingP[i];
		}

		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMapID.dwID, &NetMsg);
	}

	//club rank
	m_vecBattleRankingC.clear();

	MAP_BATTLE_RANKING_ITER iter_begin2	= m_mapBattleRankingC.begin();
	MAP_BATTLE_RANKING_ITER iter_end2	= m_mapBattleRankingC.end();

	for (; iter_begin2 != iter_end2; iter_begin2++)
		m_vecBattleRankingC.push_back((*iter_begin2).second);

	if (!m_vecBattleRankingC.empty())
	{
		sort(m_vecBattleRankingC.begin(), m_vecBattleRankingC.end(), _FreePKTSort);

		for (size_t i = 0; i < m_vecBattleRankingC.size(); i++)
			m_vecBattleRankingC[i].nIndex = i;

		GLMSG::SNET_REQ_CLUB_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMapID.dwID;

		if (m_vecBattleRankingC.size() < 10)
		{
			for (size_t i = 0; i < m_vecBattleRankingC.size(); i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingC[i];
		}
		else
		{
			for (int i = 0; i < 10; i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingC[i];
		}

		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMapID.dwID, &NetMsg);
	}
}
void GLFreePKField::ReqBattleRanking(DWORD dwClientID, int nPage)
{
	if( nPage==2 )
	{
		if( m_vecBattleRankingC.empty() )	return;

		GLMSG::SNET_REQ_CLUB_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMapID.dwID;

		if (m_vecBattleRankingC.size() < 10)
		{
			for (size_t i = 0; i < m_vecBattleRankingC.size(); i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingC[i];
		}
		else
		{
			for (int i = 0; i < 10; i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingC[i];
		}
		GLGaeaServer::GetInstance().SENDTOCLIENT(dwClientID, &NetMsg);
	}
	else
	{
		if (m_vecBattleRankingP.empty()) return;

		GLMSG::SNET_REQ_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMapID.dwID;

		if (m_vecBattleRankingP.size() < 10)
		{
			for (size_t i = 0; i < m_vecBattleRankingP.size(); i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingP[i];
		}
		else
		{
			for (int i = 0; i < 10; i++)
				NetMsg.sRanking_[i] = m_vecBattleRankingP[i];
		}
		GLGaeaServer::GetInstance().SENDTOCLIENT(dwClientID, &NetMsg);
	}	
}
void GLFreePKField::ReqMyBattleRanking(DWORD dwClientID, DWORD dwCharID, int nPage)
{
	if (m_vecBattleRankingP.empty()) return;

	VEC_BATTLE_RANKING_ITER iter_begin	= m_vecBattleRankingP.begin();
	VEC_BATTLE_RANKING_ITER iter_end	= m_vecBattleRankingP.end();

	if( nPage == 2 )	
	{
		iter_begin = m_vecBattleRankingC.begin();
		iter_end = m_vecBattleRankingC.end();
	}

	for (; iter_begin != iter_end; iter_begin++)
	{
		if (dwCharID == (*iter_begin).dwCharID)
		{
			if( nPage == 2 )
			{

				GLMSG::SNET_REQ_MY_CLUB_BATTLE_RANKING_FB NetMsg;
				NetMsg.sRanking = (*iter_begin);
				GLGaeaServer::GetInstance().SENDTOCLIENT(dwClientID, &NetMsg);
			}
			else
			{
				GLMSG::SNET_REQ_MY_BATTLE_RANKING_FB NetMsg;
				NetMsg.sRanking = (*iter_begin);
				GLGaeaServer::GetInstance().SENDTOCLIENT(dwClientID, &NetMsg);
			}
			break;
		}
	}
}

void GLFreePKField::SendBattleReward()
{
	CONSOLEMSG_WRITE( "Send Reward Map:%d-%d Item:%d-%d", m_sMapID.wMainID,m_sMapID.wSubID, m_sItemReward.wMainID, m_sItemReward.wSubID  );
	if (!m_vecBattleRankingP.empty())
	{
		if (m_vecBattleRankingP.size() < 10)
		{
			for (size_t i = 0; i < m_vecBattleRankingP.size(); i++)
			{
				PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID(m_vecBattleRankingP[i].dwCharID);
				if (pChar)
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sItemReward );
					if (pItem)
					{
						SITEMCUSTOM sITEM_NEW;
						sITEM_NEW.sNativeID	= pItem->sBasicOp.sNativeID;
						sITEM_NEW.tBORNTIME	= CTime::GetCurrentTime().GetTime();
						sITEM_NEW.cGenType	= EMGEN_SYSTEM;
						sITEM_NEW.cChnID	= (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
						sITEM_NEW.cFieldID	= (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
						sITEM_NEW.lnGenNum	= GLITEMLMT::GetInstance().RegItemGen(sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType);

						CItemDrop cDropItem;
						cDropItem.sItemCustom = sITEM_NEW;

						if (pChar->IsInsertToInvenEx(&cDropItem))
						{
							pChar->InsertToInvenEx(&cDropItem);
						}
						else
						{
							pChar->m_pLandMan->DropItem(
								pChar->m_vPos,
								&(cDropItem.sItemCustom),
								EMGROUP_ONE,
								pChar->m_dwGaeaID
								);
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID(m_vecBattleRankingP[i].dwCharID);
				if (pChar)
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sItemReward );
					if (pItem)
					{
						SITEMCUSTOM sITEM_NEW;
						sITEM_NEW.sNativeID	= pItem->sBasicOp.sNativeID;
						sITEM_NEW.tBORNTIME	= CTime::GetCurrentTime().GetTime();
						sITEM_NEW.cGenType	= EMGEN_SYSTEM;
						sITEM_NEW.cChnID	= (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
						sITEM_NEW.cFieldID	= (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
						sITEM_NEW.lnGenNum	= GLITEMLMT::GetInstance().RegItemGen(sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType);

						CItemDrop cDropItem;
						cDropItem.sItemCustom = sITEM_NEW;

						if (pChar->IsInsertToInvenEx(&cDropItem))
						{
							pChar->InsertToInvenEx(&cDropItem);
						}
						else
						{
							pChar->m_pLandMan->DropItem(
								pChar->m_vPos,
								&(cDropItem.sItemCustom),
								EMGROUP_ONE,
								pChar->m_dwGaeaID
								);
						}
					}
				}
			}
		}
	}

	m_mapBattleRankingP.clear();
	m_vecBattleRankingP.clear();
	m_mapBattleRankingC.clear();
	m_vecBattleRankingC.clear();
}

void GLFreePKField::RequestInfoPC( DWORD dwCharID )
{
	if( m_nState == 0 )	return;

	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;
	
	GLMSG::SNET_GM_FREEPK_F2A_BATTLEINFO_PC netmsg2agent;
	netmsg2agent.dwCharID = dwCharID;
	GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg2agent );
}


GLFreePKField& GLFreePKField::GetInstance()
{
	static GLFreePKField cInstance;
	return cInstance;
}