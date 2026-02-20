#include "stdafx.h"

#include "Psapi.h"
#pragma comment(lib, "Psapi.lib")

#include "./UserTypeDefine.h"

#include "./GLAgentServer.h"
#include "./GLItemLMT.h"

#include "../DbActionLogic.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "./GLTaxiStation.h"
#include "./GLClubDeathMatch.h"

/*pvp tyranny, Juver, 2017/08/24 */
#include "./GLPVPTyrannyAgent.h"

/*school wars, Juver, 2018/01/19 */
#include "./GLPVPSchoolWarsAgent.h"

/*pvp capture the flag, Juver, 2018/01/24 */
#include "./GLPVPCaptureTheFlagAgent.h"

#include "./PVPClubDeathMatchAgent.h"

#include "./PVPPartyBattleGroundsAgent.h"
#include "./PVPPartyBattleGroundsMsg.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#include "./VirtualizerSDK.h"

/*woe Arc Development 08-06-2024*/
#include "./GLPVPWoeAgent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	Note : 萄 憮幗 滲唳衛 贗塭檜樹 薑爾 滲唳 餌 奩艙.
//		睡衛, gate蒂 鱔 裘 檜翕衛 轎脾.
BOOL GLAgentServer::ReActionMoveFieldSvr(DWORD dwClientID, DWORD dwGaeaID)
{
	//	Note : 贗塭檜樹曖 雖樓 蝶鑒 葬撢.
	//
	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;

	NetMsgSkillBrd.dwID = dwGaeaID;
	NetMsgSkillBrd.emCrow = CROW_PC;
	for (int i = 0; i < SKILLFACT_SIZE; ++i)
	{
		NetMsgSkillBrd.bRESET[i] = true;
	}
	SENDTOCLIENT(dwClientID, &NetMsgSkillBrd);

	GLMSG::SNETPC_CURESTATEBLOW_BRD NetMsgBlowBrd;
	NetMsgBlowBrd.dwID = dwGaeaID;
	NetMsgBlowBrd.dwID = CROW_PC;
	NetMsgBlowBrd.dwCUREFLAG = DIS_ALL;
	SENDTOCLIENT(dwClientID, &NetMsgBlowBrd);

	GLMSG::SNETPC_QITEMFACT_END_BRD NetMsgQFactEndBrd;
	SENDTOCLIENT(dwClientID, &NetMsgQFactEndBrd);

	return TRUE;
}

// *****************************************************
// Desc: 萄憮幗 薑爾蒂 羹觼 蕾樓 薑爾 瞪歎(Agent->Field)
// *****************************************************
BOOL GLAgentServer::MsgReqFieldSvrCharChkFb(NET_MSG_GENERIC *nmg, DWORD _dwClientID, DWORD _dwGaeaID)
{
	GLMSG::SNETPC_FIELDSVR_CHARCHK_FB *pNetMsg = (GLMSG::SNETPC_FIELDSVR_CHARCHK_FB *)nmg;
	PGLCHARAG pChar = GetChar(pNetMsg->dwGaeaID);
	if (!pChar)
		return FALSE;

	//	Note : 羹觼脹 熱 LOG.
	//
	++pChar->m_dwFIELDCK_NUM;
	pChar->m_bFIELD_CK[pNetMsg->nChannel][pNetMsg->dwFIELDID] = true;
	if (pNetMsg->bExist)
		++pChar->m_dwFIELDCK_BEING;

	//	Note : 萄 羹觼 諫猿 罹睡 欽. 虜褶 寰賊 渠晦.
	//
	if (!pChar->IsFieldCheckComplete(&m_bFIELDSVR[0][0]))
		return TRUE;

	if (pChar->m_dwFIELDCK_BEING > 0)
	{
		//	萄憮幗煎 蕾樓檜 褒
		/*
		DEBUGMSG_WRITE( "Found same character in field server. %d removed exist character. ( name %s )",
			pChar->m_dwFIELDCK_BEING, pChar->m_szName );
		*/
	}

	//	Note : 議葛攪 儅撩嬪纂蒂 雖薑.
	//
	SNATIVEID sGenMapID = pChar->m_sStartMapID;
	DWORD dwGenGateID = pChar->m_dwStartGate;
	D3DXVECTOR3 vGenPos = pChar->m_vStartPos;

	//	盪濰脹 嬪纂陛 氈擊 唳辦 撲薑.
	if (pChar->m_sSaveMapID != NATIVEID_NULL())
	{
		if (GLAgentServer::GetInstance().GetFieldServer(sGenMapID) != FIELDSERVER_MAX)
		{
			sGenMapID = pChar->m_sSaveMapID;
			dwGenGateID = UINT_MAX;
			vGenPos = pChar->m_vSavePos;

			// MapList 縑 Restart flag 陛 撲薑腎橫 氈戲賊
			// 鬼薯瞳戲煎 衛濛雖薄戲煎 檜翕啪 脹棻.
			SMAPNODE *pGenMap = m_sMapList.FindMapNode(sGenMapID);
			if (!pGenMap)
			{
				DEBUGMSG_WRITE("Can't find MapNode in MapsList.");
			}
			else if (pGenMap->bRestart)
			{
				sGenMapID = pChar->m_sStartMapID;
				dwGenGateID = pChar->m_dwStartGate;
				vGenPos = D3DXVECTOR3(0, 0, 0);
			}
		}
	}

	// 霜瞪縑 謙猿 裘檜 摹紫瞪 裘檜賊 摹紫贗毀檜 嬴棍唳辦 衛濛裘戲煎 雖薑
	GLAGLandMan *pLAND = GetByMapID(sGenMapID);
	if (pLAND)
	{
		if (pLAND->m_bGuidBattleMap)
		{
			GLGuidance *pGUID = GLGuidanceAgentMan::GetInstance().Find(pLAND->m_dwClubMapID);
			if (pGUID && pGUID->m_dwGuidanceClub != pChar->m_dwGuild)
			{
				sGenMapID = pChar->m_sStartMapID;
				dwGenGateID = pChar->m_dwStartGate;
			}
		}

		if (pLAND->m_bClubDeathMatchMap)
		{
			GLClubDeathMatch *pCDM = GLClubDeathMatchAgentMan::GetInstance().Find(pLAND->m_dwClubMapID);

			if (pCDM)
			{
				sGenMapID = pCDM->m_dwClubHallMap;
				dwGenGateID = pCDM->m_dwClubHallGate;
			}
		}

		/*pvp tyranny, Juver, 2017/08/24 */
		if (pLAND->m_bPVPTyrannyMap)
		{
			sGenMapID = GLPVPTyrannyAgent::GetInstance().m_sOutMap;
			dwGenGateID = GLPVPTyrannyAgent::GetInstance().m_wOutGate;
		}

		/*school wars, Juver, 2018/01/19 */
		if (pLAND->m_bPVPSchoolWarsMap)
		{
			sGenMapID = GLPVPSchoolWarsAgent::GetInstance().m_sOutMap;
			dwGenGateID = GLPVPSchoolWarsAgent::GetInstance().m_wOutGate;
		}

		/*pvp capture the flag, Juver, 2018/01/31 */
		if (pLAND->m_bPVPCaptureTheFlagMap)
		{
			sGenMapID = GLPVPCaptureTheFlagAgent::GetInstance().m_sOutMap;
			dwGenGateID = GLPVPCaptureTheFlagAgent::GetInstance().m_wOutGate;
		}

		/* pvp club death match, Juver, 2020/11/25 */
		if (pLAND->m_bPVPClubDeathMatchLobbyMap || pLAND->m_bPVPClubDeathMatchBattleMap)
		{
			sGenMapID = PVPClubDeathMatchAgent::GetInstance().m_sOutMap;
			dwGenGateID = PVPClubDeathMatchAgent::GetInstance().m_dwOutGate;
		}

		if (pLAND->m_bPVPPBGLobbyMap || pLAND->m_bPVPPBGBattleMap)
		{
			sGenMapID = PVPPBG::ManagerAgent::GetInstance().m_sOutMap;
			dwGenGateID = PVPPBG::ManagerAgent::GetInstance().m_dwOutGate;
		}

		/*woe Arc Development 08-06-2024*/
		if (pLAND->m_bPVPWoeMap)
		{
			sGenMapID = GLPVPWoeAgent::GetInstance().m_sOutMap;
			dwGenGateID = GLPVPWoeAgent::GetInstance().m_wOutGate;
		}
	}

	//	Note : 萄憮幗 薑爾蒂 陛螳諦憮 萄憮幗煎 蕾樓
	//
	bool bCANNOTFINDMAP(false);
	DWORD dwFieldServer = GLAgentServer::GetInstance().GetFieldServer(sGenMapID);
	if (dwFieldServer == FIELDSERVER_MAX)
	{
		bCANNOTFINDMAP = true;

		//	議葛攪陛 儅撩腆 萄憮幗蒂 瓊雖 跤. 斜楝釭 霞殮 熱 氈紫煙 蟾晦 ( 議葛攪滌 掖曖 0廓 啪檜 )
		DEBUGMSG_WRITE("Can't find field server associate with character (MID[%d/%d], CID[%d] name %s)",
					   sGenMapID.wMainID, sGenMapID.wSubID, pChar->m_dwClientID, pChar->m_szName);

		// DBAction 濛機 部釭賊 夥煎 薯剪堅 纔蝶 塭
		// return FALSE;

		sGenMapID = GLCONST_CHAR::nidSTARTMAP[pChar->m_wSchool];
		dwGenGateID = GLCONST_CHAR::dwSTARTGATE[pChar->m_wSchool];

		GLAGLandMan *pLandMan = GetByMapID(sGenMapID);
		if (!pLandMan)
		{
			DEBUGMSG_WRITE("Can't find StartMap ID.");
			return FALSE;
		}

		DxLandGateMan *pGateMan = pLandMan->GetLandGateMan();
		if (!pGateMan)
		{
			DEBUGMSG_WRITE("Can't Find LandGateManager.");
			return FALSE;
		}
		PDXLANDGATE pGate = pGateMan->FindLandGate(dwGenGateID);
		if (!pGate)
		{
			DEBUGMSG_WRITE("Can't Find LandGate.");
			vGenPos = D3DXVECTOR3(0, 0, 0);
		}
		else
		{
			vGenPos = pGate->GetGenPos(DxLandGate::GEN_RENDUM);
		}

		dwFieldServer = GLAgentServer::GetInstance().GetFieldServer(sGenMapID);
		if (dwFieldServer == FIELDSERVER_MAX)
		{
			DEBUGMSG_WRITE("Can't Find FieldServer associate with character'school.");
			return FALSE;
		}

		pChar->m_sStartMapID = sGenMapID;
		pChar->m_dwStartGate = dwGenGateID;
		pChar->m_vStartPos = vGenPos;
		pChar->m_sSaveMapID = sGenMapID;
		pChar->m_vSavePos = vGenPos;

		// 檜睡碟縑憮 滲唳脹 蝶顫雖薄(MapID, GateID, 嬪纂) 擊 DB縑 盪濰
		CSaveChaPos *pDBAction = new CSaveChaPos(pChar->m_dwClientID,
												 pChar->m_dwCharID,
												 dwFieldServer,
												 pChar->m_sStartMapID.dwID,
												 pChar->m_dwStartGate,
												 pChar->m_vStartPos.x,
												 pChar->m_vStartPos.y,
												 pChar->m_vStartPos.z,
												 pChar->m_sSaveMapID.dwID,
												 pChar->m_vSavePos.x,
												 pChar->m_vSavePos.y,
												 pChar->m_vSavePos.z,
												 pChar->m_sSaveMapID.dwID,
												 pChar->m_vSavePos.x,
												 pChar->m_vSavePos.y,
												 pChar->m_vSavePos.z);

		GLDBMan *pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if (pDbMan)
			pDbMan->AddJob(pDBAction);
	}

	// 虜擒 裘擊 跤 瓊懊棻賊 裘擊 蟾晦 堅 棻艇 夠縑憮 議葛攪 褻檣薑爾蒂 瞪歎棻
	if (bCANNOTFINDMAP)
		return true;

	if (m_pMsgServer->ConnectFieldSvr(pChar->m_dwClientID, dwFieldServer, pChar->m_dwGaeaID, pChar->m_nChannel) != NET_OK)
	{
		//	議葛攪陛 儅撩腆 萄憮幗煎 蕾樓檜 褒
		DEBUGMSG_WRITE("Can't connect field. FIELDID : %d, CID[%d] name %s",
					   dwFieldServer, pChar->m_dwClientID, pChar->m_szName);
		return FALSE;
	}

	//	議葛攪 褻檣 薑爾 瞪歎
	NET_GAME_JOIN_FIELDSVR NetJoinField;
	NetJoinField.emType = EMJOINTYPE_FIRST;
	NetJoinField.dwSlotAgentClient = pChar->m_dwClientID;
	StringCchCopy(NetJoinField.szUID, DAUM_MAX_UID_LENGTH + 1, pChar->m_szUserName);
	NetJoinField.nUserNum = pChar->m_dwUserID;
	NetJoinField.dwUserLvl = pChar->m_dwUserLvl;
	NetJoinField.nChaNum = pChar->m_dwCharID;
	NetJoinField.dwGaeaID = pChar->m_dwGaeaID;
	NetJoinField.tPREMIUM = pChar->m_tPREMIUM;
	NetJoinField.tCHATBLOCK = pChar->m_tCHATBLOCK;

	NetJoinField.sStartMap = sGenMapID;
	NetJoinField.dwStartGate = dwGenGateID;
	NetJoinField.vStartPos = vGenPos;

	NetJoinField.dwActState = pChar->m_dwActState;
	NetJoinField.bUseArmSub = pChar->m_bUseArmSub;

	NetJoinField.dwThaiCCafeClass = pChar->m_dwThaiCCafeClass;
	NetJoinField.nMyCCafeClass = pChar->m_nMyCCafeClass; // 蜓溯檜衛嬴 PC寞 檜漸

	NetJoinField.sChinaTime = pChar->m_sChinaTime;
	NetJoinField.sEventTime = pChar->m_sEventTime;
	NetJoinField.sVietnamGainSystem = pChar->m_sVietnamSystem;

	/* user flag verified, Juver, 2020/02/25 */
	NetJoinField.bUserFlagVerified = pChar->m_bUserFlagVerified;

	/*dmk14 ingame web*/
	NetJoinField.dwPPoints = pChar->m_dwPPoints;
	NetJoinField.dwVPoints = pChar->m_dwVPoints;

	/* user flag restricted, Juver, 2020/04/20 */
	NetJoinField.bUserFlagRestricted = pChar->m_bUserFlagRestricted;

	//** Add EventTime

	SENDTOFIELD(pChar->m_dwClientID, &NetJoinField);

	//	Note : 馨葛攪曖 營 蕾樓 萄憮幗 塽 營 裘 ID 雖薑.
	//
	pChar->SetCurrentField(dwFieldServer, sGenMapID);

	return TRUE;
}

BOOL GLAgentServer::MsgReqGenItemAgt(NET_MSG_GENERIC *nmg, DWORD dwClientID)
{
	GLMSG::SNET_REQ_GENITEM_AGT *pNetMsg = (GLMSG::SNET_REQ_GENITEM_AGT *)nmg;

	int nCHANNEL = pNetMsg->nCHANNEL;

	const SNATIVEID &sNID = pNetMsg->sNID;
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNID);
	if (!pITEM)
		return FALSE;

	SMAPNODE *pMAPNODE = m_sMapList.FindMapNode(pNetMsg->sMAPID);
	if (!pMAPNODE)
		return FALSE;

	int nSvrNum = (int)pMAPNODE->dwFieldSID;

	if (GLITEMLMT::GetInstance().DoCheckItemGen(sNID))
	{
		LONGLONG lnGenCount = GLITEMLMT::GetInstance().RegItemGen(sNID, pNetMsg->emGENTYPE);

		GLMSG::SNET_REQ_GENITEM_FLD_FB NetMsgFb;
		NetMsgFb.sNID = pNetMsg->sNID;
		NetMsgFb.sMAPID = pNetMsg->sMAPID;
		NetMsgFb.emGENTYPE = pNetMsg->emGENTYPE;
		NetMsgFb.lnGENCOUNT = lnGenCount;
		NetMsgFb.emHoldGroup = pNetMsg->emHoldGroup;
		NetMsgFb.dwHoldGID = pNetMsg->dwHoldGID;
		NetMsgFb.vPos = pNetMsg->vPos;

		SENDTOFIELDSVR(nCHANNEL, nSvrNum, &NetMsgFb);
	}

	return TRUE;
}

BOOL GLAgentServer::SendToGMMsg(PGLCHARAG pChar, NET_CHAT_FB NetChatFB)
{
	if (!pChar)
		return FALSE;

	UINT i;
	for (i = 0; i < pChar->m_vecGetWhisperList.size(); i++)
	{
		PGLCHARAG pSendGMChar = GetChar(pChar->m_vecGetWhisperList[i].dwGaeaID);
		if (pSendGMChar == NULL ||
			pSendGMChar->m_GmWhisperList.dwClientID != pChar->m_dwClientID ||
			pSendGMChar->m_GmWhisperList.dwGaeaID != pChar->m_dwGaeaID ||
			pSendGMChar->m_bViewWhisperMSG == FALSE)
		{
			pChar->m_vecGetWhisperList.erase(pChar->m_vecGetWhisperList.begin() + i);
			i--;
			continue;
		}
		SENDTOCLIENT(pChar->m_vecGetWhisperList[i].dwClientID, &NetChatFB);
	}

	return TRUE;
}

BOOL GLAgentServer::ChatMsgProc(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	NET_CHAT *pNetMsg = (NET_CHAT *)nmg;

	BOOL bEmpty = FALSE;

#if defined(BUILD_CH) || defined(BUILD_CHY)
	VIRTUALIZER_FISH_WHITE_START
	if (pNetMsg->emType == CHAT_TYPE_NORMAL || pNetMsg->emType == CHAT_TYPE_PARTY || pNetMsg->emType == CHAT_TYPE_PRIVATE)
	{
		bEmpty = m_cWTF.CheckMsg(pNetMsg->szName, pNetMsg->szChatMsg);
	}
	VIRTUALIZER_FISH_WHITE_END
#endif // BUILD_CH

	if (bEmpty)
		return TRUE;

	switch (pNetMsg->emType)
	{
	case CHAT_TYPE_GLOBAL: // 婦葬辨 旋煎弊 詭衛雖
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
			return FALSE;

		NET_CHAT_CTRL_FB NetChatFB;
		NetChatFB.emType = CHAT_TYPE_GLOBAL;
		NetChatFB.sItemLink = pNetMsg->sItemLink;							 /*item link, Juver, 2017/07/31 */
		StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName); /*staff name in chat, Juver, 2017/12/26 */
		StringCchCopy(NetChatFB.szChatMsg, GLOBAL_CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);

		// add staffchat
		/* Chat Color, Mhundz 02/22/25 */
		if (pChar->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_MASTER)
		{
			NetChatFB.bStaff = true;
			StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pNetMsg->szName);
		}
		AGCHARNODE *pCharNode = m_PCList.m_pHead;
		for (; pCharNode; pCharNode = pCharNode->pNext)
		{
			SENDTOCLIENT(pCharNode->Data->m_dwClientID, &NetChatFB);
		}

		/*gm command logs, Juver, 2018/08/16 */
		if (GetDBMan() && pChar)
		{
			char command[1024];
			sprintf_s(command, 1024, "chat %s", pNetMsg->szChatMsg);
			gm_cmd_logs *pdb_action = new gm_cmd_logs(pChar->m_dwClientID, pChar->m_dwUserID, pChar->m_dwUserLvl, pChar->m_dwCharID, pChar->m_szName, command);
			GetDBMan()->AddJob(pdb_action);

			CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pChar->m_dwUserID, pChar->m_dwUserLvl, pChar->m_dwCharID, pChar->m_szName, command);
		}
	}
	break;

	case CHAT_TYPE_NORMAL: // 橾奩詭衛雖
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->IsCHATBLOCK())
		{
			GLMSG::SNETPC_CHAT_BLOCK NetChatBlock;
			StringCchCopy(NetChatBlock.szName, USR_ID_LENGTH + 1, pChar->m_szUserName);
			NetChatBlock.tChatBlock = pChar->m_tCHATBLOCK;

			SENDTOCLIENT(pChar->m_dwClientID, &NetChatBlock);

			return FALSE;
		}

		GLAGLandMan *pLandMan = GetByMapID(pChar->m_sCurMapID);
		if (pLandMan && pLandMan->isBlockGeneralChat())
		{
			GLMSG::SNETPC_CHAT_GENERAL_FAIL NetMsgFail;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFail);
			return FALSE;
		}

		//	萄 憮幗煎 瞪歎.
		SENDTOFIELD(pChar->m_dwClientID, nmg);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pChar->m_szUserName, "**Normal Send**, [%s][%s], %s", pChar->m_szUserName, pChar->m_szName, pNetMsg->szChatMsg);
		}
#endif
	}
	break;

	case CHAT_TYPE_PARTY: // 詭衛雖
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->m_dwPartyID == PARTY_NULL)
			return FALSE;
		if (pChar->IsCHATBLOCK())
		{
			GLMSG::SNETPC_CHAT_BLOCK NetChatBlock;
			StringCchCopy(NetChatBlock.szName, USR_ID_LENGTH + 1, pChar->m_szUserName);
			NetChatBlock.tChatBlock = pChar->m_tCHATBLOCK;

			SENDTOCLIENT(pChar->m_dwClientID, &NetChatBlock);

			return FALSE;
		}

		NET_CHAT_FB NetChatFB;
		NetChatFB.emType = pNetMsg->emType;
		NetChatFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
		StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName);
		StringCchCopy(NetChatFB.szChatMsg, CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);

		SENDTOPARTY(pChar->m_dwGaeaID, pChar->m_dwPartyID, (NET_MSG_GENERIC *)&NetChatFB);

		// 濠褐曖 詭衛雖蒂 爾鹵
		SendToGMMsg(pChar, NetChatFB);
		//// 鼻渠寞曖 詭衛雖蒂 爾鹵
		// SendToGMMsg( GetChar(pChar->m_dwGaeaID), NetChatFB );

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print

		GLPARTY *pParty = m_cPartyMan.GetParty(pChar->m_dwPartyID);
		if (pParty && pParty->ISVAILD())
		{
			GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
			GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();

			PGLCHARAG pMemChar = NULL;
			for (; iter != iter_end; ++iter)
			{
				pMemChar = GLAgentServer::GetInstance().GetChar((*iter));
				if (pMemChar && pMemChar->m_bTracingUser)
				{
					if (pMemChar->m_dwClientID == pChar->m_dwClientID)
						continue;

					CDebugSet::ToTracingFile(pMemChar->m_szUserName, "##Party Recv##, [%s][%s]->[%s][%s], %s",
											 pChar->m_szUserName, pChar->m_szName, pMemChar->m_szUserName, pMemChar->m_szName, pNetMsg->szChatMsg);
				}
			}
		}
		if (pChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pChar->m_szUserName, "##Party Send##, [%s][%s], %s",
									 pChar->m_szUserName, pChar->m_szName, pNetMsg->szChatMsg);
		}
#endif
	}
	break;

	case CHAT_TYPE_PRIVATE: // 偃檣詭衛雖
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->IsCHATBLOCK())
		{
			GLMSG::SNETPC_CHAT_BLOCK NetChatBlock;
			StringCchCopy(NetChatBlock.szName, USR_ID_LENGTH + 1, pChar->m_szUserName);
			NetChatBlock.tChatBlock = pChar->m_tCHATBLOCK;

			SENDTOCLIENT(pChar->m_dwClientID, &NetChatBlock);

			return FALSE;
		}

		CHAR_MAP_ITER iterChar = m_PCNameMap.find(std::string(pNetMsg->szName));
		if (iterChar == m_PCNameMap.end())
		{
			//	敏樓蜓曖 渠鼻檜 橈擊陽 FB 詭衛雖.
			GLMSG::SNETPC_CHAT_PRIVATE_FAIL NetMsgFail;
			StringCchCopy(NetMsgFail.szName, CHR_ID_LENGTH + 1, pNetMsg->szName);

			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFail);

			return FALSE;
		}

		//	鼻渠寞縑啪 濠晦 濠褐檜 敏樓蜓 綰楣腎橫 氈朝雖 匐餌.
		PGLCHARAG pCHAR_TAR = (*iterChar).second;
		int dwFR_FLAG = pCHAR_TAR->GetStateFriend(pChar->m_szName);

		if ((dwFR_FLAG == EMFRIEND_BLOCK) && (pChar->m_dwUserLvl > NSUSER_TYPE::USER_TYPE_GM3))
		{
			dwFR_FLAG = EMFRIEND_OFF;
		}

		if (dwFR_FLAG == EMFRIEND_BLOCK)
		{
			//	渡 餌塋縑啪 綰楣 腎歷擊陽.
			//	敏樓蜓曖 渠鼻檜 橈擊陽 FB 詭衛雖.
			GLMSG::SNETPC_CHAT_PRIVATE_FAIL NetMsgFail;
			StringCchCopy(NetMsgFail.szName, CHR_ID_LENGTH + 1, pNetMsg->szName);

			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFail);
		}
		else
		{
			NET_CHAT_FB NetChatFB;
			NetChatFB.emType = pNetMsg->emType;
			NetChatFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
			StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName);
			StringCchCopy(NetChatFB.szChatMsg, CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);

			PGLCHARAG pGLChar = (*iterChar).second;
			SENDTOCLIENT(pGLChar->m_dwClientID, &NetChatFB);

			// 濠褐曖 詭衛雖蒂 爾鹵
			SendToGMMsg(pChar, NetChatFB);
			// 鼻渠寞曖 詭衛雖蒂 爾鹵
			SendToGMMsg(pGLChar, NetChatFB);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print

			if (pChar->m_bTracingUser)
			{
				CDebugSet::ToTracingFile(pChar->m_szUserName, "@@Private Send@@, [%s][%s]->[%s][%s], %s",
										 pChar->m_szUserName, pChar->m_szName, pGLChar->m_szUserName, pGLChar->m_szName, pNetMsg->szChatMsg);
			}
			if (pGLChar->m_bTracingUser)
			{
				CDebugSet::ToTracingFile(pGLChar->m_szUserName, "@@Private Recv@@, [%s][%s]<-[%s][%s], %s",
										 pGLChar->m_szUserName, pGLChar->m_szName, pChar->m_szUserName, pChar->m_szName, pNetMsg->szChatMsg);
			}
#endif
		}
	}
	break;

	case CHAT_TYPE_GUILD: // 望萄詭衛雖
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->m_dwGuild == CLUB_NULL)
			return FALSE;
		if (pChar->IsCHATBLOCK())
		{
			GLMSG::SNETPC_CHAT_BLOCK NetChatBlock;
			StringCchCopy(NetChatBlock.szName, USR_ID_LENGTH + 1, pChar->m_szUserName);
			NetChatBlock.tChatBlock = pChar->m_tCHATBLOCK;

			SENDTOCLIENT(pChar->m_dwClientID, &NetChatBlock);

			return FALSE;
		}

		GLCLUB *pCLUB = m_cClubMan.GetClub(pChar->m_dwGuild);
		if (!pCLUB)
			return FALSE;

		CLUBMEMBERS_ITER pos = pCLUB->m_mapMembers.begin();
		CLUBMEMBERS_ITER end = pCLUB->m_mapMembers.end();
		for (; pos != end; ++pos)
		{
			PGLCHARAG pMEMBER = GetCharID((*pos).first);
			if (!pMEMBER)
				continue;

			NET_CHAT_FB NetChatFB;
			NetChatFB.emType = pNetMsg->emType;
			NetChatFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
			StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName);
			StringCchCopy(NetChatFB.szChatMsg, CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);

			SENDTOCLIENT(pMEMBER->m_dwClientID, &NetChatFB);

			// 鼻渠寞曖 詭衛雖蒂 爾鹵
			SendToGMMsg(pMEMBER, NetChatFB);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
			if (pMEMBER->m_bTracingUser)
			{
				CDebugSet::ToTracingFile(pMEMBER->m_szUserName, "%%Guild Recv%%, [%s][%s]->[%s][%s], %s",
										 pChar->m_szUserName, pChar->m_szName, pMEMBER->m_szUserName, pMEMBER->m_szName, pNetMsg->szChatMsg);
			}
#endif
		}

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pChar->m_szUserName, "%%Guild Send%%, [%s][%s], %s", pChar->m_szUserName, pChar->m_szName, pNetMsg->szChatMsg);
		}
#endif

		NET_CHAT_FB NetChatFB;
		NetChatFB.emType = pNetMsg->emType;
		NetChatFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
		StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName);
		StringCchCopy(NetChatFB.szChatMsg, CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);

		// 濠褐曖 詭衛雖蒂 爾鹵
		// SendToGMMsg( pChar, NetChatFB );
	}
	break;

	case CHAT_TYPE_ALLIANCE:
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->m_dwGuild == CLUB_NULL)
			return FALSE;
		if (pChar->IsCHATBLOCK())
		{
			GLMSG::SNETPC_CHAT_BLOCK NetChatBlock;
			StringCchCopy(NetChatBlock.szName, USR_ID_LENGTH + 1, pChar->m_szUserName);
			NetChatBlock.tChatBlock = pChar->m_tCHATBLOCK;

			SENDTOCLIENT(pChar->m_dwClientID, &NetChatBlock);

			return FALSE;
		}

		GLCLUB *pCLUB = m_cClubMan.GetClub(pChar->m_dwGuild);
		if (!pCLUB)
			return FALSE;
		if (pCLUB->m_dwAlliance == CLUB_NULL)
			return FALSE;

		GLCLUB *pCLUB_CHIEF = m_cClubMan.GetClub(pCLUB->m_dwAlliance);
		if (!pCLUB_CHIEF)
			return FALSE;

		CLUB_ALLIANCE_ITER pos_alliance = pCLUB_CHIEF->m_setAlliance.begin();
		CLUB_ALLIANCE_ITER end_alliance = pCLUB_CHIEF->m_setAlliance.end();
		for (; pos_alliance != end_alliance; ++pos_alliance)
		{
			const GLCLUBALLIANCE &sALLIANCE = (*pos_alliance);
			GLCLUB *pCLUB_INDIAN = m_cClubMan.GetClub(sALLIANCE.m_dwID);
			if (!pCLUB_INDIAN)
				continue;

			CLUBMEMBERS_ITER pos = pCLUB_INDIAN->m_mapMembers.begin();
			CLUBMEMBERS_ITER end = pCLUB_INDIAN->m_mapMembers.end();
			for (; pos != end; ++pos)
			{
				PGLCHARAG pMEMBER = GetCharID((*pos).first);
				if (!pMEMBER)
					continue;

				NET_CHAT_FB NetChatFB;
				NetChatFB.emType = pNetMsg->emType;
				NetChatFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
				StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName);
				StringCchCopy(NetChatFB.szChatMsg, CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);

				SENDTOCLIENT(pMEMBER->m_dwClientID, &NetChatFB);

				SendToGMMsg(pMEMBER, NetChatFB);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
				if (pMEMBER->m_bTracingUser)
				{
					CDebugSet::ToTracingFile(pMEMBER->m_szUserName, "**Alliance Recv**, [%s][%s]->[%s][%s], %s",
											 pChar->m_szUserName, pChar->m_szName, pMEMBER->m_szUserName, pMEMBER->m_szName, pNetMsg->szChatMsg);
				}
#endif
			}
		}

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pChar->m_szUserName, "**Alliance Send**, [%s][%s], %s", pChar->m_szUserName, pChar->m_szName, pNetMsg->szChatMsg);
		}
#endif
	}
	break;

		/*regional chat, Juver, 2017/12/06 */
	case CHAT_TYPE_REGIONAL:
	{
		PGLCHARAG pChar = GetChar(dwGaeaID);
		if (!pChar)
			return FALSE;
		if (pChar->IsCHATBLOCK())
		{
			GLMSG::SNETPC_CHAT_BLOCK NetChatBlock;
			StringCchCopy(NetChatBlock.szName, USR_ID_LENGTH + 1, pChar->m_szUserName);
			NetChatBlock.tChatBlock = pChar->m_tCHATBLOCK;
			SENDTOCLIENT(pChar->m_dwClientID, &NetChatBlock);
			return FALSE;
		}

		/*pvp capture the flag, Juver, 2018/02/08 */
		BOOL bCaptureTheFlag = FALSE;
		GLAGLandMan *pCurLandMan = GetByMapID(pChar->m_sCurMapID);
		if (pCurLandMan)
			bCaptureTheFlag = pCurLandMan->m_bPVPCaptureTheFlagMap;

		AGCHARNODE *pCharNode = m_PCList.m_pHead;
		for (; pCharNode; pCharNode = pCharNode->pNext)
		{
			if (!pCharNode->Data)
				continue;
			if (pCharNode->Data->m_sCurMapID.dwID != pChar->m_sCurMapID.dwID)
				continue;

			if (bCaptureTheFlag)
			{
				if (pCharNode->Data->m_wCaptureTheFlagTeam != pChar->m_wCaptureTheFlagTeam)
					continue;
			}
			else
			{
				if (pCharNode->Data->m_wSchool != pChar->m_wSchool)
					continue;
			}

			NET_CHAT_FB NetChatFB;
			NetChatFB.emType = pNetMsg->emType;
			NetChatFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
			StringCchCopy(NetChatFB.szName, CHR_ID_LENGTH + 1, pChar->m_szName);
			StringCchCopy(NetChatFB.szChatMsg, CHAT_MSG_SIZE + 1, pNetMsg->szChatMsg);
			SENDTOCLIENT(pCharNode->Data->m_dwClientID, &NetChatFB);

			SendToGMMsg(pCharNode->Data, NetChatFB);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
			if (pCharNode->Data && pCharNode->Data->m_bTracingUser)
			{
				CDebugSet::ToTracingFile(pCharNode->Data->m_szUserName, "**Regional Recv**, [%s][%s]->[%s][%s], %s",
										 pChar->m_szUserName, pChar->m_szName, pCharNode->Data->m_szUserName, pCharNode->Data->m_szName, pNetMsg->szChatMsg);
			}
#endif
		}

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pChar->m_szUserName, "**Regional Send**, [%s][%s], %s", pChar->m_szUserName, pChar->m_szName, pNetMsg->szChatMsg);
		}
#endif
	}
	break;

	default:
		break;
	}

	return TRUE;
}

