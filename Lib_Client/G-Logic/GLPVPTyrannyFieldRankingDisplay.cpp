#include "StdAfx.h"
#include "./GLPVPTyrannyField.h"
#include "./GLGaeaServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 7-13-23 Tyranny War Ranking System - Montage */
STYRANNY_RANK_INFO* GLPVPTyrannyField::GetTyrannyRankInfo( DWORD dwCharID )
{
	TYRANNY_RANK_INFO_MAP_ITER pos = m_mapTyrannyScore.find( dwCharID );
	if( pos == m_mapTyrannyScore.end() )
	{
		return NULL;
	}

	return &(*pos).second;;
}

void GLPVPTyrannyField::AddTyrannyScore( DWORD dwKillChar, DWORD dwDeathChar )
{
	if ( !IsBattle() ) return;

	STYRANNY_RANK_INFO* pKillTyrannyRankInfo = GetTyrannyRankInfo( dwKillChar );
	if ( pKillTyrannyRankInfo )
	{
		pKillTyrannyRankInfo->wKillNum++;
	}
	else
	{
		STYRANNY_RANK_INFO sTyrannyRankInfo;		
		sTyrannyRankInfo.dwCharID = dwKillChar;
		sTyrannyRankInfo.wKillNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwKillChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sTyrannyRankInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sTyrannyRankInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sTyrannyRankInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sTyrannyRankInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sTyrannyRankInfo.szName, TYRANNY_CHAR_NAME+1, pChar->m_szName );
			sTyrannyRankInfo.szName[TYRANNY_CHAR_NAME] = '\0';
		}	

		m_mapTyrannyScore[sTyrannyRankInfo.dwCharID] = sTyrannyRankInfo;
	}

	STYRANNY_RANK_INFO* pDeathTyrannyRankInfo = GetTyrannyRankInfo( dwDeathChar );
	if ( pDeathTyrannyRankInfo )
	{
		pDeathTyrannyRankInfo->wDeathNum++;
	}
	else
	{
		STYRANNY_RANK_INFO sTyrannyRankInfo;		
		sTyrannyRankInfo.dwCharID = dwDeathChar;
		sTyrannyRankInfo.wDeathNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwDeathChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sTyrannyRankInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sTyrannyRankInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sTyrannyRankInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sTyrannyRankInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sTyrannyRankInfo.szName, TYRANNY_CHAR_NAME+1, pChar->m_szName );
			sTyrannyRankInfo.szName[TYRANNY_CHAR_NAME] = '\0';
		}

		m_mapTyrannyScore[sTyrannyRankInfo.dwCharID] = sTyrannyRankInfo;
	}

	UpdateTyrannyRanking();
}

void GLPVPTyrannyField::AddTyrannyScoreResu( DWORD dwResuChar )
{
	if ( !IsBattle() ) return;

	STYRANNY_RANK_INFO* pResuTyrannyRankInfo = GetTyrannyRankInfo( dwResuChar );
	if ( pResuTyrannyRankInfo )
	{
		pResuTyrannyRankInfo->wResuNum++;
	}
	else
	{
		STYRANNY_RANK_INFO sTyrannyRankInfo;		
		sTyrannyRankInfo.dwCharID = dwResuChar;
		sTyrannyRankInfo.wResuNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwResuChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sTyrannyRankInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sTyrannyRankInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sTyrannyRankInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sTyrannyRankInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sTyrannyRankInfo.szName, TYRANNY_CHAR_NAME+1, pChar->m_szName );
			sTyrannyRankInfo.szName[TYRANNY_CHAR_NAME] = '\0';
		}	

		m_mapTyrannyScore[sTyrannyRankInfo.dwCharID] = sTyrannyRankInfo;
	}
	UpdateTyrannyRanking();
}


