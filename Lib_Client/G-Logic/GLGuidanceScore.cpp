#include "stdafx.h"
#include "./GLGuidance.h"
#include "./GLGaeaServer.h"
#include "./GLAgentServer.h"

#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool GLGuidanceFieldMan::ScoreKill( DWORD dwID, DWORD dwCharIDKilled, DWORD dwCharIDKiller )
{
	if ( !RPARAM::bClubWarRankings )	return false;

	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )				return false;
	if ( !pGuid->m_bBattle )	return false;

	GLMSG::SNET_CLUB_SCORE_KILL_FLD_2_AGT NetMsgAgent;
	NetMsgAgent.dwID = pGuid->m_dwID;
	NetMsgAgent.dwCharIDKilled = dwCharIDKilled;
	NetMsgAgent.dwCharIDKiller = dwCharIDKiller;
	GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsgAgent );

	return true;
}

bool GLGuidanceFieldMan::ScoreResu( DWORD dwID, DWORD dwCharID )
{
	if ( !RPARAM::bClubWarRankings )	return false;

	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )				return false;
	if ( !pGuid->m_bBattle )	return false;

	GLMSG::SNET_CLUB_SCORE_RESU_FLD_2_AGT NetMsgAgent;
	NetMsgAgent.dwID = pGuid->m_dwID;
	NetMsgAgent.dwCharID = dwCharID;
	GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsgAgent );

	return true;
}

SPVP_CLUB_WAR_PLAYER_DATA* GLGuidance::GetPlayerData( DWORD dwCharID )
{
	SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER it = m_mapPlayerData.find( dwCharID );
	if ( it != m_mapPlayerData.end() )	return &(*it).second;

	return NULL;
}

SPVP_CLUB_WAR_PLAYER_DATA* GLGuidance::CreatePlayerData( DWORD dwCharID )
{
	SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER it = m_mapPlayerData.find( dwCharID );
	if ( it != m_mapPlayerData.end() )
	{
		return &(*it).second;
	}
	else
	{
		//create new data

		PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pChar )	return NULL;

		SPVP_CLUB_WAR_PLAYER_DATA sDataNew;
		sDataNew.dwCharID = pChar->m_dwCharID;
		StringCchCopy( sDataNew.szCharName, PVP_CLUB_WAR_PLAYER_DATA_CHAR_NAME_SIZE, pChar->m_szName );
		sDataNew.wCharSchool = pChar->m_wSchool;
		sDataNew.wCharClass = (WORD)CharClassToIndex( pChar->m_emClass );

		GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub ( pChar->m_dwGuild );
		if ( pCLUB )
		{
			sDataNew.dwGuildID = pCLUB->m_dwID;
			StringCchCopy( sDataNew.szGuildName, PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE, pCLUB->m_szName );
			sDataNew.dwGuildMarkVer = pCLUB->m_dwMarkVER;
		}

		m_mapPlayerData.insert( std::make_pair( sDataNew.dwCharID, sDataNew ) );

		//RLOG::LogFile( "PVPGuidanceTest.txt", "player:[%u]%s insert", sDataNew.dwCharID, sDataNew.szCharName );

		SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER itnew = m_mapPlayerData.find( dwCharID );
		if( itnew != m_mapPlayerData.end() )
		{
			return &(*itnew).second;
		}
		else
		{
			//new data somehow lost
			RLOG::LogFile( "PVPGuidance.txt", "player:[%u]%s insert new data lost", sDataNew.dwCharID, sDataNew.szCharName );
		}

	}

	return NULL;
}

void GLGuidance::ScoreKill( DWORD dwCharIDKilled, DWORD dwCharIDKiller )
{
	if ( !IsBattle() )	return;
	if ( !RPARAM::bClubWarRankings )	return;

	SPVP_CLUB_WAR_PLAYER_DATA* pplayerkilled = CreatePlayerData( dwCharIDKilled );
	SPVP_CLUB_WAR_PLAYER_DATA* pplayerkiller = CreatePlayerData( dwCharIDKiller );

	if ( pplayerkilled && pplayerkiller )
	{
		pplayerkilled->wScoreDeath ++;
		pplayerkiller->wScoreKill ++;

		//RLOG::LogFile( "PVPGuidanceTest.txt", "player:[%u]%s add death", pplayerkilled->dwCharID, pplayerkilled->szCharName );
		//RLOG::LogFile( "PVPGuidanceTest.txt", "player:[%u]%s add kill", pplayerkiller->dwCharID, pplayerkiller->szCharName );
	}
}

