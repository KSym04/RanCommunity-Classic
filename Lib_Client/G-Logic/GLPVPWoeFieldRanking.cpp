#include "stdafx.h"
#include "GLPVPWoeField.h"
#include "GLGaeaServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLPVPWoeField::InsertPlayerData( WOE_REGISTER_DATA* pPlayerData )
{
	if ( !pPlayerData )			return;
	if ( pPlayerData->dwInfoCharID == WOE_PLAYER_NULL )
	{
		return;
	}

	WOE_PLAYER_DATA* pdata = PlayerDataGet( pPlayerData->dwInfoCharID );
	if ( pdata )
	{
		return;
	}
		
	if ( pPlayerData->wInfoClass >= GLCI_NUM_8CLASS )
	{
		return;
	}

	WOE_PLAYER_DATA sdata;
	sdata.dwInfoCharID	= pPlayerData->dwInfoCharID;
	sdata.wInfoClass	= pPlayerData->wInfoClass;
	sdata.wInfoSchool	= pPlayerData->wInfoSchool;
	StringCchCopy( sdata.szInfoCharName, WOE_CHARNAME_STRING_SIZE, pPlayerData->szInfoCharName );
	m_mapGuildData.insert(std::make_pair(sdata.dwInfoCharID, sdata));
}

void GLPVPWoeField::AddWoeGuildScore(DWORD dwKillClub, DWORD dwDeathClub)
{
	if (!IsBattle()) return;

	SWOE_GUILD_RANK_INFO* pKillClubInfo = GetWoeGuildRankInfo(dwKillClub);
	if (pKillClubInfo)
	{
		pKillClubInfo->wKillNum++;
	}
	else
	{
		SWOE_GUILD_RANK_INFO sWoeGuildRankInfo;
		sWoeGuildRankInfo.dwClubID = dwKillClub;
		sWoeGuildRankInfo.wKillNum++;

		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub(dwKillClub);
		if (pClub)
		{
			StringCchCopy(sWoeGuildRankInfo.szClubName, CLUB_NAME + 1, pClub->m_szName);
			sWoeGuildRankInfo.szClubName[CLUB_NAME] = '\0';

			StringCchCopy(sWoeGuildRankInfo.szLeaderName, CHAR_SZNAME + 1, pClub->m_szMasterName);
			sWoeGuildRankInfo.szLeaderName[CHAR_SZNAME] = '\0';

			sWoeGuildRankInfo.m_dwMarkVER = pClub->m_dwMarkVER;
		}

		m_mapWoeScore[sWoeGuildRankInfo.dwClubID] = sWoeGuildRankInfo;
	}

	SWOE_GUILD_RANK_INFO* pDeathClubInfo = GetWoeGuildRankInfo(dwDeathClub);
	if (pDeathClubInfo)
	{
		pDeathClubInfo->wDeathNum++;
	}
	else
	{
		SWOE_GUILD_RANK_INFO sWoeGuildRankInfo;
		sWoeGuildRankInfo.dwClubID = dwDeathClub;
		sWoeGuildRankInfo.wDeathNum++;

		GLCLUB* pClub = GLGaeaServer::GetInstance().GetClubMan().GetClub(dwDeathClub);
		if (pClub)
		{
			StringCchCopy(sWoeGuildRankInfo.szClubName, CLUB_NAME + 1, pClub->m_szName);
			sWoeGuildRankInfo.szClubName[CLUB_NAME] = '\0';

			StringCchCopy(sWoeGuildRankInfo.szLeaderName, CHAR_SZNAME + 1, pClub->m_szMasterName);
			sWoeGuildRankInfo.szLeaderName[CHAR_SZNAME] = '\0';

			sWoeGuildRankInfo.m_dwMarkVER = pClub->m_dwMarkVER;
		}

		m_mapWoeScore[sWoeGuildRankInfo.dwClubID] = sWoeGuildRankInfo;
	}
}
void GLPVPWoeField::UpdateGuildScore()
{
	WOE_GUILD_RANK_INFO_VEC	m_vecWoeGuild;
	m_vecWoeGuild.reserve(m_mapWoeScore.size());

	WOE_GUILD_RANK_INFO_MAP_ITER pos = m_mapWoeScore.begin();
	WOE_GUILD_RANK_INFO_MAP_ITER end = m_mapWoeScore.end();

	for (; pos != end; pos++)
	{
		const SWOE_GUILD_RANK_INFO& sRankInfo = pos->second;
		m_vecWoeGuild.push_back(sRankInfo);
	}

	std::sort(m_vecWoeGuild.begin(), m_vecWoeGuild.end());

	int nSize = m_vecWoeGuild.size();

	for (int i = 0; i < nSize; ++i)
	{
		m_vecWoeGuild[i].wClubRanking = i + 1;
		m_vecWoeGuild[i].nIndex = i;
	}

	int nRanking;

	for (int i = nSize - 1; i > 0; --i)
	{
		if (m_vecWoeGuild[i] == m_vecWoeGuild[i - 1])
		{
			m_vecWoeGuild[i - 1].wClubRanking = m_vecWoeGuild[i].wClubRanking;
		}
	}


	GLMSG::SNET_WOE_GUILD_RANKING_UPDATE	NetMsg;

	for (int i = 0; i < m_vecWoeGuild.size(); ++i)
	{
		WOE_GUILD_RANK_INFO_MAP_ITER iter = m_mapWoeScore.find(m_vecWoeGuild[i].dwClubID);
		if (iter == m_mapWoeScore.end())	continue;

		SWOE_GUILD_RANK_INFO& sGRankInfo = iter->second;

		{
			sGRankInfo.wClubRanking = m_vecWoeGuild[i].wClubRanking;
			sGRankInfo.nIndex = m_vecWoeGuild[i].nIndex;

			if (m_vecWoeGuild[i].nIndex < RANKING_NUM)
			{
				SWOE_GUILD_RANK sCWRank = sGRankInfo;
				NetMsg.ADDCLUB(sCWRank);
			}
		}
	}

	if (NetMsg.wRankNum > 0)
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMap.dwID, &NetMsg);


	GLMSG::SNET_WOE_GUILD_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapWoeScore.begin();
	end = m_mapWoeScore.end();

	for (; pos != end; pos++)
	{
		const SWOE_GUILD_RANK_INFO& sGRankInfo = pos->second;
		NetMsgMy.sMyWoeGuildRank.wClubRanking = sGRankInfo.dwClubID;
		NetMsgMy.sMyWoeGuildRank.wClubRanking = sGRankInfo.dwClubID;
		NetMsgMy.sMyWoeGuildRank.wClubRanking = sGRankInfo.wClubRanking;
		NetMsgMy.sMyWoeGuildRank.wKillNum = sGRankInfo.wKillNum;
		NetMsgMy.sMyWoeGuildRank.wDeathNum = sGRankInfo.wDeathNum;
		NetMsgMy.sMyWoeGuildRank.m_dwMarkVER = sGRankInfo.m_dwMarkVER;
		StringCchCopy(NetMsgMy.sMyWoeGuildRank.szClubName, CLUB_NAME + 1, sGRankInfo.szClubName);
		StringCchCopy(NetMsgMy.sMyWoeGuildRank.szLeaderName, CHAR_SZNAME + 1, sGRankInfo.szLeaderName);

		GLGaeaServer::GetInstance().SENDTOCLUBCLIENT_ONMAP(m_sMap.dwID, sGRankInfo.dwClubID, &NetMsgMy);
	}

}
SWOE_GUILD_RANK_INFO* GLPVPWoeField::GetWoeGuildRankInfo(DWORD dwClubID)
{
	WOE_GUILD_RANK_INFO_MAP_ITER pos = m_mapWoeScore.find(dwClubID);
	if (pos == m_mapWoeScore.end())
	{
		return NULL;
	}

	return &(*pos).second;;
}

