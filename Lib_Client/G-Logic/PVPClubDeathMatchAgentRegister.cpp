#include "stdafx.h"
#include "./PVPClubDeathMatchAgent.h"

#include "./GLAgentServer.h"

#include "../../Lib_Engine/Core/NSRLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PVP_CLUB_DEATH_MATCH_CLUB_DATA* PVPClubDeathMatchAgent::GetClubData( DWORD dwClubID )
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it = m_mapClubData.find( dwClubID );
	if ( it != m_mapClubData.end() )
	{
		return &(*it).second;
	}

	return NULL;
}

PVP_CLUB_DEATH_MATCH_CLUB_DATA* PVPClubDeathMatchAgent::CreateClubData( DWORD dwClubID, const TCHAR* szClubName, const TCHAR* szMasterName, DWORD dwClubMarkVer )
{
	if ( dwClubID == PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL ) return NULL;
	if ( !szClubName )	return NULL;
	if ( !szMasterName )	return NULL;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA sData;
	sData.dwClubID = dwClubID;
	StringCchCopy( sData.szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, szClubName );
	StringCchCopy( sData.szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, szMasterName );
	sData.dwCLUB_MARK_VER = dwClubMarkVer;

	m_mapClubData.insert( std::make_pair( sData.dwClubID, sData ) );

	return GetClubData( dwClubID );
}

void PVPClubDeathMatchAgent::RequestPlayerRegister( DWORD dwCharID, WORD wLevel, BOOL bRegister, std::string strHash )
{
	PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pChar )	return;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB netmsgfb;

	//invalid charid
	if ( pChar->m_dwCharID != dwCharID )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHARID;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}

	//registration not running
	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}

	//register only available in channel 0 
	if ( pChar->m_nChannel != 0 )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHANNEL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}

	//player level too low
	if ( wLevel < m_wLevelReq )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_LEVEL_LOW;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( bRegister )
	{
		GLClubMan &cClubMan = GLAgentServer::GetInstance().GetClubMan();
		GLCLUB *pCLUB = cClubMan.GetClub ( pChar->m_dwGuild );
		if ( !pCLUB )
		{
			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CLUB;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
			return;
		}

		if ( pCLUB->m_dwRank < m_wClubLevelLimit )
		{
			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_CLUB_LEVEL_LOW;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
			return;
		}

		if ( m_bClubPlayerReqCDMRights )
		{
			if ( !pCLUB->IsMemberFlgCDM(dwCharID) )
			{
				netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_REQUIRE_RIGHTS;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
				return;
			}
		}

		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pCLUB->m_dwID );
		if ( pClubData )
		{
			PlayerRegister( pClubData, pChar->m_dwCharID, strHash );
		}
		else
		{
			PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubDataNew = CreateClubData( pCLUB->m_dwID, pCLUB->m_szName, pCLUB->m_szMasterName, pCLUB->m_dwMarkVER );
			if ( !pClubDataNew )
			{
				RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::PlayerRegister failed to create club data char:[%u]%s club:[%u]%s master:%s", 
					pChar->m_dwCharID, pChar->m_szName, pCLUB->m_dwID, pCLUB->m_szName, pCLUB->m_szMasterName );
			}
			else
			{
				PlayerRegister( pClubDataNew, pChar->m_dwCharID, strHash );
			}
		}
	}
	else
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pChar->m_dwGuild );
		if ( pClubData )
		{
			CancelRegister( pClubData, pChar->m_dwCharID );
		}
		else
		{
			RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::PlayerRegister failed to calcel register char:[%u]%s club:[%u]", 
				pChar->m_dwCharID, pChar->m_szName, pChar->m_dwGuild );

			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_FAIL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		}
	}

}

void PVPClubDeathMatchAgent::RequestPlayerCancelRegister( DWORD dwCharID, DWORD dwGuildID )
{	
	if ( dwCharID == PVP_CLUB_DEATH_MATCH_CHAR_ID_NULL )			return;
	if ( dwGuildID == PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )			return;
	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )	return;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( dwGuildID );
	if ( pClubData )
	{
		PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pDataReg = pClubData->PlayerDataGet( dwCharID );
		if ( pDataReg )
		{
			pClubData->DeleteRegister( dwCharID );

			/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
			if ( m_bHashCheck )
				PCHashDelete( dwCharID );
		}
	}
}

