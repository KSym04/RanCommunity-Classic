#include "stdafx.h"
#include "GLPVPWoeField.h"
#include "GLGaeaServer.h"
#include "GLLandMan.h"

#include "../DbActionLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPVPWoeField& GLPVPWoeField::GetInstance()
{
	static GLPVPWoeField cInstance;
	return cInstance;
}

void GLPVPWoeField::LoadEvent( std::string strFile )
{
	ResetData();

	bool bok = Load( strFile );
	if ( bok )
	{
		FindScheduleFirst();
	}
}

bool GLPVPWoeField::SetMapState ()
{
	if( m_sMap.IsValidNativeID() )
	{
		SMAPNODE *pmapnode = GLGaeaServer::GetInstance().FindMapNode ( m_sMap );
		if ( pmapnode )
			pmapnode->bPVPWoeZone = true;

		GLLandMan* plandman = GLGaeaServer::GetInstance().GetByMapID ( m_sMap );
		if ( plandman )
		{
			plandman->m_bPVPWoeMap = true;	
			m_bValidInstance = TRUE;
		}
	}

	return true;
}

bool GLPVPWoeField::FrameMove ( float fElaps )
{
	m_fUpdateScore += fElaps;

	if (m_fUpdateScore >= 10.0f)
	{
		UpdateGuildScore();
		UpdatePlayerScore();
		UpdateResuScore();
		m_fUpdateScore = 0.0f;
	}

	return true;
}

void GLPVPWoeField::ResetData()
{
	m_emState = WOE_STATE_ENDED;
	m_sScheduleNext = WOE_SCHED_NEXT();
	m_vecSchedule.clear();

	m_sWoeTowerData.dwGuildOwner = 0;

	m_bValidInstance = FALSE;
}

void GLPVPWoeField::DoStateRegister()
{
	m_emState = WOE_STATE_REGISTER;
}

void GLPVPWoeField::DoStateBattle()
{
	//reset towers on land
	PGLLANDMAN pland = GLGaeaServer::GetInstance().GetByMapID( m_sMap );
	if ( pland )
		pland->ResetWoeTower();

	m_sWoeTowerData.dwGuildOwner = 0;

	m_emState = WOE_STATE_BATTLE;
}

void GLPVPWoeField::DoStateReward()
{
	if ( m_bValidInstance )
	{
		PointSend();
		WoeGuildReward();
		WoePlayerReward();
		WoeResuReward();
	}

	m_emState = WOE_STATE_REWARD;
}

void GLPVPWoeField::DoStateEnd()
{
	m_emState = WOE_STATE_ENDED;
}

void GLPVPWoeField::RequestInfoPC( DWORD dwCharID, BOOL bWoeMap)
{
	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNET_WOE_F2A_BATTLEINFO_PC netmsg2agent;
	netmsg2agent.dwCharID = dwCharID;
	netmsg2agent.bWoeMap = bWoeMap;
	GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg2agent );
}

WOE_PLAYER_DATA* GLPVPWoeField::PlayerDataGet( DWORD dwCharID )
{
	if ( dwCharID == WOE_PLAYER_NULL ) return NULL;

	{
		WOE_PLAYER_DATA_MAP_ITER iterdata = m_mapGuildData.find( dwCharID );
		if( iterdata != m_mapGuildData.end() ){
			return &(*iterdata).second;
		}
	}

	return NULL;
};

void GLPVPWoeField::PlayerRevive( DWORD dwGaeaID, DWORD dwCharID )
{
	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_REQ_REVIVE_FB netmsgclient;

	if ( m_emState != WOE_STATE_BATTLE &&
		m_emState != WOE_STATE_REWARD )
	{
		netmsgclient.emFB = EMREQ_REVIVE_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
		return;
	}
	
	//BOOL brevive = pchar->PVPWoeRevive(pchar->m_dwGuild);
	BOOL brevive = pchar->PVPWoeRevive(m_wSchoolGate[pchar->m_wSchool]);
	if ( !brevive )
	{
		netmsgclient.emFB = EMREQ_REVIVE_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
	}
}

