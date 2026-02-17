#include "StdAfx.h"
#include "./GLPVPSchoolWarsField.h"
#include "./GLGaeaServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL _PVPSchoolWarBattleRankSort(const SBATTLE_RANKING& L_Rank, const SBATTLE_RANKING& R_Rank)
{
	return L_Rank.wKill > R_Rank.wKill;
}

void GLPVPSchoolWarsField::InsertPlayerData( SCHOOLWARS_REGISTER_DATA* pPlayerData )
{
	if ( !pPlayerData )			return;
	if ( pPlayerData->dwInfoCharID == SCHOOLWARS_PLAYER_NULL )
	{
		CDebugSet::ToFileWithTime( "_pvpschoolwars.txt", "GLPVPSchoolWarsField invalid ID:%d %s", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName );
		return;
	}

	SCHOOLWARS_PLAYER_DATA* pdata = PlayerDataGet( pPlayerData->dwInfoCharID );
	if ( pdata )
	{
		CDebugSet::ToFileWithTime( "_pvpschoolwars.txt", "GLPVPSchoolWarsField char:%d %s already exist", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName );
		return;
	}

	if ( pPlayerData->wInfoSchool >= SCHOOLWARS_SCHOOL_SIZE )
	{
		CDebugSet::ToFileWithTime( "_pvpschoolwars.txt", "GLPVPSchoolWarsField char:%d %s invalid school %d", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName, pPlayerData->wInfoSchool );
		return;
	}

	if ( pPlayerData->wInfoClass >= GLCI_NUM_8CLASS )
	{
		CDebugSet::ToFileWithTime( "_pvpschoolwars.txt", "GLPVPSchoolWarsField char:%d %s invalid class %d", pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName, pPlayerData->wInfoClass );
		return;
	}

	SCHOOLWARS_PLAYER_DATA sdata;
	sdata.dwInfoCharID	= pPlayerData->dwInfoCharID;
	sdata.wInfoClass	= pPlayerData->wInfoClass;
	sdata.wInfoSchool	= pPlayerData->wInfoSchool;
	StringCchCopy( sdata.szInfoCharName, SCHOOLWARS_CHARNAME_STRING_SIZE, pPlayerData->szInfoCharName );

	switch ( sdata.wInfoSchool )
	{
	case SCHOOLWARS_SCHOOL_SG:
		{
			m_mapPlayerDataSG.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );
		}break;

	case SCHOOLWARS_SCHOOL_MP:
		{
			m_mapPlayerDataMP.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );
		}break;

	case SCHOOLWARS_SCHOOL_PHX:
		{
			m_mapPlayerDataPHX.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );
		}break;
	};
}

void GLPVPSchoolWarsField::ScoreKill( DWORD dwCharKilled, DWORD dwCharKiller )
{
	if ( m_emState != SCHOOLWARS_STATE_BATTLE )	return;

	SCHOOLWARS_PLAYER_DATA* pplayerkilled = PlayerDataGet( dwCharKilled );
	SCHOOLWARS_PLAYER_DATA* pplayerkiller = PlayerDataGet( dwCharKiller );

	if ( pplayerkilled && pplayerkiller )
	{
		pplayerkilled->wScoreDeath ++;
		pplayerkiller->wScoreKill ++;

		GLMSG::SNETPC_SCHOOLWARS_F2A_SCORE_UPDATE netmsgagent;
		netmsgagent.wSchoolKilled = pplayerkilled->wInfoSchool;
		netmsgagent.wSchoolKiller = pplayerkiller->wInfoSchool;
		netmsgagent.dwCharID = dwCharKiller;
		GLGaeaServer::GetInstance().SENDTOAGENT( &netmsgagent );
	}
}

void GLPVPSchoolWarsField::ScoreHeal( DWORD dwChar, DWORD dwHeal, DWORD dwCharHealer  )
{
	if ( m_emState != SCHOOLWARS_STATE_BATTLE )	return;

	/*PVPSchoolWars disable score self heal, EJCode, 2018/11/28 */
	if ( isDisableScoreSelfHeal() && dwChar == dwCharHealer )
		return;

	SCHOOLWARS_PLAYER_DATA* pplayerdata = PlayerDataGet( dwChar );
	if ( pplayerdata )
	{
		pplayerdata->dwScoreHeal += dwHeal;
	}
}

void GLPVPSchoolWarsField::ScoreResu( DWORD dwChar )
{
	if ( m_emState != SCHOOLWARS_STATE_BATTLE )	return;

	SCHOOLWARS_PLAYER_DATA* pplayerdata = PlayerDataGet( dwChar );
	if ( pplayerdata )
	{
		pplayerdata->wScoreResu ++;
	}
}