void GLPVPTyrannyField::UpdateTyrannyRanking()
{
	TYRANNY_RANK_INFO_VEC	m_vecTyrannyRankNew;
	m_vecTyrannyRankNew.reserve( m_mapTyrannyScore.size() );

	TYRANNY_RANK_INFO_MAP_ITER	pos = m_mapTyrannyScore.begin();
	TYRANNY_RANK_INFO_MAP_ITER	end = m_mapTyrannyScore.end();

	for ( ; pos != end; pos++ )
	{
		const STYRANNY_RANK_INFO&	sRankInfo = pos->second;					
		m_vecTyrannyRankNew.push_back( sRankInfo );
	}

	std::sort( m_vecTyrannyRankNew.begin(), m_vecTyrannyRankNew.end() );	

	int nSize = m_vecTyrannyRankNew.size();

	for ( int i = 0; i < nSize; ++i )
	{
		m_vecTyrannyRankNew[i].wCharRanking = i+1;
		m_vecTyrannyRankNew[i].nIndex = i;
	}

//	int nRanking;

	for ( int i = nSize-1; i > 0; --i )
	{
		if ( m_vecTyrannyRankNew[i] == m_vecTyrannyRankNew[i-1] )
		{
			m_vecTyrannyRankNew[i-1].wCharRanking = m_vecTyrannyRankNew[i].wCharRanking;			
		}
	}	


	GLMSG::SNET_TYRANNY_RANKING_UPDATE	NetMsg;

	for ( int i = 0; i < (int)m_vecTyrannyRankNew.size(); ++i )
	{
		TYRANNY_RANK_INFO_MAP_ITER iter = m_mapTyrannyScore.find( m_vecTyrannyRankNew[i].dwCharID );
		if ( iter == m_mapTyrannyScore.end() )	continue;

		STYRANNY_RANK_INFO&	sRankInfo = iter->second;

		{
			sRankInfo.wCharRanking = m_vecTyrannyRankNew[i].wCharRanking;
			sRankInfo.nIndex = m_vecTyrannyRankNew[i].nIndex;

			if ( m_vecTyrannyRankNew[i].nIndex < TYRANNY_RANKING_NUM ) 
			{
				STYRANNY_RANK sTyrannyRank = sRankInfo;
				NetMsg.ADDRANK( sTyrannyRank );
			}
		}		
	}

	if ( NetMsg.wRankNum > 0 )	
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sScheduleNext.getBatteMap().dwID, &NetMsg );


	GLMSG::SNET_TYRANNY_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapTyrannyScore.begin();
	end = m_mapTyrannyScore.end();

	for ( ; pos != end; pos++ )
	{
		const STYRANNY_RANK_INFO&	sRankInfo = pos->second;
		NetMsgMy.sMyTyrannyRank.wCharRanking = sRankInfo.wCharRanking;
		NetMsgMy.sMyTyrannyRank.nChaClass = sRankInfo.nChaClass;
		NetMsgMy.sMyTyrannyRank.wKillNum = sRankInfo.wKillNum;
		NetMsgMy.sMyTyrannyRank.wDeathNum = sRankInfo.wDeathNum;
		NetMsgMy.sMyTyrannyRank.wResuNum = sRankInfo.wResuNum;
		NetMsgMy.sMyTyrannyRank.wGuNum = sRankInfo.wGuNum;
		NetMsgMy.sMyTyrannyRank.wGuMarkVer = sRankInfo.wGuMarkVer;
		NetMsgMy.sMyTyrannyRank.wSchoolNum  = sRankInfo.wSchoolNum;
		StringCchCopy( NetMsgMy.sMyTyrannyRank.szName, TYRANNY_CHAR_NAME+1, sRankInfo.szName );

		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( m_sScheduleNext.getBatteMap().dwID, sRankInfo.dwCharID, &NetMsgMy );
	}
}

/******/
/* 7-13-23 Tyranny War Ranking System - Montage */
STYRANNY_RANK_RESU_INFO* GLPVPTyrannyField::GetTyrannyRankResuInfo( DWORD dwCharID )
{
	TYRANNY_RANK_RESU_INFO_MAP_ITER pos = m_mapTyrannyScoreResu.find( dwCharID );
	if( pos == m_mapTyrannyScoreResu.end() )
	{
		return NULL;
	}

	return &(*pos).second;;
}

