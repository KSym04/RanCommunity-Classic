#include "stdafx.h"
#include "./GLAgentServer.h"

/*pvp tyranny, Juver, 2017/08/25 */
#include "./GLPVPTyrannyAgent.h"
#include "./GLPVPCaptureTheFlagAgent.h"
#include "./GLPVPSchoolWarsAgent.h"

/* pvp club death match, Juver, 2020/11/26 */
#include "./PVPClubDeathMatchAgent.h"

#include "../DbActionLogic.h"

#include "./UserTypeDefine.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#include "./GMCommandPass.h"
#include "./GLPartyFinder.h"
#include "./GLItemMan.h"

#include "./PVPPartyBattleGroundsAgent.h"
#include "./PVPPartyBattleGroundsMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*event map move, Juver, 2017/08/25 */
BOOL GLAgentServer::MsgGMMove2_MapPos( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_MOVE2_MAPPOS* pNetMsg = (GLMSG::SNETPC_GM_MOVE2_MAPPOS*)nmg;

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )							return FALSE;

	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMEVENT_MOVEMAP_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	/*gm command logs, Juver, 2018/08/18 */
	if ( GetDBMan() && pMyChar )
	{
		char command[1024];
		sprintf_s( command, 1024, "m2mp from map:[%u/%u] to map:[%u/%u] pos:[%u/%u]", pMyChar->m_sCurMapID.wMainID, pMyChar->m_sCurMapID.wSubID, pNetMsg->nidMAP.wMainID, pNetMsg->nidMAP.wSubID, pNetMsg->wPosX, pNetMsg->wPosY );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command );
	}

	GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FLD net_msg_field;
	net_msg_field.nidMAP = pNetMsg->nidMAP;
	net_msg_field.wPosX = pNetMsg->wPosX;
	net_msg_field.wPosY = pNetMsg->wPosY;
	net_msg_field.bCurMapCheck = FALSE;
	net_msg_field.bCalculatePos = TRUE;
	net_msg_field.dwCharID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL ( &net_msg_field );

	return TRUE;
}

/*pvp tyranny, Juver, 2017/08/25 */
void GLAgentServer::TyrannyResetBuffs()
{
	int nCnt = 0;
	AGCHARNODE *pCharNode = m_PCList.m_pHead;
	for ( ; pCharNode; pCharNode=pCharNode->pNext )
	{
		PGLCHARAG pCharAG = pCharNode->Data;
		if( !pCharAG ) continue;

		GLMSG::SNETPC_TYRANNY_A2F_BUFF_RESET netmsgbuff;
		netmsgbuff.dwCharID = pCharAG->m_dwCharID;

		SENDTOFIELDSVR ( pCharAG->m_nChannel, pCharAG->m_dwCurFieldSvr, &netmsgbuff );

		nCnt ++;
	}

	CONSOLEMSG_WRITE( "Tyranny reset buffs to :%d players", nCnt );
}

/*pvp tyranny, Juver, 2017/08/25 */
void GLAgentServer::TyrannySetRewardBuffs()
{
	int nCnt = 0;

	GLMSG::SNETPC_TYRANNY_A2F_BUFF_REWARD netmsgbuff;
	netmsgbuff.sRewardBuffMain = GLPVPTyrannyAgent::GetInstance().m_sScheduleNext.m_sRewardBuffMain;

	for ( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
		netmsgbuff.sRewardBuff[i] = GLPVPTyrannyAgent::GetInstance().m_sScheduleNext.m_sRewardBuff[i];

	WORD wLastWinner = GLPVPTyrannyAgent::GetInstance().m_wLastWinner;

	if ( wLastWinner != TYRANNY_SCHOOL_SIZE )
	{
		AGCHARNODE *pCharNode = m_PCList.m_pHead;
		for ( ; pCharNode; pCharNode=pCharNode->pNext )
		{
			PGLCHARAG pCharAG = pCharNode->Data;
			if( !pCharAG )							continue;
			if( pCharAG->m_wSchool != wLastWinner )	continue;

			netmsgbuff.dwCharID = pCharAG->m_dwCharID;
			GLAgentServer::GetInstance().SENDTOFIELDSVR ( pCharAG->m_nChannel, pCharAG->m_dwCurFieldSvr, &netmsgbuff );

			nCnt ++;
		}
	}
	
	CONSOLEMSG_WRITE( "Tyranny set reward buffs to :%d players", nCnt );
}

/*pvp capture the flag, Juver, 2018/01/30 */
void GLAgentServer::CaptureTheFlagRemoveParty( DWORD dwCharID )
{
	PGLCHARAG pchar = GetCharID( dwCharID );
	if ( !pchar )	return;
	
	GLPARTY* pparty = m_cPartyMan.GetParty ( pchar->m_dwPartyID );
	if ( !pparty )	return;
	if ( !pparty->ISMEMBER( pchar->m_dwGaeaID) )	return;

	if ( pparty->ISMASTER( pchar->m_dwGaeaID ) )
	{
		m_cPartyMan.DelParty ( pchar->m_dwPartyID );
	}
	else
	{
		GLMSG::SNET_PARTY_DEL NetMsgDel;
		NetMsgDel.dwPartyID = pchar->m_dwPartyID;
		NetMsgDel.dwDelMember = pchar->m_dwGaeaID;
		SENDTOPARTY ( pchar->m_dwPartyID, (NET_MSG_GENERIC*) &NetMsgDel );
		SENDTOALLCHANNEL ( &NetMsgDel );

		pchar->ReSetPartyID ();
		pparty->DELMEMBER ( pchar->m_dwGaeaID );
	}
}

BOOL GLAgentServer::MsgEventContriTyranny( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY* pNetMsg = (GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY*)nmg;

	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )							return FALSE;

	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassEventContriTyranny.c_str() ) != 0 )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( pNetMsg->fRate < 1.0f )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_RATE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	BOOL bSet = GLPVPTyrannyAgent::GetInstance().SetContributionPointMultiplier( pNetMsg->fRate );
	if ( bSet )
	{
		float fRateNew = GLPVPTyrannyAgent::GetInstance().GetContributionPointMultiplier();

		GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FB NetMsgFB;
		NetMsgFB.fRate = fRateNew;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_OK;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );

		GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_BROADCAST NetMsgBRD;
		NetMsgBRD.fRate = fRateNew;
		SENDTOALLCLIENT( &NetMsgBRD );

		GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FLD NetMsgFLD;
		NetMsgFLD.fRate = fRateNew;
		SENDTOALLCHANNEL( &NetMsgFLD );
	}
	
	return TRUE;
}

