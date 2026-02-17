#include "StdAfx.h"
#include "./GLPVPTyrannyAgent.h"
#include "./GLAgentServer.h"
#include "../../Lib_Engine/DxOctree/DxLandGateMan.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLPVPTyrannyAgent::QueueMoveIn()
{
	//queue to move players in
	m_vecPlayerMove.clear();
	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	for( TYRANNY_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer ){
		const TYRANNY_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sScheduleNext.getBatteMap() )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	for( TYRANNY_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer ){
		const TYRANNY_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sScheduleNext.getBatteMap() )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	for( TYRANNY_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer ){
		const TYRANNY_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sScheduleNext.getBatteMap() )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	m_bMoveIn = true;
}

void GLPVPTyrannyAgent::QueueMoveOut()
{
	//queue to move players out
	m_vecPlayerMove.clear();
	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	for( TYRANNY_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataSG.begin(); iterplayer != m_mapPlayerDataSG.end(); ++ iterplayer ){
		const TYRANNY_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sOutMap )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	for( TYRANNY_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataMP.begin(); iterplayer != m_mapPlayerDataMP.end(); ++ iterplayer ){
		const TYRANNY_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sOutMap )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	for( TYRANNY_REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerDataPHX.begin(); iterplayer != m_mapPlayerDataPHX.end(); ++ iterplayer ){
		const TYRANNY_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sOutMap )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	m_bMoveOut = true;
}

void GLPVPTyrannyAgent::MapMoveInPlayer()
{
	if ( m_vecPlayerMove.empty() )	return;

	TYRANNY_REGISTER_DATA sdata = m_vecPlayerMove[0];

	DWORD dwCharID = sdata.dwInfoCharID;
	SNATIVEID sMapid = m_sScheduleNext.getBatteMap();
	WORD wGateID = m_wSchoolGate[sdata.wInfoSchool];

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
				PDXLANDGATE pGate = pGateMan->FindLandGate ( wGateID );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_TYRANNY_A2F_MAP_MOVE net_msg_field;
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

	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent char:%d failure to move map:[%d~%d] gate%d", dwCharID, sMapid.wMainID, sMapid.wSubID, wGateID );
}

void GLPVPTyrannyAgent::MapMoveOutPlayer()
{
	if ( m_vecPlayerMove.empty() )	return;

	TYRANNY_REGISTER_DATA sdata = m_vecPlayerMove[0];

	DWORD dwCharID = sdata.dwInfoCharID;
	SNATIVEID sMapid = m_sOutMap;
	WORD wGateID = m_wOutGate;

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
				PDXLANDGATE pGate = pGateMan->FindLandGate ( wGateID );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_TYRANNY_A2F_MAP_MOVE net_msg_field;
					net_msg_field.nidMAP = sMapid;
					net_msg_field.vPos = vGenPos;
					net_msg_field.bCurMapCheck = TRUE;
					net_msg_field.dwCharID = pchar->m_dwCharID;
					net_msg_field.source_map_check = TRUE;
					net_msg_field.source_map_id = m_sScheduleNext.getBatteMap();
					GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
					return;
				}
			}
		}
	}

	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent char:%d failure to move map:[%d~%d] gate%d", dwCharID, sMapid.wMainID, sMapid.wSubID, wGateID );
}

void GLPVPTyrannyAgent::PlayerRejoinBattle( DWORD dwCharID )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_TYRANNY_A2C_REJOIN_FB netmsgfb;

	//can only rejoin if battle running
	if ( m_emState != TYRANNY_STATE_BATTLE )
	{
		netmsgfb.emFB = TYRANNY_REJOIN_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//already inside map no need to rejoin
	if ( pchar->m_sCurMapID == m_sScheduleNext.getBatteMap() )
	{
		netmsgfb.emFB = TYRANNY_REJOIN_FB_INBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	TYRANNY_REGISTER_DATA* pplayerdata = PlayerDataGet( pchar->m_dwCharID );
	if ( !pplayerdata )
	{	
		//not registered
		netmsgfb.emFB = TYRANNY_REJOIN_FB_NOT_REGISTERED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sScheduleNext.getBatteMap() );
	if ( plandman )
	{
		DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
		if ( pGateMan )
		{
			PDXLANDGATE pGate = pGateMan->FindLandGate ( m_wSchoolGate[pplayerdata->wInfoSchool] );
			if ( pGate )
			{
				D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

				GLMSG::SNETPC_TYRANNY_A2F_MAP_MOVE net_msg_field;
				net_msg_field.nidMAP = m_sScheduleNext.getBatteMap();
				net_msg_field.vPos = vGenPos;
				net_msg_field.bCurMapCheck = TRUE;
				net_msg_field.dwCharID = pchar->m_dwCharID;
				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
				return;
			}
		}
	}

	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent PlayerRejoinBattle failed  [%d][%s] map[%d~%d] gate[%d]", 
		pplayerdata->dwInfoCharID, 
		pplayerdata->szInfoCharName, 
		m_sScheduleNext.getBatteMap().wMainID, 
		m_sScheduleNext.getBatteMap().wSubID, 
		m_wSchoolGate[pplayerdata->wInfoSchool] );
}