void GLGuidance::ScoreResu( DWORD dwCharID )
{
	if ( !IsBattle() )	return;
	if ( !RPARAM::bClubWarRankings )	return;

	SPVP_CLUB_WAR_PLAYER_DATA* pplayerdata = CreatePlayerData( dwCharID );
	if ( pplayerdata )
	{
		pplayerdata->wScoreResu ++;
		//RLOG::LogFile( "PVPGuidanceTest.txt", "player:[%u]%s add resu", pplayerdata->dwCharID, pplayerdata->szCharName );
	}
}

void GLGuidance::CalculateScore()
{
	std::vector<DWORD> vecCharID;
	vecCharID.reserve( m_mapPlayerData.size() );

	SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER it_b;
	SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER it_e;

	//get player ID
	it_b = m_mapPlayerData.begin();
	it_e = m_mapPlayerData.end();
	for( ; it_b != it_e; ++ it_b )
	{
		SPVP_CLUB_WAR_PLAYER_DATA& splayerdata = (*it_b).second;

		GLCLUB *pClub = GLAgentServer::GetInstance().GetClubMan().GetClub( m_dwGuidanceClub );
		if ( pClub )
		{
			if ( pClub->m_dwID == splayerdata.dwGuildID )
				splayerdata.wState = EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_MAIN;
			else if ( pClub->IsAllianceGuild( splayerdata.dwGuildID ) )
				splayerdata.wState = EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_ALLY;
			else 
				splayerdata.wState = EMPVP_CLUB_WAR_PLAYER_STATE_PARTICIPANT;
		}

		float fTotalScore = 0.0f;
		fTotalScore += float( splayerdata.wScoreKill * m_fScoreKill );
		fTotalScore += float( splayerdata.wScoreResu * m_fScoreResu );
		splayerdata.wScoreTotal = (WORD)fTotalScore;
		splayerdata.wRewardBase = 0;
		splayerdata.wRewardBonus = 0;

		if ( splayerdata.wScoreTotal >= m_wMinimumScore )
		{
			float fTotalPoint = float( splayerdata.wScoreTotal * m_fPointScore );
			splayerdata.wRewardBase = (WORD)fTotalPoint;

			if ( splayerdata.wState == EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_MAIN )
			{
				float fBonusPoint = float( splayerdata.wRewardBase * ( 1.0f + m_fPointWinMainBonus ) );
				splayerdata.wRewardBonus = (WORD)fBonusPoint;
				splayerdata.wRewardBonus -= splayerdata.wRewardBase;
			}

			if ( splayerdata.wState == EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_ALLY )
			{
				float fBonusPoint = float( splayerdata.wRewardBase * ( 1.0f + m_fPointWinAllyBonus ) );
				splayerdata.wRewardBonus = (WORD)fBonusPoint;
				splayerdata.wRewardBonus -= splayerdata.wRewardBase;
			}
		}

		vecCharID.push_back( splayerdata.dwCharID );
	}

	//ranking start
	GLMSG::SNETPC_CLUB_SCORE_A2C_RANKING_START	NetMsgRankingStart;
	NetMsgRankingStart.dwGuidanceID = m_dwID;
	StringCchCopy( NetMsgRankingStart.szGuidanceName, PVP_CLUB_WAR_NAME_SIZE, m_strName.c_str() );

	GLCLUB *pClub = GLAgentServer::GetInstance().GetClubMan().GetClub( m_dwGuidanceClub );
	if ( pClub )
	{
		NetMsgRankingStart.dwOwnerGuildID = m_dwGuidanceClub;
		StringCchCopy( NetMsgRankingStart.szOwnerGuildName, PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE, pClub->m_szName );
		NetMsgRankingStart.dwGuildMarkVer = pClub->m_dwMarkVER;
	}

	NetMsgRankingStart.fBonusMain = m_fPointWinMainBonus;
	NetMsgRankingStart.fBonusAlly = m_fPointWinAllyBonus;

	for( size_t i=0; i<vecCharID.size(); ++i )
	{
		PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( vecCharID[i] );
		if ( pChar )
		{
			GLAgentServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &NetMsgRankingStart );
		}
	}

	//calculate overall rank
	SPVP_CLUB_WAR_PLAYER_DATA_VEC vecPlayerDataSort;
	vecPlayerDataSort.reserve( m_mapPlayerData.size() );

	it_b = m_mapPlayerData.begin();
	it_e = m_mapPlayerData.end();
	for( ; it_b != it_e; ++ it_b )
	{
		const SPVP_CLUB_WAR_PLAYER_DATA& splayerdata = (*it_b).second;
		vecPlayerDataSort.push_back( splayerdata );
	}

	std::sort( vecPlayerDataSort.begin(), vecPlayerDataSort.end() );	

	for( size_t i=0; i<vecPlayerDataSort.size(); ++i )
	{
		vecPlayerDataSort[i].wRankAll = i+1;
	}

	for( size_t i=0; i<vecPlayerDataSort.size(); ++i )
	{
		const SPVP_CLUB_WAR_PLAYER_DATA& splayer_data = vecPlayerDataSort[i];

		SPVP_CLUB_WAR_PLAYER_DATA* prank_player = GetPlayerData( splayer_data.dwCharID );
		if ( prank_player && prank_player->dwCharID == splayer_data.dwCharID )
			prank_player->wRankAll = splayer_data.wRankAll;
	}

	vecPlayerDataSort.clear();

	//send rank info to client
	GLMSG::SNETPC_CLUB_SCORE_A2C_RANKING_UPDATE	NetMsgRankingUpdate;
	NetMsgRankingUpdate.dwGuidanceID = m_dwID;

	it_b = m_mapPlayerData.begin();
	it_e = m_mapPlayerData.end();
	for( ; it_b != it_e; ++ it_b )
	{
		const SPVP_CLUB_WAR_PLAYER_DATA& splayer_data = (*it_b).second;
		NetMsgRankingUpdate.ADDRANK( splayer_data );
		if ( NetMsgRankingUpdate.wRankNum == PVP_CLUB_WAR_PACKET_RANK_NUM )
		{
			for( size_t i=0; i<vecCharID.size(); ++i )
			{
				PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( vecCharID[i] );
				if ( pChar )
				{
					GLAgentServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &NetMsgRankingUpdate );
				}
			}

			NetMsgRankingUpdate.Reset();
		}
	}

	if ( NetMsgRankingUpdate.wRankNum != 0 )
	{
		for( size_t i=0; i<vecCharID.size(); ++i )
		{
			PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( vecCharID[i] );
			if ( pChar )
			{
				GLAgentServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &NetMsgRankingUpdate );
			}
		}

		NetMsgRankingUpdate.Reset();
	}

	//send end ranking
	GLMSG::SNETPC_CLUB_SCORE_F2C_RANKING_END	NetMsgRankingEnd;
	NetMsgRankingEnd.dwGuidanceID = m_dwID;
	NetMsgRankingEnd.wPlayerNum = (WORD)m_mapPlayerData.size();

	for( size_t i=0; i<vecCharID.size(); ++i )
	{
		PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( vecCharID[i] );
		if ( pChar )
		{
			GLAgentServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &NetMsgRankingEnd );
		}
	}

	vecCharID.clear();
}