BOOL GLAgentServer::MsgEventContriSchoolWar( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR* pNetMsg = (GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR*)nmg;

	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )							return FALSE;

	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassEventContriSchoolWars.c_str() ) != 0 )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( pNetMsg->fRate < 1.0f )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_RATE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	BOOL bSet = GLPVPSchoolWarsAgent::GetInstance().SetContributionPointMultiplier( pNetMsg->fRate );
	if ( bSet )
	{
		float fRateNew = GLPVPSchoolWarsAgent::GetInstance().GetContributionPointMultiplier();

		GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB NetMsgFB;
		NetMsgFB.fRate = fRateNew;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_OK;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );

		GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_BROADCAST NetMsgBRD;
		NetMsgBRD.fRate = fRateNew;
		SENDTOALLCLIENT( &NetMsgBRD );

		GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FLD NetMsgFLD;
		NetMsgFLD.fRate = fRateNew;
		SENDTOALLCHANNEL( &NetMsgFLD );
	}

	return TRUE;
}

BOOL GLAgentServer::MsgEventContriCaptureTheFlag( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG* pNetMsg = (GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG*)nmg;

	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )							return FALSE;

	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassEventContriCaptureTheFlag.c_str() ) != 0 )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( pNetMsg->fRate < 1.0f )
	{
		GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_RATE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	BOOL bSet = GLPVPCaptureTheFlagAgent::GetInstance().SetContributionPointMultiplier( pNetMsg->fRate );
	if ( bSet )
	{
		float fRateNew = GLPVPCaptureTheFlagAgent::GetInstance().GetContributionPointMultiplier();

		GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB NetMsgFB;
		NetMsgFB.fRate = fRateNew;
		NetMsgFB.emFB = EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_OK;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );

		GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_BROADCAST NetMsgBRD;
		NetMsgBRD.fRate = fRateNew;
		SENDTOALLCLIENT( &NetMsgBRD );

		GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FLD NetMsgFLD;
		NetMsgFLD.fRate = fRateNew;
		SENDTOALLCHANNEL( &NetMsgFLD );
	}

	return TRUE;
}

void GLAgentServer::MsgGMKickOut ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_KICKOUT* pNetMsg = (GLMSG::SNETPC_GM_KICKOUT*)nmg;

	PGLCHARAG pCharGM = GetChar ( dwGaeaID );
	if ( !pCharGM )	return;

	GLMSG::SNETPC_GM_KICKOUT_FB NetMsgFB;
	NetMsgFB.dwUserNum = pNetMsg->dwUserNum;
	NetMsgFB.fTime = pNetMsg->fTime;

	if ( pCharGM->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_NOT_GM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassKickOut.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_WRONG_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pNetMsg->fTime != 0.0f && pNetMsg->fTime < RPARAM::fKickOutMinTime )
	{
		NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_TIME_LOW;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pNetMsg->fTime > RPARAM::fKickOutMaxTime )
	{
		NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_TIME_HIGH;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pNetMsg->dwUserNum == GAEAID_NULL )	return;

	//delete kickout
	if ( pNetMsg->fTime == 0.0f )
	{
		KICKOUT_DATA_MAP_ITER it = m_mapKickOut.find( pNetMsg->dwUserNum );
		if ( it != m_mapKickOut.end() )
		{
			SKICKOUT_DATA& sData = (*it).second;

			RLOG::LogFile( "_kickout.txt", "kickout user:[%u] time:%g removed", sData.dwUserNum, sData.fTime );

			m_mapKickOut.erase(it);

			NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_DELETE_DONE;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
		}
		else
		{
			RLOG::LogFile( "_kickout.txt", "kickout user:[%u] failed user not found", pNetMsg->dwUserNum );

			NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_DELETE_FAIL;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
		}
	}
	else
	{
		KICKOUT_DATA_MAP_ITER it = m_mapKickOut.find( pNetMsg->dwUserNum );
		if ( it != m_mapKickOut.end() )
		{
			SKICKOUT_DATA& sData = (*it).second;
			sData.fTime = pNetMsg->fTime;
			sData.fCurrentTime = 0.0f;

			RLOG::LogFile( "_kickout.txt", "kickout user:[%u] time:%g updated", sData.dwUserNum, sData.fTime );

			NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_UPDATE_DONE;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
		}
		else
		{
			//create new
			SKICKOUT_DATA sData;
			sData.dwUserNum = pNetMsg->dwUserNum;
			sData.fTime = pNetMsg->fTime;
			sData.fCurrentTime = 0.0f;

			RLOG::LogFile( "_kickout.txt", "kickout user:[%u] time:%g added", sData.dwUserNum, sData.fTime );

			m_mapKickOut.insert( std::make_pair( sData.dwUserNum, sData ) );

			NetMsgFB.emFB = EMREQ_GM_KICKOUT_FB_ADDED_DONE;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
		}
	}

	if ( GetDBMan() && pCharGM )
	{
		char command[1024];
		sprintf_s( command, 1024, "kickout user:[%u] seconds:%g", pNetMsg->dwUserNum, pNetMsg->fTime );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( pCharGM->m_dwClientID, pCharGM->m_dwUserID, pCharGM->m_dwUserLvl, pCharGM->m_dwCharID, pCharGM->m_szName, command );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  pCharGM->m_dwUserID, pCharGM->m_dwUserLvl, pCharGM->m_dwCharID, pCharGM->m_szName, command );
	}

	KickOutListSave();
}

void GLAgentServer::KickOutUpdate( float fTime, float fElapsedTime )
{
	KICKOUT_DATA_MAP_ITER it_b = m_mapKickOut.begin();
	KICKOUT_DATA_MAP_ITER it_e = m_mapKickOut.end();

	for( ; it_b != it_e; ++it_b )
	{
		SKICKOUT_DATA& sData = (*it_b).second;
		sData.fCurrentTime += fElapsedTime;
		
		if ( sData.fCurrentTime > sData.fTime )
		{
			PGLCHARAG pChar = GetCharUID( sData.dwUserNum );
			if ( pChar )
			{
				RLOG::LogFile( "_kickout.txt", "user:[%u] %s char:[%u] %s kicked", pChar->m_dwUserID, pChar->m_szUserName, pChar->m_dwCharID, pChar->m_szName );

				GLMSG::SNET_GM_KICK_USER_PROC_FLD NetMsgFld;
				NetMsgFld.dwID = pChar->m_dwCharID;
				SENDTOALLCHANNEL ( &NetMsgFld );

				GLMSG::SNET_GM_KICK_USER_PROC NetMsgProc;
				SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgProc );

				ReserveDropOutPC ( pChar->m_dwGaeaID );
			}

			sData.fCurrentTime = 0.0f;
		}
	}
}