void GLPVPSchoolWarsField::ScoreMobKill( SNATIVEID idcrow, DWORD idchar )
{
	if ( m_emState != SCHOOLWARS_STATE_BATTLE )	return;

	SCHOOLWARS_CROW_SCORE* pcrow_score = GetCrowScore( idcrow );
	if ( !pcrow_score )	return;

	SCHOOLWARS_PLAYER_DATA* pplayerdata = PlayerDataGet( idchar );
	if ( !pplayerdata )	return;

	ScoreMobKillAll( pplayerdata->wInfoSchool, pcrow_score->score_each );

	GLMSG::SNETPC_SCHOOLWARS_F2A_MOB_KILL_UPDATE netmsgagent;
	netmsgagent.id_crow = idcrow;
	netmsgagent.id_char = idchar;
	GLGaeaServer::GetInstance().SENDTOAGENT( &netmsgagent );
}

void GLPVPSchoolWarsField::ScoreMobKillAll( WORD school, WORD score )
{
	if ( school >= SCHOOLWARS_SCHOOL_SIZE )	return;

	if ( school == SCHOOLWARS_SCHOOL_SG )
	{
		SCHOOLWARS_PLAYER_DATA_MAP_ITER it = m_mapPlayerDataSG.begin();
		SCHOOLWARS_PLAYER_DATA_MAP_ITER end = m_mapPlayerDataSG.end();
		for( ; it != end; ++it )
		{
			SCHOOLWARS_PLAYER_DATA& splayer_data = (*it).second;
			splayer_data.wMobKill += score;
		}
	}
	else if ( school == SCHOOLWARS_SCHOOL_MP )
	{
		SCHOOLWARS_PLAYER_DATA_MAP_ITER it = m_mapPlayerDataMP.begin();
		SCHOOLWARS_PLAYER_DATA_MAP_ITER end = m_mapPlayerDataMP.end();
		for( ; it != end; ++it )
		{
			SCHOOLWARS_PLAYER_DATA& splayer_data = (*it).second;
			splayer_data.wMobKill += score;
		}
	}
	else if ( school == SCHOOLWARS_SCHOOL_PHX )
	{
		SCHOOLWARS_PLAYER_DATA_MAP_ITER it = m_mapPlayerDataPHX.begin();
		SCHOOLWARS_PLAYER_DATA_MAP_ITER end = m_mapPlayerDataPHX.end();
		for( ; it != end; ++it )
		{
			SCHOOLWARS_PLAYER_DATA& splayer_data = (*it).second;
			splayer_data.wMobKill += score;
		}
	}
}

void GLPVPSchoolWarsField::ScoreCalculate(SCHOOLWARS_PLAYER_DATA_MAP& mapData, WORD wSchoolWinner )
{
	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = mapData.begin(); iterplayer != mapData.end(); ++ iterplayer )
	{
		SCHOOLWARS_PLAYER_DATA& splayer_data = (*iterplayer).second;

		//calculate score
		float ftotal_score = 0.0f;
		ftotal_score += float( splayer_data.dwScoreHeal * m_fHealVar );
		ftotal_score += float( splayer_data.wScoreKill * m_fKillVar );
		ftotal_score += float( splayer_data.wScoreResu * m_fResuVar );
		splayer_data.dwScoreTotal = (DWORD)ftotal_score;
		splayer_data.dwScoreTotal += splayer_data.wMobKill;

		//calculate points
		if ( splayer_data.dwScoreTotal > 0 )
		{
			//base point
			WORD wpoint_base = m_wBasePointLoss;
			if ( wSchoolWinner != SCHOOLWARS_SCHOOL_SIZE &&
				splayer_data.wInfoSchool == wSchoolWinner )
			{
				wpoint_base = m_wBasePointWin;
			}

			//point gained from score
			wpoint_base += WORD( splayer_data.dwScoreTotal * m_fPointVar );

			//point limit
			if ( wpoint_base >= m_wMaxRewardPoint )
				wpoint_base = m_wMaxRewardPoint;

			splayer_data.wRewardPoint = wpoint_base;

			splayer_data.wRewardPoint = WORD( splayer_data.wRewardPoint * m_fContributionPointMultiplier );
		}
		else
		{
			//no reward point for no score
			splayer_data.wRewardPoint = 0;
		}
	}
}