void GLGuidance::SendRewardPoint()
{
	SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER it_b = m_mapPlayerData.begin();
	SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER it_e = m_mapPlayerData.end();
	for( ; it_b != it_e; ++ it_b )
	{
		SPVP_CLUB_WAR_PLAYER_DATA& splayerdata = (*it_b).second;

		WORD wRewardTotal = splayerdata.wRewardBase + splayerdata.wRewardBonus;
		if ( wRewardTotal > 0 )
		{
			GLMSG::SNETPC_CLUB_SCORE_A2F_SEND_SCORE NetMsgFld;
			NetMsgFld.dwCharID = splayerdata.dwCharID;
			NetMsgFld.wRewardPoint = wRewardTotal;
			GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );
		}
	}

	m_mapPlayerData.clear();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
SCLUBWAR_RANK_INFO* GLGuidance::GetClubWarRankInfo( DWORD dwCharID )
{
	CLUBWAR_RANK_INFO_MAP_ITER pos = m_mapClubWarScore.find( dwCharID );
	if( pos == m_mapClubWarScore.end() )
	{
		return NULL;
	}

	return &(*pos).second;;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::AddClubWarScore( DWORD dwKillChar, DWORD dwDeathChar )
{
	if ( !IsBattle() ) return;

	SCLUBWAR_RANK_INFO* pKillClubWarRankInfo = GetClubWarRankInfo( dwKillChar );
	if ( pKillClubWarRankInfo )
	{
		pKillClubWarRankInfo->wKillNum++;
	}
	else
	{
		SCLUBWAR_RANK_INFO sClubWarRankInfo;		
		sClubWarRankInfo.dwCharID = dwKillChar;
		sClubWarRankInfo.wKillNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwKillChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sClubWarRankInfo.wGuNum = (WORD)(WORD)pCLUB->m_dwID;
				sClubWarRankInfo.wGuMarkVer = (WORD)(WORD)pCLUB->m_dwMarkVER;
			}
			sClubWarRankInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sClubWarRankInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sClubWarRankInfo.szName, CLUBWAR_CHAR_NAME+1, pChar->m_szName );
			sClubWarRankInfo.szName[CLUBWAR_CHAR_NAME] = '\0';
		}	

		m_mapClubWarScore[sClubWarRankInfo.dwCharID] = sClubWarRankInfo;
	}

	SCLUBWAR_RANK_INFO* pDeathClubWarRankInfo = GetClubWarRankInfo( dwDeathChar );
	if ( pDeathClubWarRankInfo )
	{
		pDeathClubWarRankInfo->wDeathNum++;
	}
	else
	{
		SCLUBWAR_RANK_INFO sClubWarRankInfo;		
		sClubWarRankInfo.dwCharID = dwDeathChar;
		sClubWarRankInfo.wDeathNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwDeathChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sClubWarRankInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sClubWarRankInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sClubWarRankInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sClubWarRankInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sClubWarRankInfo.szName, CLUBWAR_CHAR_NAME+1, pChar->m_szName );
			sClubWarRankInfo.szName[CLUBWAR_CHAR_NAME] = '\0';
		}

		m_mapClubWarScore[sClubWarRankInfo.dwCharID] = sClubWarRankInfo;
	}

	UpdateClubWarRanking();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::AddClubWarScoreResu( DWORD dwResuChar )
{
	if ( !IsBattle() ) return;

	SCLUBWAR_RANK_INFO* pResuClubWarRankInfo = GetClubWarRankInfo( dwResuChar );
	if ( pResuClubWarRankInfo )
	{
		pResuClubWarRankInfo->wResuNum++;
	}
	else
	{
		SCLUBWAR_RANK_INFO sClubWarRankInfo;		
		sClubWarRankInfo.dwCharID = dwResuChar;
		sClubWarRankInfo.wResuNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwResuChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sClubWarRankInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sClubWarRankInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sClubWarRankInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sClubWarRankInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sClubWarRankInfo.szName, CLUBWAR_CHAR_NAME+1, pChar->m_szName );
			sClubWarRankInfo.szName[CLUBWAR_CHAR_NAME] = '\0';
		}	

		m_mapClubWarScore[sClubWarRankInfo.dwCharID] = sClubWarRankInfo;
	}
	UpdateClubWarRanking();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::UpdateClubWarRanking()
{
	CLUBWAR_RANK_INFO_VEC	m_vecClubWarRankNew;
	m_vecClubWarRankNew.reserve( m_mapClubWarScore.size() );

	CLUBWAR_RANK_INFO_MAP_ITER	pos = m_mapClubWarScore.begin();
	CLUBWAR_RANK_INFO_MAP_ITER	end = m_mapClubWarScore.end();

	for ( ; pos != end; pos++ )
	{
		const SCLUBWAR_RANK_INFO&	sRankInfo = pos->second;					
		m_vecClubWarRankNew.push_back( sRankInfo );
	}

	std::sort( m_vecClubWarRankNew.begin(), m_vecClubWarRankNew.end() );	

	int nSize = m_vecClubWarRankNew.size();

	for ( int i = 0; i < nSize; ++i )
	{
		m_vecClubWarRankNew[i].wCharRanking = i+1;
		m_vecClubWarRankNew[i].nIndex = i;
	}

//	int nRanking;

	for ( int i = nSize-1; i > 0; --i )
	{
		if ( m_vecClubWarRankNew[i] == m_vecClubWarRankNew[i-1] )
		{
			m_vecClubWarRankNew[i-1].wCharRanking = m_vecClubWarRankNew[i].wCharRanking;			
		}
	}	


	GLMSG::SNET_CLUBWAR_RANKING_UPDATE	NetMsg;

	for ( int i = 0; i < (int)m_vecClubWarRankNew.size(); ++i )
	{
		CLUBWAR_RANK_INFO_MAP_ITER iter = m_mapClubWarScore.find( m_vecClubWarRankNew[i].dwCharID );
		if ( iter == m_mapClubWarScore.end() )	continue;

		SCLUBWAR_RANK_INFO&	sRankInfo = iter->second;

		{
			sRankInfo.wCharRanking = m_vecClubWarRankNew[i].wCharRanking;
			sRankInfo.nIndex = m_vecClubWarRankNew[i].nIndex;

			if ( m_vecClubWarRankNew[i].nIndex < CLUBWAR_RANKING_NUM ) 
			{
				SCLUBWAR_RANK sClubWarRank = sRankInfo;
				NetMsg.ADDRANK( sClubWarRank );
			}
		}		
	}

	if ( NetMsg.wRankNum > 0 )	
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( 204, &NetMsg );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( 214, &NetMsg );


	GLMSG::SNET_CLUBWAR_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapClubWarScore.begin();
	end = m_mapClubWarScore.end();

	for ( ; pos != end; pos++ )
	{
		const SCLUBWAR_RANK_INFO&	sRankInfo = pos->second;
		NetMsgMy.sMyClubWarRank.wCharRanking = sRankInfo.wCharRanking;
		NetMsgMy.sMyClubWarRank.nChaClass = sRankInfo.nChaClass;
		NetMsgMy.sMyClubWarRank.wKillNum = sRankInfo.wKillNum;
		NetMsgMy.sMyClubWarRank.wDeathNum = sRankInfo.wDeathNum;
		NetMsgMy.sMyClubWarRank.wResuNum = sRankInfo.wResuNum;
		NetMsgMy.sMyClubWarRank.wGuNum = sRankInfo.wGuNum;
		NetMsgMy.sMyClubWarRank.wGuMarkVer = sRankInfo.wGuMarkVer;
		NetMsgMy.sMyClubWarRank.wSchoolNum  = sRankInfo.wSchoolNum;
		StringCchCopy( NetMsgMy.sMyClubWarRank.szName, CLUBWAR_CHAR_NAME+1, sRankInfo.szName );

		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( 204, sRankInfo.dwCharID, &NetMsgMy );
		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( 214, sRankInfo.dwCharID, &NetMsgMy );
	}
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
SCLUBWAR_RANK_RESU_INFO* GLGuidance::GetClubWarRankResuInfo( DWORD dwCharID )
{
	CLUBWAR_RANK_RESU_INFO_MAP_ITER pos = m_mapClubWarScoreResu.find( dwCharID );
	if( pos == m_mapClubWarScoreResu.end() )
	{
		return NULL;
	}

	return &(*pos).second;;
}

void GLGuidance::AddClubWarResuScore( DWORD dwKillChar, DWORD dwDeathChar )
{
	if ( !IsBattle() ) return;

	SCLUBWAR_RANK_RESU_INFO* pKillClubWarRankResuInfo = GetClubWarRankResuInfo( dwKillChar );
	if ( pKillClubWarRankResuInfo )
	{
		pKillClubWarRankResuInfo->wKillNum++;
	}
	else
	{
		SCLUBWAR_RANK_RESU_INFO sClubWarRankResuInfo;		
		sClubWarRankResuInfo.dwCharID = dwKillChar;
		sClubWarRankResuInfo.wKillNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwKillChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sClubWarRankResuInfo.wGuNum = (WORD)(WORD)pCLUB->m_dwID;
				sClubWarRankResuInfo.wGuMarkVer = (WORD)(WORD)pCLUB->m_dwMarkVER;
			}
			sClubWarRankResuInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sClubWarRankResuInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sClubWarRankResuInfo.szName, CLUBWAR_CHAR_NAME+1, pChar->m_szName );
			sClubWarRankResuInfo.szName[CLUBWAR_CHAR_NAME] = '\0';
		}	

		m_mapClubWarScoreResu[sClubWarRankResuInfo.dwCharID] = sClubWarRankResuInfo;
	}

	SCLUBWAR_RANK_RESU_INFO* pDeathClubWarRankResuInfo = GetClubWarRankResuInfo( dwDeathChar );
	if ( pDeathClubWarRankResuInfo )
	{
		pDeathClubWarRankResuInfo->wDeathNum++;
	}
	else
	{
		SCLUBWAR_RANK_RESU_INFO sClubWarRankResuInfo;		
		sClubWarRankResuInfo.dwCharID = dwDeathChar;
		sClubWarRankResuInfo.wDeathNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwDeathChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sClubWarRankResuInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sClubWarRankResuInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sClubWarRankResuInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sClubWarRankResuInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sClubWarRankResuInfo.szName, CLUBWAR_CHAR_NAME+1, pChar->m_szName );
			sClubWarRankResuInfo.szName[CLUBWAR_CHAR_NAME] = '\0';
		}

		m_mapClubWarScoreResu[sClubWarRankResuInfo.dwCharID] = sClubWarRankResuInfo;
	}

	UpdateClubWarRankingResu();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::AddClubWarResuScoreResu( DWORD dwResuChar )
{
	if ( !IsBattle() ) return;

	SCLUBWAR_RANK_RESU_INFO* pResuClubWarRankResuInfo = GetClubWarRankResuInfo( dwResuChar );
	if ( pResuClubWarRankResuInfo )
	{
		pResuClubWarRankResuInfo->wResuNum++;
	}
	else
	{
		SCLUBWAR_RANK_RESU_INFO sClubWarRankResuInfo;		
		sClubWarRankResuInfo.dwCharID = dwResuChar;
		sClubWarRankResuInfo.wResuNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwResuChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sClubWarRankResuInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sClubWarRankResuInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sClubWarRankResuInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sClubWarRankResuInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sClubWarRankResuInfo.szName, CLUBWAR_CHAR_NAME+1, pChar->m_szName );
			sClubWarRankResuInfo.szName[CLUBWAR_CHAR_NAME] = '\0';
		}	

		m_mapClubWarScoreResu[sClubWarRankResuInfo.dwCharID] = sClubWarRankResuInfo;
	}
	UpdateClubWarRankingResu();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::UpdateClubWarRankingResu()
{
	CLUBWAR_RANK_RESU_INFO_VEC	m_vecClubWarRankResuNew;
	m_vecClubWarRankResuNew.reserve( m_mapClubWarScoreResu.size() );

	CLUBWAR_RANK_RESU_INFO_MAP_ITER	pos = m_mapClubWarScoreResu.begin();
	CLUBWAR_RANK_RESU_INFO_MAP_ITER	end = m_mapClubWarScoreResu.end();

	for ( ; pos != end; pos++ )
	{
		const SCLUBWAR_RANK_RESU_INFO&	sRankInfo = pos->second;					
		m_vecClubWarRankResuNew.push_back( sRankInfo );
	}

	std::sort( m_vecClubWarRankResuNew.begin(), m_vecClubWarRankResuNew.end() );	

	int nSize = m_vecClubWarRankResuNew.size();

	for ( int i = 0; i < nSize; ++i )
	{
		m_vecClubWarRankResuNew[i].wCharRanking = i+1;
		m_vecClubWarRankResuNew[i].nIndex = i;
	}

//	int nRanking;

	for ( int i = nSize-1; i > 0; --i )
	{
		if ( m_vecClubWarRankResuNew[i] == m_vecClubWarRankResuNew[i-1] )
		{
			m_vecClubWarRankResuNew[i-1].wCharRanking = m_vecClubWarRankResuNew[i].wCharRanking;			
		}
	}	


	GLMSG::SNET_CLUBWAR_RANKING_RESU_UPDATE	NetMsg;

	for ( int i = 0; i < (int)m_vecClubWarRankResuNew.size(); ++i )
	{
		CLUBWAR_RANK_RESU_INFO_MAP_ITER iter = m_mapClubWarScoreResu.find( m_vecClubWarRankResuNew[i].dwCharID );
		if ( iter == m_mapClubWarScoreResu.end() )	continue;

		SCLUBWAR_RANK_RESU_INFO&	sRankInfo = iter->second;

		{
			sRankInfo.wCharRanking = m_vecClubWarRankResuNew[i].wCharRanking;
			sRankInfo.nIndex = m_vecClubWarRankResuNew[i].nIndex;

			if ( m_vecClubWarRankResuNew[i].nIndex < CLUBWAR_RANKING_NUM ) 
			{
				SCLUBWAR_RANK_RESU sClubWarRankResu = sRankInfo;
				NetMsg.ADDRANK( sClubWarRankResu );
			}
		}		
	}

	if ( NetMsg.wRankNum > 0 )	
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( 204, &NetMsg );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( 214, &NetMsg );


	GLMSG::SNET_CLUBWAR_MYRANK_RESU_UPDATE	NetMsgMy;

	pos = m_mapClubWarScoreResu.begin();
	end = m_mapClubWarScoreResu.end();

	for ( ; pos != end; pos++ )
	{
		const SCLUBWAR_RANK_RESU_INFO&	sRankInfo = pos->second;
		NetMsgMy.sMyClubWarRankResu.wCharRanking = sRankInfo.wCharRanking;
		NetMsgMy.sMyClubWarRankResu.nChaClass = sRankInfo.nChaClass;
		NetMsgMy.sMyClubWarRankResu.wKillNum = sRankInfo.wKillNum;
		NetMsgMy.sMyClubWarRankResu.wDeathNum = sRankInfo.wDeathNum;
		NetMsgMy.sMyClubWarRankResu.wResuNum = sRankInfo.wResuNum;
		NetMsgMy.sMyClubWarRankResu.wGuNum = sRankInfo.wGuNum;
		NetMsgMy.sMyClubWarRankResu.wGuMarkVer = sRankInfo.wGuMarkVer;
		NetMsgMy.sMyClubWarRankResu.wSchoolNum = sRankInfo.wSchoolNum;
		StringCchCopy( NetMsgMy.sMyClubWarRankResu.szName, CLUBWAR_CHAR_NAME+1, sRankInfo.szName );

		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( 204, sRankInfo.dwCharID, &NetMsgMy );
		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( 214, sRankInfo.dwCharID, &NetMsgMy );
	}
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
SCLUBWAR_CLUB_RANK_INFO* GLGuidance::GetClubWarClubRankInfo( DWORD dwClubID )
{
	CLUBWAR_CLUB_RANK_INFO_MAP_ITER pos = m_mapClubWarClubScore.find( dwClubID );
	if( pos == m_mapClubWarClubScore.end() )
	{
		return NULL;
	}

	return &(*pos).second;;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::AddClubWarClubScore( DWORD dwKillClub, DWORD dwDeathClub )
{
	if ( !IsBattle() ) return;

	CLUBWAR_CLUB_RANK_INFO_MAP_ITER iter_find; 

	iter_find = m_mapClubWarClubScore.find(dwKillClub);
	if (iter_find != m_mapClubWarClubScore.end())
	{
		(*iter_find).second.wKillNum++;
	}
	else
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( dwKillClub );
		if ( pClub )
		{
			SCLUBWAR_CLUB_RANK_INFO sClubWarRankInfo;		
			sClubWarRankInfo.dwClubID = dwKillClub;
			sClubWarRankInfo.wKillNum++;

			sClubWarRankInfo.wGuNum = (WORD)pClub->m_dwID;
			sClubWarRankInfo.wGuMarkVer = (WORD)pClub->m_dwMarkVER;
			StringCchCopy ( sClubWarRankInfo.szClubName, CLUBWAR_CLUB_NAME+1, pClub->m_szName );
			sClubWarRankInfo.szClubName[CLUBWAR_CLUB_NAME] = '\0';

			m_mapClubWarClubScore.insert(std::make_pair(dwKillClub, sClubWarRankInfo));

		}
	}

	iter_find = m_mapClubWarClubScore.find(dwDeathClub);
	if (iter_find != m_mapClubWarClubScore.end())
	{
		(*iter_find).second.wDeathNum++;
	}
	else
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( dwDeathClub );
		if ( pClub )
		{
			SCLUBWAR_CLUB_RANK_INFO sClubWarRankInfo;		
			sClubWarRankInfo.dwClubID = dwDeathClub;
			sClubWarRankInfo.wDeathNum++;

			sClubWarRankInfo.wGuNum = (WORD)pClub->m_dwID;
			sClubWarRankInfo.wGuMarkVer = (WORD)pClub->m_dwMarkVER;
			StringCchCopy ( sClubWarRankInfo.szClubName, CLUBWAR_CLUB_NAME+1, pClub->m_szName );
			sClubWarRankInfo.szClubName[CLUBWAR_CLUB_NAME] = '\0';

			m_mapClubWarClubScore.insert(std::make_pair(dwDeathClub, sClubWarRankInfo));

		}
	}

	UpdateClubWarClubRanking();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::AddClubWarClubScoreResu( DWORD dwResuClub )
{
	if ( !IsBattle() ) return;

	CLUBWAR_CLUB_RANK_INFO_MAP_ITER iter_find; 

	iter_find = m_mapClubWarClubScore.find(dwResuClub);
	if (iter_find != m_mapClubWarClubScore.end())
	{
		(*iter_find).second.wResuNum++;
	}
	else
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( dwResuClub );
		if ( pClub )
		{
			SCLUBWAR_CLUB_RANK_INFO sClubWarRankInfo;		
			sClubWarRankInfo.dwClubID = dwResuClub;
			sClubWarRankInfo.wResuNum++;

			sClubWarRankInfo.wGuNum = (WORD)pClub->m_dwID;
			sClubWarRankInfo.wGuMarkVer = (WORD)pClub->m_dwMarkVER;
			StringCchCopy ( sClubWarRankInfo.szClubName, CLUBWAR_CLUB_NAME+1, pClub->m_szName );
			sClubWarRankInfo.szClubName[CLUBWAR_CLUB_NAME] = '\0';

			m_mapClubWarClubScore.insert(std::make_pair(dwResuClub, sClubWarRankInfo));

		}
	}

	UpdateClubWarClubRanking();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void GLGuidance::UpdateClubWarClubRanking()
{
	CLUBWAR_CLUB_RANK_INFO_VEC	m_vecClubWarClubRankNew;
	m_vecClubWarClubRankNew.reserve( m_mapClubWarClubScore.size() );

	CLUBWAR_CLUB_RANK_INFO_MAP_ITER	pos = m_mapClubWarClubScore.begin();
	CLUBWAR_CLUB_RANK_INFO_MAP_ITER	end = m_mapClubWarClubScore.end();

	for ( ; pos != end; pos++ )
	{
		const SCLUBWAR_CLUB_RANK_INFO&	sRankInfo = pos->second;					
		m_vecClubWarClubRankNew.push_back( sRankInfo );
	}

	std::sort( m_vecClubWarClubRankNew.begin(), m_vecClubWarClubRankNew.end() );	

	int nSize = m_vecClubWarClubRankNew.size();

	for ( int i = 0; i < nSize; ++i )
	{
		m_vecClubWarClubRankNew[i].wClubRanking = i+1;
		m_vecClubWarClubRankNew[i].nIndex = i;
	}

//	int nRanking;

	for ( int i = nSize-1; i > 0; --i )
	{
		if ( m_vecClubWarClubRankNew[i] == m_vecClubWarClubRankNew[i-1] )
		{
			m_vecClubWarClubRankNew[i-1].wClubRanking = m_vecClubWarClubRankNew[i].wClubRanking;			
		}
	}	


	GLMSG::SNET_CLUBWAR_CLUB_RANKING_UPDATE	NetMsg;

	for ( int i = 0; i < (int)m_vecClubWarClubRankNew.size(); ++i )
	{
		CLUBWAR_CLUB_RANK_INFO_MAP_ITER iter = m_mapClubWarClubScore.find( m_vecClubWarClubRankNew[i].dwClubID );
		if ( iter == m_mapClubWarClubScore.end() )	continue;

		SCLUBWAR_CLUB_RANK_INFO&	sRankInfo = iter->second;

		{
			sRankInfo.wClubRanking = m_vecClubWarClubRankNew[i].wClubRanking;
			sRankInfo.nIndex = m_vecClubWarClubRankNew[i].nIndex;

			if ( m_vecClubWarClubRankNew[i].nIndex < CLUBWAR_CLUB_RANKING_NUM ) 
			{
				SCLUBWAR_CLUB_RANK sClubWarClubRank = sRankInfo;
				NetMsg.ADDCLUB( sClubWarClubRank );
			}
		}		
	}

	if ( NetMsg.wRankNum > 0 )	
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( 204, &NetMsg );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( 214, &NetMsg );


	GLMSG::SNET_CLUBWAR_CLUB_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapClubWarClubScore.begin();
	end = m_mapClubWarClubScore.end();

	for ( ; pos != end; pos++ )
	{
		const SCLUBWAR_CLUB_RANK_INFO&	sRankInfo = pos->second;
		NetMsgMy.sMyClubWarClubRank.wClubRanking = sRankInfo.wClubRanking;
		NetMsgMy.sMyClubWarClubRank.wKillNum = sRankInfo.wKillNum;
		NetMsgMy.sMyClubWarClubRank.wDeathNum = sRankInfo.wDeathNum;
		NetMsgMy.sMyClubWarClubRank.wResuNum = sRankInfo.wResuNum;
		NetMsgMy.sMyClubWarClubRank.wGuNum = sRankInfo.wGuNum;
		NetMsgMy.sMyClubWarClubRank.wGuMarkVer = sRankInfo.wGuMarkVer;
		StringCchCopy( NetMsgMy.sMyClubWarClubRank.szClubName, CLUBWAR_CLUB_NAME+1, sRankInfo.szClubName );

		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( 204, sRankInfo.dwClubID, &NetMsgMy );
		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( 214, sRankInfo.dwClubID, &NetMsgMy );
	}

}