/* party finder, Juver, 2020/01/03 */
void GLAgentServer::PartyFinderSearch ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_PARTY_FINDER_SEARCH* pNetMsg = (GLMSG::SNETPC_PARTY_FINDER_SEARCH*)nmg;

	GLMSG::SNETPC_PARTY_FINDER_SEARCH_FB NetMsgFB;
	NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_FAIL;

	if ( !RPARAM::bUsePartyFinder )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_FUNCTION_DISABLED;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	PGLCHARAG pChar = GetChar ( dwGaeaID );
	if ( !pChar )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pNetMsg->dwIndex == SPARTY_FINDER_SEARCH_INDEX_NULL )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_INVALID_TYPE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	SPARTY_FINDER_MAP_DATA* pData = GLPartyFinder::GetInstance().GetData( pNetMsg->dwIndex );
	if ( !pData )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_INVALID_TYPE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pChar->m_fPartyFinderSearchDelayTimer < RPARAM::fPartyFinderSearchDelay )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_REQUIRE_TIME;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	pChar->m_fPartyFinderSearchDelayTimer = 0.0f;

	NetMsgFB.emFB = EMPARTY_FINDER_SEARCH_FB_START_SEARCH;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );


	SNATIVEID sMapIDSearch = pData->sMapID;
	WORD wSchool = pChar->m_wSchool;

	GLMSG::SNETPC_PARTY_FINDER_RESULT_UPDATE	NetMsgResultUpdate;

	DWORD dwMaxPartyID = m_cPartyMan.getMaxParty();
	for ( DWORD dwPartyID=0; dwPartyID<dwMaxPartyID; ++dwPartyID )
	{
		GLPARTY* pParty = m_cPartyMan.GetParty (dwPartyID);
		if ( !pParty )		continue;

		PGLCHARAG pMaster = GetChar ( pParty->m_dwMASTER );
		if ( !pMaster )		continue;

		//party is private
		if ( pParty->m_sOPTION.bPrivateParty )		continue;

		//party is full
		if ( pParty->ISFULL() )						continue;

		//gm characters can search any party
		if ( pChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER )
		{
			//check party leader school matches the school of the player searching
			if( !GLCONST_CHAR::bPARTY_2OTHERSCHOOL )
			{
				if ( wSchool != pMaster->m_wSchool )	continue;
			}
		}
		
		//instance map check
		if ( pMaster->m_sCurMapID.wSubID != 0 )
		{
			SNATIVEID sMapIDCheck = pMaster->m_sCurMapID;
			sMapIDCheck.wSubID = 0;
			
			SMAPNODE* pMapNode = m_sMapList.FindMapNode ( sMapIDCheck );
			if ( pMapNode && pMapNode->bInstantMap )
			{
				continue;
			}
		}

		GLAGLandMan* pLand = GetByMapID ( pMaster->m_sCurMapID );
		if ( pLand )
		{
			/*map party setting, Juver, 2018/06/29 */
			if ( pLand->IsBlockParty() )		continue;

			//ctf map should be party with the same team
			if ( pLand->m_bPVPCaptureTheFlagMap )		continue;

			//club death match map should be party with the same guild
			if ( pLand->m_bClubDeathMatchMap )			continue;

			/* pvp club death match, Juver, 2020/11/25 */
			//club death match map should be party with the same guild
			if ( pLand->m_bPVPClubDeathMatchBattleMap ||
				pLand->m_bPVPClubDeathMatchLobbyMap )	continue;

			if ( pLand->m_bPVPPBGBattleMap || pLand->m_bPVPPBGLobbyMap )	
				continue;
		}

		//check party leader map matches search requirement map
		if ( sMapIDSearch != NATIVEID_NULL() )
		{
			if ( sMapIDSearch != pMaster->m_sCurMapID )	continue;
		}


		SPARTY_FINDER_RESULT sResult;
		sResult.dwPartyID = pParty->m_dwPARTYID;

		sResult.dwLeaderCharID = pMaster->m_dwCharID;
		sResult.wLeaderSchool = pMaster->m_wSchool;
		sResult.wLeaderClass = (WORD)CharClassToIndex( pMaster->m_emClass );
		StringCchCopy ( sResult.szLeaderName, CHAR_SZNAME, pMaster->m_szName );

		std::string strCUR_MAP = GetMapName( pMaster->m_sCurMapID );
		StringCchCopy ( sResult.szMapName, MAP_NAME_MAX, strCUR_MAP.c_str() );

		sResult.dwPartyMembers = pParty->GETNUMBER();
		sResult.wPartyItemOption = (WORD)pParty->m_sOPTION.emGET_ITEM;
		sResult.wPartyMoneyOption = (WORD)pParty->m_sOPTION.emGET_MONEY;

		NetMsgResultUpdate.ADD( sResult );
		if ( NetMsgResultUpdate.wResultNum == SPARTY_FINDER_RESULT_NUM )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgResultUpdate );
			NetMsgResultUpdate.Reset();
		}
	}

	if ( NetMsgResultUpdate.wResultNum != 0 )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgResultUpdate );
		NetMsgResultUpdate.Reset();
	}
	
	GLMSG::SNETPC_PARTY_FINDER_RESULT_DONE	NetMsgResultDone;
	SENDTOCLIENT ( dwClientID, &NetMsgResultDone );
}

