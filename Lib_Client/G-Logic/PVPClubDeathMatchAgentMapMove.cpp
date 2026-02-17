#include "StdAfx.h"
#include "./PVPClubDeathMatchAgent.h"
#include "./GLAgentServer.h"

#include "../../Lib_Engine/DxOctree/DxLandGateMan.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void PVPClubDeathMatchAgent::QueueMoveIn()
{
	//queue to move players in
	m_vecPlayerMove.clear();
	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;

		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it2_b = sData.mapPlayerData.begin();
		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it2_e = sData.mapPlayerData.end();

		for ( ; it2_b != it2_e; ++it2_b )
		{
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA& sPlayerData = (*it2_b).second;
			
			PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( sPlayerData.dwCharID );
			if ( !pchar )	continue;
			if ( pchar->m_sCurMapID == m_sLobbyMap )	continue;
		
			m_vecPlayerMove.push_back( sPlayerData );
		}
	}

	m_bMoveIn = true;
}

void PVPClubDeathMatchAgent::QueueMoveOut()
{
	//queue to move players out
	m_vecPlayerMove.clear();
	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;

		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it2_b = sData.mapPlayerData.begin();
		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it2_e = sData.mapPlayerData.end();

		for ( ; it2_b != it2_e; ++it2_b )
		{
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA& sPlayerData = (*it2_b).second;

			PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( sPlayerData.dwCharID );
			if ( !pchar )	continue;
			if ( pchar->m_sCurMapID == m_sOutMap )	continue;

			m_vecPlayerMove.push_back( sPlayerData );
		}
	}

	m_bMoveOut = true;
}


void PVPClubDeathMatchAgent::MapMoveInPlayer()
{
	if ( m_vecPlayerMove.empty() )	return;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA sdata = m_vecPlayerMove[0];

	DWORD dwCharID = sdata.dwCharID;
	SNATIVEID sMapid = m_sLobbyMap;
	DWORD dwGateID = m_dwLobbyGate;

	m_vecPlayerMove.erase(m_vecPlayerMove.begin());

	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( pchar )
	{
		GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( sMapid );
		if ( plandman )
		{
			DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
			if ( pGateMan )
			{
				PDXLANDGATE pGate = pGateMan->FindLandGate ( dwGateID );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE net_msg_field;
					net_msg_field.nidMAP = sMapid;
					net_msg_field.vPos = vGenPos;
					net_msg_field.bCurMapCheck = TRUE;
					net_msg_field.dwCharID = pchar->m_dwCharID;
					GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
					return;
				}
			}
		}
	}

	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::MapMoveInPlayer char:%d failure to move map:[%d~%d] gate%d", dwCharID, sMapid.wMainID, sMapid.wSubID, dwGateID );
}

void PVPClubDeathMatchAgent::MapMoveOutPlayer()
{
	if ( m_vecPlayerMove.empty() )	return;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA sdata = m_vecPlayerMove[0];

	DWORD dwCharID = sdata.dwCharID;
	SNATIVEID sMapid = m_sOutMap;
	DWORD dwGateID = m_dwOutGate;

	m_vecPlayerMove.erase(m_vecPlayerMove.begin());

	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( pchar )
	{
		GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( sMapid );
		if ( plandman )
		{
			DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
			if ( pGateMan )
			{
				PDXLANDGATE pGate = pGateMan->FindLandGate ( dwGateID );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE net_msg_field;
					net_msg_field.nidMAP = sMapid;
					net_msg_field.vPos = vGenPos;
					net_msg_field.bCurMapCheck = TRUE;
					net_msg_field.dwCharID = pchar->m_dwCharID;
					net_msg_field.bSourceMapCheck = TRUE;
					net_msg_field.nidSourceLobbyMAP = m_sLobbyMap;
					net_msg_field.nidSourceBattleMAP = m_sBattleMap;
					GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
					return;
				}
			}
		}
	}

	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::MapMoveOutPlayer char:%d failure to move map:[%d~%d] gate%d", dwCharID, sMapid.wMainID, sMapid.wSubID, dwGateID );
}

void PVPClubDeathMatchAgent::PlayerRejoinBattle( DWORD dwCharID )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB netmsgfb;

	//can only rejoin if battle running
	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//already inside map no need to rejoin
	if ( pchar->m_sCurMapID == m_sLobbyMap || pchar->m_sCurMapID == m_sBattleMap )
	{
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REJOIN_FB_INBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pchar->m_dwGuild );
	if ( !pClubData )
	{
		//not registered
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOT_REGISTERED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayerData =  pClubData->PlayerDataGet( pchar->m_dwCharID );
	if ( !pPlayerData )
	{	
		//not registered
		netmsgfb.emFB = PVP_CLUB_DEATH_MATCH_REJOIN_FB_NOT_REGISTERED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
	if ( plandman )
	{
		DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
		if ( pGateMan )
		{
			PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwLobbyGate );
			if ( pGate )
			{
				D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

				GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE net_msg_field;
				net_msg_field.nidMAP = m_sLobbyMap;
				net_msg_field.vPos = vGenPos;
				net_msg_field.bCurMapCheck = TRUE;
				net_msg_field.dwCharID = pchar->m_dwCharID;
				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
				return;
			}
		}
	}

	RLOG::LogFile( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent::PlayerRejoinBattle failed  [%d][%s] map[%d~%d] gate[%d]", 
		pPlayerData->dwCharID, pPlayerData->szCharName, m_sLobbyMap.wMainID, m_sLobbyMap.wSubID, m_dwLobbyGate );
}

