#include "StdAfx.h"
#include "./GLPVPCaptureTheFlagAgent.h"
#include "./GLAgentServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAPTURE_THE_FLAG_REGISTER_DATA* GLPVPCaptureTheFlagAgent::PlayerDataGet( DWORD dwCharID )
{
	if ( dwCharID == CAPTURE_THE_FLAG_PLAYER_NULL ) return NULL;

	{
		CAPTURE_THE_FLAG_REGISTER_DATA_MAP_ITER iterdata = m_mapPlayerDataA.find( dwCharID );
		if( iterdata != m_mapPlayerDataA.end() ){
			return &(*iterdata).second;
		}
	}

	{
		CAPTURE_THE_FLAG_REGISTER_DATA_MAP_ITER iterdata = m_mapPlayerDataB.find( dwCharID );
		if( iterdata != m_mapPlayerDataB.end() ){
			return &(*iterdata).second;
		}
	}

	return NULL;
};

BOOL GLPVPCaptureTheFlagAgent::IsPlayerFull( CAPTURE_THE_FLAG_REGISTER_DATA_MAP& mapData  )
{
	//unlimited team members
	if ( m_wPlayerLimit == 0 )	return FALSE;

	if ( mapData.size() >= m_wPlayerLimit )
		return TRUE;

	return FALSE;
}

BOOL GLPVPCaptureTheFlagAgent::InsertRegister( CAPTURE_THE_FLAG_REGISTER_DATA_MAP& mapData, DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool, WORD wTeam )
{
	if ( !szName )										return FALSE;
	if ( dwCharID == CAPTURE_THE_FLAG_PLAYER_NULL )		return FALSE;
	if ( wClass >= GLCI_NUM_8CLASS )					return FALSE;
	if ( wSchool >= CAPTURE_THE_FLAG_INFO_SCHOOL_SIZE )	return FALSE;
	if ( wTeam >= CAPTURE_THE_FLAG_TEAM_SIZE )			return FALSE;

	CAPTURE_THE_FLAG_REGISTER_DATA sdata;
	sdata.dwInfoCharID = dwCharID;
	sdata.wInfoClass = wClass;
	sdata.wInfoSchool = wSchool;
	sdata.wInfoTeam = wTeam;
	StringCchCopy( sdata.szInfoCharName, CAPTURE_THE_FLAG_CHARNAME_STRING_SIZE, szName );
	mapData.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );

	return TRUE;
}

BOOL GLPVPCaptureTheFlagAgent::DeleteRegister( DWORD dwCharID, CAPTURE_THE_FLAG_REGISTER_DATA_MAP& mapData  )
{
	if ( dwCharID == CAPTURE_THE_FLAG_PLAYER_NULL )	return FALSE;

	CAPTURE_THE_FLAG_REGISTER_DATA_MAP_ITER iterdata = mapData.find( dwCharID );
	if( iterdata != mapData.end() ){
		mapData.erase( iterdata );
		return TRUE;
	}

	return FALSE;
}