void GLPVPWoeField::PointSend()
{
	WOE_PLAYER_DATA_VEC vecdata;
	vecdata.reserve( m_mapGuildData.size());

	for( WOE_PLAYER_DATA_MAP_ITER iterplayer = m_mapGuildData.begin();
		iterplayer != m_mapGuildData.end(); ++ iterplayer ){
			const WOE_PLAYER_DATA& splayerdata = (*iterplayer).second;
			if ( splayerdata.wRewardPoint == 0 )	continue;
			vecdata.push_back( splayerdata );
	}

	for ( size_t i=0; i<vecdata.size(); ++i )
	{
		const WOE_PLAYER_DATA& splayer_data = vecdata[i];

		PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( splayer_data.dwInfoCharID );
		if ( !pchar ) continue;

		pchar->m_dwContributionPoint += splayer_data.wRewardPoint;

		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netmsgclient;
		netmsgclient.bNotice = true;
		netmsgclient.dwPoint = pchar->m_dwContributionPoint;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
	}
}

void GLPVPWoeField::EmperiumCapture(SNATIVEID sidMob, WORD wClubID)
{
	if (wClubID == CLUB_NULL)				return;
	if (m_emState != TYRANNY_STATE_BATTLE)	return;

	GLMSG::SNETPC_WOE_F2A_TOWER_CAPTURE netmsgagent;
	netmsgagent.sidMob = sidMob;
	netmsgagent.wClubID = wClubID;
	GLGaeaServer::GetInstance().SENDTOAGENT(&netmsgagent);
}

void GLPVPWoeField::DeclareEmperiumWinner(const TCHAR* szName)
{
	GLMSG::SNET_MSG_WOE_F2C_DECLARE_WINNER netmsgfield;
	StringCchCopy(netmsgfield.szName, CLUB_NAME + 1, szName);
	GLGaeaServer::GetInstance().SENDTOALLCLIENT(&netmsgfield);

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write("Result: %s", szName);
}

void GLPVPWoeField::WoeGuildReward()
{
	GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID(m_sMap.dwID);
	if (!pLandMan) return;
	{
		GLClubMan& cClubMan = GLGaeaServer::GetInstance().GetClubMan();

		GLCHARNODE* pCharNode = pLandMan->m_GlobPCList.m_pHead;
		for (; pCharNode; pCharNode = pCharNode->pNext)
		{
			PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar(pCharNode->Data->m_dwGaeaID);
			if (!pChar) continue;

			GLCLUB* pClub = cClubMan.GetClub(pChar->m_dwGuild);
			if (!pClub) continue;

			WOE_GUILD_RANK_INFO_MAP_ITER iter = m_mapWoeScore.find(pClub->m_dwID);
			if (iter != m_mapWoeScore.end())
			{
				SWOE_GUILD_RANK_INFO& sWoeRankInfo = iter->second;

				WOE_AWARD_CHAR_ITER iter = m_vecWoeAwardChar.find(pChar->m_dwCharID);
				if (iter != m_vecWoeAwardChar.end())	continue;

				/*int nAwardIndex = 0;
				if (sWoeRankInfo.wClubRanking < 10 ) 
					nAwardIndex = sWoeRankInfo.wClubRanking;
				else continue;*/
				int nAwardIndex = 0;
				if (sWoeRankInfo.wClubRanking == 1)  // Check if the guild is ranked 1st
					nAwardIndex = 0;  // You can assign whatever reward index you want for the top guild
				else
					continue;  // Skip the rest of the guilds (non-1st ranked guilds)


				SITEM* pItem = GLItemMan::GetInstance().GetItem(m_sItemRewardGuild);
				if (!pItem) continue;

				SITEMCUSTOM sITEM_NEW;
				sITEM_NEW.sNativeID = pItem->sBasicOp.sNativeID;
				sITEM_NEW.tBORNTIME = CTime::GetCurrentTime().GetTime();
				sITEM_NEW.cGenType = EMGEN_SYSTEM;
				sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
				sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
				sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen(sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType);

				SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem(sITEM_NEW.sNativeID);
				if (pitem_data_check && pitem_data_check->sBasicOp.bItemColor)
				{
					sITEM_NEW.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
					sITEM_NEW.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
				}

				CItemDrop cDropItem;
				cDropItem.sItemCustom = sITEM_NEW;
				if (pChar->IsInsertToInvenEx(&cDropItem))
				{
					pChar->InsertToInvenEx(&cDropItem);

					GLITEMLMT::GetInstance().ReqItemRoute(sITEM_NEW, ID_CLUB, pChar->m_dwGuild, ID_CHAR, pChar->m_dwCharID, EMITEM_ROUTE_SYSTEM, sITEM_NEW.wTurnNum);
				}
				else
				{
					pLandMan->DropItem(pChar->m_vPos, &(cDropItem.sItemCustom), EMGROUP_ONE, pChar->m_dwGaeaID);
				}
				
				m_vecWoeAwardChar.insert(pChar->m_dwCharID);
			}
		}
	}
}

