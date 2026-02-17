#include "StdAfx.h"
#include "./GLPVPTyrannyField.h"
#include "./GLGaeaServer.h"
#include "./GLLandMan.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPVPTyrannyField& GLPVPTyrannyField::GetInstance()
{
	static GLPVPTyrannyField cInstance;
	return cInstance;
}

void GLPVPTyrannyField::LoadEvent( std::string strFile )
{
	if ( !RPARAM::bEventTyranny )	return;

	ResetData();

	bool bok = Load( strFile );
	if ( bok )
	{
		FindScheduleFirst();
	}
}

bool GLPVPTyrannyField::SetMapState ()
{
	for ( size_t i=0; i<m_vecMapRandom.size(); ++i )
	{
		const SNATIVEID& sMapID = m_vecMapRandom[i];

		if( sMapID.IsValidNativeID() )
		{
			SMAPNODE *pmapnode = GLGaeaServer::GetInstance().FindMapNode ( sMapID );
			if ( pmapnode )
				pmapnode->bPVPTyrannyZone = true;

			GLLandMan* plandman = GLGaeaServer::GetInstance().GetByMapID ( sMapID );
			if ( plandman )
			{
				plandman->m_bPVPTyrannyMap = true;	
				m_bValidInstance = TRUE;
			}
		}
	}

	return true;
}

bool GLPVPTyrannyField::FrameMove ( float fElaps )
{
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	if( m_emState == TYRANNY_STATE_BATTLE )
	{
		m_fTyrannyRankingDisplayUpdate += fElaps;

		if ( m_fTyrannyRankingDisplayUpdate > 60.0f )
		{
			UpdateTyrannyClubRanking();
			UpdateTyrannyRanking();
			UpdateTyrannyRankingResu();
			m_fTyrannyRankingDisplayUpdate = 0.0f;
		}
	}

	return true;
}

void GLPVPTyrannyField::ResetData()
{
	m_emState = TYRANNY_STATE_ENDED;
	m_sScheduleNext = TYRANNY_SCHED_NEXT();
	m_vecSchedule.clear();
	m_mapBuffData.clear();

	m_mapPlayerDataSG.clear();
	m_mapPlayerDataMP.clear();
	m_mapPlayerDataPHX.clear();

	m_mapRewarded.clear();

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	m_mapTyrannyClubScore.clear();
	m_mapTyrannyScore.clear();
	m_mapTyrannyScoreResu.clear();

	for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
	{
		m_sTowerData[i].wOwner = TYRANNY_SCHOOL_SIZE;
		m_dwGuiID[i] = CLUB_NULL;
		m_dwMarkVer[i] = 0;
	}

	m_bValidInstance = FALSE;

	/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
	m_bDisableOtherSchoolResu = FALSE;

	/*PVPTyranny disable score self heal, EJCode, 2018/11/28 */
	m_bDisableScoreSelfHeal = FALSE;
}

void GLPVPTyrannyField::DoStateRegister()
{
	//clear data at register start
	m_mapPlayerDataSG.clear();
	m_mapPlayerDataMP.clear();
	m_mapPlayerDataPHX.clear();

	m_mapRewarded.clear();

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	m_mapTyrannyClubScore.clear();
	m_mapTyrannyScore.clear();
	m_mapTyrannyScoreResu.clear();

	m_emState = TYRANNY_STATE_REGISTER;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLPVPTyrannyField Change State to Register Start" );
	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyField Change State to Register Start" );
}

void GLPVPTyrannyField::DoStateBattle()
{

	//reset towers on land
	PGLLANDMAN pland = GLGaeaServer::GetInstance().GetByMapID( m_sScheduleNext.getBatteMap() );
	if ( pland )
		pland->ResetTyrannyTower();

	//reset tower owner
	for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
	{
		m_sTowerData[i].wOwner = TYRANNY_SCHOOL_SIZE;
		m_dwGuiID[i] = CLUB_NULL;
		m_dwMarkVer[i] = 0;
	}

	m_emState = TYRANNY_STATE_BATTLE;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLPVPTyrannyField Change State to Battle Start" );
	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyField Change State to Battle Start" );
}