BOOL GLAgentServer::MsgReqCreateInstantMap(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETREQ_CREATE_INSTANT_MAP_REQ *pNetMsg = (GLMSG::SNETREQ_CREATE_INSTANT_MAP_REQ *)nmg;
	GLMSG::SNETREQ_CREATE_INSTANT_MAP_FB NetMsgFB;

	PGLCHARAG pChar = GetChar(pNetMsg->dwGaeaID);
	if (!pChar)
		return FALSE;

	//	Note : 營 裘 薑爾.
	//
	GLAGLandMan *pCurLandMan = GetByMapID(pChar->m_sCurMapID);
	if (!pCurLandMan)
	{
		CDebugSet::ToLogFile("EMCHAR_CREATE_INSTANT_MAP_FAIL_MAPID %d [%d/%d]",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID);

		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_MAPID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 營 啪檜
	DxLandGateMan *pCurGateMan = pCurLandMan->GetLandGateMan();
	PDXLANDGATE pCurGate = pCurGateMan->FindLandGate(pNetMsg->dwGateID);
	if (!pCurGate)
	{
		CDebugSet::ToLogFile("EMCHAR_CREATE_INSTANT_MAP_FAIL_GATEID %d [%d/%d] %d %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pNetMsg->dwGateID,
							 pNetMsg->dwToIndex);

		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_GATEID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	/* multi gate out, Juver, 2020/11/16 */
	DWORD dwToIndex = pNetMsg->dwToIndex;
	if (dwToIndex >= DxLandGate::MAX_GATE_OUT)
	{
		CDebugSet::ToLogFile("EMCHAR_CREATE_INSTANT_MAP_FAIL_TO_INDEX %d [%d/%d] %d %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pNetMsg->dwGateID,
							 pNetMsg->dwToIndex);

		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_TO_INDEX;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 跡 裘.
	//
	GLAGLandMan *pNextLandMan = GetByMapID(pCurGate->GetToMapID(dwToIndex));
	if (!pNextLandMan)
	{
		CDebugSet::ToLogFile("EMCHAR_CREATE_INSTANT_MAP_FAIL_TARMAPID %d [%d/%d] %d [%d/%d] %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pCurGate->GetGateID(),
							 pCurGate->GetToMapID(dwToIndex).wMainID,
							 pCurGate->GetToMapID(dwToIndex).wSubID,
							 dwToIndex);

		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_TARMAPID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 跡 啪檜.
	//
	DxLandGateMan *pNextGateMan = pNextLandMan->GetLandGateMan();
	PDXLANDGATE pNextGate = pNextGateMan->FindLandGate(pCurGate->GetToGateID(dwToIndex));
	if (!pNextGate)
	{
		CDebugSet::ToLogFile("EMCHAR_CREATE_INSTANT_MAP_FAIL_TARMAPID %d [%d/%d] %d [%d/%d] %d %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pNetMsg->dwGateID,
							 pCurGate->GetToMapID(dwToIndex).wMainID,
							 pCurGate->GetToMapID(dwToIndex).wSubID,
							 pCurGate->GetToGateID(dwToIndex),
							 dwToIndex);

		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_TARMAPID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	SMAPNODE *pMapNode = m_sMapList.FindMapNode(pCurGate->GetToMapID(dwToIndex));
	if (!pMapNode->bInstantMap)
	{
		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_NOT;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	DWORD dwNextFieldSvr = GetFieldServer(pCurGate->GetToMapID(dwToIndex));
	if (pChar->m_dwCurFieldSvr != dwNextFieldSvr)
	{
		NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*m_sTestInstantMapCharInfo.dwGaeaID	= pNetMsg->dwGaeaID;
	m_sTestInstantMapCharInfo.sBaseMapID = pCurGate->GetToMapID();

	return TRUE;*/

	GLMSG::SNETREQ_CREATE_INSTANT_MAP_FLD NetMsgFld;
	SNATIVEID sInstantMapID;

	// 檣蝶欐 湍瞪擊 儅撩晦瞪縑 薑蜓 儅撩擊 撿朝雖 罹睡蒂 廓渦 欽棻.
	if (pNetMsg->bCreate)
	{
		/*VEC_INSTANT_MAPID_ITER iter = m_vecInstantMapId.begin();
		bool  bEnterInstantMap = FALSE;
		for( ; iter != m_vecInstantMapId.end(); ++iter )
		{
			bEnterInstantMap = FALSE;

			SNATIVEID sNativeID = *iter;
			GLAGLandMan* pLandMan = GetByMapID( sNativeID );
			if( !pLandMan ) continue;

			if( pLandMan->GetMapID().wMainID == pCurGate->GetToMapID().wMainID && !pLandMan->IsInstantMap() ) continue;

			if( pLandMan->IsPartyInstantMap() )
			{
				if( pLandMan->GetInstantMapHostID() == pChar->GetPartyID() )
					bEnterInstantMap = TRUE;
			}else if( pChar->GetPartyID() == PARTY_NULL ){
				if( pLandMan->GetInstantMapHostID() == pChar->m_dwGaeaID )
					bEnterInstantMap = TRUE;
			}

			if( bEnterInstantMap )
			{
				pNetMsg->bCreate	   = FALSE;
				pNetMsg->sInstantMapID = sNativeID;
				break;
			}
		}*/
	}

	if (pNetMsg->bCreate)
	{

		sInstantMapID.wMainID = pCurGate->GetToMapID(dwToIndex).wMainID;
		sInstantMapID.wSubID = m_sInsertInstantMapId.InsertInduMapID();
		if (sInstantMapID.wSubID == SNATIVEID::ID_NULL)
		{
			CDebugSet::ToFileWithTime("instancemap.txt", "[INFO AGENT]Create Failed Instant Map! ID reached Max Size! IDSize %d Map ID [%d][%d]",
									  m_sInsertInstantMapId.useMapSubIDSet.size(), sInstantMapID.wMainID, sInstantMapID.wSubID);

			NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL_CREATEMAX;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (CreateInstantMap(pCurGate->GetToMapID(dwToIndex), sInstantMapID, pNetMsg->dwGaeaID, pChar->GetPartyID()) == E_FAIL)
		{
			NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLAGLandMan *pInLandMan = NULL;
		pInLandMan = GetByMapID(sInstantMapID);
		if (!pInLandMan)
		{
			NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		CDebugSet::ToFileWithTime("instancemap.txt", "[INFO AGENT]Create Instant Map. MapInfo: ID [%d][%d] PartyMap %d HostID %d",
								  sInstantMapID.wMainID, sInstantMapID.wSubID, pInLandMan->IsPartyInstantMap(), pInLandMan->GetInstantMapHostID());

		NetMsgFld.bCreate = TRUE;
		NetMsgFld.dwGaeaID = pNetMsg->dwGaeaID;
		NetMsgFld.dwGateID = pNetMsg->dwGateID;
		NetMsgFld.dwToIndex = dwToIndex; /* multi gate out, Juver, 2020/11/16 */
		NetMsgFld.sInstantMapID = sInstantMapID;
		NetMsgFld.sBaseMapID = pCurGate->GetToMapID(dwToIndex);
	}
	else
	{
		GLAGLandMan *pInLandMan = GetByMapID(pNetMsg->sInstantMapID);
		if (!pInLandMan)
		{
			NetMsgFB.emFB = EMCHAR_CREATE_INSTANT_MAP_FAIL;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		CDebugSet::ToFileWithTime("instancemap.txt", "[INFO AGENT]Enter Instant Map. MapInfo: ID [%d][%d] PartyMap %d HostID %d",
								  pNetMsg->sInstantMapID.wMainID, pNetMsg->sInstantMapID.wSubID, pInLandMan->IsPartyInstantMap(), pInLandMan->GetInstantMapHostID());

		sInstantMapID = pNetMsg->sInstantMapID;

		NetMsgFld.bCreate = FALSE;
		NetMsgFld.dwGaeaID = pNetMsg->dwGaeaID;
		NetMsgFld.dwGateID = pNetMsg->dwGateID;
		NetMsgFld.dwToIndex = dwToIndex; /* multi gate out, Juver, 2020/11/16 */
		NetMsgFld.sInstantMapID = sInstantMapID;
		NetMsgFld.sBaseMapID = pCurGate->GetToMapID(dwToIndex);
	}

	// Agent 憮幗縑憮 乾堅 氈朝 馨葛攪 裘薑爾 滲唳
	pChar->SetCurrentField(pChar->m_dwCurFieldSvr, sInstantMapID);

	//	Note : 萄 憮幗 滲唳 蹂 橈擠. 營 萄憮幗煎 檣湍 裘 儅撩擊 蹂羶
	SENDTOFIELDSVR(pChar->m_nChannel, pChar->m_dwCurFieldSvr, &NetMsgFld);

	return TRUE;
}

BOOL GLAgentServer::MsgReqDeleteInstantMap(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETREQ_CREATE_INSTANT_MAP_DEL *pNetMsg = (GLMSG::SNETREQ_CREATE_INSTANT_MAP_DEL *)nmg;

	GLAGLandMan *pLandMan = GetByMapID(pNetMsg->sInstantMapID);
	if (pLandMan == NULL)
		return FALSE;
	if (!pLandMan->IsInstantMap())
		return FALSE;

	m_sInsertInstantMapId.DeleteInstantMapID(pNetMsg->sInstantMapID.wSubID);

	VEC_INSTANT_MAPID_ITER MapID_iter = std::find(m_vecInstantMapId.begin(), m_vecInstantMapId.end(), pNetMsg->sInstantMapID.dwID);
	if (MapID_iter != m_vecInstantMapId.end())
		m_vecInstantMapId.erase(MapID_iter);

	SAFE_DELETE(pLandMan);
	m_pLandMan[pNetMsg->sInstantMapID.wMainID][pNetMsg->sInstantMapID.wSubID] = NULL;

	CDebugSet::ToFileWithTime("instancemap.txt", "[INFO AGENT]Delete Instant Map!! Instant Map ID [%d][%d]",
							  pNetMsg->sInstantMapID.wMainID, pNetMsg->sInstantMapID.wSubID);

	BOOL bErase = m_sMapList.EraseMapList(pNetMsg->sInstantMapID);
	if (!bErase)
		CDebugSet::ToFileWithTime("instancemap.txt", "[INFO AGENT]InstanceMap erase to mapslist failed[%d~%d]",
								  pNetMsg->sInstantMapID.wMainID, pNetMsg->sInstantMapID.wSubID);
	else
		CDebugSet::ToFileWithTime("instancemap.txt", "[INFO AGENT]InstanceMap erase to mapslist[%d~%d]",
								  pNetMsg->sInstantMapID.wMainID, pNetMsg->sInstantMapID.wSubID);

	return TRUE;
}

// *****************************************************
// Desc: 檜瞪啪檜煎 檜翕 蹂羶 (Field->Agent)
// *****************************************************
BOOL GLAgentServer::MsgReqGateOut(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETREQ_GATEOUT *pNetMsg = (GLMSG::SNETREQ_GATEOUT *)nmg;
	// if ( dwGaeaID!=GAEAID_NULL )			return FALSE;

	PGLCHARAG pChar = GetChar(pNetMsg->dwGaeaID);
	if (!pChar)
		return FALSE;

	//	Note : 營 裘 薑爾.
	//
	GLAGLandMan *pCurLandMan = GetByMapID(pChar->m_sCurMapID);
	if (!pCurLandMan)
	{
		CDebugSet::ToLogFile("EMCHAR_GATEOUT_MAPID %d [%d/%d]",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID);

		GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
		NetMsgFB.emFB = EMCHAR_GATEOUT_MAPID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 營 啪檜
	DxLandGateMan *pCurGateMan = pCurLandMan->GetLandGateMan();
	PDXLANDGATE pCurGate = pCurGateMan->FindLandGate(pNetMsg->dwGateID);
	if (!pCurGate)
	{
		CDebugSet::ToLogFile("EMCHAR_GATEOUT_GATEID %d [%d/%d] %d %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pNetMsg->dwGateID,
							 pNetMsg->dwToIndex);

		GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
		NetMsgFB.emFB = EMCHAR_GATEOUT_GATEID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	/* multi gate out, Juver, 2020/11/16 */
	DWORD dwToIndex = pNetMsg->dwToIndex;
	if (dwToIndex >= DxLandGate::MAX_GATE_OUT)
	{
		CDebugSet::ToLogFile("EMCHAR_GATEOUT_TO_INDEX %d [%d/%d] %d %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pNetMsg->dwGateID,
							 pNetMsg->dwToIndex);

		GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
		NetMsgFB.emFB = EMCHAR_GATEOUT_TO_INDEX;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 跡 裘.
	//
	GLAGLandMan *pNextLandMan = GetByMapID(pCurGate->GetToMapID(dwToIndex));
	if (!pNextLandMan)
	{
		CDebugSet::ToLogFile("EMCHAR_GATEOUT_TARMAPID %d [%d/%d] %d [%d/%d] %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pCurGate->GetGateID(),
							 pCurGate->GetToMapID(dwToIndex).wMainID,
							 pCurGate->GetToMapID(dwToIndex).wSubID,
							 dwToIndex);

		GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
		NetMsgFB.emFB = EMCHAR_GATEOUT_TARMAPID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 跡裘檜 摹舒贗毀 瞪癱襄檣等 摹紫瞪檜 部陬堅 摹紫贗毀曖 望錳檜 嬴棲賊
	if (pNextLandMan->m_bGuidBattleMap)
	{
		if (GLCONST_CHAR::bPKLESS)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLGuidance *pGuid = GLGuidanceAgentMan::GetInstance().Find(pNextLandMan->m_dwClubMapID);
		if (pGuid)
		{

			// 摹紫瞪 橾陽 望寡 醞檜剪釭 謙猿衛除 嘐殖檜賊 轎殮旎雖
			GLCLUB *pClub = m_cClubMan.GetClub(pChar->m_dwGuild);
			if (pClub && pGuid->m_bBattle && (pClub->GetAllBattleNum() > 0 || !pClub->IsGuidBattle()))
			{
				GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
				NetMsgFB.emFB = EMCHAR_GATEOUT_CLUBBATTLE;
				SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
				return FALSE;
			}

			GLCLUB *pGuidanceClub = m_cClubMan.GetClub(pGuid->m_dwGuidanceClub);
			if (!pGuid->m_bBattle && !pGuid->IsGuidClub(pChar->m_dwGuild))
			{
				if (pGuidanceClub)
				{
					// 摹紫贗毀曖 翕裊檜 嬴棲賊
					if (!pGuidanceClub->IsAllianceGuild(pChar->m_dwGuild))
					{
						GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
						NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
						SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
						return FALSE;
					}
				}
			}
		}
	}

	// 跡裘檜 摹舒贗毀 瞪癱襄檣等 摹紫瞪檜 部陬堅 摹紫贗毀曖 望錳檜 嬴棲賊
	if (pNextLandMan->m_bClubDeathMatchMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLClubDeathMatch *pCDM = GLClubDeathMatchAgentMan::GetInstance().Find(pNextLandMan->m_dwClubMapID);
		if (pCDM)
		{
			GLCLUB *pClub = m_cClubMan.GetClub(pChar->m_dwGuild);

			if (!pCDM->IsEnterMap(pChar->m_dwGuild) || !pClub)
			{
				GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
				NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
				SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
				return FALSE;
			}

			if (pClub)
			{
				// 贗毀寡 霞醞橾陽朝 跤菟橫除棻.
				if (pClub->GetAllBattleNum() > 0)
				{
					GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
					NetMsgFB.emFB = EMCHAR_GATEOUT_CLUBBATTLE2;
					SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
					return FALSE;
				}

				if (!pClub->IsMemberFlgCDM(pChar->m_dwCharID))
				{
					GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
					NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
					SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
					return FALSE;
				}
			}
		}
	}

	/*pvp tyranny, Juver, 2017/08/24 */
	if (pNextLandMan->m_bPVPTyrannyMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (!GLPVPTyrannyAgent::GetInstance().IsBattle())
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	/*school wars, Juver, 2018/01/19 */
	if (pNextLandMan->m_bPVPSchoolWarsMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (!GLPVPSchoolWarsAgent::GetInstance().IsBattle())
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	/*pvp capture the flag, Juver, 2018/01/31 */
	if (pNextLandMan->m_bPVPCaptureTheFlagMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (!GLPVPCaptureTheFlagAgent::GetInstance().IsBattle())
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	/* pvp club death match, Juver, 2020/11/26 */
	if (pNextLandMan->m_bPVPClubDeathMatchLobbyMap || pNextLandMan->m_bPVPClubDeathMatchBattleMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (!PVPClubDeathMatchAgent::GetInstance().IsBattle())
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	if (pNextLandMan->m_bPVPPBGLobbyMap || pNextLandMan->m_bPVPPBGBattleMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (!PVPPBG::ManagerAgent::GetInstance().IsBattle())
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	/*woe Arc Development 08-06-2024*/
	if (pNextLandMan->m_bPVPWoeMap)
	{
		if (GLCONST_CHAR::bPKLESS || pChar->m_nChannel != 0)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (!GLPVPWoeAgent::GetInstance().IsBattle())
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_HOLD;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	//	Note : 跡 啪檜.
	//
	DxLandGateMan *pNextGateMan = pNextLandMan->GetLandGateMan();
	PDXLANDGATE pNextGate = pNextGateMan->FindLandGate(pCurGate->GetToGateID(dwToIndex));
	if (!pNextGate)
	{
		CDebugSet::ToLogFile("EMCHAR_GATEOUT_TARGATEID %d [%d/%d] %d [%d/%d] %d %d",
							 pChar->m_dwCharID,
							 pChar->m_sCurMapID.wMainID,
							 pChar->m_sCurMapID.wSubID,
							 pNetMsg->dwGateID,
							 pCurGate->GetToMapID(dwToIndex).wMainID,
							 pCurGate->GetToMapID(dwToIndex).wSubID,
							 pCurGate->GetToGateID(dwToIndex),
							 dwToIndex);

		GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
		NetMsgFB.emFB = EMCHAR_GATEOUT_TARGATEID;
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 萄 憮幗陛 偽擎 裘謙盟 橾陽縑朝 萄 憮幗 滲唳檜 蹂 橈擠.
	//
	DWORD dwNextFieldSvr = GetFieldServer(pCurGate->GetToMapID(dwToIndex));
	if (dwNextFieldSvr == pChar->m_dwCurFieldSvr)
	{
		// Agent 憮幗縑憮 乾堅 氈朝 馨葛攪 裘薑爾 滲唳
		pChar->SetCurrentField(pChar->m_dwCurFieldSvr, pCurGate->GetToMapID(dwToIndex));

		//	Note : 萄 憮幗 滲唳 蹂 橈擠. 營 萄憮幗煎 裘檜翕擊 蹂羶.
		//
		SENDTOFIELDSVR(pChar->m_nChannel, pChar->m_dwCurFieldSvr, nmg);
	}
	//	Note : 萄 憮幗 滲唳 蹂.
	//
	else
	{
		//	Note : 萄 憮幗陛 薑鼻檣雖 匐餌.
		//
		BOOL bSVR_OK = m_pMsgServer->IsSlotOnline(dwNextFieldSvr);
		if (!bSVR_OK)
		{
			GLMSG::SNETREQ_GATEOUT_FB NetMsgFB;
			NetMsgFB.emFB = EMCHAR_GATEOUT_FIELDSVR;
			SENDTOCLIENT(pChar->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		//	Note : 萄 憮幗 滲唳衛 贗塭檜樹 薑爾 滲唳 餌 奩艙.
		//
		ReActionMoveFieldSvr(pChar->m_dwClientID, pChar->m_dwGaeaID);

		//	營 萄 憮幗縑 濠褐檜 集釭陞 蕨薑歜擊 鱔爾.
		GLMSG::SNETPC_FIELDSVR_OUT NetMsg;
		SENDTOFIELD(pChar->m_dwClientID, &NetMsg);

		//	檜翕 裘 ID 盪濰.
		pChar->SetNextFieldSvr(pCurGate->GetToMapID(dwToIndex), pCurGate->GetToGateID(dwToIndex), D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX), dwNextFieldSvr);
	}

	return TRUE;
}

BOOL GLAgentServer::CheckStartMap(DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLAGLandMan *pLandMan = NULL;
	DWORD dwGenGate = 0;
	pLandMan = GetByMapID(pChar->m_sStartMapID);

	if (pLandMan)
	{
		if (pLandMan->m_bClubDeathMatchMap)
		{
			if (GLCONST_CHAR::bPKLESS)
				return FALSE;

			if (pChar->m_nChannel != 0)
				return FALSE;

			GLClubDeathMatch *pCDM = GLClubDeathMatchAgentMan::GetInstance().Find(pLandMan->m_dwClubMapID);
			if (pCDM && !pCDM->IsEnterMap(pChar->m_dwGuild))
				return FALSE;

			GLCLUB *pClub = m_cClubMan.GetClub(pChar->m_dwGuild);
			if (!pClub)
				return FALSE;

			// 贗毀寡 霞醞橾陽朝 跤菟橫除棻.
			if (pClub->GetAllBattleNum() > 0)
				return FALSE;
			if (!pClub->IsMemberFlgCDM(pChar->m_dwCharID))
				return FALSE;
		}

		/*pvp tyranny, Juver, 2017/08/24 */
		/*school wars, Juver, 2018/01/19 */
		/*pvp capture the flag, Juver, 2018/01/31 */
		/* pvp club death match, Juver, 2020/11/26 */
		if (pLandMan->m_bPVPTyrannyMap ||
			pLandMan->m_bPVPSchoolWarsMap ||
			pLandMan->m_bPVPCaptureTheFlagMap ||
			pLandMan->m_bPVPClubDeathMatchLobbyMap ||
			pLandMan->m_bPVPClubDeathMatchBattleMap ||
			pLandMan->m_bPVPPBGLobbyMap ||
			pLandMan->m_bPVPPBGBattleMap ||
			pLandMan->m_bPVPWoeMap)
		{
			return FALSE;
		}
	}

	if (!pLandMan)
	{
		return FALSE;
	}
	else if (!pLandMan->GetLandGateMan()->FindLandGate(pChar->m_dwStartGate))
	{
		PDXLANDGATE pLandGate = pLandMan->GetLandGateMan()->GetListLandGate();
		if (!pLandGate)
			return FALSE; //	Gate陛 釭紫 橈朝 夠縛 睡 熱 橈擠.
	}

	return TRUE;
}

// *****************************************************
// Desc: 睡蹂羶 籀葬
// *****************************************************
BOOL GLAgentServer::MsgReqReBirth(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	//  ºÎÈ° ÁöÁ¡À¸·Î ÁøÀÔ ºÒ°¡½Ã ºÎÈ° ÁöÁ¡À» ÃÊ±âÈ­ÇÑ´Ù.
	GLMSG::SNETPC_REQ_REBIRTH *pNetMsg = (GLMSG::SNETPC_REQ_REBIRTH *)nmg;

	bool bStartMap = false;

	//	睡.
	GLAGLandMan *pLandMan = NULL;
	DWORD dwGenGate = 0;

	//	衛濛嬪纂陛 滲唳腎橫撿 朝 唳辦
	if (pNetMsg->bRegenEntryFailed || !CheckStartMap(dwGaeaID))
	{
		pChar->m_sStartMapID = GLCONST_CHAR::nidSTARTMAP[pChar->m_wSchool];
		pChar->m_dwStartGate = GLCONST_CHAR::dwSTARTGATE[pChar->m_wSchool];
		pChar->m_vStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		SNATIVEID nidSTARTMAP = pChar->m_sStartMapID;
		DWORD dwSTARTGATE = pChar->m_dwStartGate;

		pLandMan = GetByMapID(nidSTARTMAP);
		if (pLandMan)
		{
			DxLandGateMan *pLandGateMan = pLandMan->GetLandGateMan();
			if (!pLandGateMan->FindLandGate(dwSTARTGATE))
			{
				DEBUGMSG_WRITE("(Revive) - Default gate is a failure. school %d", pChar->m_wSchool);
				return FALSE;
			}
			else
			{
				dwGenGate = dwSTARTGATE;
			}
		}
		else
		{
			DEBUGMSG_WRITE("GLAgentServer::MsgReqReBirth Default map is a failure. school %d", pChar->m_wSchool);
			return FALSE;
		}
	}
	else
	{
		pLandMan = GetByMapID(pChar->m_sStartMapID);

		//	罹晦 檜諼曖 鼻擎 嬪縑憮 棻 勘楝霞棻.
		if (pLandMan)
		{
			if (!pLandMan->GetLandGateMan()->FindLandGate(pChar->m_dwStartGate))
			{
				PDXLANDGATE pLandGate = pLandMan->GetLandGateMan()->GetListLandGate();
				if (pLandGate)
					dwGenGate = pLandGate->GetGateID(); //	羅廓簞 Gate 縑憮 儅梯.
				else
				{
					//	橾橫陳熱 橈朝 鼻
					DEBUGMSG_WRITE("GLAgentServer::MsgReqReBirth Error Logic No Gate");
					return FALSE;
				}
			}
			else
				dwGenGate = pChar->m_dwStartGate;
		}
		else
		{
			//	橾橫陳熱 橈朝 鼻
			DEBUGMSG_WRITE("GLAgentServer::MsgReqReBirth Error Logic No LandMan");
			return FALSE;
		}
	}

	/*
		if( pNetMsg->bRegenEntryFailed )
		{
			pChar->m_sStartMapID = GLCONST_CHAR::nidSTARTMAP[pChar->m_wSchool];
			pChar->m_dwStartGate = GLCONST_CHAR::dwSTARTGATE[pChar->m_wSchool];
			pChar->m_vStartPos   = D3DXVECTOR3(0.0f,0.0f,0.0f);
		}

		//	睡.
		GLAGLandMan *pLandMan=NULL;
		DWORD dwGenGate = 0;

		//	睡 Map婁 Gate 嶸撩 匐餌.
		if ( pChar->m_sStartMapID!=NATIVEID_NULL() )
		{
			pLandMan = GetByMapID ( pChar->m_sStartMapID );

			if ( pLandMan )
			{
				DxLandGateMan* pLandGateMan = pLandMan->GetLandGateMan();
				if ( pLandGateMan->FindLandGate(pChar->m_dwStartGate)==NULL )
				{
					PDXLANDGATE pLandGate = pLandGateMan->GetListLandGate();
					if ( !pLandGate )	pLandMan = NULL;					//	Gate陛 釭紫 橈朝 夠縛 睡 熱 橈擠.
					else				dwGenGate = pLandGate->GetGateID();	//	羅廓簞 Gate 縑憮 儅梯.
				}
				else					dwGenGate = pChar->m_dwStartGate;
			}
		}


		if ( !pLandMan )
		{
			SNATIVEID nidSTARTMAP = GLCONST_CHAR::nidSTARTMAP[pChar->m_wSchool];
			DWORD dwSTARTGATE = GLCONST_CHAR::dwSTARTGATE[pChar->m_wSchool];

			pLandMan = GetByMapID ( nidSTARTMAP );
			if ( pLandMan )
			{
				DxLandGateMan* pLandGateMan = pLandMan->GetLandGateMan();
				if ( !pLandGateMan->FindLandGate ( dwSTARTGATE ) )
				{
					DEBUGMSG_WRITE ( "(Revive) - Default gate is a failure. school %d", pChar->m_wSchool );
					return FALSE;
				}
				else
				{
					dwGenGate = dwSTARTGATE;
				}
			}
		}

		if ( !pLandMan )
		{
			DEBUGMSG_WRITE(
				"GLAgentServer::MsgReqReBirth Default map is a failure. school %d",
				pChar->m_wSchool );
			return FALSE;
		}
	*/
	SNATIVEID sBirthMapID = pLandMan->GetMapID();

	DWORD dwNextFieldSvr = GetFieldServer(sBirthMapID);
	if (dwNextFieldSvr == pChar->m_dwCurFieldSvr)
	{
		pChar->SetCurrentField(pChar->m_dwCurFieldSvr, sBirthMapID);

		//	Note : 萄 憮幗 滲唳檜 蹂 橈擊陽. 營 萄憮幗煎 睡擊 蹂羶.
		//
		SENDTOFIELD(dwClientID, nmg);
	}
	else
	{
		//	Note : 萄 憮幗曖 滲唳檜 蹂 唳辦.
		//

		//	Note : 萄 憮幗 滲唳衛 贗塭檜樹 薑爾 滲唳 餌 奩艙.
		//
		ReActionMoveFieldSvr(dwClientID, dwGaeaID);

		//	薯 萄 憮幗縑 濠褐檜 棻艇 夠縑憮 睡 蕨薑歜擊 鱔爾.
		GLMSG::SNETPC_REBIRTH_OUT NetMsg;
		SENDTOFIELD(dwClientID, &NetMsg);

		//	檜翕 裘 ID 盪濰.
		pChar->SetNextFieldSvr(sBirthMapID, dwGenGate, D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX), dwNextFieldSvr);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgReqReGenGate(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_REQ_REGEN_GATE_FB *pNetMsg = (GLMSG::SNETPC_REQ_REGEN_GATE_FB *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	//	Note : 滲唳脹 蝶顫 詻, 蝶顫 啪檜曖 薑爾蒂 瞳辨.
	//
	pChar->m_sStartMapID = pNetMsg->sMapID;
	pChar->m_dwStartGate = pNetMsg->dwGateID;

	SENDTOCLIENT(pChar->m_dwClientID, nmg);

	return TRUE;
}

// *****************************************************
// Desc: 萄憮幗縑 蕾樓薑爾 瞪歎(萄憮幗陛 夥莞唳辦)
// *****************************************************
BOOL GLAgentServer::MsgFieldSvrOutFb(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	if (dwGaeaID < 0 || dwGaeaID >= m_dwMaxClient)
		return FALSE;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	if (pChar->m_sNextMapID == SNATIVEID(MAXLANDMID, MAXLANDSID))
		return FALSE;

	//	Note : 營 萄 憮幗 塽 裘 ID 雖薑.
	pChar->SetCurrentField(pChar->m_dwNextFieldSvr, pChar->m_sNextMapID);

	//	Note : 裘縑 億煎 霞殮晦 嬪憮 Field憮幗縑 詭衛雖 歎褐.
	//
	NET_GAME_JOIN_FIELDSVR NetJoinField;
	NetJoinField.emType = EMJOINTYPE_MOVEMAP;

	NetJoinField.dwSlotAgentClient = pChar->m_dwClientID;
	StringCchCopy(NetJoinField.szUID, DAUM_MAX_UID_LENGTH + 1, pChar->m_szUserName);
	NetJoinField.nUserNum = pChar->m_dwUserID;
	NetJoinField.dwUserLvl = pChar->m_dwUserLvl;
	NetJoinField.nChaNum = pChar->m_dwCharID;
	NetJoinField.dwGaeaID = pChar->m_dwGaeaID;
	NetJoinField.tPREMIUM = pChar->m_tPREMIUM;
	NetJoinField.tCHATBLOCK = pChar->m_tCHATBLOCK;

	NetJoinField.sStartMap = pChar->m_sNextMapID;
	NetJoinField.dwStartGate = pChar->m_dwNextGateID;
	NetJoinField.vStartPos = pChar->m_vNextPos;

	NetJoinField.dwActState = pChar->m_dwActState;
	NetJoinField.bUseArmSub = pChar->m_bUseArmSub;

	NetJoinField.dwThaiCCafeClass = pChar->m_dwThaiCCafeClass;
	NetJoinField.nMyCCafeClass = pChar->m_nMyCCafeClass; // 蜓溯檜衛嬴 PC寞 檜漸

	NetJoinField.sChinaTime = pChar->m_sChinaTime;
	NetJoinField.sEventTime = pChar->m_sEventTime;
	NetJoinField.sVietnamGainSystem = pChar->m_sVietnamSystem;

	/* user flag verified, Juver, 2020/02/25 */
	NetJoinField.bUserFlagVerified = pChar->m_bUserFlagVerified;

	/*dmk14 ingame web*/
	NetJoinField.dwPPoints = pChar->m_dwPPoints;
	NetJoinField.dwVPoints = pChar->m_dwVPoints;

	/* user flag restricted, Juver, 2020/04/20 */
	NetJoinField.bUserFlagRestricted = pChar->m_bUserFlagRestricted;

	//** Add EventTime

	pChar->ResetNextFieldSvr();

	SENDTOFIELD(dwClientID, &NetJoinField);

	return TRUE;
}

BOOL GLAgentServer::MsgReBirthOutFb(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	if (pChar->m_sNextMapID == SNATIVEID(MAXLANDMID, MAXLANDSID))
		return FALSE;

	//	Note : 營 萄 憮幗 塽 裘 ID 雖薑.
	pChar->SetCurrentField(pChar->m_dwNextFieldSvr, pChar->m_sNextMapID);

	//	Note : 裘縑 億煎 霞殮晦 嬪憮 Field憮幗縑 詭衛雖 歎褐.
	//
	NET_GAME_JOIN_FIELDSVR NetJoinField;
	NetJoinField.emType = EMJOINTYPE_REBIRTH;

	NetJoinField.dwSlotAgentClient = pChar->m_dwClientID;
	NetJoinField.nUserNum = pChar->m_dwUserID;
	StringCchCopy(NetJoinField.szUID, DAUM_MAX_UID_LENGTH + 1, pChar->m_szUserName);
	NetJoinField.dwUserLvl = pChar->m_dwUserLvl;
	NetJoinField.nChaNum = pChar->m_dwCharID;
	NetJoinField.dwGaeaID = pChar->m_dwGaeaID;
	NetJoinField.tPREMIUM = pChar->m_tPREMIUM;
	NetJoinField.tCHATBLOCK = pChar->m_tCHATBLOCK;

	NetJoinField.sStartMap = pChar->m_sNextMapID;
	NetJoinField.dwStartGate = pChar->m_dwNextGateID;
	NetJoinField.vStartPos = D3DXVECTOR3(0, 0, 0);

	NetJoinField.dwActState = pChar->m_dwActState;
	NetJoinField.bUseArmSub = pChar->m_bUseArmSub;

	NetJoinField.dwThaiCCafeClass = pChar->m_dwThaiCCafeClass;
	NetJoinField.nMyCCafeClass = pChar->m_nMyCCafeClass; // 蜓溯檜衛嬴 PC寞 檜漸

	NetJoinField.sChinaTime = pChar->m_sChinaTime;
	NetJoinField.sEventTime = pChar->m_sEventTime;
	NetJoinField.sVietnamGainSystem = pChar->m_sVietnamSystem;

	/* user flag verified, Juver, 2020/02/25 */
	NetJoinField.bUserFlagVerified = pChar->m_bUserFlagVerified;

	/*dmk14 ingame web*/
	NetJoinField.dwPPoints = pChar->m_dwPPoints;
	NetJoinField.dwVPoints = pChar->m_dwVPoints;

	/* user flag restricted, Juver, 2020/04/20 */
	NetJoinField.bUserFlagRestricted = pChar->m_bUserFlagRestricted;

	//** Add EventTime

	pChar->ResetNextFieldSvr();

	SENDTOFIELD(dwClientID, &NetJoinField);

	return TRUE;
}

/* pvp club death match, Juver, 2020/11/26 */
BOOL GLAgentServer::MsgPVPClubDeathMatchReBirthOutFb(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	if (pChar->m_sNextMapID == SNATIVEID(MAXLANDMID, MAXLANDSID))
		return FALSE;

	//	Note : 營 萄 憮幗 塽 裘 ID 雖薑.
	pChar->SetCurrentField(pChar->m_dwNextFieldSvr, pChar->m_sNextMapID);

	//	Note : 裘縑 億煎 霞殮晦 嬪憮 Field憮幗縑 詭衛雖 歎褐.
	//
	NET_GAME_JOIN_FIELDSVR NetJoinField;
	NetJoinField.emType = EMJOINTYPE_PVP_CLUB_DEATH_MATCH_REBIRTH;

	NetJoinField.dwSlotAgentClient = pChar->m_dwClientID;
	NetJoinField.nUserNum = pChar->m_dwUserID;
	StringCchCopy(NetJoinField.szUID, DAUM_MAX_UID_LENGTH + 1, pChar->m_szUserName);
	NetJoinField.dwUserLvl = pChar->m_dwUserLvl;
	NetJoinField.nChaNum = pChar->m_dwCharID;
	NetJoinField.dwGaeaID = pChar->m_dwGaeaID;
	NetJoinField.tPREMIUM = pChar->m_tPREMIUM;
	NetJoinField.tCHATBLOCK = pChar->m_tCHATBLOCK;

	NetJoinField.sStartMap = pChar->m_sNextMapID;
	NetJoinField.dwStartGate = pChar->m_dwNextGateID;
	NetJoinField.vStartPos = D3DXVECTOR3(0, 0, 0);

	NetJoinField.dwActState = pChar->m_dwActState;
	NetJoinField.bUseArmSub = pChar->m_bUseArmSub;

	NetJoinField.dwThaiCCafeClass = pChar->m_dwThaiCCafeClass;
	NetJoinField.nMyCCafeClass = pChar->m_nMyCCafeClass; // 蜓溯檜衛嬴 PC寞 檜漸

	NetJoinField.sChinaTime = pChar->m_sChinaTime;
	NetJoinField.sEventTime = pChar->m_sEventTime;
	NetJoinField.sVietnamGainSystem = pChar->m_sVietnamSystem;

	/* user flag verified, Juver, 2020/02/25 */
	NetJoinField.bUserFlagVerified = pChar->m_bUserFlagVerified;

	/* user flag restricted, Juver, 2020/04/20 */
	NetJoinField.bUserFlagRestricted = pChar->m_bUserFlagRestricted;

	//** Add EventTime

	pChar->ResetNextFieldSvr();

	SENDTOFIELD(dwClientID, &NetJoinField);

	return TRUE;
}

// *****************************************************
// Desc: 溯漣 機衛 營 裘縑 該僭楝 氈雖 彊嬴撿 朝 唳辦
// *****************************************************
BOOL GLAgentServer::MsgLvUpMustLeaveMap(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_AG *pNetMsg = (GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_AG *)nmg;

	//	Note : 跡 裘.
	//
	GLAGLandMan *pNextLandMan = GetByMapID(pNetMsg->sMAPID);
	if (!pNextLandMan)
	{
		GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_MUST_LEAVE_MAP_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 檜翕 嬪纂 薑爾 嶸撩 匐餌.
	//
	if (pNetMsg->dwGATEID == UINT_MAX && pNetMsg->vPOS == D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX))
	{
		GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_MUST_LEAVE_MAP_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 裘檜 氈朝雖
	SMAPNODE *pMapNode = m_sMapList.FindMapNode(pNetMsg->sMAPID);
	if (!pMapNode)
	{
		GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_MUST_LEAVE_MAP_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	DWORD dwNextFieldSvr = pMapNode->dwFieldSID;

	// 萄憮幗蒂 衡啖撿  唳辦
	{
		//	Note : 萄憮幗陛 薑鼻檣雖 匐餌.
		//
		BOOL bSVR_OK = m_pMsgServer->IsSlotOnline(dwNextFieldSvr);
		if (!bSVR_OK)
		{
			GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB NetMsgFB;
			NetMsgFB.emFB = EMREQ_MUST_LEAVE_MAP_FB_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		//	Note : 萄 憮幗 滲唳衛 贗塭檜樹 薑爾 滲唳 餌 奩艙.
		//
		ReActionMoveFieldSvr(dwClientID, dwGaeaID);

		//	營 萄 憮幗縑 濠褐檜 集釭陞 蕨薑歜擊 鱔爾.
		GLMSG::SNETPC_FIELDSVR_OUT NetMsg;
		SENDTOFIELD(pChar->m_dwClientID, &NetMsg);

		//	檜翕 裘 ID 盪濰.
		pChar->SetNextFieldSvr(pNetMsg->sMAPID, pNetMsg->dwGATEID, pNetMsg->vPOS, dwNextFieldSvr);
	}

	return TRUE;
}

// *****************************************************
// Desc: 模蹂羶 籀葬 (棻艇 萄憮幗橾 唳辦)
// *****************************************************
BOOL GLAgentServer::MsgReCallAg(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNETPC_REQ_RECALL_AG *pNetMsg = (GLMSG::SNETPC_REQ_RECALL_AG *)nmg;

	//	Note : 跡 裘.
	//
	GLAGLandMan *pNextLandMan = GetByMapID(pNetMsg->sMAPID);
	if (!pNextLandMan)
	{
		GLMSG::SNETPC_REQ_RECALL_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_RECALL_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 檜翕 嬪纂 薑爾 嶸撩 匐餌.
	//
	if (pNetMsg->dwGATEID == UINT_MAX && pNetMsg->vPOS == D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX))
	{
		GLMSG::SNETPC_REQ_RECALL_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_RECALL_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 裘檜 氈朝雖
	SMAPNODE *pMapNode = m_sMapList.FindMapNode(pNetMsg->sMAPID);
	if (!pMapNode)
	{
		GLMSG::SNETPC_REQ_RECALL_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_RECALL_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	DWORD dwNextFieldSvr = pMapNode->dwFieldSID;

	// 萄憮幗蒂 衡啖撿  唳辦
	{
		//	Note : 萄憮幗陛 薑鼻檣雖 匐餌.
		//
		BOOL bSVR_OK = m_pMsgServer->IsSlotOnline(dwNextFieldSvr);
		if (!bSVR_OK)
		{
			GLMSG::SNETPC_REQ_RECALL_FB NetMsgFB;
			NetMsgFB.emFB = EMREQ_RECALL_FB_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		//	Note : 萄 憮幗 滲唳衛 贗塭檜樹 薑爾 滲唳 餌 奩艙.
		//
		ReActionMoveFieldSvr(dwClientID, dwGaeaID);

		//	營 萄 憮幗縑 濠褐檜 集釭陞 蕨薑歜擊 鱔爾.
		GLMSG::SNETPC_FIELDSVR_OUT NetMsg;
		SENDTOFIELD(pChar->m_dwClientID, &NetMsg);

		//	檜翕 裘 ID 盪濰.
		pChar->SetNextFieldSvr(pNetMsg->sMAPID, pNetMsg->dwGATEID, pNetMsg->vPOS, dwNextFieldSvr);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgTeleportAg(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNETPC_REQ_TELEPORT_AG *pNetMsg = (GLMSG::SNETPC_REQ_TELEPORT_AG *)nmg;

	//	Note : 跡 裘.
	//
	GLAGLandMan *pNextLandMan = GetByMapID(pNetMsg->sMAPID);
	if (!pNextLandMan)
	{
		GLMSG::SNETPC_REQ_TELEPORT_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_TELEPORT_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 檜翕 嬪纂 薑爾 嶸撩 匐餌.
	//
	if (pNetMsg->vPOS == D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX))
	{
		GLMSG::SNETPC_REQ_TELEPORT_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_TELEPORT_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 裘檜 氈朝雖
	SMAPNODE *pMapNode = m_sMapList.FindMapNode(pNetMsg->sMAPID);
	if (!pMapNode)
	{
		GLMSG::SNETPC_REQ_TELEPORT_FB NetMsgFB;
		NetMsgFB.emFB = EMREQ_TELEPORT_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	DWORD dwNextFieldSvr = pMapNode->dwFieldSID;

	// 萄憮幗蒂 衡啖撿  唳辦
	{
		//	Note : 萄憮幗陛 薑鼻檣雖 匐餌.
		//
		BOOL bSVR_OK = m_pMsgServer->IsSlotOnline(dwNextFieldSvr);
		if (!bSVR_OK)
		{
			GLMSG::SNETPC_REQ_TELEPORT_FB NetMsgFB;
			NetMsgFB.emFB = EMREQ_TELEPORT_FB_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		//	Note : 萄 憮幗 滲唳衛 贗塭檜樹 薑爾 滲唳 餌 奩艙.
		//
		ReActionMoveFieldSvr(dwClientID, dwGaeaID);

		//	營 萄 憮幗縑 濠褐檜 集釭陞 蕨薑歜擊 鱔爾.
		GLMSG::SNETPC_FIELDSVR_OUT NetMsg;
		SENDTOFIELD(pChar->m_dwClientID, &NetMsg);

		//	檜翕 裘 ID 盪濰.
		pChar->SetNextFieldSvr(pNetMsg->sMAPID, UINT_MAX, pNetMsg->vPOS, dwNextFieldSvr);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgPartyLure(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_LURE *pNetMsg = (GLMSG::SNET_PARTY_LURE *)nmg;

	PGLCHARAG pCharMaster = GetChar(dwGaeaID);
	if (!pCharMaster)
		return FALSE;
	DWORD dwPartyID = pCharMaster->m_dwPartyID;

	PGLCHARAG pCharTar = GetChar(pNetMsg->dwGaeaID);
	if (!pCharTar)
		return FALSE;

	if (!pNetMsg->sOption.VALID_OPT())
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_OPT;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	//	Note : 渠鼻檜 檜嘐 縑 陛殮腎橫 氈擠.
	//
	if (pCharTar->m_dwPartyID != PARTY_NULL)
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_OTHER;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	GLClubMan &cClubMan = GLAgentServer::GetInstance().GetClubMan();

	GLCLUB *pCLUB = cClubMan.GetClub(pCharTar->m_dwGuild);
	GLCLUB *pCLUB_M = cClubMan.GetClub(pCharMaster->m_dwGuild);

	if (pCLUB)
	{
		bool bClubBattle = false;
		// 陛 橈擊唳辦 ( 濠褐虜 羹觼 )
		if (dwPartyID == PARTY_NULL)
		{
			if (pCLUB->IsBattle(pCharMaster->m_dwGuild))
				bClubBattle = true;
			if (pCLUB_M && pCLUB->IsBattleAlliance(pCLUB_M->m_dwAlliance))
				bClubBattle = true;
		}
		//	氈擊唳辦 賅萇 詹幗嫌 羹觼
		else
		{
			GLPARTY *pParty = m_cPartyMan.GetParty(dwPartyID);

			if (pParty && pParty->ISVAILD())
			{
				GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
				GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();
				PGLCHARAG pMemChar = NULL;

				for (; iter != iter_end; ++iter)
				{
					pMemChar = GLAgentServer::GetInstance().GetChar((*iter));
					if (!pMemChar)
						continue;

					if (pCLUB->IsBattle(pMemChar->m_dwGuild))
					{
						bClubBattle = true;
						break;
					}

					GLCLUB *pClub_Mem = cClubMan.GetClub(pMemChar->m_dwGuild);

					if (pClub_Mem && pCLUB->IsBattleAlliance(pClub_Mem->m_dwAlliance))
					{
						bClubBattle = true;
						break;
					}
				}
			}
		}

		if (bClubBattle)
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_CLUBBATTLE;

			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

			return FALSE;
		}
	}

	/*pvp capture the flag, Juver, 2018/02/03 */
	BOOL bCaptureTheFlag = FALSE;

	/* pvp club death match, Juver, 2020/11/26 */
	BOOL bPVPClubDeathMatch = FALSE;

	GLAGLandMan *pLandMan = GetByMapID(pCharMaster->m_sCurMapID);
	if (pLandMan)
	{
		/*pvp capture the flag, Juver, 2018/02/03 */
		bCaptureTheFlag = pLandMan->m_bPVPCaptureTheFlagMap;

		/* pvp club death match, Juver, 2020/11/26 */
		if (pLandMan->m_bPVPClubDeathMatchLobbyMap || pLandMan->m_bPVPClubDeathMatchBattleMap)
		{
			bPVPClubDeathMatch = TRUE;
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		/*pvp capture the flag, Juver, 2018/02/03 */
		if (bCaptureTheFlag && pCharMaster && pCharTar)
		{
			if (pCharMaster->m_wCaptureTheFlagTeam != pCharTar->m_wCaptureTheFlagTeam)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHER_TEAM;
				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
				return FALSE;
			}
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		/* pvp club death match, Juver, 2020/11/26 */
		if (bPVPClubDeathMatch && pCharMaster && pCharTar)
		{
			if (pCharMaster->m_dwGuild != pCharTar->m_dwGuild)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHER_GUILD;
				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
				return FALSE;
			}
		}
	}

	/*map party setting, Juver, 2018/06/29 */
	if (pCharMaster && pCharTar)
	{
		GLAGLandMan *pland_master = GetByMapID(pCharMaster->m_sCurMapID);
		GLAGLandMan *pland_target = GetByMapID(pCharTar->m_sCurMapID);

		if (pland_master && pland_master->IsBlockParty())
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_BLOCK_MAP_MASTER;
			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (pland_target && pland_target->IsBlockParty())
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_BLOCK_MAP_TARGET;
			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{

		//	Note : 棻艇 錳儅婁  唸撩 薯 褫暮檜 氈擊 唳辦 薯.
		if (!GLCONST_CHAR::bPARTY_2OTHERSCHOOL && !bCaptureTheFlag)
		{
			if (pCharTar->m_wSchool != pCharMaster->m_wSchool)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHERSCHOOL;

				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

				return FALSE;
			}
		}
	}

	//	Note : 嬴霜 濠褐曖 陛 虜菟橫 雖雖 彊懊擊 唳辦. 渠鼻縑啪  霤陛 蹂羶.
	//
	if (dwPartyID == PARTY_NULL)
	{
		pCharMaster->m_sReqPartyOpt = pNetMsg->sOption;

		GLMSG::SNET_PARTY_LURE_TAR NetMsgLureTar;
		NetMsgLureTar.dwMasterID = dwGaeaID;
		NetMsgLureTar.dwPartyID = PARTY_NULL;
		NetMsgLureTar.sOption = pCharMaster->m_sReqPartyOpt;
		StringCchCopy(NetMsgLureTar.szCharName, CHAR_SZNAME, pCharMaster->m_szName);
		SENDTOCLIENT(pCharTar->m_dwClientID, &NetMsgLureTar);

		return TRUE;
	}

	GLPARTY *pParty = m_cPartyMan.GetParty(dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMASTER(dwGaeaID))
		return FALSE;

	if (pParty->ISFULL())
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_FULL;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	//	Note : 渠鼻濠蒂 賅餵 渠鼻縑 蛔煙.
	//
	pParty->PREADDMEMBER(pCharTar->m_dwGaeaID);

	//	Note : 渠鼻縑啪  霤陛 蹂羶.
	//
	GLMSG::SNET_PARTY_LURE_TAR NetMsgLureTar;
	NetMsgLureTar.dwMasterID = dwGaeaID;
	NetMsgLureTar.dwPartyID = dwPartyID;
	NetMsgLureTar.sOption = pCharMaster->m_sReqPartyOpt;
	StringCchCopy(NetMsgLureTar.szCharName, CHAR_SZNAME, pCharMaster->m_szName);
	SENDTOCLIENT(pCharTar->m_dwClientID, &NetMsgLureTar);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if (pCharMaster->m_bTracingUser)
	{
		CDebugSet::ToTracingFile(pCharMaster->m_szUserName, "Party Request, [%s][%s], Target Char: [%s][%s]",
								 pCharMaster->m_szUserName, pCharMaster->m_szName, pCharTar->m_szUserName, pCharTar->m_szName);
	}
#endif

	return TRUE;
}

/* party charid/charname, Juver, 2020/03/02 */
BOOL GLAgentServer::MsgPartyLureCharID(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_LURE_CHAR_ID *pNetMsg = (GLMSG::SNET_PARTY_LURE_CHAR_ID *)nmg;

	PGLCHARAG pCharMaster = GetChar(dwGaeaID);
	if (!pCharMaster)
		return FALSE;
	DWORD dwPartyID = pCharMaster->m_dwPartyID;

	PGLCHARAG pCharTar = GetCharID(pNetMsg->dwCharID);
	if (!pCharTar)
		return FALSE;

	if (!pNetMsg->sOption.VALID_OPT())
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_OPT;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	//	Note : 渠鼻檜 檜嘐 縑 陛殮腎橫 氈擠.
	//
	if (pCharTar->m_dwPartyID != PARTY_NULL)
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_OTHER;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	GLClubMan &cClubMan = GLAgentServer::GetInstance().GetClubMan();

	GLCLUB *pCLUB = cClubMan.GetClub(pCharTar->m_dwGuild);
	GLCLUB *pCLUB_M = cClubMan.GetClub(pCharMaster->m_dwGuild);

	if (pCLUB)
	{
		bool bClubBattle = false;
		// 陛 橈擊唳辦 ( 濠褐虜 羹觼 )
		if (dwPartyID == PARTY_NULL)
		{
			if (pCLUB->IsBattle(pCharMaster->m_dwGuild))
				bClubBattle = true;
			if (pCLUB_M && pCLUB->IsBattleAlliance(pCLUB_M->m_dwAlliance))
				bClubBattle = true;
		}
		//	氈擊唳辦 賅萇 詹幗嫌 羹觼
		else
		{
			GLPARTY *pParty = m_cPartyMan.GetParty(dwPartyID);

			if (pParty && pParty->ISVAILD())
			{
				GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
				GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();
				PGLCHARAG pMemChar = NULL;

				for (; iter != iter_end; ++iter)
				{
					pMemChar = GLAgentServer::GetInstance().GetChar((*iter));
					if (!pMemChar)
						continue;

					if (pCLUB->IsBattle(pMemChar->m_dwGuild))
					{
						bClubBattle = true;
						break;
					}

					GLCLUB *pClub_Mem = cClubMan.GetClub(pMemChar->m_dwGuild);

					if (pClub_Mem && pCLUB->IsBattleAlliance(pClub_Mem->m_dwAlliance))
					{
						bClubBattle = true;
						break;
					}
				}
			}
		}

		if (bClubBattle)
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_CLUBBATTLE;

			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

			return FALSE;
		}
	}

	/*pvp capture the flag, Juver, 2018/02/03 */
	BOOL bCaptureTheFlag = FALSE;

	/* pvp club death match, Juver, 2020/11/26 */
	BOOL bPVPClubDeathMatch = FALSE;

	GLAGLandMan *pLandMan = GetByMapID(pCharMaster->m_sCurMapID);
	if (pLandMan)
	{
		/*pvp capture the flag, Juver, 2018/02/03 */
		bCaptureTheFlag = pLandMan->m_bPVPCaptureTheFlagMap;

		/* pvp club death match, Juver, 2020/11/26 */
		if (pLandMan->m_bPVPClubDeathMatchLobbyMap || pLandMan->m_bPVPClubDeathMatchBattleMap)
		{
			bPVPClubDeathMatch = TRUE;
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		/*pvp capture the flag, Juver, 2018/02/03 */
		if (bCaptureTheFlag && pCharMaster && pCharTar)
		{
			if (pCharMaster->m_wCaptureTheFlagTeam != pCharTar->m_wCaptureTheFlagTeam)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHER_TEAM;
				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
				return FALSE;
			}
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		/* pvp club death match, Juver, 2020/11/26 */
		if (bPVPClubDeathMatch && pCharMaster && pCharTar)
		{
			if (pCharMaster->m_dwGuild != pCharTar->m_dwGuild)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHER_GUILD;
				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
				return FALSE;
			}
		}
	}

	/*map party setting, Juver, 2018/06/29 */
	if (pCharMaster && pCharTar)
	{
		GLAGLandMan *pland_master = GetByMapID(pCharMaster->m_sCurMapID);
		GLAGLandMan *pland_target = GetByMapID(pCharTar->m_sCurMapID);

		if (pland_master && pland_master->IsBlockParty())
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_BLOCK_MAP_MASTER;
			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (pland_target && pland_target->IsBlockParty())
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_BLOCK_MAP_TARGET;
			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		//	Note : 棻艇 錳儅婁  唸撩 薯 褫暮檜 氈擊 唳辦 薯.
		if (!GLCONST_CHAR::bPARTY_2OTHERSCHOOL && !bCaptureTheFlag)
		{
			if (pCharTar->m_wSchool != pCharMaster->m_wSchool)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHERSCHOOL;

				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

				return FALSE;
			}
		}
	}

	//	Note : 嬴霜 濠褐曖 陛 虜菟橫 雖雖 彊懊擊 唳辦. 渠鼻縑啪  霤陛 蹂羶.
	//
	if (dwPartyID == PARTY_NULL)
	{
		pCharMaster->m_sReqPartyOpt = pNetMsg->sOption;

		GLMSG::SNET_PARTY_LURE_TAR NetMsgLureTar;
		NetMsgLureTar.dwMasterID = dwGaeaID;
		NetMsgLureTar.dwPartyID = PARTY_NULL;
		NetMsgLureTar.sOption = pCharMaster->m_sReqPartyOpt;
		StringCchCopy(NetMsgLureTar.szCharName, CHAR_SZNAME, pCharMaster->m_szName);
		SENDTOCLIENT(pCharTar->m_dwClientID, &NetMsgLureTar);

		return TRUE;
	}

	GLPARTY *pParty = m_cPartyMan.GetParty(dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMASTER(dwGaeaID))
		return FALSE;

	if (pParty->ISFULL())
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_FULL;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	//	Note : 渠鼻濠蒂 賅餵 渠鼻縑 蛔煙.
	//
	pParty->PREADDMEMBER(pCharTar->m_dwGaeaID);

	//	Note : 渠鼻縑啪  霤陛 蹂羶.
	//
	GLMSG::SNET_PARTY_LURE_TAR NetMsgLureTar;
	NetMsgLureTar.dwMasterID = dwGaeaID;
	NetMsgLureTar.dwPartyID = dwPartyID;
	NetMsgLureTar.sOption = pCharMaster->m_sReqPartyOpt;
	StringCchCopy(NetMsgLureTar.szCharName, CHAR_SZNAME, pCharMaster->m_szName);
	SENDTOCLIENT(pCharTar->m_dwClientID, &NetMsgLureTar);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if (pCharMaster->m_bTracingUser)
	{
		CDebugSet::ToTracingFile(pCharMaster->m_szUserName, "Party Request, [%s][%s], Target Char: [%s][%s]",
								 pCharMaster->m_szUserName, pCharMaster->m_szName, pCharTar->m_szUserName, pCharTar->m_szName);
	}
#endif

	return TRUE;
}

/* party charid/charname, Juver, 2020/03/02 */
BOOL GLAgentServer::MsgPartyLureCharName(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_LURE_CHAR_NAME *pNetMsg = (GLMSG::SNET_PARTY_LURE_CHAR_NAME *)nmg;

	PGLCHARAG pCharMaster = GetChar(dwGaeaID);
	if (!pCharMaster)
		return FALSE;
	DWORD dwPartyID = pCharMaster->m_dwPartyID;

	if (strlen(pNetMsg->szCharName) == 0)
		return FALSE;

	PGLCHARAG pCharTar = GetChar(pNetMsg->szCharName);
	if (!pCharTar)
		return FALSE;

	if (!pNetMsg->sOption.VALID_OPT())
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_OPT;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	//	Note : 渠鼻檜 檜嘐 縑 陛殮腎橫 氈擠.
	//
	if (pCharTar->m_dwPartyID != PARTY_NULL)
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_OTHER;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	GLClubMan &cClubMan = GLAgentServer::GetInstance().GetClubMan();

	GLCLUB *pCLUB = cClubMan.GetClub(pCharTar->m_dwGuild);
	GLCLUB *pCLUB_M = cClubMan.GetClub(pCharMaster->m_dwGuild);

	if (pCLUB)
	{
		bool bClubBattle = false;
		// 陛 橈擊唳辦 ( 濠褐虜 羹觼 )
		if (dwPartyID == PARTY_NULL)
		{
			if (pCLUB->IsBattle(pCharMaster->m_dwGuild))
				bClubBattle = true;
			if (pCLUB_M && pCLUB->IsBattleAlliance(pCLUB_M->m_dwAlliance))
				bClubBattle = true;
		}
		//	氈擊唳辦 賅萇 詹幗嫌 羹觼
		else
		{
			GLPARTY *pParty = m_cPartyMan.GetParty(dwPartyID);

			if (pParty && pParty->ISVAILD())
			{
				GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
				GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();
				PGLCHARAG pMemChar = NULL;

				for (; iter != iter_end; ++iter)
				{
					pMemChar = GLAgentServer::GetInstance().GetChar((*iter));
					if (!pMemChar)
						continue;

					if (pCLUB->IsBattle(pMemChar->m_dwGuild))
					{
						bClubBattle = true;
						break;
					}

					GLCLUB *pClub_Mem = cClubMan.GetClub(pMemChar->m_dwGuild);

					if (pClub_Mem && pCLUB->IsBattleAlliance(pClub_Mem->m_dwAlliance))
					{
						bClubBattle = true;
						break;
					}
				}
			}
		}

		if (bClubBattle)
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_CLUBBATTLE;

			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

			return FALSE;
		}
	}

	/*pvp capture the flag, Juver, 2018/02/03 */
	BOOL bCaptureTheFlag = FALSE;

	/* pvp club death match, Juver, 2020/11/26 */
	BOOL bPVPClubDeathMatch = FALSE;

	GLAGLandMan *pLandMan = GetByMapID(pCharMaster->m_sCurMapID);
	if (pLandMan)
	{
		bCaptureTheFlag = pLandMan->m_bPVPCaptureTheFlagMap;

		if (pLandMan->m_bPVPClubDeathMatchLobbyMap || pLandMan->m_bPVPClubDeathMatchBattleMap)
		{
			bPVPClubDeathMatch = TRUE;
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		if (bCaptureTheFlag && pCharMaster && pCharTar)
		{
			if (pCharMaster->m_wCaptureTheFlagTeam != pCharTar->m_wCaptureTheFlagTeam)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHER_TEAM;
				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
				return FALSE;
			}
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		/* pvp club death match, Juver, 2020/11/26 */
		if (bPVPClubDeathMatch && pCharMaster && pCharTar)
		{
			if (pCharMaster->m_dwGuild != pCharTar->m_dwGuild)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHER_GUILD;
				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
				return FALSE;
			}
		}
	}

	/*map party setting, Juver, 2018/06/29 */
	if (pCharMaster && pCharTar)
	{
		GLAGLandMan *pland_master = GetByMapID(pCharMaster->m_sCurMapID);
		GLAGLandMan *pland_target = GetByMapID(pCharTar->m_sCurMapID);

		if (pland_master && pland_master->IsBlockParty())
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_BLOCK_MAP_MASTER;
			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (pland_target && pland_target->IsBlockParty())
		{
			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = EMPARTY_LURE_BLOCK_MAP_TARGET;
			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	// gm character bypass party requirements
	if (pCharTar && pCharTar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER &&
		pCharMaster && pCharMaster->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		//	Note : 棻艇 錳儅婁  唸撩 薯 褫暮檜 氈擊 唳辦 薯.
		if (!GLCONST_CHAR::bPARTY_2OTHERSCHOOL && !bCaptureTheFlag)
		{
			if (pCharTar->m_wSchool != pCharMaster->m_wSchool)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OTHERSCHOOL;

				SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

				return FALSE;
			}
		}
	}

	//	Note : 嬴霜 濠褐曖 陛 虜菟橫 雖雖 彊懊擊 唳辦. 渠鼻縑啪  霤陛 蹂羶.
	//
	if (dwPartyID == PARTY_NULL)
	{
		pCharMaster->m_sReqPartyOpt = pNetMsg->sOption;

		GLMSG::SNET_PARTY_LURE_TAR NetMsgLureTar;
		NetMsgLureTar.dwMasterID = dwGaeaID;
		NetMsgLureTar.dwPartyID = PARTY_NULL;
		NetMsgLureTar.sOption = pCharMaster->m_sReqPartyOpt;
		StringCchCopy(NetMsgLureTar.szCharName, CHAR_SZNAME, pCharMaster->m_szName);
		SENDTOCLIENT(pCharTar->m_dwClientID, &NetMsgLureTar);

		return TRUE;
	}

	GLPARTY *pParty = m_cPartyMan.GetParty(dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMASTER(dwGaeaID))
		return FALSE;

	if (pParty->ISFULL())
	{
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = EMPARTY_LURE_FULL;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

		return FALSE;
	}

	//	Note : 渠鼻濠蒂 賅餵 渠鼻縑 蛔煙.
	//
	pParty->PREADDMEMBER(pCharTar->m_dwGaeaID);

	//	Note : 渠鼻縑啪  霤陛 蹂羶.
	//
	GLMSG::SNET_PARTY_LURE_TAR NetMsgLureTar;
	NetMsgLureTar.dwMasterID = dwGaeaID;
	NetMsgLureTar.dwPartyID = dwPartyID;
	NetMsgLureTar.sOption = pCharMaster->m_sReqPartyOpt;
	StringCchCopy(NetMsgLureTar.szCharName, CHAR_SZNAME, pCharMaster->m_szName);
	SENDTOCLIENT(pCharTar->m_dwClientID, &NetMsgLureTar);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if (pCharMaster->m_bTracingUser)
	{
		CDebugSet::ToTracingFile(pCharMaster->m_szUserName, "Party Request, [%s][%s], Target Char: [%s][%s]",
								 pCharMaster->m_szUserName, pCharMaster->m_szName, pCharTar->m_szUserName, pCharTar->m_szName);
	}
#endif

	return TRUE;
}

BOOL GLAgentServer::MsgPartyLureTarAns(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_LURE_TAR_ANS *pNetMsg = (GLMSG::SNET_PARTY_LURE_TAR_ANS *)nmg;

	PGLCHARAG pCharMaster = GetChar(pNetMsg->dwMasterID);
	if (!pCharMaster)
		return FALSE;

	PGLCHARAG pCharTar = GetChar(dwGaeaID);
	if (!pCharTar)
		return FALSE;

	// 陛 譆蟾煎 掘撩腆陽
	if (pNetMsg->dwPartyID == PARTY_NULL && pCharMaster->GetPartyID() == PARTY_NULL)
	{
		if (pCharTar->GetPartyID() != PARTY_NULL)
			return FALSE;

		if (pNetMsg->emAnswer != EMPARTY_LURE_OK)
		{
			if (pCharMaster->m_sReqPartyOpt != pNetMsg->sOption)
			{
				GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
				NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
				NetMsgFB.dwCharID = pCharTar->m_dwCharID;
				StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
				NetMsgFB.emAnswer = EMPARTY_LURE_OPT;

				SENDTOCLIENT(pCharMaster->GetPartyID(), &NetMsgFB);
				return FALSE;
			}

			GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
			NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
			NetMsgFB.dwCharID = pCharTar->m_dwCharID;
			StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
			NetMsgFB.emAnswer = pNetMsg->emAnswer;

			SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);

			return FALSE;
		}

		/*map party setting, Juver, 2018/06/29 */
		GLAGLandMan *pland_master = GetByMapID(pCharMaster->m_sCurMapID);
		if (pland_master && pland_master->IsBlockParty())
			return FALSE;

		/*map party setting, Juver, 2018/06/29 */
		GLAGLandMan *pland_target = GetByMapID(pCharTar->m_sCurMapID);
		if (pland_target && pland_target->IsBlockParty())
			return FALSE;

		/* log gm party, Juver, 2021/06/13 */
		if (pCharMaster->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_SPECIAL)
		{
			LogGMParty *pdb_action = new LogGMParty(
				pCharMaster->m_dwClientID,
				pCharMaster->m_dwUserID, pCharMaster->m_dwCharID, pCharMaster->m_szName, pCharMaster->m_dwUserLvl,
				pCharTar->m_dwUserID, pCharTar->m_dwCharID, pCharTar->m_szName, pCharTar->m_dwUserLvl);
			m_pDBMan->AddJob(pdb_action);
		}

		/* log gm party, Juver, 2021/06/13 */
		if (pCharTar->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_SPECIAL)
		{
			LogGMParty *pdb_action = new LogGMParty(
				pCharTar->m_dwClientID,
				pCharTar->m_dwUserID, pCharTar->m_dwCharID, pCharTar->m_szName, pCharTar->m_dwUserLvl,
				pCharMaster->m_dwUserID, pCharMaster->m_dwCharID, pCharMaster->m_szName, pCharMaster->m_dwUserLvl);
			m_pDBMan->AddJob(pdb_action);
		}

		//	Note :  儅撩.
		//
		GLPARTY *pParty = m_cPartyMan.NewParty(pNetMsg->dwMasterID);
		if (!pParty)
			return FALSE;

		//	Note :  褫暮 雖薑.
		//
		pParty->m_sOPTION = pNetMsg->sOption;

		//	Note :  掘撩錳縑 蹺陛. ( 羅廓藍 掘撩錳 )
		//
		pCharTar->SetPartyID(pParty->m_dwPARTYID);
		pParty->ADDMEMBER(dwGaeaID);

		//	Note :  儅撩擊 萄縑 憲葡. ( 萄 憮幗朝  薑爾擊 嫡戲賊 億煎檜 蒂 掘撩棻. )
		//
		GLMSG::SNET_PARTY_FNEW NetPartyInfo;
		NetPartyInfo.SETINFO(pParty->m_dwPARTYID, pParty->m_dwMASTER);
		NetPartyInfo.sOption = pParty->m_sOPTION;

		GLPARTY::MEMBER_ITER iter = pParty->m_cMEMBER.begin();
		GLPARTY::MEMBER_ITER iter_end = pParty->m_cMEMBER.end();

		for (; iter != iter_end; ++iter)
		{
			PGLCHARAG pCharMem = GetChar((*iter));
			NetPartyInfo.ADDMEMBER(GLPARTY_FNET(pCharMem->m_dwGaeaID, pCharMem->m_szName, pCharMem->m_emClass, pCharMem->m_wSchool, pCharMem->m_sCurMapID));
		}

		SENDTOALLCHANNEL(&NetPartyInfo);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pCharTar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pCharTar->m_szUserName, "Create Party Answer OK, [%s][%s], Master Char: [%s][%s]",
									 pCharTar->m_szUserName, pCharTar->m_szName, pCharMaster->m_szUserName, pCharMaster->m_szName);
		}
#endif

		return TRUE;
	}
	// 錳擊 蹺陛陽
	else if (pNetMsg->dwMasterID == pCharMaster->m_dwGaeaID)
	{
		GLPARTY *pParty = m_cPartyMan.GetParty(pCharMaster->GetPartyID());
		if (!pParty)
			return FALSE;
		if (pParty->ISFULL())
			return FALSE;

		// 陛 唸撩腎雖 彊擎 鼻縑憮朝 蕨綠詹幗菟擊 蹺陛 熱 橈朝
		// 唳辦陛 嫦儅 熱 氈戲嘎煎 罹晦憮 嬴楚 囀萄蒂 籀葬雖 彊朝棻!
		// if ( !pParty->ISPREMEMBER ( dwGaeaID ) )		return FALSE;

		//	Note : 渠鼻濠蒂 賅餵 渠鼻縑憮 薯諼.
		//
		pParty->PREDELMEMBER(dwGaeaID);

		if (pNetMsg->emAnswer == EMPARTY_LURE_OK)
		{
			if (pCharTar->GetPartyID() != PARTY_NULL)
				return FALSE;

			/*map party setting, Juver, 2018/06/29 */
			GLAGLandMan *pland_master = GetByMapID(pCharMaster->m_sCurMapID);
			if (pland_master && pland_master->IsBlockParty())
				return FALSE;

			/*map party setting, Juver, 2018/06/29 */
			GLAGLandMan *pland_target = GetByMapID(pCharTar->m_sCurMapID);
			if (pland_target && pland_target->IsBlockParty())
				return FALSE;

			/* log gm party, Juver, 2021/06/13 */
			if (pCharMaster->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_SPECIAL)
			{
				LogGMParty *pdb_action = new LogGMParty(
					pCharMaster->m_dwClientID,
					pCharMaster->m_dwUserID, pCharMaster->m_dwCharID, pCharMaster->m_szName, pCharMaster->m_dwUserLvl,
					pCharTar->m_dwUserID, pCharTar->m_dwCharID, pCharTar->m_szName, pCharTar->m_dwUserLvl);
				m_pDBMan->AddJob(pdb_action);
			}

			/* log gm party, Juver, 2021/06/13 */
			if (pCharTar->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_SPECIAL)
			{
				LogGMParty *pdb_action = new LogGMParty(
					pCharTar->m_dwClientID,
					pCharTar->m_dwUserID, pCharTar->m_dwCharID, pCharTar->m_szName, pCharTar->m_dwUserLvl,
					pCharMaster->m_dwUserID, pCharMaster->m_dwCharID, pCharMaster->m_szName, pCharMaster->m_dwUserLvl);
				m_pDBMan->AddJob(pdb_action);
			}

			//	Note :  掘撩錳縑 蹺陛.
			//
			pCharTar->SetPartyID(pCharMaster->GetPartyID());
			pParty->ADDMEMBER(dwGaeaID);

			//	Note : 萄 憮幗縑 錳檜 蹺陛脾擊 憲葡.
			//
			GLMSG::SNET_PARTY_ADD NetPartyAdd;
			NetPartyAdd.dwPartyID = pCharMaster->GetPartyID();
			NetPartyAdd.sPartyInfo = GLPARTY_FNET(pCharTar->m_dwGaeaID, pCharTar->m_szName, pCharTar->m_emClass, pCharTar->m_wSchool, pCharTar->m_sCurMapID);

			SENDTOALLCHANNEL(&NetPartyAdd);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
			if (pCharTar->m_bTracingUser)
			{
				CDebugSet::ToTracingFile(pCharTar->m_szUserName, "Add Party Answer OK, [%s][%s], Master Char: [%s][%s]",
										 pCharTar->m_szUserName, pCharTar->m_szName, pCharMaster->m_szUserName, pCharMaster->m_szName);
			}
#endif
		}

		//	Note :  賅餵 蹂羶 擬港擊 master縑啪 憲葡.
		//
		GLMSG::SNET_PARTY_LURE_FB NetMsgFB;
		NetMsgFB.dwGaeaID = pCharTar->m_dwGaeaID;
		NetMsgFB.dwCharID = pCharTar->m_dwCharID;
		StringCchCopy(NetMsgFB.szCharName, CHAR_SZNAME, pCharTar->m_szName);
		NetMsgFB.emAnswer = pNetMsg->emAnswer;

		SENDTOCLIENT(pCharMaster->m_dwClientID, &NetMsgFB);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgPartySecede(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_SECEDE *pNetMsg = (GLMSG::SNET_PARTY_SECEDE *)nmg;

	GLPARTY *pParty = m_cPartyMan.GetParty(pNetMsg->dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMEMBER(pNetMsg->dwGaeaID))
		return FALSE;

	//	Note : 濠褐 檜諼曖 餌塋檜 驍黴腆 唳辦朝 濰曖 掏檜 氈橫撿 陛棟.
	//
	if ((pNetMsg->dwGaeaID != dwGaeaID) && !pParty->ISMASTER(dwGaeaID))
		return FALSE;

	PGLCHARAG pCharTar = GetChar(pNetMsg->dwGaeaID);
	if (!pCharTar)
		return FALSE;

	//	Note : 錳檜 驍黴 擊 錳菟縑啪 憲葡.
	//
	GLMSG::SNET_PARTY_DEL NetMsgDel;
	NetMsgDel.dwPartyID = pNetMsg->dwPartyID;
	NetMsgDel.dwDelMember = pNetMsg->dwGaeaID;

	SENDTOPARTY(pNetMsg->dwPartyID, (NET_MSG_GENERIC *)&NetMsgDel);

	//	Note : 錳檜 驍黴 擊 萄 憮幗縑 憲葡.
	//
	SENDTOALLCHANNEL(&NetMsgDel);

	//	Note :  驍黴 籀葬.
	//
	pCharTar->ReSetPartyID();
	pParty->DELMEMBER(pNetMsg->dwGaeaID);

	return TRUE;
}

BOOL GLAgentServer::MsgPartyAuthority(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_AUTHORITY *pNetMsg = (GLMSG::SNET_PARTY_AUTHORITY *)nmg;

	GLPARTY *pParty = m_cPartyMan.GetParty(pNetMsg->dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMEMBER(pNetMsg->dwGaeaID))
		return FALSE;

	if ((pNetMsg->dwGaeaID != dwGaeaID) && !pParty->ISMASTER(dwGaeaID))
		return FALSE;

	PGLCHARAG pCharTar = GetChar(pNetMsg->dwGaeaID);
	if (!pCharTar)
		return FALSE;

	//	Note : 濰檜 夥莎 匙擊 錳菟縑啪 憲葡.
	//
	GLMSG::SNET_PARTY_AUTHORITY NetMsgAuth;
	NetMsgAuth.dwPartyID = pNetMsg->dwPartyID;
	NetMsgAuth.dwGaeaID = pNetMsg->dwGaeaID;

	SENDTOPARTY(pNetMsg->dwPartyID, (NET_MSG_GENERIC *)&NetMsgAuth);

	//	Note : 濰檜 夥莎 匙擊擊 萄 憮幗縑 憲葡.
	//
	SENDTOALLCHANNEL(&NetMsgAuth);

	pParty->CHANGEMASTER(pNetMsg->dwGaeaID);

	return TRUE;
}

BOOL GLAgentServer::MsgPartyDissolve(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_DISSOLVE *pNetMsg = (GLMSG::SNET_PARTY_DISSOLVE *)nmg;

	GLPARTY *pParty = m_cPartyMan.GetParty(pNetMsg->dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMASTER(dwGaeaID))
		return FALSE;

	//	Note : 蒂 羹 棻.
	//
	m_cPartyMan.DelParty(pNetMsg->dwPartyID);

	return TRUE;
}

/* modern party window, Juver, 2019/12/20 */
BOOL GLAgentServer::MsgPartyChangeOption(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_PARTY_CHANGEOPTION *pNetMsg = (GLMSG::SNET_PARTY_CHANGEOPTION *)nmg;

	GLPARTY *pParty = m_cPartyMan.GetParty(pNetMsg->dwPartyID);
	if (!pParty)
		return FALSE;
	if (!pParty->ISMASTER(dwGaeaID))
		return FALSE;

	GLMSG::SNET_PARTY_OPTIONTOPARTY NetMsgChange;
	NetMsgChange.dwPartyID = pNetMsg->dwPartyID;
	NetMsgChange.sOption = pNetMsg->sOption;

	SENDTOPARTY(pNetMsg->dwPartyID, (NET_MSG_GENERIC *)&NetMsgChange);

	SENDTOALLCHANNEL(&NetMsgChange);

	pParty->ChangeOption(pNetMsg->sOption);

	return TRUE;
}

BOOL GLAgentServer::MsgReqGateOutFB(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETREQ_GATEOUT_FB *pNetMsg = (GLMSG::SNETREQ_GATEOUT_FB *)nmg;

	//	Note : 贗塭檜樹縑 瞪歎棻.
	SENDTOCLIENT(dwClientID, nmg);

	return TRUE;
}

BOOL GLAgentServer::MsgReqReBirthFB(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_REQ_REBIRTH_FB *pNetMsg = (GLMSG::SNETPC_REQ_REBIRTH_FB *)nmg;

	//	Note : 贗塭檜樹縑 瞪歎棻.
	SENDTOCLIENT(dwClientID, nmg);

	return TRUE;
}

BOOL GLAgentServer::MsgReqCreateInstantMapFB(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETREQ_CREATE_INSTANT_MAP_FB *pNetMsg = (GLMSG::SNETREQ_CREATE_INSTANT_MAP_FB *)nmg;

	//	Note : 贗塭檜樹縑 瞪歎棻.
	SENDTOCLIENT(dwClientID, nmg);

	return TRUE;
}

BOOL GLAgentServer::MsgReqConfront(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_REQ_CONFRONT *pNetMsg = (GLMSG::SNETPC_REQ_CONFRONT *)nmg;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	GLAGLandMan *pLandMan = GetByMapID(pMyChar->m_sCurMapID);
	if (!pLandMan)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pLandMan->m_bGuidBattleMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pLandMan->m_bClubDeathMatchMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*pvp tyranny, Juver, 2017/08/24 */
	if (pLandMan->m_bPVPTyrannyMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*school wars, Juver, 2018/01/19 */
	if (pLandMan->m_bPVPSchoolWarsMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*pvp capture the flag, Juver, 2018/01/31 */
	if (pLandMan->m_bPVPCaptureTheFlagMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/* pvp club death match, Juver, 2020/11/26 */
	if (pLandMan->m_bPVPClubDeathMatchLobbyMap || pLandMan->m_bPVPClubDeathMatchBattleMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pLandMan->m_bPVPPBGLobbyMap || pLandMan->m_bPVPPBGBattleMap)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*woe Arc Development 08-06-2024*/
	if (pLandMan->m_bPVPWoeMap)
	{
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	PGLCHARAG pTarChar = GetChar(pNetMsg->dwID);
	if (!pTarChar)
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (!pNetMsg->sOption.VALID_OPT())
	{
		//	Note : 橾奩 螃盟.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_OPTION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pLandMan->IsPeaceZone())
	{
		//	Note :  雖羲縑憮朝 渠溼 碳陛.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_PEACE;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	DWORD dwMY_PARTYID = pMyChar->GetPartyID();
	if (PARTY_NULL != dwMY_PARTYID)
	{
		GLPARTY *pPARTY = m_cPartyMan.GetParty(dwMY_PARTYID);
		if (pPARTY && pPARTY->ISMEMBER(pTarChar->m_dwGaeaID))
		{
			//	Note : 偽擎  詹幗 郭葬朝 渠溼 碳陛.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_PARTY;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
		}
	}

	const SCONFTING &sMYCONFTING = pMyChar->GETCONFRONTING();
	if (sMYCONFTING.IsCONFRONTING())
	{
		//	Note : (濠褐檜) 檜嘐 渠溼 醞殮棲棻.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_ALREADY_ME;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	const SCONFTING &sTARCONFTING = pTarChar->GETCONFRONTING();
	if (sTARCONFTING.IsCONFRONTING())
	{
		//	Note : (鼻渠寞檜) 檜嘐 渠溼 醞殮棲棻.
		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emFB = EMCONFRONT_ALREADY_TAR;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	switch (pNetMsg->emTYPE)
	{
	case EMCONFT_ONE:
	{
		//	Note : 渠溼 薑爾朝 "渠溼" 謙猿衛 曄濠 賅舒 煎斜蒂 陴晦嘎煎 GaeaID 釭虜 檣賊 醞犒 渠溼 羹觼陛 陛棟.
		//
		DWORD dwCHARID = pMyChar->m_dwCharID;
		const SCONFT_LOG *pCONFT = pTarChar->GETCONFRONT_LOG(dwCHARID);
		if (pCONFT)
		{
			//	Note : 橾薑 衛除寰縑 雖薑熱 檜鼻 渠溼 寞雖.
			if (pCONFT->dwCOUNT >= GLCONST_CHAR::dwCONFRONT_MAX)
			{
				GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
				NetMsgFB.emFB = EMCONFRONT_MAX;
				NetMsgFB.fTIME = GLCONST_CHAR::fCONFRONT_RESET - pCONFT->fTIMER; //	渠溼 陛棟縑 陴擎衛除 : <翱樓渠溼寞雖>葬撢衛除 - 唳婁衛除.
				SENDTOCLIENT(dwClientID, &NetMsgFB);
				return FALSE;
			}

			//	Note : 謙瞪 渠溼 橾薑衛除 唳婁 檜瞪縑 渠溼 寞雖.
			if (pCONFT->fTIMER < GLCONST_CHAR::fCONFRONT_ELAP)
			{
				GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
				NetMsgFB.emFB = EMCONFRONT_ELAP;
				NetMsgFB.fTIME = GLCONST_CHAR::fCONFRONT_ELAP - pCONFT->fTIMER; //	渠溼 陛棟縑 陴擎衛除 : <翱樓渠溼寞雖>葬撢衛除 - 唳婁衛除.
				SENDTOCLIENT(dwClientID, &NetMsgFB);
				return FALSE;
			}
		}

		//	Note : 渠溼 螃羶 薑爾 盪濰.
		//
		pMyChar->m_sREQCONFTING.RESET();
		pMyChar->m_sREQCONFTING.emTYPE = EMCONFT_ONE;
		pMyChar->m_sREQCONFTING.dwTAR_ID = pTarChar->m_dwGaeaID;
		pMyChar->m_sREQCONFTING.sOption = pNetMsg->sOption;

		//	Note : 渠溼 (鼻渠寞縑啪) 蹂羶.
		//
		GLMSG::SNETPC_REQ_CONFRONT_TAR NetMsgTAR;
		NetMsgTAR.dwID = dwGaeaID; // 濠褐曖 ID.
		NetMsgTAR.sOption = pNetMsg->sOption;
		SENDTOCLIENT(pTarChar->m_dwClientID, &NetMsgTAR);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pMyChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pMyChar->m_szUserName, "Confront1 Request, [%s][%s], Target Char: [%s][%s]",
									 pMyChar->m_szUserName, pMyChar->m_szName, pTarChar->m_szUserName, pTarChar->m_szName);
		}
#endif
	}
	break;

	case EMCONFT_PARTY:
	{
		DWORD dwMyPartyID = pMyChar->GetPartyID();
		DWORD dwTarPartyID = pTarChar->GetPartyID();

		if (dwMyPartyID == GAEAID_NULL)
		{
			//	Note : (濠褐檜) 蒂 唸撩雖 彊懊蝗棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_MENOTPARTY;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLPARTY *pMyParty = m_cPartyMan.GetParty(dwMyPartyID);
		if (NULL == pMyParty)
		{
			//	Note : (濠褐檜) 蒂 唸撩雖 彊懊蝗棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_MENOTPARTY;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (FALSE == pMyParty->ISMASTER(pMyChar->m_dwGaeaID))
		{
			//	Note : (濠褐檜)  葆蝶攪陛 嬴椎棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_MENOTMASTER;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (dwTarPartyID == GAEAID_NULL)
		{
			//	Note : (鼻渠寞檜) 蒂 唸撩雖 彊懊蝗棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_TARMASTER;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLPARTY *pTarParty = m_cPartyMan.GetParty(dwTarPartyID);
		if (NULL == pTarParty)
		{
			//	Note : (鼻渠寞檜) 蒂 唸撩雖 彊懊蝗棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_TARMASTER;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (FALSE == pTarParty->ISMASTER(pTarChar->m_dwGaeaID))
		{
			//	Note : (鼻渠寞檜)  葆蝶攪陛 嬴椎棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_TARMASTER;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		//	Note : 渠溼 螃羶 薑爾 盪濰.
		//
		pMyChar->m_sREQCONFTING.RESET();
		pMyChar->m_sREQCONFTING.emTYPE = EMCONFT_PARTY;
		pMyChar->m_sREQCONFTING.dwTAR_ID = pTarChar->m_dwGaeaID;
		pMyChar->m_sREQCONFTING.sOption = pNetMsg->sOption;

		//	Note : 渠溼 (鼻渠寞縑啪) 蹂羶.
		//
		GLMSG::SNETPC_REQ_CONFRONT_TAR NetMsgTAR;
		NetMsgTAR.emTYPE = EMCONFT_PARTY;
		NetMsgTAR.dwID = dwGaeaID; // 濠褐曖 ID.
		NetMsgTAR.sOption = pNetMsg->sOption;
		SENDTOCLIENT(pTarChar->m_dwClientID, &NetMsgTAR);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pMyChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pMyChar->m_szUserName, "Confront2 Request, [%s][%s], Target Char: [%s][%s]",
									 pMyChar->m_szUserName, pMyChar->m_szName, pTarChar->m_szUserName, pTarChar->m_szName);
		}
#endif
	}
	break;

	case EMCONFT_GUILD:
	{
		DWORD dwMyClubID = pMyChar->m_dwGuild;
		DWORD dwTarClubID = pTarChar->m_dwGuild;

		if (dwTarClubID == CLUB_NULL)
		{
			//	Note : 贗毀 葆蝶攪陛 嬴椎棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLCLUB *pTarClub = m_cClubMan.GetClub(dwTarClubID);
		if (!pTarClub || pTarClub->m_dwMasterID != pTarChar->m_dwCharID)
		{
			//	Note : 贗毀 葆蝶攪陛 嬴椎棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (dwMyClubID == CLUB_NULL)
		{
			//	Note : 贗毀 葆蝶攪陛 嬴椎棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		GLCLUB *pMyClub = m_cClubMan.GetClub(dwMyClubID);
		if (!pMyClub || pMyClub->m_dwMasterID != pMyChar->m_dwCharID)
		{
			//	Note : 贗毀 葆蝶攪陛 嬴椎棲棻.
			GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
			NetMsgFB.emFB = EMCONFRONT_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		//	Note : 渠溼 螃羶 薑爾 盪濰.
		//
		pMyChar->m_sREQCONFTING.RESET();
		pMyChar->m_sREQCONFTING.emTYPE = EMCONFT_GUILD;
		pMyChar->m_sREQCONFTING.dwTAR_ID = pTarChar->m_dwGaeaID;
		pMyChar->m_sREQCONFTING.sOption = pNetMsg->sOption;

		//	Note : 渠溼 (鼻渠寞縑啪) 蹂羶.
		//
		GLMSG::SNETPC_REQ_CONFRONT_TAR NetMsgTAR;
		NetMsgTAR.emTYPE = EMCONFT_GUILD;
		NetMsgTAR.dwID = dwGaeaID; // 濠褐曖 ID.
		NetMsgTAR.sOption = pNetMsg->sOption;
		SENDTOCLIENT(pTarChar->m_dwClientID, &NetMsgTAR);

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pMyChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pMyChar->m_szUserName, "Confront3 Request, [%s][%s], Target Char: [%s][%s]",
									 pMyChar->m_szUserName, pMyChar->m_szName, pTarChar->m_szUserName, pTarChar->m_szName);
		}
#endif
	}
	break;
	};

	return TRUE;
}

BOOL GLAgentServer::MsgAnsConfront(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_REQ_CONFRONT_ANS *pNetMsg = (GLMSG::SNETPC_REQ_CONFRONT_ANS *)nmg;

	// Absolute Non-PK State Check. 渠溼 寰脾.
	if (GLCONST_CHAR::bPKLESS)
		return FALSE;

	if (pNetMsg->emFB == EMCONFRONT_REFUSE)
	{
		PGLCHARAG pReqChar = GetChar(pNetMsg->dwID);
		if (!pReqChar)
			return FALSE;

		GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
		NetMsgFB.emTYPE = pNetMsg->emTYPE;
		NetMsgFB.dwID = dwGaeaID;
		NetMsgFB.emFB = EMCONFRONT_REFUSE;

		SENDTOCLIENT(pReqChar->m_dwClientID, &NetMsgFB);
	}
	else if (pNetMsg->emFB == EMCONFRONT_AGREE)
	{
		//	渠溼 晦獄 薑爾.
		const EMCONFT_TYPE emTYPE = pNetMsg->emTYPE;
		const DWORD dwMY_ID = dwGaeaID;
		const DWORD dwREQ_ID = pNetMsg->dwID;
		const SCONFT_OPTION _sOption = pNetMsg->sOption;

		//	渠溼 鼻渠寞 ID 菟.
		const DWORD dwMY_TAR = dwREQ_ID;
		const DWORD dwREQ_TAR = dwMY_ID;

		PGLCHARAG pMyChar = GetChar(dwMY_ID);
		if (!pMyChar)
			return FALSE;

		PGLCHARAG pReqChar = GetChar(dwREQ_ID);
		if (!pReqChar)
			return FALSE;

		const SCONFTING &sMY_CONFTING = pMyChar->GETCONFRONTING();
		const SCONFTING &sREQ_CONFTING = pReqChar->GETCONFRONTING();

		if (sMY_CONFTING.IsCONFRONTING() || sREQ_CONFTING.IsCONFRONTING())
			return FALSE;

		//	Note : 渠溼 褻勒 鼠唸撩 匐餌.
		//
		if (emTYPE != pReqChar->m_sREQCONFTING.emTYPE)
			return FALSE;
		if (dwREQ_TAR != pReqChar->m_sREQCONFTING.dwTAR_ID)
			return FALSE;
		if (_sOption != pReqChar->m_sREQCONFTING.sOption)
			return FALSE;

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if (pMyChar->m_bTracingUser)
		{
			CDebugSet::ToTracingFile(pMyChar->m_szUserName, "Confront Answer OK, [%s][%s], Request Char: [%s][%s]",
									 pMyChar->m_szUserName, pMyChar->m_szName, pReqChar->m_szUserName, pReqChar->m_szName);
		}
#endif

		//	Note : 渠溼 衛濛.
		//
		switch (emTYPE)
		{
		case EMCONFT_ONE:
		{
			SCONFT_OPTION sREQOption = _sOption;		//	A ( 蹂羶濠 )
			SCONFT_OPTION sMYOption = _sOption;			//	B ( 熱塊濠 )
			sMYOption.fHP_RATE = _sOption.fTAR_HP_RATE; //	( 熱塊濠 ) 朝 羹溘 隸陛徽 棻腦偃 陛颶.

			pMyChar->StartConfront(emTYPE, dwMY_TAR, sMYOption);
			pReqChar->StartConfront(emTYPE, dwREQ_TAR, sREQOption);
		}
		break;

		case EMCONFT_PARTY:
		{
			bool bmyparty(false), breqparty(false);
			DWORD dwMyPartyID = pMyChar->GetPartyID();
			DWORD dwReqPartyID = pReqChar->GetPartyID();

			if (dwMyPartyID != GAEAID_NULL)
			{
				GLPARTY *pMyParty = m_cPartyMan.GetParty(dwMyPartyID);
				if (pMyParty && pMyParty->ISMASTER(pMyChar->m_dwGaeaID))
				{
					bmyparty = true;
				}
			}

			if (dwReqPartyID != GAEAID_NULL)
			{
				GLPARTY *pReqParty = m_cPartyMan.GetParty(dwReqPartyID);
				if (pReqParty && pReqParty->ISMASTER(pReqChar->m_dwGaeaID))
				{
					breqparty = true;
				}
			}

			//	Note :  嶸陽 渠溼 衛濛脾.
			//
			if (bmyparty && breqparty)
			{
				//	Note : 渠溼衛濛瞪 霤罹 詹幗 羹觼.
				//
				GLMSG::SNETPC_CONFRONTPTY_CHECKMBR2_FLD NetMsgCheck;
				NetMsgCheck.sMapID = pReqChar->m_sCurMapID;
				NetMsgCheck.dwPARTY_A = dwReqPartyID;
				NetMsgCheck.dwPARTY_B = dwMyPartyID;
				NetMsgCheck.sOption = _sOption;

				SENDTOFIELDSVR(pMyChar->m_nChannel, pMyChar->m_dwCurFieldSvr, &NetMsgCheck);
			}
			else
			{
				//	Note : 陛 嬴棲罹憮 渠溼 碳陛棟.
				//
				GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
				NetMsgFB.emTYPE = emTYPE;
				NetMsgFB.dwID = dwGaeaID;
				NetMsgFB.emFB = EMCONFRONT_FAIL;

				SENDTOCLIENT(pMyChar->m_dwClientID, &NetMsgFB);
				SENDTOCLIENT(pReqChar->m_dwClientID, &NetMsgFB);
			}
		}
		break;

		case EMCONFT_GUILD:
		{
			bool bmyclub(false), breqclub(false);
			DWORD dwMyClubID = pMyChar->m_dwGuild;
			DWORD dwReqClubID = pReqChar->m_dwGuild;

			if (dwMyClubID != CLUB_NULL)
			{
				GLCLUB *pCLUB = m_cClubMan.GetClub(dwMyClubID);
				if (pCLUB && pCLUB->m_dwMasterID == pMyChar->m_dwCharID)
				{
					bmyclub = true;
				}
			}

			if (dwReqClubID != GAEAID_NULL)
			{
				GLCLUB *pCLUB = m_cClubMan.GetClub(dwReqClubID);
				if (pCLUB && pCLUB->m_dwMasterID == pReqChar->m_dwCharID)
				{
					breqclub = true;
				}
			}

			//	Note : 贗毀 嶸陽 渠溼 衛濛脾.
			//
			if (bmyclub && breqclub)
			{
				//	Note : 渠溼衛濛瞪 霤罹 詹幗 羹觼.
				//
				GLMSG::SNET_CONFRONTCLB_CHECKMBR_FLD NetMsgCheck;
				NetMsgCheck.sMapID = pReqChar->m_sCurMapID;
				NetMsgCheck.dwCLUB_A = dwReqClubID;
				NetMsgCheck.dwCLUB_B = dwMyClubID;
				NetMsgCheck.sOption = _sOption;

				SENDTOFIELDSVR(pMyChar->m_nChannel, pMyChar->m_dwCurFieldSvr, &NetMsgCheck);
			}
			else
			{
				//	Note : 贗毀檜 嬴棲罹憮 渠溼 碳陛棟.
				//
				GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
				NetMsgFB.emTYPE = emTYPE;
				NetMsgFB.dwID = dwGaeaID;
				NetMsgFB.emFB = EMCONFRONT_FAIL;

				SENDTOCLIENT(pMyChar->m_dwClientID, &NetMsgFB);
				SENDTOCLIENT(pReqChar->m_dwClientID, &NetMsgFB);
			}
		}
		break;
		};
	}

	return TRUE;
}

BOOL GLAgentServer::MsgConfrontPtyCheckMber(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_CONFRONTPTY_CHECKMBR2_AGT *pNetMsg = (GLMSG::SNETPC_CONFRONTPTY_CHECKMBR2_AGT *)nmg;

	StartPartyConfront(pNetMsg->dwPARTY_A, pNetMsg->dwPARTY_B, pNetMsg->sMapID, pNetMsg->sOption,
					   pNetMsg->dwPARTY_A_MEMBER, pNetMsg->dwPARTY_B_MEMBER, pNetMsg->vPosition);

	return TRUE;
}

BOOL GLAgentServer::MsgConfrontClbCheckMber(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CONFRONTCLB_CHECKMBR_AGT *pNetMsg = (GLMSG::SNET_CONFRONTCLB_CHECKMBR_AGT *)nmg;

	GLCLUB *pClubA = m_cClubMan.GetClub(pNetMsg->dwCLUB_A);
	GLCLUB *pClubB = m_cClubMan.GetClub(pNetMsg->dwCLUB_B);
	if (!pClubA || !pClubB)
		return FALSE;

	CLUBCONFT_ITER pos, end;

	SCONFT_OPTION sOPT_A = pNetMsg->sOption;
	SCONFT_OPTION sOPT_B = pNetMsg->sOption;

	DWORD i(0);

	//	Note : A  '贗毀渠溼' 蟾晦.
	//
	pClubA->CLEARCONFT();

	for (i = 0; i < pNetMsg->dwCLUB_A_NUM; ++i)
	{
		DWORD dwMBR = pNetMsg->dwCLUB_A_MEMBER[i];

		PGLCHARAG pChar = GetCharID(dwMBR);
		if (!pChar)
			continue;
		if (pChar->m_sCurMapID != pNetMsg->sMapID)
			continue;

		//	Note :贗毀錳曖 渠溼薑爾 撲薑.
		//
		pChar->m_sCONFTING.RESET();
		pChar->m_sCONFTING.emTYPE = EMCONFT_GUILD;
		pChar->m_sCONFTING.dwTAR_ID = pNetMsg->dwCLUB_B;
		pChar->m_sCONFTING.sOption = sOPT_A;

		pClubA->ADDCONFT_MEMBER(dwMBR);
	}

	if (pClubA->GETCONFT_NUM() == 0)
		return FALSE;

	//	Note : B  '贗毀渠溼' 蟾晦.
	//
	pClubB->CLEARCONFT();

	for (i = 0; i < pNetMsg->dwCLUB_B_NUM; ++i)
	{
		DWORD dwMBR = pNetMsg->dwCLUB_B_MEMBER[i];

		PGLCHARAG pChar = GetCharID(dwMBR);
		if (!pChar)
			continue;
		if (pChar->m_sCurMapID != pNetMsg->sMapID)
			continue;

		//	Note : 贗毀錳曖 渠溼薑爾 撲薑.
		//
		pChar->m_sCONFTING.RESET();
		pChar->m_sCONFTING.emTYPE = EMCONFT_GUILD;
		pChar->m_sCONFTING.dwTAR_ID = pNetMsg->dwCLUB_A;
		pChar->m_sCONFTING.sOption = sOPT_B;

		pClubB->ADDCONFT_MEMBER(dwMBR);
	}

	if (pClubB->GETCONFT_NUM() == 0)
		return FALSE;

	//	Note : 贗毀渠溼 薑爾 蟾晦.
	//
	pClubA->m_conftOPT = sOPT_A;
	pClubA->m_dwconftCLUB = pNetMsg->dwCLUB_B;
	pClubA->m_fconftTIMER = 0.0f;
	pClubA->SETCONFT_MEMBER();

	pClubB->m_conftOPT = sOPT_B;
	pClubB->m_dwconftCLUB = pNetMsg->dwCLUB_A;
	pClubB->m_fconftTIMER = 0.0f;
	pClubB->SETCONFT_MEMBER();

	//	Note : 萄縑 渠溼 蟾晦 詭衛雖.
	//		詭衛雖縑 瞪歎脹 my, tar school 擎 餌辨腎雖 彊朝棻.
	GLMSG::SNETPC_CONFRONTCLB_START2_FLD NetMsgFld;
	NetMsgFld.sMapID = pNetMsg->sMapID;
	NetMsgFld.dwCLUB_A = pNetMsg->dwCLUB_A;
	NetMsgFld.dwCLUB_B = pNetMsg->dwCLUB_B;
	NetMsgFld.sOption = sOPT_A;
	NetMsgFld.vPos = pNetMsg->vPosition;

	pos = pClubA->m_setConftCURRENT.begin();
	end = pClubA->m_setConftCURRENT.end();
	for (; pos != end; ++pos)
	{
		NetMsgFld.dwCLUB_A_MEMBER[NetMsgFld.dwCLUB_A_NUM++] = (*pos);
	}

	pos = pClubB->m_setConftCURRENT.begin();
	end = pClubB->m_setConftCURRENT.end();
	for (; pos != end; ++pos)
	{
		NetMsgFld.dwCLUB_B_MEMBER[NetMsgFld.dwCLUB_B_NUM++] = (*pos);
	}

	SENDTOALLCHANNEL(&NetMsgFld);

	return TRUE;
}

BOOL GLAgentServer::MsgAnsConfrontFB(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_REQ_CONFRONT_FB *pNetMsg = (GLMSG::SNETPC_REQ_CONFRONT_FB *)nmg;

	switch (pNetMsg->emTYPE)
	{
	case EMCONFT_ONE:
	{
		PGLCHARAG pCHAR = GetChar(pNetMsg->dwID);
		if (pCHAR->m_sCONFTING.IsCONFRONTING())
		{
			PGLCHARAG pTAR = GetChar(pCHAR->m_sCONFTING.dwTAR_ID);
			if (pTAR)
			{
				pTAR->m_sCONFTING.RESET();

				// Note : 贗塭檜樹縑 渠溼 螃盟 歎褐.
				if (pNetMsg->emFB == EMCONFRONT_FAIL)
					SENDTOCLIENT(pTAR->m_dwClientID, pNetMsg);
			}
		}

		pCHAR->m_sCONFTING.RESET();

		//	Note : 贗塭檜樹縑 渠溼 螃盟 歎褐.
		SENDTOCLIENT(pCHAR->m_dwClientID, pNetMsg);
	}
	break;

	case EMCONFT_PARTY:
	{
		m_cPartyMan.ConfrontFB((NET_MSG_GENERIC *)pNetMsg);
	}
	break;

	case EMCONFT_GUILD:
	{
		GLCLUB *pCLUB = m_cClubMan.GetClub(pNetMsg->dwID);
		if (pCLUB)
		{
			pCLUB->CLEARCONFT();

			CLUBMEMBERS_ITER pos = pCLUB->m_mapMembers.begin();
			CLUBMEMBERS_ITER end = pCLUB->m_mapMembers.end();
			for (; pos != end; ++pos)
			{
				PGLCHARAG pCHAR = GetCharID((*pos).first);
				if (!pCHAR)
					continue;

				pCHAR->m_sCONFTING.RESET();

				//	Note : 贗塭檜樹縑 渠溼 螃盟 歎褐.
				//
				GLMSG::SNETPC_REQ_CONFRONT_FB NetMsgFB;
				NetMsgFB.emFB = pNetMsg->emFB;
				SENDTOCLIENT(pCHAR->m_dwClientID, &NetMsgFB);
			}
		}
	}
	break;
	};

	return TRUE;
}

BOOL GLAgentServer::MsgConfrontEnd2Agt(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_CONFRONT_END2_AGT *pNetMsg = (GLMSG::SNETPC_CONFRONT_END2_AGT *)nmg;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	const SCONFTING &sCONFT = pMyChar->GETCONFRONTING();

	switch (sCONFT.emTYPE)
	{
	case EMCONFT_ONE:
	{
		DWORD dwTAR_ID = sCONFT.dwTAR_ID;
		pMyChar->ResetConfrontOk();

		PGLCHARAG pTarChar = GetChar(dwTAR_ID);
		if (!pTarChar)
			return TRUE;
		pTarChar->ResetConfrontOk();
	}
	break;

	case EMCONFT_PARTY:
	{
		pMyChar->ResetConfrontOk();

		GLPARTY *pParty = GetParty(pMyChar->GetPartyID());
		if (!pParty)
			return FALSE;

		pParty->DELCONFT_MEMBER(pMyChar->m_dwGaeaID);
	}
	break;

	case EMCONFT_GUILD:
	{
		pMyChar->ResetConfrontOk();

		GLCLUB *pCLUB = m_cClubMan.GetClub(pMyChar->m_dwGuild);
		if (!pCLUB)
			return FALSE;

		pCLUB->DELCONFT_MEMBER(pMyChar->m_dwCharID);
	}
	break;
	};

	return TRUE;
}

BOOL GLAgentServer::MsgServerCtrlWeather(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	SENDTOALLCHANNEL(nmg);

	GLMSG::SNETSERVER_CTRL_WEATHER *pNetMsg = (GLMSG::SNETSERVER_CTRL_WEATHER *)nmg;

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "weather all active:%d weather:%u", pNetMsg->bActive, pNetMsg->dwWeather);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgServerCtrlWeather2(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	SENDTOALLCHANNEL(nmg);

	GLMSG::SNETSERVER_CTRL_WEATHER2 *pNetMsg = (GLMSG::SNETSERVER_CTRL_WEATHER2 *)nmg;

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "weather map[%u/%u] weather:%u apply time:%u", pNetMsg->map_mID, pNetMsg->map_sID, pNetMsg->dwWeather, pNetMsg->dwApplyTime);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgServerCtrlGenHold(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	SENDTOALLCHANNEL(nmg);

	GLMSG::SNETSERVER_CTRL_GENITEMHOLD *pNetMsg = (GLMSG::SNETSERVER_CTRL_GENITEMHOLD *)nmg;

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "itemhold %u", pNetMsg->bHold);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgServerCtrlTime(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNETSERVER_CTRL_TIME *pNetMsg = (GLMSG::SNETSERVER_CTRL_TIME *)nmg;
	GLPeriod::GetInstance().SetHour(pNetMsg->dwTime);

	SENDTOALLCHANNEL(pNetMsg);

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "time :%u", pNetMsg->dwTime);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgServerCtrlMonth(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNETSERVER_CTRL_MONTH *pNetMsg = (GLMSG::SNETSERVER_CTRL_MONTH *)nmg;

	// GLPeriod::GetInstance();

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "month :%u", pNetMsg->dwMonth);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgBlockDetected(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	GLMSG::SNET_BLOCK_DETECTED *pNetMsg = (GLMSG::SNET_BLOCK_DETECTED *)nmg;
	if (pMyChar->m_dwCharID == pNetMsg->dwCHARID)
	{
		std::string strTIME = CDebugSet::GetCurTime();
		CDebugSet::ToFile("block_detected.txt", "%s	userid= %s (%d), charid= %s (%d), detect=%d, %s",
						  strTIME.c_str(),
						  pMyChar->m_szUserName,
						  pMyChar->m_dwUserID, pMyChar->m_szName, pMyChar->m_dwCharID, pNetMsg->dwDETECTED, pNetMsg->szINFO);

		CLogHackProgram *pDbAction = new CLogHackProgram(
			pMyChar->m_dwUserID,
			pMyChar->m_dwCharID,
			(int)pNetMsg->dwDETECTED,
			pNetMsg->szINFO);
		GLDBMan *pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if (pDbMan)
			pDbMan->AddJob(pDbAction);
	}

	return TRUE;
}

// *****************************************************
// Desc: 馨葛攪 鬼薯 蕾樓 謙猿 蹂羶 籀葬
// *****************************************************
BOOL GLAgentServer::MsgGmKickUser(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_KICK_USER *pNetMsg = (GLMSG::SNET_GM_KICK_USER *)nmg;

	PGLCHARAG pTarChar = GetCharID(pNetMsg->dwID);
	if (!pTarChar)
		return FALSE;

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar && pTarChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "kickuser [%u]%s in map [%u/%u]", pTarChar->m_dwCharID, pTarChar->m_szName, pTarChar->m_sCurMapID.wMainID, pTarChar->m_sCurMapID.wSubID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	if (pNetMsg->bGaeaID)
	{
		for (DWORD i = 0; i < m_dwMaxClient; i++)
		{
			if (i == dwGaeaID)
				continue;
			PGLCHARAG pTarChar = m_PCArray[i];
			if (pTarChar)
			{
				GLMSG::SNET_GM_KICK_USER_PROC_FLD NetMsgFld;
				NetMsgFld.dwID = pTarChar->m_dwCharID;
				SENDTOALLCHANNEL(&NetMsgFld);

				GLMSG::SNET_GM_KICK_USER_PROC NetMsgProc;
				SENDTOCLIENT(pTarChar->m_dwClientID, &NetMsgProc);
				ReserveDropOutPC(i);
			}
		}
	}
	else if (pNetMsg->bName)
	{
		PGLCHARAG pTarChar = GetChar(pNetMsg->szNAME);
		if (!pTarChar)
			return FALSE;

		GLMSG::SNET_GM_KICK_USER_PROC_FLD NetMsgFld;
		NetMsgFld.dwID = pTarChar->m_dwCharID;
		SENDTOALLCHANNEL(&NetMsgFld);

		GLMSG::SNET_GM_KICK_USER_PROC NetMsgProc;
		SENDTOCLIENT(pTarChar->m_dwClientID, &NetMsgProc);

		ReserveDropOutPC(pTarChar->m_dwGaeaID);
	}
	else
	{
		PGLCHARAG pTarChar = GetCharID(pNetMsg->dwID);
		if (!pTarChar)
			return FALSE;

		GLMSG::SNET_GM_KICK_USER_PROC_FLD NetMsgFld;
		NetMsgFld.dwID = pNetMsg->dwID;
		SENDTOALLCHANNEL(&NetMsgFld);

		GLMSG::SNET_GM_KICK_USER_PROC NetMsgProc;
		SENDTOCLIENT(pTarChar->m_dwClientID, &NetMsgProc);
		ReserveDropOutPC(pTarChar->m_dwGaeaID);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGMMove2Gate(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;
	if (pCHAR->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
		return FALSE;

	GLMSG::SNET_GM_MOVE2GATE *pNetMsg = (GLMSG::SNET_GM_MOVE2GATE *)nmg;

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "move2gate in map [%u/%u] gate:%u", pCHAR->m_sCurMapID.wMainID, pCHAR->m_sCurMapID.wSubID, pNetMsg->dwGATE);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pCHAR->m_dwClientID, pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
	}

	GLMSG::SNET_GM_MOVE2GATE_FLD NetMsgFld;
	NetMsgFld.dwGATE = pNetMsg->dwGATE;
	NetMsgFld.dwGaeaID = dwGaeaID;

	SENDTOALLCHANNEL(&NetMsgFld);

	return TRUE;
}

// *****************************************************
// Desc: 薑裘曖 薑嬪纂縑 檜翕 蹂羶 籀葬
// *****************************************************
BOOL GLAgentServer::MsgGMMove2Pos(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;
	if (pCHAR->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
		return FALSE;

	GLMSG::SNET_GM_MOVE2MAPPOS *pNetMsg = (GLMSG::SNET_GM_MOVE2MAPPOS *)nmg;

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "move2pos in map [%u/%u] to map [%u/%u] x:%u y:%u", pCHAR->m_sCurMapID.wMainID, pCHAR->m_sCurMapID.wSubID, pNetMsg->nidMAP.wMainID, pNetMsg->nidMAP.wSubID, pNetMsg->dwPOSX, pNetMsg->dwPOSY);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pCHAR->m_dwClientID, pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
	}

	GLMSG::SNET_GM_MOVE2MAPPOS_FLD NetMsgFld;
	NetMsgFld.nidMAP = pNetMsg->nidMAP;
	NetMsgFld.dwGaeaID = dwGaeaID;
	NetMsgFld.dwPOSX = pNetMsg->dwPOSX;
	NetMsgFld.dwPOSY = pNetMsg->dwPOSY;

	SENDTOALLCHANNEL(&NetMsgFld);

	return TRUE;
}

BOOL GLAgentServer::MsgServerGeneralChat(NET_MSG_GENERIC *nmg)
{
	SENDTOALLCLIENT(nmg);

	return TRUE;
}

BOOL GLAgentServer::ChatMsgLoudSpeakerAgt(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_CHAT_LOUDSPEAKER_AGT *pNetMsg = (GLMSG::SNETPC_CHAT_LOUDSPEAKER_AGT *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;
	if (pChar->IsCHATBLOCK())
		return FALSE;

	NET_CHAT_FB NetMsgFB;
	NetMsgFB.emType = CHAT_TYPE_LOUDSPEAKER;
	NetMsgFB.sItemLink = pNetMsg->sItemLink; /*item link, Juver, 2017/07/31 */
	StringCchCopy(NetMsgFB.szName, CHAR_SZNAME, pChar->m_szName);
	StringCchCopy(NetMsgFB.szChatMsg, CHAT_MSG_SIZE, pNetMsg->szMSG);
	/* Chat Color, Mhundz 02/22/25 */
	if (pChar->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_MASTER)
	{
		NetMsgFB.bStaff = true;
	}

	if (pNetMsg->bchannel_all)
		SENDTOALLCLIENT(&NetMsgFB);
	else
		SENDTOALLCLIENT(&NetMsgFB, pChar->m_nChannel);

	return TRUE;
}

BOOL GLAgentServer::MsgClubNew2Agt(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_CLUB_NEW_2AGT *pNetMsg = (GLMSG::SNET_CLUB_NEW_2AGT *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNET_CLUB_NEW_FB NetMsgFB;
	NetMsgFB.emFB = EMCLUB_NEW_FB_FAIL;
	StringCchCopy(NetMsgFB.szClubName, CHAR_SZNAME, pNetMsg->szClubName);

	if (pChar->m_dwGuild != CLUB_NULL)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	GLPARTY *pPARTY = m_cPartyMan.GetParty(pChar->m_dwPartyID);
	if (!pPARTY || !pPARTY->ISMASTER(pChar->m_dwGaeaID))
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_NOTMASTER;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pPARTY->GETNUMBER() < GLCONST_CHAR::dwCLUB_PARTYNUM)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_NOTMEMBER;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return S_FALSE;
	}

	GLPARTY::MEMBER_ITER pos = pPARTY->m_cMEMBER.begin();
	GLPARTY::MEMBER_ITER end = pPARTY->m_cMEMBER.end();
	for (; pos != end; ++pos)
	{
		PGLCHARAG pMEMBER = GetChar((*pos));
		if (!pMEMBER)
		{
			NetMsgFB.emFB = EMCLUB_NEW_FB_NOTEXIST_MEM;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		if (pMEMBER->m_dwGuild != CLUB_NULL)
		{
			NetMsgFB.emFB = EMCLUB_NEW_FB_MEM_ALREADY;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}
	}

	//	Note : 褐羶 贗毀 檜葷檜 檜嘐 餌辨醞.
	//
	DWORD dwOLDGLUB = m_cClubMan.FindClub(pNetMsg->szClubName);
	if (dwOLDGLUB != CLUB_NULL)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_NAME;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : DB縑 等檜攪 撲薑.
	//
	CCreateClub *pDbAction = new CCreateClub(dwClientID, pNetMsg->szClubName, pChar->m_dwCharID);
	m_pDBMan->AddJob(pDbAction);

	return TRUE;
}

BOOL GLAgentServer::MsgClubNewDb2Agt(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_CLUB_NEW_DB2AGT *pNetMsg = (GLMSG::SNET_CLUB_NEW_DB2AGT *)nmg;

	PGLCHARAG pChar = GetCharID(pNetMsg->dwMasterID);
	if (!pChar)
	{
		//	Note : db縑 贗毀擊 虜菟歷戲釭 贗毀 葆蝶攪陛 襄薯雖 彊擊 陽朝 贗毀擊 餉薯棻.
		CDeleteClub *pDbAction = new CDeleteClub(pNetMsg->dwClubID, pNetMsg->dwMasterID);
		m_pDBMan->AddJob(pDbAction);
		return FALSE;
	}

	GLMSG::SNET_CLUB_NEW_FB NetMsgFB;

	if (pNetMsg->emFB == EMCLUB_NEW_FB_ALREADY)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_ALREADY;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pNetMsg->emFB == EMCLUB_NEW_FB_NAME)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_NAME;
		StringCchCopy(NetMsgFB.szClubName, CHAR_SZNAME, pNetMsg->szClubName);
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pNetMsg->emFB == EMCLUB_NEW_FB_FAIL)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	NetMsgFB.emFB = EMCLUB_NEW_FB_FAIL;
	StringCchCopy(NetMsgFB.szClubName, CHAR_SZNAME, pNetMsg->szClubName);

	GLPARTY *pPARTY = m_cPartyMan.GetParty(pChar->m_dwPartyID);
	if (!pPARTY || !pPARTY->ISMASTER(pChar->m_dwGaeaID))
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_NOTMASTER;
		SENDTOCLIENT(dwClientID, &NetMsgFB);

		CDeleteClub *pDbAction = new CDeleteClub(pNetMsg->dwClubID, pNetMsg->dwMasterID);
		m_pDBMan->AddJob(pDbAction);

		return FALSE;
	}

	if (pPARTY->GETNUMBER() < GLCONST_CHAR::dwCLUB_PARTYNUM)
	{
		NetMsgFB.emFB = EMCLUB_NEW_FB_NOTMEMBER;
		SENDTOCLIENT(dwClientID, &NetMsgFB);

		CDeleteClub *pDbAction = new CDeleteClub(pNetMsg->dwClubID, pNetMsg->dwMasterID);
		m_pDBMan->AddJob(pDbAction);

		return S_FALSE;
	}

	//	Note : 贗毀 撲薑.
	//
	GLCLUB cClubNew;
	cClubNew.m_dwID = pNetMsg->dwClubID;
	StringCchCopy(cClubNew.m_szName, CHAR_SZNAME, pNetMsg->szClubName);

	CTime cTIME_CUR = CTime::GetCurrentTime();
	cClubNew.m_tOrganize = cTIME_CUR.GetTime();

	cClubNew.m_dwMasterID = pChar->m_dwCharID;
	StringCchCopy(cClubNew.m_szMasterName, CHAR_SZNAME, pChar->m_szName);

	//	Note : 馨葛攪 偃羹縑 贗毀 ID 雖薑.
	// pChar->m_dwGuild = cClubNew.m_dwID;

	CTime cTIME_DIS(1970, 2, 1, 1, 1, 1);
	cClubNew.m_tDissolution = cTIME_DIS.GetTime();

	//	Note : 詹幗錳 蛔煙.
	{
		GLPARTY::MEMBER_ITER pos = pPARTY->m_cMEMBER.begin();
		GLPARTY::MEMBER_ITER end = pPARTY->m_cMEMBER.end();
		for (; pos != end; ++pos)
		{
			PGLCHARAG pMEMBER = GetChar((*pos));
			if (!pMEMBER)
				continue;
			if (pMEMBER->m_dwGuild != CLUB_NULL)
				continue;

			//	Note : 馨葛攪 偃羹縑 贗毀 ID 雖薑.
			pMEMBER->m_dwGuild = cClubNew.m_dwID;

			GLCLUBMEMBER cMEMBER;
			cMEMBER.dwID = pMEMBER->m_dwCharID;
			StringCchCopy(cMEMBER.m_szName, CHAR_SZNAME, pMEMBER->m_szName);

			cClubNew.m_mapMembers.insert(std::make_pair(cMEMBER.dwID, cMEMBER));

			//	Note : DB縑 詹幗 撲薑.
			//
			CAddClubMember *pDbAction = new CAddClubMember(cClubNew.m_dwID, cMEMBER.dwID);
			m_pDBMan->AddJob(pDbAction);
		}
	}

	m_cClubMan.AddClub(cClubNew);

	//	Note : 贗毀 儅撩 撩奢.
	GLMSG::SNET_CLUB_NEW_2FLD NetMsgFLD;
	NetMsgFLD.emFB = EMCLUB_NEW_FB_OK;
	NetMsgFLD.dwCLUBID = cClubNew.m_dwID;
	StringCchCopy(NetMsgFLD.szClubName, CHAR_SZNAME, cClubNew.m_szName);
	SENDTOFIELD(dwClientID, &NetMsgFLD);

	//	Note : 儅撩 薑爾 瞪歎.
	//
	GLMSG::SNET_CLUB_INFO_2FLD NetMsgInfo;
	NetMsgInfo.dwClubID = cClubNew.m_dwID;
	StringCchCopy(NetMsgInfo.szClubName, CHAR_SZNAME, cClubNew.m_szName);

	NetMsgInfo.dwMasterID = cClubNew.m_dwMasterID;
	StringCchCopy(NetMsgInfo.szMasterName, CHAR_SZNAME, cClubNew.m_szMasterName);
	NetMsgInfo.dwCDCertifior = cClubNew.m_dwCDCertifior;

	NetMsgInfo.dwRank = cClubNew.m_dwRank;
	NetMsgInfo.dwMarkVER = cClubNew.m_dwMarkVER;

	NetMsgInfo.tOrganize = cClubNew.m_tOrganize;
	NetMsgInfo.tDissolution = cClubNew.m_tDissolution;

	NetMsgInfo.dwMemberNum = cClubNew.GetNemberNum();
	{
		DWORD i = 0;
		CLUBMEMBERS_ITER pos = cClubNew.m_mapMembers.begin();
		CLUBMEMBERS_ITER end = cClubNew.m_mapMembers.end();
		for (; pos != end; ++pos)
		{
			const GLCLUBMEMBER &sCLUBMEMBER = (*pos).second;

			StringCchCopy(NetMsgInfo.szMemberName[i], CHAR_SZNAME, sCLUBMEMBER.m_szName);
			NetMsgInfo.aMembers[i] = sCLUBMEMBER.dwID;
			++i;

			//	Note : 贗毀 陛殮 煎斜 晦煙.
			GLITEMLMT::GetInstance().ReqAction(
				cClubNew.m_dwMasterID,	   //	渡餌濠.
				EMLOGACT_CLUB_JOIN_MEMBER, //	嬪.
				ID_CHAR, sCLUBMEMBER.dwID, //	鼻渠寞.
				0,						   //	exp
				0,						   //	bright
				0,						   //	life
				0						   //	money
			);
		}
	}
	SENDTOALLCHANNEL(&NetMsgInfo);

	return TRUE;
}

BOOL GLAgentServer::MsgClubDissolution(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_CLUB_DISSOLUTION *pNetMsg = (GLMSG::SNET_CLUB_DISSOLUTION *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNET_CLUB_DISSOLUTION_FB NetMsgFB;

	if (pChar->m_dwGuild == CLUB_NULL)
	{
		NetMsgFB.emFB = EMCLUB_DIS_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	GLCLUB *pCLUB = m_cClubMan.GetClub(pChar->m_dwGuild);
	if (!pCLUB)
	{
		NetMsgFB.emFB = EMCLUB_DIS_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (!pCLUB->IsMaster(pChar->m_dwCharID))
	{
		NetMsgFB.emFB = EMCLUB_DIS_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Memo :	翕裊縑 陛殮腎橫 氈棻賊 鏃模.
	if (pCLUB->IsAlliance())
	{
		NetMsgFB.emFB = EMCLUB_DIS_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pCLUB->GetAllBattleNum() > 0)
	{
		NetMsgFB.emFB = EMCLUB_DIS_FB_CLUBBATTLE;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pNetMsg->bCANCEL)
	{
		if (!pCLUB->IsRegDissolution())
		{
			NetMsgFB.emFB = EMCLUB_DIS_FB_FAIL;
			SENDTOCLIENT(dwClientID, &NetMsgFB);
			return FALSE;
		}

		pCLUB->DoDissolutionCancel();

		//	Note : DB縑 贗毀 骯 衛除 撲薑.
		CSetClubDissolutionTime *pDbAction = new CSetClubDissolutionTime(pCLUB->m_dwID, pCLUB->m_tDissolution);
		m_pDBMan->AddJob(pDbAction);

		//	Note : 羹 鏃模 to FIELD
		//
		GLMSG::SNET_CLUB_DISSOLUTION_2FLD NetMsgFld;
		NetMsgFld.dwClubID = pCLUB->m_dwID;
		CTime tDisTime(1970, 2, 1, 1, 1, 1);
		NetMsgFld.tDissolution = tDisTime.GetTime();
		SENDTOALLCHANNEL(&NetMsgFld);

		//	Note : 羹 鏃模 FB
		//
		NetMsgFB.emFB = EMCLUB_DIS_FB_CANCEL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
	}
	else
	{
		//	Note : 羹 蕨薑橾 雖薑.
		CTime tDisTime = CTime::GetCurrentTime();
		CTimeSpan tADD(GLCONST_CHAR::dwCLUB_DISSOLUTION_DAY, 0, 0, 0);
		tDisTime += tADD;

		pCLUB->DoDissolution(tDisTime.GetTime());

		//	Note : DB縑 贗毀 骯 衛除 撲薑.
		CSetClubDissolutionTime *pDbAction = new CSetClubDissolutionTime(pCLUB->m_dwID, pCLUB->m_tDissolution);
		m_pDBMan->AddJob(pDbAction);

		//	Note : 羹 FB.
		//
		NetMsgFB.emFB = EMCLUB_DIS_FB_OK;
		NetMsgFB.tDissolution = pCLUB->m_tDissolution;
		SENDTOCLIENT(dwClientID, &NetMsgFB);

		//	Note : 羹 to FIELD
		//
		GLMSG::SNET_CLUB_DISSOLUTION_2FLD NetMsgFld;
		NetMsgFld.dwClubID = pCLUB->m_dwID;
		NetMsgFld.tDissolution = pCLUB->m_tDissolution;
		SENDTOALLCHANNEL(&NetMsgFld);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgClubCertified(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_CLUB_CERTIFIED_AGT *pNetMsg = (GLMSG::SNET_CLUB_CERTIFIED_AGT *)nmg;

	bool bOK = GLGuidanceAgentMan::GetInstance().ChangeGuidClub(pNetMsg->dwID, pNetMsg->dwCLUBID);
	if (!bOK)
		return FALSE;

	//	Note : 萄憮幗縑 憲葡.
	//
	GLMSG::SNET_CLUB_CERTIFIED_FLD NetMsgFld;
	NetMsgFld.dwID = pNetMsg->dwID;
	NetMsgFld.dwCLUBID = pNetMsg->dwCLUBID;
	SENDTOALLCHANNEL(&NetMsgFld);

	//	Note : 贗塭檜樹縑啪 憲葡.
	//
	std::string strGuidName, strClubName;
	GLGuidance *pGuid = GLGuidanceAgentMan::GetInstance().Find(pNetMsg->dwID);
	if (pGuid)
		strGuidName = pGuid->m_strName;

	GLCLUB *pCLUB = m_cClubMan.GetClub(pNetMsg->dwCLUBID);
	if (pCLUB)
		strClubName = pCLUB->m_szName;

	//	Note : 贗塭檜樹縑 憲葡.
	CString strTEXT;
	strTEXT.Format(ID2SERVERTEXT("EMGUIDCLUB_CERTIFIED"), strClubName.c_str(), strGuidName.c_str());

	GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
	NetMsg.SETTEXT(strTEXT.GetString());
	SENDTOALLCLIENT(&NetMsg);

	return TRUE;
}

BOOL GLAgentServer::MsgClubIncomeMoney(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CLUB_INCOME_MONEY_AGT *pNetMsg = (GLMSG::SNET_CLUB_INCOME_MONEY_AGT *)nmg;

	GLCLUB *pCLUB = m_cClubMan.GetClub(pNetMsg->dwID);
	if (!pCLUB)
		return FALSE;

	if (pNetMsg->lnMoney < 0)
		return FALSE;

	//	Note : 贗毀曖 熱櫛擊 奩艙.
	pCLUB->m_lnIncomeMoney += LONGLONG(pNetMsg->lnMoney * GLCONST_CHAR::fEARNING_RATE + 0.1f);

	GLITEMLMT::GetInstance().ReqMoneyExc(ID_CLUB, pNetMsg->dwID,
										 ID_CLUB, pNetMsg->dwID,
										 pNetMsg->lnMoney,
										 EMITEM_ROUTE_CLUBINCOME);

	CDebugSet::ToFile("club_income_agent.txt", "{MsgClubIncomeMoney}, ClubID[%u], MoneyFromField[%I64d], IncomeMoney[%I64d]",
					  pNetMsg->dwID, pNetMsg->lnMoney, pCLUB->m_lnIncomeMoney);

	//	Note : Db縑 盪濰.
	CSetClubIncomeMoney *pDbAction = new CSetClubIncomeMoney(pCLUB->m_dwID, pCLUB->m_lnIncomeMoney);
	m_pDBMan->AddJob(pDbAction);

	return TRUE;
}

BOOL GLAgentServer::MsgClubIncomeMoneyUp(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CLUB_INCOME_UP *pNetMsg = (GLMSG::SNET_CLUB_INCOME_UP *)nmg;

	GLCLUB *pCLUB = m_cClubMan.GetClub(pNetMsg->dwClubID);
	if (!pCLUB)
		return FALSE;

	//	Note : 贗毀曖 熱櫛擊 奩艙.
	if (pCLUB->m_lnIncomeMoney > 0)
	{
		GLMSG::SNET_CLUB_INCOME_DN NetMsgDn;
		NetMsgDn.dwClubID = pCLUB->m_dwID;
		NetMsgDn.lnInComeMoney = pCLUB->m_lnIncomeMoney;
		SENDTOALLCHANNEL(&NetMsgDn);

		pCLUB->m_lnIncomeMoney = 0;

		CDebugSet::ToFile("club_income_agent.txt", "{MsgClubIncomeMoneyUp}, ClubID[%u], IncomeMoney[%I64d]",
						  pCLUB->m_dwID, NetMsgDn.lnInComeMoney);

		//	Note : Db縑 盪濰.
		CSetClubIncomeMoney *pDbAction = new CSetClubIncomeMoney(pCLUB->m_dwID, pCLUB->m_lnIncomeMoney);
		m_pDBMan->AddJob(pDbAction);
	}

	return TRUE;
}

// *****************************************************
// Desc: 贗毀熱櫛縑憮 絲擊 貍除棻 incomemoney->storagemoney
// *****************************************************
// BOOL GLAgentServer::MsgClubIncomeMoneyFb ( NET_MSG_GENERIC* nmg )
//{
//	GLMSG::SNET_CLUB_INCOME_FB *pNetMsg = (GLMSG::SNET_CLUB_INCOME_FB *) nmg;
//
//	GLCLUB *pCLUB = m_cClubMan.GetClub ( pNetMsg->dwClubID );
//	if ( !pCLUB )		return FALSE;
//
//	if( pNetMsg->lnInComeMoney <= 0 ) return FALSE;
//
//	// 絲擊 渦 號檜 貍陛溥掘 衛紫憮 DB縑憮 Overflow 儅梯
//	if ( pCLUB->m_lnIncomeMoney < pNetMsg->lnInComeMoney ) return FALSE;
//
//	//	Note : 贗毀曖 熱櫛檜 奩艙脹菴 薑骯.
//	pCLUB->m_lnIncomeMoney -= pNetMsg->lnInComeMoney;
//
//	//m_pConsoleMsg->Write( LOG_CONSOLE, "m_lnIncomeMoney : %d", pCLUB->m_lnIncomeMoney );
//
//	//	Note : Db縑 盪濰.
//	CSetClubIncomeMoney *pDbAction = new CSetClubIncomeMoney ( pCLUB->m_dwID, pCLUB->m_lnIncomeMoney );
//	m_pDBMan->AddJob ( pDbAction );
//
//	return TRUE;
//}

BOOL GLAgentServer::MsgClubMemberDb2Del(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_CLUB_MEMBER_DB2DEL *pNetMsg = (GLMSG::SNET_CLUB_MEMBER_DB2DEL *)nmg;

	if (pNetMsg->dwClub == CLUB_NULL)
	{
		return S_FALSE;
	}

	GLClubMan &sClubMan = GLAgentServer::GetInstance().GetClubMan();
	GLCLUB *pCLUB = sClubMan.GetClub(pNetMsg->dwClub);
	if (!pCLUB)
	{
		return S_FALSE;
	}

	if (pCLUB->m_dwMasterID == pNetMsg->dwMember)
	{
		return S_FALSE;
	}

	bool bFOUND = pCLUB->IsMember(pNetMsg->dwMember);
	if (!bFOUND)
	{
		return S_FALSE;
	}

	//	Note : 贗毀 補幗 驍黴 煎斜 晦煙.
	GLITEMLMT::GetInstance().ReqAction(
		pCLUB->m_dwMasterID,		 //	渡餌濠.
		EMLOGACT_CLUB_SECEDE_MEMBER, //	嬪.
		ID_CHAR, pNetMsg->dwMember,	 //	鼻渠寞.
		0,							 //	exp
		0,							 //	bright
		0,							 //	life
		0							 //	money
	);

	//	Note : DB縑憮 餉薯.
	CDeleteClubMember *pDbAction = new CDeleteClubMember(pNetMsg->dwMember);
	GLAgentServer::GetInstance().GetDBMan()->AddJob(pDbAction);

	//	Note : 謙瞪 檣隸 掏濠 寥機.
	DWORD dwCD_OLD = pCLUB->m_dwCDCertifior;

	//	CDM 掏 氈擊唳辦 薯剪
	pCLUB->SetMemberCDMFlag(pNetMsg->dwMember, FALSE);

	//	Note : 贗毀 跡煙縑憮 薯剪.
	pCLUB->DelMember(pNetMsg->dwMember);

	//	Note : 摹紫贗毀 檣隸濠 滲唳衛.
	if (dwCD_OLD != pCLUB->m_dwCDCertifior)
	{
		//	Note : 憮幗縑 檣隸 掏濠 盪濰.
		CDbAction *pDbAction = new CSetClubDeputy(pCLUB->m_dwID, pCLUB->m_dwCDCertifior);
		GLDBMan *pDbMan = GLAgentServer::GetInstance().GetDBMan();
		if (pDbMan)
			pDbMan->AddJob(pDbAction);

		//	Note : 濠晦 贗毀錳縑啪 檣隸掏 憲葡.
		GLMSG::SNET_CLUB_SUBMASTER_BRD NetMsgBrd;
		NetMsgBrd.dwCharID = pCLUB->m_dwMasterID; //  ( 葆蝶攪 ID 蒂 餌辨. )
		NetMsgBrd.dwFlags = NULL;
		NetMsgBrd.dwCDCertifior = pCLUB->m_dwCDCertifior;
		GLAgentServer::GetInstance().SENDTOCLUBCLIENT(pCLUB->m_dwID, &NetMsgBrd);
	}

	//	Note : 補幗縑憮 餉薯 萄縑 憲葡.
	GLMSG::SNET_CLUB_MEMBER_DEL_2FLD NetMsgFld;
	NetMsgFld.dwClub = pNetMsg->dwClub;
	NetMsgFld.dwMember = pNetMsg->dwMember;
	SENDTOALLCHANNEL(&NetMsgFld);

	return TRUE;
}

BOOL GLAgentServer::MsgGmViewWorkEvent(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_VIEWWORKEVENT_FB NetMsgFB;

	if (m_WorkEvent.bWorkEvent[0]) // 唳纂
	{
		sprintf(NetMsgFB.szWorkEventInfo, "Event Type : Exp, Event Rate : %.2f", m_WorkEvent.exp_Event.fRATE);
		SENDTOCLIENT(dwClientID, &NetMsgFB);
	}

	if (m_WorkEvent.bWorkEvent[1]) // 嬴檜蠱
	{
		sprintf(NetMsgFB.szWorkEventInfo, "Event Type : Item, Event Rate : %.2f", m_WorkEvent.item_Event.fRATE);
		SENDTOCLIENT(dwClientID, &NetMsgFB);
	}

	if (m_WorkEvent.bWorkEvent[2]) // 絲 萄奧徽
	{
		sprintf(NetMsgFB.szWorkEventInfo, "Event Type : Money, Event Rate : %.2f", m_WorkEvent.money_Event.fRATE);
		SENDTOCLIENT(dwClientID, &NetMsgFB);
	}

	if (m_WorkEvent.bWorkEvent[3]) // EX 檜漸
	{
		char szTempChar[32] = {
			0,
		};
		if (m_WorkEvent.ex_Event.emType & EMGM_EVENT_SPEED)
			strcat(szTempChar, "MOVEMENT SPEED,");
		if (m_WorkEvent.ex_Event.emType & EMGM_EVENT_ASPEED)
			strcat(szTempChar, "ATTACK SPEED,");
		if (m_WorkEvent.ex_Event.emType & EMGM_EVENT_ATTACK)
			strcat(szTempChar, "ATTACK RATE,");

		szTempChar[strlen(szTempChar) - 1] = '\0';
		sprintf(NetMsgFB.szWorkEventInfo, "Event Type : Ex Event[%s] Event Rate : %d", szTempChar, m_WorkEvent.ex_Event.wValue);
		SENDTOCLIENT(dwClientID, &NetMsgFB);
	}

	if (m_WorkEvent.bWorkEvent[4]) // 葬嘐 檜漸
	{
		sprintf(NetMsgFB.szWorkEventInfo, "Event Type : LimitEvent, Start_LV %d, End_LV %d, Play_Time %d, Boost_Time %d, Exp Rate : %.2f, Item Rate : %.2f, EventMinute %d",
				m_WorkEvent.limit_Event.start_Lv,
				m_WorkEvent.limit_Event.end_Lv,
				m_WorkEvent.limit_Event.play_Time,
				m_WorkEvent.limit_Event.buster_Time,
				m_WorkEvent.limit_Event.expGain_Rate,
				m_WorkEvent.limit_Event.itemGain_Rate,
				m_WorkEvent.limit_Event.dwEventMinute);
		SENDTOCLIENT(dwClientID, &NetMsgFB);
	}

	/*gm command logs, Juver, 2018/08/15 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "dsp event result %s", NetMsgFB.szWorkEventInfo);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventEx(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_EX *pNetMsg = (GLMSG::SNET_GM_EVENT_EX *)nmg;
	if (pNetMsg->wValue < 1)
		pNetMsg->wValue = 1;
	else if (pNetMsg->wValue > 100)
		pNetMsg->wValue = 100;

	SENDTOALLCHANNEL(pNetMsg);

	m_WorkEvent.bWorkEvent[3] = TRUE;
	m_WorkEvent.ex_Event = *pNetMsg;

	/*gm command logs, Juver, 2018/08/18 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "event_ex_begin type:%d value:%u", pNetMsg->emType, pNetMsg->wValue);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventExEnd(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	SENDTOALLCHANNEL(nmg);

	m_WorkEvent.bWorkEvent[3] = FALSE;

	/*gm command logs, Juver, 2018/08/18 */
	GLMSG::SNET_GM_EVENT_EX_END *pNetMsg = (GLMSG::SNET_GM_EVENT_EX_END *)nmg;
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "event_ex_end type:%d", pNetMsg->emType);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

// 檜漸 衛濛!!
BOOL GLAgentServer::MsgGmLimitEventBegin(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	//** Add EventTime
	GLMSG::SNET_GM_LIMIT_EVENT_BEGIN *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_BEGIN *)nmg;

	m_sEventState.bEventStart = TRUE;
	m_sEventState.EventBusterTime = pNetMsg->buster_Time * 60;
	m_sEventState.EventPlayTime = pNetMsg->play_Time * 60;

	m_sEventState.fExpGainRate = pNetMsg->expGain_Rate;
	m_sEventState.fItemGainRate = pNetMsg->itemGain_Rate;
	m_sEventState.MinEventLevel = pNetMsg->start_Lv;
	m_sEventState.MaxEventLevel = pNetMsg->end_Lv;
	m_sEventState.dwEventEndMinute = pNetMsg->dwEventMinute;

	m_sEventState.EventStartTime = CTime::GetCurrentTime();

	CTime crtTime = CTime::GetCurrentTime();
	for (DWORD i = 0; i < m_dwMaxClient; ++i)
	{
		PGLCHARAG pChar = m_PCArray[i];
		if (pChar)
		{
			pChar->m_sEventTime.loginTime = crtTime.GetTime();
		}
	}

	SENDTOALLCHANNEL(pNetMsg);

	m_WorkEvent.bWorkEvent[4] = TRUE;
	m_WorkEvent.limit_Event = *pNetMsg;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "limit_event_begin StartLv: %d, EndLv: %d, PlayTime: %d, BusterTime: %d ExpGainRate: %.3f ItemGainRate: %.3f EventMinute %d",
				  pNetMsg->start_Lv, pNetMsg->end_Lv, pNetMsg->play_Time, pNetMsg->buster_Time, pNetMsg->expGain_Rate, pNetMsg->itemGain_Rate, pNetMsg->dwEventMinute);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

// 檜漸 謙猿!!
BOOL GLAgentServer::MsgGmLimitEventEnd(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	m_sEventState.bEventStart = false;

	GLMSG::SNET_GM_LIMIT_EVENT_END *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_END *)nmg;

	SENDTOALLCHANNEL(nmg);

	m_WorkEvent.bWorkEvent[4] = FALSE;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "%s", "limit_event_end");
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmLimitEventRestart(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	GLMSG::SNET_GM_LIMIT_EVENT_RESTART *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_RESTART *)nmg;

	pMyChar->m_sEventTime.loginTime = pNetMsg->restartTime;

	return TRUE;
}

BOOL GLAgentServer::MsgGmLimitEventTimeReq(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	//	GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ *)nmg;;

	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;

	GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ_FB NetSendMsg;

	if (m_sEventState.bEventStart == FALSE)
	{
		NetSendMsg.bEventStart = FALSE;
	}
	else
	{
		NetSendMsg.bEventStart = TRUE;

		NetSendMsg.buster_Time = m_sEventState.EventBusterTime;
		NetSendMsg.play_Time = m_sEventState.EventPlayTime;
		NetSendMsg.start_Lv = m_sEventState.MinEventLevel;
		NetSendMsg.end_Lv = m_sEventState.MaxEventLevel;

		// 虜擒 籀擠 啪歜擊 衛濛棻賊 煎斜檣 衛除擊 RESET棻.
		if (pCHAR->m_bReciveEventTime == FALSE)
		{
			CTime crtTime = CTime::GetCurrentTime();

			pCHAR->m_sEventTime.loginTime = crtTime.GetTime();
			NetSendMsg.loginTime = pCHAR->m_sEventTime.loginTime;

			GLMSG::SNET_GM_LIMIT_EVENT_TIME_RESET NetResetMsg;
			NetResetMsg.dwGaeaID = pCHAR->m_dwGaeaID;
			NetResetMsg.loginTime = crtTime.GetTime();
			SENDTOFIELD(pCHAR->m_dwClientID, &NetResetMsg);

			pCHAR->m_bReciveEventTime = TRUE;
		}
		else
		{
			NetSendMsg.loginTime = pCHAR->m_sEventTime.loginTime;
		}
	}

	SENDTOCLIENT(pCHAR->m_dwClientID, &NetSendMsg);

	return TRUE;
}

BOOL GLAgentServer::MsgCyberCafeClassUpdate(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;

	GLMSG::SNET_CYBERCAFECLASS_UPDATE *pNetMsg = (GLMSG::SNET_CYBERCAFECLASS_UPDATE *)nmg;
	pCHAR->m_dwThaiCCafeClass = pNetMsg->dwCyberCafeClass;
	pCHAR->m_nMyCCafeClass = pNetMsg->dwCyberCafeClass;

	SENDTOFIELD(pCHAR->m_dwClientID, pNetMsg);
	SENDTOCLIENT(pCHAR->m_dwClientID, pNetMsg);

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventExp(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_EXP *pNetMsg = (GLMSG::SNET_GM_EVENT_EXP *)nmg;
	if (pNetMsg->fRATE < 1.0f)
		pNetMsg->fRATE = 1.0f;
	if (pNetMsg->fRATE > 10.0f)
		pNetMsg->fRATE = 10.0f;

	if (pNetMsg->dwCHARID != pMyChar->m_dwCharID)
		return FALSE;

	GLMSG::SNET_GM_EVENT_EXP NetMsgField;
	NetMsgField.fRATE = pNetMsg->fRATE;
	NetMsgField.dwCHARID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL(&NetMsgField);

	m_WorkEvent.bWorkEvent[0] = TRUE;
	m_WorkEvent.exp_Event = *pNetMsg;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "eventexp_begin %.3f", pNetMsg->fRATE);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventExpEnd(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_EXP_END *pNetMsg = (GLMSG::SNET_GM_EVENT_EXP_END *)nmg;
	if (pNetMsg->dwCHARID != pMyChar->m_dwCharID)
		return FALSE;

	GLMSG::SNET_GM_EVENT_EXP_END NetMsgField;
	NetMsgField.dwCHARID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL(&NetMsgField);

	m_WorkEvent.bWorkEvent[0] = FALSE;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "%s", "eventexp_end");
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventItemGen(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_ITEM_GEN *pNetMsg = (GLMSG::SNET_GM_EVENT_ITEM_GEN *)nmg;
	if (pNetMsg->fRATE < 1.0f)
		pNetMsg->fRATE = 1.0f;
	if (pNetMsg->fRATE > 10.0f)
		pNetMsg->fRATE = 10.0f;

	if (pNetMsg->dwCHARID != pMyChar->m_dwCharID)
		return FALSE;

	GLMSG::SNET_GM_EVENT_ITEM_GEN NetMsgField;
	NetMsgField.fRATE = pNetMsg->fRATE;
	NetMsgField.dwCHARID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL(&NetMsgField);

	m_WorkEvent.bWorkEvent[1] = TRUE;
	m_WorkEvent.item_Event = *pNetMsg;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "eventitem_begin %.3f", pNetMsg->fRATE);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventItemGenEnd(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_ITEM_GEN_END *pNetMsg = (GLMSG::SNET_GM_EVENT_ITEM_GEN_END *)nmg;
	if (pNetMsg->dwCHARID != pMyChar->m_dwCharID)
		return FALSE;

	GLMSG::SNET_GM_EVENT_ITEM_GEN_END NetMsgField;
	NetMsgField.dwCHARID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL(&NetMsgField);

	m_WorkEvent.bWorkEvent[1] = FALSE;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "%s", "eventitem_end");
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventMoneyGen(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_MONEY_GEN *pNetMsg = (GLMSG::SNET_GM_EVENT_MONEY_GEN *)nmg;
	if (pNetMsg->fRATE < 1.0f)
		pNetMsg->fRATE = 1.0f;
	if (pNetMsg->fRATE > 10.0f)
		pNetMsg->fRATE = 10.0f;

	if (pNetMsg->dwCHARID != pMyChar->m_dwCharID)
		return FALSE;

	GLMSG::SNET_GM_EVENT_MONEY_GEN NetMsgField;
	NetMsgField.fRATE = pNetMsg->fRATE;
	NetMsgField.dwCHARID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL(&NetMsgField);

	m_WorkEvent.bWorkEvent[2] = TRUE;
	m_WorkEvent.money_Event = *pNetMsg;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "eventmoney_begin %.3f", pNetMsg->fRATE);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmEventMoneyGenEnd(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_EVENT_MONEY_GEN_END *pNetMsg = (GLMSG::SNET_GM_EVENT_MONEY_GEN_END *)nmg;
	if (pNetMsg->dwCHARID != pMyChar->m_dwCharID)
		return FALSE;

	GLMSG::SNET_GM_EVENT_MONEY_GEN_END NetMsgField;
	NetMsgField.dwCHARID = pMyChar->m_dwCharID;
	SENDTOALLCHANNEL(&NetMsgField);

	m_WorkEvent.bWorkEvent[2] = FALSE;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "%s", "eventmoney_end");
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmBigHead(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM2)
		return FALSE;

	GLMSG::SNET_GM_BIGHEAD *pNetMsg = (GLMSG::SNET_GM_BIGHEAD *)nmg;

	//	Note : 賅萇 萄 憮幗縑 歎褐.
	//
	SENDTOALLCHANNEL(nmg);

	//	Note : 賅萇 贗塭檜樹縑 歎褐.
	//
	GLMSG::SNET_GM_BIGHEAD_BRD NetMsgBrd;
	NetMsgBrd.bBIGHEAD = pNetMsg->bBIGHEAD;
	SENDTOALLCLIENT(&NetMsgBrd);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "bighead %d", pNetMsg->bBIGHEAD);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmBigHand(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM2)
		return FALSE;

	GLMSG::SNET_GM_BIGHAND *pNetMsg = (GLMSG::SNET_GM_BIGHAND *)nmg;

	//	Note : 賅萇 萄 憮幗縑 歎褐.
	//
	SENDTOALLCHANNEL(nmg);

	//	Note : 賅萇 贗塭檜樹縑 歎褐.
	//
	GLMSG::SNET_GM_BIGHAND_BRD NetMsgBrd;
	NetMsgBrd.bBIGHAND = pNetMsg->bBIGHAND;
	SENDTOALLCLIENT(&NetMsgBrd);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "bighand %d", pNetMsg->bBIGHAND);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmFreePK(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM2)
		return FALSE;

	GLMSG::SNET_GM_FREEPK *pNetMsg = (GLMSG::SNET_GM_FREEPK *)nmg;

	// m_cFreePK.StartFREEPKMODE ( pNetMsg->dwPKTIME );
	/*dmk14 freepk*/
	m_cFreePK.StartFREEPKMODE(pNetMsg->dwPKTIME, pNetMsg->sMapID, pNetMsg->sItemReward);

	SENDTOALLCHANNEL(nmg);

	GLMSG::SNET_GM_FREEPK_BRD NetMsgBrd;
	NetMsgBrd.dwPKTIME = pNetMsg->dwPKTIME;
	SENDTOALLCLIENT(&NetMsgBrd);

	/*gm command logs, Juver, 2018/08/18 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "freepk %d", pNetMsg->dwPKTIME);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

/*private market set, Juver, 2018/01/02 */
BOOL GLAgentServer::MsgGmSetPrivateMarket(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_SET_PRIVATE_MARKET *pNetMsg = (GLMSG::SNET_GM_SET_PRIVATE_MARKET *)nmg;

	m_pConsoleMsg->Write(LOG_TEXT_CONSOLE, "Set Private Market :%s", pNetMsg->bEnable ? "Enable" : "Disable");

	SENDTOALLCHANNEL(nmg);

	GLMSG::SNET_GM_SET_PRIVATE_MARKET_BRD NetMsgBrd;
	NetMsgBrd.bEnable = pNetMsg->bEnable;
	SENDTOALLCLIENT(&NetMsgBrd);

	/*gm command logs, Juver, 2018/08/18 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "setvend %d", pNetMsg->bEnable);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

/*megaphone set, Juver, 2018/01/02 */
BOOL GLAgentServer::MsgGmSetMegaPhone(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_SET_MEGAPHONE *pNetMsg = (GLMSG::SNET_GM_SET_MEGAPHONE *)nmg;

	m_pConsoleMsg->Write(LOG_TEXT_CONSOLE, "Set Megaphone :%s", pNetMsg->bEnable ? "Enable" : "Disable");

	SENDTOALLCHANNEL(nmg);

	GLMSG::SNET_GM_SET_MEGAPHONE_BRD NetMsgBrd;
	NetMsgBrd.bEnable = pNetMsg->bEnable;
	SENDTOALLCLIENT(&NetMsgBrd);

	/*gm command logs, Juver, 2018/08/18 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "setmega %d", pNetMsg->bEnable);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmViewAllPlayer(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_VIEWALLPLAYER_FLD_REQ NetMsgFld;
	NetMsgFld.dwID = dwGaeaID;

	// 萄憮幗縑 蹂羶
	SENDTOFIELD(pMyChar->m_dwClientID, &NetMsgFld);

	return TRUE;
}

// *****************************************************
// Desc: 雖薑脹 馨葛攪蒂 濠晦 蕙戲煎 模
// *****************************************************
BOOL GLAgentServer::MsgGmGenChar(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNETPC_GM_GENCHAR *pNetMsg = (GLMSG::SNETPC_GM_GENCHAR *)nmg;
	GLMSG::SNETPC_GM_GENCHAR_FB NetMsgFB;

	PGLCHARAG pGenCHAR;

	if (pNetMsg->dwCHARID != 0)
	{
		pGenCHAR = GetCharID(pNetMsg->dwCHARID);
	}
	else
	{
		pGenCHAR = GetChar(pNetMsg->szNAME);
	}

	if (!pGenCHAR)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pGenCHAR->m_dwCurFieldSvr == FIELDSERVER_MAX)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_TO_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pGenCHAR->m_nChannel != pMyChar->m_nChannel)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_CHANNEL;
		NetMsgFB.nChannel = pGenCHAR->m_nChannel;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*instance disable move, Juver, 2018/07/13 */
	GLAGLandMan *plandman_current = GetByMapID(pMyChar->m_sCurMapID);
	if (plandman_current && plandman_current->IsInstantMap())
		return FALSE;

	/*instance disable move, Juver, 2018/07/13 */
	GLAGLandMan *plandman_target = GetByMapID(pGenCHAR->m_sCurMapID);
	if (plandman_target && plandman_target->IsInstantMap())
		return FALSE;

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar && pGenCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "genchar [%u]%s from map [%u/%u] to map [%u/%u]", pGenCHAR->m_dwCharID, pGenCHAR->m_szName, pGenCHAR->m_sCurMapID.wMainID, pGenCHAR->m_sCurMapID.wSubID, pMyChar->m_sCurMapID.wMainID, pMyChar->m_sCurMapID.wSubID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	// GM曖 嬪纂蒂 檣棻.
	GLMSG::SNETPC_GM_MOVE2CHAR_POS NetMsgCK;
	NetMsgCK.dwCOMMAND_CHARID = pGenCHAR->m_dwCharID;
	NetMsgCK.dwTO_CHARID = pMyChar->m_dwCharID;
	SENDTOFIELDSVR(pMyChar->m_nChannel, pMyChar->m_dwCurFieldSvr, &NetMsgCK);

	NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_OK;

	SENDTOCLIENT(dwClientID, &NetMsgFB);

	return TRUE;
}

BOOL GLAgentServer::MsgGmPrintCrowList(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;
	if (pCHAR->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_PRINT_CROWLIST *pNetMsg = (GLMSG::SNET_GM_PRINT_CROWLIST *)nmg;

	GLMSG::SNET_GM_PRINT_CROWLIST_FLD NetMsgFld;
	NetMsgFld.dwFindMob = pNetMsg->dwFindMob;
	NetMsgFld.dwGaeaID = dwGaeaID;
	SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "crow_list type:%d in map[%u/%u]", pNetMsg->dwFindMob, pCHAR->m_sCurMapID.wMainID, pCHAR->m_sCurMapID.wSubID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pCHAR->m_dwClientID, pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmGenMob(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_MOB_GEN *pNetMsg = (GLMSG::SNET_GM_MOB_GEN *)nmg;

	GLMSG::SNET_GM_MOB_GEN_FLD NetMsgFld;

	NetMsgFld.dwGaeaID = dwGaeaID;
	NetMsgFld.wMainID = pNetMsg->wMainID;
	NetMsgFld.wSubID = pNetMsg->wSubID;
	NetMsgFld.wPosX = pNetMsg->wPosX;
	NetMsgFld.wPosY = pNetMsg->wPosY;

	SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "mob_gen in map:[%u/%u] mob:[%u/%u] pos:[%u/%u]", pMyChar->m_sCurMapID.wMainID, pMyChar->m_sCurMapID.wSubID, pNetMsg->wMainID, pNetMsg->wSubID, pNetMsg->wPosX, pNetMsg->wPosY);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmGenMobEx(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_MOB_GEN_EX *pNetMsg = (GLMSG::SNET_GM_MOB_GEN_EX *)nmg;

	SNATIVEID sMapID(pNetMsg->wMapMID, pNetMsg->wMapSID);
	GLMapList::FIELDMAP MapsList = m_sMapList.GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find(sMapID.dwID);
	if (iter == MapsList.end())
		return FALSE;

	GLMSG::SNET_GM_MOB_GEN_EX_FLD NetMsgFld;
	NetMsgFld.wMobMID = pNetMsg->wMobMID;
	NetMsgFld.wMobSID = pNetMsg->wMobSID;
	NetMsgFld.wMapMID = pNetMsg->wMapMID;
	NetMsgFld.wMapSID = pNetMsg->wMapSID;
	NetMsgFld.wPosX = pNetMsg->wPosX;
	NetMsgFld.wPosY = pNetMsg->wPosY;
	NetMsgFld.wRange = pNetMsg->wRange;
	NetMsgFld.wNum = pNetMsg->wNum;
	NetMsgFld.dwGaeaID = dwGaeaID;

	if (pNetMsg->bThisChannel)
		SENDTOCHANNEL(&NetMsgFld, pMyChar->m_nChannel);
	else
		SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "mob_gen_ex in map:[%u/%u] mob:[%u/%u] pos:[%u/%u] range:%u num:%u thischannel:%d", pNetMsg->wMapMID, pNetMsg->wMapSID, pNetMsg->wMobMID, pNetMsg->wMobSID, pNetMsg->wPosX, pNetMsg->wPosY, pNetMsg->wRange, pNetMsg->wNum, pNetMsg->bThisChannel);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmDelMobEx(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_MOB_DEL_EX *pNetMsg = (GLMSG::SNET_GM_MOB_DEL_EX *)nmg;

	SNATIVEID sMapID(pNetMsg->wMapMID, pNetMsg->wMapSID);
	GLMapList::FIELDMAP MapsList = m_sMapList.GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find(sMapID.dwID);
	if (iter == MapsList.end())
		return FALSE;

	GLMSG::SNET_GM_MOB_DEL_EX_FLD NetMsgFld;
	NetMsgFld.wMobMID = pNetMsg->wMobMID;
	NetMsgFld.wMobSID = pNetMsg->wMobSID;
	NetMsgFld.wMapMID = pNetMsg->wMapMID;
	NetMsgFld.wMapSID = pNetMsg->wMapSID;

	if (pNetMsg->bThisChannel)
		SENDTOCHANNEL(&NetMsgFld, pMyChar->m_nChannel);
	else
		SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "mob_del_ex in map:[%u/%u] mob:[%u/%u] thischannel:%d", pNetMsg->wMapMID, pNetMsg->wMapSID, pNetMsg->wMobMID, pNetMsg->wMobSID, pNetMsg->bThisChannel);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmDelMob(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_MOB_DEL *pNetMsg = (GLMSG::SNET_GM_MOB_DEL *)nmg;

	GLMSG::SNET_GM_MOB_DEL_FLD NetMsgFld;
	NetMsgFld.dwGaeaID = dwGaeaID;
	NetMsgFld.sMobID = pNetMsg->sMobID;
	NetMsgFld.bMaterial = pNetMsg->bMaterial;

	SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "mob_del in map:[%u/%u] mob:[%u/%u] material:%u", pMyChar->m_sCurMapID.wMainID, pMyChar->m_sCurMapID.wSubID, pNetMsg->sMobID.wMainID, pNetMsg->sMobID.wSubID, pNetMsg->bMaterial);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmMobLevel(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;
	if (pCHAR->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	SENDTOALLCHANNEL(nmg);

	/*gm command logs, Juver, 2018/08/18 */
	GLMSG::SNET_GM_MOB_LEV *pNetMsg = (GLMSG::SNET_GM_MOB_LEV *)nmg;

	if (GetDBMan() && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "lev_new %s in map:[%u/%u]", pNetMsg->szLevelFile, pNetMsg->wMAP_MID, pNetMsg->wMAP_SID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pCHAR->m_dwClientID, pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmMobLevelClear(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;
	if (pCHAR->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	SENDTOALLCHANNEL(nmg);

	/*gm command logs, Juver, 2018/08/18 */
	GLMSG::SNET_GM_MOB_LEV_CLEAR *pNetMsg = (GLMSG::SNET_GM_MOB_LEV_CLEAR *)nmg;

	if (GetDBMan() && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "lev_del in map:[%u/%u]", pNetMsg->wMAP_MID, pNetMsg->wMAP_SID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pCHAR->m_dwClientID, pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmWhereNPC(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCHAR = GetChar(dwGaeaID);
	if (!pCHAR)
		return FALSE;
	if (pCHAR->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_WHERE_NPC *pNetMsg = (GLMSG::SNET_GM_WHERE_NPC *)nmg;

	GLMSG::SNET_GM_WHERE_NPC_FLD NetMsgFld;

	NetMsgFld.nidNPC = pNetMsg->nidNPC;
	NetMsgFld.dwGaeaID = dwGaeaID;
	SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "where_npc [%u/%u] on map [%u/%u]", pNetMsg->nidNPC.wMainID, pNetMsg->nidNPC.wSubID, pCHAR->m_sCurMapID.wMainID, pCHAR->m_sCurMapID.wSubID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pCHAR->m_dwClientID, pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pCHAR->m_dwUserID, pCHAR->m_dwUserLvl, pCHAR->m_dwCharID, pCHAR->m_szName, command);
	}

	return TRUE;
}

// *****************************************************
// Desc: 耀掘嬪纂 檣 萄憮幗縑 檜翕 蹂羶 (field->Agent)
// *****************************************************
BOOL GLAgentServer::Msg2FriendAG(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_2_FRIEND_AG *pNetMsg = (GLMSG::SNETPC_2_FRIEND_AG *)nmg;
	GLMSG::SNETPC_2_FRIEND_FB NetMsgFB;

	PGLCHARAG pCHAR = GetCharID(pNetMsg->dwCOMMAND_CHARID);
	if (!pCHAR)
		return FALSE;

	if (pNetMsg->emFB == EM2FRIEND_FB_OK)
	{
		GLMSG::SNETPC_2_FRIEND_FLD NetMsgFld;
		NetMsgFld.sFriendMapID = pNetMsg->sFriendMapID;
		NetMsgFld.vFriendPos = pNetMsg->vFriendPos;
		NetMsgFld.wItemPosX = pNetMsg->wItemPosX;
		NetMsgFld.wItemPosY = pNetMsg->wItemPosY;
		SENDTOFIELD(pCHAR->m_dwClientID, &NetMsgFld);

		/*dmk14 fast teleport fix*/
		NetMsgFB.emFB = pNetMsg->emFB;
		SENDTOCLIENT(pCHAR->m_dwClientID, &NetMsgFB);
	}
	else
	{
		//	Note : 耀掘縑啪 檜翕 褒 奩.
		NetMsgFB.emFB = pNetMsg->emFB;
		SENDTOCLIENT(pCHAR->m_dwClientID, &NetMsgFB);
	}

	return TRUE;
}

/*dmk14 fast teleport fix*/
void GLAgentServer::Do2FriendReq(DWORD dwGaeaID, DWORD dwFriendID, WORD wPosX, WORD wPosY)
{
	GLMSG::SNETPC_2_FRIEND_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return;

	DWORD dwClientID = pMyChar->m_dwClientID;
	PGLCHARAG pCHAR_FRIEND = GetCharID(pMyChar->m_dwFriendID);
	if (!pCHAR_FRIEND)
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return;
	}

	GLCHARAG_DATA::MAPFRIEND_ITER pos = pCHAR_FRIEND->m_mapFriend.find(pMyChar->m_szName);
	if (pos == pCHAR_FRIEND->m_mapFriend.end())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return;
	}
	SFRIEND &sFRIEND2 = (*pos).second;

	if (sFRIEND2.IsBLOCK())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_BLOCK;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return;
	}

	if (sFRIEND2.IsOFF())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_OFF;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return;
	}

	if (pCHAR_FRIEND->m_dwCurFieldSvr == FIELDSERVER_MAX)
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return;
	}

	if (pCHAR_FRIEND->m_nChannel != pMyChar->m_nChannel)
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CHANNEL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return;
	}

	GLMSG::SNETPC_2_FRIEND_CK NetMsgCK;
	NetMsgCK.dwCOMMAND_CHARID = pMyChar->m_dwCharID;
	NetMsgCK.dwFRIEND_CHARID = pCHAR_FRIEND->m_dwCharID;
	NetMsgCK.wItemPosX = wPosX;
	NetMsgCK.wItemPosY = wPosY;
	SENDTOFIELDSVR(pCHAR_FRIEND->m_nChannel, pCHAR_FRIEND->m_dwCurFieldSvr, &NetMsgCK);
}

BOOL GLAgentServer::Msg2FriendCancel(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_2_FRIEND_REQ_CANCEL *pNetMsg = (GLMSG::SNETPC_2_FRIEND_REQ_CANCEL *)nmg;
	GLMSG::SNETPC_2_FRIEND_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	pMyChar->m_bStartTeleport = FALSE;
	pMyChar->m_fTeleporTimer = 0.0f;

	NetMsgFB.emFB = EM2FRIEND_FB_CANCEL;
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	return TRUE;
}

// *****************************************************
// Desc: 耀掘檜翕 蹂羶 籀葬
// *****************************************************
BOOL GLAgentServer::Msg2FriendReq(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_2_FRIEND_REQ *pNetMsg = (GLMSG::SNETPC_2_FRIEND_REQ *)nmg;
	GLMSG::SNETPC_2_FRIEND_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	/*map move settings, Juver, 2017/11/25 */
	GLAGLandMan *plandman_current = GetByMapID(pMyChar->m_sCurMapID);
	if (plandman_current && plandman_current->IsBlockFriendCard())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*instance disable move, Juver, 2018/07/13 */
	if (plandman_current && plandman_current->IsInstantMap())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	GLCHARAG_DATA::MAPFRIEND_ITER pos = pMyChar->m_mapFriend.find(pNetMsg->szFRIEND_NAME);
	if (pos == pMyChar->m_mapFriend.end())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	SFRIEND * pFRIEND = (*pos).second;
	//	if( !pFRIEND ) return FALSE;
	SFRIEND &sFRIEND = (*pos).second;

	if (!sFRIEND.bONLINE)
	//	if ( !pFRIEND->bONLINE )
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	PGLCHARAG pCHAR_FRIEND = GetCharID ( pFRIEND->nCharID );
	PGLCHARAG pCHAR_FRIEND = GetCharID(sFRIEND.nCharID);
	if (!pCHAR_FRIEND)
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*instance disable move, Juver, 2018/07/13 */
	GLAGLandMan *plandman_target = GetByMapID(pCHAR_FRIEND->m_sCurMapID);
	if (plandman_target && plandman_target->IsInstantMap())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	// Memo : 鼻渠陛 釭蒂 餉薯剪釭 離欽 鼻鷓檜賊 檜翕檜 鏃模脹棻.
	//
	pos = pCHAR_FRIEND->m_mapFriend.find(pMyChar->m_szName);
	if (pos == pCHAR_FRIEND->m_mapFriend.end())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	SFRIEND &sFRIEND2 = (*pos).second;
	//	if( !pMe ) return FALSE;

	if (sFRIEND2.IsBLOCK())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_BLOCK;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (sFRIEND2.IsOFF())
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_OFF;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pCHAR_FRIEND->m_dwCurFieldSvr == FIELDSERVER_MAX)
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pCHAR_FRIEND->m_nChannel != pMyChar->m_nChannel)
	{
		NetMsgFB.emFB = EM2FRIEND_FB_FRIEND_CHANNEL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	Note : 耀掘曖 嬪纂 薄匐 蹂羶.
	/*dmk14 fast teleport fix*/
	NetMsgFB.emFB = EM2FRIEND_FB_REQ;
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	pMyChar->m_bStartTeleport = TRUE;
	pMyChar->m_dwFriendID = pCHAR_FRIEND->m_dwCharID;
	pMyChar->m_wItemX = pNetMsg->wItemPosX;
	pMyChar->m_wItemY = pNetMsg->wItemPosY;

	return TRUE;
}

// *****************************************************
// Desc: 雖薑 議葛攪曖 敏樓蜓擊 興菔朝棻.
// *****************************************************
BOOL GLAgentServer::MsgGetWhisperMsg(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_GM_GETWHISPERMSG *pNetMsg = (GLMSG::SNETPC_GM_GETWHISPERMSG *)nmg;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
	{
		return FALSE;
	}

	if (strlen(pNetMsg->szNAME) != 0)
	{
		PGLCHARAG pTo_CHAR;
		pTo_CHAR = GetChar(pNetMsg->szNAME);
		if (pTo_CHAR)
		{
			UINT i;
			bool bAdd = TRUE;
			for (i = 0; i < pTo_CHAR->m_vecGetWhisperList.size(); i++)
			{
				if (dwClientID == pTo_CHAR->m_vecGetWhisperList[i].dwClientID &&
					dwGaeaID == pTo_CHAR->m_vecGetWhisperList[i].dwGaeaID)
				{
					bAdd = FALSE;
					break;
				}
			}
			if (bAdd)
			{
				SGETWHISPERLIST WhisperList;
				WhisperList.dwClientID = dwClientID;
				WhisperList.dwGaeaID = dwGaeaID;
				pTo_CHAR->m_vecGetWhisperList.push_back(WhisperList);

				pMyChar->m_GmWhisperList.dwGaeaID = pTo_CHAR->m_dwGaeaID;
				pMyChar->m_GmWhisperList.dwClientID = pTo_CHAR->m_dwClientID;
				pMyChar->m_bViewWhisperMSG = TRUE;
			}
		}
	}
	else
	{
		pMyChar->m_bViewWhisperMSG = FALSE;
	}

	/*gm command logs, Juver, 2018/08/17 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "msgview to char %s", pNetMsg->szNAME);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

// *****************************************************
// Desc: 雖薑脹 馨葛攪 嬪纂煎 檜翕
// *****************************************************
BOOL GLAgentServer::MsgMove2Char(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_GM_MOVE2CHAR *pNetMsg = (GLMSG::SNETPC_GM_MOVE2CHAR *)nmg;
	GLMSG::SNETPC_GM_MOVE2CHAR_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	PGLCHARAG pTO_CHAR;
	;

	if (pNetMsg->dwCHARID != 0)
	{
		pTO_CHAR = GetCharID(pNetMsg->dwCHARID);
	}
	else
	{
		pTO_CHAR = GetChar(pNetMsg->szNAME);
	}

	if (!pTO_CHAR)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*instance disable move, Juver, 2018/07/13 */
	GLAGLandMan *plandman_current = GetByMapID(pMyChar->m_sCurMapID);
	if (plandman_current && plandman_current->IsInstantMap())
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*instance disable move, Juver, 2018/07/13 */
	GLAGLandMan *plandman_target = GetByMapID(pTO_CHAR->m_sCurMapID);
	if (plandman_target && plandman_target->IsInstantMap())
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_FAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pTO_CHAR->m_dwCurFieldSvr == FIELDSERVER_MAX)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_TO_CONDITION;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	if (pTO_CHAR->m_nChannel != pMyChar->m_nChannel)
	{
		NetMsgFB.emFB = EMGM_MOVE2CHAR_FB_CHANNEL;
		NetMsgFB.nChannel = pTO_CHAR->m_nChannel;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar && pTO_CHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "move2char [%d]%s from map [%u/%u] to map [%u/%u]", pTO_CHAR->m_dwCharID, pTO_CHAR->m_szName, pMyChar->m_sCurMapID.wMainID, pMyChar->m_sCurMapID.wSubID, pTO_CHAR->m_sCurMapID.wMainID, pTO_CHAR->m_sCurMapID.wSubID);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	//	Note : 耀掘曖 嬪纂 薄匐 蹂羶.
	GLMSG::SNETPC_GM_MOVE2CHAR_POS NetMsgCK;
	NetMsgCK.dwCOMMAND_CHARID = pMyChar->m_dwCharID;
	NetMsgCK.dwTO_CHARID = pTO_CHAR->m_dwCharID;
	SENDTOFIELDSVR(pTO_CHAR->m_nChannel, pTO_CHAR->m_dwCurFieldSvr, &NetMsgCK);

	return TRUE;
}

// *****************************************************
// Desc: 嶸盪 瓣 綰楣 籀葬 (User Account)
// *****************************************************
BOOL GLAgentServer::MsgGmChatBlockUA(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_CHAT_BLOCK_UACCOUNT *pNetMsg = (GLMSG::SNET_GM_CHAT_BLOCK_UACCOUNT *)nmg;
	GLMSG::SNET_GM_CHAT_BLOCK_FB NetMsgFB;

	PGLCHARAG pCHAR = GetCharUA(pNetMsg->szUACCOUNT);
	// 蕾樓醞檜 嬴棍唳辦
	if (!pCHAR)
	{
		NetMsgFB.bBLOCK = false;
		StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pNetMsg->szUACCOUNT);
		SENDTOCLIENT(dwClientID, &NetMsgFB);

		// 瓣綰楣 蕨擒濠 DB縑 盪濰棻.

		return FALSE;
	}

	/*chatblock reason, Juver, 2018/09/25 */
	CString strChatBlockReason = pNetMsg->szReason;
	strChatBlockReason.Trim();
	if (STRUTIL::CheckString_Special3(strChatBlockReason.GetString()))
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE > RPARAM::dwChatBlockMaxMinute)
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE > MAXIMUM_CHATBLOCK_MINUTE)
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE != 0)
	{
		//	綰楣衛除 雖薑.
		CTime cBLOCK = CTime::GetCurrentTime();
		CTimeSpan cSPAN(0, 0, pNetMsg->dwBLOCK_MINUTE, 0);
		cBLOCK += cSPAN;

		pCHAR->m_tCHATBLOCK = cBLOCK.GetTime();
	}
	else
	{
		//	綰楣衛除 雖.
		pCHAR->m_tCHATBLOCK = 0;
	}

	//	DB縑 盪濰.
	CSetChatBlockTime *pDbAction = new CSetChatBlockTime(pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_tCHATBLOCK, strChatBlockReason.GetString(), pMyChar->m_dwUserID, pMyChar->m_szUserName);
	if (GetDBMan())
		GetDBMan()->AddJob(pDbAction);

	//	FLD縑 憲葡.
	GLMSG::SNET_GM_CHAT_BLOCK_FLD NetMsgFLD;
	NetMsgFLD.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFLD.dwBLOCK_MINUTE = pNetMsg->dwBLOCK_MINUTE;
	SENDTOALLCHANNEL(&NetMsgFLD);

	//	FB.
	NetMsgFB.bBLOCK = true;
	NetMsgFB.dwBLOCK_MINUTE = pNetMsg->dwBLOCK_MINUTE;
	StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, pCHAR->m_szName);
	StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pCHAR->m_szUserName);
	StringCchCopy(NetMsgFB.szReason, GM_CMD_CHATBLOCK_REASON_TEXT_SIZE, strChatBlockReason.GetString());
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	/*gm command logs, Juver, 2018/08/16 */
	/*chatblock reason, Juver, 2018/09/25 */
	if (GetDBMan() && pMyChar && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "chatblock_ua user:[%u][%s] char:[%d]%s block minute:%u reason:%s", pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_dwCharID, pCHAR->m_szName, pNetMsg->dwBLOCK_MINUTE, strChatBlockReason.GetString());
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

// *****************************************************
// Desc: 嶸盪 瓣 綰楣 籀葬 (議葛攪貲)
// *****************************************************
BOOL GLAgentServer::MsgGmChatBlockCN(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_CHAT_BLOCK_CHARNAME *pNetMsg = (GLMSG::SNET_GM_CHAT_BLOCK_CHARNAME *)nmg;
	GLMSG::SNET_GM_CHAT_BLOCK_FB NetMsgFB;

	PGLCHARAG pCHAR = GetChar(pNetMsg->szCHARNAME);
	// 蕾樓醞檜 嬴棍唳辦
	if (!pCHAR)
	{
		NetMsgFB.bBLOCK = false;
		StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pNetMsg->szCHARNAME);
		SENDTOCLIENT(dwClientID, &NetMsgFB);

		// 瓣綰楣 蕨擒濠 DB縑 盪濰棻.

		return FALSE;
	}

	/*chatblock reason, Juver, 2018/09/25 */
	CString strChatBlockReason = pNetMsg->szReason;
	strChatBlockReason.Trim();
	if (STRUTIL::CheckString_Special3(strChatBlockReason.GetString()))
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE > RPARAM::dwChatBlockMaxMinute)
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE > MAXIMUM_CHATBLOCK_MINUTE)
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE != 0)
	{
		//	綰楣衛除 雖薑.
		CTime cBLOCK = CTime::GetCurrentTime();
		CTimeSpan cSPAN(0, 0, pNetMsg->dwBLOCK_MINUTE, 0);
		cBLOCK += cSPAN;

		pCHAR->m_tCHATBLOCK = cBLOCK.GetTime();
	}
	else
	{
		//	綰楣衛除 雖.
		pCHAR->m_tCHATBLOCK = 0;
	}

	//	DB縑 盪濰.
	CSetChatBlockTime *pDbAction = new CSetChatBlockTime(pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_tCHATBLOCK, strChatBlockReason.GetString(), pMyChar->m_dwUserID, pMyChar->m_szUserName);
	if (GetDBMan())
		GetDBMan()->AddJob(pDbAction);

	//	FLD縑 憲葡.
	GLMSG::SNET_GM_CHAT_BLOCK_FLD NetMsgFLD;
	NetMsgFLD.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFLD.dwBLOCK_MINUTE = pNetMsg->dwBLOCK_MINUTE;
	SENDTOALLCHANNEL(&NetMsgFLD);

	//	FB.
	NetMsgFB.bBLOCK = true;
	NetMsgFB.dwBLOCK_MINUTE = pNetMsg->dwBLOCK_MINUTE;
	StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, pCHAR->m_szName);
	StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pCHAR->m_szUserName);
	StringCchCopy(NetMsgFB.szReason, GM_CMD_CHATBLOCK_REASON_TEXT_SIZE, strChatBlockReason.GetString());
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	/*gm command logs, Juver, 2018/08/16 */
	/*chatblock reason, Juver, 2018/09/25 */
	if (GetDBMan() && pMyChar && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "chatblock_cn user:[%u][%s] char:[%d]%s block minute:%u reason:%s", pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_dwCharID, pCHAR->m_szName, pNetMsg->dwBLOCK_MINUTE, strChatBlockReason.GetString());
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmChatBlockCI(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_CHAT_BLOCK_CHARID *pNetMsg = (GLMSG::SNET_GM_CHAT_BLOCK_CHARID *)nmg;
	GLMSG::SNET_GM_CHAT_BLOCK_FB NetMsgFB;

	PGLCHARAG pCHAR = GetCharID(pNetMsg->dwCHARID);
	if (!pCHAR)
	{
		NetMsgFB.bBLOCK = false;
		StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, "notfound");
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	/*chatblock reason, Juver, 2018/09/25 */
	CString strChatBlockReason = pNetMsg->szReason;
	strChatBlockReason.Trim();
	if (STRUTIL::CheckString_Special3(strChatBlockReason.GetString()))
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE > RPARAM::dwChatBlockMaxMinute)
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE > MAXIMUM_CHATBLOCK_MINUTE)
		return FALSE;

	if (pNetMsg->dwBLOCK_MINUTE != 0)
	{
		//	綰楣衛除 雖薑.
		pCHAR->m_tCHATBLOCK;

		CTime cBLOCK = CTime::GetCurrentTime();
		CTimeSpan cSPAN(0, 0, pNetMsg->dwBLOCK_MINUTE, 0);
		cBLOCK += cSPAN;

		pCHAR->m_tCHATBLOCK = cBLOCK.GetTime();
	}
	else
	{
		//	綰楣衛除 雖.
		pCHAR->m_tCHATBLOCK = 0;
	}

	//	DB縑 盪濰.
	CSetChatBlockTime *pDbAction = new CSetChatBlockTime(pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_tCHATBLOCK, strChatBlockReason.GetString(), pMyChar->m_dwUserID, pMyChar->m_szUserName);
	if (GetDBMan())
		GetDBMan()->AddJob(pDbAction);

	//	FLD縑 憲葡.
	GLMSG::SNET_GM_CHAT_BLOCK_FLD NetMsgFLD;
	NetMsgFLD.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFLD.dwBLOCK_MINUTE = pNetMsg->dwBLOCK_MINUTE;
	SENDTOALLCHANNEL(&NetMsgFLD);

	//	FB.
	NetMsgFB.bBLOCK = true;
	NetMsgFB.dwBLOCK_MINUTE = pNetMsg->dwBLOCK_MINUTE;
	StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, pCHAR->m_szName);
	StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pCHAR->m_szUserName);
	StringCchCopy(NetMsgFB.szReason, GM_CMD_CHATBLOCK_REASON_TEXT_SIZE, strChatBlockReason.GetString());
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	/*gm command logs, Juver, 2018/08/16 */
	/*chatblock reason, Juver, 2018/09/25 */
	if (GetDBMan() && pMyChar && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "chatblock_ci user:[%u][%s] char:[%d]%s block minute:%u reason:%s", pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_dwCharID, pCHAR->m_szName, pNetMsg->dwBLOCK_MINUTE, strChatBlockReason.GetString());
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmWarningMSG(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM1)
		return FALSE;

	GLMSG::SNET_GM_WARNING_MSG *pNetMsg = (GLMSG::SNET_GM_WARNING_MSG *)nmg;
	GLMSG::SNET_GM_WARNING_MSG_FLD NetMsgBrd;

	NetMsgBrd.dwGaeaID = dwGaeaID;
	NetMsgBrd.bOn = pNetMsg->bOn;
	SENDTOALLCHANNEL(&NetMsgBrd);

	/*gm command logs, Juver, 2018/08/18 */
	if (GetDBMan() && pMyChar)
	{
		char command[1024];
		sprintf_s(command, 1024, "warning_msg %d", pNetMsg->bOn);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmShowMeTheMoney(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
#if defined(_RELEASED) || defined(KRT_PARAM)
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < USER_GM3)
		return FALSE;

	GLMSG::SNET_GM_SHOWMETHEMONEY *pNetMsg = (GLMSG::SNET_GM_SHOWMETHEMONEY *)nmg;
	GLMSG::SNET_GM_SHOWMETHEMONEY_FLD NetMsgBrd;

	NetMsgBrd.dwGaeaID = dwGaeaID;
	NetMsgBrd.llMoney = pNetMsg->llMoney;
	SENDTOALLCHANNEL(&NetMsgBrd);
#endif

	return TRUE;
}

BOOL GLAgentServer::MsgGmCharInfo4NAME(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_CHAR_INFO_4NAME *pNetMsg = (GLMSG::SNET_GM_CHAR_INFO_4NAME *)nmg;
	GLMSG::SNET_GM_CHAR_INFO_AGT_FB NetMsgFB;

	PGLCHARAG pCHAR = GetChar(pNetMsg->szCHARNAME);
	if (!pCHAR)
	{
		StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, "notfound");
		StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, "notfound");
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	贗塭檜樹縑 agt fb
	StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, pCHAR->m_szName);
	StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pCHAR->m_szUserName);
	NetMsgFB.dwSERVER;
	NetMsgFB.dwCHANNEL = (DWORD)pCHAR->m_nChannel;
	NetMsgFB.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFB.dwGAEAID = pCHAR->m_dwGaeaID;
	NetMsgFB.dwPartyID = pCHAR->m_dwPartyID;
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	//	萄縑 鼻撮 薑爾 蹂羶.
	GLMSG::SNET_GM_CHAR_INFO_FLD NetMsgFld;
	NetMsgFld.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFld.dwCLIENTID_FB = dwClientID;
	SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "charinfo_cn user:[%u][%s] char:[%d]%s", pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_dwCharID, pCHAR->m_szName);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmCharInfo4CID(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;
	if (pMyChar->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3)
		return FALSE;

	GLMSG::SNET_GM_CHAR_INFO_4CHARID *pNetMsg = (GLMSG::SNET_GM_CHAR_INFO_4CHARID *)nmg;
	GLMSG::SNET_GM_CHAR_INFO_AGT_FB NetMsgFB;

	PGLCHARAG pCHAR = GetCharID(pNetMsg->dwCHARID);
	if (!pCHAR)
	{
		StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, "notfound");
		StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, "notfound");
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	//	贗塭檜樹縑 agt fb
	StringCchCopy(NetMsgFB.szCHARNAME, CHAR_SZNAME, pCHAR->m_szName);
	StringCchCopy(NetMsgFB.szUACCOUNT, USR_ID_LENGTH + 1, pCHAR->m_szUserName);
	NetMsgFB.dwSERVER;
	NetMsgFB.dwCHANNEL = (DWORD)pCHAR->m_nChannel;
	NetMsgFB.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFB.dwGAEAID = pCHAR->m_dwGaeaID;
	NetMsgFB.dwPartyID = pCHAR->m_dwPartyID;
	SENDTOCLIENT(dwClientID, &NetMsgFB);

	//	萄縑 鼻撮 薑爾 蹂羶.
	GLMSG::SNET_GM_CHAR_INFO_FLD NetMsgFld;
	NetMsgFld.dwCHARID = pCHAR->m_dwCharID;
	NetMsgFld.dwCLIENTID_FB = dwClientID;
	SENDTOALLCHANNEL(&NetMsgFld);

	/*gm command logs, Juver, 2018/08/16 */
	if (GetDBMan() && pMyChar && pCHAR)
	{
		char command[1024];
		sprintf_s(command, 1024, "charinfo_ci user:[%u][%s] char:[%d]%s", pCHAR->m_dwUserID, pCHAR->m_szUserName, pCHAR->m_dwCharID, pCHAR->m_szName);
		gm_cmd_logs *pdb_action = new gm_cmd_logs(pMyChar->m_dwClientID, pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
		GetDBMan()->AddJob(pdb_action);

		CDebugSet::ToFileWithTime("gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", pMyChar->m_dwUserID, pMyChar->m_dwUserLvl, pMyChar->m_dwCharID, pMyChar->m_szName, command);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgGmCharInfoFldFB(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_GM_CHAR_INFO_FLD_FB *pNetMsg = (GLMSG::SNET_GM_CHAR_INFO_FLD_FB *)nmg;

	SENDTOCLIENT(pNetMsg->dwCLIENTID_FB, pNetMsg);

	return TRUE;
}

// *****************************************************
// Desc: 馨葛攪 檜翕擊 萄憮幗縑 蹂羶
// *****************************************************
BOOL GLAgentServer::MsgMove2CharAG(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_GM_MOVE2CHAR_AG *pNetMsg = (GLMSG::SNETPC_GM_MOVE2CHAR_AG *)nmg;
	GLMSG::SNETPC_GM_MOVE2CHAR_FB NetMsgFB;

	PGLCHARAG pCHAR = GetCharID(pNetMsg->dwCOMMAND_CHARID);
	if (!pCHAR)
		return FALSE;

	if (pNetMsg->emFB == EMGM_MOVE2CHAR_FB_OK)
	{
		GLMSG::SNETPC_GM_MOVE2CHAR_FLD NetMsgFld;
		NetMsgFld.sToMapID = pNetMsg->sToMapID;
		NetMsgFld.vToPos = pNetMsg->vToPos;
		SENDTOFIELD(pCHAR->m_dwClientID, &NetMsgFld);
	}
	else
	{
		//	Note : 耀掘縑啪 檜翕 褒 奩.
		NetMsgFB.emFB = pNetMsg->emFB;
		SENDTOCLIENT(pCHAR->m_dwClientID, &NetMsgFB);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgClubBattleRemainTime(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_SERVER_CLUB_BATTLE_REMAIN_AG *pNetMsg =
		(GLMSG::SNETPC_SERVER_CLUB_BATTLE_REMAIN_AG *)nmg;

	GLGuidanceAgentMan::GetInstance().RemainBattleTimeReq(pNetMsg->dwGaeaID);

	return TRUE;
}

BOOL GLAgentServer::MsgClubDeathMatchRemainTime(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_SERVER_CLUB_DEATHMATCH_REMAIN_AG *pNetMsg =
		(GLMSG::SNETPC_SERVER_CLUB_DEATHMATCH_REMAIN_AG *)nmg;

	PGLCHARAG pChar = GetChar(pNetMsg->dwGaeaID);
	if (pChar)
	{
		GLMSG::SNET_CLUB_DEATHMATCH_REMAIN_BRD NetMsgBrd;
		NetMsgBrd.dwTime = GLClubDeathMatchAgentMan().GetInstance().GetRemainTime();
		SENDTOCLIENT(pChar->m_dwClientID, &NetMsgBrd);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgClubCDCertifyIngBrd(NET_MSG_GENERIC *nmg)
{
	SENDTOALLCLIENT(nmg);

	return TRUE;
}

BOOL GLAgentServer::MsgClubCDCertifyBrd(NET_MSG_GENERIC *nmg)
{
	SENDTOALLCLIENT(nmg);

	return TRUE;
}

BOOL GLAgentServer::MsgMarketOpenCheck(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);

	if (pMyChar == NULL)
	{
		m_pConsoleMsg->Write(LOG_TEXT_CONSOLE, "%%%% Could not found Character..%d %%%%", dwGaeaID);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgClubBattleKillUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CLUB_BATTLE_KILL_UPDATE_AGT *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_KILL_UPDATE_AGT *)nmg;

	GLCLUB *pClub = m_cClubMan.GetClub(pNetMsg->dwClubID);
	if (!pClub)
		return FALSE;

	pClub->KillBattleClubMem(pNetMsg->dwBattleClubID, pNetMsg->wKillPoint);
	pClub->DeathBattleClubMem(pNetMsg->dwBattleClubID, pNetMsg->wDeathPoint);

	return TRUE;
}

BOOL GLAgentServer::MsgClubBattleLastKillUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CLUB_BATTLE_LAST_KILL_UPDATE_AGT *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_LAST_KILL_UPDATE_AGT *)nmg;

	GLCLUB *pClub = m_cClubMan.GetClub(pNetMsg->dwClubID_A);
	if (!pClub)
		return FALSE;

	pClub->SetLastBattlePoint(pNetMsg->dwClubID_B, pNetMsg->wKillPoint, pNetMsg->wDeathPoint);

	return TRUE;
}

BOOL GLAgentServer::MsgReqTaxi(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_REQ_TAXI *pNetMsg = (GLMSG::SNETPC_REQ_TAXI *)nmg;

	GLMSG::SNETPC_REQ_TAXI_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	// 薑盟濰 匐餌
	STAXI_MAP *pTaxiMap = GLTaxiStation::GetInstance().GetTaxiMap(pNetMsg->dwSelectMap);
	if (!pTaxiMap)
	{
		NetMsgFB.emFB = EMTAXI_TAKE_MAPFAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 薑盟濰 匐餌
	STAXI_STATION *pStation = pTaxiMap->GetStation(pNetMsg->dwSelectStop);
	if (!pStation)
	{
		NetMsgFB.emFB = EMTAXI_TAKE_STATIONFAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	SNATIVEID sMAPID(pStation->dwMAPID);

	if (sMAPID == SNATIVEID(false))
	{
		NetMsgFB.emFB = EMTAXI_TAKE_MAPFAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	DWORD dwFieldServer = GetFieldServer(sMAPID);
	if (dwFieldServer == FIELDSERVER_MAX)
	{
		NetMsgFB.emFB = EMTAXI_TAKE_MAPFAIL;
		SENDTOCLIENT(dwClientID, &NetMsgFB);
		return FALSE;
	}

	// 偽擎 萄檜賊 斜傖 爾頂堅
	// 棻艇 萄檜賊 謝蒂 掘憮 爾頂遽棻.
	if (dwFieldServer == pMyChar->m_dwCurFieldSvr)
	{
		SENDTOFIELD(dwClientID, pNetMsg);
	}
	else
	{
		GLMSG::SNETPC_REQ_TAXI_NPCPOS NetMsgPos;
		NetMsgPos.dwSelectMap = pNetMsg->dwSelectMap;
		NetMsgPos.dwSelectStop = pNetMsg->dwSelectStop;
		NetMsgPos.dwGaeaID = pNetMsg->dwGaeaID;
		NetMsgPos.wPosX = pNetMsg->wPosX;
		NetMsgPos.wPosY = pNetMsg->wPosY;

		SENDTOFIELDSVR(pMyChar->m_nChannel, dwFieldServer, &NetMsgPos);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgReqTaxiNpcPos(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_TAXI_NPCPOS_FB *pNetMsg = (GLMSG::SNETPC_REQ_TAXI_NPCPOS_FB *)nmg;
	GLMSG::SNETPC_REQ_TAXI_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(pNetMsg->dwGaeaID);
	if (!pMyChar)
	{
		return FALSE;
	}

	if (pNetMsg->vPos == D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX))
	{
		NetMsgFB.emFB = EMTAXI_TAKE_NPCFAIL;
		SENDTOCLIENT(pMyChar->m_dwClientID, &NetMsgFB);
		return FALSE;
	}

	GLMSG::SNETPC_REQ_TAXI NetMsg;
	NetMsg.dwSelectMap = pNetMsg->dwSelectMap;
	NetMsg.dwSelectStop = pNetMsg->dwSelectStop;
	NetMsg.wPosX = pNetMsg->wPosX;
	NetMsg.wPosY = pNetMsg->wPosY;
	NetMsg.dwGaeaID = pNetMsg->dwGaeaID;
	NetMsg.vPos = pNetMsg->vPos;

	SENDTOFIELD(pMyChar->m_dwClientID, &NetMsg);

	return TRUE;
}

BOOL GLAgentServer::MsgNpcCommission(NET_MSG_GENERIC *nmg, DWORD dwClientID)
{
	GLMSG::SNET_INVEN_NPC_COMMISSION *pNetMsg = (GLMSG::SNET_INVEN_NPC_COMMISSION *)nmg;
	GLMSG::SNET_INVEN_NPC_COMMISSION_FB NetMsgFB;

	NetMsgFB.dwCharID = pNetMsg->dwCharID;
	NetMsgFB.dwUserID = pNetMsg->dwUserID;
	NetMsgFB.lnCommission = pNetMsg->lnCommission;

	PGLCHARAG pChar = GetCharID(pNetMsg->dwCharID);

	//	議葛攪陛 蕾樓醞橾陽 ( 顫萄縑 氈朝 鼻 )
	//	綠蕾樓醞橾陽 萄縑 盪濰擊 蹂羶棻.
	if (pChar)
	{
		NetMsgFB.emFB = EMREQ_NPC_COMMISSION_CHAR;
		SENDTOFIELDSVR(pChar->m_nChannel, pChar->m_dwCurFieldSvr, &NetMsgFB);
	}
	else
	{
		NetMsgFB.emFB = EMREQ_NPC_COMMISSION_DB;
		SENDTOFIELDSVR(pNetMsg->nCHANNEL, pNetMsg->dwFieldID, &NetMsgFB);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgWoeKillAgent(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_MSG_WOE_RANKING_AGT *netmsg = (GLMSG::SNET_MSG_WOE_RANKING_AGT *)nmg;

	GLCLUB *pclub = GetClubMan().GetClub(netmsg->wClubID);

	CONSOLEMSG_WRITE("Rank: %d ~ Club Name: %s Killer: %d ~ Death: %d", netmsg->nIndex, pclub->m_szName, netmsg->wClubKiller, netmsg->wClubKilled);

	GLMSG::SNET_MSG_WOE_RANKING_CLT sendtoclient;

	sendtoclient.wClubKiller = netmsg->wClubKiller;
	sendtoclient.wClubKilled = netmsg->wClubKilled;
	StringCchCopy(sendtoclient.szName, CLUB_NAME + 1, pclub->m_szName);
	SENDTOALLCLIENT(&sendtoclient);

	return TRUE;
}

/* security 2025 - DISABLED: This handler was weaponized for mass-disconnect attacks.
   It accepts dwCharID from packet data (not authenticated sender), allowing ANY player
   to kick+ban ANY other player via WPE packet injection. Permanently disabled. */
BOOL GLAgentServer::MsgDetectCheat(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	// SECURITY FIX: Entire function disabled - was mass-disconnect backdoor
	// The packet-supplied dwCharID was used instead of authenticated dwGaeaID,
	// allowing attackers to target and ban arbitrary players
	CDebugSet::ToFileWithTime("_security_cheat_detect.txt",
							  "BLOCKED MsgDetectCheat exploit attempt from ClientID: %u, GaeaID: %u",
							  dwClientID, dwGaeaID);
	return FALSE;
}

HRESULT GLAgentServer::MsgProcess(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	switch (nmg->nType)
	{
	default:
		break;
	/* security 2025 - DISABLED: Mass-disconnect backdoor, see MsgDetectCheat() */
	// case NET_DETECT_CHEAT_C2A:
	//	MsgDetectCheat(nmg, dwClientID, dwGaeaID);
	//	break;

	/////////////////////////////////////////////////////////////////////////////
	/*woe Arc Development 08-06-2024*/
	case NET_MSG_WOE_RANKING_AGT:
		MsgWoeKillAgent(nmg);
		break;
		/*ecall function Arc Development 08-16-2024*/
	case NET_MSG_CALL_GUILD_MEM:
		MsgCallGuildMem(nmg, dwClientID, dwGaeaID);
		break;
	// LG-7 GlobalRanking
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING:
		MsgReqGlobalRanking(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_UPDATE:
		MsgReqGlobalRankingKillUpdate(nmg);
		break;
	/*re-coded LG-7 Global Ranking by DevArt22*/
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_UPDATE:
		MsgReqGlobalRankingKillUpdateBr(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_UPDATE:
		MsgReqGlobalRankingKillUpdateSw(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_UPDATE:
		MsgReqGlobalRankingKillUpdateAr(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_UPDATE:
		MsgReqGlobalRankingKillUpdateSh(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_UPDATE:
		MsgReqGlobalRankingKillUpdateEx(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_UPDATE:
		MsgReqGlobalRankingKillUpdateSc(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_UPDATE:
		MsgReqGlobalRankingKillUpdateAs(nmg);
		break;
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_UPDATE:
		MsgReqGlobalRankingRichUpdate(nmg);
		break;
	/*Top Level*/
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_LEVEL_UPDATE:
		MsgReqGlobalRankingLevelUpdate(nmg);
		break;
	/*Top Resu, Mhundz */
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RESU_UPDATE:
		MsgReqGlobalRankingResuUpdate(nmg);
		break;
	/*Top MMR, Mhundz */
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_MMR_UPDATE:
		MsgReqGlobalRankingMMRUpdate(nmg);
		break;
	/////////////////////////////////////////////////////////////////////////////
	case NET_MSG_GCTRL_REQ_GATEOUT:
		MsgReqGateOut(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_GENITEM_AGT:
		MsgReqGenItemAgt(nmg, dwClientID);
		break;
	case NET_MSG_FIELDSVR_CHARCHK_FB:
		MsgReqFieldSvrCharChkFb(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_SERVER_GENERALCHAT:
		MsgServerGeneralChat(nmg);
		break;

	case NET_MSG_GCTRL_CONFRONTPTY_CHECKMBR_AGT:
		MsgConfrontPtyCheckMber(nmg);
		break;
	case NET_MSG_GCTRL_CONFRONTCLB_CHECKMBR_AGT:
		MsgConfrontClbCheckMber(nmg);
		break;

	case NET_MSG_GCTRL_CLUB_MEMBER_DB2DEL:
		MsgClubMemberDb2Del(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_CLUB_CERTIFIED_AGT:
		MsgClubCertified(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_CLUB_INCOME_MONEY_AGT:
		MsgClubIncomeMoney(nmg);
		break;
	case NET_MSG_GCTRL_CLUB_INCOME_UP:
		MsgClubIncomeMoneyUp(nmg);
		break;
	// case NET_MSG_GCTRL_CLUB_INCOME_FB:				MsgClubIncomeMoneyFb ( nmg ); break;
	case NET_MSG_GCTRL_2_FRIEND_AG:
		Msg2FriendAG(nmg);
		break;
	case NET_MSG_GM_MOVE2CHAR_AG:
		MsgMove2CharAG(nmg);
		break;
	case NET_MSG_GM_CHAR_INFO_FLD_FB:
		MsgGmCharInfoFldFB(nmg);
		break;

	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_ING_BRD:
		MsgClubCDCertifyIngBrd(nmg);
		break;
	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_BRD:
		MsgClubCDCertifyBrd(nmg);
		break;
	case NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE_AGT:
		MsgClubBattleKillUpdate(nmg);
		break;
	case NET_MSG_GCTRL_CLUB_BATTLE_LAST_KILL_UPDATE_AGT:
		MsgClubBattleLastKillUpdate(nmg);
		break;
	case NET_MSG_GCTRL_REQ_TAXI_NPCPOS_FB:
		MsgReqTaxiNpcPos(nmg);
		break;
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_REQ:
		MsgReqCreateInstantMap(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_DEL:
		MsgReqDeleteInstantMap(nmg);
		break;
	case NET_MSG_GCTRL_NPC_COMMISSION:
		MsgNpcCommission(nmg, dwClientID);
		break;

	/* Codex, Archie 02/16/24 */
	case NET_MSG_GCTRL_CODEX_NOTIFY_AGENT:
		MsgActivityBroadcast(nmg);
		break;
		/*activity system, Juver, 2017/10/30 */
	case NET_MSG_GCTRL_ACTIVITY_NOTIFY_AGENT:
		MsgActivityBroadcast(nmg);
		break;

		/*character disconnect function, EJCode, 2018/11/25 */
	case NET_MSG_GCTRL_CHARACTER_DISCONNECT_FIELD_TO_AGENT:
		msg_character_disconnect(nmg, dwClientID);
		break;

	case NET_MSG_GCTRL_CLUB_SCORE_KILL_FLD_2_AGT:
		MsgClubScoreKill(nmg);
		break;
	case NET_MSG_GCTRL_CLUB_SCORE_RESU_FLD_2_AGT:
		MsgClubScoreResu(nmg);
		break;

		/* charinfoview, Juver, 2020/03/03 */
	case NET_MSG_GCTRL_REQ_CHARINFO_AGT:
		CharInfoViewRequestAGT(nmg);
		break;

		/* gm command send item, Juver, 2021/02/14 */
	case NET_MSG_GCTRL_GM_COMMAND_F2A_SEND_ITEM_FB:
		MsgGMSendItemFB(nmg);
		break;

		/* max level notice, Juver, 2021/07/26 */
	case NET_MSG_GCTRL_MAX_LEVEL_NOTICE_AGENT:
		MsgMaxLevelNotice(nmg);
		break;
	};

	/*pvp tyranny, Juver, 2017/08/24 */
	switch (nmg->nType)
	{
	default:
		break;

	case NET_MSG_GCTRL_TYRANNY_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_TYRANNY_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_TYRANNY_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_TYRANNY_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_TYRANNY_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_TYRANNY_F2A_TOWER_CAPTURE:
	case NET_MSG_GCTRL_TYRANNY_C2A_TOWER_INFO_REQ:
	case NET_MSG_GCTRL_TYRANNY_F2A_TOWER_SCORE:
	case NET_MSG_GCTRL_TYRANNY_C2A_LATE_JOIN_REQ:
	case NET_MSG_GCTRL_TYRANNY_F2A_GUILD_TOWER_CAPTURE:
		GLPVPTyrannyAgent::GetInstance().MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	};

	/*school wars, Juver, 2018/01/19 */
	switch (nmg->nType)
	{
	default:
		break;

	case NET_MSG_GCTRL_SCHOOLWARS_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_C2A_SCORE_INFO_REQ:
	case NET_MSG_GCTRL_SCHOOLWARS_F2A_SCORE_UPDATE:
	case NET_MSG_GCTRL_SCHOOLWARS_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_SCHOOLWARS_F2A_MOB_KILL_UPDATE:
		GLPVPSchoolWarsAgent::GetInstance().MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	};

	/*pvp capture the flag, Juver, 2018/01/26 */
	switch (nmg->nType)
	{
	default:
		break;

	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_SCORE_INFO_REQ:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_KILL_UPDATE:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_CAPTURE_UPDATE:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_FLAG_HOLD:
		GLPVPCaptureTheFlagAgent::GetInstance().MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	};

	/*dmk14 freepk*/
	switch (nmg->nType)
	{
	default:
		break;
	case NET_MSG_GCTRL_GM_FREEPK_F2A_BATTLEINFO_PC:
		m_cFreePK.MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	}

	switch (nmg->nType)
	{
	default:
		break;

		/* pvp club death match, Juver, 2020/11/11 */
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE:
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END:
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ:
		PVPClubDeathMatchAgent::GetInstance().MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	};

	/*woe Arc Development 08-06-2024*/
	switch (nmg->nType)
	{
	default:
		break;
	case NET_MSG_GCTRL_WOE_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_WOE_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_WOE_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_WOE_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_WOE_C2A_MAPEXIT_REQ:
	case NET_MSG_GCTRL_WOE_F2A_TOWER_CAPTURE:
	case NET_MSG_GCTRL_WOE_C2A_TOWER_INFO_REQ:
	case NET_MSG_GCTRL_WOE_F2A_TOWER_SCORE:
	case NET_MSG_WOE_CAPTURE_AGT:
		GLPVPWoeAgent::GetInstance().MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	};

	switch (nmg->nType)
	{
	default:
		break;

	case NET_MSG_GCTRL_PVP_PBG_F2A_BATTLEINFO_PC:
	case NET_MSG_GCTRL_PVP_PBG_C2A_REGISTER_REQ:
	case NET_MSG_GCTRL_PVP_PBG_C2A_REJOIN_REQ:
	case NET_MSG_GCTRL_PVP_PBG_F2A_RANKINFO_PC:
	case NET_MSG_GCTRL_PVP_PBG_C2A_MAPEXIT_REQ:
		PVPPBG::ManagerAgent::GetInstance().MsgProcess(nmg, dwClientID, dwGaeaID);
		break;
	};

	if (dwGaeaID == GAEAID_NULL)
		return E_FAIL;
	if (!GetChar(dwGaeaID))
		return E_FAIL;

	switch (nmg->nType)
	{
	default:
		break;

	case NET_MSG_REQ_FRIENDLIST:
	case NET_MSG_REQ_FRIENDADD:
	case NET_MSG_REQ_FRIENDADD_ANS:
	case NET_MSG_REQ_FRIENDDEL:
	case NET_MSG_REQ_FRIENDBLOCK:

	case NET_MSG_GCTRL_ACTSTATE:

	case NET_MSG_GCTRL_CLUB_MEMBER_REQ_2AGT:
	case NET_MSG_GCTRL_CLUB_MEMBER_DEL:
	case NET_MSG_GCTRL_CLUB_MEMBER_SECEDE:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_MARK_INFO:
	case NET_MSG_GCTRL_CLUB_MARK_CHANGE:
	case NET_MSG_GCTRL_CLUB_RANK_2AGT:

	case NET_MSG_GCTRL_CLUB_COMMISSION:
	case NET_MSG_GCTRL_CLUB_NOTICE_REQ:
	case NET_MSG_GCTRL_CLUB_SUBMASTER:

	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_REQ:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_REQ:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_REQ:

	case NET_MSG_GCTRL_CLUB_BATTLE_REQ:
	case NET_MSG_GCTRL_CLUB_BATTLE_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ANS:
	case NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ:

	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ANS:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ:

	case NET_MSG_GCTRL_INVEN_RENAME_AGTBRD:
	case NET_MSG_SMS_PHONE_NUMBER_AGTBRD:
	case NET_MSG_GCTRL_UPDATE_STARTCALL:

	case NET_MSG_GCTRL_PUTON_CHANGE_AG:
	case NET_MSG_REQ_FRIEND_CLUB_OPEN:
	case NET_QBOX_OPTION_REQ_AG:
	case NET_MSG_REQ_ATTENDLIST:
	case NET_MSG_REQ_ATTENDANCE:

		/* award title, Juver, 2021/02/12 */
	case NET_MSG_GCTRL_C2A_AWARD_TITLE_REQUEST_DATA:
		// offline vend
	case NET_MSG_GCTRL_PMARKET_OPEN_AGT:
	{
		PGLCHARAG pMyChar = GetChar(dwGaeaID);
		pMyChar->MsgProcess(nmg);
	}
	break;

	case NET_MSG_CHAT:
		ChatMsgProc(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_CHAT_LOUDSPEAKER_AGT:
		ChatMsgLoudSpeakerAgt(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_REBIRTH:
		MsgReqReBirth(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REGEN_GATE_FB:
		MsgReqReGenGate(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_FIELDSVR_OUT_FB:
		MsgFieldSvrOutFb(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REBIRTH_OUT_FB:
		MsgReBirthOutFb(nmg, dwClientID, dwGaeaID);
		break;

		/* pvp club death match, Juver, 2020/11/26 */
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_FIELD_OUT_FB:
		MsgPVPClubDeathMatchReBirthOutFb(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_PVP_PBG_F2A_FIELD_OUT_FB:
		MsgPVPPBGReBirthOutFb(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_REQ_RECALL_AG:
		MsgReCallAg(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_REQ_MUST_LEAVE_MAP_AG:
		MsgLvUpMustLeaveMap(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_TELEPORT_AG:
		MsgTeleportAg(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_SERVER_CLUB_BATTLE_REMAIN_AG:
		MsgClubBattleRemainTime(nmg);
		break;
	case NET_MSG_GCTRL_REQ_SERVER_CLUB_DEATHMATCH_REMAIN_AG:
		MsgClubDeathMatchRemainTime(nmg);
		break;

	case NET_MSG_GCTRL_PARTY_LURE:
		MsgPartyLure(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_PARTY_LURE_TAR_ANS:
		MsgPartyLureTarAns(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_PARTY_SECEDE:
		MsgPartySecede(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_PARTY_AUTHORITY:
		MsgPartyAuthority(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_PARTY_DISSOLVE:
		MsgPartyDissolve(nmg, dwClientID, dwGaeaID);
		break;

		/* modern party window, Juver, 2019/12/20 */
	case NET_MSG_GCTRL_PARTY_CHANGEOPTION:
		MsgPartyChangeOption(nmg, dwClientID, dwGaeaID);
		break;

		/* party charid/charname, Juver, 2020/03/02 */
	case NET_MSG_GCTRL_PARTY_LURE_CHAR_ID:
		MsgPartyLureCharID(nmg, dwClientID, dwGaeaID);
		break;

		/* party charid/charname, Juver, 2020/03/02 */
	case NET_MSG_GCTRL_PARTY_LURE_CHAR_NAME:
		MsgPartyLureCharName(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_2_FRIEND_REQ:
		Msg2FriendReq(nmg, dwClientID, dwGaeaID);
		break;
	/*dmk14 fast teleport fix*/
	case NET_MSG_GCTRL_2_FRIEND_REQ_CANCEL:
		Msg2FriendCancel(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_MOVE2CHAR:
		MsgMove2Char(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_REQ_GATEOUT_FB:
		MsgReqGateOutFB(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_REBIRTH_FB:
		MsgReqReBirthFB(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_FB:
		MsgReqCreateInstantMapFB(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_CONFRONT:
		MsgReqConfront(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_CONFRONT_ANS:
		MsgAnsConfront(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_CONFRONT_FB:
		MsgAnsConfrontFB(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_CONFRONT_END2_AGT:
		MsgConfrontEnd2Agt(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_CLUB_NEW_2AGT:
		MsgClubNew2Agt(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_CLUB_NEW_DB2AGT:
		MsgClubNewDb2Agt(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_CLUB_DISSOLUTION:
		MsgClubDissolution(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_CHARPOS_FROMDB2AGT:
		GameJoinToFieldSvr(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_SERVER_CTRL_WEATHER:
		MsgServerCtrlWeather(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_SERVER_CTRL_WEATHER2:
		MsgServerCtrlWeather2(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_SERVER_CTRL_GENITEMHOLD:
		MsgServerCtrlGenHold(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_SERVER_CTRL_TIME:
		MsgServerCtrlTime(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_SERVER_CTRL_MONTH:
		MsgServerCtrlMonth(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_BLOCK_DETECTED:
		MsgBlockDetected(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_KICK_USER:
		MsgGmKickUser(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOVE2GATE:
		MsgGMMove2Gate(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOVE2MAPPOS:
		MsgGMMove2Pos(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_EVENT_ITEM_GEN:
		MsgGmEventItemGen(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_EVENT_ITEM_GEN_END:
		MsgGmEventItemGenEnd(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_EVENT_MONEY_GEN:
		MsgGmEventMoneyGen(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_EVENT_MONEY_GEN_END:
		MsgGmEventMoneyGenEnd(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_EVENT_EXP:
		MsgGmEventExp(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_EVENT_EXP_END:
		MsgGmEventExpEnd(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_EVENT_EX:
		MsgGmEventEx(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_EVENT_EX_END:
		MsgGmEventExEnd(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_VIEWWORKEVENT:
		MsgGmViewWorkEvent(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_LIMIT_EVENT_BEGIN:
		MsgGmLimitEventBegin(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_LIMIT_EVENT_END:
		MsgGmLimitEventEnd(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_LIMIT_EVENT_RESTART:
		MsgGmLimitEventRestart(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_LIMIT_EVENT_TIME_REQ:
		MsgGmLimitEventTimeReq(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_CYBERCAFECLASS_UPDATE:
		MsgCyberCafeClassUpdate(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_MARKET_CHECKTIME:
		MsgMarketOpenCheck(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_BIGHEAD:
		MsgGmBigHead(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_BIGHAND:
		MsgGmBigHand(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_CHAT_BLOCK_UACCOUNT:
		MsgGmChatBlockUA(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_CHAT_BLOCK_CHARNAME:
		MsgGmChatBlockCN(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_CHAT_BLOCK_CHARID:
		MsgGmChatBlockCI(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_CHAR_INFO_4NAME:
		MsgGmCharInfo4NAME(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_CHAR_INFO_4CHARID:
		MsgGmCharInfo4CID(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_FREEPK:
		MsgGmFreePK(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_VIEWALLPLAYER:
		MsgGmViewAllPlayer(nmg, dwClientID, dwGaeaID);
		break;

		/*private market set, Juver, 2018/01/02 */
	case NET_MSG_GM_SET_PRIVATE_MARKET:
		MsgGmSetPrivateMarket(nmg, dwClientID, dwGaeaID);
		break;

		/*megaphone set, Juver, 2018/01/02 */
	case NET_MSG_GM_SET_MEGAPHONE:
		MsgGmSetMegaPhone(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_GENCHAR:
		MsgGmGenChar(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOB_GEN:
		MsgGmGenMob(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOB_GEN_EX:
		MsgGmGenMobEx(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOB_DEL_EX:
		MsgGmDelMobEx(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOB_DEL:
		MsgGmDelMob(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOB_LEVEL:
		MsgGmMobLevel(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_MOB_LEVEL_CLEAR:
		MsgGmMobLevelClear(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_WHERE_NPC:
		MsgGmWhereNPC(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_PRINT_CROWLIST:
		MsgGmPrintCrowList(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_WARNING_MSG:
		MsgGmWarningMSG(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GM_SHOWMETHEMONEY:
		MsgGmShowMeTheMoney(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GM_GETWHISPERMSG:
		MsgGetWhisperMsg(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_REQ_TAXI:
		MsgReqTaxi(nmg, dwClientID, dwGaeaID);
		break;

		/*game stats, Juver, 2017/06/21 */
	case NET_MSG_GCTRL_PING_PACKET_AGENT:
		MsgReplyPing(nmg, dwClientID, dwGaeaID);
		break;

		/*event map move, Juver, 2017/08/25 */
	case NET_MSG_GM_MOVE2_MAPPOS:
		MsgGMMove2_MapPos(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY:
		MsgEventContriTyranny(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR:
		MsgEventContriSchoolWar(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG:
		MsgEventContriCaptureTheFlag(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_GM_COMMAND_KICKOUT:
		MsgGMKickOut(nmg, dwClientID, dwGaeaID);
		break;

		/* party finder, Juver, 2020/01/03 */
	case NET_MSG_GCTRL_PARTY_FINDER_SEARCH:
		PartyFinderSearch(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_PARTY_FINDER_JOIN:
		PartyFinderJoin(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_PARTY_FINDER_REPLY:
		PartyFinderReply(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_GM_PCID:
		MsgGMPCID(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_GM_PCID_REPLY:
		MsgGMPCIDReply(nmg, dwClientID, dwGaeaID);
		break;
	case NET_MSG_GCTRL_GM_PCID_REPLY2:
		MsgGMPCIDReply2(nmg, dwClientID, dwGaeaID);
		break;

		/* charinfoview, Juver, 2020/03/03 */
	case NET_MSG_GCTRL_REQ_CHARINFO_GAEA:
		CharInfoViewRequestGaea(nmg, dwClientID, dwGaeaID);
		break;

		/* charinfoview, Juver, 2020/03/03 */
	case NET_MSG_GCTRL_REQ_CHARINFO_CHAR_ID:
		CharInfoViewRequestCharID(nmg, dwClientID, dwGaeaID);
		break;

		/* charinfoview, Juver, 2020/03/03 */
	case NET_MSG_GCTRL_REQ_CHARINFO_CHAR_NAME:
		CharInfoViewRequestCharName(nmg, dwClientID, dwGaeaID);
		break;

		/* user flag restricted, Juver, 2020/04/21 */
	case NET_MSG_GCTRL_GM_USER_RESTRICT:
		MsgGMUserRestrict(nmg, dwClientID, dwGaeaID);
		break;

		/* pvp club death match, Juver, 2020/11/26 */
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ:
		MsgReqPVPClubDeathMatchReBirth(nmg, dwClientID, dwGaeaID);
		break;

		/* pvp club death match, Juver, 2020/11/26 */
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB:
		MsgReqPVPClubDeathMatchReBirthFB(nmg, dwClientID, dwGaeaID);
		break;

		/* genchar pt, Juver, 2020/12/03 */
	case NET_MSG_GM_GENCHAR_PARTY:
		MsgGmGenCharParty(nmg, dwClientID, dwGaeaID);
		break;

		/* force auto revive, Juver, 2020/12/24 */
	case NET_MSG_GCTRL_F2A_FORCE_AUTO_REVIVE:
		MsgForceAutoRevive(nmg);
		break;

		/* gm command send item, Juver, 2021/02/14 */
	case NET_MSG_GCTRL_GM_COMMAND_C2A_SEND_ITEM:
		MsgGMSendItem(nmg, dwClientID, dwGaeaID);
		break;

		/* game notice, Juver, 2021/06/12 */
	case NET_MSG_GCTRL_GM_GAME_NOTICE_RELOAD:
		MsgGetGameNotice(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_GM_GIS_RELOAD:
		MsgGISReload(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_ENCHANT_NOTIFY_AGT:
		MsgEnchantNotify(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_COMBO_WHORE_NOTIFY_AGT:
		MsgComboWhoreNotify(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_CAPTCHA_UPDATE_INPUT_CA:
		MsgCaptchaInput(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_PVP_PBG_C2AF_REVIVE_REQ:
		MsgReqPVPPBGReBirth(nmg, dwClientID, dwGaeaID);
		break;

	case NET_MSG_GCTRL_PVP_PBG_F2AC_REVIVE_REQ_FB:
		MsgReqPVPPBGReBirthFB(nmg, dwClientID, dwGaeaID);
		break;
	};

	return S_OK;
}

/*game stats, Juver, 2017/06/21 */
BOOL GLAgentServer::MsgReplyPing(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_PING_PACKET_AGENT *pNetMsg = (GLMSG::SNETPC_PING_PACKET_AGENT *)nmg;

	GLMSG::SNETPC_PING_PACKET_AGENT_FB NetMsgFB;

	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	SENDTOCLIENT(dwClientID, &NetMsgFB);

	return TRUE;
}

/*activity system, Juver, 2017/10/30 */
BOOL GLAgentServer::MsgActivityBroadcast(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_ACTIVITY_NOTIFY_AGENT *pNetMsg = (GLMSG::SNETPC_ACTIVITY_NOTIFY_AGENT *)nmg;

	GLMSG::SNETPC_ACTIVITY_NOTIFY_CLIENT netmsgclient;
	StringCchCopy(netmsgclient.szCharacterName, CHAR_SZNAME + 1, pNetMsg->szCharacterName);
	StringCchCopy(netmsgclient.szActivityTitle, ACTIVITY_TITLE_MSG_SIZE + 1, pNetMsg->szActivityTitle);
	GLAgentServer::GetInstance().SENDTOALLCLIENT(&netmsgclient);

	return TRUE;
}
/* Codex, Archie 02/16/24 */
BOOL GLAgentServer::MsgCodexBroadcast(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_CODEX_NOTIFY_AGENT *pNetMsg = (GLMSG::SNETPC_CODEX_NOTIFY_AGENT *)nmg;

	GLMSG::SNETPC_CODEX_NOTIFY_CLIENT netmsgclient;
	StringCchCopy(netmsgclient.szCharacterName, CHAR_SZNAME + 1, pNetMsg->szCharacterName);
	StringCchCopy(netmsgclient.szCodexTitle, CODEX_TITLE_MSG_SIZE + 1, pNetMsg->szCodexTitle);
	GLAgentServer::GetInstance().SENDTOALLCLIENT(&netmsgclient);

	return TRUE;
}
/*character disconnect function, EJCode, 2018/11/25 - DISABLED FOR SECURITY */
BOOL GLAgentServer::msg_character_disconnect(NET_MSG_GENERIC *nmg, DWORD dwClientID)
{
	GLMSG::SNET_CHARACTER_DISCONNECT_FIELD_TO_AGENT *pnet_msg = (GLMSG::SNET_CHARACTER_DISCONNECT_FIELD_TO_AGENT *)nmg;

	PGLCHARAG pchar = GetCharID(pnet_msg->char_id);
	if (!pchar)
		return FALSE;

	// SECURITY FIX: Simple logging and blocking without complex IP tracking
	// This prevents mass disconnections during SERVER_INFO updates

	// Display to console/admin - simple version
	CONSOLEMSG_WRITE("[SECURITY ALERT] AGENT DISCONNECT ATTACK BLOCKED!");
	CONSOLEMSG_WRITE("  Client ID: %u | Target Player: %s [ID:%u] | Force Close: %s",
					 dwClientID, pchar->m_szName, pchar->m_dwCharID, pnet_msg->bCloseClient ? "YES" : "NO");
	CONSOLEMSG_WRITE("  Attack neutralized - Player remains connected");

	// Simple security logging
	CDebugSet::ToFileWithTime("_security_disconnect_attacks.txt",
							  "BLOCKED AGENT DISCONNECT ATTACK - Client: %u | Target: %s [%u] | ForceClose: %s",
							  dwClientID, pchar->m_szName, pchar->m_dwCharID,
							  pnet_msg->bCloseClient ? "true" : "false");

	// Log the original behavior for debugging
	if (RPARAM::bCharacterDcLogs == 1)
	{
		CDebugSet::ToFileWithTime("_character_disconnect.txt",
								  "BLOCKED agent disconnect [%u] %s", pchar->m_dwCharID, pchar->m_szName);
	}

	// ABSOLUTE PROTECTION: Never execute these under any circumstances
	// SENDTOALLCHANNEL ( &net_msg_field );     // PERMANENTLY DISABLED
	// SENDTOCLIENT ( pchar->m_dwClientID, &net_msg_client );  // PERMANENTLY DISABLED
	// ReserveDropOutPC ( pchar->m_dwGaeaID );  // PERMANENTLY DISABLED

	return TRUE; // Return success but don't actually disconnect
}

void GLAgentServer::MsgClubScoreKill(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CLUB_SCORE_KILL_FLD_2_AGT *pNetMsg = (GLMSG::SNET_CLUB_SCORE_KILL_FLD_2_AGT *)nmg;

	GLGuidance *pGUID = GLGuidanceAgentMan::GetInstance().Find(pNetMsg->dwID);
	if (pGUID)
		pGUID->ScoreKill(pNetMsg->dwCharIDKilled, pNetMsg->dwCharIDKiller);
}

void GLAgentServer::MsgClubScoreResu(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNET_CLUB_SCORE_RESU_FLD_2_AGT *pNetMsg = (GLMSG::SNET_CLUB_SCORE_RESU_FLD_2_AGT *)nmg;

	GLGuidance *pGUID = GLGuidanceAgentMan::GetInstance().Find(pNetMsg->dwID);
	if (pGUID)
		pGUID->ScoreResu(pNetMsg->dwCharID);
}

/* force auto revive, Juver, 2020/12/24 */
BOOL GLAgentServer::MsgForceAutoRevive(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_F2C_FORCE_AUTO_REVIVE *pNetMsg = (GLMSG::SNETPC_F2C_FORCE_AUTO_REVIVE *)nmg;

	DWORD dwGaeaID = pNetMsg->dwGaeaID;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	DWORD dwClientID = pChar->m_dwClientID;

	RLOG::LogFile("_ForceAutoRevive.txt", "[%u]%s map[%u/%u]", pChar->m_dwCharID, pChar->m_szName, pChar->m_sCurMapID.wMainID, pChar->m_sCurMapID.wSubID);

	GLMSG::SNETPC_REQ_REBIRTH netMsg;
	netMsg.bRegenEntryFailed = false;
	MsgReqReBirth((NET_MSG_GENERIC *)&netMsg, dwClientID, dwGaeaID);

	return TRUE;
}

BOOL GLAgentServer::MsgEnchantNotify(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_ENCHANT_NOTIFY_AGT *pNetMsg = (GLMSG::SNETPC_ENCHANT_NOTIFY_AGT *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNETPC_ENCHANT_NOTIFY_CLIENT NetMsgClient;
	StringCchCopy(NetMsgClient.szName, CHAR_SZNAME, pNetMsg->szName);
	NetMsgClient.sItemCustom = pNetMsg->sItemCustom;
	GLAgentServer::GetInstance().SENDTOALLCLIENT(&NetMsgClient);

	return TRUE;
}

BOOL GLAgentServer::MsgComboWhoreNotify(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNETPC_COMBO_WHORE_NOTIFY_AGT *pNetMsg = (GLMSG::SNETPC_COMBO_WHORE_NOTIFY_AGT *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNETPC_COMBO_WHORE_NOTIFY_CLIENT NetMsgClient;
	StringCchCopy(NetMsgClient.szName, CHAR_SZNAME, pNetMsg->szName);
	GLAgentServer::GetInstance().SENDTOALLCLIENT(&NetMsgClient);

	return TRUE;
}

BOOL GLAgentServer::MsgCaptchaInput(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	GLMSG::SNET_CAPTCHA_UPDATE_INPUT_CA *pNetMsg = (GLMSG::SNET_CAPTCHA_UPDATE_INPUT_CA *)nmg;

	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	pChar->CaptchaInput(pNetMsg->dwCaptcha);

	return TRUE;
}

BOOL GLAgentServer::MsgReqPVPPBGReBirth(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	GLMSG::SNETPC_PVP_PBG_C2AF_REVIVE_REQ *pNetMsg = (GLMSG::SNETPC_PVP_PBG_C2AF_REVIVE_REQ *)nmg;

	SNATIVEID sLobbyMap = PVPPBG::ManagerAgent::GetInstance().m_sLobbyMap;
	DWORD dwLobbyGate = PVPPBG::ManagerAgent::GetInstance().m_dwLobbyGate;

	GLAGLandMan *pLandMan = GetByMapID(sLobbyMap);
	if (!pLandMan)
	{
		CDebugSet::ToFileWithTime("_PVPPBG.txt", "GLAgentServer::MsgReqPVPPBGReBirth invalid lobby map[%d/%d]", sLobbyMap.wMainID);
		GLAgentServer::GetInstance().CONSOLEMSG_WRITE("GLAgentServer::MsgReqPVPPBGReBirth invalid lobby map[%d/%d]", sLobbyMap.wSubID);

		return FALSE;
	}

	PDXLANDGATE pLandGate = pLandMan->GetLandGateMan()->FindLandGate(dwLobbyGate);
	if (!pLandGate)
	{
		CDebugSet::ToFileWithTime("_PVPPBG.txt", "GLAgentServer::MsgReqPVPPBGReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wMainID);
		GLAgentServer::GetInstance().CONSOLEMSG_WRITE("GLAgentServer::MsgReqPVPPBGReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wSubID);

		return FALSE;
	}

	DWORD dwNextFieldSvr = GetFieldServer(sLobbyMap);
	if (dwNextFieldSvr == pChar->m_dwCurFieldSvr)
	{
		pChar->SetCurrentField(pChar->m_dwCurFieldSvr, sLobbyMap);

		SENDTOFIELD(dwClientID, nmg);
	}
	else
	{
		ReActionMoveFieldSvr(dwClientID, dwGaeaID);

		GLMSG::SNETPC_PVP_PBG_A2F_REVIVE_REQ_FIELD_OUT NetMsg;
		SENDTOFIELD(dwClientID, &NetMsg);

		pChar->SetNextFieldSvr(sLobbyMap, dwLobbyGate, D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX), dwNextFieldSvr);
	}

	return TRUE;
}

BOOL GLAgentServer::MsgPVPPBGReBirthOutFb(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pChar = GetChar(dwGaeaID);
	if (!pChar)
		return FALSE;

	if (pChar->m_sNextMapID == SNATIVEID(MAXLANDMID, MAXLANDSID))
		return FALSE;

	//	Note : 營 萄 憮幗 塽 裘 ID 雖薑.
	pChar->SetCurrentField(pChar->m_dwNextFieldSvr, pChar->m_sNextMapID);

	//	Note : 裘縑 億煎 霞殮晦 嬪憮 Field憮幗縑 詭衛雖 歎褐.
	//
	NET_GAME_JOIN_FIELDSVR NetJoinField;
	NetJoinField.emType = EMJOINTYPE_PVP_CLUB_DEATH_MATCH_REBIRTH;

	NetJoinField.dwSlotAgentClient = pChar->m_dwClientID;
	NetJoinField.nUserNum = pChar->m_dwUserID;
	StringCchCopy(NetJoinField.szUID, DAUM_MAX_UID_LENGTH + 1, pChar->m_szUserName);
	NetJoinField.dwUserLvl = pChar->m_dwUserLvl;
	NetJoinField.nChaNum = pChar->m_dwCharID;
	NetJoinField.dwGaeaID = pChar->m_dwGaeaID;
	NetJoinField.tPREMIUM = pChar->m_tPREMIUM;
	NetJoinField.tCHATBLOCK = pChar->m_tCHATBLOCK;

	NetJoinField.sStartMap = pChar->m_sNextMapID;
	NetJoinField.dwStartGate = pChar->m_dwNextGateID;
	NetJoinField.vStartPos = D3DXVECTOR3(0, 0, 0);

	NetJoinField.dwActState = pChar->m_dwActState;
	NetJoinField.bUseArmSub = pChar->m_bUseArmSub;

	NetJoinField.dwThaiCCafeClass = pChar->m_dwThaiCCafeClass;
	NetJoinField.nMyCCafeClass = pChar->m_nMyCCafeClass; // 蜓溯檜衛嬴 PC寞 檜漸

	NetJoinField.sChinaTime = pChar->m_sChinaTime;
	NetJoinField.sEventTime = pChar->m_sEventTime;
	NetJoinField.sVietnamGainSystem = pChar->m_sVietnamSystem;

	/* user flag verified, Juver, 2020/02/25 */
	NetJoinField.bUserFlagVerified = pChar->m_bUserFlagVerified;

	/* user flag restricted, Juver, 2020/04/20 */
	NetJoinField.bUserFlagRestricted = pChar->m_bUserFlagRestricted;

	//** Add EventTime

	pChar->ResetNextFieldSvr();

	SENDTOFIELD(dwClientID, &NetJoinField);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
BOOL SortByHighestKill(const STOP_RANK_KILL &lTopRank, const STOP_RANK_KILL &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillBr(const STOP_RANK_KILL_BR &lTopRank, const STOP_RANK_KILL_BR &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillSw(const STOP_RANK_KILL_SW &lTopRank, const STOP_RANK_KILL_SW &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillAr(const STOP_RANK_KILL_AR &lTopRank, const STOP_RANK_KILL_AR &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillSh(const STOP_RANK_KILL_SH &lTopRank, const STOP_RANK_KILL_SH &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillEx(const STOP_RANK_KILL_EX &lTopRank, const STOP_RANK_KILL_EX &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillSc(const STOP_RANK_KILL_SC &lTopRank, const STOP_RANK_KILL_SC &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
/*re-coded LG-7 Global Ranking by DevArt22*/
BOOL SortByHighestKillAs(const STOP_RANK_KILL_AS &lTopRank, const STOP_RANK_KILL_AS &RTopRank)
{
	return lTopRank.dwChaPKWin > RTopRank.dwChaPKWin;
}
BOOL SortByHighestRich(const STOP_RANK_RICH &lTopRank, const STOP_RANK_RICH &RTopRank)
{
	return lTopRank.llnMoney > RTopRank.llnMoney;
}
/*Top Level*/
BOOL SortByHighestLevel(const STOP_RANK_LEVEL &lTopRank, const STOP_RANK_LEVEL &RTopRank)
{
	return lTopRank.llnExp > RTopRank.llnExp;
}
/*Top Resu, Mhundz */
BOOL SortByHighestResu(const STOP_RANK_RESU &lTopRank, const STOP_RANK_RESU &RTopRank)
{
	return lTopRank.wResu > RTopRank.wResu;
}
/*Top MMR, Mhundz */
BOOL SortByHighestMMR(const STOP_RANK_MMR &lTopRank, const STOP_RANK_MMR &RTopRank)
{
	return lTopRank.wMMR > RTopRank.wMMR;
}
BOOL SortByHighestWin(const STOP_RANK_GUILD &lTopRank, const STOP_RANK_GUILD &RTopRank)
{
	return lTopRank.dwWin > RTopRank.dwWin;
}
BOOL SortByOnline(const STOP_RANK_GUILD &lTopRank, const STOP_RANK_GUILD &RTopRank)
{
	return lTopRank.wOnline > RTopRank.wOnline;
}

BOOL GLAgentServer::MsgReqGlobalRanking(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pCharAG = GetChar(dwGaeaID);
	if (!pCharAG)
		return FALSE;

	if (!pCharAG->m_bGlobalRanking)
	{
		GLMSG::SNETPC_REQ_GLOBAL_RANKING_FB NetMsg;
		NetMsg.emFB = EM_GLOBAL_RANKING_FB_DELAY;
		NetMsg.fDelay = pCharAG->m_fGlobalRanking;
		SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsg);

		return FALSE;
	}

	pCharAG->m_bGlobalRanking = FALSE;

	// Top Kill |
	{
		VEC_TOP_KILL vecTopKill = m_vecTopKill;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKill);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_KILL;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_KILL;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_KILL;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_KILL;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_KILL;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	{
	}

	/*re-coded LG-7 Global Ranking by DevArt22*/
	// Top Kill Brawler
	{
		VEC_TOP_KILL_BR vecTopKill = m_vecTopKillBr;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillBr);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_BRAWLER;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_Br[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Br[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Br[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_BRAWLER;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_Br[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Br[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Br[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_BRAWLER;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_Br[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Br[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Br[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_BRAWLER;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_Br[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Br[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Br[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_BRAWLER;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_Br[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Br[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Br[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Br[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	{
	}

	// Top Kill Swordsman
	{
		VEC_TOP_KILL_SW vecTopKill = m_vecTopKillSw;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillSw);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SWORD;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_Sw[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SWORD;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_Sw[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SWORD;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_Sw[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SWORD;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_Sw[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SWORD;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_Sw[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sw[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	{
	}

	// Top Kill Archer
	{
		VEC_TOP_KILL_AR vecTopKill = m_vecTopKillAr;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillAr);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ARCHER;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_Ar[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ARCHER;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_Ar[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ARCHER;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_Ar[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ARCHER;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_Ar[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ARCHER;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_Ar[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ar[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	{
	}

	// Top Kill Shaman
	{
		VEC_TOP_KILL_SH vecTopKill = m_vecTopKillSh;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillSh);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SHAMAN;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_Sh[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SHAMAN;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_Sh[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SHAMAN;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_Sh[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SHAMAN;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_Sh[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_SHAMAN;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_Sh[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sh[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	// Top Kill Extreme
	{
		VEC_TOP_KILL_EX vecTopKill = m_vecTopKillEx;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillEx);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_EXTREME;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_Ex[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_EXTREME;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_Ex[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_EXTREME;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_Ex[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_EXTREME;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_Ex[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_EXTREME;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_Ex[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Ex[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	// Top Kill Gunner
	{
		VEC_TOP_KILL_SC vecTopKill = m_vecTopKillSc;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillSc);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUNNER;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_Sc[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUNNER;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_Sc[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUNNER;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_Sc[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUNNER;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_Sc[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUNNER;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_Sc[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_Sc[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	// Top Kill Assassin
	{
		VEC_TOP_KILL_AS vecTopKill = m_vecTopKillAs;

		if (!vecTopKill.empty())
		{
			sort(vecTopKill.begin(), vecTopKill.end(), SortByHighestKillAs);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ASSASSIN;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopKill_As[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_As[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_As[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ASSASSIN;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopKill_As[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_As[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_As[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ASSASSIN;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopKill_As[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_As[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_As[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ASSASSIN;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopKill_As[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_As[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_As[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopKill.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_ASSASSIN;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopKill_As[NetMsgFB.wSize] = vecTopKill[j];

					PGLCHARAG pCharAG = GetCharID(vecTopKill[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopKill_As[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopKill[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopKill[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopKill[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopKill_As[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopKill_As[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopKill.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	{
	}

	// Top Rich
	{
		VEC_TOP_RICH vecTopRich = m_vecTopRich;

		if (!vecTopRich.empty())
		{
			sort(vecTopRich.begin(), vecTopRich.end(), SortByHighestRich);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RICH;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopRich_[NetMsgFB.wSize] = vecTopRich[j];

					PGLCHARAG pCharAG = GetCharID(vecTopRich[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopRich[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopRich[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopRich[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopRich_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopRich_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopRich.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopRich.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RICH;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopRich_[NetMsgFB.wSize] = vecTopRich[j];

					PGLCHARAG pCharAG = GetCharID(vecTopRich[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopRich[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopRich[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopRich[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopRich_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopRich_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopRich.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopRich.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RICH;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopRich_[NetMsgFB.wSize] = vecTopRich[j];

					PGLCHARAG pCharAG = GetCharID(vecTopRich[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopRich[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopRich[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopRich[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopRich_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopRich_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopRich.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopRich.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RICH;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopRich_[NetMsgFB.wSize] = vecTopRich[j];

					PGLCHARAG pCharAG = GetCharID(vecTopRich[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopRich[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopRich[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopRich[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopRich_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopRich_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopRich.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopRich.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RICH;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopRich_[NetMsgFB.wSize] = vecTopRich[j];

					PGLCHARAG pCharAG = GetCharID(vecTopRich[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopRich_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopRich[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopRich[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopRich[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopRich_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopRich_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopRich.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}
	/*Top Level*/
	{
		VEC_TOP_LEVEL vecTopLevel = m_vecTopLevel;

		if (!vecTopLevel.empty())
		{
			sort(vecTopLevel.begin(), vecTopLevel.end(), SortByHighestLevel);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_LEVEL;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopLevel_[NetMsgFB.wSize] = vecTopLevel[j];

					PGLCHARAG pCharAG = GetCharID(vecTopLevel[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopLevel[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopLevel[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopLevel[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopLevel_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopLevel_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopLevel.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopLevel.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_LEVEL;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopLevel_[NetMsgFB.wSize] = vecTopLevel[j];

					PGLCHARAG pCharAG = GetCharID(vecTopLevel[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopLevel[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopLevel[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopLevel[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopLevel_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopLevel_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopLevel.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopLevel.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_LEVEL;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopLevel_[NetMsgFB.wSize] = vecTopLevel[j];

					PGLCHARAG pCharAG = GetCharID(vecTopLevel[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopLevel[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopLevel[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopLevel[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopLevel_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopLevel_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopLevel.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopLevel.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_LEVEL;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopLevel_[NetMsgFB.wSize] = vecTopLevel[j];

					PGLCHARAG pCharAG = GetCharID(vecTopLevel[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopLevel[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopLevel[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopLevel[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopLevel_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopLevel_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopLevel.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopLevel.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_LEVEL;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopLevel_[NetMsgFB.wSize] = vecTopLevel[j];

					PGLCHARAG pCharAG = GetCharID(vecTopLevel[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopLevel_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopLevel[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopLevel[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopLevel[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopLevel_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopLevel_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopLevel.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}
	/*Top Resu, Mhundz */
	{
		VEC_TOP_RESU vecTopResu = m_vecTopResu;

		if (!vecTopResu.empty())
		{
			sort(vecTopResu.begin(), vecTopResu.end(), SortByHighestResu);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RESU;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopResu_[NetMsgFB.wSize] = vecTopResu[j];

					PGLCHARAG pCharAG = GetCharID(vecTopResu[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopResu[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopResu[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopResu[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopResu_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopResu_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopResu.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopResu.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RESU;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopResu_[NetMsgFB.wSize] = vecTopResu[j];

					PGLCHARAG pCharAG = GetCharID(vecTopResu[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopResu[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopResu[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopResu[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopResu_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopResu_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopResu.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
			if (vecTopResu.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RESU;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopResu_[NetMsgFB.wSize] = vecTopResu[j];

					PGLCHARAG pCharAG = GetCharID(vecTopResu[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopResu[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopResu[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopResu[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopResu_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopResu_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopResu.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
			if (vecTopResu.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RESU;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopResu_[NetMsgFB.wSize] = vecTopResu[j];

					PGLCHARAG pCharAG = GetCharID(vecTopResu[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopResu[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopResu[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopResu[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopResu_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopResu_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopResu.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
			if (vecTopResu.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_RESU;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopResu_[NetMsgFB.wSize] = vecTopResu[j];

					PGLCHARAG pCharAG = GetCharID(vecTopResu[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopResu_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopResu[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopResu[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopResu[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopResu_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopResu_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopResu.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}
	/*Top MMR, Mhundz */
	{
		VEC_TOP_MMR vecTopMMR = m_vecTopMMR;

		if (!vecTopMMR.empty())
		{
			sort(vecTopMMR.begin(), vecTopMMR.end(), SortByHighestMMR);

			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_MMR;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopMMR_[NetMsgFB.wSize] = vecTopMMR[j];

					PGLCHARAG pCharAG = GetCharID(vecTopMMR[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopMMR[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopMMR[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopMMR[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopMMR_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopMMR_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopMMR.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}

			if (vecTopMMR.size() > 10)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_MMR;
				NetMsgFB.wPart = 1;

				for (size_t j = 10; j < 20; j++)
				{
					NetMsgFB.sTopMMR_[NetMsgFB.wSize] = vecTopMMR[j];

					PGLCHARAG pCharAG = GetCharID(vecTopMMR[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopMMR[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopMMR[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopMMR[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopMMR_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopMMR_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopMMR.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
			if (vecTopMMR.size() > 20)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_MMR;
				NetMsgFB.wPart = 2;

				for (size_t j = 20; j < 30; j++)
				{
					NetMsgFB.sTopMMR_[NetMsgFB.wSize] = vecTopMMR[j];

					PGLCHARAG pCharAG = GetCharID(vecTopMMR[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopMMR[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopMMR[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopMMR[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopMMR_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopMMR_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopMMR.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
			if (vecTopMMR.size() > 30)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_MMR;
				NetMsgFB.wPart = 3;

				for (size_t j = 30; j < 40; j++)
				{
					NetMsgFB.sTopMMR_[NetMsgFB.wSize] = vecTopMMR[j];

					PGLCHARAG pCharAG = GetCharID(vecTopMMR[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopMMR[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopMMR[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopMMR[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopMMR_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopMMR_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopMMR.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
			if (vecTopMMR.size() > 40)
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_MMR;
				NetMsgFB.wPart = 4;

				for (size_t j = 40; j < 50; j++)
				{
					NetMsgFB.sTopMMR_[NetMsgFB.wSize] = vecTopMMR[j];

					PGLCHARAG pCharAG = GetCharID(vecTopMMR[j].dwChaNum);
					if (!pCharAG)
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = FALSE;
					else
						NetMsgFB.sTopMMR_[NetMsgFB.wSize].bChaOnline = TRUE;

					BOOL bGuild = vecTopMMR[j].wGuNum ? TRUE : FALSE;
					if (bGuild && vecTopMMR[j].wGuNum > 0)
					{
						GLClubMan &cClubMan = GetClubMan();
						GLCLUB *pCLUB = cClubMan.GetClub(vecTopMMR[j].wGuNum);
						if (pCLUB)
						{
							NetMsgFB.sTopMMR_[NetMsgFB.wSize].wGuMarkVer = (WORD)pCLUB->m_dwMarkVER;
							StringCchCopy(NetMsgFB.sTopMMR_[NetMsgFB.wSize].szGuName, CHAR_SZNAME, pCLUB->m_szName);
						}
					}

					NetMsgFB.wSize++;

					if (j == vecTopMMR.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}
	}

	{
	}

	// Top GUild
	// Top Guild
	{
		VEC_TOP_GUILD vecTopGuild;

		GLClubMan &cClubMan = GetClubMan();

		for (size_t i = 0; i < cClubMan.m_mapCLUB.size(); i++)
		{
			GLCLUB *pCLUB = cClubMan.GetClub(cClubMan.m_mapCLUB[i].m_dwID);
			if (pCLUB && pCLUB->m_dwID > 0)
			{
				STOP_RANK_GUILD sGuild;
				sGuild.wGuNum = static_cast<WORD>(pCLUB->m_dwID);
				StringCchCopy(sGuild.szGuName, CHAR_SZNAME, pCLUB->m_szName);

				sGuild.wRank = static_cast<WORD>(pCLUB->m_dwRank);
				sGuild.wGuMarkVer = static_cast<WORD>(pCLUB->m_dwMarkVER);

				CLUBMEMBERS_ITER pos = pCLUB->m_mapMembers.begin();
				CLUBMEMBERS_ITER end = pCLUB->m_mapMembers.end();
				PGLCHARAG pMemberOn = NULL;
				for (; pos != end; ++pos)
				{
					pMemberOn = GetCharID((*pos).first);
					if (pMemberOn)
						sGuild.wOnline++;
				}

				vecTopGuild.push_back(sGuild);
			}
		}
		if (!vecTopGuild.empty())
		{
			sort(vecTopGuild.begin(), vecTopGuild.end(), SortByOnline);
			{
				GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT NetMsgFB;
				NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUILD;
				NetMsgFB.wPart = 0;

				for (size_t j = 0; j < 10; j++)
				{
					NetMsgFB.sTopGuild_[NetMsgFB.wSize] = vecTopGuild[j];
					NetMsgFB.wSize++;

					if (j == vecTopGuild.size() - 1)
						break;
				}

				if (NetMsgFB.wSize > 0)
					SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
			}
		}

		if (vecTopGuild.size() > 10)
		{
			GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT NetMsgFB;
			NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUILD;
			NetMsgFB.wPart = 1;

			for (size_t j = 10; j < 20; j++)
			{
				NetMsgFB.sTopGuild_[NetMsgFB.wSize] = vecTopGuild[j];
				NetMsgFB.wSize++;

				if (j == vecTopGuild.size() - 1)
					break;
			}

			if (NetMsgFB.wSize > 0)
				SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
		}

		if (vecTopGuild.size() > 20)
		{
			GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT NetMsgFB;
			NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUILD;
			NetMsgFB.wPart = 2;

			for (size_t j = 20; j < 30; j++)
			{
				NetMsgFB.sTopGuild_[NetMsgFB.wSize] = vecTopGuild[j];
				NetMsgFB.wSize++;

				if (j == vecTopGuild.size() - 1)
					break;
			}

			if (NetMsgFB.wSize > 0)
				SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
		}

		if (vecTopGuild.size() > 30)
		{
			GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT NetMsgFB;
			NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUILD;
			NetMsgFB.wPart = 3;

			for (size_t j = 30; j < 40; j++)
			{
				NetMsgFB.sTopGuild_[NetMsgFB.wSize] = vecTopGuild[j];
				NetMsgFB.wSize++;

				if (j == vecTopGuild.size() - 1)
					break;
			}

			if (NetMsgFB.wSize > 0)
				SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
		}

		if (vecTopGuild.size() > 40)
		{
			GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT NetMsgFB;
			NetMsgFB.emFB = EM_GLOBAL_RANKING_FB_TOP_GUILD;
			NetMsgFB.wPart = 4;

			for (size_t j = 40; j < 50; j++)
			{
				NetMsgFB.sTopGuild_[NetMsgFB.wSize] = vecTopGuild[j];
				NetMsgFB.wSize++;

				if (j == vecTopGuild.size() - 1)
					break;
			}

			if (NetMsgFB.wSize > 0)
				SENDTOCLIENT(pCharAG->m_dwClientID, &NetMsgFB);
		}
	}

	return TRUE;
}

BOOL GLAgentServer::MsgReqGlobalRankingKillUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER iter = m_vecTopKill.begin();
			VEC_TOP_KILL_ITER iter_end = m_vecTopKill.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKill.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKill.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKill.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKill.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKill.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKill.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKill.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKill.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKill.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKill.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKill.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKill.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKill.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKill.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKill.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKill.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKill.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKill.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKill.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKill.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKill.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKill.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKill.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKill.dwChaPKLoss;

				m_vecTopKill.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER iter = m_vecTopKill.begin();
			VEC_TOP_KILL_ITER iter_end = m_vecTopKill.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKill.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKill.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKill.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKill.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKill.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKill.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKill.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKill.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKill.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKill.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKill.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKill.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKill.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKill.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateBr(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_BR_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_BR iter = m_vecTopKillBr.begin();
			VEC_TOP_KILL_ITER_BR iter_end = m_vecTopKillBr.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillBr.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillBr.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillBr.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillBr.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillBr.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillBr.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillBr.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillBr.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillBr.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillBr.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillBr.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillBr.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillBr.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillBr.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_BR sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillBr.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillBr.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillBr.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillBr.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillBr.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillBr.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillBr.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillBr.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillBr.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillBr.dwChaPKLoss;

				m_vecTopKillBr.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_BR iter = m_vecTopKillBr.begin();
			VEC_TOP_KILL_ITER_BR iter_end = m_vecTopKillBr.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillBr.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillBr.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillBr.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillBr.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillBr.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillBr.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillBr.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillBr.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillBr.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillBr.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillBr.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillBr.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillBr.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillBr.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateSw(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_SW_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_SW iter = m_vecTopKillSw.begin();
			VEC_TOP_KILL_ITER_SW iter_end = m_vecTopKillSw.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillSw.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillSw.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillSw.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillSw.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillSw.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillSw.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillSw.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillSw.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillSw.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillSw.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillSw.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillSw.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillSw.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillSw.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_SW sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillSw.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillSw.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillSw.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillSw.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillSw.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillSw.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillSw.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillSw.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillSw.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillSw.dwChaPKLoss;

				m_vecTopKillSw.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_SW iter = m_vecTopKillSw.begin();
			VEC_TOP_KILL_ITER_SW iter_end = m_vecTopKillSw.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillSw.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillSw.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillSw.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillSw.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillSw.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillSw.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillSw.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillSw.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillSw.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillSw.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillSw.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillSw.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillSw.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillSw.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateAr(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_AR_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_AR iter = m_vecTopKillAr.begin();
			VEC_TOP_KILL_ITER_AR iter_end = m_vecTopKillAr.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillAr.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillAr.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillAr.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillAr.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillAr.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillAr.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillAr.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillAr.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillAr.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillAr.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillAr.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillAr.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillAr.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillAr.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_AR sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillAr.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillAr.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillAr.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillAr.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillAr.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillAr.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillAr.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillAr.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillAr.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillAr.dwChaPKLoss;

				m_vecTopKillAr.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_AR iter = m_vecTopKillAr.begin();
			VEC_TOP_KILL_ITER_AR iter_end = m_vecTopKillAr.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillAr.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillAr.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillAr.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillAr.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillAr.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillAr.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillAr.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillAr.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillAr.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillAr.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillAr.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillAr.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillAr.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillAr.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateSh(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_SH_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_SH iter = m_vecTopKillSh.begin();
			VEC_TOP_KILL_ITER_SH iter_end = m_vecTopKillSh.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillSh.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillSh.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillSh.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillSh.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillSh.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillSh.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillSh.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillSh.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillSh.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillSh.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillSh.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillSh.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillSh.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillSh.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_SH sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillSh.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillSh.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillSh.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillSh.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillSh.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillSh.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillSh.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillSh.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillSh.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillSh.dwChaPKLoss;

				m_vecTopKillSh.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_SH iter = m_vecTopKillSh.begin();
			VEC_TOP_KILL_ITER_SH iter_end = m_vecTopKillSh.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillSh.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillSh.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillSh.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillSh.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillSh.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillSh.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillSh.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillSh.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillSh.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillSh.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillSh.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillSh.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillSh.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillSh.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateEx(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_EX_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_EX iter = m_vecTopKillEx.begin();
			VEC_TOP_KILL_ITER_EX iter_end = m_vecTopKillEx.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillEx.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillEx.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillEx.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillEx.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillEx.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillEx.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillEx.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillEx.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillEx.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillEx.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillEx.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillEx.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillEx.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillEx.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_EX sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillEx.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillEx.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillEx.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillEx.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillEx.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillEx.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillEx.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillEx.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillEx.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillEx.dwChaPKLoss;

				m_vecTopKillEx.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_EX iter = m_vecTopKillEx.begin();
			VEC_TOP_KILL_ITER_EX iter_end = m_vecTopKillEx.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillEx.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillEx.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillEx.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillEx.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillEx.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillEx.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillEx.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillEx.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillEx.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillEx.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillEx.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillEx.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillEx.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillEx.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateSc(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_SC_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_SC iter = m_vecTopKillSc.begin();
			VEC_TOP_KILL_ITER_SC iter_end = m_vecTopKillSc.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillSc.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillSc.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillSc.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillSc.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillSc.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillSc.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillSc.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillSc.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillSc.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillSc.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillSc.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillSc.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillSc.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillSc.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_SC sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillSc.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillSc.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillSc.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillSc.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillSc.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillSc.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillSc.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillSc.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillSc.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillSc.dwChaPKLoss;

				m_vecTopKillSc.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_SC iter = m_vecTopKillSc.begin();
			VEC_TOP_KILL_ITER_SC iter_end = m_vecTopKillSc.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillSc.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillSc.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillSc.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillSc.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillSc.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillSc.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillSc.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillSc.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillSc.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillSc.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillSc.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillSc.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillSc.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillSc.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingKillUpdateAs(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_AS_UPDATE *)nmg;

	if (pNetMsg->bWIN)
	{
		// Top Kill
		{
			BOOL bFound = FALSE;
			VEC_TOP_KILL_ITER_AS iter = m_vecTopKillAs.begin();
			VEC_TOP_KILL_ITER_AS iter_end = m_vecTopKillAs.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillAs.dwChaNum)
				{
					bFound = TRUE;

					if (strcmp((*iter).szChaName, pNetMsg->sTopKillAs.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillAs.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillAs.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillAs.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillAs.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillAs.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillAs.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillAs.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillAs.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillAs.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillAs.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillAs.szGuName);

					(*iter).dwChaPKWin = pNetMsg->sTopKillAs.dwChaPKWin;

					break;
				}
			}

			if (!bFound)
			{
				STOP_RANK_KILL_AS sTEMP;
				sTEMP.dwChaNum = pNetMsg->sTopKillAs.dwChaNum;
				StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopKillAs.szChaName);
				sTEMP.wChaLevel = pNetMsg->sTopKillAs.wChaLevel;
				sTEMP.nChaClass = pNetMsg->sTopKillAs.nChaClass;
				sTEMP.wChaSchool = pNetMsg->sTopKillAs.wChaSchool;
				sTEMP.wGuNum = pNetMsg->sTopKillAs.wGuNum;
				sTEMP.wGuMarkVer = pNetMsg->sTopKillAs.wGuMarkVer;
				StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopKillAs.szGuName);
				sTEMP.dwChaPKWin = pNetMsg->sTopKillAs.dwChaPKWin;
				sTEMP.dwChaPKLoss = pNetMsg->sTopKillAs.dwChaPKLoss;

				m_vecTopKillAs.push_back(sTEMP);
			}
		}
	}
	else
	{
		// Top Kill
		{
			VEC_TOP_KILL_ITER_AS iter = m_vecTopKillAs.begin();
			VEC_TOP_KILL_ITER_AS iter_end = m_vecTopKillAs.end();

			for (; iter != iter_end; iter++)
			{
				if ((*iter).dwChaNum == pNetMsg->sTopKillAs.dwChaNum)
				{
					if (strcmp((*iter).szChaName, pNetMsg->sTopKillAs.szChaName) != 0)
						StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopKillAs.szChaName);

					if ((*iter).wChaLevel != pNetMsg->sTopKillAs.wChaLevel)
						(*iter).wChaLevel = pNetMsg->sTopKillAs.wChaLevel;

					if ((*iter).wChaSchool != pNetMsg->sTopKillAs.wChaSchool)
						(*iter).wChaSchool = pNetMsg->sTopKillAs.wChaSchool;

					if ((*iter).wGuNum != pNetMsg->sTopKillAs.wGuNum)
						(*iter).wGuNum = pNetMsg->sTopKillAs.wGuNum;

					if ((*iter).wGuMarkVer != pNetMsg->sTopKillAs.wGuMarkVer)
						(*iter).wGuMarkVer = pNetMsg->sTopKillAs.wGuMarkVer;

					if (strcmp((*iter).szGuName, pNetMsg->sTopKillAs.szGuName) != 0)
						StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopKillAs.szGuName);

					(*iter).dwChaPKLoss = pNetMsg->sTopKillAs.dwChaPKLoss;

					break;
				}
			}
		}
	}

	return TRUE;
}
BOOL GLAgentServer::MsgReqGlobalRankingRichUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_UPDATE *)nmg;

	BOOL bFound = FALSE;
	VEC_TOP_RICH_ITER iter = m_vecTopRich.begin();
	VEC_TOP_RICH_ITER iter_end = m_vecTopRich.end();

	for (; iter != iter_end; iter++)
	{
		if ((*iter).dwChaNum == pNetMsg->sTopRich.dwChaNum)
		{
			bFound = TRUE;

			if (strcmp((*iter).szChaName, pNetMsg->sTopRich.szChaName) != 0)
				StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopRich.szChaName);

			if ((*iter).wChaLevel != pNetMsg->sTopRich.wChaLevel)
				(*iter).wChaLevel = pNetMsg->sTopRich.wChaLevel;

			if ((*iter).wChaSchool != pNetMsg->sTopRich.wChaSchool)
				(*iter).wChaSchool = pNetMsg->sTopRich.wChaSchool;

			if ((*iter).wGuNum != pNetMsg->sTopRich.wGuNum)
				(*iter).wGuNum = pNetMsg->sTopRich.wGuNum;

			if ((*iter).wGuMarkVer != pNetMsg->sTopRich.wGuMarkVer)
				(*iter).wGuMarkVer = pNetMsg->sTopRich.wGuMarkVer;

			if (strcmp((*iter).szGuName, pNetMsg->sTopRich.szGuName) != 0)
				StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopRich.szGuName);

			(*iter).llnMoney = pNetMsg->sTopRich.llnMoney;

			break;
		}
	}

	if (!bFound)
	{
		STOP_RANK_RICH sTEMP;
		sTEMP.dwChaNum = pNetMsg->sTopRich.dwChaNum;
		StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopRich.szChaName);
		sTEMP.wChaLevel = pNetMsg->sTopRich.wChaLevel;
		sTEMP.nChaClass = pNetMsg->sTopRich.nChaClass;
		sTEMP.wChaSchool = pNetMsg->sTopRich.wChaSchool;
		sTEMP.wGuNum = pNetMsg->sTopRich.wGuNum;
		sTEMP.wGuMarkVer = pNetMsg->sTopRich.wGuMarkVer;
		StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopRich.szGuName);
		sTEMP.llnMoney = pNetMsg->sTopRich.llnMoney;
		m_vecTopRich.push_back(sTEMP);
	}

	return TRUE;
}
/*Top Level*/
BOOL GLAgentServer::MsgReqGlobalRankingLevelUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_LEVEL_UPDATE *)nmg;

	BOOL bFound = FALSE;
	VEC_TOP_LEVEL_ITER iter = m_vecTopLevel.begin();
	VEC_TOP_LEVEL_ITER iter_end = m_vecTopLevel.end();

	for (; iter != iter_end; iter++)
	{
		if ((*iter).dwChaNum == pNetMsg->sTopLevel.dwChaNum)
		{
			bFound = TRUE;

			if (strcmp((*iter).szChaName, pNetMsg->sTopLevel.szChaName) != 0)
				StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopLevel.szChaName);

			if ((*iter).wChaLevel != pNetMsg->sTopLevel.wChaLevel)
				(*iter).wChaLevel = pNetMsg->sTopLevel.wChaLevel;

			if ((*iter).wChaSchool != pNetMsg->sTopLevel.wChaSchool)
				(*iter).wChaSchool = pNetMsg->sTopLevel.wChaSchool;

			if ((*iter).wGuNum != pNetMsg->sTopLevel.wGuNum)
				(*iter).wGuNum = pNetMsg->sTopLevel.wGuNum;

			if ((*iter).wGuMarkVer != pNetMsg->sTopLevel.wGuMarkVer)
				(*iter).wGuMarkVer = pNetMsg->sTopLevel.wGuMarkVer;

			if (strcmp((*iter).szGuName, pNetMsg->sTopLevel.szGuName) != 0)
				StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopLevel.szGuName);

			(*iter).llnExp = pNetMsg->sTopLevel.llnExp;

			break;
		}
	}

	if (!bFound)
	{
		STOP_RANK_LEVEL sTEMP;
		sTEMP.dwChaNum = pNetMsg->sTopLevel.dwChaNum;
		StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopLevel.szChaName);
		sTEMP.wChaLevel = pNetMsg->sTopLevel.wChaLevel;
		sTEMP.nChaClass = pNetMsg->sTopLevel.nChaClass;
		sTEMP.wChaSchool = pNetMsg->sTopLevel.wChaSchool;
		sTEMP.wGuNum = pNetMsg->sTopLevel.wGuNum;
		sTEMP.wGuMarkVer = pNetMsg->sTopLevel.wGuMarkVer;
		StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopLevel.szGuName);
		sTEMP.llnExp = pNetMsg->sTopLevel.llnExp;
		m_vecTopLevel.push_back(sTEMP);
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*Top Resu, Mhundz */
BOOL GLAgentServer::MsgReqGlobalRankingResuUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_RESU_UPDATE *)nmg;

	BOOL bFound = FALSE;
	VEC_TOP_RESU_ITER iter = m_vecTopResu.begin();
	VEC_TOP_RESU_ITER iter_end = m_vecTopResu.end();

	for (; iter != iter_end; iter++)
	{
		if ((*iter).dwChaNum == pNetMsg->sTopResu.dwChaNum)
		{
			bFound = TRUE;

			if (strcmp((*iter).szChaName, pNetMsg->sTopResu.szChaName) != 0)
				StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopResu.szChaName);

			if ((*iter).wChaLevel != pNetMsg->sTopResu.wChaLevel)
				(*iter).wChaLevel = pNetMsg->sTopResu.wChaLevel;

			if ((*iter).wChaSchool != pNetMsg->sTopResu.wChaSchool)
				(*iter).wChaSchool = pNetMsg->sTopResu.wChaSchool;

			if ((*iter).wGuNum != pNetMsg->sTopResu.wGuNum)
				(*iter).wGuNum = pNetMsg->sTopResu.wGuNum;

			if ((*iter).wGuMarkVer != pNetMsg->sTopResu.wGuMarkVer)
				(*iter).wGuMarkVer = pNetMsg->sTopResu.wGuMarkVer;

			if (strcmp((*iter).szGuName, pNetMsg->sTopResu.szGuName) != 0)
				StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopResu.szGuName);

			(*iter).wResu = pNetMsg->sTopResu.wResu;

			break;
		}
	}

	if (!bFound)
	{
		STOP_RANK_RESU sTEMP;
		sTEMP.dwChaNum = pNetMsg->sTopResu.dwChaNum;
		StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopResu.szChaName);
		sTEMP.wChaLevel = pNetMsg->sTopResu.wChaLevel;
		sTEMP.nChaClass = pNetMsg->sTopResu.nChaClass;
		sTEMP.wChaSchool = pNetMsg->sTopResu.wChaSchool;
		sTEMP.wGuNum = pNetMsg->sTopResu.wGuNum;
		sTEMP.wGuMarkVer = pNetMsg->sTopResu.wGuMarkVer;
		StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopResu.szGuName);
		sTEMP.wResu = pNetMsg->sTopResu.wResu;
		m_vecTopResu.push_back(sTEMP);
	}

	return TRUE;
}
/*Top MMR, Mhundz */
BOOL GLAgentServer::MsgReqGlobalRankingMMRUpdate(NET_MSG_GENERIC *nmg)
{
	GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_UPDATE *pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_MMR_UPDATE *)nmg;

	BOOL bFound = FALSE;
	VEC_TOP_MMR_ITER iter = m_vecTopMMR.begin();
	VEC_TOP_MMR_ITER iter_end = m_vecTopMMR.end();

	for (; iter != iter_end; iter++)
	{
		if ((*iter).dwChaNum == pNetMsg->sTopMMR.dwChaNum)
		{
			bFound = TRUE;

			if (strcmp((*iter).szChaName, pNetMsg->sTopMMR.szChaName) != 0)
				StringCchCopy((*iter).szChaName, CHAR_SZNAME, pNetMsg->sTopMMR.szChaName);

			if ((*iter).wChaLevel != pNetMsg->sTopMMR.wChaLevel)
				(*iter).wChaLevel = pNetMsg->sTopMMR.wChaLevel;

			if ((*iter).wChaSchool != pNetMsg->sTopMMR.wChaSchool)
				(*iter).wChaSchool = pNetMsg->sTopMMR.wChaSchool;

			if ((*iter).wGuNum != pNetMsg->sTopMMR.wGuNum)
				(*iter).wGuNum = pNetMsg->sTopMMR.wGuNum;

			if ((*iter).wGuMarkVer != pNetMsg->sTopMMR.wGuMarkVer)
				(*iter).wGuMarkVer = pNetMsg->sTopMMR.wGuMarkVer;

			if (strcmp((*iter).szGuName, pNetMsg->sTopMMR.szGuName) != 0)
				StringCchCopy((*iter).szGuName, CHAR_SZNAME, pNetMsg->sTopMMR.szGuName);

			(*iter).wMMR = pNetMsg->sTopMMR.wMMR;

			break;
		}
	}

	if (!bFound)
	{
		STOP_RANK_MMR sTEMP;
		sTEMP.dwChaNum = pNetMsg->sTopMMR.dwChaNum;
		StringCchCopy(sTEMP.szChaName, CHAR_SZNAME, pNetMsg->sTopMMR.szChaName);
		sTEMP.wChaLevel = pNetMsg->sTopMMR.wChaLevel;
		sTEMP.nChaClass = pNetMsg->sTopMMR.nChaClass;
		sTEMP.wChaSchool = pNetMsg->sTopMMR.wChaSchool;
		sTEMP.wGuNum = pNetMsg->sTopMMR.wGuNum;
		sTEMP.wGuMarkVer = pNetMsg->sTopMMR.wGuMarkVer;
		StringCchCopy(sTEMP.szGuName, CHAR_SZNAME, pNetMsg->sTopMMR.szGuName);
		sTEMP.wMMR = pNetMsg->sTopMMR.wMMR;
		m_vecTopMMR.push_back(sTEMP);
	}

	return TRUE;
}
/*ecall function Arc Development 08-16-2024*/
BOOL GLAgentServer::MsgCallGuildMem(NET_MSG_GENERIC *nmg, DWORD dwClientID, DWORD dwGaeaID)
{
	PGLCHARAG pMyChar = GetChar(dwGaeaID);
	if (!pMyChar)
		return FALSE;

	GLCLUB *pCLUB = GetClubMan().GetClub(pMyChar->m_dwGuild);
	if (!pCLUB->m_dwMasterID)
		return FALSE;

	GLMSG::SNET_CALL_GUILD_MEM *pNetMsg = (GLMSG::SNET_CALL_GUILD_MEM *)nmg;

	PGLCHARAG pGenCHAR = GetChar(pNetMsg->szName);

	GLMSG::SNETPC_GM_MOVE2CHAR_POS netmsg;
	netmsg.dwCOMMAND_CHARID = pGenCHAR->m_dwCharID;
	netmsg.dwTO_CHARID = pMyChar->m_dwCharID;
	SENDTOFIELDSVR(pMyChar->m_nChannel, pMyChar->m_dwCurFieldSvr, &netmsg);

	return TRUE;
}