void GLPVPTyrannyAgent::PlayerLateJoinBattle( DWORD dwCharID, WORD wLevel, std::string strHash )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_TYRANNY_A2C_LATE_JOIN_FB netmsgfb;

	//can only rejoin if battle running
	if ( m_emState != TYRANNY_STATE_BATTLE )
	{
		netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//already inside map no need to rejoin
	if ( pchar->m_sCurMapID == m_sScheduleNext.getBatteMap() )
	{
		netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_INBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	TYRANNY_REGISTER_DATA* pplayerdata = PlayerDataGet( pchar->m_dwCharID );
	if ( pplayerdata )
	{	
		//already joined
		netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_ALREADY_JOINED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//register only available in channel 0 
	if ( pchar->m_nChannel != 0 )
	{
		netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_INVALID_CHANNEL;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	//player level too low
	if ( wLevel < m_wLevelReq )
	{
		netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_LEVEL_LOW;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( RPARAM::bCTFGVG )
	{
		GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
		GLCLUB *pCLUB = sClubMan.GetClub ( pchar->m_dwGuild );		
	}

	/* PVPTyranny Hash Check, Juver, 2020/07/28 */
	if ( m_bHashCheck )
	{
		//hash check enabled, hash should not be empty
		if ( strHash.empty() )
		{
			netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_DEVICE_INFO_ERROR;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//hash already used
		if ( PCHashCheck( strHash ) )
		{
			netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_DEVICE_REGISTERED;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
	}

	if ( pchar->m_wSchool == TYRANNY_SCHOOL_SG )
	{
		if ( IsPlayerFull( m_mapPlayerDataSG ) )
		{
			netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_FULL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
		else
		{
			InsertRegister( m_mapPlayerDataSG, pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool, TRUE );
		}
	}
	else if ( pchar->m_wSchool == TYRANNY_SCHOOL_MP )
	{
		if ( IsPlayerFull( m_mapPlayerDataMP ) )
		{
			netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_FULL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
		else
		{
			InsertRegister( m_mapPlayerDataMP, pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool, TRUE );
		}
	}
	else if ( pchar->m_wSchool == TYRANNY_SCHOOL_PHX )
	{
		if ( IsPlayerFull( m_mapPlayerDataPHX ) )
		{
			netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_FULL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
		else
		{
			InsertRegister( m_mapPlayerDataPHX, pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool, TRUE );
		}
	}

	TYRANNY_REGISTER_DATA* pRegisterData = PlayerDataGet( dwCharID );
	if ( pRegisterData )
	{
		netmsgfb.emFB = TYRANNY_LATE_JOIN_FB_REGISTERED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

		GLMSG::SNETPC_TYRANNY_A2F_PLAYER_DATA	net_msg_data;
		net_msg_data.ADDRANK( *pRegisterData );
		GLAgentServer::GetInstance().SENDTOCHANNEL( &net_msg_data, 0 );
		net_msg_data.Reset();

		/* PVPTyranny Hash Check, Juver, 2020/07/28 */
		if ( m_bHashCheck )
			PCHashInsert( strHash, pchar->m_dwCharID );

		GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sScheduleNext.getBatteMap() );
		if ( plandman )
		{
			DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
			if ( pGateMan )
			{
				PDXLANDGATE pGate = pGateMan->FindLandGate ( m_wSchoolGate[pRegisterData->wInfoSchool] );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_TYRANNY_A2F_MAP_MOVE net_msg_field;
					net_msg_field.nidMAP = m_sScheduleNext.getBatteMap();
					net_msg_field.vPos = vGenPos;
					net_msg_field.bCurMapCheck = TRUE;
					net_msg_field.dwCharID = pchar->m_dwCharID;
					GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
					return;
				}
			}
		}
	}
}

void GLPVPTyrannyAgent::MapExit( DWORD dwCharID )
{
	if ( m_emState != TYRANNY_STATE_REWARD && m_emState != TYRANNY_STATE_ENDED )	return;

	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	TYRANNY_REGISTER_DATA* pdata = PlayerDataGet( dwCharID );
	if ( !pdata	)	return;

	GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sOutMap );
	if ( plandman )
	{
		DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
		if ( pGateMan )
		{
			PDXLANDGATE pGate = pGateMan->FindLandGate ( m_wOutGate );
			if ( pGate )
			{
				D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

				GLMSG::SNETPC_TYRANNY_A2F_MAP_MOVE net_msg_field;
				net_msg_field.nidMAP = m_sOutMap;
				net_msg_field.vPos = vGenPos;
				net_msg_field.bCurMapCheck = TRUE;
				net_msg_field.dwCharID = pchar->m_dwCharID;
				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
				return;
			}
		}
	}

	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent MapExit failed  [%d][%s] map[%d~%d] gate[%d]", 
		pdata->dwInfoCharID, pdata->szInfoCharName, m_sOutMap.wMainID, m_sOutMap.wSubID, m_wOutGate );
}