void GLPVPSchoolWarsField::CalculateRankingSchool( )
{

	{	//sg rank
		SCHOOLWARS_PLAYER_DATA_VEC vecdata;
		vecdata.reserve( m_mapPlayerDataSG.size() );

		for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer ){
			const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
		}

		std::sort( vecdata.begin(), vecdata.end() );	

		for( size_t i=0; i<vecdata.size(); ++i )
			vecdata[i].wRankSchool = i+1;

		for( size_t i=0; i<vecdata.size(); ++i )
		{
			const SCHOOLWARS_PLAYER_DATA& splayer_data = vecdata[i];

			SCHOOLWARS_PLAYER_DATA* prank_player = PlayerDataGet( splayer_data.dwInfoCharID );
			if ( prank_player && prank_player->dwInfoCharID == splayer_data.dwInfoCharID )
				prank_player->wRankSchool = splayer_data.wRankSchool;
		}
	}

	{	//mp rank
		SCHOOLWARS_PLAYER_DATA_VEC vecdata;
		vecdata.reserve( m_mapPlayerDataMP.size() );

		for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer ){
			const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
		}

		std::sort( vecdata.begin(), vecdata.end() );	

		for( size_t i=0; i<vecdata.size(); ++i )
			vecdata[i].wRankSchool = i+1;

		for( size_t i=0; i<vecdata.size(); ++i )
		{
			const SCHOOLWARS_PLAYER_DATA& splayer_data = vecdata[i];

			SCHOOLWARS_PLAYER_DATA* prank_player = PlayerDataGet( splayer_data.dwInfoCharID );
			if ( prank_player && prank_player->dwInfoCharID == splayer_data.dwInfoCharID )
				prank_player->wRankSchool = splayer_data.wRankSchool;
		}
	}

	{	//phx rank
		SCHOOLWARS_PLAYER_DATA_VEC vecdata;
		vecdata.reserve( m_mapPlayerDataPHX.size() );

		for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer ){
			const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
		}

		std::sort( vecdata.begin(), vecdata.end() );	

		for( size_t i=0; i<vecdata.size(); ++i )
			vecdata[i].wRankSchool = i+1;

		for( size_t i=0; i<vecdata.size(); ++i )
		{
			const SCHOOLWARS_PLAYER_DATA& splayer_data = vecdata[i];

			SCHOOLWARS_PLAYER_DATA* prank_player = PlayerDataGet( splayer_data.dwInfoCharID );
			if ( prank_player && prank_player->dwInfoCharID == splayer_data.dwInfoCharID )
				prank_player->wRankSchool = splayer_data.wRankSchool;
		}
	}
}

void GLPVPSchoolWarsField::CalculateRankingAll()
{
	SCHOOLWARS_PLAYER_DATA_VEC vecdata;
	vecdata.reserve( m_mapPlayerDataSG.size() + m_mapPlayerDataMP.size() + m_mapPlayerDataPHX.size() );

	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer ){
		const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer ){
		const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer ){
		const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	std::sort( vecdata.begin(), vecdata.end() );	

	for( size_t i=0; i<vecdata.size(); ++i )
		vecdata[i].wRankAll = i+1;

	for( size_t i=0; i<vecdata.size(); ++i )
	{
		const SCHOOLWARS_PLAYER_DATA& splayer_data = vecdata[i];
		SCHOOLWARS_PLAYER_DATA* prank_player = PlayerDataGet( splayer_data.dwInfoCharID );
		if ( prank_player && prank_player->dwInfoCharID == splayer_data.dwInfoCharID )
			prank_player->wRankAll = splayer_data.wRankAll;
	}
}