void GLPVPWoeField::UpdatePlayerScore()
{
	WOE_PLAYER_RANK_INFO_VEC m_vecWoePlayer;
	m_vecWoePlayer.reserve(m_mapWoePlayerScore.size());

	WOE_PLAYER_RANK_INFO_MAP_ITER pos = m_mapWoePlayerScore.begin();
	WOE_PLAYER_RANK_INFO_MAP_ITER end = m_mapWoePlayerScore.end();

	for (; pos != end; pos++)
	{
		const SWOE_PLAYER_RANK_INFO& sPlayerRankInfo = pos->second;
		m_vecWoePlayer.push_back(sPlayerRankInfo);
	}

	std::sort(m_vecWoePlayer.begin(), m_vecWoePlayer.end());

	int nSize = m_vecWoePlayer.size();

	for (int i = 0; i < nSize; ++i)
	{
		m_vecWoePlayer[i].wRanking = i + 1;
		m_vecWoePlayer[i].nIndex = i;
	}

	int nRanking;

	for (int i = nSize - 1; i > 0; --i)
	{
		if (m_vecWoePlayer[i] == m_vecWoePlayer[i - 1])
		{
			m_vecWoePlayer[i - 1].wRanking = m_vecWoePlayer[i].wRanking;
		}
	}


	GLMSG::SNET_WOE_PLAYER_RANKING_UPDATE	NetMsg;

	for (int i = 0; i < m_vecWoePlayer.size(); ++i)
	{
		WOE_PLAYER_RANK_INFO_MAP_ITER iter = m_mapWoePlayerScore.find(m_vecWoePlayer[i].dwCharID);
		if (iter == m_mapWoePlayerScore.end())	continue;

		SWOE_PLAYER_RANK_INFO& sPlayerRankInfo = iter->second;
		{
			sPlayerRankInfo.wRanking = m_vecWoePlayer[i].wRanking;
			sPlayerRankInfo.nIndex = m_vecWoePlayer[i].nIndex;

			if (m_vecWoePlayer[i].nIndex < RANKING_NUM)
			{
				SWOE_PLAYER_RANK sTwPlayerRank = sPlayerRankInfo;
				NetMsg.ADDPLAYER(sTwPlayerRank);
			}
		}
	}

	if (NetMsg.wRankNum > 0)
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMap.dwID, &NetMsg);


	GLMSG::SNET_WOE_PLAYER_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapWoePlayerScore.begin();
	end = m_mapWoePlayerScore.end();

	for (; pos != end; pos++)
	{
		const SWOE_PLAYER_RANK_INFO& sPlayerRankInfo = pos->second;
		NetMsgMy.sMySelfPlayerRank.wRanking = sPlayerRankInfo.wRanking;
		NetMsgMy.sMySelfPlayerRank.wKillNum = sPlayerRankInfo.wKillNum;
		NetMsgMy.sMySelfPlayerRank.wDeathNum = sPlayerRankInfo.wDeathNum;
		NetMsgMy.sMySelfPlayerRank.wSchool = sPlayerRankInfo.wSchool;
		NetMsgMy.sMySelfPlayerRank.wClass = sPlayerRankInfo.wClass;
		NetMsgMy.sMySelfPlayerRank.dwHitCount = sPlayerRankInfo.dwCountHit;
		StringCchCopy(NetMsgMy.sMySelfPlayerRank.szName, CHAR_NAME + 1, sPlayerRankInfo.szName);

		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMap.dwID, &NetMsgMy);
	}

}
SWOE_PLAYER_RANK_INFO* GLPVPWoeField::GetWoePlayerRankInfo(DWORD dwCharID)
{
	WOE_PLAYER_RANK_INFO_MAP_ITER pos = m_mapWoePlayerScore.find(dwCharID);
	if (pos == m_mapWoePlayerScore.end())
	{
		return NULL;
	}

	return &(*pos).second;;
}
void GLPVPWoeField::AddWoeResuScore(DWORD dwResuScore)
{
	if (!IsBattle()) return;

	SWOE_RESU_PLAYER_INFO* pResuRankInfo = GetWoeResuRankInfo(dwResuScore);
	if (pResuRankInfo)
	{
		pResuRankInfo->wResuNum++;
	}
	else
	{
		SWOE_RESU_PLAYER_INFO sWoeRankInfo;
		sWoeRankInfo.dwCharID = dwResuScore;
		sWoeRankInfo.wResuNum++;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID(dwResuScore);
		if (pChar)
		{
			EMCHARINDEX emIndex = CharClassToIndex(pChar->m_emClass);

			StringCchCopy(sWoeRankInfo.szName, CHAR_NAME + 1, pChar->m_szName);
			sWoeRankInfo.szName[CHAR_NAME] = '\0';
			sWoeRankInfo.wSchool = pChar->m_wSchool;
			sWoeRankInfo.wClass = emIndex;
		}

		m_mapWoeResuScore[sWoeRankInfo.dwCharID] = sWoeRankInfo;
	}
}
void GLPVPWoeField::UpdateResuScore()
{
	WOE_RESU_PLAYER_INFO_VEC m_vecWoeResu;
	m_vecWoeResu.reserve(m_mapWoeResuScore.size());

	WOE_RESU_PLAYER_INFO_MAP_ITER pos = m_mapWoeResuScore.begin();
	WOE_RESU_PLAYER_INFO_MAP_ITER end = m_mapWoeResuScore.end();

	for (; pos != end; pos++)
	{
		const SWOE_RESU_PLAYER_INFO& sPlayerRankInfo = pos->second;
		m_vecWoeResu.push_back(sPlayerRankInfo);
	}

	std::sort(m_vecWoeResu.begin(), m_vecWoeResu.end());

	int nSize = m_vecWoeResu.size();

	for (int i = 0; i < nSize; ++i)
	{
		m_vecWoeResu[i].wRanking = i + 1;
		m_vecWoeResu[i].nIndex = i;
	}

	int nRanking;

	for (int i = nSize - 1; i > 0; --i)
	{
		if (m_vecWoeResu[i] == m_vecWoeResu[i - 1])
		{
			m_vecWoeResu[i - 1].wRanking = m_vecWoeResu[i].wRanking;
		}
	}


	GLMSG::SNET_WOE_RESU_RANKING_UPDATE	NetMsg;

	for (int i = 0; i < m_vecWoeResu.size(); ++i)
	{
		WOE_RESU_PLAYER_INFO_MAP_ITER iter = m_mapWoeResuScore.find(m_vecWoeResu[i].dwCharID);
		if (iter == m_mapWoeResuScore.end())	continue;

		SWOE_RESU_PLAYER_INFO& sPlayerRankInfo = iter->second;

		{
			sPlayerRankInfo.wRanking = m_vecWoeResu[i].wRanking;
			sPlayerRankInfo.nIndex = m_vecWoeResu[i].nIndex;

			if (m_vecWoeResu[i].nIndex < RANKING_NUM)
			{
				SWOE_RANK_RESU sTwPlayerRank = sPlayerRankInfo;
				NetMsg.ADDPLAYER(sTwPlayerRank);
			}
		}
	}

	if (NetMsg.wRankNum > 0)
		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMap.dwID, &NetMsg);


	GLMSG::SNET_WOE_RESU_MYRANK_UPDATE	NetMsgMy;

	pos = m_mapWoeResuScore.begin();
	end = m_mapWoeResuScore.end();

	for (; pos != end; pos++)
	{
		const SWOE_RESU_PLAYER_INFO& sPlayerRankInfo = pos->second;
		NetMsgMy.sMySelfResuRank.wRanking = sPlayerRankInfo.wRanking;
		NetMsgMy.sMySelfResuRank.wResuNum = sPlayerRankInfo.wResuNum;
		NetMsgMy.sMySelfResuRank.wClass = sPlayerRankInfo.wClass;
		NetMsgMy.sMySelfResuRank.wSchool = sPlayerRankInfo.wSchool;
		StringCchCopy(NetMsgMy.sMySelfResuRank.szName, CHAR_NAME + 1, sPlayerRankInfo.szName);

		GLGaeaServer::GetInstance().SENDTOCLIENT_ONMAP(m_sMap.dwID, &NetMsgMy);
	}
}
SWOE_RESU_PLAYER_INFO* GLPVPWoeField::GetWoeResuRankInfo(DWORD dwCharID)
{
	WOE_RESU_PLAYER_INFO_MAP_ITER pos = m_mapWoeResuScore.find(dwCharID);
	if (pos == m_mapWoeResuScore.end())
	{
		return NULL;
	}

	return &(*pos).second;;
}