void GLPVPTyrannyField::DoStateReward( WORD wSchoolWinner )
{
	if ( m_bValidInstance )
	{
		//calculate final scores
		ScoreCalculate( m_mapPlayerDataSG, wSchoolWinner );
		ScoreCalculate( m_mapPlayerDataMP, wSchoolWinner );
		ScoreCalculate( m_mapPlayerDataPHX, wSchoolWinner );

		//sort rankings per school
		CalculateRankingSchool();

		//sort overall rank
		CalculateRankingAll();

		//send rankings to all players on map
		RankingSend();

		//send contribution points
		PointSend();

		//check reward
		GetRewards( wSchoolWinner );

		//send rewards
		RewardSend();

		//check if rewards were sent
		CheckRewards();

		//log result to db
		/* tyranny result log, Juver, 2021/09/09 */
		LogTyrannyResult( wSchoolWinner );

	}
	
	m_emState = TYRANNY_STATE_REWARD;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLPVPTyrannyField Change State to Award Start" );
	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyField Change State to Award Start" );
}

void GLPVPTyrannyField::DoStateEnd()
{
	m_mapPlayerDataSG.clear();
	m_mapPlayerDataMP.clear();
	m_mapPlayerDataPHX.clear();

	m_mapRewarded.clear();

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	m_mapTyrannyClubScore.clear();
	m_mapTyrannyScore.clear();
	m_mapTyrannyScoreResu.clear();

	m_emState = TYRANNY_STATE_ENDED;

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLPVPTyrannyField Change State to Event End" );
	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyField Change State to Event End" );
}

/*global buffs map setting, Juver, 2018/01/23 */
void GLPVPTyrannyField::RequestInfoPC( DWORD dwCharID, BOOL bTyrannyMap, BOOL bDisableSystemBuffs )
{
	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNET_TYRANNY_F2A_BATTLEINFO_PC netmsg2agent;
	netmsg2agent.dwCharID = dwCharID;
	netmsg2agent.bTyrannyMap = bTyrannyMap;
	netmsg2agent.bDisableSystemBuffs = bDisableSystemBuffs;
	GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg2agent );
}

TYRANNY_PLAYER_DATA* GLPVPTyrannyField::PlayerDataGet( DWORD dwCharID )
{
	if ( dwCharID == TYRANNY_PLAYER_NULL ) return NULL;

	{
		TYRANNY_PLAYER_DATA_MAP_ITER iterdata = m_mapPlayerDataSG.find( dwCharID );
		if( iterdata != m_mapPlayerDataSG.end() ){
			return &(*iterdata).second;
		}
	}

	{
		TYRANNY_PLAYER_DATA_MAP_ITER iterdata = m_mapPlayerDataMP.find( dwCharID );
		if( iterdata != m_mapPlayerDataMP.end() ){
			return &(*iterdata).second;
		}
	}

	{
		TYRANNY_PLAYER_DATA_MAP_ITER iterdata = m_mapPlayerDataPHX.find( dwCharID );
		if( iterdata != m_mapPlayerDataPHX.end() ){
			return &(*iterdata).second;
		}
	}

	return NULL;
};

void GLPVPTyrannyField::PlayerRevive( DWORD dwGaeaID, DWORD dwCharID )
{
	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_REQ_REVIVE_FB netmsgclient;

	if ( m_emState != TYRANNY_STATE_BATTLE &&
		m_emState != TYRANNY_STATE_REWARD )
	{
		netmsgclient.emFB = EMREQ_REVIVE_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
		return;
	}
	
	BOOL brevive = pchar->PVPTyrannyRevive( m_wSchoolGate[pchar->m_wSchool] );
	if ( !brevive )
	{
		netmsgclient.emFB = EMREQ_REVIVE_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
	}
}