/* party finder, Juver, 2020/01/16 */
void GLAgentServer::PartyFinderJoin ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_PARTY_FINDER_JOIN* pNetMsg = (GLMSG::SNETPC_PARTY_FINDER_JOIN*)nmg;

	GLMSG::SNETPC_PARTY_FINDER_JOIN_FB NetMsgFB;
	NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_FAIL;

	if ( !RPARAM::bUsePartyFinder )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_FUNCTION_DISABLED;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	PGLCHARAG pChar = GetChar ( dwGaeaID );
	if ( !pChar )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pChar->m_dwPartyID != PARTY_NULL )
	{

		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_IN_PARTY;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pNetMsg->dwPartyID == PARTY_NULL )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_INVALID_PARTY;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	GLPARTY* pParty = m_cPartyMan.GetParty (pNetMsg->dwPartyID);
	if ( !pParty )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_INVALID_PARTY;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pParty->m_sOPTION.bPrivateParty )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_PARTY_PRIVATE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pParty->ISFULL() )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_PARTY_FULL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	PGLCHARAG pMaster = GLAgentServer::GetInstance().GetChar ( pParty->m_dwMASTER );
	if ( !pMaster )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_INVALID_PARTY;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	//gm characters can join any school party
	if ( pChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER )
	{
		if ( !GLCONST_CHAR::bPARTY_2OTHERSCHOOL )
		{
			if ( pChar->m_wSchool!=pMaster->m_wSchool )
			{
				NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_INVALID_PARTY;
				SENDTOCLIENT ( dwClientID, &NetMsgFB );
				return;
			}
		}
	}

	GLClubMan& cClubMan = GLAgentServer::GetInstance().GetClubMan();

	GLCLUB *pCLUB = cClubMan.GetClub ( pChar->m_dwGuild );
	GLCLUB *pCLUB_M = cClubMan.GetClub ( pMaster->m_dwGuild );

	if ( pCLUB )
	{
		bool bClubBattle = false;

		if( pParty && pParty->ISVAILD() )
		{
			GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
			GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();
			PGLCHARAG pMemChar = NULL;

			for ( ; iter!=iter_end; ++iter )
			{
				pMemChar = GLAgentServer::GetInstance().GetChar ( (*iter) );
				if ( !pMemChar ) continue;

				if ( pCLUB->IsBattle(pMemChar->m_dwGuild) )
				{
					bClubBattle = true;
					break;
				}

				GLCLUB* pClub_Mem = cClubMan.GetClub( pMemChar->m_dwGuild );

				if( pClub_Mem && pCLUB->IsBattleAlliance( pClub_Mem->m_dwAlliance ))
				{
					bClubBattle = true;
					break;
				}
			}
		}	

		if ( bClubBattle ) 
		{
			NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_REQUIRE_TIME;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return;
		}
	}

	/*map party setting, Juver, 2018/06/29 */
	GLAGLandMan* pland_master = GetByMapID ( pMaster->m_sCurMapID );
	if ( pland_master && pland_master->IsBlockParty() )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_LEADER;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	/*map party setting, Juver, 2018/06/29 */
	GLAGLandMan* pland_target = GetByMapID ( pChar->m_sCurMapID );
	if ( pland_target && pland_target->IsBlockParty() )	
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_MEMBER;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	if ( pChar->m_fPartyFinderJoinDelayTimer < RPARAM::fPartyFinderJoinDelay )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_REQUIRE_TIME;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	pChar->m_fPartyFinderJoinDelayTimer = 0.0f;

	NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_REQUEST_SENT;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );

	pParty->PREADDMEMBER ( pChar->m_dwGaeaID );

	GLMSG::SNETPC_PARTY_FINDER_QUESTION NetMsgQuestion;
	NetMsgQuestion.sQuestion.dwPartyID = pParty->m_dwPARTYID;
	NetMsgQuestion.sQuestion.dwCharID = pChar->m_dwCharID;
	NetMsgQuestion.sQuestion.wSchool = pChar->m_wSchool;
	NetMsgQuestion.sQuestion.wClass = (WORD)CharClassToIndex( pChar->m_emClass );
	NetMsgQuestion.sQuestion.wLevel = pNetMsg->wLevel;
	StringCchCopy ( NetMsgQuestion.sQuestion.szName, CHAR_SZNAME, pChar->m_szName );

	SENDTOCLIENT ( pMaster->m_dwClientID, &NetMsgQuestion );
}

/* party finder, Juver, 2020/01/19 */
void GLAgentServer::PartyFinderReply ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_PARTY_FINDER_REPLY* pNetMsg = (GLMSG::SNETPC_PARTY_FINDER_REPLY*)nmg;

	if ( !RPARAM::bUsePartyFinder )	return;
	
	GLPARTY* pParty = m_cPartyMan.GetParty (pNetMsg->dwPartyID);
	if ( !pParty )	return;
	
	PGLCHARAG pMaster = GLAgentServer::GetInstance().GetChar ( pParty->m_dwMASTER );
	if ( !pMaster )	return;

	PGLCHARAG pChar = GetCharID( pNetMsg->dwCharID );
	if ( !pChar )	return;

	GLMSG::SNETPC_PARTY_FINDER_JOIN_FB NetMsgFB;
	NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_FAIL;

	pParty->PREDELMEMBER ( pChar->m_dwGaeaID );

	//leader declined
	if ( !pNetMsg->bAccept )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_DECLINE;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	//party is full
	if ( pParty->ISFULL() )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_PARTY_FULL;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	//party is private
	if ( pParty->m_sOPTION.bPrivateParty )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_PARTY_PRIVATE;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	//gm characters can join any school party
	if ( pChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER )
	{
		if ( !GLCONST_CHAR::bPARTY_2OTHERSCHOOL )
		{
			if ( pChar->m_wSchool!=pMaster->m_wSchool )
			{
				NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_INVALID_PARTY;
				SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
				return;
			}
		}
	}

	if ( pChar->m_dwPartyID != PARTY_NULL )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_IN_PARTY;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		SENDTOCLIENT ( pMaster->m_dwClientID, &NetMsgFB );
		return;
	}

	pParty->PREDELMEMBER ( pChar->m_dwGaeaID );

	/*map party setting, Juver, 2018/06/29 */
	GLAGLandMan* pland_master = GetByMapID ( pMaster->m_sCurMapID );
	if ( pland_master && pland_master->IsBlockParty() )
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_LEADER;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	/*map party setting, Juver, 2018/06/29 */
	GLAGLandMan* pland_target = GetByMapID ( pChar->m_sCurMapID );
	if ( pland_target && pland_target->IsBlockParty() )	
	{
		NetMsgFB.emFB = EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_MEMBER;
		SENDTOCLIENT ( pChar->m_dwClientID, &NetMsgFB );
		return;
	}

	/* log gm party, Juver, 2021/06/13 */
	if ( pMaster->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_SPECIAL )
	{
		LogGMParty *pdb_action = new LogGMParty( 
			pMaster->m_dwClientID, 
			pMaster->m_dwUserID, pMaster->m_dwCharID, pMaster->m_szName, pMaster->m_dwUserLvl, 
			pChar->m_dwUserID, pChar->m_dwCharID, pChar->m_szName, pChar->m_dwUserLvl );
		m_pDBMan->AddJob ( pdb_action );
	}

	/* log gm party, Juver, 2021/06/13 */
	if ( pChar->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_SPECIAL )
	{
		LogGMParty *pdb_action = new LogGMParty( 
			pChar->m_dwClientID, 
			pChar->m_dwUserID, pChar->m_dwCharID, pChar->m_szName, pChar->m_dwUserLvl, 
			pMaster->m_dwUserID, pMaster->m_dwCharID, pMaster->m_szName, pMaster->m_dwUserLvl );
		m_pDBMan->AddJob ( pdb_action );
	}


	pChar->SetPartyID ( pMaster->GetPartyID() );
	pParty->ADDMEMBER ( pChar->m_dwGaeaID );

	GLMSG::SNET_PARTY_ADD NetPartyAdd;
	NetPartyAdd.dwPartyID = pMaster->GetPartyID();
	NetPartyAdd.sPartyInfo = GLPARTY_FNET(pChar->m_dwGaeaID,pChar->m_szName,pChar->m_emClass,pChar->m_wSchool,pChar->m_sCurMapID);

	SENDTOALLCHANNEL ( &NetPartyAdd );
}