void GLPVPWoeField::HitScore(DWORD dwCharID, WORD wHitScore)
{
	UpdateWoePlayerInfo(dwCharID, true, false, false);  // Only update hit count
}

void GLPVPWoeField::AddWoePlayerScore(DWORD dwKillScore, DWORD dwDeathScore)
{
	if (!IsBattle()) return;

	UpdateWoePlayerInfo(dwKillScore, false, true, false);

	UpdateWoePlayerInfo(dwDeathScore, false, false, true);
}

void GLPVPWoeField::UpdateWoePlayerInfo(DWORD dwCharID, bool isHit, bool isKill, bool isDeath)
{
	SWOE_PLAYER_RANK_INFO* pPlayerInfo = GetWoePlayerRankInfo(dwCharID);
	if (pPlayerInfo)
	{
		// Update based on the flags
		if (isHit)
			pPlayerInfo->dwCountHit++;
		if (isKill)
			pPlayerInfo->wKillNum++;
		if (isDeath)
			pPlayerInfo->wDeathNum++;
	}
	else
	{
		// New player, initialize their rank info
		SWOE_PLAYER_RANK_INFO sWoeRankInfo;
		sWoeRankInfo.dwCharID = dwCharID;
		sWoeRankInfo.dwCountHit = 0;
		sWoeRankInfo.wKillNum = 0;
		sWoeRankInfo.wDeathNum = 0;

		PGLCHAR pChar = GLGaeaServer::GetInstance().GetCharID(dwCharID);
		if (pChar)
		{
			EMCHARINDEX emIndex = CharClassToIndex(pChar->m_emClass);

			StringCchCopy(sWoeRankInfo.szName, CHAR_NAME + 1, pChar->m_szName);
			sWoeRankInfo.szName[CHAR_NAME] = '\0';
			sWoeRankInfo.wSchool = pChar->m_wSchool;
			sWoeRankInfo.wClass = emIndex;
		}

		// Update the initialized player info based on the flags
		if (isHit)
			sWoeRankInfo.dwCountHit = 1;
		if (isKill)
			sWoeRankInfo.wKillNum = 1;
		if (isDeath)
			sWoeRankInfo.wDeathNum = 1;

		// Add to the map
		m_mapWoePlayerScore[sWoeRankInfo.dwCharID] = sWoeRankInfo;
	}
}