void GLPVPTyrannyField::GetRewards( WORD wSchoolWinner )
{
	m_mapRewarded.clear();

	if ( wSchoolWinner >= TYRANNY_SCHOOL_SIZE )	return;

	for( TYRANNY_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); 
		iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer )
	{
		const TYRANNY_PLAYER_DATA& splayerdata = (*iterplayer).second;
		if ( splayerdata.wRankAll == 0 )						continue;
		//if ( splayerdata.dwScoreTotal < m_wRewardLimitScore )	continue;
		//if ( splayerdata.wInfoSchool != wSchoolWinner )		continue;
		if ( splayerdata.wInfoSchool == wSchoolWinner && splayerdata.dwScoreTotal < m_wRewardLimitScoreWin )			continue;
		if ( splayerdata.wInfoSchool != wSchoolWinner && splayerdata.dwScoreTotal < m_wRewardLimitScoreLost )			continue;
		if ( splayerdata.bLateJoin )	continue;

		TYRANNY_REWARD_DATA sdata;
		sdata.dwCharID = splayerdata.dwInfoCharID;
		sdata.sItemReward = (splayerdata.wInfoSchool == wSchoolWinner) ? m_sItemRewardWin:m_sItemRewardLost;
		sdata.bSent = false;
		m_mapRewarded.insert( std::make_pair( sdata.dwCharID, sdata ) );
	}

	for( TYRANNY_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); 
		iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer )
	{
		const TYRANNY_PLAYER_DATA& splayerdata = (*iterplayer).second;
		if ( splayerdata.wRankAll == 0 )						continue;
		//if ( splayerdata.dwScoreTotal < m_wRewardLimitScore )	continue;
		//if ( splayerdata.wInfoSchool != wSchoolWinner )		continue;
		if ( splayerdata.wInfoSchool == wSchoolWinner && splayerdata.dwScoreTotal < m_wRewardLimitScoreWin )			continue;
		if ( splayerdata.wInfoSchool != wSchoolWinner && splayerdata.dwScoreTotal < m_wRewardLimitScoreLost )			continue;
		if ( splayerdata.bLateJoin )	continue;

		TYRANNY_REWARD_DATA sdata;
		sdata.dwCharID = splayerdata.dwInfoCharID;
		sdata.sItemReward = (splayerdata.wInfoSchool == wSchoolWinner) ? m_sItemRewardWin:m_sItemRewardLost;
		sdata.bSent = false;
		m_mapRewarded.insert( std::make_pair( sdata.dwCharID, sdata ) );
	}

	for( TYRANNY_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); 
		iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer )
	{
		const TYRANNY_PLAYER_DATA& splayerdata = (*iterplayer).second;
		if ( splayerdata.wRankAll == 0 )						continue;
		//if ( splayerdata.dwScoreTotal < m_wRewardLimitScore )	continue;
		//if ( splayerdata.wInfoSchool != wSchoolWinner )		continue;
		if ( splayerdata.wInfoSchool == wSchoolWinner && splayerdata.dwScoreTotal < m_wRewardLimitScoreWin )			continue;
		if ( splayerdata.wInfoSchool != wSchoolWinner && splayerdata.dwScoreTotal < m_wRewardLimitScoreLost )			continue;
		if ( splayerdata.bLateJoin )	continue;

		TYRANNY_REWARD_DATA sdata;
		sdata.dwCharID = splayerdata.dwInfoCharID;
		sdata.sItemReward = (splayerdata.wInfoSchool == wSchoolWinner) ? m_sItemRewardWin:m_sItemRewardLost;
		sdata.bSent = false;
		m_mapRewarded.insert( std::make_pair( sdata.dwCharID, sdata ) );
	}
}