void GLPVPWoeField::WoePlayerReward()
{
	WOE_PLAYER_RANK_INFO_VEC m_vecPlayer;

	WOE_PLAYER_RANK_INFO_MAP_ITER pos = m_mapWoePlayerScore.begin();
	WOE_PLAYER_RANK_INFO_MAP_ITER end = m_mapWoePlayerScore.end();

	for (; pos != end; pos++)
	{
		SWOE_PLAYER_RANK_INFO& sPlayerInfo = (*pos).second;

		PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID(sPlayerInfo.dwCharID);
		if (!pchar)		continue;

		SITEM* sitem = GLItemMan::GetInstance().GetItem(m_sItemRewardPlayer);
		if (!sitem)		continue;

		int nAwardIndex = 0;
		if (sPlayerInfo.wRanking < 10)
			nAwardIndex = sPlayerInfo.wRanking;
		else continue;
		
		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = sitem->sBasicOp.sNativeID;
		sITEM_NEW.tBORNTIME = CTime::GetCurrentTime().GetTime();
		sITEM_NEW.cGenType = EMGEN_SYSTEM;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen(sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType);

		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem(sITEM_NEW.sNativeID);
		if (pitem_data_check && pitem_data_check->sBasicOp.bItemColor)
		{
			sITEM_NEW.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			sITEM_NEW.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		CItemDrop cDropItem;
		cDropItem.sItemCustom = sITEM_NEW;
		if (pchar->IsInsertToInvenEx(&cDropItem))
		{
			pchar->InsertToInvenEx(&cDropItem);
			GLITEMLMT::GetInstance().ReqItemRoute(sITEM_NEW, ID_CLUB, pchar->m_dwGuild, ID_CHAR, pchar->m_dwCharID, EMITEM_ROUTE_SYSTEM, sITEM_NEW.wTurnNum);
		}

		m_vecPlayer.push_back(sPlayerInfo);

	}
}

void GLPVPWoeField::WoeResuReward()
{
	WOE_RESU_PLAYER_INFO_VEC m_vecResu;

	WOE_RESU_PLAYER_INFO_MAP_ITER pos = m_mapWoeResuScore.begin();
	WOE_RESU_PLAYER_INFO_MAP_ITER end = m_mapWoeResuScore.end();

	for (; pos != end; pos++)
	{
		SWOE_RESU_PLAYER_INFO& sResuInfo = (*pos).second;

		SITEM* sitem = GLItemMan::GetInstance().GetItem(m_sItemRewardResu);
		if (!sitem)		continue;

		PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID(sResuInfo.dwCharID);
		if (!pchar)		continue;

		int nAwardIndex = 0;
		if (sResuInfo.wRanking < 10)
			nAwardIndex = sResuInfo.wRanking;
		else continue;

		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = sitem->sBasicOp.sNativeID;
		sITEM_NEW.tBORNTIME = CTime::GetCurrentTime().GetTime();
		sITEM_NEW.cGenType = EMGEN_SYSTEM;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen(sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType);

		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem(sITEM_NEW.sNativeID);
		if (pitem_data_check && pitem_data_check->sBasicOp.bItemColor)
		{
			sITEM_NEW.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			sITEM_NEW.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		CItemDrop cDropItem;
		cDropItem.sItemCustom = sITEM_NEW;
		if (pchar->IsInsertToInvenEx(&cDropItem))
		{
			pchar->InsertToInvenEx(&cDropItem);
			GLITEMLMT::GetInstance().ReqItemRoute(sITEM_NEW, ID_CLUB, pchar->m_dwGuild, ID_CHAR, pchar->m_dwCharID, EMITEM_ROUTE_SYSTEM, sITEM_NEW.wTurnNum);
		}

		m_vecResu.push_back(sResuInfo);
	}
}