/* charinfoview, Juver, 2020/03/03 */
void GLAgentServer::CharInfoViewRequestGaea ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !RPARAM::bViewCharInfo )	return;

	PGLCHARAG pCharViewer = GetChar ( dwGaeaID );
	if ( !pCharViewer )				return;

	GLMSG::SNETPC_REQ_CHARINFO_GAEA* pNetMsg = (GLMSG::SNETPC_REQ_CHARINFO_GAEA*)nmg;

	GLMSG::SNETPC_REQ_CHARINFO_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;

	PGLCHARAG pCharRequested = GetChar( pNetMsg->dwGaeaID );
	if ( !pCharRequested )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	/*hide gm info, Juver, 2017/11/26 */
	if ( RPARAM::bHideGMInfo && pCharRequested->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 && pCharViewer->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	if  ( pCharViewer->m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_TIMEDELAY;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	GLMSG::SNETPC_REQ_CHARINFO_FLD NetMsgFLD;
	NetMsgFLD.dwCharIDViewer = pCharViewer->m_dwCharID;
	NetMsgFLD.dwUserLevel = pCharViewer->m_dwUserLvl;
	NetMsgFLD.dwCharIDRequested = pCharRequested->m_dwCharID;
	SENDTOALLCHANNEL ( &NetMsgFLD );

	pCharViewer->m_fReqCharInfoTimer = 0.0f;
}

/* charinfoview, Juver, 2020/03/03 */
void GLAgentServer::CharInfoViewRequestCharID ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !RPARAM::bViewCharInfo )	return;

	PGLCHARAG pCharViewer = GetChar ( dwGaeaID );
	if ( !pCharViewer )				return;

	GLMSG::SNETPC_REQ_CHARINFO_CHAR_ID* pNetMsg = (GLMSG::SNETPC_REQ_CHARINFO_CHAR_ID*)nmg;

	GLMSG::SNETPC_REQ_CHARINFO_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;

	PGLCHARAG pCharRequested = GetCharID( pNetMsg->dwCharID );
	if ( !pCharRequested )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	/*hide gm info, Juver, 2017/11/26 */
	if ( RPARAM::bHideGMInfo && pCharRequested->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 && pCharViewer->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	if  ( pCharViewer->m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_TIMEDELAY;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	GLMSG::SNETPC_REQ_CHARINFO_FLD NetMsgFLD;
	NetMsgFLD.dwCharIDViewer = pCharViewer->m_dwCharID;
	NetMsgFLD.dwUserLevel = pCharViewer->m_dwUserLvl;
	NetMsgFLD.dwCharIDRequested = pCharRequested->m_dwCharID;
	SENDTOALLCHANNEL ( &NetMsgFLD );

	pCharViewer->m_fReqCharInfoTimer = 0.0f;
}

/* charinfoview, Juver, 2020/03/03 */
void GLAgentServer::CharInfoViewRequestCharName ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !RPARAM::bViewCharInfo )	return;

	PGLCHARAG pCharViewer = GetChar ( dwGaeaID );
	if ( !pCharViewer )				return;

	GLMSG::SNETPC_REQ_CHARINFO_CHAR_NAME* pNetMsg = (GLMSG::SNETPC_REQ_CHARINFO_CHAR_NAME*)nmg;

	GLMSG::SNETPC_REQ_CHARINFO_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;

	if ( strlen(pNetMsg->szCharName) == 0 )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	PGLCHARAG pCharRequested = GetChar( pNetMsg->szCharName );
	if ( !pCharRequested )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	/*hide gm info, Juver, 2017/11/26 */
	if ( RPARAM::bHideGMInfo && pCharRequested->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 && pCharViewer->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	if  ( pCharViewer->m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_TIMEDELAY;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	GLMSG::SNETPC_REQ_CHARINFO_FLD NetMsgFLD;
	NetMsgFLD.dwCharIDViewer = pCharViewer->m_dwCharID;
	NetMsgFLD.dwUserLevel = pCharViewer->m_dwUserLvl;
	NetMsgFLD.dwCharIDRequested = pCharRequested->m_dwCharID;
	SENDTOALLCHANNEL ( &NetMsgFLD );

	pCharViewer->m_fReqCharInfoTimer = 0.0f;
}

/* charinfoview, Juver, 2020/03/03 */
void GLAgentServer::CharInfoViewRequestAGT ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_CHARINFO_AGT *pNetMsg = (GLMSG::SNETPC_REQ_CHARINFO_AGT *) nmg;

	PGLCHARAG pCharViewer = GetCharID ( pNetMsg->dwCharIDViewer );
	if ( !pCharViewer )				return;

	GLMSG::SNETPC_REQ_CHARINFO_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;

	if ( pNetMsg->emFB == EMREQ_CHARINFO_AGT_FB_FAIL )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_INVALID_TARGET;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
		return;
	}

	if ( pNetMsg->emFB == EMREQ_CHARINFO_AGT_FB_WEAR_INFO )
	{
		NetMsgFB.emFB = EMREQ_CHARINFO_FB_WEARINFO;
		NetMsgFB.sCharInfo = pNetMsg->sCharInfo;
		SENDTOCLIENT ( pCharViewer->m_dwClientID, &NetMsgFB );
	}
}

void GLAgentServer::MsgGMPCID ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_PCID* pNetMsg = (GLMSG::SNETPC_GM_PCID*)nmg;

	PGLCHARAG pCharGM = GetChar ( dwGaeaID );
	if ( !pCharGM )	return;

	GLMSG::SNETPC_GM_PCID_FB NetMsgFB;

	PGLCHARAG pCharTarget = GetCharID(pNetMsg->dwCharID);
	if ( !pCharTarget )
	{
		NetMsgFB.emFB = EMREQ_GM_PCID_FB_INVALID_TARGET;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pCharGM->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_GM_PCID_FB_NOT_GM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassPCID.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_PCID_FB_WRONG_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	GLMSG::SNETPC_GM_PCID_TO_CHAR NetMsgtoChar;
	NetMsgtoChar.dwType = pNetMsg->dwType;
	NetMsgtoChar.dwCharIDSource = pCharGM->m_dwCharID;
	NetMsgtoChar.dwCharIDTarget = pCharTarget->m_dwCharID;
	SENDTOCLIENT ( pCharTarget->m_dwClientID, &NetMsgtoChar );

	if ( GetDBMan() && pCharGM )
	{
		char command[1024];
		sprintf_s( command, 1024, "pcid usernum:[%u] charnum:[%u]%s type:%u", pCharTarget->m_dwUserID, pCharTarget->m_dwCharID, pCharTarget->m_szName, pNetMsg->dwType );

		gm_cmd_logs *pdb_action = new gm_cmd_logs( pCharGM->m_dwClientID, pCharGM->m_dwUserID, pCharGM->m_dwUserLvl, pCharGM->m_dwCharID, pCharGM->m_szName, command );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  pCharGM->m_dwUserID, pCharGM->m_dwUserLvl, pCharGM->m_dwCharID, pCharGM->m_szName, command );
	}
}