void GLPVPSchoolWarsField::RankingSend()
{
	SCHOOLWARS_PLAYER_DATA_VEC vecdata;
	vecdata.reserve( m_mapPlayerDataSG.size() + m_mapPlayerDataMP.size() + m_mapPlayerDataPHX.size() );

	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer ){
		const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer ){
		const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	for( SCHOOLWARS_PLAYER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer ){
		const SCHOOLWARS_PLAYER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	GLMSG::SNETPC_SCHOOLWARS_F2C_RANKING_UPDATE	netmsg_ranking;
	GLMSG::SNETPC_SCHOOLWARS_F2A_RANKINFO_PC	netmsg_ranking_agent;

	WORD wNum = 0;
	for ( size_t i=0; i<vecdata.size(); ++i )
	{
		const SCHOOLWARS_PLAYER_DATA& splayer_data = vecdata[i];
		netmsg_ranking.ADDRANK( splayer_data );
		if ( netmsg_ranking.wRankNum == SCHOOLWARS_PACKET_RANK_NUM )
		{
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sMap.dwID, &netmsg_ranking );
			wNum += netmsg_ranking.wRankNum;
			netmsg_ranking.Reset();
		}

		if ( splayer_data.wRankAll > 0 && splayer_data.wRankAll <= SCHOOLWARS_MINI_RANKING_NUM )
			netmsg_ranking_agent.ADDRANK( splayer_data );
	}

	if ( netmsg_ranking.wRankNum != 0 )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sMap.dwID, &netmsg_ranking );
		wNum += netmsg_ranking.wRankNum;
		netmsg_ranking.Reset();
	}

	GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLPVPSchoolWarsField Send Rank num: %u", wNum );
	CDebugSet::ToFileWithTime( "_pvpschoolwars.txt", "GLPVPSchoolWarsField Send Rank num: %u", wNum );

	if ( netmsg_ranking_agent.wRankNum > 0 )	
	{
		GLGaeaServer::GetInstance().SENDTOAGENT( &netmsg_ranking_agent );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLPVPSchoolWarsField Send to agent rank num: %u", netmsg_ranking_agent.wRankNum );
		CDebugSet::ToFileWithTime( "_pvpschoolwars.txt", "GLPVPSchoolWarsField Send to agent rank num: %u", netmsg_ranking_agent.wRankNum );
		netmsg_ranking_agent.wRankNum = 0;
	}

	GLMSG::SNETPC_SCHOOLWARS_F2C_RANKING_END	netmsg_ranking_end;
	netmsg_ranking_end.wPlayerNum[SCHOOLWARS_SCHOOL_SG] = (WORD)m_mapPlayerDataSG.size();
	netmsg_ranking_end.wPlayerNum[SCHOOLWARS_SCHOOL_MP] = (WORD)m_mapPlayerDataMP.size();
	netmsg_ranking_end.wPlayerNum[SCHOOLWARS_SCHOOL_PHX] = (WORD)m_mapPlayerDataPHX.size();
	GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sMap.dwID, &netmsg_ranking_end );
}

void GLPVPSchoolWarsField::AddBattleRanking(DWORD dwKillID, DWORD dwDeathID)
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

void GLPVPSchoolWarsField::UpdateBattleRanking()
{
	m_vecBattleRankingP.clear();

	MAP_BATTLE_RANKING_ITER iter_begin	= m_mapBattleRankingP.begin();
	MAP_BATTLE_RANKING_ITER iter_end	= m_mapBattleRankingP.end();

	for (; iter_begin != iter_end; iter_begin++)
		m_vecBattleRankingP.push_back((*iter_begin).second);

	if (!m_vecBattleRankingP.empty())
	{
		sort(m_vecBattleRankingP.begin(), m_vecBattleRankingP.end(), _PVPSchoolWarBattleRankSort);

		for (size_t i = 0; i < m_vecBattleRankingP.size(); i++)
			m_vecBattleRankingP[i].nIndex = i;

		GLMSG::SNET_REQ_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMap.dwID;

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

		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sMap.dwID, &NetMsg);
	}

	//club rank
	m_vecBattleRankingC.clear();

	MAP_BATTLE_RANKING_ITER iter_begin2	= m_mapBattleRankingC.begin();
	MAP_BATTLE_RANKING_ITER iter_end2	= m_mapBattleRankingC.end();

	for (; iter_begin2 != iter_end2; iter_begin2++)
		m_vecBattleRankingC.push_back((*iter_begin2).second);

	if (!m_vecBattleRankingC.empty())
	{
		sort(m_vecBattleRankingC.begin(), m_vecBattleRankingC.end(), _PVPSchoolWarBattleRankSort);

		for (size_t i = 0; i < m_vecBattleRankingC.size(); i++)
			m_vecBattleRankingC[i].nIndex = i;

		GLMSG::SNET_REQ_CLUB_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMap.dwID;

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

		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sMap.dwID, &NetMsg);
	}
}
void GLPVPSchoolWarsField::ReqBattleRanking(DWORD dwClientID, int nPage)
{
	if( nPage==2 )
	{
		if( m_vecBattleRankingC.empty() )	return;

		GLMSG::SNET_REQ_CLUB_BATTLE_RANKING_FB NetMsg;
		NetMsg.dwID = m_sMap.dwID;

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
		NetMsg.dwID = m_sMap.dwID;

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
void GLPVPSchoolWarsField::ReqMyBattleRanking(DWORD dwClientID, DWORD dwCharID, int nPage)
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