void GLPVPTyrannyField::RewardSend()
{
	if ( m_mapRewarded.empty() )				return;

	for( TYRANNY_REWARD_MAP_ITER iterreward = m_mapRewarded.begin();
		iterreward != m_mapRewarded.end();
		++ iterreward )
	{
		TYRANNY_REWARD_DATA &sReward = (*iterreward).second; 
		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID( sReward.dwCharID );
		if ( !pChar )	continue;

		SITEM* pitem_reward = GLItemMan::GetInstance().GetItem( sReward.sItemReward );
		if ( !pitem_reward )	continue;

		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = sReward.sItemReward;
		sITEM_NEW.tBORNTIME = CTime::GetCurrentTime().GetTime();
		sITEM_NEW.cGenType = EMGEN_PVP_EVENT_REWARD;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( sITEM_NEW.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			sITEM_NEW.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			sITEM_NEW.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		CItemDrop cDropItem;
		cDropItem.sItemCustom = sITEM_NEW;
		if ( pChar->IsInsertToInvenEx ( &cDropItem ) )
		{
			pChar->InsertToInvenEx ( &cDropItem );
			GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CLUB, pChar->m_dwGuild, ID_CHAR, pChar->m_dwCharID, EMITEM_ROUTE_SYSTEM, sITEM_NEW.wTurnNum );
			sReward.bSent = TRUE;
		}
	}
}

void GLPVPTyrannyField::PointSend()
{
	TYRANNY_PLAYER_DATA_VEC vecdata;
	vecdata.reserve( m_mapPlayerDataSG.size() + m_mapPlayerDataMP.size() + m_mapPlayerDataPHX.size() );

	for( TYRANNY_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); 
		iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer )
	{
			const TYRANNY_PLAYER_DATA& splayerdata = (*iterplayer).second;
			if ( splayerdata.wRewardPoint == 0 )	continue;
			if ( splayerdata.bLateJoin )	continue;
			vecdata.push_back( splayerdata );
	}

	for( TYRANNY_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); 
		iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer )
	{
			const TYRANNY_PLAYER_DATA& splayerdata = (*iterplayer).second;
			if ( splayerdata.wRewardPoint == 0 )	continue;
			if ( splayerdata.bLateJoin )	continue;
			vecdata.push_back( splayerdata );
	}

	for( TYRANNY_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); 
		iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer )
	{
			const TYRANNY_PLAYER_DATA& splayerdata = (*iterplayer).second;
			if ( splayerdata.wRewardPoint == 0 )	continue;
			if ( splayerdata.bLateJoin )	continue;
			vecdata.push_back( splayerdata );
	}

	for ( size_t i=0; i<vecdata.size(); ++i )
	{
		const TYRANNY_PLAYER_DATA& splayer_data = vecdata[i];

		PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( splayer_data.dwInfoCharID );
		if ( !pchar ) continue;

		pchar->m_dwContributionPoint += splayer_data.wRewardPoint;
		/*12-9-14, Battle Pass - CNDev*/
		pchar->DoBattlePassGainContributionPoint( pchar->m_dwContributionPoint );

		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netmsgclient;
		netmsgclient.bNotice = true;
		netmsgclient.dwPoint = pchar->m_dwContributionPoint;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsgclient );
	}
}

void GLPVPTyrannyField::CheckRewards()
{
	for( TYRANNY_REWARD_MAP_ITER iterreward = m_mapRewarded.begin();
		iterreward != m_mapRewarded.end();
		++ iterreward )
	{
		TYRANNY_REWARD_DATA &sReward = (*iterreward).second; 

		if( !sReward.bSent )
		{
			CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyField Failed to insert reward to :%d item[%d~%d]", sReward.dwCharID, sReward.sItemReward.wMainID, sReward.sItemReward.wSubID );
		}
	}
}