void GLPVPTyrannyField::AddTyrannyResuScore( DWORD dwKillChar, DWORD dwDeathChar )
{
	if ( !IsBattle() ) return;

	STYRANNY_RANK_RESU_INFO* pKillTyrannyRankResuInfo = GetTyrannyRankResuInfo( dwKillChar );
	if ( pKillTyrannyRankResuInfo )
	{
		pKillTyrannyRankResuInfo->wKillNum++;
	}
	else
	{
		STYRANNY_RANK_RESU_INFO sTyrannyRankResuInfo;		
		sTyrannyRankResuInfo.dwCharID = dwKillChar;
		sTyrannyRankResuInfo.wKillNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwKillChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sTyrannyRankResuInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sTyrannyRankResuInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sTyrannyRankResuInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sTyrannyRankResuInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sTyrannyRankResuInfo.szName, TYRANNY_CHAR_NAME+1, pChar->m_szName );
			sTyrannyRankResuInfo.szName[TYRANNY_CHAR_NAME] = '\0';
		}	

		m_mapTyrannyScoreResu[sTyrannyRankResuInfo.dwCharID] = sTyrannyRankResuInfo;
	}

	STYRANNY_RANK_RESU_INFO* pDeathTyrannyRankResuInfo = GetTyrannyRankResuInfo( dwDeathChar );
	if ( pDeathTyrannyRankResuInfo )
	{
		pDeathTyrannyRankResuInfo->wDeathNum++;
	}
	else
	{
		STYRANNY_RANK_RESU_INFO sTyrannyRankResuInfo;		
		sTyrannyRankResuInfo.dwCharID = dwDeathChar;
		sTyrannyRankResuInfo.wDeathNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwDeathChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sTyrannyRankResuInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sTyrannyRankResuInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sTyrannyRankResuInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sTyrannyRankResuInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sTyrannyRankResuInfo.szName, TYRANNY_CHAR_NAME+1, pChar->m_szName );
			sTyrannyRankResuInfo.szName[TYRANNY_CHAR_NAME] = '\0';
		}

		m_mapTyrannyScoreResu[sTyrannyRankResuInfo.dwCharID] = sTyrannyRankResuInfo;
	}

	UpdateTyrannyRankingResu();
}

void GLPVPTyrannyField::AddTyrannyResuScoreResu( DWORD dwResuChar )
{
	if ( !IsBattle() ) return;

	STYRANNY_RANK_RESU_INFO* pResuTyrannyRankResuInfo = GetTyrannyRankResuInfo( dwResuChar );
	if ( pResuTyrannyRankResuInfo )
	{
		pResuTyrannyRankResuInfo->wResuNum++;
	}
	else
	{
		STYRANNY_RANK_RESU_INFO sTyrannyRankResuInfo;		
		sTyrannyRankResuInfo.dwCharID = dwResuChar;
		sTyrannyRankResuInfo.wResuNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID ( dwResuChar );
		if ( pChar )
		{
			GLClubMan& cClubMan	= GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB* pCLUB		= cClubMan.GetClub(pChar->m_dwGuild);
			if ( pCLUB )
			{
				sTyrannyRankResuInfo.wGuNum = (WORD)pCLUB->m_dwID;
				sTyrannyRankResuInfo.wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
			}
			sTyrannyRankResuInfo.nChaClass = (BYTE)CharClassToIndex( pChar->m_emClass );
			sTyrannyRankResuInfo.wSchoolNum = pChar->m_wSchool;

			StringCchCopy ( sTyrannyRankResuInfo.szName, TYRANNY_CHAR_NAME+1, pChar->m_szName );
			sTyrannyRankResuInfo.szName[TYRANNY_CHAR_NAME] = '\0';
		}	

		m_mapTyrannyScoreResu[sTyrannyRankResuInfo.dwCharID] = sTyrannyRankResuInfo;
	}
	UpdateTyrannyRankingResu();
}