void GLAgentServer::MsgGMPCIDReply ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_PCID_REPLY* pNetMsg = (GLMSG::SNETPC_GM_PCID_REPLY*)nmg;

	PGLCHARAG pCharTarget = GetChar ( dwGaeaID );
	if ( !pCharTarget )	return;
	if ( pCharTarget->m_dwCharID != pNetMsg->dwCharIDTarget )	return;

	PGLCHARAG pCharGM = GetCharID ( pNetMsg->dwCharIDSource );
	if ( !pCharGM )	
		return;

	if ( pCharGM->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
		return;

	GLMSG::SNETPC_GM_PCID_FB NetMsgFB;

	CString strHWID = pNetMsg->szPCID_HWID;
	strHWID.Trim();
	if ( STRUTIL::CheckString_Special3( strHWID.GetString() ) )
	{
		NetMsgFB.emFB = EMREQ_GM_PCID_FB_FAIL;
		SENDTOCLIENT ( pCharGM->m_dwClientID, &NetMsgFB );
		return;
	}

	CString strMAC = pNetMsg->szPCID_MAC;
	strMAC.Trim();
	if ( STRUTIL::CheckString_Special3( strMAC.GetString() ) )
	{
		NetMsgFB.emFB = EMREQ_GM_PCID_FB_FAIL;
		SENDTOCLIENT ( pCharGM->m_dwClientID, &NetMsgFB );
		return;
	}

	NetMsgFB.emFB = EMREQ_GM_PCID_FB_REPLY;
	StringCchCopy(NetMsgFB.szPCID_HWID, PCID_LENGTH, pNetMsg->szPCID_HWID );
	StringCchCopy(NetMsgFB.szPCID_MAC, PCID_LENGTH, pNetMsg->szPCID_MAC );
	SENDTOCLIENT ( pCharGM->m_dwClientID, &NetMsgFB );
	
	if ( pNetMsg->dwType == 1 || pNetMsg->dwType == 2 || pNetMsg->dwType == 3 )
	{
		PCIDBlock *pDBActionPCIDBlock = new PCIDBlock( 
			pCharTarget->m_dwClientID, 
			pCharTarget->m_dwUserID, 
			pCharTarget->m_dwCharID, 
			pCharTarget->m_szName, 
			pNetMsg->dwType, 
			pNetMsg->szPCID_HWID, 
			pNetMsg->szPCID_MAC );
		
		GetDBMan()->AddJob ( pDBActionPCIDBlock );

		GLMSG::SNET_GM_KICK_USER_PROC_FLD NetMsgFld;
		NetMsgFld.dwID = pCharTarget->m_dwCharID;
		SENDTOALLCHANNEL ( &NetMsgFld );

		GLMSG::SNET_GM_KICK_USER_PROC NetMsgProc;
		SENDTOCLIENT ( pCharTarget->m_dwClientID, &NetMsgProc );

		ReserveDropOutPC ( pCharTarget->m_dwGaeaID );
	}
}

void GLAgentServer::MsgGMPCIDReply2 ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_PCID_REPLY2* pNetMsg = (GLMSG::SNETPC_GM_PCID_REPLY2*)nmg;

	PGLCHARAG pCharTarget = GetChar ( dwGaeaID );
	if ( !pCharTarget )	return;
	if ( pCharTarget->m_dwCharID != pNetMsg->dwCharIDTarget )	return;

	PGLCHARAG pCharGM = GetCharID ( pNetMsg->dwCharIDSource );
	if ( !pCharGM )	
		return;

	if ( pCharGM->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
		return;

	if ( pNetMsg->dwType == 0 )
	{	
		GLMSG::SNETPC_GM_PCID_FB2 NetMsgFB;
		StringCchCopy(NetMsgFB.szPCID_HWID, PCID_LENGTH, pNetMsg->szPCID_HWID );
		StringCchCopy(NetMsgFB.szPCID_MAC, PCID_LENGTH, pNetMsg->szPCID_MAC );

		StringCchCopy(NetMsgFB.szBiosUUID, PCID_LENGTH, pNetMsg->szBiosUUID );
		StringCchCopy(NetMsgFB.szBoardModel, PCID_LENGTH, pNetMsg->szBoardModel );
		StringCchCopy(NetMsgFB.szBoardSerial, PCID_LENGTH, pNetMsg->szBoardSerial );
		StringCchCopy(NetMsgFB.szCPUVersion, PCID_LENGTH, pNetMsg->szCPUVersion );
		StringCchCopy(NetMsgFB.szCPUID, PCID_LENGTH, pNetMsg->szCPUID );
		StringCchCopy(NetMsgFB.szMAC, PCID_LENGTH, pNetMsg->szMAC );

		SENDTOCLIENT ( pCharGM->m_dwClientID, &NetMsgFB );
	}
}

/* user flag restricted, Juver, 2020/04/21 */
void GLAgentServer::MsgGMUserRestrict ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_GM_USER_RESTRICT* pNetMsg = (GLMSG::SNETPC_GM_USER_RESTRICT*)nmg;

	PGLCHARAG pCharGM = GetChar ( dwGaeaID );
	if ( !pCharGM )	return;

	GLMSG::SNETPC_GM_USER_RESTRICT_FB NetMsgFB;

	PGLCHARAG pCharTarget = GetCharID(pNetMsg->dwCharID);
	if ( !pCharTarget )
	{
		NetMsgFB.emFB = EMREQ_GM_USER_RESTRICT_FB_INVALID_TARGET;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pCharGM->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_GM_USER_RESTRICT_FB_NOT_GM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassUserRestrict.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_USER_RESTRICT_FB_WRONG_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	UserRestrict *pDBAction = new UserRestrict( pCharTarget->m_dwClientID, pCharTarget->m_dwUserID, pNetMsg->bRestrict );
	GetDBMan()->AddJob ( pDBAction );

	pCharTarget->m_bUserFlagRestricted = pNetMsg->bRestrict;

	NetMsgFB.bUserRestricted = pCharTarget->m_bUserFlagRestricted;
	NetMsgFB.emFB = EMREQ_GM_USER_RESTRICT_FB_DONE;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );

	GLMSG::SNET_GM_USER_RESTRICT_PROC_FLD NetMsgFld;
	NetMsgFld.dwCharID = pCharTarget->m_dwCharID;
	NetMsgFld.bUserRestricted = pCharTarget->m_bUserFlagRestricted;
	SENDTOALLCHANNEL ( &NetMsgFld );

	if ( GetDBMan() && pCharGM )
	{
		char szCommand[1024];
		sprintf_s( szCommand, 1024, "UserRestrict usernum:[%u] charnum:[%u]%s type:%u", pCharTarget->m_dwUserID, pCharTarget->m_dwCharID, pCharTarget->m_szName, pNetMsg->bRestrict );

		gm_cmd_logs *pDBActionGMCommand = new gm_cmd_logs( pCharGM->m_dwClientID, pCharGM->m_dwUserID, pCharGM->m_dwUserLvl, pCharGM->m_dwCharID, pCharGM->m_szName, szCommand );
		GetDBMan()->AddJob ( pDBActionGMCommand );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  pCharGM->m_dwUserID, pCharGM->m_dwUserLvl, pCharGM->m_dwCharID, pCharGM->m_szName, szCommand );
	}
}