void GLPVPCaptureTheFlagAgent::RequestPlayerRegister( DWORD dwCharID, WORD wLevel, DWORD dwContri, BOOL bRegister, WORD wTeam, std::string strHash )
{	
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_CAPTURE_THE_FLAG_A2C_REGISTER_FB netmsgfb;
	netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_A] = (WORD)m_mapPlayerDataA.size();
	netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_B] = (WORD)m_mapPlayerDataB.size();
	netmsgfb.wTeam = wTeam;

	//invalid charid
	if ( pchar->m_dwCharID != dwCharID ){
		netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_INVALID_CHARID;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//registration not running
	if ( m_emState != CAPTURE_THE_FLAG_STATE_REGISTER ){
		netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//register only available in channel 0 
	if ( pchar->m_nChannel != 0 ){
		netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_INVALID_CHANNEL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//player level too low
	if ( wLevel < m_wLevelReq ){
		netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_LEVEL_LOW;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//contri low
	if ( dwContri < m_dwContributionReq ){
		netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_CONTRI_LOW;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( bRegister )
	{
		/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
		if ( m_bHashCheck )
		{
			//hash check enabled, hash should not be empty
			if ( strHash.empty() )
			{
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_DEVICE_INFO_ERROR;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}

			//hash already used
			if ( PCHashCheck( strHash ) )
			{
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_DEVICE_REGISTERED;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}
		}

		//already registered
		CAPTURE_THE_FLAG_REGISTER_DATA*	pPlayerData = PlayerDataGet( dwCharID );
		if ( pPlayerData )
		{
			if ( pPlayerData->wInfoTeam != wTeam )
			{
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_OTHER_TEAM;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}else{
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_ALREADY_REG;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}
		}

		if ( wTeam == CAPTURE_THE_FLAG_TEAM_A )
		{
			if ( m_wBalancer != 0 && m_mapPlayerDataA.size() > m_mapPlayerDataB.size() )
			{
				if ( std::abs( (int)m_mapPlayerDataA.size() - (int)m_mapPlayerDataB.size() ) >= m_wBalancer )
				{
					netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_TEAM_A_BALANCE;
					GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
					return;
				}
			}

			if ( IsPlayerFull( m_mapPlayerDataA ) ){
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_TEAM_A_FULL;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}else{
				//insert player data
				BOOL binsert = InsertRegister( m_mapPlayerDataA, pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool, wTeam );
				if ( binsert ){
					netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_REGISTERED_A;
					netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_A] = (WORD)m_mapPlayerDataA.size();
					netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_B] = (WORD)m_mapPlayerDataB.size();
					GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

					/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
					if ( m_bHashCheck )
						PCHashInsert( strHash, pchar->m_dwCharID );

					return;
				}
			}
		}
		else if ( wTeam == CAPTURE_THE_FLAG_TEAM_B )
		{
			if ( m_wBalancer != 0 && m_mapPlayerDataB.size() > m_mapPlayerDataA.size() )
			{
				if ( std::abs( (int)m_mapPlayerDataB.size() - (int)m_mapPlayerDataA.size() ) >= m_wBalancer )
				{
					netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_TEAM_B_BALANCE;
					GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
					return;
				}
			}

			if ( IsPlayerFull( m_mapPlayerDataB ) ){
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_TEAM_B_FULL;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}else{
				//insert player data
				BOOL binsert = InsertRegister( m_mapPlayerDataB, pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool, wTeam );
				if ( binsert ){
					netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_REGISTERED_B;
					netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_A] = (WORD)m_mapPlayerDataA.size();
					netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_B] = (WORD)m_mapPlayerDataB.size();
					GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

					/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
					if ( m_bHashCheck )
						PCHashInsert( strHash, pchar->m_dwCharID );

					return;
				}
			}
		}
		else
		{
			netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_INVALID_TEAM;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		CAPTURE_THE_FLAG_REGISTER_DATA* pdataregchk = PlayerDataGet( dwCharID );
		if ( !pdataregchk ){
			CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "RequestPlayerRegister failed to register [%d]'%s'", 
				pchar->m_dwCharID, pchar->m_szName );
		}
	}
	else
	{
		CAPTURE_THE_FLAG_REGISTER_DATA*	pPlayerData = PlayerDataGet( dwCharID );
		if ( pPlayerData )
		{
			if ( pPlayerData->wInfoTeam != wTeam )
			{
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_OTHER_TEAM;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}
		}

		if ( wTeam == CAPTURE_THE_FLAG_TEAM_A )
		{
			//if exist in registered, remove
			BOOL bdelete = DeleteRegister( pchar->m_dwCharID, m_mapPlayerDataA );
			if ( bdelete ){
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_CANCEL_REG;
				netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_A] = (WORD)m_mapPlayerDataA.size();
				netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_B] = (WORD)m_mapPlayerDataB.size();
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

				/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
				if ( m_bHashCheck )
					PCHashDelete( pchar->m_dwCharID );

				return;
			}

			//fail to delete
			if ( !bdelete ){
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_CANCEL_FAIL;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}
		}
		else if ( wTeam == CAPTURE_THE_FLAG_TEAM_B )
		{
			//if exist in registered, remove
			BOOL bdelete = DeleteRegister( pchar->m_dwCharID, m_mapPlayerDataB );
			if ( bdelete ){
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_CANCEL_REG;
				netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_A] = (WORD)m_mapPlayerDataA.size();
				netmsgfb.wPlayerNum[CAPTURE_THE_FLAG_TEAM_B] = (WORD)m_mapPlayerDataB.size();
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

				/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
				if ( m_bHashCheck )
					PCHashDelete( pchar->m_dwCharID );

				return;
			}

			//fail to delete
			if ( !bdelete ){
				netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_CANCEL_FAIL;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
				return;
			}
		}
		else
		{
			netmsgfb.emFB = CAPTURE_THE_FLAG_REGISTER_FB_INVALID_TEAM;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		CAPTURE_THE_FLAG_REGISTER_DATA* pdataregchk = PlayerDataGet( dwCharID );
		if ( pdataregchk ){
			CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "RequestPlayerRegister failed to unregister [%d]'%s'", 
				pchar->m_dwCharID, pchar->m_szName );
		}
	}
}