void GLPVPTyrannyField::UpdateTyrannyRankingResu()
{
	TYRANNY_RANK_RESU_INFO_VEC	m_vecTyrannyRankResuNew;
	m_vecTyrannyRankResuNew.reserve( m_mapTyrannyScoreResu.size() );

	TYRANNY_RANK_RESU_INFO_MAP_ITER	pos = m_mapTyrannyScoreResu.begin();
	TYRANNY_RANK_RESU_INFO_MAP_ITER	end = m_mapTyrannyScoreResu.end();

	for ( ; pos != end; pos++ )
	{
		const STYRANNY_RANK_RESU_INFO&	sRankInfo = pos->second;					
		m_vecTyrannyRankResuNew.push_back( sRankInfo );
	}

	std::sort( m_vecTyrannyRankResuNew.begin(), m_vecTyrannyRankResuNew.end() );	

	int nSize = m_vecTyrannyRankResuNew.size();

	for ( int i = 0; i < nSize; ++i )
	{
		m_vecTyrannyRankResuNew[i].wCharRanking = i+1;
		m_vecTyrannyRankResuNew[i].nIndex = i;
	}

//	int nRanking;

	for ( int i = nSize-1; i > 0; --i )
	{
		if ( m_vecTyrannyRankResuNew[i] == m_vecTyrannyRankResuNew[i-1] )
		{
			m_vecTyrannyRankResuNew[i-1].wCharRanking = m_vecTyrannyRankResuNew[i].wCharRanking;			
		}
	}	


	GLMSG::SNET_TYRANNY_RANKING_RESU_UPDATE	NetMsg;

	for ( int i = 0; i < (int)m_vecTyrannyRankResuNew.size(); ++i )
	{
		TYRANNY_RANK_RESU_INFO_MAP_ITER iter = m_mapTyrannyScoreResu.find( m_vecTyrannyRankResuNew[i].dwCharID );
		if ( iter == m_mapTyrannyScoreResu.end() )	continue;

		STYRANNY_RANK_RESU_INFO&	sRankInfo = iter->second;

		{
			sRankInfo.wCharRanking = m_vecTyrannyRankResuNew[i].wCharRanking;
			sRankInfo.nIndex = m_vecTyrannyRankResuNew[i].nIndex;

			if ( m_vecTyrannyRankResuNew[i].nIndex < TYRANNY_RANKING_NUM ) 
			{
				STYRANNY_RANK_RESU sTyrannyRankResu = sRankInfo;
				NetMsg.ADDRANK( sTyrannyRankResu );
			}
		}		
	}

	if ( NetMsg.wRankNum > 0 )	
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sScheduleNext.getBatteMap().dwID, &NetMsg );


	GLMSG::SNET_TYRANNY_MYRANK_RESU_UPDATE	NetMsgMy;

	pos = m_mapTyrannyScoreResu.begin();
	end = m_mapTyrannyScoreResu.end();

	for ( ; pos != end; pos++ )
	{
		const STYRANNY_RANK_RESU_INFO&	sRankInfo = pos->second;
		NetMsgMy.sMyTyrannyRankResu.wCharRanking = sRankInfo.wCharRanking;
		NetMsgMy.sMyTyrannyRankResu.nChaClass = sRankInfo.nChaClass;
		NetMsgMy.sMyTyrannyRankResu.wKillNum = sRankInfo.wKillNum;
		NetMsgMy.sMyTyrannyRankResu.wDeathNum = sRankInfo.wDeathNum;
		NetMsgMy.sMyTyrannyRankResu.wResuNum = sRankInfo.wResuNum;
		NetMsgMy.sMyTyrannyRankResu.wGuNum = sRankInfo.wGuNum;
		NetMsgMy.sMyTyrannyRankResu.wGuMarkVer = sRankInfo.wGuMarkVer;
		NetMsgMy.sMyTyrannyRankResu.wSchoolNum = sRankInfo.wSchoolNum;
		StringCchCopy( NetMsgMy.sMyTyrannyRankResu.szName, TYRANNY_CHAR_NAME+1, sRankInfo.szName );

		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( m_sScheduleNext.getBatteMap().dwID, sRankInfo.dwCharID, &NetMsgMy );
	}
}