void PVPClubDeathMatchAgent::PlayerRegister( PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData, DWORD dwCharID, std::string strHash )
{
	if ( !pClubData )	return;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pChar )		return;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB netmsgfb;

	/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
	if ( m_bHashCheck )
	{
		//hash check enabled, hash should not be empty
		if ( strHash.empty() )
		{
			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_DEVICE_INFO_ERROR;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
			return;
		}

		//hash already used
		if ( PCHashCheck( strHash ) )
		{
			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_DEVICE_REGISTERED;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
			return;
		}
	}

	//player already registered
	if ( pClubData->isPlayerReg( pChar->m_dwCharID ) )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_ALREADY_REG;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( pClubData->isPlayerFull( m_wClubPlayerMax ) )
	{
		//registration full
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_FULL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}
	else
	{
		//insert player data
		BOOL binsert = pClubData->InsertRegister( pChar->m_dwCharID, pChar->m_szName, CharClassToIndex( pChar->m_emClass ), pChar->m_wSchool );
		if ( binsert )
		{
			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_REGISTERED;
			netmsgfb.wClubParticipants = pClubData->GetPlayerNum();
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );

			/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
			if ( m_bHashCheck )
				PCHashInsert( strHash, pChar->m_dwCharID );

			return;
		}
	}

	netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_FAILED;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
}

void PVPClubDeathMatchAgent::CancelRegister( PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData, DWORD dwCharID )
{
	if ( !pClubData )	return;

	PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pChar )		return;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB netmsgfb;

	//if exist in registered, remove
	BOOL bdelete = pClubData->DeleteRegister( pChar->m_dwCharID );
	if ( bdelete )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_REG;
		netmsgfb.wClubParticipants = pClubData->GetPlayerNum();
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );

		/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
		if ( m_bHashCheck )
			PCHashDelete( pChar->m_dwCharID );

		return;
	}
	else
	{
		//fail to delete
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_FAIL;
		netmsgfb.wClubParticipants = pClubData->GetPlayerNum();
		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );
		return;
	}
}

//remove all club data
void PVPClubDeathMatchAgent::ProcessClubCleanUp()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;

		//call cleanup
		sData.CleanUp();
	}

	//erase all data
	m_mapClubData.clear();
}

//qualify if club have enough participants and delete if not
void PVPClubDeathMatchAgent::ProcessClubQualify()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

	std::vector<DWORD> vecDeleteID;

	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;
		
		BOOL bQualify = sData.isPlayerEnough( m_wClubPlayerMin, m_wClubPlayerMax );
		if ( !bQualify )
		{
			//inform club members that their participants are not enough and battle entry will be removed
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it2_b = sData.mapPlayerData.begin();
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it2_e = sData.mapPlayerData.end();

			GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB netmsgfb;
			netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REGISTER_FB_CANCEL_NOT_ENOUGH_MEMBERS;
			netmsgfb.wClubParticipants = sData.GetPlayerNum();

			for ( ; it2_b != it2_e; ++it2_b )
			{
				PVP_CLUB_DEATH_MATCH_PLAYER_DATA& sPlayerData = (*it2_b).second;
				PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( sPlayerData.dwCharID );
				if ( !pChar )	continue;

				GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgfb );

				/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
				if ( m_bHashCheck )
					PCHashDelete( pChar->m_dwCharID );
			}

			vecDeleteID.push_back( sData.dwClubID );
		}
	}

	//delete disqualified entry 
	UINT nSize = vecDeleteID.size();
	for ( UINT i=0; i<nSize; ++i )
	{
		DWORD dwClubID = vecDeleteID[i];

		PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it = m_mapClubData.find( dwClubID );
		if ( it != m_mapClubData.end() )
		{
			m_mapClubData.erase( it );
		}
	}

}

/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
BOOL PVPClubDeathMatchAgent::PCHashCheck( std::string strHash )
{
	if ( strHash.empty() )	return FALSE;

	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP_ITER it = m_mapPlayerHash.find( strHash );
	if ( it != m_mapPlayerHash.end() )
		return TRUE;

	return FALSE;
}

/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
void PVPClubDeathMatchAgent::PCHashInsert( std::string strHash, DWORD dwCharID )
{
	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA sData;
	sData.dwCharID = dwCharID;
	sData.strHash = strHash;

	m_mapPlayerHash.insert( std::make_pair( sData.strHash, sData ) );
}

/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
void PVPClubDeathMatchAgent::PCHashDelete( DWORD dwCharID )
{
	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP_ITER it_b = m_mapPlayerHash.begin();
	PVP_CLUB_DEATH_MATCH_PLAYER_HASH_DATA_MAP_ITER it_e = m_mapPlayerHash.end();

	for( ; it_b != it_e; )
	{
		if ( it_b->second.dwCharID == dwCharID )
			m_mapPlayerHash.erase( it_b++ );
		else
			++it_b;
	}
}