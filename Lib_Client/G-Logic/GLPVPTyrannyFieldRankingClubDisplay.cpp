#include "StdAfx.h"
#include "./GLPVPTyrannyField.h"
#include "./GLGaeaServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 7-13-23 Tyranny War Ranking System - Montage */
STYRANNY_CLUB_RANK_INFO* GLPVPTyrannyField::GetTyrannyClubRankInfo( DWORD dwClubID )
{
	TYRANNY_CLUB_RANK_INFO_MAP_ITER pos = m_mapTyrannyClubScore.find( dwClubID );
	if( pos == m_mapTyrannyClubScore.end() )
	{
		return NULL;
	}

	return &(*pos).second;;
}

void GLPVPTyrannyField::AddTyrannyClubScore( DWORD dwKillClub, DWORD dwDeathClub )
{
	if ( !IsBattle() ) return;

	TYRANNY_CLUB_RANK_INFO_MAP_ITER iter_find; 

	iter_find = m_mapTyrannyClubScore.find(dwKillClub);
	if (iter_find != m_mapTyrannyClubScore.end())
	{
		(*iter_find).second.wKillNum++;
	}
	else
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( dwKillClub );
		if ( pClub )
		{
			STYRANNY_CLUB_RANK_INFO sTyrannyRankInfo;		
			sTyrannyRankInfo.dwClubID = dwKillClub;
			sTyrannyRankInfo.wKillNum++;

			sTyrannyRankInfo.wGuNum = (WORD)pClub->m_dwID;
			sTyrannyRankInfo.wGuMarkVer = (WORD)pClub->m_dwMarkVER;
			StringCchCopy ( sTyrannyRankInfo.szClubName, TYRANNY_CLUB_NAME+1, pClub->m_szName );
			sTyrannyRankInfo.szClubName[TYRANNY_CLUB_NAME] = '\0';

			m_mapTyrannyClubScore.insert(std::make_pair(dwKillClub, sTyrannyRankInfo));

		}
	}

	iter_find = m_mapTyrannyClubScore.find(dwDeathClub);
	if (iter_find != m_mapTyrannyClubScore.end())
	{
		(*iter_find).second.wDeathNum++;
	}
	else
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( dwDeathClub );
		if ( pClub )
		{
			STYRANNY_CLUB_RANK_INFO sTyrannyRankInfo;		
			sTyrannyRankInfo.dwClubID = dwDeathClub;
			sTyrannyRankInfo.wDeathNum++;

			sTyrannyRankInfo.wGuNum = (WORD)pClub->m_dwID;
			sTyrannyRankInfo.wGuMarkVer = (WORD)pClub->m_dwMarkVER;
			StringCchCopy ( sTyrannyRankInfo.szClubName, TYRANNY_CLUB_NAME+1, pClub->m_szName );
			sTyrannyRankInfo.szClubName[TYRANNY_CLUB_NAME] = '\0';

			m_mapTyrannyClubScore.insert(std::make_pair(dwDeathClub, sTyrannyRankInfo));

		}
	}

	UpdateTyrannyClubRanking();
}

void GLPVPTyrannyField::AddTyrannyClubScoreResu( DWORD dwResuClub )
{
	if ( !IsBattle() ) return;

	TYRANNY_CLUB_RANK_INFO_MAP_ITER iter_find; 

	iter_find = m_mapTyrannyClubScore.find(dwResuClub);
	if (iter_find != m_mapTyrannyClubScore.end())
	{
		(*iter_find).second.wResuNum++;
	}
	else
	{
		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub( dwResuClub );
		if ( pClub )
		{
			STYRANNY_CLUB_RANK_INFO sTyrannyRankInfo;		
			sTyrannyRankInfo.dwClubID = dwResuClub;
			sTyrannyRankInfo.wResuNum++;

			sTyrannyRankInfo.wGuNum = (WORD)pClub->m_dwID;
			sTyrannyRankInfo.wGuMarkVer = (WORD)pClub->m_dwMarkVER;
			StringCchCopy ( sTyrannyRankInfo.szClubName, TYRANNY_CLUB_NAME+1, pClub->m_szName );
			sTyrannyRankInfo.szClubName[TYRANNY_CLUB_NAME] = '\0';

			m_mapTyrannyClubScore.insert(std::make_pair(dwResuClub, sTyrannyRankInfo));

		}
	}

	UpdateTyrannyClubRanking();
}