void GLPVPCaptureTheFlagAgent::RequestPlayerCancelRegister( DWORD dwCharID )
{	
	if ( dwCharID == CAPTURE_THE_FLAG_PLAYER_NULL )	return;
	if ( m_emState != CAPTURE_THE_FLAG_STATE_REGISTER )	return;

	CAPTURE_THE_FLAG_REGISTER_DATA* pdatareg = PlayerDataGet( dwCharID );
	if ( pdatareg )
	{
		WORD wteam = pdatareg->wInfoTeam;

		if ( wteam == CAPTURE_THE_FLAG_TEAM_A )
		{
			DeleteRegister( dwCharID, m_mapPlayerDataA );

			/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
			if ( m_bHashCheck )
				PCHashDelete( dwCharID );
		}
		else if ( wteam == CAPTURE_THE_FLAG_TEAM_B )
		{
			DeleteRegister( dwCharID, m_mapPlayerDataB );

			/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
			if ( m_bHashCheck )
				PCHashDelete( dwCharID );
		}
	}

	CAPTURE_THE_FLAG_REGISTER_DATA* pdataregchk = PlayerDataGet( dwCharID );
	if ( pdataregchk ){
		CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "RequestPlayerCancelRegister failed to delete registered [%d]'%s'", 
			pdataregchk->dwInfoCharID, pdataregchk->szInfoCharName );
	}
}

void GLPVPCaptureTheFlagAgent::SendPlayerData()
{
	CAPTURE_THE_FLAG_REGISTER_DATA_VEC vecdata;
	vecdata.reserve( m_mapPlayerDataA.size() + m_mapPlayerDataB.size() );

	for( CAPTURE_THE_FLAG_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataA.begin(); iterplayer != m_mapPlayerDataA.end(); ++ iterplayer ){
		const CAPTURE_THE_FLAG_REGISTER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	for( CAPTURE_THE_FLAG_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataB.begin(); iterplayer != m_mapPlayerDataB.end(); ++ iterplayer ){
		const CAPTURE_THE_FLAG_REGISTER_DATA& splayerdata = (*iterplayer).second;
		vecdata.push_back( splayerdata );
	}

	WORD wNum = 0;
	GLMSG::SNETPC_CAPTURE_THE_FLAG_A2F_PLAYER_DATA	net_msg_data;
	for ( size_t i=0; i<vecdata.size(); ++i )
	{
		const CAPTURE_THE_FLAG_REGISTER_DATA& splayer_data = vecdata[i];
		net_msg_data.ADDRANK( splayer_data );
		if ( net_msg_data.wRankNum == CAPTURE_THE_FLAG_PACKET_PLAYER_DATA_NUM )
		{
			GLAgentServer::GetInstance().SENDTOCHANNEL( &net_msg_data, 0 );
			wNum += net_msg_data.wRankNum;
			net_msg_data.Reset();
		}
	}

	if ( net_msg_data.wRankNum != 0 )
	{
		GLAgentServer::GetInstance().SENDTOCHANNEL( &net_msg_data, 0 );
		wNum += net_msg_data.wRankNum;
		net_msg_data.Reset();
	}

	CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "GLPVPCaptureTheFlagAgent Send PlayerData num: %u", wNum );
	GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "GLPVPCaptureTheFlagAgent Send PlayerData num: %u", wNum );
}

/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
BOOL GLPVPCaptureTheFlagAgent::PCHashCheck( std::string strHash )
{
	if ( strHash.empty() )	return FALSE;

	CAPTURE_THE_FLAG_PLAYER_HASH_DATA_MAP_ITER it = m_mapPlayerHash.find( strHash );
	if ( it != m_mapPlayerHash.end() )
		return TRUE;

	return FALSE;
}

/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
void GLPVPCaptureTheFlagAgent::PCHashInsert( std::string strHash, DWORD dwCharID )
{
	CAPTURE_THE_FLAG_PLAYER_HASH_DATA sData;
	sData.dwCharID = dwCharID;
	sData.strHash = strHash;

	m_mapPlayerHash.insert( std::make_pair( sData.strHash, sData ) );
}

/* PVPCaptureTheFlag hash check, Juver, 2021/07/18 */
void GLPVPCaptureTheFlagAgent::PCHashDelete( DWORD dwCharID )
{
	CAPTURE_THE_FLAG_PLAYER_HASH_DATA_MAP_ITER it_b = m_mapPlayerHash.begin();
	CAPTURE_THE_FLAG_PLAYER_HASH_DATA_MAP_ITER it_e = m_mapPlayerHash.end();

	for( ; it_b != it_e; )
	{
		if ( it_b->second.dwCharID == dwCharID )
			m_mapPlayerHash.erase( it_b++ );
		else
			++it_b;
	}
}