/* pvp club death match, Juver, 2020/11/26 */
BOOL GLAgentServer::MsgReqPVPClubDeathMatchReBirth ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	PGLCHARAG pChar = GetChar ( dwGaeaID );
	if ( !pChar )			return FALSE;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ *pNetMsg = (GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ *) nmg;

	SNATIVEID sLobbyMap = PVPClubDeathMatchAgent::GetInstance().m_sLobbyMap;
	DWORD dwLobbyGate = PVPClubDeathMatchAgent::GetInstance().m_dwLobbyGate;
	
	GLAGLandMan *pLandMan = GetByMapID ( sLobbyMap );
	if ( !pLandMan )
	{
		CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "GLAgentServer::MsgReqPVPClubDeathMatchReBirth invalid lobby map[%d/%d]", sLobbyMap.wMainID );
		GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "GLAgentServer::MsgReqPVPClubDeathMatchReBirth invalid lobby map[%d/%d]", sLobbyMap.wSubID );

		return FALSE;
	}

	PDXLANDGATE pLandGate = pLandMan->GetLandGateMan()->FindLandGate(dwLobbyGate);
	if ( !pLandGate )
	{
		CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "GLAgentServer::MsgReqPVPClubDeathMatchReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wMainID );
		GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "GLAgentServer::MsgReqPVPClubDeathMatchReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wSubID );

		return FALSE;
	}

	DWORD dwNextFieldSvr = GetFieldServer ( sLobbyMap );
	if ( dwNextFieldSvr==pChar->m_dwCurFieldSvr )
	{
		pChar->SetCurrentField ( pChar->m_dwCurFieldSvr, sLobbyMap );

		SENDTOFIELD ( dwClientID, nmg );
	}
	else
	{
		ReActionMoveFieldSvr ( dwClientID, dwGaeaID );

		GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_A2F_REVIVE_REQ_FIELD_OUT NetMsg;
		SENDTOFIELD ( dwClientID, &NetMsg );

		pChar->SetNextFieldSvr ( sLobbyMap, dwLobbyGate, D3DXVECTOR3(FLT_MAX,FLT_MAX,FLT_MAX), dwNextFieldSvr );
	}

	return TRUE;
}

/* pvp club death match, Juver, 2020/11/26 */
BOOL GLAgentServer::MsgReqPVPClubDeathMatchReBirthFB ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB *pNetMsg = (GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB *) nmg;

	SENDTOCLIENT ( dwClientID, nmg );

	return TRUE;
}

/* genchar pt, Juver, 2020/12/03 */
BOOL GLAgentServer::MsgGmGenCharParty ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )							return FALSE;
	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )	return FALSE;

	GLMSG::SNETPC_GM_GENCHAR_PARTY* pNetMsg = (GLMSG::SNETPC_GM_GENCHAR_PARTY*) nmg;
	
	GLPARTY* pParty = m_cPartyMan.GetParty ( pNetMsg->dwPartyID );
	if ( pParty )
	{
		GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
		GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();

		for ( ; iter!=iter_end; ++iter )
		{
			PGLCHARAG pGenCHAR = GetChar ( (*iter) );
			if ( !pGenCHAR ) continue;

			GLMSG::SNETPC_GM_GENCHAR_FB NetMsgFB;

			if ( !pGenCHAR )
			{
				NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_FAIL;
				SENDTOCLIENT ( dwClientID, &NetMsgFB );
				continue;
			}

			if ( pGenCHAR->m_dwCurFieldSvr==FIELDSERVER_MAX )
			{
				NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_TO_CONDITION;
				SENDTOCLIENT ( dwClientID, &NetMsgFB );
				continue;
			}

			if ( pGenCHAR->m_nChannel!=pMyChar->m_nChannel )
			{
				NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_CHANNEL;
				NetMsgFB.nChannel = pGenCHAR->m_nChannel;
				SENDTOCLIENT ( dwClientID, &NetMsgFB );
				continue;
			}

			/*instance disable move, Juver, 2018/07/13 */
			GLAGLandMan* plandman_current = GetByMapID( pMyChar->m_sCurMapID );
			if ( plandman_current && plandman_current->IsInstantMap() )
				continue;

			/*instance disable move, Juver, 2018/07/13 */
			GLAGLandMan* plandman_target = GetByMapID( pGenCHAR->m_sCurMapID );
			if ( plandman_target && plandman_target->IsInstantMap() )
				continue;

			// GM의 위치를 확인한다.
			GLMSG::SNETPC_GM_MOVE2CHAR_POS NetMsgCK;
			NetMsgCK.dwCOMMAND_CHARID = pGenCHAR->m_dwCharID;
			NetMsgCK.dwTO_CHARID = pMyChar->m_dwCharID;
			SENDTOFIELDSVR ( pMyChar->m_nChannel, pMyChar->m_dwCurFieldSvr, &NetMsgCK );

			NetMsgFB.emFB  = EMGM_MOVE2CHAR_FB_OK;

			SENDTOCLIENT ( dwClientID, &NetMsgFB );
		}
	}

	/*gm command logs, Juver, 2018/08/17 */
	if ( GetDBMan() && pMyChar )
	{
		char command[1024];
		sprintf_s( command, 1024, "genchar_party [%u] to map [%u/%u]", pNetMsg->dwPartyID, pMyChar->m_sCurMapID.wMainID, pMyChar->m_sCurMapID.wSubID );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command );
	}

	return TRUE;
}

/* play time system, Juver, 2021/01/26 */
void GLAgentServer::UpdatePlayTime()
{
	GLMSG::SNET_UPDATE_PLAY_TIME_A2F netMsgField;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &netMsgField );

	//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "GLAgentServer::UpdatePlayTime" );
}