void GLPVPTyrannyField::TowerCapture( SNATIVEID sidMob, WORD wSchool, DWORD dwClubID, DWORD dwClubMarkVer )
{
	if ( wSchool >= TYRANNY_SCHOOL_SIZE )		return;
	if ( m_emState != TYRANNY_STATE_BATTLE )	return;

	GLMSG::SNETPC_TYRANNY_F2A_TOWER_CAPTURE netmsgagent;
	netmsgagent.sidMob = sidMob;
	netmsgagent.wSchool = wSchool;
	netmsgagent.dwClubID = dwClubID;
	netmsgagent.dwClubMarkVer = dwClubMarkVer;
	GLGaeaServer::GetInstance().SENDTOAGENT( &netmsgagent );
}

/* tyranny result log, Juver, 2021/09/09 */
void GLPVPTyrannyField::LogTyrannyResult( WORD wSchoolWinner )
{
	if ( m_bValidInstance == FALSE )	return;

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CLogTyrannyResult *pdb_action = new CLogTyrannyResult( wSchoolWinner, m_mapPlayerDataSG.size(), m_mapPlayerDataMP.size(), m_mapPlayerDataPHX.size() );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );
	}
}

void GLPVPTyrannyField::LogTyrannyParticipant( int nSessionID )
{
	if ( nSessionID == DB_ERROR )	return;

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		{
			TYRANNY_PLAYER_DATA_MAP_ITER it_b = m_mapPlayerDataSG.begin();
			TYRANNY_PLAYER_DATA_MAP_ITER it_e = m_mapPlayerDataSG.end();
			for ( ; it_b != it_e; ++it_b )
			{
				const TYRANNY_PLAYER_DATA& sData = (*it_b).second;

				CLogTyrannyParticipant *pdb_action = new CLogTyrannyParticipant( 
					nSessionID, 
					sData.dwInfoCharID, 
					sData.szInfoCharName, 
					sData.wInfoSchool, 
					sData.wInfoClass, 
					sData.wRankAll, 
					sData.wRankSchool, 
					sData.wScoreKill, 
					sData.wScoreDeath, 
					sData.dwScoreDamage, 
					sData.dwScoreHeal, 
					sData.wScoreResu, 
					sData.dwScoreTotal,
					sData.wRewardPoint );
				GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );
			}
		}
		
		{
			TYRANNY_PLAYER_DATA_MAP_ITER it_b = m_mapPlayerDataMP.begin();
			TYRANNY_PLAYER_DATA_MAP_ITER it_e = m_mapPlayerDataMP.end();
			for ( ; it_b != it_e; ++it_b )
			{
				const TYRANNY_PLAYER_DATA& sData = (*it_b).second;

				CLogTyrannyParticipant *pdb_action = new CLogTyrannyParticipant( 
					nSessionID,
					sData.dwInfoCharID, 
					sData.szInfoCharName, 
					sData.wInfoSchool, 
					sData.wInfoClass, 
					sData.wRankAll, 
					sData.wRankSchool, 
					sData.wScoreKill, 
					sData.wScoreDeath, 
					sData.dwScoreDamage, 
					sData.dwScoreHeal, 
					sData.wScoreResu, 
					sData.dwScoreTotal,
					sData.wRewardPoint );
				GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );
			}
		}

		{
			TYRANNY_PLAYER_DATA_MAP_ITER it_b = m_mapPlayerDataPHX.begin();
			TYRANNY_PLAYER_DATA_MAP_ITER it_e = m_mapPlayerDataPHX.end();
			for ( ; it_b != it_e; ++it_b )
			{
				const TYRANNY_PLAYER_DATA& sData = (*it_b).second;

				CLogTyrannyParticipant *pdb_action = new CLogTyrannyParticipant( 
					nSessionID,
					sData.dwInfoCharID, 
					sData.szInfoCharName, 
					sData.wInfoSchool, 
					sData.wInfoClass, 
					sData.wRankAll, 
					sData.wRankSchool, 
					sData.wScoreKill, 
					sData.wScoreDeath, 
					sData.dwScoreDamage, 
					sData.dwScoreHeal, 
					sData.wScoreResu, 
					sData.dwScoreTotal,
					sData.wRewardPoint );
				GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );
			}
		}
		
	}
}