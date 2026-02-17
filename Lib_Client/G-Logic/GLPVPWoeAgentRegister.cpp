#include "stdafx.h"
#include "./GLPVPWoeAgent.h"
#include "./GLAgentServer.h"
#include "./GLGaeaClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WOE_REGISTER_DATA* GLPVPWoeAgent::PlayerDataGet( DWORD dwCharID )
{
	if ( dwCharID == WOE_PLAYER_NULL ) return NULL;
	{
		WOE_REGISTER_DATA_MAP_ITER iterdata = m_mapGuildData.find( dwCharID );
		if( iterdata != m_mapGuildData.end() ){
			return &(*iterdata).second;
		}
	}
	return NULL;
};

BOOL GLPVPWoeAgent::IsPlayerFull( WOE_REGISTER_DATA_MAP& mapData  )
{
	if ( m_wPlayerLimit == 0 )	return FALSE;

	if ( mapData.size() >= m_wPlayerLimit )
		return TRUE;

	return FALSE;
}

BOOL GLPVPWoeAgent::IsPlayerReg( WOE_REGISTER_DATA_MAP& mapData, DWORD dwCharID )
{
	if ( dwCharID == WOE_PLAYER_NULL )	return FALSE;

	WOE_REGISTER_DATA_MAP_ITER iterdata = mapData.find( dwCharID );
	if( iterdata != mapData.end() ){
		return TRUE;
	}

	return FALSE;
}
BOOL GLPVPWoeAgent::InsertRegister( WOE_REGISTER_DATA_MAP& mapData, DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool )
{
	if ( !szName )							return FALSE;
	if ( dwCharID == WOE_PLAYER_NULL )	return FALSE;
	if ( wClass >= GLCI_NUM_8CLASS )		return FALSE;
	
	WOE_REGISTER_DATA sdata;
	sdata.dwInfoCharID = dwCharID;
	sdata.wInfoClass = wClass;
	sdata.wInfoSchool = wSchool;
	StringCchCopy( sdata.szInfoCharName, WOE_CHARNAME_STRING_SIZE, szName );
	mapData.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );

	return TRUE;
}

BOOL GLPVPWoeAgent::DeleteRegister( DWORD dwCharID, WOE_REGISTER_DATA_MAP& mapData  )
{
	if ( dwCharID == WOE_PLAYER_NULL )	return FALSE;

	WOE_REGISTER_DATA_MAP_ITER iterdata = mapData.find( dwCharID );
	if( iterdata != mapData.end() ){
		mapData.erase( iterdata );
		return TRUE;
	}

	return FALSE;
}
void GLPVPWoeAgent::RequestPlayerRegister( DWORD dwCharID, WORD wLevel, BOOL bRegister )
{	
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_WOE_A2C_REGISTER_FB netmsgfb;

	if ( pchar->m_dwCharID != dwCharID ){
		netmsgfb.emFB = WOE_REGISTER_FB_INVALID_CHARID;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	/*if ( m_emState != WOE_STATE_REGISTER ){
		netmsgfb.emFB = WOE_REGISTER_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}*/

	if ( pchar->m_nChannel != 0 ){
		netmsgfb.emFB = WOE_REGISTER_FB_INVALID_CHANNEL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( wLevel < m_wLevelReq ){
		netmsgfb.emFB = WOE_REGISTER_FB_LEVEL_LOW;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( bRegister )
	{
		if ( pchar->m_dwGuild)
			PlayerRegister( pchar->m_dwCharID, m_mapGuildData );
		else
		{
			netmsgfb.emFB = WOE_REGISTER_FB_INVALID_SCHOOL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

	}
	else
	{
		if ( pchar->m_dwGuild )
			CancelRegister( pchar->m_dwCharID, m_mapGuildData );
		else
		{
			netmsgfb.emFB = WOE_REGISTER_FB_INVALID_SCHOOL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
	}
}

void GLPVPWoeAgent::RequestPlayerCancelRegister( DWORD dwCharID )
{	
	if ( dwCharID == WOE_PLAYER_NULL )	return;
	if ( m_emState != WOE_STATE_REGISTER )	return;

	return;
}

void GLPVPWoeAgent::PlayerRegister( DWORD dwCharID, WOE_REGISTER_DATA_MAP& mapData)
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_WOE_A2C_REGISTER_FB netmsgfb;

	GLPetClient* pPetClient = GLGaeaClient::GetInstance().GetPetClient();
	if ( pPetClient->IsVALID())
	{
		return;
	}

	if ( IsPlayerFull( mapData ) ){
		return;
	}
	else
	{

		BOOL binsert = InsertRegister( mapData, pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool );
		if ( binsert ){
			netmsgfb.emFB = WOE_REGISTER_FB_REGISTERED;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
	}

	netmsgfb.emFB = WOE_REGISTER_FB_FAILED;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
}

void GLPVPWoeAgent::CancelRegister( DWORD dwCharID, WOE_REGISTER_DATA_MAP& mapData)
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_WOE_A2C_REGISTER_FB netmsgfb;

	BOOL bdelete = DeleteRegister( pchar->m_dwCharID, mapData );
	if ( bdelete ){
		netmsgfb.emFB = WOE_REGISTER_FB_CANCEL_REG;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( !bdelete ){
		netmsgfb.emFB = WOE_REGISTER_FB_CANCEL_FAIL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}
}

void GLPVPWoeAgent::SendPlayerData()
{
	WOE_REGISTER_DATA_VEC vecdata;
	vecdata.reserve(m_mapGuildData.size());

	for( WOE_REGISTER_DATA_MAP_ITER iterplayer = m_mapGuildData.begin();
		iterplayer != m_mapGuildData.end(); ++ iterplayer ){
			const WOE_REGISTER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
	}

	WORD wNum = 0;
	GLMSG::SNETPC_WOE_A2F_PLAYER_DATA	net_msg_data;
	for ( size_t i=0; i<vecdata.size(); ++i )
	{
		const WOE_REGISTER_DATA& splayer_data = vecdata[i];
		net_msg_data.ADDRANK( splayer_data );
		if ( net_msg_data.wRankNum == WOE_PACKET_PLAYER_DATA_NUM )
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
}