void GLPVPTyrannyField::UpdateTyrannyClubRanking()
{
	TYRANNY_CLUB_RANK_INFO_VEC	m_vecTyrannyClubRankNew;
	m_vecTyrannyClubRankNew.reserve( m_mapTyrannyClubScore.size() );

	TYRANNY_CLUB_RANK_INFO_MAP_ITER	pos = m_mapTyrannyClubScore.begin();
	TYRANNY_CLUB_RANK_INFO_MAP_ITER	end = m_mapTyrannyClubScore.end();

	for ( ; pos != end; pos++ )
	{
		const STYRANNY_CLUB_RANK_INFO&	sRankInfo = pos->second;					
		m_vecTyrannyClubRankNew.push_back( sRankInfo );
	}

	std::sort( m_vecTyrannyClubRankNew.begin(), m_vecTyrannyClubRankNew.end() );	

	int nSize = m_vecTyrannyClubRankNew.size();

	for ( int i = 0; i < nSize; ++i )
	{
		m_vecTyrannyClubRankNew[i].wClubRanking = i+1;
		m_vecTyrannyClubRankNew[i].nIndex = i;
	}

//	int nRanking;

	for ( int i = nSize-1; i > 0; --i )
	{
		if ( m_vecTyrannyClubRankNew[i] == m_vecTyrannyClubRankNew[i-1] )
		{
			m_vecTyrannyClubRankNew[i-1].wClubRanking = m_vecTyrannyClubRankNew[i].wClubRanking;			
		}
	}	


	GLMSG::SNET_TYRANNY_CLUB_RANKING_UPDATE	NetMsg;

	for ( int i = 0; i < (int)m_vecTyrannyClubRankNew.size(); ++i )
	{
		TYRANNY_CLUB_RANK_INFO_MAP_ITER iter = m_mapTyrannyClubScore.find( m_vecTyrannyClubRankNew[i].dwClubID );
		if ( iter == m_mapTyrannyClubScore.end() )	continue;

		STYRANNY_CLUB_RANK_INFO&	sRankInfo = iter->second;

		{
			sRankInfo.wClubRanking = m_vecTyrannyClubRankNew[i].wClubRanking;
			sRankInfo.nIndex = m_vecTyrannyClubRankNew[i].nIndex;

			if ( m_vecTyrannyClubRankNew[i].nIndex < TYRANNY_CLUB_RANKING_NUM ) 
			{
				STYRANNY_CLUB_RANK sTyrannyClubRank = sRankInfo;
				NetMsg.ADDCLUB( sTyrannyClubRank );
			}
		}		
	}

	if ( NetMsg.wRankNum > 0 )	
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sScheduleNext.getBatteMap().dwID, &NetMsg );


	GLMSG::SNET_TYRANNY_CLUB_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapTyrannyClubScore.begin();
	end = m_mapTyrannyClubScore.end();

	for ( ; pos != end; pos++ )
	{
		const STYRANNY_CLUB_RANK_INFO&	sRankInfo = pos->second;
		NetMsgMy.sMyTyrannyClubRank.wClubRanking = sRankInfo.wClubRanking;
		NetMsgMy.sMyTyrannyClubRank.wKillNum = sRankInfo.wKillNum;
		NetMsgMy.sMyTyrannyClubRank.wDeathNum = sRankInfo.wDeathNum;
		NetMsgMy.sMyTyrannyClubRank.wResuNum = sRankInfo.wResuNum;
		NetMsgMy.sMyTyrannyClubRank.wGuNum = sRankInfo.wGuNum;
		NetMsgMy.sMyTyrannyClubRank.wGuMarkVer = sRankInfo.wGuMarkVer;
		StringCchCopy( NetMsgMy.sMyTyrannyClubRank.szClubName, TYRANNY_CLUB_NAME+1, sRankInfo.szClubName );

		GLGaeaServer::GetInstance().SENDTOCLIENTPLAYERCLUB_ONMAP( m_sScheduleNext.getBatteMap().dwID, sRankInfo.dwClubID, &NetMsgMy );
	}

}