/* gm command send item, Juver, 2021/02/14 */
void GLAgentServer::MsgGMSendItem( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )												return;
	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )	return;

	GLMSG::SNETPC_GM_COMMAND_C2A_SEND_ITEM* pNetMsg = (GLMSG::SNETPC_GM_COMMAND_C2A_SEND_ITEM*) nmg;

	GLMSG::SNETPC_GM_COMMAND_A2C_SEND_ITEM_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_GM_SEND_ITEM_FB_FAIL;
	
	PGLCHARAG pCharTarget = GetCharID ( pNetMsg->dwCharID );
	if ( !pCharTarget )
	{
		NetMsgFB.emFB = EMREQ_GM_SEND_ITEM_FB_INVALID_CHARACTER;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassSendItem.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_SEND_ITEM_FB_INVALID_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( pNetMsg->sidItem );
	if ( !pItemData )
	{
		NetMsgFB.emFB = EMREQ_GM_SEND_ITEM_FB_INVALID_ITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_A2F_SEND_ITEM NetMsgFld;
	NetMsgFld.dwCharID = pNetMsg->dwCharID;
	NetMsgFld.dwCharIDSender = pMyChar->m_dwCharID;
	NetMsgFld.sidItem = pNetMsg->sidItem;
	StringCchCopy( NetMsgFld.szPass, MAX_PATH, pNetMsg->szPass );

	SENDTOALLCHANNEL ( &NetMsgFld );

	/*gm command logs, Juver, 2018/08/18 */
	if ( GetDBMan() && pMyChar )
	{
		char command[1024];
		sprintf_s( command, 1024, "send_item to char[%u] %s Item:[%u/%u]%s", 
			pCharTarget->m_dwCharID, 
			pCharTarget->m_szName, 
			pItemData->sBasicOp.sNativeID.wMainID, 
			pItemData->sBasicOp.sNativeID.wSubID, 
			pItemData->GetName() );

		gm_cmd_logs *pdb_action = new gm_cmd_logs( pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command );
	}
}

/* gm command send item, Juver, 2021/02/14 */
void GLAgentServer::MsgGMSendItemFB ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_F2A_SEND_ITEM_FB *pNetMsg = (GLMSG::SNETPC_GM_COMMAND_F2A_SEND_ITEM_FB *) nmg;

	PGLCHARAG pCharSender = GetCharID ( pNetMsg->dwCharSender );
	if ( !pCharSender )				return;

	GLMSG::SNETPC_GM_COMMAND_A2C_SEND_ITEM_FB NetMsgFB;
	NetMsgFB.emFB = pNetMsg->emFB;;
	SENDTOCLIENT ( pCharSender->m_dwClientID, &NetMsgFB );

	if ( pNetMsg->emFB == EMREQ_GM_SEND_ITEM_FB_OK )
	{
		PGLCHARAG pCharReceiver = GetCharID ( pNetMsg->dwCharReceiver );
		if ( pCharReceiver )
		{
			//notify target
			GLMSG::SNETPC_GM_COMMAND_A2C_SEND_ITEM_NOTICE NetMsgNotice;
			NetMsgNotice.sidItem = pNetMsg->sidItem;
			StringCchCopy( NetMsgNotice.szName, CHAR_SZNAME, pCharSender->m_szName );
			SENDTOCLIENT ( pCharReceiver->m_dwClientID, &NetMsgNotice );
		}
	}
}

void GLAgentServer::InstanceClear()
{
	for ( UINT i=0; i<m_vecInstantMapId.size(); ++i )
	{
		DWORD dwMapID = m_vecInstantMapId[i];
		SNATIVEID sMapID(dwMapID);

		GLAGLandMan *pLandMan = GetByMapID ( sMapID );
		if ( pLandMan && pLandMan->IsInstantMap() )
		{
			//erase from land map container
			SAFE_DELETE ( pLandMan );
			m_pLandMan[sMapID.wMainID][sMapID.wSubID] = NULL;

			//erase from mapslist
			m_sMapList.EraseMapList( sMapID );

			//erase from map sub id container
			m_sInsertInstantMapId.DeleteInstantMapID( sMapID.wSubID );

			GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "InstanceClear map[%d/%d]", sMapID.wMainID, sMapID.wSubID );
		}
		else
		{
			GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "InstanceClear failed to find map[%d/%d]", sMapID.wMainID, sMapID.wSubID );
		}
	}
	
	//clear id container
	m_vecInstantMapId.clear();

}

/* game notice, Juver, 2021/06/11 */
void GLAgentServer::GetGameNotice()
{
	m_cGameNotice.ClearData();

	if ( !m_pDBMan )	return;

	GAME_NOTICE_VEC vecNotice;

	m_pDBMan->GetGameNotice( vecNotice );

	for ( size_t i=0; i<vecNotice.size(); ++i )
	{
		m_cGameNotice.AddData( vecNotice[i] );
	}

	GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "Loaded %d game notice", (int)vecNotice.size() );
}

/* game notice, Juver, 2021/06/12 */
void GLAgentServer::MsgGetGameNotice( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )												return;
	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )	return;

	GLMSG::SNETPC_GM_GAME_NOTICE_RELOAD* pNetMsg = (GLMSG::SNETPC_GM_GAME_NOTICE_RELOAD*) nmg;

	GLMSG::SNETPC_GM_GAME_NOTICE_RELOAD_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_GM_GAME_NOTICE_RELOAD_FB_FAIL;

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassGameNoticeReload.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_GAME_NOTICE_RELOAD_FB_INVALID_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}


	NetMsgFB.emFB = EMREQ_GM_GAME_NOTICE_RELOAD_FB_OK;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );

	GetGameNotice();

	/*gm command logs, Juver, 2018/08/18 */
	if ( GetDBMan() && pMyChar )
	{
		gm_cmd_logs *pdb_action = new gm_cmd_logs( pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, "game notice reload" );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, "game notice reload" );
	}
}


/* max level notice, Juver, 2021/07/26 */
BOOL GLAgentServer::MsgMaxLevelNotice( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_MAX_LEVEL_NOTICE_AGENT* pNetMsg = (GLMSG::SNETPC_MAX_LEVEL_NOTICE_AGENT*)nmg;

	GLMSG::SNETPC_MAX_LEVEL_NOTICE_CLIENT netMsgClient;
	StringCchCopy ( netMsgClient.szName, CHAR_SZNAME+1, pNetMsg->szName );
	netMsgClient.wLevel = pNetMsg->wLevel;
	GLAgentServer::GetInstance().SENDTOALLCLIENT( &netMsgClient );

	PGLCHARAG pChar = GetCharID ( pNetMsg->dwCharID );
	if ( pChar )
	{
		if ( GetDBMan() )
		{
			LogMaxLevel *pDBAction = new LogMaxLevel( pChar->m_dwClientID, pChar->m_dwUserID, pChar->m_dwCharID, pChar->m_szName, pNetMsg->wLevel );
			GetDBMan()->AddJob ( pDBAction );
		}
	}

	

	return TRUE;
}


void GLAgentServer::MsgGISReload( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	PGLCHARAG pMyChar = GetChar ( dwGaeaID );
	if ( !pMyChar )												return;
	if ( pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )	return;

	GLMSG::SNETPC_GM_GIS_RELOAD* pNetMsg = (GLMSG::SNETPC_GM_GIS_RELOAD*) nmg;

	GLMSG::SNETPC_GM_GIS_RELOAD_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_GM_GIS_RELOAD_FB_FAIL;

	if ( strcmp( pNetMsg->szPass, CGMCommandPass::GetInstance().strPassGISReload.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_GIS_RELOAD_FB_INVALID_PASS;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	NetMsgFB.emFB = EMREQ_GM_GIS_RELOAD_FB_OK;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );

	/*gm command logs, Juver, 2018/08/18 */
	if ( GetDBMan() && pMyChar )
	{
		gm_cmd_logs *pdb_action = new gm_cmd_logs( pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, "gis reload" );
		GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, "gis reload" );
	}
}

BOOL GLAgentServer::MsgReqPVPPBGReBirthFB ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB *pNetMsg = (GLMSG::SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB *) nmg;

	SENDTOCLIENT ( dwClientID, nmg );

	return TRUE;
}