void PVPClubDeathMatchAgent::CheckForceMove( float fElapsedTime )
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();
	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sClubData = (*it_b).second;

		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it_b2 = sClubData.mapPlayerData.begin();
		PVP_CLUB_DEATH_MATCH_PLAYER_DATA_MAP_ITER it_e2 = sClubData.mapPlayerData.end();

		for ( ; it_b2 != it_e2; ++it_b2 )
		{
			PVP_CLUB_DEATH_MATCH_PLAYER_DATA& sPlayerData = (*it_b2).second;

			PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( sPlayerData.dwCharID );
			if ( !pChar )	continue;

			//player is outside lobby and battle map
			if ( pChar->m_sCurMapID != m_sLobbyMap && pChar->m_sCurMapID != m_sBattleMap )
			{
				sPlayerData.fForceMoveLobbyAreaTime += fElapsedTime;

				if ( sPlayerData.fForceMoveLobbyAreaTime >= m_fMoveLobbyAreaTime )
				{
					GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
					if ( plandman )
					{
						DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
						if ( pGateMan )
						{
							PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwLobbyGate );
							if ( pGate )
							{
								D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

								GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE net_msg_field;
								net_msg_field.nidMAP = m_sLobbyMap;
								net_msg_field.vPos = vGenPos;
								net_msg_field.bCurMapCheck = TRUE;
								net_msg_field.dwCharID = pChar->m_dwCharID;
								GLAgentServer::GetInstance().SENDTOFIELDSVR ( pChar->m_nChannel, pChar->m_dwCurFieldSvr, &net_msg_field );
							}
						}
					}


					sPlayerData.fForceMoveLobbyAreaTime = 0.0f;
				}
			}
			//player is outside battle map but idle in lobby
			else if ( pChar->m_sCurMapID == m_sLobbyMap )
			{
				sPlayerData.fForceMoveBattleAreaTime += fElapsedTime;

				if ( sPlayerData.fForceMoveBattleAreaTime >= m_fMoveBattleAreaTime )
				{
					GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sBattleMap );
					if ( plandman )
					{
						DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
						if ( pGateMan )
						{
							PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwBattleGate );
							if ( pGate )
							{
								D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

								GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE net_msg_field;
								net_msg_field.nidMAP = m_sBattleMap;
								net_msg_field.vPos = vGenPos;
								net_msg_field.bCurMapCheck = TRUE;
								net_msg_field.dwCharID = pChar->m_dwCharID;
								GLAgentServer::GetInstance().SENDTOFIELDSVR ( pChar->m_nChannel, pChar->m_dwCurFieldSvr, &net_msg_field );
							}
						}
					}

					sPlayerData.fForceMoveBattleAreaTime = 0.0f;
				}
			}
			else
			{
				sPlayerData.fForceMoveLobbyAreaTime = 0.0f;
				sPlayerData.fForceMoveBattleAreaTime = 0.0f;
			}
		}
	}
}

void PVPClubDeathMatchAgent::MapExit( DWORD dwCharID )
{
	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD && m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED )	return;

	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA* pClubData = GetClubData( pchar->m_dwGuild );
	if ( !pClubData )	return;

	PVP_CLUB_DEATH_MATCH_PLAYER_DATA* pPlayerData =  pClubData->PlayerDataGet( pchar->m_dwCharID );
	if ( !pPlayerData )	return;

	GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sOutMap );
	if ( plandman )
	{
		DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
		if ( pGateMan )
		{
			PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwOutGate );
			if ( pGate )
			{
				D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

				GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE net_msg_field;
				net_msg_field.nidMAP = m_sOutMap;
				net_msg_field.vPos = vGenPos;
				net_msg_field.bCurMapCheck = TRUE;
				net_msg_field.dwCharID = pchar->m_dwCharID;
				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
				return;
			}
		}
	}

	CDebugSet::ToFileWithTime( "_PVPClubDeathMatch.txt", "PVPClubDeathMatchAgent MapExit failed  [%d][%s] map[%d~%d] gate[%d]", 
		pPlayerData->dwCharID, pPlayerData->szCharName, m_sOutMap.wMainID, m_sOutMap.wSubID, m_dwOutGate );
}