#include "stdafx.h"
#include "./GLPVPWoeAgent.h"
#include "./GLAgentServer.h"
#include "../Lib_Engine/DxOctree/DxLandGateMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLPVPWoeAgent::QueueMoveIn()
{
	m_vecPlayerMove.clear();
	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	for( WOE_REGISTER_DATA_MAP_ITER iterplayer = m_mapGuildData.begin(); iterplayer != m_mapGuildData.end(); ++ iterplayer ){
		const WOE_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
		if ( !pchar )	continue;
		if ( pchar->m_sCurMapID == m_sMap )	continue;

		m_vecPlayerMove.push_back( splayerdata );
	}

	m_bMoveIn = true;
}

void GLPVPWoeAgent::QueueMoveOut()
{
	m_vecPlayerMove.clear();
	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	for (WOE_REGISTER_DATA_MAP_ITER iterplayer = m_mapGuildData.begin(); iterplayer != m_mapGuildData.end(); ++iterplayer) {
		const WOE_REGISTER_DATA& splayerdata = (*iterplayer).second;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID(splayerdata.dwInfoCharID);
		if (!pchar)	continue;
		if (pchar->m_sCurMapID == m_sOutMap)	continue;

		m_vecPlayerMove.push_back(splayerdata);
	}

	m_bMoveOut = true;
}

void GLPVPWoeAgent::MapMoveInPlayer()
{
	if ( m_vecPlayerMove.empty() )	return;

	WOE_REGISTER_DATA sdata = m_vecPlayerMove[0];

	DWORD dwCharID = sdata.dwInfoCharID;
	SNATIVEID sMapid = m_sMap;
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

					GLMSG::SNETPC_WOE_A2F_MAP_MOVE net_msg_field;
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
}

void GLPVPWoeAgent::MapMoveOutPlayer()
{
	if ( m_vecPlayerMove.empty() )	return;

	WOE_REGISTER_DATA sdata = m_vecPlayerMove[0];

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

					GLMSG::SNETPC_WOE_A2F_MAP_MOVE net_msg_field;
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
}

void GLPVPWoeAgent::PlayerRejoinBattle( DWORD dwCharID )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	GLMSG::SNETPC_WOE_A2C_REJOIN_FB netmsgfb;

	if ( m_emState != WOE_STATE_BATTLE )
	{
		netmsgfb.emFB = WOE_REJOIN_FB_NOTPOSSIBLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	if ( pchar->m_sCurMapID == m_sMap )
	{
		netmsgfb.emFB = WOE_REJOIN_FB_INBATTLE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	WOE_REGISTER_DATA* pplayerdata = PlayerDataGet( pchar->m_dwCharID );
	if ( !pplayerdata )
	{	
		netmsgfb.emFB = WOE_REJOIN_FB_NOT_REGISTERED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return;
	}

	GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sMap );
	if ( plandman )
	{
		DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
		if ( pGateMan )
		{
			PDXLANDGATE pGate = pGateMan->FindLandGate ( m_wSchoolGate[pplayerdata->wInfoSchool] );
			if ( pGate )
			{
				D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

				GLMSG::SNETPC_WOE_A2F_MAP_MOVE net_msg_field;
				net_msg_field.nidMAP = m_sMap;
				net_msg_field.vPos = vGenPos;
				net_msg_field.bCurMapCheck = TRUE;
				net_msg_field.dwCharID = pchar->m_dwCharID;
				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
				return;
			}
		}
	}
}

void GLPVPWoeAgent::MapExit( DWORD dwCharID )
{
	if ( m_emState != WOE_STATE_REWARD && m_emState != WOE_STATE_ENDED )	return;

	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;

	WOE_REGISTER_DATA* pdata = PlayerDataGet( dwCharID );
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

				GLMSG::SNETPC_WOE_A2F_MAP_MOVE net_msg_field;
				net_msg_field.nidMAP = m_sOutMap;
				net_msg_field.vPos = vGenPos;
				net_msg_field.bCurMapCheck = TRUE;
				net_msg_field.dwCharID = pchar->m_dwCharID;
				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
				return;
			}
		}
	}
}