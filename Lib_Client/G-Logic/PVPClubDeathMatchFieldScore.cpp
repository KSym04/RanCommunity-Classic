#include "stdafx.h"
#include "./PVPClubDeathMatchField.h"

#include "./GLGaeaServer.h"

#include "../../Lib_Engine/Core/NSRLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void PVPClubDeathMatchField::ScoreKill( DWORD dwCharKilled, DWORD dwCharKiller )
{
	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE ) return; 

	PGLCHAR pCharKilled = GLGaeaServer::GetInstance().GetCharID( dwCharKilled );
	if ( pCharKilled )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClub = GetClubData( pCharKilled->m_dwGuild );
		if ( pClub )
		{
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayer = pClub->PlayerDataGet( pCharKilled->m_dwCharID );
			if ( pPlayer )
			{
				WORD wUsePoint = m_wPointDeathLoss;
				if ( m_bDoublePoint )
					wUsePoint = m_wDoublePointDeathLoss;

				if ( pClub->wPoints >= wUsePoint )
					pClub->wPoints -= wUsePoint;
				else
					pClub->wPoints = 0;

				pClub->wDeaths ++;
			}
		}
	}

	PGLCHAR pCharKiller = GLGaeaServer::GetInstance().GetCharID( dwCharKiller );
	if ( pCharKiller )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClub = GetClubData( pCharKiller->m_dwGuild );
		if ( pClub )
		{
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayer = pClub->PlayerDataGet( pCharKiller->m_dwCharID );
			if ( pPlayer )
			{
				WORD wUsePoint = m_wPointKillGain;
				if ( m_bDoublePoint )
					wUsePoint = m_wDoublePointKillGain;

				pClub->wPoints += wUsePoint;
				pClub->wKills ++;
			}
		}
	}
}

void PVPClubDeathMatchField::ScoreResu( DWORD dwChar )
{
	PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID( dwChar );
	if ( pChar )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClub = GetClubData( pChar->m_dwGuild );
		if ( pClub )
		{
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayer = pClub->PlayerDataGet( pChar->m_dwCharID );
			if ( pPlayer )
			{
				pClub->wPoints += m_wPointResu;
				pClub->wResu ++;
			}
		}
	}
}

void PVPClubDeathMatchField::RankingUpdate()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK_VEC vecRank;
	vecRank.reserve( m_mapClubData.size() );

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();
	for( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;
		PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK sRank = sData;

		vecRank.push_back( sRank );
	}

	std::sort( vecRank.begin(), vecRank.end() );	

	for( size_t i=0; i<vecRank.size(); ++i )
	{
		vecRank[i].wRank = i+1;
	}

	for( size_t i=0; i<vecRank.size(); ++i )
	{
		const PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK& sRank = vecRank[i];

		if ( sRank.wRank == 1 )
			m_sFirstClubRank = sRank;

		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( sRank.dwClubID );
		if ( pClubData && pClubData->dwClubID == sRank.dwClubID )
		{
			pClubData->wRank = sRank.wRank;
		}
	}
}

void PVPClubDeathMatchField::RankingSend( BOOL bFinal /*= FALSE*/ )
{
	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE NetMsgUpdate;
	NetMsgUpdate.bFinal = bFinal;

	WORD wNum = 0;
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();
	for( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;
		NetMsgUpdate.ADD( sData );
		if ( NetMsgUpdate.wDataNum == PVP_CLUB_DEATH_MATCH_PACKET_CLUB_DATA_NUM )
		{
			GLGaeaServer::GetInstance().SENDTOAGENT( &NetMsgUpdate );
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &NetMsgUpdate );
			GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &NetMsgUpdate );

			wNum += NetMsgUpdate.wDataNum;
			NetMsgUpdate.RESET();
		}
	}

	if ( NetMsgUpdate.wDataNum != 0 )
	{
		GLGaeaServer::GetInstance().SENDTOAGENT( &NetMsgUpdate );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &NetMsgUpdate );
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &NetMsgUpdate );

		wNum += NetMsgUpdate.wDataNum;
		NetMsgUpdate.RESET();
	}

	//RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchField::RankingSend num: %u", wNum );
	//GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PVPClubDeathMatchField::RankingSend num: %u", wNum );

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END NetMsgEnd;
	NetMsgEnd.bFinal = bFinal;
	NetMsgEnd.dwRankNum = (DWORD)m_mapClubData.size();
	GLGaeaServer::GetInstance().SENDTOAGENT( &NetMsgEnd );
	GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sBattleMap.dwID, &NetMsgEnd );
	GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP( m_sLobbyMap.dwID, &NetMsgEnd );
}

void PVPClubDeathMatchField::RequestScoreInfo( DWORD dwCharID )
{
	PGLCHAR pchar = GLGaeaServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE	NetMsgClient;
	NetMsgClient.sFirstRank = m_sFirstClubRank;
	
	PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pchar->m_dwGuild );
	if ( pClubData )
	{
		PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayerData = pClubData->PlayerDataGet( pchar->m_dwCharID );
		if ( pPlayerData )
		{
			NetMsgClient.sMyRank = *pClubData;
		}
	}

	NetMsgClient.bDoublePoint = m_bDoublePoint;

	GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &NetMsgClient );
}
