#include "StdAfx.h"
#include "./InnerInterface.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "./BasicButton.h"
#include "./BasicTextBox.h"
#include "../../Lib_Engine/DxSound/DxSoundLib.h"

#include "./VoidMainNews.h"



// pandora 
#include "./PandoraBox/PandoraBoxWindow.h"
#include "./PandoraBox/PandoraBoxButton.h"
#include "./AdminMessageDisplay.h"
#include "./AcademyConftDisplay.h"
#include "./BasicChatRightBody.h"
#include "./BasicVarTextBox.h"
#include "./BasicChat.h"
#include "./BasicQuickSkillSlot.h"
#include "./BasicGameMenu.h"
#include "./BasicHWOptionWindow.h"
#include "./BasicEscMenu.h"
#include "./BasicInfoView.h"
#include "./BonusTimeGauge.h"
#include "./BonusTimeDisplay.h"
#include "./BusWindow.h"
#include "./BlockProgramAlarm.h"
#include "./ClubWindow.h"
#include "./ClubMake.h"
#include "./ConftDisplayMan.h"
#include "./ConftConfirm.h"
#include "./ConftModalWindow.h"
#include "../../Lib_Engine/Common/CommonWeb.h"
#include "./CharacterWindow.h"
#include "./ChatMacroWindow.h"
#include "./CountMsg.h"
#include "./DamageDisplayMan.h"
#include "./DialogueWindow.h"
#include "../../Lib_Engine/DxCommon/DxViewPort.h"
#include "../../Lib_Client/DxGlobalStage.h"
#include "../../Lib_Client/DxParamSet.h"
#include "./EscMenuOpen.h"
#include "./QBoxButton.h"
#include "./FriendWindow.h"
#include "./GameTextControl.h"
#include "./GambleBox.h"
#include "./GambleSelectBox.h"
#include "./GambleAgainBox.h"
#include "./GambleResultOddBox.h"
#include "./GambleResultEvenBox.h"
#include "./GambleAnimationBox.h"
#include "./PetskinMixImage.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPartyClient.h"
#include "../../Lib_Client/G-Logic/GLQuest.h"
#include "./HeadChatDisplayMan.h"
#include "./HairColorCard.h"
#include "./HairStyleCard.h"
#include "./HelpWindow.h"
#include "./HelpWindowWeb.h"
#include "./InventoryWindow.h"
#include "./VNGainSysInventory.h"
#include "./VNGainSysGauge.h"
#include "./ItemBankWindow.h"
#include "./ItemGarbage.h"
#include "./ItemMove.h"
#include "./ItemShopIconMan.h"
#include "./ItemShopWindowWeb.h"
#include "./ItemRebuild.h"	// ITEMREBUILD_MARK
#include "./KeySettingWindow.h"
#include "./LargeMapWindow.h"
#include "./MarketWindow.h"
#include "./MiniMap.h"
#include "./ModalWindow.h"
#include "./ModalCallerID.h"
#include "./MiniPartyWindow.h"
#include "./MapMoveDisplay.h"
#include "./NameDisplayMan.h"
#include "./PrivateMarketWindow.h"
#include "./PrivateMarketSellWindow.h"
#include "./PrivateMarketMake.h"
#include "./PrivateMarketShowMan.h"
#include "./PtoPWindow.h"
#include "./PartyWindow.h"
#include "./PartyModalWindow.h"
#include "./PetRebirthDialogue.h"//Monster7j
#include "./PetColorCard.h"
#include "./PetStyleCard.h"
#include "./PetWindow.h"
#include "./PetSkillWindow.h"
#include "./QuestionItemDisplay.h"
#include "./QuestWindow.h"
#include "./QuestAlarm.h"
#include "./QuestHelper.h"
#include "./RebuildInventoryWindow.h"
#include "./RebirthDialogue.h"
#include "./ReceiveNoteWindow.h"
#include "./SimpleHP.h"
#include "./SimpleMessageMan.h"
#include "./SystemMessageWindow.h"
#include "./StorageWindow.h"
#include "./StorageChargeCard.h"
#include "./SkillTimeDisplay.h"

#include "./SkillWindow.h"
#include "./ModernSkillWindow.h"
#include "./SkillWindowToTray.h"
#include "./SubMasterSet.h"
#include "./SMSSendWindow.h"
#include "./TradeWindow.h"
#include "./TradeInventoryWindow.h"
#include "./TargetInfoDisplay.h"
#include "./UIInfoLoader.h"
#include "./UILeftTopGroup.h"
#include "./WaitServerDialogue.h"
#include "./WeaponDisplay.h"
#include "./WriteNoteWindow.h"
#include "./WarningMsg.h"
#include "./VehicleWindow.h"
#include "./GenderChangeWindow.h"
#include "./ThaiCCafeMark.h"
#include "./GarbageInventoryWindow.h"
#include "./ShopItemSearchWindow.h"
#include "./ItemSearchResultWindow.h"
#include "./AttenDanceBookWindow.h"
#include "./FaceStyleCard.h"
#include "./MapRequireCheck.h"
#include "./ClubBattleModalWindow.h"
#include "./TaxiWindow.h"
#include "./ItemMixWindow.h"
#include "./ItemMixInventoryWindow.h"
#include "./GatherGauge.h"
#include "./CdmRankingDisplay.h"

/*game stats, Juver, 2017/06/22 */
#include "./GameStats.h" 

/*itemfood system, Juver, 2017/05/25 */
#include "./FItemFactTimeDisplay.h" 

/*npc shop, Juver, 2017/07/25 */
#include "./NPCShopWindow.h" 

/*item preview, Juver, 2017/07/27 */
#include "./ItemPreviewWindow.h" 

/*pet status, Juver, 2017/07/30 */
#include "./PetStatus.h" 

 /*vehicle status, Juver, 2017/07/30 */
#include "./VehicleStatus.h"

/*item link, Juver, 2017/07/31 */
#include "./BasicVarTextBoxItemLink.h" 

/*vehicle booster system, Juver, 2017/08/11 */
#include "./VehicleBoosterDisplay.h" 

/*box contents, Juver, 2017/08/30 */
#include "./InfoBoxContents.h"

/*system buffs, Juver, 2017/09/05 */
#include "./SystemBuffTimeDisplay.h"

/* Competition UI, Juver, 2017/08/24 */
#include "./PVPCompetitionButton.h"
#include "./PVPCompetitionNotify.h"
#include "./PVPCompetitionWindow.h"
#include "./PVPCompetitionNotice.h"
#include "PVPNextBattleAndWinner.h"	// PVP Next Battle UI - Jxyyy
/* PK Streak Sigaw - Jxyyy */
#include "./PKStreakShutdownNotice.h" 
#include "./PKStreakKillNotice.h" 
/*pvp tyranny, Juver, 2017/08/25 */
#include "./PVPTyrannyTowerCapture.h"
#include "./PVPTyrannyTowerProgress.h"
#include "./PVPTyrannyRebirthDialogue.h"
#include "./PVPTyrannyCaptureNotice.h"
#include "./PVPTyranny2CaptureNotice.h"
#include "./PVPTyrannyRankings.h"
#include "./PVPTyrannyTopRankings.h"
#include "./PVPTyrannyBattleNotice.h"

/*woe Arc Development 08-06-2024*/
#include "./PVPWoeTowerCapture.h"
#include "./PVPWoeTowerProgress.h"
#include "./PVPWoeRebirthDialogue.h"
#include "../Lib_ClientUI/Interface/WoeRanking/WoeRanking.h"
/*item exchange, Juver, 2017/10/12 */
#include "./NPCItemExchangeWindow.h"

/*product item, Juver, 2017/10/15 */
#include "./ProductButton.h"
#include "./ProductWindow.h"

/*student record ui, Juver, 2017/10/30 */
#include "./StudentRecordButton.h"
#include "./StudentRecordWindow.h"
/* Codex, Archie 02/16/24 */
//#include "CodexWindow/StudentCodexButton.h" 
#include "CodexWindow/StudentCodexWindow.h"

/*activity system, Juver, 2017/11/05 */
#include "./ActivityItemWindow.h"

/*quest ui, Juver, 2017/11/06 */
#include "./ModernQuestWindow.h"

/*charinfoview , Juver, 2017/11/11 */
#include "./CharacterInfoViewWindow.h"

/*bike color , Juver, 2017/11/12 */
#include "./BikeColorWindow.h"

/*pet skill info display, Juver, 2017/12/01 */
/*dual pet skill, Juver, 2017/12/29 */
#include "./PetSkillInfoDisplay.h"
#include "./PetSkillInfoDisplay2.h"

/*change scale card, Juver, 2018/01/03 */
#include "./ChangeScaleWindow.h"

/*item color, Juver, 2018/01/10 */
#include "./ItemColorWindow.h"

/*change school card, Juver, 2018/01/12 */
#include "./ChangeSchoolWindow.h"

/*item transfer card, Juver, 2018/01/18 */
#include "./ItemTransferInventoryWindow.h"
#include "./ItemTransferWindow.h"

/*school wars, Juver, 2018/01/19 */
#include "./PVPSchoolWarsScore.h"
#include "./PVPSchoolWarsRebirthDialogue.h"
#include "./PVPSchoolWarsBattleNotice.h"
#include "./PVPSchoolWarsTopRankings.h"
#include "./PVPSchoolWarsRankings.h"

/*pvp capture the flag, Juver, 2018/02/01 */
#include "./PVPCaptureTheFlagRebirthDialogue.h"
#include "./PVPCaptureTheFlagTopRankings.h"
#include "./PVPCaptureTheFlagBattleNotice.h"
#include "./PVPCaptureTheFlagScore.h"
#include "./PVPCaptureTheFlagRankings.h"
#include "./PVPCaptureTheFlagHoldIcon.h"

/* car, cart color, Juver, 2018/02/14 */
#include "./CarColorWindow.h"

/*info window, Juver, 2018/03/12 */
#include "./InfoWindow.h"

/*modern character window, Juver, 2018/03/12 */
#include "./ModernCharacterWindow.h"

/*modern inventory window, Juver, 2018/04/06 */
#include "./ModernInventoryWindow.h"
#include "./ModernGarbageInventoryWindow.h"
#include "./ModernItemMixInventoryWindow.h"
#include "./ModernRebuildInventoryWindow.h"
#include "./ModernTradeInventoryWindow.h"
#include "./ModernItemTransferInventoryWindow.h"

/*item random option rebuild, Juver, 2018/07/03 */
#include "./ItemRandomOptionRebuildInventoryWindow.h"
#include "./ModernItemRandomOptionRebuildInventoryWindow.h"
#include "./ItemRandomOptionRebuildWindow.h"

/*pvp reward info, Juver, 2018/07/25 */
#include "./PVPTyrannyRewardInfo.h"
#include "./PVPSchoolWarsRewardInfo.h"
#include "./PVPCaptureTheFlagRewardInfo.h"
/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
#include "./GlobalRankingWindow/GlobalRankingWindow.h"
/////////////////////////////////////////////////////////////////////////////
/*modern skill tray, Juver, 2018/08/21 */
#include "./SkillTrayTab.h"
#include "./SkillTrayTabMini.h"
#include "./SkillTrayTabEx.h"
#include "./SkillTrayTabExMini.h"

/*specific item box, Juver, 2018/09/02 */
#include "./ItemBoxSpecificSelectionWindow.h"

/*extended inventory window, EJCode, 2018/10/16 */
#include "./EP7InventoryWindow.h"
#include "./EP7GarbageInventoryWindow.h"
#include "./EP7ItemMixInventoryWindow.h"
#include "./EP7RebuildInventoryWindow.h"
#include "./EP7TradeInventoryWindow.h"
#include "./EP7ItemTransferInventoryWindow.h"
#include "./EP7ItemRandomOptionRebuildInventoryWindow.h"

/*pk combo, EJCode, 2018/11/15 */
#include "./PKComboDisplay.h"

#include "./PVPClubWarRankingWindow.h"

#include "./CurrencyShopWindow.h"

/* personal lock system, Juver, 2019/12/06 */
#include "./PersonalLockInfo.h"
#include "./PersonalLockPinCreate.h"
#include "./PersonalLockPinInput.h"
#include "./PersonalLockPinReset.h"
#include "./PersonalLockPinChange.h"
#include "./PersonalLockPinRecover.h"
#include "./PersonalLockPinRecoverResult.h"

/* modern party window, Juver, 2019/12/19 */
#include "./ModernPartyWindow.h"
#include "./ModernMiniPartyWindow.h"
#include "./ModernPartyState.h"

/* party finder, Juver, 2019/12/28 */
#include "./PartyFinderWindow.h"
#include "./PartyFinderQuestion.h"
#include "./PartyFinderIcon.h"

/* modern dialogue window, Juver, 2020/02/03 */
#include "./ModernDialogueWindow.h"

/* modern market window, Juver, 2020/02/05 */
#include "./ModernMarketWindow.h"

/* modern friend window, Juver, 2020/03/02 */
#include "./ModernFriendWindow.h"

/* modern rebirth dialogue, Juver, 2020/03/31 */
#include "./ModernRebirthDialogue.h"

/* modern info tooltip, Juver, 2020/04/09 */
#include "./BasicVarTextBoxEx.h"
#include "./BasicVarTextBoxExItemLink.h"

/* multi gate out, Juver, 2020/11/19 */
#include "./MultiMapMoveDisplay.h"

/* pvp club death match, Juver, 2020/11/26 */
#include "./PVPClubDeathMatchRebirthDialogue.h"
#include "./PVPclubDeathMatchBattleNotice.h"
#include "./PVPClubDeathMatchScore.h"
#include "./PVPClubDeathMatchTopRankings.h"
#include "./PVPClubDeathMatchRankings.h"
#include "./PVPClubDeathMatchDoublePointDisplay.h"
#include "./PVPClubDeathMatchRewardInfo.h"

/* chaos machine, Juver, 2021/07/08 */
#include "./ChaosMachineWindow.h"
#include "./ChaosMachineInventoryWindow.h"
#include "./EP7ChaosMachineInventoryWindow.h"
#include "./ModernChaosMachineInventoryWindow.h"

#include "./PKMarkDisplay.h"
#include "./NPCContributionShopWindow.h"
#include "./MaxRvPreviewWindow.h"
#include "./AutoPotionWindow.h"
#include "./AutoPilotWindow.h"

#include "./CharacterRebornAWindow.h"

#include "./ExchangeItemWindow.h"
#include "./ModernExchangeItemInventoryWindow.h"
#include "./ExchangeItemInventoryWindow.h"
#include "./EP7ExchangeItemInventoryWindow.h"

#include "./CaptchaWindow.h"

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
#include "_TyrannyWarRankingDisplay/PVPTyrannyRankingsDisplayMain.h"

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
#include "_ClubWarRankingDisplay/ClubWarRankingsDisplayMain.h"
#include "./PVPPBGScore.h"
#include "./PVPPBGRebirthDialogue.h"
#include "./PVPPBGTopRankings.h"
#include "./PVPPBGRankings.h"

#include "./TGRankingDisplay.h"
/*dmk14 freepk*/
#include "../Lib_ClientUI/Interface/BattleRankingDisplay.h"

/* Gacha System, MontageDev 7/10/24 */
#include "./GachaSystem/GachaWindow.h"
#include "./GachaSystem/GachaButton.h"

/* Boss Spawn Viewer, Review000 */
#include "BossDetailsWindow.h"
#include "BossViewerButton.h"

/* Add ItemShop, Ssodomain, 11-09-2023, Start */
#include "ItemShop/ItemShopWindow.h"
#include "ItemShop/ItemShopButton.h"
/* Add ItemShop, Ssodomain, 11-09-2023, End */

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void	CInnerInterface::SetPandoraBoxInfo ()
{
	if ( RPARAM::bFeaturePandora )
	{
		if ( m_pPandoraBoxWindow )
		{
			ShowGroupFocus ( PANDORA_BOX );
			m_pPandoraBoxWindow->InitWindow();
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
void CInnerInterface::SetGlobalRankingWindowOpen(BOOL bOpen)
{
	if (m_pGlobalRankingWindow)
	{
		if (bOpen)
		{
			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			if (pCharacter)
				pCharacter->ReqGlobalRanking();

			m_pGlobalRankingWindow->RESET();
			ShowGroupFocus(GLOBAL_RANKING_WINDOW);
			//HideGroup(GLOBAL_RANKING_BUTTON);
		}
		else
		{
			m_pGlobalRankingWindow->RESET();
			HideGroup(GLOBAL_RANKING_WINDOW);
			//ShowGroupBottom(GLOBAL_RANKING_BUTTON);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

void	CInnerInterface::ClosePandoraBoxWindow()
{
	HideGroup ( PANDORA_BOX );
}

void CInnerInterface::WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer )
{
	if ( IsVisibleGroup ( WAITSERVER_DISPLAY ) ) return ;

	ShowGroupFocus ( WAITSERVER_DISPLAY );
	m_pWaitServerDisplay->SetDisplayMessage ( strMessage, nAction, fTimer );
}

void CInnerInterface::WAITSERVER_DIALOGUE_CLOSE ()
{
	HideGroup ( WAITSERVER_DISPLAY );
}

void CInnerInterface::SetMiniMapInfo ( GLMapAxisInfo &sInfo, CString strMapName )
{
	if ( m_pMiniMap && m_pLargeMapWindow )
	{
		m_pMiniMap->SetMapAxisInfo ( sInfo, strMapName );
		m_pLargeMapWindow->SetMapAxisInfo ( sInfo, strMapName );
	}
}

const char* CInnerInterface::MakeString ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 256;

	va_list argList;
	static	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	return szBuffer;
}

void CInnerInterface::CloseAllWindow ()
{	
	CUIFocusContainer::UICONTROL_FOCUSLIST list = GetFocusList ()->GetFocusList ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_CITER citer = list.begin ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_CITER citer_end = list.end ();
	for ( ; citer != citer_end; ++citer )
	{
		UIGUID cID = citer->cID;

		if ( !IsVisibleGroup ( cID ) ) continue;

		if ( cID == INVENTORY_WINDOW || cID == MARKET_WINDOW ||
			cID == STORAGE_WINDOW || cID == CLUB_STORAGE_WINDOW )
		{
			SetMarketWindowClose ();
			SetStorageWindowClose ();
			SetClubStorageWindowClose ();
		}
		else if ( cID == TRADEINVENTORY_WINDOW )
		{
			if ( IsVisibleGroup ( TRADE_WINDOW ) ) SetTradeWindowCloseReq ();
			else if ( IsVisibleGroup ( PRIVATE_MARKET_WINDOW ) ) SetPrivateMarketClose ();
		}
		else if ( cID == TRADE_WINDOW )
		{							
			SetTradeWindowCloseReq ();
		}
		else if ( cID == MINIPARTY_WINDOW )
		{
			ShowGroupBottom ( MINIPARTY_OPEN );

//			#ifdef CH_PARAM // 중국 인터페이스 변경
//				UpdatePotionTrayPosition();
//			#endif
		}
		else if ( cID == PRIVATE_MARKET_WINDOW )
		{
			SetPrivateMarketClose ();
		}
		else if( cID == ITEM_REBUILD_WINDOW || cID == REBUILDINVENTORY_WINDOW )	// ITEMREBUILD_MARK
		{
			CloseItemRebuildWindow();
		}
		else if( cID == ITEM_GARBAGE_WINDOW || cID == GARBAGEINVENTORY_WINDOW )	// 휴지통
		{
			CloseItemGarbageWindow();
		}
		else if( cID == ITEM_MIX_WINDOW || cID == ITEM_MIX_INVEN_WINDOW )	// 아이템 조합
		{
			CloseItemMixWindow();
		}
		/*item transfer card, Juver, 2018/01/18 */
		else if( cID == ITEM_TRANSFER_WINDOW || cID == ITEM_TRANSFER_INVEN_WINDOW )
		{
			CloseItemTransferWindow();
		}
		/*item random option rebuild, Juver, 2018/07/03 */
		else if( cID == ITEM_RANDOM_OPTION_REBUILD_WINDOW || cID == ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW )
		{
			CloseItemRandomOptionRebuildWindow();
		}
		/* chaos machine, Juver, 2021/07/08 */
		else if( cID == CHAOS_MACHINE_WINDOW || cID == CHAOS_MACHINE_INVEN_WINDOW )
		{
			CloseChaosMachineWindow();
		}
		else if( cID == EXCHANGE_ITEM_WINDOW || cID == EXCHANGE_ITEM_INVEN_WINDOW )
		{
			CloseExchangeItemWindow();
		}
		else if ( IsVisibleGroup( PVP_CLUB_WAR_RANKING_WINDOW ) )
		{
			m_bPVPClubWarShowRanking = true;
		}

		// 사라지면 안되는 컨트롤을 제외한 컨트롤을 숨긴다.
		if ( cID != BASIC_CHAT_BOX && cID != BASIC_INFO_VIEW)
		{
#if defined(VN_PARAM) //vietnamtest%%%
			if( cID != VNGAINSYS_GAUGE_WINDOW )
#endif
			HideGroup ( citer->cID );
		}
	}
	
	/*item link, Juver, 2017/07/31 */
	if( IsVisibleGroup( INFO_DISPLAY_ITEM_LINK ) )
	{
		InfoDisplayItemLinkClearText ();
	}

	m_bPartyStateBack = FALSE;
}

void	CInnerInterface::SetPtoPWindowOpen ( PGLCHARCLIENT pCharClient, DWORD dwGaeaID, GLCLUB & sClub )
{
	GASSERT( pCharClient );

	D3DXVECTOR3 *pPos = GLGaeaClient::GetInstance().FindCharPos( pCharClient->GetName() );
	if ( pPos )
	{
		D3DXVECTOR3 vPos = *pPos;
		vPos.y += pCharClient->GetBodyHeight();

		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );

		const UIRECT& rcOriginPos = GetPtoPWindow ()->GetGlobalPos ();
		int nPosX = (int)( vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
		int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );
		
		GetPtoPWindow ()->SetPtoPMenu ( pCharClient, dwGaeaID, sClub );	
		GetPtoPWindow ()->SetGlobalPos ( D3DXVECTOR2( float(nPosX), float(nPosY) ) );
		ShowGroupFocus ( PTOPMENU_WINDOW );
	}
}

void CInnerInterface::SetTradeWindowOpen ( CString szPlayerName, DWORD dwTargetID )
{
	if ( IsVisibleGroup ( TRADE_WINDOW ) ) return ;

	GetTradeWindow ()->SetTradeInfo ( szPlayerName, dwTargetID );
	CloseAllWindow ();	

	const long lResolution = GetResolution ();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	{
		const UIRECT& rcTradeWindow = GetTradeWindow()->GetGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = ((X_RES) / 2.0f) - rcTradeWindow.sizeX;
		vPos.y = (Y_RES - rcTradeWindow.sizeY) / 2.0f;
		GetTradeWindow()->SetGlobalPos ( vPos );
	}

	{
		const UIRECT& rcTradeInventoryWindow = GetTradeInventoryWindow()->GetGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = (X_RES) / 2.0f;
		vPos.y = (Y_RES - rcTradeInventoryWindow.sizeY) / 2.0f;
		GetTradeInventoryWindow()->SetGlobalPos ( vPos );
	}

	ShowGroupFocus ( TRADE_WINDOW );
	ShowGroupFocus ( TRADEINVENTORY_WINDOW );
}

void CInnerInterface::SetDamage ( D3DXVECTOR3 vPos, int nDamage, DWORD dwDamageFlag, BOOL bAttack )
{
	//	카메라 프리즘 공간안에 데미지 위치가 있는지 검사.
	//
	const CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume();
	BOOL bOK = COLLISION::IsCollisionVolume(CV,vPos,vPos);
	if ( !bOK )				return;
	
	//	지형에 가려지는 부분에서 데미지가 위치하는지 검사.
	//
	PLANDMANCLIENT pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
	GASSERT(pLandClient&&"GLGaeaClient::GetInstance().GetActiveMap()");
	if ( !pLandClient )		return;

	DxLandMan *pLandMan = pLandClient->GetLandMan();
	GASSERT(pLandMan&&"pLandClient->GetLandMan()");
	if ( !pLandMan )		return;

	D3DXVECTOR3 vPoint1 = DxViewPort::GetInstance().GetFromPt();
	D3DXVECTOR3 vPoint2 = vPos;
	D3DXVECTOR3 vCollision;
	BOOL bCollision(FALSE);
	LPDXFRAME pDxFrame;
	pLandMan->IsCollision( vPoint1, vPoint2, vCollision, bCollision, pDxFrame, TRUE );
	if ( bCollision )		return;


	D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );
	const UIRECT& rcOriginPos = m_pDamageDisplayMan->GetGlobalPos ();

	//	Note : 데미지 좌우로 흩어져서 나오도록.
	//	20 이하의 값에서 설정
    int nPosX = (int)( vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
	int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );

	static const int nMAXINTERVAL = 20;
	static const int nBASE = int(nMAXINTERVAL / 2);

	int xInterval = rand()%nMAXINTERVAL - nBASE;
	int yInterval = rand()%nMAXINTERVAL - nBASE;
	nPosX += xInterval;
	nPosY += yInterval;

	long lResolution = CUIMan::GetResolution ();
	WORD X_RES = HIWORD(lResolution);
	WORD Y_RES = LOWORD(lResolution);
	if ( ( nPosX < 0.0f ) || ( nPosY < 0.0f )  || 
		( float(X_RES) < nPosX )  || ( float(Y_RES) < nPosY ) )
	{
		return ;
	}

	m_pDamageDisplayMan->SetDamage ( nPosX, nPosY, nDamage, dwDamageFlag, bAttack );
}

void CInnerInterface::SetTargetInfo ( STARGETID sTargetID )
{
	CString strName;
	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	GLDWDATA sHP;
	//WORD wSchool = CTargetInfoDisplay::NODATA;
	//WORD wParty = CTargetInfoDisplay::NODATA;


	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();
	sHP = pCOPY->GetHp();
	
	if ( pCOPY->GetCrow() == CROW_MOB || pCOPY->GetCrow() == CROW_SUMMON )
	{
		dwColor = GLGaeaClient::GetInstance().GetMobNameColor ( sTargetID.dwID );
	}
	else if ( pCOPY->GetCrow () == CROW_PC )
	{
		bool bBRIGHTEVENT   = GLGaeaClient::GetInstance().IsBRIGHTEVENT();
		bool bSCHOOL_FREEPK = GLGaeaClient::GetInstance().IsSchoolFreePk();
		bool bPVPMap = false;	/*pvp tyranny, Juver, 2017/08/25 */
	
		bool bOpenPK = false;	/* open pk function, Juver, 2019/12/24 */

		PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
		if ( pLand )
		{
			/*pvp tyranny, Juver, 2017/08/25 */
			/*school wars, Juver, 2018/01/19 */
			/*pvp capture the flag, Juver, 2018/01/31 */
			/* pvp club death match, Juver, 2020/11/26 */
			if ( pLand->m_bClubBattle || 
				pLand->m_bClubDeathMatch || 
				pLand->m_bPVPTyrannyMap || 
				pLand->m_bPVPSchoolWarsMap || 
				pLand->m_bPVPCaptureTheFlagMap ||
				pLand->m_bPVPClubDeathMatchLobbyMap || 
				pLand->m_bPVPClubDeathMatchBattleMap ||
				pLand->m_bPVPPBGLobbyMap ||
				pLand->m_bPVPPBGBattleMap ||
				pLand->m_bPVPWoeMap )
			{
				bPVPMap = true;
			}

			/* open pk function, Juver, 2019/12/24 */
			if ( pLand->IsOpenPK() )
			{
				bOpenPK = true;
			}
		}

		//	대련 또는 PK가 아니면, 피를 표시하지 않는다.
		if ( !GLGaeaClient::GetInstance().GetCharacter()->ISCONFRONT_TAR ( sTargetID ) &&
			!GLGaeaClient::GetInstance().GetCharacter()->IS_PLAYHOSTILE ( pCOPY->GetCharID() ) )
		{
			/*pvp tyranny, Juver, 2017/08/25 */
			/* open pk function, Juver, 2019/12/24 */
			if ( !(bBRIGHTEVENT || bSCHOOL_FREEPK || bPVPMap || bOpenPK ) )
			{
				//	피표시만 막음 ( 맥시멈 )
				sHP.TO_FULL ();
			}
		}

		//	이름 색 (PK_COLOR)
		PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );
		if ( !pCHAR ) return ;
		dwColor = pCHAR->GET_PK_COLOR ();
	}

	float fPercent = float(sHP.dwNow) / float(sHP.dwMax);
	if ( m_pTargetInfoDisplay ) m_pTargetInfoDisplay->SetTargetInfo ( fPercent, strName, dwColor );

	//	NOTE
	//		포지션 설정
	{
		// Note : 이름표시 높이조절
		D3DXVECTOR3 vPos = pCOPY->GetPosBodyHeight();
		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

		const UIRECT& rcOriginPos = m_pTargetInfoDisplay->GetGlobalPos ();
		int nPosX = (int)( floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f )));
		int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );
		if ( m_pTargetInfoDisplay )
		{
			//if ( abs ( rcOriginPos.left - nPosX ) > 1 || abs ( rcOriginPos.top - nPosY ) > 1 )
				m_pTargetInfoDisplay->SetGlobalPos ( D3DXVECTOR2 ( float ( nPosX ), float ( nPosY ) ) );
		}
		ShowGroupBottom ( TARGETINFO_DISPLAY );

		//	중복 이름 지우기
		if ( m_pNameDisplayMan )	m_pNameDisplayMan->VisibleTargetInfoCtrl ( sTargetID.emCrow, sTargetID.dwID, FALSE );
	}

	{
		/*pvp tyranny, Juver, 2017/08/25 */
		if ( pCOPY->GetCrow() == CROW_MOB )
		{
			PLANDMANCLIENT plandclient = GLGaeaClient::GetInstance().GetActiveMap();
			if ( plandclient )
			{
				PGLCROWCLIENT _pcrow = plandclient->GetCrow ( sTargetID.dwID );
				if ( _pcrow && _pcrow->m_pCrowData && _pcrow->m_pCrowData->m_sBasic.m_emNPCType == EMNPC_TYPE_EVENT_TOWER )
				{
					if ( m_pPVPTyrannyTowerProgress )
						m_pPVPTyrannyTowerProgress->UpdateInfo( _pcrow->m_wTyrannyOwner, _pcrow->m_fTyrannyDamage, _pcrow->m_dwTyrannyClubOwner, _pcrow->m_dwTyrannyClubMarkVer );

					D3DXVECTOR3 vPos = pCOPY->GetPosBodyHeight();
					D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

					const UIRECT& rcOriginPos = m_pPVPTyrannyTowerProgress->GetGlobalPos ();
					int nPosX = (int)( floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f )));
					int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );

					if ( m_pPVPTyrannyTowerProgress )
						m_pPVPTyrannyTowerProgress->SetGlobalPos ( D3DXVECTOR2 ( float ( nPosX ), float ( nPosY ) ) );

					ShowGroupBottom ( PVP_TYRANNY_TOWER_PROGRESS );
				}
			}
		}
	}
}

void CInnerInterface::SetMarketWindowOpen ( SNATIVEID sNativeID )
{
	if ( m_pMarketWindow )
	{
		m_pMarketWindow->InitMarket ( sNativeID );
		ShowGroupFocus ( MARKET_WINDOW );
	}
}

void CInnerInterface::SetStorageWindowOpen ( DWORD dwNPCID = 0 )
{
	if ( m_pStorageWindow )
	{
		m_pStorageWindow->InitStorage ( dwNPCID );
		ShowGroupFocus ( STORAGE_WINDOW );
	}
}

void CInnerInterface::SetBusWindowOpen ( const DWORD dwGaeaID, SNpcTalk* pNpcTalk )
{
	if ( m_pBusWindow )
	{
		if ( !m_pBusWindow->SetBusData ( dwGaeaID, pNpcTalk ) )
		{
			return ;
		}

		ShowGroupFocus ( BUS_WINDOW, true );
	}
}

void CInnerInterface::SetDialogueWindowOpen ( STARGETID sCrowID, GLCHARLOGIC_CLIENT* pCHAR )
{
	if ( m_pDialogueWindow )
	{
		//	TO DO :
		//		버스가 추가 되더라도, 이 함수를 거치게 된다.
		//		따라서, 버스 노선을 표시해야하는 것인지,		
		//		대화를 표시해야하는 것인지를 여기에서 구분해야한다.

		//	NOTE :
		//		sCrowID로 '버스'인지, '일반 NPC'인지 확인할 수 있다.
		if ( !m_pDialogueWindow->SetDialogueData ( sCrowID, pCHAR ) )
		{
//			GASSERT ( 0 && "오류! npc파일 읽기 실패!" );
			return;
		}
		ShowGroupFocus ( DIALOGUE_WINDOW );
	}
}

BOOL CInnerInterface::PrintMsgText ( D3DCOLOR dwColor, const char* szFormat, ... )
{	
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		 /*item link, Juver, 2017/07/31 */
		GetChat()->AddStringToSystemMessage ( szBuffer, dwColor, NULL );

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*link drop item, EJCode, 2018/10/20 */
BOOL CInnerInterface::PrintMsgText ( D3DCOLOR dwColor, SITEMLINK* pItemLink, const char* szFormat, ... )
{	
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		/*item link, Juver, 2017/07/31 */
		GetChat()->AddStringToSystemMessage ( szBuffer, dwColor, pItemLink );

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CInnerInterface::PrintConsoleText ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		 /*item link, Juver, 2017/07/31 */
		GetChat()->AddStringToChatEx ( szBuffer, CHAT_SYSTEM, NULL );

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CInnerInterface::PrintMsgTextDlg ( D3DCOLOR dwColor, const char* szFormat, ... )
{	
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		/*item link, Juver, 2017/07/31 */
		GetChat()->AddStringToSystemMessage ( szBuffer, dwColor, NULL ); 

		m_pSystemMessageWindow->SetSystemMessage ( szBuffer, dwColor );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CInnerInterface::PrintConsoleTextDlg ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		/*item link, Juver, 2017/07/31 */
		GetChat()->AddStringToChatEx ( szBuffer, CHAT_SYSTEM, NULL );

		m_pSystemMessageWindow->SetSystemMessage ( szBuffer, NS_UITEXTCOLOR::HOTPINK );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CInnerInterface::PrintMsgDlg ( D3DCOLOR dwColor, const char* szFormat, ... )
{
	GASSERT( szFormat );

	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	GASSERT( m_pSystemMessageWindow );
	if( m_pSystemMessageWindow )
	{
		m_pSystemMessageWindow->SetSystemMessage( szBuffer, dwColor );
		return TRUE;
	}
	else return FALSE;
}

void CInnerInterface::SetTradeWindowClose ()
{
	HideGroup ( TRADE_WINDOW );
	HideGroup ( TRADEINVENTORY_WINDOW );
}

void	CInnerInterface::SetTradeWindowCloseReq ()
{
	//	취소 버튼 누름
	//
	GLMSG::SNET_TRADE_CANCEL NetMsg;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

void	CInnerInterface::SetFightBegin ( const int nIndex )
{
	GetConftDisplayMan()->SetFightBegin ( nIndex );
	ShowGroupTop ( CONFT_DISPLAY_MAN );
}

void	CInnerInterface::SetFightEnd ( const int nResult )
{
	GetConftDisplayMan()->SetFightEnd ( nResult );
	ShowGroupTop ( CONFT_DISPLAY_MAN );
}

void	CInnerInterface::SetStorageWindowClose ()
{
	HideGroup ( STORAGE_WINDOW );
	HideGroup ( INVENTORY_WINDOW );
}

void	CInnerInterface::SetClubStorageWindowOpen ()
{
	if ( m_pClubStorageWindow )
	{
		m_pClubStorageWindow->InitStorage ();
		ShowGroupFocus ( CLUB_STORAGE_WINDOW );
	}
}

void	CInnerInterface::SetClubStorageWindowClose ()
{
	HideGroup ( CLUB_STORAGE_WINDOW );
	HideGroup ( INVENTORY_WINDOW );
}

void	CInnerInterface::SetMarketWindowClose ()
{
	HideGroup ( MARKET_WINDOW );
	HideGroup ( INVENTORY_WINDOW );
}

BOOL CInnerInterface::IsPartyMember ( const char* szName )
{
	GLPARTY_CLIENT* pMaster = GLPartyClient::GetInstance().GetMaster();
	if ( pMaster )
	{
		//	이름 비교

		//	NOTE
		//		마스터이면
		if ( !strcmp ( pMaster->m_szName, szName ) )
		{
			return TRUE;
		}
		else
		{
			//	NOTE
			//		멤버이면
			DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
			if( nMEMBER_NUM > 1 )
			{
				nMEMBER_NUM -= 1; // Note : 마스터는 제외
				for ( DWORD i = 0; i < nMEMBER_NUM; i++ )
				{
					GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember ( i );			
					if ( pMember )
					{
						if ( !strcmp ( pMember->m_szName, szName ) )
						{						
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

void CInnerInterface::ClearNameList ()
{
	if ( m_pNameDisplayMan ) m_pNameDisplayMan->ClearList ();
}

void CInnerInterface::SetAcademyFightBegin ( const int& nLMARK, const int& nLNUMBER, const int& nRMARK, const int& nRNUMBER )
{
	m_pAcademyConftDisplay->SET_MARK_L ( nLMARK );
	m_pAcademyConftDisplay->SET_MARK_R ( nRMARK );
	m_pAcademyConftDisplay->SET_NUMBER_L ( nLNUMBER );
	m_pAcademyConftDisplay->SET_NUMBER_R ( nRNUMBER );
	m_pAcademyConftDisplay->UPDATE_ALPHA ( USHRT_MAX );

	ShowGroupTop ( ACADEMY_CONFT_DISPLAY, true );

	m_bACADEMY_FIGHT_READY = true;
}

void CInnerInterface::SetAcademyUpdateNumber ( const int& nLNUMBER, const int& nRNUMBER )
{
	const int nLNUMBER_BACK = m_pAcademyConftDisplay->GET_NUMBER_L ();
	const int nRNUMBER_BACK = m_pAcademyConftDisplay->GET_NUMBER_R ();
	bool bUPDATE = false;

	if ( nLNUMBER_BACK != nLNUMBER )
	{
		m_pAcademyConftDisplay->SET_NUMBER_L ( nLNUMBER );
		bUPDATE = true;
	}
	if ( nRNUMBER_BACK != nRNUMBER )
	{
		m_pAcademyConftDisplay->SET_NUMBER_R ( nRNUMBER );
		bUPDATE = true;
	}

	if ( bUPDATE || m_bACADEMY_FIGHT_READY ) m_pAcademyConftDisplay->UPDATED_NUMBER ();

	m_bACADEMY_FIGHT_READY = false;
}

void CInnerInterface::SetAcademyFightEnd ()
{
	HideGroup ( ACADEMY_CONFT_DISPLAY, true );
}

void CInnerInterface::SetQuestWindowOpen ( DWORD dwQuestID )
{
	if ( NATIVEID_NULL().dwID != m_dwEventQuestID )
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqQuestREADINGReset ( m_dwEventQuestID );
		m_dwEventQuestID = NATIVEID_NULL().dwID;
	}

	ShowGroupFocus ( QUEST_WINDOW );
	m_pQuestWindow->AUTOSELECT_PAGE ( dwQuestID );	
}

void CInnerInterface::REFRESH_QUEST_WINDOW ()
{
	if ( IsVisibleGroup ( QUEST_WINDOW ) )
		m_pQuestWindow->REFRESH_QUEST_WINDOW ();	
}

void CInnerInterface::REFRESH_CLUB_LIST ()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) 
	{
		m_pClubWindow->LoadClubMemberList ();
	}
}

void CInnerInterface::REFRESH_CLUB_ALLIANCE_LIST()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->LoadClubAllianceList ();
}

void CInnerInterface::REFRESH_CLUB_BATTLE_LIST()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->LoadClubBattleList ();
}

void CInnerInterface::REFRESH_CLUB_STATE ()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->RefreshState ();
}

void CInnerInterface::REFRESH_CLUB_NOTICE()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->SetAnnouncement();
}

void CInnerInterface::REFRESH_ATTENDBOOK()
{
	if ( m_pAttendanceBookWindow ) m_pAttendanceBookWindow->RefreshAttend();
}


void	CInnerInterface::UpdateStateQuestAlarm ()
{
	if ( !IsVisibleGroup ( QUEST_WINDOW ) )	//	퀘스트창이 닫긴 상태
	{
		GLQuestPlay& cQuestPlay = GLGaeaClient::GetInstance().GetCharacter()->m_cQuestPlay;
		if ( !cQuestPlay.GetQuestProc ().empty () )	//	진행중인 퀘스트가 존재한다.
		{
			if ( !IsVisibleGroup ( QUEST_ALARM ) )
			{
				ShowGroupBottom ( QUEST_ALARM );	//	퀘스트 아이콘 표시
			}
			
			DWORD dwQuestID = NATIVEID_NULL().dwID;			
			if ( cQuestPlay.GetReqREADING ( dwQuestID ) )	//	퀘스트 이벤트 발생
			{
				GLQUESTPROG* pQuestProg = cQuestPlay.FindProc ( dwQuestID );
				if ( !pQuestProg ) return ;				
				const DWORD dwEventQuestStep = pQuestProg->m_dwSTEP;

				if ( m_dwEventQuestID != dwQuestID || dwEventQuestStep != m_dwEventQuestStep )
				{
					DxSoundLib::GetInstance()->PlaySound ( "QuestAlarm" );
					m_dwEventQuestID = dwQuestID;
					m_dwEventQuestStep = dwEventQuestStep;
					
					RESET_QUEST_HELPER( dwQuestID );
				}

				m_pQuestAlarm->SET_ALARM ();
//				m_pQuestWindow->SetQuestID ( m_dwEventQuestID );

				return ;
			}
		}
		else
		{
			HideGroup ( QUEST_ALARM );
		}
	}
	else
	{
		GLQuestPlay& cQuestPlay = GLGaeaClient::GetInstance().GetCharacter()->m_cQuestPlay;
		if ( !cQuestPlay.GetQuestProc ().empty () )	//	진행중인 퀘스트가 존재한다.
		{
			DWORD dwQuestID = NATIVEID_NULL().dwID;
			if ( cQuestPlay.GetReqREADING ( dwQuestID ) )	//	퀘스트 이벤트 발생
			{
				GLQUESTPROG* pQuestProg = cQuestPlay.FindProc ( dwQuestID );
				if ( !pQuestProg ) return ;				
				const DWORD dwEventQuestStep = pQuestProg->m_dwSTEP;

				if ( m_dwEventQuestID != dwQuestID || dwEventQuestStep != m_dwEventQuestStep )
				{
					DxSoundLib::GetInstance()->PlaySound ( "QuestAlarm" );
					m_dwEventQuestID = dwQuestID;
					m_dwEventQuestStep = dwEventQuestStep;

					RESET_QUEST_HELPER( dwQuestID );
				}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 )
				//	리스트 페이지가 아니고.
				if ( CQuestWindow::QUEST_LIST != m_pQuestWindow->GET_SHOW_PAGE () )
				{
					//	이벤트 발생한 ID와 현재 보고 있는 퀘스트의 ID가 다를경우
					if ( m_dwEventQuestID != m_pQuestWindow->GetQuestID () )
					{
						m_pQuestAlarm->SET_ALARM ();
						//						m_pQuestWindow->SetQuestID ( m_dwEventQuestID );

						if ( !IsVisibleGroup ( QUEST_ALARM ) )
						{
							ShowGroupBottom ( QUEST_ALARM );	//	퀘스트 아이콘 표시
						}

						return ;
					}
				}
#else
				if ( m_pQuestWindow )
				{
					if ( m_dwEventQuestID != m_pQuestWindow->GetQuestID () )
					{
						m_pQuestAlarm->SET_ALARM ();

						if ( !IsVisibleGroup ( QUEST_ALARM ) )
						{
							ShowGroupBottom ( QUEST_ALARM );
						}

						return ;
					}
				}
#endif
				GLGaeaClient::GetInstance().GetCharacter()->ReqQuestREADINGReset ( m_dwEventQuestID );					
				m_dwEventQuestID = NATIVEID_NULL().dwID;				
			}
		}

		HideGroup ( QUEST_ALARM );
	}

//#ifdef CH_PARAM // 중국 인터페이스 변경
//	UpdatePotionTrayPosition();
//#endif
}

void CInnerInterface::DOMODAL_ADD_FRIEND_REQ ( const CString& strName )
{
	if ( IsVisibleGroup ( MODAL_WINDOW ) )
	{
		return ;
	}

	m_strFriendName = strName;
	CString strCombine = MakeString ( ID2GAMEINTEXT("ADD_FRIEND_REQ"), strName );	
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ADD_FRIEND_LURE_ANS );
}

void CInnerInterface::REFRESH_FRIEND_LIST ()
{
	if ( !IsVisibleGroup ( FRIEND_WINDOW ) ) return ;

	//	목록 변경 ( 추가, 삭제... 그외 )
	m_pFriendWindow->LoadFriendList ();
	m_pChat->REFRESH_FRIEND_LIST ();
}

void CInnerInterface::REFRESH_FRIEND_STATE()
{
	if ( !IsVisibleGroup ( FRIEND_WINDOW ) ) return ;

	//	상태 변경 ( 온/오프라인 )
	m_pFriendWindow->LoadFriendList ();
}

void CInnerInterface::OPEN_MODAL ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
	const int nMODAL_NEWID = CModalWindow::MAKE_MODAL_NEWID () + MODAL_WINDOW;	

	CModalWindow* pModalWindow = new CModalWindow;
	pModalWindow->Create ( nMODAL_NEWID, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
	pModalWindow->CreateSubControl ();
	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	pModalWindow->DoModal ( strText, nModalTitle, nModalType, CallerID );
	pModalWindow->SetHide ( bHide );
	pModalWindow->m_bTEST_MULTI = true;
	pModalWindow->InitDeviceObjects ( m_pd3dDevice );
	pModalWindow->RestoreDeviceObjects ( m_pd3dDevice );
	RegisterControl ( pModalWindow );	

	ShowGroupFocus ( nMODAL_NEWID );
}

void CInnerInterface::OPEN_MODALESS ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
	const int nMODAL_NEWID = CModalWindow::MAKE_MODAL_NEWID () + MODAL_WINDOW;

	CModalWindow* pModalWindow = new CModalWindow;
	pModalWindow->Create ( nMODAL_NEWID, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
	pModalWindow->CreateSubControl ();
	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	pModalWindow->DoModal ( strText, nModalTitle, nModalType, CallerID );
	pModalWindow->SetHide ( bHide );
	pModalWindow->m_bTEST_MULTI = true;
	pModalWindow->SetUseModaless ( true );
	pModalWindow->InitDeviceObjects ( m_pd3dDevice );
	pModalWindow->RestoreDeviceObjects ( m_pd3dDevice );
	RegisterControl ( pModalWindow );	

	ShowGroupFocus ( nMODAL_NEWID );
}

void CInnerInterface::CLOSE_MODAL ( const UIGUID cID, bool bMakeMsg )
{		
	CUIControl* pControl = GetFocusControl ();	
	if ( pControl && pControl->GetWndID () != cID )
	{
		if ( !((CModalWindow*)pControl)->IsUseModaless () )
		{
			GASSERT ( 0 && "오류, 모달 삭제가 정상적이지 않습니다." );
		}
	}
	HideGroup ( cID, bMakeMsg );
}

void	CInnerInterface::UpdateStateSimpleHP ()
{
	if ( DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP )
	{
		if ( !IsVisibleGroup ( SIMPLE_HP ) )
			ShowGroupBottom ( SIMPLE_HP );
	}
	else
	{
		if ( IsVisibleGroup ( SIMPLE_HP ) )
			HideGroup ( SIMPLE_HP );
	}

	if ( IsVisibleGroup ( SIMPLE_HP ) )
	{
		const long lResolution = GetResolution ();
		WORD X_RES = HIWORD ( lResolution );
		WORD Y_RES = LOWORD ( lResolution );

		D3DXVECTOR3 vPos = GLGaeaClient::GetInstance().GetCharacter()->GetPosition ();	

		//	NOTE
		//		포지션 설정
		{
			vPos.y -= 2.0f;
			D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

			const UIRECT& rcOriginPos = m_pSimpleHP->GetGlobalPos ();
			D3DXVECTOR2 vNewPos;
			vNewPos.x = (X_RES - rcOriginPos.sizeX) / 2.0f;
			vNewPos.y = ( vScreen.y - rcOriginPos.sizeY );

			if ( fabs ( rcOriginPos.left - vNewPos.x ) > 1.0f ||
				fabs ( rcOriginPos.top - vNewPos.y ) > 1.0f ) 

			m_pSimpleHP->SetGlobalPos ( vNewPos );			
		}
	}
}

void	CInnerInterface::SetLottoOpen ( const DWORD& dwGaeaID )
{
	CString strCombine = MakeString ( ID2GAMEINTEXT("UI_LOTTO"), "2004년 7월 2주차" );	
	DoModal ( strCombine, MODAL_INPUT, MODAL_EDITBOX_LOTTERY, MODAL_LOTTO );	
}

void	CInnerInterface::SetStorageChargeOpen ( const WORD& wPosX, const WORD& wPosY )
{
	if ( m_pStorageChargeCard )
	{
		ShowGroupFocus ( STORAGE_CHARGE_CARD );
		m_pStorageChargeCard->InitStorageChargeCard ( wPosX, wPosY );
	}
}

void	CInnerInterface::SetItemBankInfo ()
{     
	if ( m_pItemBankWindow )
	{
		ShowGroupFocus ( ITEMBANK_WINDOW );
		m_pItemBankWindow->InitItemBank ();
	}
}

void	CInnerInterface::SetItemBankWindowOpen ()
{

	if( m_fItemBankDelay >= 5.0f )
	{
		m_fItemBankDelay = 0.0f;
	}else{
		PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("ITEM_BANK_DELAY") );
		return;
	}

	ClearItemBank ();

	// 여기서 서버에 유료아이템 정보 요청
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
	if ( pCharacter )
	{
		pCharacter->ReqItemBankInfo ();
	}

	SetItemBankInfo ();
}

void	CInnerInterface::SetVNGainSysInfo ()
{     
	if ( m_pVNGainSysInventory )
	{
		ShowGroupFocus ( VNGAINSYS_WINDOW );
		m_pVNGainSysInventory->InitVNGainSys();
	}
}

void	CInnerInterface::SetVNGainSysWindowOpen ()
{
	if ( m_bFirstVNGainSysCall )
	{
		// 여기서 서버에 유료아이템 정보 요청
		//GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
		/*if ( pCharacter )
		{
			pCharacter->ReqVNGainSysInfo ();
		}*/
		m_bFirstVNGainSysCall = false;
	}

	SetVNGainSysInfo ();
}
	
void CInnerInterface::OpenItemRebuildWindow()	// ITEMREBUILD_MARK
{
	if ( isReviveWaiting() )				return;

	if( !m_pItemRebuildWindow || !m_pRebuildInventoryWindow )
		return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	// RebuildInventory Window
	const UIRECT& rcInvenWindow = m_pRebuildInventoryWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pRebuildInventoryWindow->SetGlobalPos( vPos );
	ShowGroupFocus( REBUILDINVENTORY_WINDOW );

	// ItemRebuild Window
	const UIRECT& rcRebuildWindow = m_pItemRebuildWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
	m_pItemRebuildWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_REBUILD_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildOpen();
}

void CInnerInterface::CloseItemRebuildWindow()
{
	HideGroup( REBUILDINVENTORY_WINDOW );
	HideGroup( ITEM_REBUILD_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildClose();
}

void CInnerInterface::OpenItemGarbageWindow( BOOL bUseCard )	// 휴지통
{
	if ( isReviveWaiting() )				return;

	if( !m_pItemGarbageWindow || !m_pGarbageInventoryWindow )
		return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	// RebuildInventory Window
	const UIRECT& rcInvenWindow = m_pGarbageInventoryWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pGarbageInventoryWindow->SetGlobalPos( vPos );
	ShowGroupFocus( GARBAGEINVENTORY_WINDOW );

	// ItemRebuild Window
	const UIRECT& rcRebuildWindow = m_pItemGarbageWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
	m_pItemGarbageWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_GARBAGE_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageOpen( bUseCard );
}

void CInnerInterface::CloseItemGarbageWindow()
{
	HideGroup( GARBAGEINVENTORY_WINDOW );
	HideGroup( ITEM_GARBAGE_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageClose();
}

void CInnerInterface::OpenItemMixWindow( DWORD dwNpcID )
{
	if ( isReviveWaiting() )				return;

	if( !m_pItemMixWindow || !m_pItemMixInvenWindow )
		return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	const UIRECT& rcInvenWindow = m_pItemMixInvenWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pItemMixInvenWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_MIX_INVEN_WINDOW );

	// ItemRebuild Window
	const UIRECT& rcRebuildWindow = m_pItemMixWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
	m_pItemMixWindow->SetGlobalPos( vPos );
	m_pItemMixWindow->m_dwNpcID = dwNpcID;
	ShowGroupFocus( ITEM_MIX_WINDOW );
}

void CInnerInterface::CloseItemMixWindow()
{
	HideGroup( ITEM_MIX_INVEN_WINDOW );
	HideGroup( ITEM_MIX_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ResetItemMix();
}

void	CInnerInterface::REFRESH_ITEMBANK ()
{
	if ( m_pItemBankWindow )
	{		
		m_pItemBankWindow->InitItemBank ();
	}
}

void	CInnerInterface::REFRESH_VNGAINSYSTEM ()
{
	if ( m_pVNGainSysInventory )
	{		
		m_pVNGainSysInventory->InitVNGainSys();
	}
}

void	CInnerInterface::SetInventorySlotViewSize ( const int& nSLOT_VIEW_SIZE )
{
	if ( m_pInventoryWindow )
		m_pInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	if ( m_pGarbageInventoryWindow )
		m_pGarbageInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	if ( m_pItemMixInvenWindow )
		m_pItemMixInvenWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	if ( m_pRebuildInventoryWindow )
		m_pRebuildInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	if ( m_pTradeInventoryWindow )
		m_pTradeInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	/*item transfer card, Juver, 2018/01/18 */
	if ( m_pItemTransferInvenWindow )
		m_pItemTransferInvenWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	/*item random option rebuild, Juver, 2018/07/03 */
	if ( m_pItemRandomOptionRebuildInventoryWindow )
		m_pItemRandomOptionRebuildInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	/* chaos machine, Juver, 2021/07/08 */
	if ( m_pChaosMachineInvenWindow )
		m_pChaosMachineInvenWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );

	if ( m_pExchangeItemInvenWindow )
		m_pExchangeItemInvenWindow->SetOneViewSlot( nSLOT_VIEW_SIZE );
}

int CInnerInterface::GetInventorySlotViewSize () const
{
	if ( m_pInventoryWindow )
	{
		return m_pInventoryWindow->GetOneViewSlot ();
	}

	GASSERT ( 0 && "인벤토리 사이즈를 알 수 없습니다." );
	return 0;
}

HRESULT CInnerInterface::FinalCleanup ()
{
	HRESULT hr = S_OK;

	hr = CUIMan::FinalCleanup ();
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

void CInnerInterface::SetPrivateMarketOpen ( const bool& bOPENER, const DWORD& dwGaeaID )
{
	if ( IsVisibleGroup ( PRIVATE_MARKET_WINDOW ) ) return ;

	if ( m_pPrivateMarketWindow )
	{
		m_pPrivateMarketWindow->InitPrivateMarket ( bOPENER, dwGaeaID );
		m_pTradeInventoryWindow->SetOPENER ( bOPENER );
        
		const BOOL bTemp_STATEBACK = m_bPartyStateBack;
		CloseAllWindow ();

		//	NOTE
		//			파티를 한 상태에서 미니파티창을 줄여두고,
		//			개인상점을 여는 경우...
		//			미니파티창이 다시 켜진다.
		//			이유는 CloseAllWindow()를 호출하면,
		//			모든 창을 닫게 되고, 파티상태(m_bPartyStateBack)를 리셋하게 되는데
		//			이렇게 될 경우, 새로운 파티가 구성된것으로 처리해서
		//			미니파티창을 다시 띄우게 되는 것이다.
		//			따라서, CloseAllWindow()이전에 m_bPartyStateBack이 true였다면,
		//			이후에도 true로 유지시켜 준다.
		m_bPartyStateBack = bTemp_STATEBACK;
		//////////////////////////////////////////////////////////////////

		const long lResolution = GetResolution ();
		WORD X_RES = HIWORD ( lResolution );
		WORD Y_RES = LOWORD ( lResolution );
		{
			const UIRECT& rcPrivateMarketWindow = m_pPrivateMarketWindow->GetGlobalPos ();

			D3DXVECTOR2 vPos;
			vPos.x = ((X_RES) / 2.0f) - rcPrivateMarketWindow.sizeX;
			vPos.y = (Y_RES - rcPrivateMarketWindow.sizeY) / 2.0f;
			m_pPrivateMarketWindow->SetGlobalPos ( vPos );
		}

		{
			const UIRECT& rcTradeInventoryWindow = GetTradeInventoryWindow()->GetGlobalPos ();

			D3DXVECTOR2 vPos;
			vPos.x = (X_RES) / 2.0f;
			vPos.y = (Y_RES - rcTradeInventoryWindow.sizeY) / 2.0f;
			GetTradeInventoryWindow()->SetGlobalPos ( vPos );
		}

		ShowGroupFocus ( PRIVATE_MARKET_WINDOW );
		ShowGroupFocus ( TRADEINVENTORY_WINDOW );
	}
}

void CInnerInterface::GetPrivateMarketInfo ( bool& bOPENER, DWORD& dwGaeaID )
{	
	bOPENER = m_pPrivateMarketWindow->IsOPENER ();
	dwGaeaID = m_pPrivateMarketWindow->GetPrivateMarketID ();;
}

void CInnerInterface::SetPrivateMarketClose ()
{
	HideGroup ( PRIVATE_MARKET_WINDOW );
	HideGroup ( TRADEINVENTORY_WINDOW );
    
	if ( m_pPrivateMarketWindow->IsOPENER () )
		GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketClose ();
	else
		GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketInfoRelease ( m_pPrivateMarketWindow->GetPrivateMarketID () );
}

void CInnerInterface::MODAL_PRIVATE_MARKET_SELLITEM ( const bool& bCOUNTABLE, const WORD& wPosX, const WORD& wPosY )
{
	if ( m_pPrivateMarketSellWindow )
	{
		m_pPrivateMarketSellWindow->SetType ( bCOUNTABLE );
		m_pPrivateMarketSellWindow->SetCallerID ( MODAL_PRIVATE_MARKET_SELL );
		m_pPrivateMarketSellWindow->SetItemIndex ( wPosX, wPosY );
		ShowGroupFocus ( PRIVATE_MARKET_SELL_WINDOW, true );
	}
}

const	CString& CInnerInterface::GetPrivateMarketTitle ()
{
	return m_pPrivateMarketMake->GetTITLE ();
}

void	CInnerInterface::SetPrivateMarketMake ()
{	
	if( !IsVisibleGroup ( PRIVATE_MARKET_MAKE ) )
	{
		if ( m_pPrivateMarketMake )
		{
			m_pPrivateMarketMake->Init ();
		}
	}

	ShowGroupFocus ( PRIVATE_MARKET_MAKE, true ); // 
}

void	CInnerInterface::SetClubMake ( const DWORD& dwNpcID )
{
	if ( !IsVisibleGroup ( CLUB_MAKE ) )
	{
		if ( m_pClubMake )
		{
			m_pClubMake->Init ( dwNpcID );
			ShowGroupFocus ( CLUB_MAKE, true );
		}
	}	
}

CString CInnerInterface::GetdwKeyToString(int dwKey)
{
	return GetKeySettingWindow()->GetdwKeyToString(dwKey);
}

void 	CInnerInterface::SetShotcutText ( DWORD nID, CString& strTemp )
{
	if( nID >= 0 && nID < QUICK_SLOT_NUM )
	{
		m_pUILeftTopGroup->SetShotcutText( nID, strTemp );
	}
	else if( nID >= QUICK_SLOT_NUM && nID < (QUICK_SLOT_NUM + QUICK_SKILL_NUM) )
	{
		m_pSkillTrayTab->SetShotcutText( nID - ( QUICK_SLOT_NUM) , strTemp);
		m_pSkillTrayTabEx->SetShotcutText( nID - ( QUICK_SLOT_NUM) , strTemp);
	}
	else if( nID >= (QUICK_SLOT_NUM + QUICK_SKILL_NUM) && 
			  nID < (QUICK_SLOT_NUM + QUICK_SKILL_NUM + MENU_SHOTCUT_NUM ))
	{
		int ranIndex = RPROFILE::KeySettingToRanparam[nID- (QUICK_SLOT_NUM + QUICK_SKILL_NUM) ];		
		int menuIndex;
		
		for (int i = 0; i < BASIC_MENU_NUM; ++i)
		{
			menuIndex = RPROFILE::BasicMenuToRanparam[i];
			if( ranIndex == menuIndex )
			{
				m_pGameMenu->SetShotcutText( i , strTemp);
				return;
			}
		}		
	}
}

void CInnerInterface::GetShotCutKey(){
	m_pKeySettingWindow->GetShotCutKey();
}

void CInnerInterface::GetChatMacro(){
	m_pChatMacroWindow->GetChatMacro();
}
void CInnerInterface::AddChatMacro(int nIndex)
{
	GetChat()->AddChatMacro (RPROFILE::ChatMacro[nIndex]);
}

void CInnerInterface::ChatLog( bool bChatLogt, int nChatLogType )
{
	GetChat()->ChatLog( bChatLogt, nChatLogType );
}

CString CInnerInterface::GET_RECORD_CHAT()
{
	if( !GetChat() ) return NULL;
	return GetChat()->GET_RECORD_CHAT();
}

void CInnerInterface::DOMODAL_CLUB_JOIN_ASK ( const DWORD& dwMasterID, const CString& strClubName, const CString& strClubMaster )
{
	m_dwClubMasterID = dwMasterID;
	
	CString strCombine = MakeString ( ID2GAMEINTEXT("CLUB_JOIN_ASK"), strClubName, strClubMaster );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_JOIN_ASK );
}

void CInnerInterface::DOMODAL_UNION_JOIN_ASK ( const DWORD& dwMasterID, const CString& strClubMaster )
{
	m_dwClubMasterID = dwMasterID;

	CString strCombine = MakeString ( ID2GAMEINTEXT("UNION_JOIN_ASK"), strClubMaster );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_UNION_JOIN_ASK );
}

void CInnerInterface::DOMODAL_CLUB_BATTLE_ASK ( const DWORD& dwMasterID, const CString& strClubMaster, DWORD dwBattleTime, bool bAlliance )
{
	m_dwClubMasterID = dwMasterID;

	if ( bAlliance ) 
	{
		CString strCombine = MakeString ( ID2GAMEINTEXT("ALLIANCE_BATTLE_ASK"), strClubMaster, dwBattleTime/60, dwBattleTime%60 );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ALLIANCE_BATTLE_ASK );
	}
	else
	{
		CString strCombine = MakeString ( ID2GAMEINTEXT("CLUB_BATTLE_ASK"), strClubMaster, dwBattleTime/60, dwBattleTime%60 );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_BATTLE_ASK );
	}
}

void CInnerInterface::DOMODAL_CLUB_AUTHORITY_ASK( const CString& strClubName )
{
	CString strCombine = MakeString ( ID2GAMEINTEXT("CLUB_AUTHORITY_MEMBER_ASK"), strClubName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_AUTHORITY_ASK );
}

void CInnerInterface::DOMODAL_CLUB_BATTLE_ARMISTICE_ASK( const DWORD& dwClubID, const CString& strClubName, bool bAlliance )
{
	// 임시 저장
	m_dwClubMasterID = dwClubID;
	CString strCombine;
	
	if ( bAlliance ) 
	{
		strCombine = MakeString ( ID2GAMEINTEXT("ALLIANCE_BATTLE_ARMISTICE_ASK"), strClubName );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ALLIANCE_BATTLE_ARMISTICE_ASK );
	}
	else 
	{
		strCombine = MakeString ( ID2GAMEINTEXT("CLUB_BATTLE_ARMISTICE_ASK"), strClubName );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_BATTLE_ARMISTICE_ASK );
	}
}

bool CInnerInterface::SET_QUESTION_ITEM_ID ( int nID )
{
	UIGUID cID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		cID = CQuestionItemDisplay::SPEED_UP;		break;
	case QUESTION_CRAZY:		cID = CQuestionItemDisplay::CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	cID = CQuestionItemDisplay::POWER_UP;		break;
	case QUESTION_EXP_UP:		cID = CQuestionItemDisplay::EXP_TIME;		break;
	case QUESTION_EXP_GET:		cID = CQuestionItemDisplay::EXP_GET;		break;
	case QUESTION_LUCKY:		cID = CQuestionItemDisplay::LUCKY;			break;
	case QUESTION_BOMB:			cID = CQuestionItemDisplay::BOMB;			break;
	case QUESTION_MOBGEN:		cID = CQuestionItemDisplay::OH_NO;			break;
	case QUESTION_SPEED_UP_M:	cID = CQuestionItemDisplay::SPEED_UP_M;		break;
	case QUESTION_MADNESS:		cID = CQuestionItemDisplay::MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	cID = CQuestionItemDisplay::ATTACK_UP_M;	break;
	case QUESTION_HEAL:			cID = CQuestionItemDisplay::HEAL;			break;
	}

	if ( cID == NO_ID ) return false;

	ShowGroupTop ( QUESTION_ITEM_DISPLAY );
	m_pQuestionItemDisplay->START ( cID );

	return true;
}

bool	CInnerInterface::SET_KEEP_QUESTION_ITEM_ID ( int nID )
{
	UIGUID cID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		cID = CQuestionItemDisplay::SPEED_UP;		break;
	case QUESTION_CRAZY:		cID = CQuestionItemDisplay::CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	cID = CQuestionItemDisplay::POWER_UP;		break;
	case QUESTION_EXP_UP:		cID = CQuestionItemDisplay::EXP_TIME;		break;
	case QUESTION_EXP_GET:		cID = CQuestionItemDisplay::EXP_GET;		break;
	case QUESTION_LUCKY:		cID = CQuestionItemDisplay::LUCKY;			break;
	case QUESTION_BOMB:			cID = CQuestionItemDisplay::BOMB;			break;
	case QUESTION_MOBGEN:		cID = CQuestionItemDisplay::OH_NO;			break;
	case QUESTION_SPEED_UP_M:	cID = CQuestionItemDisplay::SPEED_UP_M;		break;
	case QUESTION_MADNESS:		cID = CQuestionItemDisplay::MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	cID = CQuestionItemDisplay::ATTACK_UP_M;	break;
	case QUESTION_HEAL:			cID = CQuestionItemDisplay::HEAL;			break;
	}

	if ( m_pQBoxButton )
		m_pQBoxButton->ResetType();

	if ( cID == NO_ID ) return false;

	RESET_KEEP_QUESTION_ITEM();

	ShowGroupTop ( QUESTION_ITEM_DISPLAY );
	m_pQuestionItemDisplay->KEEP_START ( cID );

	return true;
}

void	CInnerInterface::RESET_KEEP_QUESTION_ITEM ()
{
	if( m_pQuestionItemDisplay )
	{
		m_pQuestionItemDisplay->KEEP_STOP ();
	}
}

void	CInnerInterface::SET_CONFT_CONFIRM_VISIBLE ( bool bVisible )
{
	if ( bVisible )	ShowGroupTop( CONFT_CONFIRM );
	else			HideGroup( CONFT_CONFIRM );
}

void	CInnerInterface::SET_CONFT_CONFIRM_PERCENT ( float fPercent )
{
	if( m_pConftConfirm )
	{
		m_pConftConfirm->SetPercent ( fPercent );
	}
}

HRESULT CInnerInterface::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	TEXTURE_PRE_LOAD ( pd3dDevice );
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	{
		m_pGlobalRankingWindow = new CGlobalRankingWindow;
		m_pGlobalRankingWindow->Create(GLOBAL_RANKING_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pGlobalRankingWindow->CreateBaseWindowLightGray("BASIC_WINDOW", NULL);
		m_pGlobalRankingWindow->CreateSubControl();
		m_pGlobalRankingWindow->SetAlignFlag(UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X);
		RegisterControl(m_pGlobalRankingWindow, true);
		ShowGroupFocus(GLOBAL_RANKING_WINDOW);

		CBasicButton* pGlobalRankingButton = new CBasicButton;
		pGlobalRankingButton->Create(GLOBAL_RANKING_BUTTON, "GLOBAL_RANKING_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM);
		pGlobalRankingButton->CreateFlip("GLOBAL_RANKING_BUTTON_F", CBasicButton::CLICK_FLIP);
		pGlobalRankingButton->SetUseDynamic(TRUE);
		pGlobalRankingButton->SetUseGlobalAction(TRUE);
		RegisterControl(pGlobalRankingButton);
		ShowGroupBottom(GLOBAL_RANKING_BUTTON);
	}
	/////////////////////////////////////////////////////////////////////////////
	
	/*woe Arc Development 08-06-2024*/
	{
		m_pWoeRanking = new CWoeRanking;
		m_pWoeRanking->Create(WOE_RANKING_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pWoeRanking->CreateBaseWindowLightGray("WOE_RANKING_WINDOW", "Woe Dashboard");
		m_pWoeRanking->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pWoeRanking->CreateSubControl();
		RegisterControl(m_pWoeRanking);
		ShowGroupFocus(WOE_RANKING_WINDOW);
	}
	{
		m_pPVPWoeTowerCapture = new CPVPWoeTowerCapture;
		m_pPVPWoeTowerCapture->Create(PVP_WOE_TOWER_CAPTURE, "WOE_TOWER_CAPTURE", UI_FLAG_CENTER_X);
		m_pPVPWoeTowerCapture->CreateSubControl();
		RegisterControl(m_pPVPWoeTowerCapture);
		ShowGroupBottom(PVP_WOE_TOWER_CAPTURE);
		HideGroup(PVP_WOE_TOWER_CAPTURE);

		m_pPVPWoeTowerCaptureDummy = new CUIControl;
		m_pPVPWoeTowerCaptureDummy->Create(NO_ID, "WOE_TOWER_CAPTURE", UI_FLAG_CENTER_X);
		RegisterControl(m_pPVPWoeTowerCaptureDummy);
		m_pPVPWoeTowerCaptureDummy->SetVisibleSingle(FALSE);

		m_pRebirthDialogueWoe = new CPVPWoeRebirthDialogue;
		m_pRebirthDialogueWoe->Create(REBIRTH_DIALOGUE_WOE, "WOE_FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_BOTTOM);
		m_pRebirthDialogueWoe->CreateSubControl();
		RegisterControl(m_pRebirthDialogueWoe);
		ShowGroupBottom(REBIRTH_DIALOGUE_WOE);
		HideGroup(REBIRTH_DIALOGUE_WOE);

		m_pPVPWoeTowerProgress = new CPVPWoeTowerProgress;
		m_pPVPWoeTowerProgress->Create(PVP_WOE_TOWER_PROGRESS, "WOE_TOWER_PROGRESS");
		m_pPVPWoeTowerProgress->CreateSubControl();
		RegisterControl(m_pPVPWoeTowerProgress);
		ShowGroupBottom(PVP_WOE_TOWER_PROGRESS);
		HideGroup(PVP_WOE_TOWER_PROGRESS);
	}

	// pandora kuno
	{
		if ( RPARAM::bFeaturePandora )
		{
			m_pPandoraBoxWindow = new CPandoraBoxWindow ( &GLGaeaClient::GetInstance().GetGaeaClient(), this );
			m_pPandoraBoxWindow->Create ( PANDORA_BOX, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pPandoraBoxWindow->CreateBaseWindowLightGray ( "PANDORA_WINDOW", (char*)"Pandora's Box" );
			m_pPandoraBoxWindow->CreateSubControl ();
			m_pPandoraBoxWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( m_pPandoraBoxWindow );
			ShowGroupFocus ( PANDORA_BOX );
			HideGroup ( PANDORA_BOX );
		}
	}

	{
		m_pPandoraBoxButton = new CPandoraBoxButton;
		m_pPandoraBoxButton->Create ( PANDORA_BUTTON, "PANDORA_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pPandoraBoxButton->CreateSubControl ();
		RegisterControl ( m_pPandoraBoxButton );
		//ShowGroupBottom ( PANDORA_BUTTON );
		if ( RPARAM::bFeaturePandora )
			ShowGroupBottom ( PANDORA_BUTTON );
		else
			HideGroup ( PANDORA_BUTTON );
	}

	{

		m_pKeySettingWindow = new CKeySettingWindow;
		m_pKeySettingWindow->Create ( KEY_SETTING_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pKeySettingWindow->CreateBaseWindowLightGray ( "KEY_SETTING_WINDOW", (char*)ID2GAMEWORD("KEYSETTING_WINDOW_NAME_STATIC") );
		m_pKeySettingWindow->CreateSubControl ();
		m_pKeySettingWindow->SetControlNameEx ( "게임키설정창" );
		m_pKeySettingWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pKeySettingWindow, true);
		ShowGroupFocus ( KEY_SETTING_WINDOW );
	}

	{

		m_pChatMacroWindow = new CChatMacroWindow;
		m_pChatMacroWindow->Create ( CHATMACRO_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChatMacroWindow->CreateBaseWindowLightGray ( "CHATMACRO_WINDOW", (char*)ID2GAMEWORD("CHATMACRO_WINDOW_NAME_STATIC") );
		m_pChatMacroWindow->CreateSubControl ();
		m_pChatMacroWindow->SetControlNameEx ( "채팅매크로" );
		m_pChatMacroWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pChatMacroWindow, true );
		ShowGroupFocus ( CHATMACRO_WINDOW );
		
	}

	{
		m_pNameDisplayMan = new CNameDisplayMan;
		m_pNameDisplayMan->Create ( NAME_DISPLAY_MAN, "NAME_DISPLAY_MAN" );
		RegisterControl ( m_pNameDisplayMan );
		ShowGroupBottom ( NAME_DISPLAY_MAN );
		HideGroup( NAME_DISPLAY_MAN );

		m_pPrivateMarketShowMan = new CPrivateMarketShowMan;
		m_pPrivateMarketShowMan->Create ( PRIVATE_MARKET_SHOW_MAN, "NAME_DISPLAY_MAN" );
		RegisterControl ( m_pPrivateMarketShowMan );
		ShowGroupBottom ( PRIVATE_MARKET_SHOW_MAN );
	}

	{
		m_pTargetInfoDisplay = new CTargetInfoDisplay;
		m_pTargetInfoDisplay->Create ( TARGETINFO_DISPLAY, "TARGETINFO_DISPLAY" );
		m_pTargetInfoDisplay->CreateSubControl ();
		RegisterControl ( m_pTargetInfoDisplay );
		ShowGroupBottom ( TARGETINFO_DISPLAY );
		HideGroup( TARGETINFO_DISPLAY );
	}

	{
		m_pDamageDisplayMan = new CDamageDisplayMan;
		m_pDamageDisplayMan->Create ( DAMAGE_MAN, "DAMAGE_MAN" );
		m_pDamageDisplayMan->CreateSubControl ();
		RegisterControl ( m_pDamageDisplayMan );
		ShowGroupBottom ( DAMAGE_MAN );
	}

	{
        m_pHeadChatDisplayMan = new CHeadChatDisplayMan;
		m_pHeadChatDisplayMan->Create ( HEADCHAT_MAN, "HEADCHAT_MAN" );
		RegisterControl ( m_pHeadChatDisplayMan );
		ShowGroupBottom ( HEADCHAT_MAN );
	}

	{
		m_pUILeftTopGroup = new CUILeftTopGroup;
		m_pUILeftTopGroup->CreateEx( LEFTTOP_CONTROL_GROUP, "LEFTTOP_CONTROL_GROUP" );
		m_pUILeftTopGroup->CreateSubControl ();
		RegisterControl ( m_pUILeftTopGroup );
		ShowGroupBottom ( LEFTTOP_CONTROL_GROUP );
	}

	{

		const EMCHARCLASS emCharClass = DxGlobalStage::GetInstance().GetGameStage()->GetCharJoinData().m_CharData2.m_emClass;
		const int nClassType = CharClassToIndex ( emCharClass );

		CSkillTimeDisplay* pSkillTimeDisplay = new CSkillTimeDisplay;	

		// 극강부와 일반 캐릭터를 구분해서 버프UI 위치 조정
		if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
		{
			pSkillTimeDisplay->Create ( SKILL_TIME_DISPLAY, "SKILL_TIME_DISPLAY_EXTREME" );
		}
		else
		{
			pSkillTimeDisplay->Create ( SKILL_TIME_DISPLAY, "SKILL_TIME_DISPLAY" );
		}
		
		pSkillTimeDisplay->CreateSubControl ();
		RegisterControl ( pSkillTimeDisplay );
		ShowGroupBottom ( SKILL_TIME_DISPLAY );


		// 극강부 무기 표시 UI 
		if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
		{
			CWeaponDisplay* pWeaponDisplay = new CWeaponDisplay;
			pWeaponDisplay->Create ( WEAPON_DISPLAY, "WEAPON_DISPLAY" );
			pWeaponDisplay->CreateSubControl ();
			RegisterControl ( pWeaponDisplay );
			ShowGroupBottom ( WEAPON_DISPLAY );
		}

	}

	{	/*itemfood system, Juver, 2017/05/25 */
		CFITemFactTimeDisplay* pFItemFactTimeDisplay = new CFITemFactTimeDisplay;	
		pFItemFactTimeDisplay->Create ( FITEMFACT_TIME_DISPLAY, "LUNCHBOX_TIME_DISPLAY", UI_FLAG_RIGHT );
		pFItemFactTimeDisplay->CreateSubControl ();
		RegisterControl ( pFItemFactTimeDisplay );
		ShowGroupBottom ( FITEMFACT_TIME_DISPLAY );
	}

	{	/*system buffs, Juver, 2017/09/05 */
		CSystemBuffTimeDisplay* pSystemBuffTimeDisplay = new CSystemBuffTimeDisplay;	
		pSystemBuffTimeDisplay->Create ( SYSTEM_BUFF_TIME_DISPLAY, "SYSTEMBUFF_TIME_DISPLAY", UI_FLAG_RIGHT );
		pSystemBuffTimeDisplay->CreateSubControl ();
		RegisterControl ( pSystemBuffTimeDisplay );
		ShowGroupBottom ( SYSTEM_BUFF_TIME_DISPLAY );
	}

	{
		m_pMiniMap = new CMiniMap;
		m_pMiniMap->Create ( MINIMAP, "BASIC_MINIMAP", UI_FLAG_RIGHT );
		m_pMiniMap->CreateSubControl ();
		RegisterControl ( m_pMiniMap );
		ShowGroupBottom ( MINIMAP );
	}

	{
		/*game stats, Juver, 2017/06/22 */
		m_pGameStats = new CGameStats;
		m_pGameStats->Create ( GAMESTATS_DISPLAY, "GAMESTATS_DISPLAY", UI_FLAG_CENTER_X );
		m_pGameStats->CreateSubControl ();
		RegisterControl ( m_pGameStats );
		ShowGroupBottom ( GAMESTATS_DISPLAY );
	}

	{
		m_pQuestHelper = new CQuestHelper;
		m_pQuestHelper->Create ( QUEST_HELPER_DISPLAY, "QUEST_HELPER_DISPLAY", UI_FLAG_RIGHT );
		m_pQuestHelper->CreateSubControl ();
		RegisterControl ( m_pQuestHelper );
		ShowGroupBottom ( QUEST_HELPER_DISPLAY );
	}

	{
		m_pCdmRankingDisplay = new CCdmRankingDisplay;
		m_pCdmRankingDisplay->Create ( CDM_RANKING_DISPLAY, "PVP_CDM_RANKING_DISPLAY", UI_FLAG_RIGHT );
		m_pCdmRankingDisplay->CreateSubControl ();
		RegisterControl ( m_pCdmRankingDisplay );
		ShowGroupBottom ( CDM_RANKING_DISPLAY );	
		HideGroup( CDM_RANKING_DISPLAY );

	}

	{
		m_pTGRankingDisplay = new CTGRankingDisplay;
		m_pTGRankingDisplay->Create ( TG_RANKING_DISPLAY, "PVP_CDM_RANKING_DISPLAY", UI_FLAG_RIGHT );
		m_pTGRankingDisplay->CreateSubControl ();
		RegisterControl ( m_pTGRankingDisplay );
		ShowGroupBottom ( TG_RANKING_DISPLAY );	
		HideGroup( TG_RANKING_DISPLAY );
	}


	{
		m_pBattleRankingDisplay = new CBattleRankingDisplay;
		m_pBattleRankingDisplay->Create(BATTLE_RANKING_DISPLAY, "BASIC_WINDOW", UI_FLAG_RIGHT);
		m_pBattleRankingDisplay->CreateSubControl();
		RegisterControl(m_pBattleRankingDisplay);
		ShowGroupBottom(BATTLE_RANKING_DISPLAY);
		HideGroup(BATTLE_RANKING_DISPLAY);
	}

	{
		CBasicButton* pMiniPartyOpen = new CBasicButton;
		pMiniPartyOpen->Create ( MINIPARTY_OPEN, "MINIPARTY_OPEN", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pMiniPartyOpen->CreateFlip ( "MINIPARTY_OPEN_F", CBasicButton::CLICK_FLIP );
		pMiniPartyOpen->SetUseDynamic ( TRUE );
		pMiniPartyOpen->SetUseGlobalAction ( TRUE );
		RegisterControl ( pMiniPartyOpen );
		ShowGroupBottom ( MINIPARTY_OPEN );

		m_pQuestAlarm = new CQuestAlarm;
		m_pQuestAlarm->Create ( QUEST_ALARM, "QUEST_ALARM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pQuestAlarm->CreateSubControl ();
		RegisterControl ( m_pQuestAlarm );
		ShowGroupBottom ( QUEST_ALARM );
		HideGroup ( QUEST_ALARM );

		CEscMenuOpen* pEscMenuOpen = new CEscMenuOpen;
		pEscMenuOpen->Create ( ESCMENU_OPEN, "ESCMENU_OPEN", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pEscMenuOpen->CreateSubControl ();
		RegisterControl ( pEscMenuOpen );		
		ShowGroupBottom ( ESCMENU_OPEN );

		m_pQBoxButton = new CQBoxButton;
		m_pQBoxButton->Create ( QBOX_BUTTON, "QBOX_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pQBoxButton->CreateSubControl ();
		RegisterControl ( m_pQBoxButton );		
		ShowGroupBottom ( QBOX_BUTTON );

		/*pet status, Juver, 2017/07/30 */
		m_pPetStatus = new CPetStatus;
		m_pPetStatus->Create ( PET_STATUS_INFO, "PET_STATUS_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pPetStatus->CreateSubControl ();
		RegisterControl ( m_pPetStatus );		
		ShowGroupBottom ( PET_STATUS_INFO );
		HideGroup( PET_STATUS_INFO );

		/*vehicle status, Juver, 2017/07/30 */
		m_pVehicleStatus = new CVehicleStatus;
		m_pVehicleStatus->Create ( VEHICLE_STATUS_INFO, "VEHICLE_STATUS_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pVehicleStatus->CreateSubControl ();
		RegisterControl ( m_pVehicleStatus );		
		ShowGroupBottom ( VEHICLE_STATUS_INFO );
		HideGroup( VEHICLE_STATUS_INFO );

		/* Competition UI, Juver, 2017/08/24 */
		/*m_pPVPCompetitionButton = new CPVPCompetitionButton;
		m_pPVPCompetitionButton->Create ( PVP_COMPETITION_BUTTON, "PVP_COMPETITION_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pPVPCompetitionButton->CreateSubControl ();
		RegisterControl ( m_pPVPCompetitionButton );
		ShowGroupBottom ( PVP_COMPETITION_BUTTON );*/

		/*product item, Juver, 2017/10/15 */
		m_pProductButton = new CProductButton;
		m_pProductButton->Create ( PRODUCT_BUTTON, "RAN_PRODUCT_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pProductButton->CreateSubControl ();
		RegisterControl ( m_pProductButton );
		
		if ( RPARAM::bProductIcon )
			ShowGroupBottom ( PRODUCT_BUTTON );
		else
			HideGroup ( PRODUCT_BUTTON );

		/*student record ui, Juver, 2017/10/30 */
		m_pStudentRecordButton = new CStudentRecordButton;
		m_pStudentRecordButton->Create ( STUDENT_RECORD_BUTTON, "RAN_STUDENTRECORD_NOTIFY_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pStudentRecordButton->CreateSubControl ();
		RegisterControl ( m_pStudentRecordButton );

		if ( RPARAM::bStudentRecord )
			ShowGroupBottom ( STUDENT_RECORD_BUTTON );
		else
			HideGroup ( STUDENT_RECORD_BUTTON );

		
		///* Codex, Archie 02/16/24 */
		//m_pStudentCodexButton = new CStudentCodexButton;
		//m_pStudentCodexButton->Create ( STUDENT_CODEX_BUTTON, "STUDENT_CODEX_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		//m_pStudentCodexButton->CreateSubControl ();
		//RegisterControl ( m_pStudentCodexButton );
		//ShowGroupBottom ( STUDENT_CODEX_BUTTON );


// #ifndef CH_PARAM // 중국 인터페이스 변경
		CBasicButton* pGameMenuOpen = new CBasicButton;
		pGameMenuOpen->Create ( GAME_MENU_OPEN_BUTTON, "GAME_MENU_OPEN_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pGameMenuOpen->CreateFlip ( "GAME_MENU_OPEN_BUTTON_F", CBasicButton::MOUSEIN_FLIP );
		pGameMenuOpen->SetUseGlobalAction ( TRUE );
		RegisterControl ( pGameMenuOpen );
		ShowGroupBottom ( GAME_MENU_OPEN_BUTTON );
		HideGroup( GAME_MENU_OPEN_BUTTON );
// #endif

		m_pGameMenu = new CBasicGameMenu;
		m_pGameMenu->Create ( GAME_MENU, "GAME_MENU", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pGameMenu->CreateSubControl ();
		RegisterControl ( m_pGameMenu );
		ShowGroupBottom ( GAME_MENU );
		{
			// 최초 캐릭터의 달리기 모드를 설정한다.
			BOOL bRun = GLGaeaClient::GetInstance().GetCharacter()->IsRunMode();
			m_pGameMenu->SetFlipRunButton( bRun );
		}
	}

	{	/*modern skill tray, Juver, 2018/08/21 */
		m_pSkillTrayTabMini = new CSkillTrayTabMini;
		m_pSkillTrayTabMini->CreateEx( QUICK_SKILL_TRAY_TAB_MINI, "QUICK_SKILL_TRAY_TAB_MINI" );
		m_pSkillTrayTabMini->CreateSubControl();
		RegisterControl( m_pSkillTrayTabMini );
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_MINI );
		HideGroup( QUICK_SKILL_TRAY_TAB_MINI );

		m_pSkillTrayTab = new CSkillTrayTab;
		m_pSkillTrayTab->CreateEx( QUICK_SKILL_TRAY_TAB_FULL, "QUICK_SKILL_TRAY_TAB_WINDOW" );
		m_pSkillTrayTab->CreateSubControl();
		RegisterControl( m_pSkillTrayTab );
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_FULL );

		m_pSkillTrayTabExMini = new CSkillTrayTabExMini;
		m_pSkillTrayTabExMini->CreateEx( QUICK_SKILL_TRAY_TAB_EX_MINI, "QUICK_SKILL_TRAY_TAB_MINI_H" );
		m_pSkillTrayTabExMini->CreateSubControl();
		RegisterControl( m_pSkillTrayTabExMini );
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_EX_MINI );
		HideGroup( QUICK_SKILL_TRAY_TAB_EX_MINI );

		m_pSkillTrayTabEx = new CSkillTrayTabEx;
		m_pSkillTrayTabEx->CreateEx( QUICK_SKILL_TRAY_TAB_EX_FULL, "QUICK_SKILL_TRAY_TAB_WINDOW_H" );
		m_pSkillTrayTabEx->CreateSubControl();
		RegisterControl( m_pSkillTrayTabEx );
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_EX_FULL );
		HideGroup( QUICK_SKILL_TRAY_TAB_EX_FULL );
	}

	{
		m_pChat = new CBasicChat;
		m_pChat->Create( BASIC_CHAT_BOX, "BASIC_CHAT_MINBOX", UI_FLAG_BOTTOM );
		m_pChat->CreateSubControl();
		m_pChat->SetProtectSizePoint();

		// 최초 채팅 박스의 크기를 최소 사이즈가 아니라 지정된 사이즈로 리사이즈 합니다.
		{
			WORD wAlignFlag = m_pChat->GetAlignFlag ();
			m_pChat->SetAlignFlag ( UI_FLAG_YSIZE );

			CUIControl TempControl;
			TempControl.Create ( 1, "BASIC_CHAT_BOX" );
			const UIRECT& rcParentOldPos = m_pChat->GetLocalPos ();
			const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
			m_pChat->AlignSubControl ( rcParentOldPos, rcParentNewPos );
			m_pChat->SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );

			m_pChat->SetAlignFlag ( wAlignFlag );
		}		
		RegisterControl ( m_pChat );
		ShowGroupFocus ( BASIC_CHAT_BOX );
	}

#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
	{	
		m_pCharacterWindow = new CCharacterWindow;
		m_pCharacterWindow->Create ( CHARACTER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
#if defined(KRT_PARAM)
		m_pCharacterWindow->CreateBaseWindowBlack ( "CHARACTER_WINDOW_SMS", (char*)ID2GAMEWORD("CHARACTER_NAME_STATIC") );
#else
		m_pCharacterWindow->CreateBaseWindowBlack ( "CHARACTER_WINDOW", (char*)ID2GAMEWORD("CHARACTER_NAME_STATIC") );
#endif
		m_pCharacterWindow->CreateSubControl ();
		m_pCharacterWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pCharacterWindow, true );
		ShowGroupFocus ( CHARACTER_WINDOW );
	}
#else
	{
		/*modern character window, Juver, 2018/03/12 */
		m_pCharacterWindow = new CModernCharacterWindow;
		m_pCharacterWindow->Create ( CHARACTER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCharacterWindow->CreateBaseWindowLightGray ( "MODERN_CHARACTER_WINDOW_MAIN", (char*)ID2GAMEWORD("MODERN_CHARACTER_WINDOW", 0 ) );
		m_pCharacterWindow->CreateSubControl ();
		m_pCharacterWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pCharacterWindow, true );
		ShowGroupFocus ( CHARACTER_WINDOW );
	}
#endif // 

	

	{
		CBasicHWOptionWindow* pOptionWindow = new CBasicHWOptionWindow;
		pOptionWindow->Create ( OPTION_HW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pOptionWindow->CreateBaseWindowLightGray ( "HWOPTION_WINDOW", (char*)ID2GAMEWORD("HWOPTION_WINDOW_NAME_STATIC") );
		pOptionWindow->CreateSubControl ();
		pOptionWindow->SetControlNameEx ( "옵션창" );
		pOptionWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( pOptionWindow, true );
		ShowGroupFocus ( OPTION_HW_WINDOW );
	}

	{
#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
		m_pCharacterSkillWindow = new CSkillWindow;
		m_pCharacterSkillWindow->Create ( SKILL_WINDOW,  "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCharacterSkillWindow->CreateBaseWindowLightGray ( "SKILL_WINDOW", (char*)ID2GAMEWORD("SKILL_NAME_STATIC") );
		m_pCharacterSkillWindow->CreateSubControl ();
		m_pCharacterSkillWindow->SetControlNameEx ( "CSkillWindow" );
		m_pCharacterSkillWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pCharacterSkillWindow, true );
		ShowGroupFocus ( SKILL_WINDOW );
#else
		m_pCharacterSkillWindow = new CModernSkillWindow;
		m_pCharacterSkillWindow->Create ( SKILL_WINDOW,  "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCharacterSkillWindow->CreateBaseWindowLightGray ( "RAN_CHARACTER_SKILL_WINDOW", (char*)ID2GAMEWORD("SKILL_NAME_STATIC") );
		m_pCharacterSkillWindow->CreateSubControl ();
		m_pCharacterSkillWindow->SetControlNameEx ( "CModernSkillWindow" );
		m_pCharacterSkillWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pCharacterSkillWindow, true );
		ShowGroupFocus ( SKILL_WINDOW );
#endif

	}

	{
		CBasicEscMenu* pEscMenu = new CBasicEscMenu;
		pEscMenu->Create ( ESC_MENU, "ESC_MENU", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pEscMenu->CreateSubControl ();
		RegisterControl ( pEscMenu );
		ShowGroupFocus ( ESC_MENU );
	}

	{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		m_pPartyWindow = new CPartyWindow;
		m_pPartyWindow->Create ( PARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPartyWindow->CreateBaseWindowLightGray ( "PARTY_WINDOW", (char*)ID2GAMEWORD("PARTY_NAME_STATIC") );
		m_pPartyWindow->CreateSubControl ();
		m_pPartyWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pPartyWindow, true );
		ShowGroupFocus ( PARTY_WINDOW );
#else
		/* modern party window, Juver, 2019/12/19 */
		m_pPartyWindow = new CModernPartyWindow;
		m_pPartyWindow->Create ( PARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPartyWindow->CreateBaseWindowLightGray ( "MODERN_PARTY_WINDOW_RENEWAL", (char*)ID2GAMEWORD("PARTY_NAME_STATIC") );
		m_pPartyWindow->CreateSubControl ();
		m_pPartyWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pPartyWindow, true );
		ShowGroupFocus ( PARTY_WINDOW );
#endif
	}

	{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		m_pMarketWindow = new CMarketWindow;
		m_pMarketWindow->Create ( MARKET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pMarketWindow->CreateBaseWindowLightGray ( "MARKET_WINDOW", (char*)ID2GAMEWORD("MARKET_NAME_STATIC") );
		m_pMarketWindow->CreateSubControl ();
		m_pMarketWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pMarketWindow, true );
		ShowGroupFocus ( MARKET_WINDOW );
#else
		/* modern market window, Juver, 2020/02/05 */
		m_pMarketWindow = new CModernMarketWindow;
		m_pMarketWindow->Create ( MARKET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pMarketWindow->CreateBaseWindowLightGray ( "MODERN_MARKET_WINDOW", (char*)ID2GAMEWORD("MARKET_NAME_STATIC") );
		m_pMarketWindow->CreateSubControl ();
		m_pMarketWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pMarketWindow, true );
		ShowGroupFocus ( MARKET_WINDOW );
#endif
	
	}

	
	{	//	인벤토리
#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )

		m_pInventoryWindow = new CInventoryWindow;
		m_pInventoryWindow->Create ( INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInventoryWindow->CreateBaseWindowLightGray ( "INVENTORY_WINDOW", (char*)ID2GAMEWORD("INVENTORY_NAME_STATIC") );
		m_pInventoryWindow->CreateSubControl ();
		m_pInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pInventoryWindow, true );
		ShowGroupFocus ( INVENTORY_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pInventoryWindow = new CEP7InventoryWindow;
		m_pInventoryWindow->Create ( INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pInventoryWindow->CreateBaseWindowLightGray ( "EP7_INVENTORY_WINDOW", (char*)ID2GAMEINTEXT("EP7_INVENTORY_WINDOW_TITLE") );
#else
		m_pInventoryWindow->CreateBaseWindowLightGray ( "EP7_INVENTORY_WINDOW2", (char*)ID2GAMEINTEXT("EP7_INVENTORY_WINDOW_TITLE") );
#endif

		m_pInventoryWindow->CreateSubControl ();
		m_pInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pInventoryWindow, true );
		ShowGroupFocus ( INVENTORY_WINDOW );

#else

		m_pInventoryWindow = new CModernInventoryWindow;
		m_pInventoryWindow->Create ( INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInventoryWindow->CreateBaseWindowLightGray ( "MODERN_INVENTORY_WINDOW", (char*)ID2GAMEWORD("MODERN_INVENTORY_WINDOW_TEXT", 0 ) );
		m_pInventoryWindow->CreateSubControl ();
		m_pInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pInventoryWindow, true );
		ShowGroupFocus ( INVENTORY_WINDOW );

#endif // 
	}
	

	{	//	거래
		m_pTradeWindow = new CTradeWindow;
		m_pTradeWindow->Create ( TRADE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTradeWindow->CreateBaseWindowLightGray ( "TRADE_WINDOW", (char*)ID2GAMEWORD("TRADE_NAME_STATIC") );
		m_pTradeWindow->CreateSubControl ();
		m_pTradeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTradeWindow, true );
		ShowGroupFocus ( TRADE_WINDOW );

#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pTradeInventoryWindow = new CTradeInventoryWindow;
		m_pTradeInventoryWindow->Create ( TRADEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTradeInventoryWindow->CreateBaseWindowLightGray ( "TRADEINVENTORY_WINDOW", (char*)ID2GAMEWORD("TRADEINVENTORY_NAME_STATIC") );
		m_pTradeInventoryWindow->CreateSubControl ();
		m_pTradeInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTradeInventoryWindow, true );
		ShowGroupFocus ( TRADEINVENTORY_WINDOW );
		HideGroup ( TRADEINVENTORY_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pTradeInventoryWindow = new CEP7TradeInventoryWindow;
		m_pTradeInventoryWindow->Create ( TRADEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pTradeInventoryWindow->CreateBaseWindowLightGray ( "EP7_TRADE_INVENTORY_WINDOW", (char*)ID2GAMEINTEXT("EP7_TRADE_INVENTORY_WINDOW_TITLE") );
#else
		m_pTradeInventoryWindow->CreateBaseWindowLightGray ( "EP7_TRADE_INVENTORY_WINDOW2", (char*)ID2GAMEINTEXT("EP7_TRADE_INVENTORY_WINDOW_TITLE") );
#endif

		m_pTradeInventoryWindow->CreateSubControl ();
		m_pTradeInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTradeInventoryWindow, true );
		ShowGroupFocus ( TRADEINVENTORY_WINDOW );
		HideGroup ( TRADEINVENTORY_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pTradeInventoryWindow = new CModernTradeInventoryWindow;
		m_pTradeInventoryWindow->Create ( TRADEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTradeInventoryWindow->CreateBaseWindowLightGray ( "MODERN_TRADE_INVENTORY_WINDOW", (char*)ID2GAMEINTEXT("MODERN_TRADE_INVENTORY_WINDOW_TEXT" ) );
		m_pTradeInventoryWindow->CreateSubControl ();
		m_pTradeInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTradeInventoryWindow, true );
		ShowGroupFocus ( TRADEINVENTORY_WINDOW );
		HideGroup ( TRADEINVENTORY_WINDOW );
#endif // 
		
	}

	{	//	창고
		m_pStorageWindow = new CStorageWindow;		
		m_pStorageWindow->Create ( STORAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStorageWindow->CreateBaseWindowLightGray ( "STORAGE_WINDOW", (char*)ID2GAMEWORD("STORAGE_NAME_STATIC") );
		m_pStorageWindow->CreateSubControl ();
		m_pStorageWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pStorageWindow, true );
		ShowGroupFocus ( STORAGE_WINDOW );
	}

	{	//	창고
		m_pClubStorageWindow = new CClubStorageWindow;		
		m_pClubStorageWindow->Create ( CLUB_STORAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubStorageWindow->CreateBaseWindowLightGray ( "CLUB_STORAGE_WINDOW", (char*)ID2GAMEWORD("CLUB_STORAGE_NAME_STATIC") );
		m_pClubStorageWindow->CreateSubControl ();
		m_pClubStorageWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pClubStorageWindow, true );
		ShowGroupFocus ( CLUB_STORAGE_WINDOW );
	}


	{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		//	미니 파티
		m_pMiniPartyWindow = new CMiniPartyWindow;
		m_pMiniPartyWindow->Create ( MINIPARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pMiniPartyWindow->CreateBaseWindowLightGray ( "MINIPARTY_WINDOW", (char*)ID2GAMEWORD("MINIPARTY_NAME_STATIC") );
		m_pMiniPartyWindow->CreateSubControl ();
		m_pMiniPartyWindow->SetAlignFlag ( UI_FLAG_BOTTOM | UI_FLAG_RIGHT );
		RegisterControl ( m_pMiniPartyWindow, true );
		ShowGroupFocus ( MINIPARTY_WINDOW );
#else
		/* modern party window, Juver, 2019/12/19 */
		m_pMiniPartyWindow = new CModernMiniPartyWindow;
		m_pMiniPartyWindow->Create ( MINIPARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pMiniPartyWindow->CreateBaseWindowLightGray ( "MODERN_MINI_PARTY_WINDOW", (char*)ID2GAMEWORD("MINIPARTY_NAME_STATIC") );
		m_pMiniPartyWindow->CreateSubControl ();
		m_pMiniPartyWindow->SetAlignFlag ( UI_FLAG_BOTTOM | UI_FLAG_RIGHT );
		RegisterControl ( m_pMiniPartyWindow, true );
		ShowGroupFocus ( MINIPARTY_WINDOW );
#endif
	}


	{	//	모달
//		if ( 1 || "테스트" )
		{
			m_pModalWindow = new CModalWindow;
			m_pModalWindow->Create ( MODAL_WINDOW, "BASIC_MODAL_WINDOW", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			m_pModalWindow->CreateSubControl ();
			RegisterControl ( m_pModalWindow );
			ShowGroupFocus ( MODAL_WINDOW );
		}
//		else
//		{
//			m_pModalWindow = new CModalWindow;
//			m_pModalWindow->Create ( MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
//			m_pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
//			m_pModalWindow->CreateSubControl ();
//			m_pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
//			RegisterControl ( m_pModalWindow );
////			m_pModalWindow[i] = pModalWindow;
//			ShowGroupFocus ( MODAL_WINDOW );
//
////			m_nModalWindowIndex = 0;
//		}
	}

	{
		m_pPtoPWindow = new CPtoPWindow;		
		m_pPtoPWindow->Create ( PTOPMENU_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
//		m_pPtoPWindow->Create ( PTOPMENU_WINDOW, "PTOPMENU_WINDOW" );		
		m_pPtoPWindow->CreateSubControl ();
		m_pPtoPWindow->ResetAlignFlag ();
		RegisterControl ( m_pPtoPWindow );
		ShowGroupFocus ( PTOPMENU_WINDOW );
	}

	{
#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
		{	
			m_pDialogueWindow = new CDialogueWindow;
			m_pDialogueWindow->Create ( DIALOGUE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pDialogueWindow->CreateBaseWindowLightGray ( "DIALOGUE_WINDOW", (char*)ID2GAMEWORD("DIALOGUE_WINDOW_STATIC") );
			m_pDialogueWindow->CreateSubControl ();
			m_pDialogueWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
			RegisterControl ( m_pDialogueWindow, true );
			ShowGroupFocus ( DIALOGUE_WINDOW );
		}
#else
		{
			/* modern dialogue window, Juver, 2020/02/03 */
			m_pDialogueWindow = new CModernDialogueWindow;
			m_pDialogueWindow->Create ( DIALOGUE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pDialogueWindow->CreateBaseWindowLightGray( "MODERN_DIALOGUE_WINDOW", (char*)ID2GAMEWORD("DIALOGUE_WINDOW_STATIC") );
			m_pDialogueWindow->CreateSubControl ();
			m_pDialogueWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
			RegisterControl ( m_pDialogueWindow, true );
			ShowGroupFocus ( DIALOGUE_WINDOW );
		}
#endif // 
	}
	
	{	//	스킬 옮기기
		m_pSkillWindowToTray = new CSkillWindowToTray;
		m_pSkillWindowToTray->Create ( SKILL_WINDOW_TO_TRAY, "SKILL_WINDOW_TO_TRAY" );
		m_pSkillWindowToTray->CreateSubControl ();
		RegisterControl ( m_pSkillWindowToTray );
		ShowGroupTop ( SKILL_WINDOW_TO_TRAY );
	}

	{
		m_pItemMove = new CItemMove;
		m_pItemMove->Create ( ITEM_MOVE, "ITEM_MOVE" );
		m_pItemMove->CreateSubControl ();
		RegisterControl ( m_pItemMove );
		ShowGroupTop ( ITEM_MOVE );
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	{
		m_pInfoDisplayMain = new CBasicVarTextBox;
		m_pInfoDisplayMain->Create ( INFO_DISPLAY_MAIN, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayMain->CreateSubControl ();
		m_pInfoDisplayMain->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplayMain );
		ShowGroupTop ( INFO_DISPLAY_MAIN );
	}

	{
		m_pInfoDisplaySub = new CBasicVarTextBox;
		m_pInfoDisplaySub->Create ( INFO_DISPLAY_SUB, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplaySub->CreateSubControl ();
		m_pInfoDisplaySub->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplaySub );
		ShowGroupTop ( INFO_DISPLAY_SUB );
	}

	{
		/*item link, Juver, 2017/07/31 */
		m_pInfoDisplayItemLink = new CBasicVarTextBoxItemLink;
		m_pInfoDisplayItemLink->Create ( INFO_DISPLAY_ITEM_LINK, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayItemLink->CreateSubControl ();
		m_pInfoDisplayItemLink->ShowCloseButton( TRUE );
		m_pInfoDisplayItemLink->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplayItemLink );
		ShowGroupTop ( INFO_DISPLAY_ITEM_LINK );
	}
#else
	{
		/* modern info tooltip, Juver, 2020/04/09 */
		m_pInfoDisplayMain = new CBasicVarTextBoxEx;
		m_pInfoDisplayMain->Create ( INFO_DISPLAY_MAIN, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayMain->CreateSubControl ();
		m_pInfoDisplayMain->SetTextAlign(TEXT_ALIGN_LEFT);
		m_pInfoDisplayMain->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplayMain );
		ShowGroupTop ( INFO_DISPLAY_MAIN );

		m_pInfoDisplaySub = new CBasicVarTextBoxEx;
		m_pInfoDisplaySub->Create ( INFO_DISPLAY_SUB, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplaySub->CreateSubControl ();
		m_pInfoDisplaySub->SetTextAlign(TEXT_ALIGN_LEFT);
		m_pInfoDisplaySub->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplaySub );
		ShowGroupTop ( INFO_DISPLAY_SUB );

		/*item link, Juver, 2017/07/31 */
		m_pInfoDisplayItemLink = new CBasicVarTextBoxExItemLink;
		m_pInfoDisplayItemLink->Create ( INFO_DISPLAY_ITEM_LINK, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayItemLink->CreateSubControl ();
		m_pInfoDisplayItemLink->SetTextAlign(TEXT_ALIGN_LEFT);
		m_pInfoDisplayItemLink->ShowCloseButton( TRUE );
		m_pInfoDisplayItemLink->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplayItemLink );
		ShowGroupTop ( INFO_DISPLAY_ITEM_LINK );
	}
#endif

	{
		m_pAdminMessageDisplay = new CAdminMessageDisplay;
		m_pAdminMessageDisplay->Create ( ADMIN_MESSAGE_DISPLAY, "ADMIN_MESSAGE_DISPLAY" );
		m_pAdminMessageDisplay->CreateSubControl ();		
		RegisterControl ( m_pAdminMessageDisplay );
		ShowGroupTop ( ADMIN_MESSAGE_DISPLAY );

		CSimpleMessageMan*	pSimpleMessageMan = new CSimpleMessageMan;
		pSimpleMessageMan->Create ( SIMPLE_MESSAGE_MAN, "SIMPLE_MESSAGE_MAN" );
		pSimpleMessageMan->CreateSubControl ();		
		RegisterControl ( pSimpleMessageMan );
		ShowGroupBottom ( SIMPLE_MESSAGE_MAN );
		HideGroup ( SIMPLE_MESSAGE_MAN );
	}

	{	//	대련 신청 모달
		m_pConftModalWindow = new CConftModalWindow;
		m_pConftModalWindow->Create ( CONFT_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pConftModalWindow->CreateBaseModal ( "CONFT_MODAL_WINDOW");
		m_pConftModalWindow->CreateSubControl ();
		m_pConftModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pConftModalWindow->LoadDefaultCondition ();
		RegisterControl ( m_pConftModalWindow );
		ShowGroupFocus ( CONFT_MODAL_WINDOW );
	}

	{	//	파티 신청 모달
		m_pPartyModalWindow = new CPartyModalWindow;
		m_pPartyModalWindow->Create ( PARTY_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pPartyModalWindow->CreateBaseModal ( "PARTY_MODAL_WINDOW");
		m_pPartyModalWindow->CreateSubControl ();
		m_pPartyModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPartyModalWindow->LoadDefaultCondition ();
		RegisterControl ( m_pPartyModalWindow );
		ShowGroupFocus ( PARTY_MODAL_WINDOW );
	}

	{	//	물건 팔기 세팅 모달
		m_pPrivateMarketSellWindow = new CPrivateMarketSellWindow;
		m_pPrivateMarketSellWindow->Create ( PRIVATE_MARKET_SELL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pPrivateMarketSellWindow->CreateBaseModal ( "PRIVATE_MARKET_SELL_WINDOW");
		m_pPrivateMarketSellWindow->CreateSubControl ();
		m_pPrivateMarketSellWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
//		m_pPrivateMarketSellWindow->LoadDefaultCondition ();
		RegisterControl ( m_pPrivateMarketSellWindow );
		ShowGroupFocus ( PRIVATE_MARKET_SELL_WINDOW );
	}

	{	//	맵 이동
		m_pMapMoveDisplay = new CMapMoveDisplay;
		m_pMapMoveDisplay->Create ( MAPMOVE_DISPLAY, "BASIC_MAPMOVE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMapMoveDisplay->CreateSubControl ();
		RegisterControl ( m_pMapMoveDisplay );
		ShowGroupFocus ( MAPMOVE_DISPLAY );

		m_pMapMoveDisplayDummy = new CUIControl;
		m_pMapMoveDisplayDummy->Create ( MAPMOVE_DISPLAY_DUMMY, "BASIC_MAPMOVE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        RegisterControl ( m_pMapMoveDisplayDummy );

	}

	{
		/* multi gate out, Juver, 2020/11/19 */
		m_pMultiMapMoveDisplay = new CMultiMapMoveDisplay;
		m_pMultiMapMoveDisplay->Create ( MULTI_MAPMOVE_DISPLAY, "MULTIMAPMOVE_WINDOW_BOX", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMultiMapMoveDisplay->CreateSubControl ();
		RegisterControl ( m_pMultiMapMoveDisplay );
		ShowGroupFocus ( MULTI_MAPMOVE_DISPLAY );

		m_pMultiMapMoveDisplayDummy = new CUIControl;
		m_pMultiMapMoveDisplayDummy->Create ( MULTI_MAPMOVE_DISPLAY_DUMMY, "MULTIMAPMOVE_WINDOW_BOX", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pMultiMapMoveDisplayDummy );
	}

	{
		m_pSystemMessageWindow = new CSystemMessageWindow;
		m_pSystemMessageWindow->Create ( SYSTEM_MESSAGE_WINDOW, "BASIC_SYSTEM_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
		m_pSystemMessageWindow->CreateSubControl ();
		RegisterControl ( m_pSystemMessageWindow );
		ShowGroupTop ( SYSTEM_MESSAGE_WINDOW );
		HideGroup ( SYSTEM_MESSAGE_WINDOW );

		m_pSystemMessageWindowDummy = new CUIControl;
		m_pSystemMessageWindowDummy->Create ( SYSTEM_MESSAGE_WINDOW_DUMMY, "BASIC_SYSTEM_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
        RegisterControl ( m_pSystemMessageWindowDummy );
	}

	{	
		m_pBlockProgramAlarm = new CBlockProgramAlarm;
		m_pBlockProgramAlarm->Create ( BLOCK_PROGRAM_ALARM, "BLOCK_PROGRAM_ALARM" );		
		m_pBlockProgramAlarm->CreateSubControl ();
		m_pBlockProgramAlarm->ResetAlignFlag ();
		RegisterControl ( m_pBlockProgramAlarm, true );
		ShowGroupFocus ( BLOCK_PROGRAM_ALARM );

		m_pBlockProgramAlarmDummy = new CUIControl;
		m_pBlockProgramAlarmDummy->Create ( BLOCK_PROGRAM_ALARM_DUMMY, "BLOCK_PROGRAM_ALARM", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        RegisterControl ( m_pBlockProgramAlarmDummy );
	}

	{		
		m_pWaitServerDisplay = new CWaitServerDialogue;
		m_pWaitServerDisplay->Create ( WAITSERVER_DISPLAY, "WAITSERVER_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pWaitServerDisplay->CreateSubControl ();
		RegisterControl ( m_pWaitServerDisplay );
		ShowGroupFocus ( WAITSERVER_DISPLAY );
	}

	{	//	대련 카운트 매니저
		m_pConftDisplayMan = new CConftDisplayMan;
		m_pConftDisplayMan->Create ( CONFT_DISPLAY_MAN, "CONFT_DISPLAY_MAN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pConftDisplayMan->CreateSubControl ();
		RegisterControl ( m_pConftDisplayMan );
	}

	{
		// WebBrowser 생성되어 있으면 CHelpWindowWeb 생성, 없으면 CHelpWindow 생성 ( 준혁 )
		if( CCommonWeb::Get()->GetCreate() )
		{
			CHelpWindowWeb* pHelpWindow = new CHelpWindowWeb;
			pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pHelpWindow->CreateBaseWindowLightGray ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
			pHelpWindow->CreateWeb ();
			pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( pHelpWindow, true );
		}
		else
		{
			CHelpWindow* pHelpWindow = new CHelpWindow;
			pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pHelpWindow->CreateBaseWindowLightGray ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
			pHelpWindow->CreateSubControl ();
			pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( pHelpWindow, true );
		}
		ShowGroupFocus ( HELP_WINDOW );
	}

#if defined ( CH_PARAM ) || defined ( TH_PARAM ) || defined( PH_PARAM ) || defined ( JP_PARAM ) || defined( _RELEASED ) // 필리핀 아이템샵
	{
		if( CCommonWeb::Get()->GetCreate() )
		{
			CItemShopWindowWeb* pItemShopWindow = new CItemShopWindowWeb;
			pItemShopWindow->Create ( ITEMSHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pItemShopWindow->CreateBaseWindowLightGray ( "ITEMSHOP_WINDOW", (char*)ID2GAMEWORD("ITEMSHOP_WINDOW_STATIC") );
			pItemShopWindow->CreateWeb ();
			pItemShopWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( pItemShopWindow, true );			
		}
		ShowGroupFocus ( ITEMSHOP_WINDOW );
	}
#endif

	{		
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 )
		m_pQuestWindow = new CQuestWindow;
		m_pQuestWindow->Create ( QUEST_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pQuestWindow->CreateBaseWindowLightGray ( "QUEST_WINDOW", (char*)ID2GAMEWORD("QUEST_WINDOW_STATIC") );
		m_pQuestWindow->CreateSubControl ();
		m_pQuestWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pQuestWindow, true );
		ShowGroupFocus ( QUEST_WINDOW );
#else	
		m_pQuestWindow = new CModernQuestWindow;
		m_pQuestWindow->Create ( QUEST_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pQuestWindow->CreateBaseWindowLightGray ( "RAN_QUEST_WINDOW_LUXES", (char*)ID2GAMEWORD("QUEST_WINDOW_STATIC") );
		m_pQuestWindow->CreateSubControl ();
		m_pQuestWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pQuestWindow, true );
		ShowGroupFocus ( QUEST_WINDOW );
#endif
	}

	{
		m_pSimpleHP = new CSimpleHP;
		m_pSimpleHP->Create ( SIMPLE_HP, "SIMPLE_HP" );
		m_pSimpleHP->CreateSubControl ();
		RegisterControl ( m_pSimpleHP );
		ShowGroupBottom ( SIMPLE_HP );
		HideGroup ( SIMPLE_HP );

		m_pBasicInfoView = new CBasicInfoView;
		m_pBasicInfoView->Create ( BASIC_INFO_VIEW, "BASIC_INFO_VIEW" );
		m_pBasicInfoView->CreateSubControl ();	
		RegisterControl ( m_pBasicInfoView, true );	
		ShowGroupFocus ( BASIC_INFO_VIEW );
	}

	{
		m_pBonusTimeGauge = new CBonusTimeGauge;
		m_pBonusTimeGauge->Create ( BONUSTIME_EVENT_GAUGE, "BONUSTIME_EVENT_GAUGE", UI_FLAG_RIGHT );
		m_pBonusTimeGauge->CreateSubControl ();
		RegisterControl ( m_pBonusTimeGauge );
		ShowGroupBottom ( BONUSTIME_EVENT_GAUGE );
		HideGroup ( BONUSTIME_EVENT_GAUGE );

		m_pBonusTimeDisplay = new CBonusTimeDisplay;
		m_pBonusTimeDisplay->Create ( BONUSTIME_EVENT_DISPLAY, "BONUSTIME_EVENT_DISPLAY");
		m_pBonusTimeDisplay->CreateSubControl ();
		RegisterControl ( m_pBonusTimeDisplay );
		ShowGroupTop ( BONUSTIME_EVENT_DISPLAY );
		HideGroup( BONUSTIME_EVENT_DISPLAY );
	}


	{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		m_pFriendWindow = new CFriendWindow;
		m_pFriendWindow->Create ( FRIEND_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pFriendWindow->CreateBaseWindowBlack ( "FRIEND_WINDOW", (char*)ID2GAMEWORD("FRIEND_WINDOW_NAME_STATIC") );
		m_pFriendWindow->CreateSubControl ();
		m_pFriendWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pFriendWindow, true );
		ShowGroupFocus ( FRIEND_WINDOW );
#else
		/* modern friend window, Juver, 2020/03/02 */
		m_pFriendWindow = new CModernFriendWindow;
		m_pFriendWindow->Create ( FRIEND_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pFriendWindow->CreateBaseWindowLightGray ( "MODERN_FRIEND_WINDOW", (char*)ID2GAMEWORD("FRIEND_WINDOW_NAME_STATIC") );
		m_pFriendWindow->CreateSubControl ();
		m_pFriendWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pFriendWindow, true );
		ShowGroupFocus ( FRIEND_WINDOW );
#endif
	}	

	{
		m_pClubWindow = new CClubWindow;
		m_pClubWindow->Create ( CLUB_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubWindow->CreateBaseWindowLightGray ( "CLUB_WINDOW", (char*)ID2GAMEWORD("CLUB_WINDOW_NAME_STATIC") );
		m_pClubWindow->CreateSubControl ();
		m_pClubWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pClubWindow, true );
		ShowGroupFocus ( CLUB_WINDOW );
	}

	{
		m_pSubMasterSet = new CSubMasterSet;
		m_pSubMasterSet->Create( SUBMASTER_MAKE, "SUBMASTER_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pSubMasterSet->CreateSubControl();
		RegisterControl ( m_pSubMasterSet );
		ShowGroupFocus ( SUBMASTER_MAKE );
	}

	{
		CReceiveNoteWindow* pReceiveNote = new CReceiveNoteWindow;
		pReceiveNote->Create( RECEIVE_NOTE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pReceiveNote->CreateBaseWindowLightGray( "RECEIVE_NOTE_WINDOW", (char*)ID2GAMEWORD("FRIEND_TAB_BUTTON", 5) );
		pReceiveNote->CreateSubControl();
		pReceiveNote->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		RegisterControl( pReceiveNote );
		ShowGroupFocus ( RECEIVE_NOTE_WINDOW );
	}

	{
		CWriteNoteWindow* pWirteNote = new CWriteNoteWindow;
		pWirteNote->Create( WRITE_NOTE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pWirteNote->CreateBaseWindowLightGray( "WRITE_NOTE_WINDOW", (char*)ID2GAMEWORD("FRIEND_TAB_BUTTON", 4) );
		pWirteNote->CreateSubControl();
		pWirteNote->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		RegisterControl( pWirteNote );
		ShowGroupFocus ( WRITE_NOTE_WINDOW );
	}

#if defined(KRT_PARAM)
	{
		CSMSSendWindow* pSMSSendWindow = new CSMSSendWindow;
		pSMSSendWindow->Create( SMS_SEND_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pSMSSendWindow->CreateBaseWindowLightGray( "SMS_SEND_WINDOW", (char*)ID2GAMEWORD("FRIEND_BUTTON", 12) );
		pSMSSendWindow->CreateSubControl();
		pSMSSendWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		RegisterControl( pSMSSendWindow );
		ShowGroupFocus ( SMS_SEND_WINDOW );
	}
#endif

	{
		CHairColorCard * pHairColorCard = new CHairColorCard;
		pHairColorCard->Create( HAIRCOLORCARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pHairColorCard->CreateBaseWindowLightGray( "HAIRCOLORCARD_WINDOW", NULL );
		pHairColorCard->CreateSubControl();
		pHairColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	// Note : 실제 윈도우 타이틀을 여기서 입력했다.
			// 타이틀 입력후 윈도우 사이즈가 변경되므로
			// 80이 넘는 스트링은 짤리는 현상이 생긴다.
			// 그래서 윈도우가 리사이즈 된 후에 타이틀을 셋팅하면 정상적으로 출력된다.
			pHairColorCard->SetTitleName( (char*)ID2GAMEWORD("HAIRCOLORCARD_WINDOW_TITLE") );
		}
		RegisterControl( pHairColorCard );
		//ShowGroupFocus( HAIRCOLORCARD_WINDOW );

		CPetColorCard * pPetColorCard = new CPetColorCard;
		pPetColorCard->Create( PETCOLORCARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetColorCard->CreateBaseWindowLightGray( "HAIRCOLORCARD_WINDOW", NULL );
		pPetColorCard->CreateSubControl();
		pPetColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pPetColorCard->SetTitleName( (char*)ID2GAMEWORD("PETCOLORCARD_WINDOW_TITLE") );
		RegisterControl( pPetColorCard );
		//ShowGroupFocus( PETCOLORCARD_WINDOW );
	}
	
	{
		CHairStyleCard * pHairStyleCard = new CHairStyleCard;
		pHairStyleCard->Create( HAIRSTYLECARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pHairStyleCard->CreateBaseWindowLightGray( "HAIRSTYLECARD_WINDOW", NULL );
		pHairStyleCard->CreateSubControl();
		pHairStyleCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	// Note : 실제 윈도우 타이틀을 여기서 입력했다.
			// 타이틀 입력후 윈도우 사이즈가 변경되므로
			// 80이 넘는 스트링은 짤리는 현상이 생긴다.
			// 그래서 윈도우가 리사이즈 된 후에 타이틀을 셋팅하면 정상적으로 출력된다.
			pHairStyleCard->SetTitleName( (char*)ID2GAMEWORD("HAIRSTYLECARD_WINDOW_TITLE") );
		}
		RegisterControl( pHairStyleCard );

		CGenderChangeWindow * pGenderChangeWindow = new CGenderChangeWindow;
		pGenderChangeWindow->Create( GENDER_CHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pGenderChangeWindow->CreateBaseWindowLightGray( "GENDER_CHANGE_WINDOW", NULL );
		pGenderChangeWindow->CreateSubControl();
		pGenderChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	
			pGenderChangeWindow->SetTitleName( (char*)ID2GAMEWORD("GENDER_CHANGE_WINDOW_TITLE") );
		}
		RegisterControl( pGenderChangeWindow );
		

		CPetStyleCard * pPetStyleCard = new CPetStyleCard;
		pPetStyleCard->Create( PETSTYLECARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetStyleCard->CreateBaseWindowLightGray( "HAIRSTYLECARD_WINDOW", NULL );
		pPetStyleCard->CreateSubControl();
		pPetStyleCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pPetStyleCard->SetTitleName( (char*)ID2GAMEWORD("PETSTYLECARD_WINDOW_TITLE") );
		RegisterControl( pPetStyleCard );
		//ShowGroupFocus( PETSTYLECARD_WINDOW );
	}

	{
		m_pLargeMapWindow = new CLargeMapWindow;
		m_pLargeMapWindow->Create ( LARGEMAP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pLargeMapWindow->CreateBaseWindowLightGray ( "LARGEMAP_WINDOW", (char*)ID2GAMEWORD("LARGEMAP_WINDOW_STATIC") );
		m_pLargeMapWindow->CreateSubControl ();
		m_pLargeMapWindow->SetProtectSizePoint ();
		m_pLargeMapWindow->SetAlignFlag ( UI_FLAG_RIGHT | UI_FLAG_TOP );
		RegisterControl ( m_pLargeMapWindow, true );
		ShowGroupFocus ( LARGEMAP_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemRebuildWindow = new CItemRebuild;
		m_pItemRebuildWindow->Create( ITEM_REBUILD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemRebuildWindow->CreateBaseWindowLightGray( "ITEM_REBUILD_WINDOW", NULL );
		m_pItemRebuildWindow->CreateSubControl();
		m_pItemRebuildWindow->ResetAlignFlag();
		m_pItemRebuildWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRebuildWindow->SetTitleName( ID2GAMEINTEXT("ITEMREBUILD_WINDOW_TITLE") );
		RegisterControl( m_pItemRebuildWindow, true );
		ShowGroupFocus( ITEM_REBUILD_WINDOW );
	}


	{
#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pRebuildInventoryWindow = new CRebuildInventoryWindow;
		m_pRebuildInventoryWindow->Create( REBUILDINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pRebuildInventoryWindow->CreateBaseWindowLightGray( "REBUILDINVENTORY_WINDOW", NULL );
		m_pRebuildInventoryWindow->CreateSubControl();
		m_pRebuildInventoryWindow->ResetAlignFlag();
		m_pRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebuildInventoryWindow->SetTitleName( ID2GAMEINTEXT("REBUILDINVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pRebuildInventoryWindow, true );
		ShowGroupFocus( REBUILDINVENTORY_WINDOW );
		HideGroup( REBUILDINVENTORY_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pRebuildInventoryWindow = new CEP7RebuildInventoryWindow;
		m_pRebuildInventoryWindow->Create( REBUILDINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pRebuildInventoryWindow->CreateBaseWindowLightGray( "EP7_REBUILD_INVENTORY_WINDOW", NULL );
#else
		m_pRebuildInventoryWindow->CreateBaseWindowLightGray( "EP7_REBUILD_INVENTORY_WINDOW2", NULL );
#endif

		m_pRebuildInventoryWindow->CreateSubControl();
		m_pRebuildInventoryWindow->ResetAlignFlag();
		m_pRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebuildInventoryWindow->SetTitleName( ID2GAMEINTEXT("EP7_REBUILD_INVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pRebuildInventoryWindow, true );
		ShowGroupFocus( REBUILDINVENTORY_WINDOW );
		HideGroup( REBUILDINVENTORY_WINDOW );
#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pRebuildInventoryWindow = new CModernRebuildInventoryWindow;
		m_pRebuildInventoryWindow->Create( REBUILDINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pRebuildInventoryWindow->CreateBaseWindowLightGray( "MODERN_REBUILD_INVENTORY_WINDOW", NULL );
		m_pRebuildInventoryWindow->CreateSubControl();
		m_pRebuildInventoryWindow->ResetAlignFlag();
		m_pRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebuildInventoryWindow->SetTitleName( ID2GAMEINTEXT("MODERN_REBUILD_INVENTORY_WINDOW_TEXT" ) );
		RegisterControl( m_pRebuildInventoryWindow, true );
		ShowGroupFocus( REBUILDINVENTORY_WINDOW );
		HideGroup( REBUILDINVENTORY_WINDOW );

#endif // 
	}

	{	// ITEMREBUILD_MARK
		m_pItemGarbageWindow = new CItemGarbage;
		m_pItemGarbageWindow->Create( ITEM_GARBAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemGarbageWindow->CreateBaseWindowLightGray( "ITEM_GARBAGE_WINDOW", NULL );
		m_pItemGarbageWindow->CreateSubControl();
		m_pItemGarbageWindow->ResetAlignFlag();
		m_pItemGarbageWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemGarbageWindow->SetTitleName( ID2GAMEINTEXT("ITEMGARBAGE_WINDOW_TITLE") );
		RegisterControl( m_pItemGarbageWindow, true );
		ShowGroupFocus( ITEM_GARBAGE_WINDOW );
	}


	{
#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pGarbageInventoryWindow = new CGarbageInventoryWindow;
		m_pGarbageInventoryWindow->Create( GARBAGEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pGarbageInventoryWindow->CreateBaseWindowLightGray( "GARBAGEINVENTORY_WINDOW", NULL );
		m_pGarbageInventoryWindow->CreateSubControl();
		m_pGarbageInventoryWindow->ResetAlignFlag();
		m_pGarbageInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pGarbageInventoryWindow->SetTitleName( ID2GAMEINTEXT("GARBAGEINVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pGarbageInventoryWindow, true );
		ShowGroupFocus( GARBAGEINVENTORY_WINDOW );
		HideGroup( GARBAGEINVENTORY_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pGarbageInventoryWindow = new CEP7GarbageInventoryWindow;
		m_pGarbageInventoryWindow->Create( GARBAGEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pGarbageInventoryWindow->CreateBaseWindowLightGray( "EP7_GARBAGE_INVENTORY_WINDOW", NULL );
#else
		m_pGarbageInventoryWindow->CreateBaseWindowLightGray( "EP7_GARBAGE_INVENTORY_WINDOW2", NULL );
#endif

		m_pGarbageInventoryWindow->CreateSubControl();
		m_pGarbageInventoryWindow->ResetAlignFlag();
		m_pGarbageInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pGarbageInventoryWindow->SetTitleName( ID2GAMEINTEXT("EP7_GARBAGE_INVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pGarbageInventoryWindow, true );
		ShowGroupFocus( GARBAGEINVENTORY_WINDOW );
		HideGroup( GARBAGEINVENTORY_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pGarbageInventoryWindow = new CModernGarbageInventoryWindow;
		m_pGarbageInventoryWindow->Create( GARBAGEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pGarbageInventoryWindow->CreateBaseWindowLightGray( "MODERN_GARBAGE_INVENTORY_WINDOW", NULL );
		m_pGarbageInventoryWindow->CreateSubControl();
		m_pGarbageInventoryWindow->ResetAlignFlag();
		m_pGarbageInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pGarbageInventoryWindow->SetTitleName( ID2GAMEINTEXT("MODERN_GARBAGE_INVENTORY_WINDOW_TEXT" ) );
		RegisterControl( m_pGarbageInventoryWindow, true );
		ShowGroupFocus( GARBAGEINVENTORY_WINDOW );
		HideGroup( GARBAGEINVENTORY_WINDOW );

#endif // 
	}

	

	{
	
		m_pPetRebirthDialogue = new CPetRebirthDialogue;	// Monster7j
		m_pPetRebirthDialogue->Create ( PET_REBIRTH_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pPetRebirthDialogue->CreateBaseWindowLightGray ( "PET_REBIRTH_DIALOGUE", (char*)ID2GAMEWORD("PET_REBIRTH_NAME_STATIC") );
		m_pPetRebirthDialogue->CreateSubControl ();
		RegisterControl ( m_pPetRebirthDialogue ,true);
		ShowGroupFocus ( PET_REBIRTH_DIALOGUE );
	}

	{
		m_pGambleBox = new CGambleBox;	// Monster7j
		m_pGambleBox->Create ( GAMBLE_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pGambleBox->CreateBaseWindowLightGray ( "GAMBLE_DIALOGUE", (char*)ID2GAMEWORD("GAMBLE_NAME_STATIC") );
		m_pGambleBox->CreateSubControl ();
		RegisterControl ( m_pGambleBox ,true);
		ShowGroupFocus ( GAMBLE_DIALOGUE );
	}
	
	{
		m_pGambleSelectBox = new CGambleSelectBox;	// Monster7j
		m_pGambleSelectBox->Create ( GAMBLE_SELECT_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pGambleSelectBox->CreateBaseWindowLightGray ( "GAMBLE_SELECT_DIALOGUE", (char*)ID2GAMEWORD("GAMBLE_NAME_STATIC") );
		m_pGambleSelectBox->CreateSubControl ();
		RegisterControl ( m_pGambleSelectBox ,true);
		ShowGroupFocus ( GAMBLE_SELECT_DIALOGUE );
	}

	{
		m_pGambleAgainBox = new CGambleAgainBox;	// Monster7j
		m_pGambleAgainBox->Create ( GAMBLE_AGAIN_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pGambleAgainBox->CreateBaseWindowLightGray ( "GAMBLE_AGAIN_DIALOGUE", (char*)ID2GAMEWORD("GAMBLE_NAME_STATIC") );
		m_pGambleAgainBox->CreateSubControl ();
		RegisterControl ( m_pGambleAgainBox ,true);
		ShowGroupFocus ( GAMBLE_AGAIN_DIALOGUE );
	}
	
	{
		m_pGambleAnimationBox = new CGambleAnimationBox;
		m_pGambleAnimationBox->CreateEx( GAMBLE_ANIMATION_DIALOGUE, "GAMBLE_ANIMATION_DIALOGUE" );
		m_pGambleAnimationBox->CreateSubControl();
		RegisterControl( m_pGambleAnimationBox );
		ShowGroupFocus ( GAMBLE_ANIMATION_DIALOGUE );
	}

	{
		m_pPetSkinMixImage = new CPetSkinMixImage;
		m_pPetSkinMixImage->CreateEx( PETSKIN_MIX_IMAGE_WINDOW, "PETSKIN_MIX_IMAGE_WINDOW" );
		m_pPetSkinMixImage->CreateSubControl();
		m_pPetSkinMixImage->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl( m_pPetSkinMixImage );
		ShowGroupFocus ( PETSKIN_MIX_IMAGE_WINDOW );
	}

	{
		m_pGambleResultOddBox = new CGambleResultOddBox;
		m_pGambleResultOddBox->CreateEx( GAMBLE_RESULT_ODD_DIALOGUE, "GAMBLE_RESULT_ODD_DIALOGUE" );
		m_pGambleResultOddBox->CreateSubControl();
		RegisterControl( m_pGambleResultOddBox );
		ShowGroupFocus ( GAMBLE_RESULT_ODD_DIALOGUE );
	}

	{
		m_pGambleResultEvenBox = new CGambleResultEvenBox;
		m_pGambleResultEvenBox->CreateEx( GAMBLE_RESULT_EVEN_DIALOGUE, "GAMBLE_RESULT_EVEN_DIALOGUE" );
		m_pGambleResultEvenBox->CreateSubControl();
		RegisterControl( m_pGambleResultEvenBox );
		ShowGroupFocus ( GAMBLE_RESULT_EVEN_DIALOGUE );
	}

	{
		m_pBusWindow = new CBusWindow;
		m_pBusWindow->Create ( BUS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBusWindow->CreateBaseWindowLightGray ( "BUS_WINDOW", (char*)ID2GAMEWORD("BUS_WINDOW_NAME_STATIC") );
		m_pBusWindow->CreateSubControl ();
		m_pBusWindow->ResetAlignFlag ();
		RegisterControl ( m_pBusWindow, true );
		ShowGroupFocus ( BUS_WINDOW );
	}

	{
		m_pAcademyConftDisplay = new CAcademyConftDisplay;
		m_pAcademyConftDisplay->Create ( ACADEMY_CONFT_DISPLAY, "ACADEMY_CONFT_DISPLAY", UI_FLAG_CENTER_X );
		m_pAcademyConftDisplay->CreateSubControl ();
		RegisterControl ( m_pAcademyConftDisplay );
		ShowGroupTop ( ACADEMY_CONFT_DISPLAY );
		HideGroup ( ACADEMY_CONFT_DISPLAY );
	}

	{	//	유료아이템 수령
		m_pItemBankWindow = new CItemBankWindow;		
		m_pItemBankWindow->Create ( ITEMBANK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemBankWindow->CreateBaseWindowLightGray ( "ITEMBANK_WINDOW", (char*)ID2GAMEWORD("ITEMBANK_NAME_STATIC") );
		m_pItemBankWindow->CreateSubControl ();
		m_pItemBankWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pItemBankWindow, true );
		ShowGroupFocus ( ITEMBANK_WINDOW );
	}

#if defined(VN_PARAM) //vietnamtest%%%
	{	//	베트남 탐닉 방지 인벤토리
		m_pVNGainSysInventory = new CVNGainSysInventory;		
		m_pVNGainSysInventory->Create ( VNGAINSYS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pVNGainSysInventory->CreateBaseWindowLightGray ( "VNGAINSYS_WINDOW", (char*)ID2GAMEWORD("VNGAINSYSTEM_NAME_STATIC") );
		m_pVNGainSysInventory->CreateSubControl ();
		m_pVNGainSysInventory->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pVNGainSysInventory, true );
		ShowGroupFocus ( VNGAINSYS_WINDOW );
//		HideGroup ( VNGAINSYS_WINDOW );
	}

	{	//	베트남 탐닉 방지 게이지
		m_pVNGainSysGauge = new CVNGainSysGauge;		
		m_pVNGainSysGauge->Create ( VNGAINSYS_GAUGE_WINDOW, "VNGAINSYS_GAUGE_WINDOW", UI_FLAG_RIGHT );
		m_pVNGainSysGauge->CreateSubControl ();
		RegisterControl ( m_pVNGainSysGauge );
		ShowGroupBottom ( VNGAINSYS_GAUGE_WINDOW );
//		ShowGroupFocus ( VNGAINSYS_GAUGE_WINDOW );
	}
#endif

	
	{	
		m_pPrivateMarketWindow = new CPrivateMarketWindow;		
		m_pPrivateMarketWindow->Create ( PRIVATE_MARKET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPrivateMarketWindow->CreateBaseWindowLightGray ( "PRIVATE_MARKET_WINDOW", (char*)ID2GAMEWORD("PRIVATE_MARKET_NAME_STATIC") );
		m_pPrivateMarketWindow->CreateSubControl ();
		m_pPrivateMarketWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pPrivateMarketWindow, true );
		ShowGroupFocus ( PRIVATE_MARKET_WINDOW );
		HideGroup ( PRIVATE_MARKET_WINDOW );
	}

	{
		m_pStorageChargeCard = new CStorageChargeCard;
		m_pStorageChargeCard->Create ( STORAGE_CHARGE_CARD, "STORAGE_CHARGE_CARD", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pStorageChargeCard->CreateSubControl ();
		RegisterControl ( m_pStorageChargeCard );
		ShowGroupFocus ( STORAGE_CHARGE_CARD );
	}

	{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		m_pRebirthDialogue = new CRebirthDialogue;
		m_pRebirthDialogue->Create ( REBIRTH_DIALOGUE, "REBIRTH_DIALOGUE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebirthDialogue->CreateSubControl ();
		RegisterControl ( m_pRebirthDialogue );
		ShowGroupFocus ( REBIRTH_DIALOGUE );
#else
		/* modern rebirth dialogue, Juver, 2020/03/31 */
		m_pRebirthDialogue = new CModernRebirthDialogue;
		m_pRebirthDialogue->Create ( REBIRTH_DIALOGUE, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pRebirthDialogue->CreateBaseWindowReviveWindow ( "MODERN_REBIRTH_WINDOW" );
		m_pRebirthDialogue->CreateSubControl ();
		m_pRebirthDialogue->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pRebirthDialogue, TRUE );
		ShowGroupFocus ( REBIRTH_DIALOGUE );
#endif
	}

	{
		CWarningMsg* pWarningMsg = new CWarningMsg;
		pWarningMsg->CreateEx( WARNING_MSG_WINDOW, "WARNING_MSG_WINDOW" );
		pWarningMsg->CreateSubControl();
		RegisterControl( pWarningMsg );
	}

	{
		m_pCountMsgWindow = new CCountMsg;
		m_pCountMsgWindow->Create( COUNT_MSG_WINDOW, "COUNT_MSG_WINDOW", UI_FLAG_CENTER_Y );
		m_pCountMsgWindow->CreateSubControl();
		RegisterControl( m_pCountMsgWindow );
	}

	{
		m_pPrivateMarketMake = new CPrivateMarketMake;
		m_pPrivateMarketMake->Create ( PRIVATE_MARKET_MAKE, "PRIVATE_MARKET_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPrivateMarketMake->CreateSubControl ();
		RegisterControl ( m_pPrivateMarketMake );
		ShowGroupFocus ( PRIVATE_MARKET_MAKE );		

		m_pClubMake = new CClubMake;
		m_pClubMake->Create ( CLUB_MAKE, "PRIVATE_MARKET_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pClubMake->CreateSubControl ();
		RegisterControl ( m_pClubMake );
		ShowGroupFocus ( CLUB_MAKE );		
	}

	{
		m_pQuestionItemDisplay = new CQuestionItemDisplay;		
		m_pQuestionItemDisplay->Create ( QUESTION_ITEM_DISPLAY, "QUESTION_ITEM_DISPLAY" );
		m_pQuestionItemDisplay->CreateSubControl ();
		RegisterControl ( m_pQuestionItemDisplay );
		ShowGroupTop ( QUESTION_ITEM_DISPLAY );
	}

	{
		m_pConftConfirm = new CConftConfirm;
		m_pConftConfirm->Create ( CONFT_CONFIRM, "CONFT_CONFIRM", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pConftConfirm->CreateSubControl ();
		RegisterControl ( m_pConftConfirm );
		ShowGroupTop ( CONFT_CONFIRM );
		HideGroup ( CONFT_CONFIRM );
	}

	{	//	펫
		m_pPetWindow = new CPetWindow;
		m_pPetWindow->Create ( PET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPetWindow->CreateBaseWindowLightGray ( "RN_PET_WINDOW", (char*)ID2GAMEWORD("PET_NAME_STATIC") );
		m_pPetWindow->CreateSubControl ();
		m_pPetWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPetWindow, true );
		ShowGroupFocus ( PET_WINDOW );

		CPetSkillWindow* pPetSkillWindow = new CPetSkillWindow;
		pPetSkillWindow->Create ( PET_SKILL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetSkillWindow->CreateBaseWindowLightGray ( "PET_SKILL_WINDOW", (char*)ID2GAMEWORD("PET_NAME_STATIC", 1) );
		pPetSkillWindow->CreateSubControl ();
		pPetSkillWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl( pPetSkillWindow, true );
		ShowGroupFocus( PET_SKILL_WINDOW );
	}

	{	// 탈것 
		
		m_pVehicleWindow = new CVehicleWindow;
		m_pVehicleWindow->Create ( VEHICLE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pVehicleWindow->CreateBaseWindowLightGray ( "RN_VEHICLE_WINDOW", (char*)ID2GAMEWORD("VEHICLE_NAME_STATIC",0 ) );
		m_pVehicleWindow->CreateSubControl ();
		m_pVehicleWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		RegisterControl ( m_pVehicleWindow, true );
		ShowGroupFocus ( VEHICLE_WINDOW );

	}


	{	// 아이템 검색
		m_pShopItemSearchWindow = new CShopItemSearchWindow;
		m_pShopItemSearchWindow->Create ( ITEM_SHOP_SEARCH_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pShopItemSearchWindow->CreateBaseWindowLightGray ( "ITEM_SHOP_SEARCH_WINDOW", (char*)ID2GAMEWORD("ITEM_SHOP_SEARCH_STATIC",0 ) );
		m_pShopItemSearchWindow->CreateSubControl ();
		m_pShopItemSearchWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pShopItemSearchWindow, true );
		ShowGroupFocus ( ITEM_SHOP_SEARCH_WINDOW );
	
		// 아이템 검색 결과
		m_pItemSearchResultWindow = new CItemSearchResultWindow;
		m_pItemSearchResultWindow->Create ( ITEM_SEARCH_RESULT_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemSearchResultWindow->CreateBaseWindowLightGray ( "ITEM_SEARCH_RESULT_WINDOW", (char*)ID2GAMEWORD("ITEM_SHOP_SEARCH_STATIC",0 ) );
		m_pItemSearchResultWindow->CreateSubControl ();
		m_pItemSearchResultWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pItemSearchResultWindow, true );
		ShowGroupFocus ( ITEM_SEARCH_RESULT_WINDOW );
	}


	{
		m_pBasicInfoViewDummy = new CUIControl;
        m_pBasicInfoViewDummy->Create ( NO_ID, "BASIC_INFO_VIEW" );
		RegisterControl ( m_pBasicInfoViewDummy );
		m_pBasicInfoViewDummy->SetVisibleSingle( FALSE );

		m_pLeftTopGroupDummy = new CUIControl;
//#ifndef CH_PARAM // 중국 인터페이스 변경
		m_pLeftTopGroupDummy->Create( NO_ID, "LEFTTOP_CONTROL_GROUP" );
//#else
//		m_pLeftTopGroupDummy->Create( NO_ID, "BASIC_QUICK_SKILL_SLOT" );
//#endif
		RegisterControl ( m_pLeftTopGroupDummy );
		m_pLeftTopGroupDummy->SetVisibleSingle( FALSE );

	}

/* // 중국 인터페이스 변경
#ifdef CH_PARAM
		m_pBasicPotionTrayDummy = new CUIControl;
		m_pBasicPotionTrayDummy->CreateEx( NO_ID, "LEFTTOP_CONTROL_GROUP" );
		RegisterControl ( m_pBasicPotionTrayDummy );
		m_pBasicPotionTrayDummy->SetVisibleSingle( FALSE );

		m_pQuestAlarmDummy = new CUIControl;
		m_pQuestAlarmDummy->Create( NO_ID, "QUEST_ALARM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		RegisterControl( m_pQuestAlarmDummy );
		m_pQuestAlarmDummy->SetVisibleSingle( FALSE );
#endif
*/

#if defined( TH_PARAM ) || defined( MYE_PARAM ) || defined(MY_PARAM)
	{	// 태국 사이버까페 마크
		m_pThaiCCafeMark = new CThaiCCafeMark;		
		m_pThaiCCafeMark->Create ( THAI_CCAFE_MARK, "THAI_CCAFE_MARK", UI_FLAG_RIGHT );
		m_pThaiCCafeMark->CreateSubControl ();
		RegisterControl ( m_pThaiCCafeMark );
		ShowGroupBottom ( THAI_CCAFE_MARK );	
	}
#endif

#if defined ( JP_PARAM ) || defined ( _RELEASED )
	{
		// ItemShopIcon
		m_pItemShopIconMan = new CItemShopIconMan;
		m_pItemShopIconMan->Create ( ITEM_SHOP_ICON_MAN, "ITEM_SHOP_ICON_MAN" );		
		RegisterControl ( m_pItemShopIconMan );
		ShowGroupBottom ( ITEM_SHOP_ICON_MAN );
	}
#endif 

	{
		m_pAttendanceBookWindow = new CAttendanceBookWindow;
		m_pAttendanceBookWindow->Create ( ATTENDANCE_BOOK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pAttendanceBookWindow->CreateBaseWindowLightGray ( "ATTENDANCE_BOOK_WINDOW", (char*)ID2GAMEWORD("ATTENDANCE_BOOK", 0) );
		m_pAttendanceBookWindow->CreateSubControl ();
		m_pAttendanceBookWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pAttendanceBookWindow, true );
		ShowGroupFocus ( ATTENDANCE_BOOK_WINDOW );	
	}


	{
		CFaceStyleCard * pFaceChangeWindow = new CFaceStyleCard;
		pFaceChangeWindow->Create( FACE_CHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pFaceChangeWindow->CreateBaseWindowLightGray( "FACESTYLECARD_WINDOW", NULL );
		pFaceChangeWindow->CreateSubControl();
		pFaceChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pFaceChangeWindow->SetTitleName( (char*)ID2GAMEWORD("FACE_CHANGE_WINDOW_TITLE") );
		
		RegisterControl( pFaceChangeWindow );
	}

	{
		m_pMapRequireCheckWindow = new CMapRequireCheck;
		m_pMapRequireCheckWindow->Create ( MAP_REQUIRE_CHECK, "MAP_REQUIRE_CHECK_DIALOGUE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMapRequireCheckWindow->CreateSubControl ();
		RegisterControl ( m_pMapRequireCheckWindow );
		ShowGroupFocus ( MAP_REQUIRE_CHECK );
	}


	{
		m_pClubBattleModalWindow = new CClubBattleModal;
		m_pClubBattleModalWindow->Create( CLUB_BATTLE_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubBattleModalWindow->CreateBaseWindowLightGray( "CLUB_BATTLE_MODAL_WINDOW", NULL );
		m_pClubBattleModalWindow->CreateSubControl();
		m_pClubBattleModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pClubBattleModalWindow->SetTitleName( (char*)ID2GAMEWORD("CLUB_BATTLE_MODAL_WINDOW_TITLE") );
		
		RegisterControl( m_pClubBattleModalWindow );

	}

	{
		m_pTaxiWindow = new CTaxiWindow;
		m_pTaxiWindow->Create ( TAXI_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTaxiWindow->CreateBaseWindowLightGray ( "TAXI_WINDOW", (char*)ID2GAMEWORD("TAXI_WINDOW_NAME_STATIC") );
		m_pTaxiWindow->CreateSubControl ();
		m_pTaxiWindow->ResetAlignFlag ();
		RegisterControl ( m_pTaxiWindow, true );
		ShowGroupFocus ( TAXI_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemMixWindow = new CItemMixWindow;
		m_pItemMixWindow->Create( ITEM_MIX_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixWindow->CreateBaseWindowLightGray( "ITEM_MIX_WINDOW", NULL );
		m_pItemMixWindow->CreateSubControl();
		m_pItemMixWindow->ResetAlignFlag();
		m_pItemMixWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixWindow->SetTitleName( ID2GAMEINTEXT("ITEM_MIX_WINDOW_TITLE") );
		RegisterControl( m_pItemMixWindow, true );
		ShowGroupFocus( ITEM_MIX_WINDOW );
	}


	{
#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pItemMixInvenWindow = new CItemMixInvenWindow;
		m_pItemMixInvenWindow->Create( ITEM_MIX_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixInvenWindow->CreateBaseWindowLightGray( "ITEM_MIX_INVEN_WINDOW", NULL );
		m_pItemMixInvenWindow->CreateSubControl();
		m_pItemMixInvenWindow->ResetAlignFlag();
		m_pItemMixInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixInvenWindow->SetTitleName( ID2GAMEINTEXT("ITEM_MIX_INVEN_WINDOW_TITLE") );
		RegisterControl( m_pItemMixInvenWindow, true );
		ShowGroupFocus( ITEM_MIX_INVEN_WINDOW );
		HideGroup( ITEM_MIX_INVEN_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pItemMixInvenWindow = new CEP7ItemMixInvenWindow;
		m_pItemMixInvenWindow->Create( ITEM_MIX_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pItemMixInvenWindow->CreateBaseWindowLightGray( "EP7_ITEMMIX_INVENTORY_WINDOW", NULL );
#else
		m_pItemMixInvenWindow->CreateBaseWindowLightGray( "EP7_ITEMMIX_INVENTORY_WINDOW2", NULL );
#endif

		m_pItemMixInvenWindow->CreateSubControl();
		m_pItemMixInvenWindow->ResetAlignFlag();
		m_pItemMixInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixInvenWindow->SetTitleName( ID2GAMEINTEXT("EP7_ITEMMIX_INVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pItemMixInvenWindow, true );
		ShowGroupFocus( ITEM_MIX_INVEN_WINDOW );
		HideGroup( ITEM_MIX_INVEN_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pItemMixInvenWindow = new CModernItemMixInvenWindow;
		m_pItemMixInvenWindow->Create( ITEM_MIX_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixInvenWindow->CreateBaseWindowLightGray( "MODERN_ITEMMIX_INVENTORY_WINDOW", NULL );
		m_pItemMixInvenWindow->CreateSubControl();
		m_pItemMixInvenWindow->ResetAlignFlag();
		m_pItemMixInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixInvenWindow->SetTitleName( ID2GAMEINTEXT("MODERN_ITEMMIX_INVENTORY_WINDOW_TEXT", 0) );
		RegisterControl( m_pItemMixInvenWindow, true );
		ShowGroupFocus( ITEM_MIX_INVEN_WINDOW );
		HideGroup( ITEM_MIX_INVEN_WINDOW );

#endif // 
	}

	

	{
		m_pGatherGauge = new CGatherGauge;
		m_pGatherGauge->Create ( GATHER_GAUGE, "GATHER_GAUGE", UI_FLAG_CENTER_X );
		m_pGatherGauge->CreateSubControl ();
		RegisterControl ( m_pGatherGauge );
		ShowGroupBottom ( GATHER_GAUGE );
		HideGroup ( GATHER_GAUGE );
	}

	{   /*npc shop, Juver, 2017/07/25 */
		m_pNPCShopWindow = new CNPCShopWindow;
		m_pNPCShopWindow->Create( NPC_SHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pNPCShopWindow->CreateBaseWindowLightGray( "NPCSHOP_WINDOW", NULL );
		m_pNPCShopWindow->CreateSubControl();
		m_pNPCShopWindow->ResetAlignFlag();
		m_pNPCShopWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pNPCShopWindow->SetTitleName( ID2GAMEWORD("NPC_SHOP_TEXT", 0) );
		RegisterControl( m_pNPCShopWindow, true );
		ShowGroupFocus( NPC_SHOP_WINDOW );
		HideGroup( NPC_SHOP_WINDOW );
	}

	{	/*item preview, Juver, 2017/07/27 */
		m_pItemPreviewWindow = new CItemPreviewWindow;
		m_pItemPreviewWindow->Create( ITEMPREVIEW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemPreviewWindow->CreateBaseWindowLightGray( "ITEMPREVIEW_WINDOW", NULL );
		m_pItemPreviewWindow->CreateSubControl ();
		m_pItemPreviewWindow->ResetAlignFlag ();
		m_pItemPreviewWindow->SetTitleName( ID2GAMEWORD("ITEMPREVIEW_TITLE", 0 ) );
		RegisterControl ( m_pItemPreviewWindow );
		ShowGroupFocus ( ITEMPREVIEW_WINDOW );
		HideGroup ( ITEMPREVIEW_WINDOW );
	}


	{	/*vehicle booster system, Juver, 2017/08/11 */
		m_pVehicleBoosterDisplay = new CVehicleBoosterDisplay;
		m_pVehicleBoosterDisplay->Create ( VEHICLE_BOOSTER_DISPLAY, "RN_BOOSTER_DISPLAY", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pVehicleBoosterDisplay->CreateSubControl ();
		RegisterControl ( m_pVehicleBoosterDisplay );
		ShowGroupBottom ( VEHICLE_BOOSTER_DISPLAY );
		HideGroup ( VEHICLE_BOOSTER_DISPLAY);
	}

	{	/*box contents, Juver, 2017/08/30 */
		m_pInfoBoxContents = new CInfoBoxContents;
		m_pInfoBoxContents->Create ( INFO_BOX_CONTENTS, "INFO_BOX_CONTENTS", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pInfoBoxContents->CreateSubControl ();		
		RegisterControl ( m_pInfoBoxContents );
		ShowGroupFocus ( INFO_BOX_CONTENTS );
		HideGroup ( INFO_BOX_CONTENTS );
	}

	{   
		/* Competition UI, Juver, 2017/08/24 */
		m_pPVPCompetitionNotify = new CPVPCompetitionNotify;
		m_pPVPCompetitionNotify->Create ( PVP_COMPETITION_NOTIFY, "PVP_COMPETITION_NOTIFY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPCompetitionNotify->CreateSubControl ();
		m_pPVPCompetitionNotify->SetControlPos( m_pPVPCompetitionButton );
		RegisterControl ( m_pPVPCompetitionNotify );
		ShowGroupTop ( PVP_COMPETITION_NOTIFY );
		HideGroup ( PVP_COMPETITION_NOTIFY );
		
		m_pPVPCompetitionWindow = new CPVPCompetitionWindow;
		m_pPVPCompetitionWindow->Create ( PVP_COMPETITION_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPCompetitionWindow->CreateBaseWindowLightGray ( "PVP_COMPETITION_WINDOW", (char*)ID2GAMEWORD("PVP_COMPETITION_WINDOW", 0 ) );
		m_pPVPCompetitionWindow->CreateSubControl ();
		m_pPVPCompetitionWindow->ResetAlignFlag();
		m_pPVPCompetitionWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPCompetitionWindow );
		ShowGroupFocus ( PVP_COMPETITION_WINDOW, true );
		HideGroup ( PVP_COMPETITION_WINDOW );

		m_pPVPCompetitionNotice = new CPVPCompetitionNotice;
		m_pPVPCompetitionNotice->Create ( PVP_COMPETITION_NOTICE, "PVP_COMPETITION_NOTICE", UI_FLAG_CENTER_X );
		m_pPVPCompetitionNotice->CreateSubControl ();		
		RegisterControl ( m_pPVPCompetitionNotice );
		ShowGroupTop ( PVP_COMPETITION_NOTICE );
		HideGroup ( PVP_COMPETITION_NOTICE );
	}
	
	/* PK Streak Sigaw - Jxyyy */
	{
		m_pPKStreakKillNotice = new CPKStreakKillNotice;
		m_pPKStreakKillNotice->Create ( PKSTREAK_KILL_NOTICE, "PKSTREAK_KILL_NOTICE", UI_FLAG_CENTER_X );
		m_pPKStreakKillNotice->CreateSubControl ();		
		RegisterControl ( m_pPKStreakKillNotice );
		ShowGroupTop ( PKSTREAK_KILL_NOTICE );
		HideGroup ( PKSTREAK_KILL_NOTICE );
	}

	{	
		m_pPKStreakShutdownNotice = new CPKStreakShutdownNotice;
		m_pPKStreakShutdownNotice->Create ( PKSTREAK_SHUTDOWN_NOTICE, "PKSTREAK_SHUTDOWN_NOTICE", UI_FLAG_CENTER_X );
		m_pPKStreakShutdownNotice->CreateSubControl ();		
		RegisterControl ( m_pPKStreakShutdownNotice );
		ShowGroupTop ( PKSTREAK_SHUTDOWN_NOTICE );
		HideGroup ( PKSTREAK_SHUTDOWN_NOTICE );
	}		
	/* PK Streak Sigaw - Jxyyy End */

	{
		m_pVoidMainNews = new VoidMainNews;
		m_pVoidMainNews->Create ( VOIDMAIN_NEWS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pVoidMainNews->CreateBaseWindowLightGray ( "VOIDMAIN_NEWS_WINDOW", "News and Guides" );
		m_pVoidMainNews->CreateSubControl ();
		m_pVoidMainNews->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pVoidMainNews, true );
		HideGroup ( VOIDMAIN_NEWS_WINDOW );
	}
	
	if ( RPARAM::bShowNextTyranny )
	{
		// PVP Next Battle UI - Jxyyy
		m_pPVPNextBattleAndWinner = new CPVPNextBattleAndWinner;
		m_pPVPNextBattleAndWinner->Create( PVP_TYRANNY_NEXT_AND_WINNER, "TYRANNY_NEXT_AND_WINNER", UI_FLAG_CENTER_X | UI_FLAG_TOP);
		m_pPVPNextBattleAndWinner->CreateSubControl ();
		RegisterControl ( m_pPVPNextBattleAndWinner );
		ShowGroupBottom ( PVP_TYRANNY_NEXT_AND_WINNER );
		//HideGroup ( PVP_TYRANNY_NEXT_AND_WINNER );
	}

	{
		/*pvp tyranny, Juver, 2017/08/25 */
		m_pPVPTyrannyTowerCapture = new CPVPTyrannyTowerCapture;
		m_pPVPTyrannyTowerCapture->Create( PVP_TYRANNY_TOWER_CAPTURE, "TYRANNY_TOWER_CAPTURE", UI_FLAG_CENTER_X );
		m_pPVPTyrannyTowerCapture->CreateSubControl ();
		RegisterControl ( m_pPVPTyrannyTowerCapture );
		ShowGroupBottom ( PVP_TYRANNY_TOWER_CAPTURE );
		HideGroup ( PVP_TYRANNY_TOWER_CAPTURE );

		m_pPVPTyrannyTowerCaptureDummy = new CUIControl;
		m_pPVPTyrannyTowerCaptureDummy->Create ( NO_ID, "TYRANNY_TOWER_CAPTURE", UI_FLAG_CENTER_X );
		RegisterControl ( m_pPVPTyrannyTowerCaptureDummy );
		m_pPVPTyrannyTowerCaptureDummy->SetVisibleSingle( FALSE );

		m_pRebirthDialogueTyranny = new CPVPTyrannyRebirthDialogue;
		m_pRebirthDialogueTyranny->Create ( REBIRTH_DIALOGUE_TYRANNY, "TYRANNY_FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_BOTTOM );
		m_pRebirthDialogueTyranny->CreateSubControl ();
		RegisterControl ( m_pRebirthDialogueTyranny );
		ShowGroupBottom ( REBIRTH_DIALOGUE_TYRANNY );
		HideGroup ( REBIRTH_DIALOGUE_TYRANNY );

		m_pPVPTyrannyTowerProgress = new CPVPTyrannyTowerProgress;
		m_pPVPTyrannyTowerProgress->Create( PVP_TYRANNY_TOWER_PROGRESS, "TYRANNY_TOWER_PROGRESS" );
		m_pPVPTyrannyTowerProgress->CreateSubControl ();
		RegisterControl ( m_pPVPTyrannyTowerProgress );
		ShowGroupBottom ( PVP_TYRANNY_TOWER_PROGRESS );
		HideGroup ( PVP_TYRANNY_TOWER_PROGRESS );

		for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
		{
			UIGUID uiguid = PVP_TYRANNY_TOWER_CAPTURE_NOTICE_SG + i; 
			m_pPVPTyrannyNoticeCapture[i] = new CPVPTyrannyCaptureNotice;
			m_pPVPTyrannyNoticeCapture[i]->Create( uiguid , "PROGRESS_DISPLAY_CTF_BACK" , UI_FLAG_CENTER_X );
			m_pPVPTyrannyNoticeCapture[i]->CreateSubControl ( i );
			RegisterControl ( m_pPVPTyrannyNoticeCapture[i] );
			ShowGroupBottom ( uiguid );
			HideGroup ( uiguid );
		}

		for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
		{
			UIGUID uiguid = PVP_TYRANNY_TOWER_2CAPTURE_NOTICE_SG + i; 
			m_pPVPTyranny2Capture[i] = new CPVPTyranny2CaptureNotice;
			m_pPVPTyranny2Capture[i]->Create( uiguid , "NOTIFY_CTF_SPURT" , UI_FLAG_CENTER_X );
			m_pPVPTyranny2Capture[i]->CreateSubControl ( i );
			RegisterControl ( m_pPVPTyranny2Capture[i] );
			ShowGroupBottom ( uiguid );
			HideGroup ( uiguid );
		}

		m_pPVPTyranny2CaptureOwn = new CPVPTyranny2CaptureNotice;
		m_pPVPTyranny2CaptureOwn->Create( PVP_TYRANNY_TOWER_2CAPTURE_NOTICE_OWN , "NOTIFY_CTF_SPURT" , UI_FLAG_CENTER_X );
		m_pPVPTyranny2CaptureOwn->CreateSubControl( TYRANNY_TOWER_SIZE );
		RegisterControl ( m_pPVPTyranny2CaptureOwn );
		ShowGroupBottom ( PVP_TYRANNY_TOWER_2CAPTURE_NOTICE_OWN );
		HideGroup ( PVP_TYRANNY_TOWER_2CAPTURE_NOTICE_OWN );

		m_pPVPTyrannyRankings = new CPVPTyrannyRankings;
		m_pPVPTyrannyRankings->Create ( PVP_TYRANNY_RANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPTyrannyRankings->CreateBaseWindowLightGray ( "CTF_RESULT_WINDOW", "" );
		m_pPVPTyrannyRankings->CreateSubControl ();
		m_pPVPTyrannyRankings->ResetAlignFlag();
		m_pPVPTyrannyRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPTyrannyRankings );
		ShowGroupFocus ( PVP_TYRANNY_RANKINGS, true );
		HideGroup ( PVP_TYRANNY_RANKINGS );

		m_pPVPTyrannyTopRankings = new CPVPTyrannyTopRankings;
		m_pPVPTyrannyTopRankings->Create ( PVP_TYRANNY_TOPRANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPTyrannyTopRankings->CreateBaseWindowLightGray ( "COMPETITION_TOPRANKING", (char*)ID2GAMEWORD("COMPETITION_TOPRANKING", 1 ) );
		m_pPVPTyrannyTopRankings->CreateSubControl ();
		m_pPVPTyrannyTopRankings->ResetAlignFlag();
		m_pPVPTyrannyTopRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPTyrannyTopRankings );
		ShowGroupFocus ( PVP_TYRANNY_TOPRANKINGS, true );
		HideGroup ( PVP_TYRANNY_TOPRANKINGS );

		m_pPVPTyrannyBattleNoticeStart = new CPVPTyrannyBattleNotice;
		m_pPVPTyrannyBattleNoticeStart->Create( PVP_TYRANNY_BATTLE_START_NOTICE , "PROGRESS_DISPLAY_CTF_BACK1" , UI_FLAG_CENTER_X );
		m_pPVPTyrannyBattleNoticeStart->CreateSubControlStartImage();
		RegisterControl ( m_pPVPTyrannyBattleNoticeStart );
		ShowGroupBottom ( PVP_TYRANNY_BATTLE_START_NOTICE );
		HideGroup ( PVP_TYRANNY_BATTLE_START_NOTICE );

		m_pPVPTyrannyBattleNoticeEnd = new CPVPTyrannyBattleNotice;
		m_pPVPTyrannyBattleNoticeEnd->Create( PVP_TYRANNY_BATTLE_END_NOTICE , "PROGRESS_DISPLAY_CTF_BACK1" , UI_FLAG_CENTER_X );
		m_pPVPTyrannyBattleNoticeEnd->CreateSubControlEndImage();
		RegisterControl ( m_pPVPTyrannyBattleNoticeEnd );
		ShowGroupBottom ( PVP_TYRANNY_BATTLE_END_NOTICE );
		HideGroup ( PVP_TYRANNY_BATTLE_END_NOTICE );
	}


	{  /*item exchange, Juver, 2017/10/12 */
		m_pNPCItemExchangeWindow = new CNPCItemExchangeWindow;
		m_pNPCItemExchangeWindow->Create( NPC_ITEM_EXCHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pNPCItemExchangeWindow->CreateBaseWindowLightGray( "NPC_ITEM_EXCHANGE_WINDOW", NULL );
		m_pNPCItemExchangeWindow->CreateSubControl();
		m_pNPCItemExchangeWindow->ResetAlignFlag();
		m_pNPCItemExchangeWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pNPCItemExchangeWindow->SetTitleName( ID2GAMEWORD("NPC_ITEM_EXCHANGE_TEXT", 0) );
		RegisterControl( m_pNPCItemExchangeWindow, true );
		ShowGroupFocus( NPC_ITEM_EXCHANGE_WINDOW );
		HideGroup( NPC_ITEM_EXCHANGE_WINDOW );
	}


	{  /*product item, Juver, 2017/10/15 */
		m_pProductWindow = new CProductWindow;
		m_pProductWindow->Create( PRODUCT_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pProductWindow->CreateBaseWindowLightGray( "RAN_PRODUCT_WINDOW_BASE", NULL );
		m_pProductWindow->CreateSubControl();
		m_pProductWindow->ResetAlignFlag();
		m_pProductWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pProductWindow->SetTitleName( ID2GAMEWORD("RAN_PRODUCT_WINDOW_TEXT", 0 ) );
		RegisterControl( m_pProductWindow, true );
		ShowGroupFocus( PRODUCT_WINDOW );
		HideGroup( PRODUCT_WINDOW );
	}

	{ /*student record ui, Juver, 2017/10/30 */
		m_pStudentRecordWindow = new CStudentRecordWindow;
		m_pStudentRecordWindow->Create( STUDENT_RECORD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStudentRecordWindow->CreateBaseWindowLightGray( "RAN_STUDENTRECORD_WINDOW", NULL );
		m_pStudentRecordWindow->CreateSubControl();
		m_pStudentRecordWindow->ResetAlignFlag();
		m_pStudentRecordWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pStudentRecordWindow->SetTitleName( ID2GAMEWORD("RAN_STUDENTRECORD_WINDOW_TITLE", 0 ) );
		RegisterControl( m_pStudentRecordWindow, true );
		ShowGroupFocus( STUDENT_RECORD_WINDOW );
		HideGroup( STUDENT_RECORD_WINDOW );
	}
	{	/* Codex, Archie 02/16/24 */
		m_pStudentCodexWindow = new CStudentCodexWindow;
		m_pStudentCodexWindow->Create( STUDENT_CODEX_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStudentCodexWindow->CreateBaseWindowLightGray( "RAN_STUDENTRECORD_WINDOW", NULL );
		m_pStudentCodexWindow->CreateSubControl();
		m_pStudentCodexWindow->ResetAlignFlag();
		m_pStudentCodexWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pStudentCodexWindow->SetTitleName( ID2GAMEWORD("RAN_CODEX_WINDOW_TITLE", 0 ) );
		RegisterControl( m_pStudentCodexWindow, true );
		ShowGroupFocus( STUDENT_CODEX_WINDOW );
		HideGroup( STUDENT_CODEX_WINDOW );
	}

	{ /*activity system, Juver, 2017/11/05 */
		m_pActivityItemWindow = new CActivityItemWindow;
		m_pActivityItemWindow->Create( ACTIVITY_ITEM_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pActivityItemWindow->CreateBaseWindowLightGray( "RAN_ACTIVITY_SALESITEM_WINDOW", NULL );
		m_pActivityItemWindow->CreateSubControl();
		m_pActivityItemWindow->ResetAlignFlag();
		m_pActivityItemWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pActivityItemWindow->SetTitleName( ID2GAMEWORD("RAN_ACTIVITY_SALESITEM_TITLE", 0 ) );
		RegisterControl( m_pActivityItemWindow, true );
		ShowGroupFocus( ACTIVITY_ITEM_WINDOW );
		HideGroup( ACTIVITY_ITEM_WINDOW );
	}

	if ( RPARAM::bFeatureBossViewer )
	{	/* Boss Spawn Viewer, Review000 */
		m_pBossDetailsWindow = new CBossDetailsWindow;
		m_pBossDetailsWindow->Create( BOSS_DETAILS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBossDetailsWindow->CreateBaseWindowLightGray( "BOSS_DETAILS_WINDOW", NULL );
		m_pBossDetailsWindow->CreateSubControl();
		m_pBossDetailsWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pBossDetailsWindow->SetTitleName( "BOSS VIEWER" );
		RegisterControl( m_pBossDetailsWindow, true );
		ShowGroupFocus( BOSS_DETAILS_WINDOW );

		m_pBossViewerButton = new CBossViewerButton;
		m_pBossViewerButton->Create ( BOSS_VIEWER_BUTTON, "BOSS_VIEWER_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pBossViewerButton->CreateSubControl ();
		RegisterControl ( m_pBossViewerButton );
		ShowGroupBottom ( BOSS_VIEWER_BUTTON );


	}

	if ( RPARAM::bGameItemShop )
	{
		m_pItemShopButton = new CItemShopButton;
		m_pItemShopButton->Create ( ITEMSHOP_BUTTON, "ITEM_SHOP_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pItemShopButton->CreateSubControl ();
		RegisterControl ( m_pItemShopButton );
		ShowGroupBottom ( ITEMSHOP_BUTTON );
	}
	
	{	/*charinfoview , Juver, 2017/11/11 */
		m_pCharacterInfoViewWindow = new CCharacterInfoViewWindow;
		m_pCharacterInfoViewWindow->Create ( CHARACTER_INFO_VIEW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCharacterInfoViewWindow->CreateBaseWindowLightGray ( "RAN_ANOTHER_CHAR_WINDOW", "" );
		m_pCharacterInfoViewWindow->CreateSubControl ();
		m_pCharacterInfoViewWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pCharacterInfoViewWindow, true );
		ShowGroupFocus ( CHARACTER_INFO_VIEW_WINDOW );
	}

	{	/*bike color , Juver, 2017/11/12 */
		m_pBikeColorWindow = new CBikeColorWindow;
		m_pBikeColorWindow->Create( BIKE_COLOR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBikeColorWindow->CreateBaseWindowLightGray( "RAN_BIKE_COLOR_CHANGE_WINDOW", NULL );
		m_pBikeColorWindow->CreateSubControl();
		m_pBikeColorWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pBikeColorWindow->SetTitleName( (char*)ID2GAMEWORD("RAN_BIKE_COLOR_CHANGE_WINDOW", 0 ) );
		RegisterControl( m_pBikeColorWindow );
		ShowGroupFocus( BIKE_COLOR_WINDOW );
		HideGroup( BIKE_COLOR_WINDOW );
	}

	{
		/*pet skill info display, Juver, 2017/12/01 */
		/*dual pet skill, Juver, 2017/12/29 */
		if ( RPARAM::bPetSkillDisplay )
		{
			m_pPetSkillInfoDisplay = new CPetSkillInfoDisplay;
			m_pPetSkillInfoDisplay->Create ( PET_SKILL_INFO_DISPLAY, "PET_SKILL_INFO_DISPLAY" );
			m_pPetSkillInfoDisplay->CreateSubControl ();
			RegisterControl ( m_pPetSkillInfoDisplay );
			ShowGroupBottom ( PET_SKILL_INFO_DISPLAY );
			HideGroup( PET_SKILL_INFO_DISPLAY );

			m_pPetSkillInfoDisplay2 = new CPetSkillInfoDisplay2;
			m_pPetSkillInfoDisplay2->Create ( PET_SKILL_INFO_DISPLAY2, "PET_SKILL_INFO_DISPLAY2" );
			m_pPetSkillInfoDisplay2->CreateSubControl ();
			RegisterControl ( m_pPetSkillInfoDisplay2 );
			ShowGroupBottom ( PET_SKILL_INFO_DISPLAY2 );
			HideGroup( PET_SKILL_INFO_DISPLAY2 );
		}
	}

	{	/*change scale card, Juver, 2018/01/03 */
		m_pChangeScaleWindow = new CChangeScaleWindow;
		m_pChangeScaleWindow->Create( CHANGE_SCALE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChangeScaleWindow->CreateBaseWindowLightGray( "RAN_CHANGE_SCALE_WINDOW", NULL );
		m_pChangeScaleWindow->CreateSubControl();
		m_pChangeScaleWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pChangeScaleWindow->SetTitleName( (char*)ID2GAMEWORD("RAN_CHANGE_SCALE_WINDOW", 0 ) );
		RegisterControl( m_pChangeScaleWindow );
		ShowGroupFocus( CHANGE_SCALE_WINDOW );
		HideGroup( CHANGE_SCALE_WINDOW );
	}

	{	/*item color, Juver, 2018/01/10 */
		m_pItemColorWindow = new CItemColorWindow;
		m_pItemColorWindow->Create( ITEM_COLOR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemColorWindow->CreateBaseWindowLightGray( "RAN_ITEM_COLOR_WINDOW", NULL );
		m_pItemColorWindow->CreateSubControl();
		m_pItemColorWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemColorWindow->SetTitleName( (char*)ID2GAMEWORD("RAN_ITEM_COLOR_WINDOW", 0 ) );
		RegisterControl( m_pItemColorWindow );
		ShowGroupFocus( ITEM_COLOR_WINDOW );
		HideGroup( ITEM_COLOR_WINDOW );
	}

	{	/*change school card, Juver, 2018/01/12 */
		m_pChangeSchoolWindow = new CChangeSchoolWindow;
		m_pChangeSchoolWindow->Create( CHANGE_SCHOOL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChangeSchoolWindow->CreateBaseWindowLightGray( "RAN_CHANGE_SCHOOL_WINDOW", NULL );
		m_pChangeSchoolWindow->CreateSubControl();
		m_pChangeSchoolWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pChangeSchoolWindow->SetTitleName( (char*)ID2GAMEWORD("RAN_CHANGE_SCHOOL_WINDOW", 0 ) );
		RegisterControl( m_pChangeSchoolWindow );
		ShowGroupFocus( CHANGE_SCHOOL_WINDOW );
		HideGroup( CHANGE_SCHOOL_WINDOW );
	}

	{	/*item transfer card, Juver, 2018/01/18 */
		m_pItemTransferWindow = new CItemTransferWindow;
		m_pItemTransferWindow->Create( ITEM_TRANSFER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemTransferWindow->CreateBaseWindowLightGray( "ITEM_TRANSFER_WINDOW", NULL );
		m_pItemTransferWindow->CreateSubControl();
		m_pItemTransferWindow->ResetAlignFlag();
		m_pItemTransferWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemTransferWindow->SetTitleName( ID2GAMEWORD("ITEM_TRANSFER_WINDOW", 0 ) );
		RegisterControl( m_pItemTransferWindow, true );
		ShowGroupFocus( ITEM_TRANSFER_WINDOW );

#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pItemTransferInvenWindow = new CItemTransferInvenWindow;
		m_pItemTransferInvenWindow->Create( ITEM_TRANSFER_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemTransferInvenWindow->CreateBaseWindowLightGray( "ITEM_TRANSFER_INVEN_WINDOW", NULL );
		m_pItemTransferInvenWindow->CreateSubControl();
		m_pItemTransferInvenWindow->ResetAlignFlag();
		m_pItemTransferInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemTransferInvenWindow->SetTitleName( ID2GAMEWORD("ITEM_TRANSFER_INVEN_WINDOW", 0 ) );
		RegisterControl( m_pItemTransferInvenWindow, true );
		ShowGroupFocus( ITEM_TRANSFER_INVEN_WINDOW );
		HideGroup( ITEM_TRANSFER_INVEN_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pItemTransferInvenWindow = new CEP7ItemTransferInvenWindow;
		m_pItemTransferInvenWindow->Create( ITEM_TRANSFER_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pItemTransferInvenWindow->CreateBaseWindowLightGray( "EP7_ITEM_TRANSFER_INVENTORY_WINDOW", NULL );
#else
		m_pItemTransferInvenWindow->CreateBaseWindowLightGray( "EP7_ITEM_TRANSFER_INVENTORY_WINDOW2", NULL );
#endif

		m_pItemTransferInvenWindow->CreateSubControl();
		m_pItemTransferInvenWindow->ResetAlignFlag();
		m_pItemTransferInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemTransferInvenWindow->SetTitleName( ID2GAMEINTEXT("EP7_ITEM_TRANSFER_INVENTORY_WINDOW_TITLE", 0 ) );
		RegisterControl( m_pItemTransferInvenWindow, true );
		ShowGroupFocus( ITEM_TRANSFER_INVEN_WINDOW );
		HideGroup( ITEM_TRANSFER_INVEN_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pItemTransferInvenWindow = new CModernItemTransferInvenWindow;
		m_pItemTransferInvenWindow->Create( ITEM_TRANSFER_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemTransferInvenWindow->CreateBaseWindowLightGray( "MODERN_ITEM_TRANSFER_INVENTORY_WINDOW", NULL );
		m_pItemTransferInvenWindow->CreateSubControl();
		m_pItemTransferInvenWindow->ResetAlignFlag();
		m_pItemTransferInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemTransferInvenWindow->SetTitleName( ID2GAMEINTEXT("MODERN_ITEM_TRANSFER_INVENTORY_WINDOW_TEXT" ) );
		RegisterControl( m_pItemTransferInvenWindow, true );
		ShowGroupFocus( ITEM_TRANSFER_INVEN_WINDOW );
		HideGroup( ITEM_TRANSFER_INVEN_WINDOW );

#endif // 
		
	}

	{
		/*school wars, Juver, 2018/01/19 */
		m_pPVPSchoolWarsScore = new CPVPSchoolWarsScore;
		m_pPVPSchoolWarsScore->Create( PVP_SCHOOLWARS_SCORE, "PVP_SCHOOLWARS_SCORE", UI_FLAG_CENTER_X );
		m_pPVPSchoolWarsScore->CreateSubControl ();
		RegisterControl ( m_pPVPSchoolWarsScore );
		ShowGroupBottom ( PVP_SCHOOLWARS_SCORE );
		HideGroup ( PVP_SCHOOLWARS_SCORE );

		m_pPVPSchoolWarsScoreDummy = new CUIControl;
		m_pPVPSchoolWarsScoreDummy->Create ( NO_ID, "PVP_SCHOOLWARS_SCORE", UI_FLAG_CENTER_X );
		RegisterControl ( m_pPVPSchoolWarsScoreDummy );
		m_pPVPSchoolWarsScoreDummy->SetVisibleSingle( FALSE );

		m_pRebirthDialogueSchoolWars = new CPVPSchoolWarsRebirthDialogue;
		m_pRebirthDialogueSchoolWars->Create ( REBIRTH_DIALOGUE_SCHOOLWARS, "PVP_SCHOOLWARS_FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_BOTTOM );
		m_pRebirthDialogueSchoolWars->CreateSubControl ();
		RegisterControl ( m_pRebirthDialogueSchoolWars );
		ShowGroupBottom ( REBIRTH_DIALOGUE_SCHOOLWARS );
		HideGroup ( REBIRTH_DIALOGUE_SCHOOLWARS );

		m_pPVPSchoolWarsRankings = new CPVPSchoolWarsRankings;
		m_pPVPSchoolWarsRankings->Create ( PVP_SCHOOLWARS_RANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPSchoolWarsRankings->CreateBaseWindowLightGray ( "SCHOOLWARS_RESULT_WINDOW", "" );
		m_pPVPSchoolWarsRankings->CreateSubControl ();
		m_pPVPSchoolWarsRankings->ResetAlignFlag();
		m_pPVPSchoolWarsRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPSchoolWarsRankings );
		ShowGroupFocus ( PVP_SCHOOLWARS_RANKINGS, true );
		HideGroup ( PVP_SCHOOLWARS_RANKINGS );


		m_pPVPSchoolWarsTopRankings = new CPVPSchoolWarsTopRankings;
		m_pPVPSchoolWarsTopRankings->Create ( PVP_SCHOOLWARS_TOPRANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPSchoolWarsTopRankings->CreateBaseWindowLightGray ( "SCHOOLWARS_TOPRANKING", (char*)ID2GAMEWORD("SCHOOLWARS_TOPRANKING", 1 ) );
		m_pPVPSchoolWarsTopRankings->CreateSubControl ();
		m_pPVPSchoolWarsTopRankings->ResetAlignFlag();
		m_pPVPSchoolWarsTopRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPSchoolWarsTopRankings );
		ShowGroupFocus ( PVP_SCHOOLWARS_TOPRANKINGS, true );
		HideGroup ( PVP_SCHOOLWARS_TOPRANKINGS );


		m_pPVPSchoolWarsBattleNoticeStart = new CPVPSchoolWarsBattleNotice;
		m_pPVPSchoolWarsBattleNoticeStart->Create( PVP_SCHOOLWARS_BATTLE_START_NOTICE , "PROGRESS_DISPLAY_SCHOOLWARS_BACK_1" , UI_FLAG_CENTER_X );
		m_pPVPSchoolWarsBattleNoticeStart->CreateSubControlStartImage();
		RegisterControl ( m_pPVPSchoolWarsBattleNoticeStart );
		ShowGroupBottom ( PVP_SCHOOLWARS_BATTLE_START_NOTICE );
		HideGroup ( PVP_SCHOOLWARS_BATTLE_START_NOTICE );

		m_pPVPSchoolWarsBattleNoticeEnd = new CPVPSchoolWarsBattleNotice;
		m_pPVPSchoolWarsBattleNoticeEnd->Create( PVP_SCHOOLWARS_BATTLE_END_NOTICE , "PROGRESS_DISPLAY_SCHOOLWARS_BACK_1" , UI_FLAG_CENTER_X );
		m_pPVPSchoolWarsBattleNoticeEnd->CreateSubControlEndImage();
		RegisterControl ( m_pPVPSchoolWarsBattleNoticeEnd );
		ShowGroupBottom ( PVP_SCHOOLWARS_BATTLE_END_NOTICE );
		HideGroup ( PVP_SCHOOLWARS_BATTLE_END_NOTICE );
	}

	{	/*pvp capture the flag, Juver, 2018/02/01 */
		m_pRebirthDialogueCaptureTheFlag = new CPVPCaptureTheFlagRebirthDialogue;
		m_pRebirthDialogueCaptureTheFlag->Create ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG, "PVP_CAPTURE_THE_FLAG_FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_BOTTOM );
		m_pRebirthDialogueCaptureTheFlag->CreateSubControl ();
		RegisterControl ( m_pRebirthDialogueCaptureTheFlag );
		ShowGroupBottom ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG );
		HideGroup ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG );

		m_pPVPCaptureTheFlagTopRankings = new CPVPCaptureTheFlagTopRankings;
		m_pPVPCaptureTheFlagTopRankings->Create ( PVP_CAPTURE_THE_FLAG_TOPRANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPCaptureTheFlagTopRankings->CreateBaseWindowLightGray ( "CAPTURE_THE_FLAG_TOPRANKING", (char*)ID2GAMEWORD("CAPTURE_THE_FLAG_TOPRANKING", 1 ) );
		m_pPVPCaptureTheFlagTopRankings->CreateSubControl ();
		m_pPVPCaptureTheFlagTopRankings->ResetAlignFlag();
		m_pPVPCaptureTheFlagTopRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPCaptureTheFlagTopRankings );
		ShowGroupFocus ( PVP_CAPTURE_THE_FLAG_TOPRANKINGS, true );
		HideGroup ( PVP_CAPTURE_THE_FLAG_TOPRANKINGS );

		m_pPVPCaptureTheFlagScore = new CPVPCaptureTheFlagScore;
		m_pPVPCaptureTheFlagScore->Create( PVP_CAPTURE_THE_FLAG_SCORE, "PVP_CAPTURE_THE_FLAG_SCORE", UI_FLAG_CENTER_X );
		m_pPVPCaptureTheFlagScore->CreateSubControl ();
		RegisterControl ( m_pPVPCaptureTheFlagScore );
		ShowGroupBottom ( PVP_CAPTURE_THE_FLAG_SCORE );
		HideGroup ( PVP_CAPTURE_THE_FLAG_SCORE );

		m_pPVPCaptureTheFlagScoreDummy = new CUIControl;
		m_pPVPCaptureTheFlagScoreDummy->Create ( NO_ID, "PVP_CAPTURE_THE_FLAG_SCORE", UI_FLAG_CENTER_X );
		RegisterControl ( m_pPVPCaptureTheFlagScoreDummy );
		m_pPVPCaptureTheFlagScoreDummy->SetVisibleSingle( FALSE );

		m_pPVPCaptureTheFlagRankings = new CPVPCaptureTheFlagRankings;
		m_pPVPCaptureTheFlagRankings->Create ( PVP_CAPTURE_THE_FLAG_RANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPCaptureTheFlagRankings->CreateBaseWindowLightGray ( "CAPTURE_THE_FLAG_RESULT_WINDOW", "" );
		m_pPVPCaptureTheFlagRankings->CreateSubControl ();
		m_pPVPCaptureTheFlagRankings->ResetAlignFlag();
		m_pPVPCaptureTheFlagRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPCaptureTheFlagRankings );
		ShowGroupFocus ( PVP_CAPTURE_THE_FLAG_RANKINGS, true );
		HideGroup ( PVP_CAPTURE_THE_FLAG_RANKINGS );

		m_pPVPCaptureTheFlagBattleNoticeStart = new CPVPCaptureTheFlagBattleNotice;
		m_pPVPCaptureTheFlagBattleNoticeStart->Create( PVP_CAPTURE_THE_FLAG_BATTLE_START_NOTICE , "PROGRESS_DISPLAY_CAPTURE_THE_FLAG_BACK_1" , UI_FLAG_CENTER_X );
		m_pPVPCaptureTheFlagBattleNoticeStart->CreateSubControlStartImage();
		RegisterControl ( m_pPVPCaptureTheFlagBattleNoticeStart );
		ShowGroupBottom ( PVP_CAPTURE_THE_FLAG_BATTLE_START_NOTICE );
		HideGroup ( PVP_CAPTURE_THE_FLAG_BATTLE_START_NOTICE );

		m_pPVPCaptureTheFlagBattleNoticeEnd = new CPVPCaptureTheFlagBattleNotice;
		m_pPVPCaptureTheFlagBattleNoticeEnd->Create( PVP_CAPTURE_THE_FLAG_BATTLE_END_NOTICE , "PROGRESS_DISPLAY_CAPTURE_THE_FLAG_BACK_1" , UI_FLAG_CENTER_X );
		m_pPVPCaptureTheFlagBattleNoticeEnd->CreateSubControlEndImage();
		RegisterControl ( m_pPVPCaptureTheFlagBattleNoticeEnd );
		ShowGroupBottom ( PVP_CAPTURE_THE_FLAG_BATTLE_END_NOTICE );
		HideGroup ( PVP_CAPTURE_THE_FLAG_BATTLE_END_NOTICE );

		m_pPVPCaptureTheFlagHoldIcon = new CPVPCaptureTheFlagHoldIcon;
		m_pPVPCaptureTheFlagHoldIcon->Create ( PVP_CAPTURE_THE_FLAG_HOLD_ICON, "PVP_CAPTURE_THE_FLAG_HOLD_ICON_DISPLAY" );
		m_pPVPCaptureTheFlagHoldIcon->CreateSubControl ();
		RegisterControl ( m_pPVPCaptureTheFlagHoldIcon );
		ShowGroupBottom ( PVP_CAPTURE_THE_FLAG_HOLD_ICON );
	}


	{	/* car, cart color, Juver, 2018/02/14 */
		m_pCarColorWindow = new CCarColorWindow;
		m_pCarColorWindow->Create( CAR_COLOR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCarColorWindow->CreateBaseWindowLightGray( "RAN_CAR_COLOR_WINDOW", NULL );
		m_pCarColorWindow->CreateSubControl();
		m_pCarColorWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pCarColorWindow->SetTitleName( (char*)ID2GAMEWORD("RAN_CAR_COLOR_WINDOW", 0 ) );
		RegisterControl( m_pCarColorWindow );
		ShowGroupFocus( CAR_COLOR_WINDOW );
		HideGroup( CAR_COLOR_WINDOW );
	}

	{	/*info window, Juver, 2018/03/12 */
		m_pInfoWindow = new CInfoWindow;
		m_pInfoWindow->Create( UI_INFO_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoWindow->CreateBaseWindowLightGray( "RAN_INFO_WINDOW", NULL );
		m_pInfoWindow->CreateSubControl();
		m_pInfoWindow->ResetAlignFlag();
		m_pInfoWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pInfoWindow->SetTitleName( (char*)ID2GAMEWORD("RAN_INFO_WINDOW", 0 ) );
		RegisterControl( m_pInfoWindow );
		ShowGroupFocus( UI_INFO_WINDOW );
		HideGroup( UI_INFO_WINDOW );
	}

	
	{	/*item random option rebuild, Juver, 2018/07/03 */
		m_pItemRandomOptionRebuildWindow = new CItemRandomOptionRebuildWindow;
		m_pItemRandomOptionRebuildWindow->Create( ITEM_RANDOM_OPTION_REBUILD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemRandomOptionRebuildWindow->CreateBaseWindowLightGray( "ITEM_RANDOM_OPTION_REBUILD_WINDOW", NULL );
		m_pItemRandomOptionRebuildWindow->CreateSubControl();
		m_pItemRandomOptionRebuildWindow->ResetAlignFlag();
		m_pItemRandomOptionRebuildWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRandomOptionRebuildWindow->SetTitleName( ID2GAMEWORD("ITEM_RANDOM_OPTION_REBUILD_WINDOW", 0 ) );
		RegisterControl( m_pItemRandomOptionRebuildWindow, true );
		ShowGroupFocus( ITEM_RANDOM_OPTION_REBUILD_WINDOW );

#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pItemRandomOptionRebuildInventoryWindow = new CItemRandomOptionRebuildInventoryWindow;
		m_pItemRandomOptionRebuildInventoryWindow->Create( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemRandomOptionRebuildInventoryWindow->CreateBaseWindowLightGray( "ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW", NULL );
		m_pItemRandomOptionRebuildInventoryWindow->CreateSubControl();
		m_pItemRandomOptionRebuildInventoryWindow->ResetAlignFlag();
		m_pItemRandomOptionRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRandomOptionRebuildInventoryWindow->SetTitleName( ID2GAMEWORD("ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW", 0 ) );
		RegisterControl( m_pItemRandomOptionRebuildInventoryWindow, true );
		ShowGroupFocus( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );
		HideGroup( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pItemRandomOptionRebuildInventoryWindow = new CEP7ItemRandomOptionRebuildInventoryWindow;
		m_pItemRandomOptionRebuildInventoryWindow->Create( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pItemRandomOptionRebuildInventoryWindow->CreateBaseWindowLightGray( "EP7_ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW", NULL );
#else
		m_pItemRandomOptionRebuildInventoryWindow->CreateBaseWindowLightGray( "EP7_ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW2", NULL );
#endif

		m_pItemRandomOptionRebuildInventoryWindow->CreateSubControl();
		m_pItemRandomOptionRebuildInventoryWindow->ResetAlignFlag();
		m_pItemRandomOptionRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRandomOptionRebuildInventoryWindow->SetTitleName( ID2GAMEINTEXT("EP7_ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW_TITLE", 0 ) );
		RegisterControl( m_pItemRandomOptionRebuildInventoryWindow, true );
		ShowGroupFocus( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );
		HideGroup( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pItemRandomOptionRebuildInventoryWindow = new CModernItemRandomOptionRebuildInventoryWindow;
		m_pItemRandomOptionRebuildInventoryWindow->Create( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemRandomOptionRebuildInventoryWindow->CreateBaseWindowLightGray( "MODERN_ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW", NULL );
		m_pItemRandomOptionRebuildInventoryWindow->CreateSubControl();
		m_pItemRandomOptionRebuildInventoryWindow->ResetAlignFlag();
		m_pItemRandomOptionRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRandomOptionRebuildInventoryWindow->SetTitleName( ID2GAMEINTEXT("MODERN_ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW_TEXT" ) );
		RegisterControl( m_pItemRandomOptionRebuildInventoryWindow, true );
		ShowGroupFocus( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );
		HideGroup( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );
#endif // 

	}

	
	{	/*pvp reward info, Juver, 2018/07/25 */
		m_pPVPTyrannyRewardInfo = new CPVPTyrannyRewardInfo;
		m_pPVPTyrannyRewardInfo->Create ( PVP_TYRANNY_REWARD_INFO, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPTyrannyRewardInfo->CreateBaseWindowLightGray ( "PVP_TYRANNY_REWARD_INFO", (char*)ID2GAMEWORD("PVP_TYRANNY_REWARD_INFO", 0 ) );
		m_pPVPTyrannyRewardInfo->CreateSubControl ();
		m_pPVPTyrannyRewardInfo->ResetAlignFlag();
		m_pPVPTyrannyRewardInfo->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPTyrannyRewardInfo );
		ShowGroupFocus ( PVP_TYRANNY_REWARD_INFO, true );
		HideGroup ( PVP_TYRANNY_REWARD_INFO );

		m_pPVPSchoolWarsRewardInfo = new CPVPSchoolWarsRewardInfo;
		m_pPVPSchoolWarsRewardInfo->Create ( PVP_SCHOOLWARS_REWARD_INFO, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPSchoolWarsRewardInfo->CreateBaseWindowLightGray ( "PVP_SCHOOLWARS_REWARD_INFO", (char*)ID2GAMEWORD("PVP_SCHOOLWARS_REWARD_INFO", 0 ) );
		m_pPVPSchoolWarsRewardInfo->CreateSubControl ();
		m_pPVPSchoolWarsRewardInfo->ResetAlignFlag();
		m_pPVPSchoolWarsRewardInfo->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPSchoolWarsRewardInfo );
		ShowGroupFocus ( PVP_SCHOOLWARS_REWARD_INFO, true );
		HideGroup ( PVP_SCHOOLWARS_REWARD_INFO );

		m_pPVPCaptureTheFlagRewardInfo = new CPVPCaptureTheFlagRewardInfo;
		m_pPVPCaptureTheFlagRewardInfo->Create ( PVP_CAPTURE_THE_FLAG_REWARD_INFO, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPCaptureTheFlagRewardInfo->CreateBaseWindowLightGray ( "PVP_CAPTURE_THE_FLAG_REWARD_INFO", (char*)ID2GAMEWORD("PVP_CAPTURE_THE_FLAG_REWARD_INFO", 0 ) );
		m_pPVPCaptureTheFlagRewardInfo->CreateSubControl ();
		m_pPVPCaptureTheFlagRewardInfo->ResetAlignFlag();
		m_pPVPCaptureTheFlagRewardInfo->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPCaptureTheFlagRewardInfo );
		ShowGroupFocus ( PVP_CAPTURE_THE_FLAG_REWARD_INFO, true );
		HideGroup ( PVP_CAPTURE_THE_FLAG_REWARD_INFO );
	}

	{
		/*target information display, Juver, 2018/08/22 */
		m_ptarget_information_display = new CBasicVarTextBox;
		m_ptarget_information_display->Create ( TARGET_INFORMATION_DISPLAY, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_ptarget_information_display->CreateSubControl ();
		m_ptarget_information_display->SetVisibleSingle ( FALSE );
		m_ptarget_information_display->SetTextAlign( TEXT_ALIGN_LEFT );
		RegisterControl ( m_ptarget_information_display );
		ShowGroupTop ( TARGET_INFORMATION_DISPLAY );

		m_ptarget_information_display_dummy = new CUIControl;
		m_ptarget_information_display_dummy->Create ( TARGET_INFORMATION_DISPLAY_DUMMY, "CURSOR_TARGET_INFO_DUMMY", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		RegisterControl ( m_ptarget_information_display_dummy );
	}

	{	/*specific item box, Juver, 2018/09/02 */
		m_pitem_box_specific_selection_window = new CItemBoxSpecificSelectionWindow;
		m_pitem_box_specific_selection_window->Create( ITEM_BOX_SPECIFIC_SELECTION_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pitem_box_specific_selection_window->CreateBaseWindowLightGray( "ITEM_BOX_SPECIFIC_SELECTION_WINDOW", NULL );
		m_pitem_box_specific_selection_window->CreateSubControl();
		m_pitem_box_specific_selection_window->ResetAlignFlag();
		m_pitem_box_specific_selection_window->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pitem_box_specific_selection_window->SetTitleName( ID2GAMEWORD("ITEM_BOX_SPECIFIC_SELECTION_WINDOW", 0 ) );
		RegisterControl( m_pitem_box_specific_selection_window, true );
		ShowGroupFocus( ITEM_BOX_SPECIFIC_SELECTION_WINDOW );
		HideGroup( ITEM_BOX_SPECIFIC_SELECTION_WINDOW );
	}

	{
		/*pk combo, EJCode, 2018/11/15 */
		m_ppk_combo_display = new CPKComboDisplay;		
		m_ppk_combo_display->Create ( PK_COMBO_DISPLAY, "PK_COMBO_DISPLAY" );
		m_ppk_combo_display->CreateSubControl ();
		RegisterControl ( m_ppk_combo_display );
		ShowGroupTop ( PK_COMBO_DISPLAY );
		HideGroup ( PK_COMBO_DISPLAY );
	}

	{
		m_pPVPClubWarRankingWindow = new CPVPClubWarRankingWindow;
		m_pPVPClubWarRankingWindow->Create( PVP_CLUB_WAR_RANKING_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPClubWarRankingWindow->CreateBaseWindowLightGray( "PVP_CLUB_WAR_RANKING_WINDOW", NULL );
		m_pPVPClubWarRankingWindow->CreateSubControl();
		m_pPVPClubWarRankingWindow->ResetAlignFlag();
		m_pPVPClubWarRankingWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPVPClubWarRankingWindow->SetTitleName( ID2GAMEWORD("PVP_CLUB_WAR_RANKING_WINDOW", 0 ) );
		RegisterControl( m_pPVPClubWarRankingWindow, true );
		ShowGroupFocus( PVP_CLUB_WAR_RANKING_WINDOW );
		HideGroup( PVP_CLUB_WAR_RANKING_WINDOW );
	}
	
	{ 
		m_pCurrencyShopWindow = new CCurrencyShopWindow;
		m_pCurrencyShopWindow->Create( CURRENCY_SHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCurrencyShopWindow->CreateBaseWindowLightGray( "CURRENCY_SHOP_WINDOW", NULL );
		m_pCurrencyShopWindow->CreateSubControl();
		m_pCurrencyShopWindow->ResetAlignFlag();
		m_pCurrencyShopWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pCurrencyShopWindow->SetTitleName( ID2GAMEWORD("CURRENCY_SHOP_WINDOW", 0) );
		RegisterControl( m_pCurrencyShopWindow, true );
		ShowGroupFocus( CURRENCY_SHOP_WINDOW );
		HideGroup( CURRENCY_SHOP_WINDOW );
	}

	/* personal lock system, Juver, 2019/12/06 */
	if ( RPARAM::bUsePersonalLock )
	{
		/*m_pPersonalLockInfo = new CPersonalLockInfo;
		m_pPersonalLockInfo->Create ( PERSONAL_LOCK_INFO, "PERSONAL_LOCK_INFO",  UI_FLAG_CENTER_X|UI_FLAG_BOTTOM );
		m_pPersonalLockInfo->CreateSubControl ();	
		RegisterControl ( m_pPersonalLockInfo, true );	
		ShowGroupBottom ( PERSONAL_LOCK_INFO );*/

		m_pPersonalLockPinCreate = new CPersonalLockPinCreate;
		m_pPersonalLockPinCreate->Create( PERSONAL_LOCK_PIN_CREATE, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPersonalLockPinCreate->CreateBaseWindowLightGray( "PERSONAL_LOCK_PIN_CREATE", NULL );
		m_pPersonalLockPinCreate->CreateSubControl();
		m_pPersonalLockPinCreate->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPersonalLockPinCreate->SetTitleName( (char*)ID2GAMEWORD("PERSONAL_LOCK_PIN_CREATE", 0 ) );
		RegisterControl( m_pPersonalLockPinCreate );
		ShowGroupFocus( PERSONAL_LOCK_PIN_CREATE );
		HideGroup( PERSONAL_LOCK_PIN_CREATE );

		m_pPersonalLockPinInput = new CPersonalLockPinInput;
		m_pPersonalLockPinInput->Create( PERSONAL_LOCK_PIN_INPUT, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPersonalLockPinInput->CreateBaseWindowLightGray( "PERSONAL_LOCK_PIN_INPUT", NULL );
		m_pPersonalLockPinInput->CreateSubControl();
		m_pPersonalLockPinInput->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPersonalLockPinInput->SetTitleName( (char*)ID2GAMEWORD("PERSONAL_LOCK_PIN_INPUT", 0 ) );
		RegisterControl( m_pPersonalLockPinInput );
		ShowGroupFocus( PERSONAL_LOCK_PIN_INPUT );
		HideGroup( PERSONAL_LOCK_PIN_INPUT );

		m_pPersonalLockPinReset = new CPersonalLockPinReset;
		m_pPersonalLockPinReset->Create( PERSONAL_LOCK_PIN_RESET, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPersonalLockPinReset->CreateBaseWindowLightGray( "PERSONAL_LOCK_PIN_RESET", NULL );
		m_pPersonalLockPinReset->CreateSubControl();
		m_pPersonalLockPinReset->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPersonalLockPinReset->SetTitleName( (char*)ID2GAMEWORD("PERSONAL_LOCK_PIN_RESET", 0 ) );
		RegisterControl( m_pPersonalLockPinReset );
		ShowGroupFocus( PERSONAL_LOCK_PIN_RESET );
		HideGroup( PERSONAL_LOCK_PIN_RESET );

		m_pPersonalLockPinChange = new CPersonalLockPinChange;
		m_pPersonalLockPinChange->Create( PERSONAL_LOCK_PIN_CHANGE, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPersonalLockPinChange->CreateBaseWindowLightGray( "PERSONAL_LOCK_PIN_CHANGE", NULL );
		m_pPersonalLockPinChange->CreateSubControl();
		m_pPersonalLockPinChange->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPersonalLockPinChange->SetTitleName( (char*)ID2GAMEWORD("PERSONAL_LOCK_PIN_CHANGE", 0 ) );
		RegisterControl( m_pPersonalLockPinChange );
		ShowGroupFocus( PERSONAL_LOCK_PIN_CHANGE );
		HideGroup( PERSONAL_LOCK_PIN_CHANGE );

		m_pPersonalLockPinRecover = new CPersonalLockPinRecover;
		m_pPersonalLockPinRecover->Create( PERSONAL_LOCK_PIN_RECOVER, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPersonalLockPinRecover->CreateBaseWindowLightGray( "PERSONAL_LOCK_PIN_RECOVER", NULL );
		m_pPersonalLockPinRecover->CreateSubControl();
		m_pPersonalLockPinRecover->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPersonalLockPinRecover->SetTitleName( (char*)ID2GAMEWORD("PERSONAL_LOCK_PIN_RECOVER", 0 ) );
		RegisterControl( m_pPersonalLockPinRecover );
		ShowGroupFocus( PERSONAL_LOCK_PIN_RECOVER );
		HideGroup( PERSONAL_LOCK_PIN_RECOVER );

		m_pPersonalLockPinRecoverResult = new CPersonalLockPinRecoverResult;
		m_pPersonalLockPinRecoverResult->Create( PERSONAL_LOCK_PIN_RECOVER_RESULT, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPersonalLockPinRecoverResult->CreateBaseWindowLightGray( "PERSONAL_LOCK_PIN_RECOVER_RESULT", NULL );
		m_pPersonalLockPinRecoverResult->CreateSubControl();
		m_pPersonalLockPinRecoverResult->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPersonalLockPinRecoverResult->SetTitleName( (char*)ID2GAMEWORD("PERSONAL_LOCK_PIN_RECOVER_RESULT", 0 ) );
		RegisterControl( m_pPersonalLockPinRecoverResult );
		ShowGroupFocus( PERSONAL_LOCK_PIN_RECOVER_RESULT );
		HideGroup( PERSONAL_LOCK_PIN_RECOVER_RESULT );
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
#else
	/* modern party window, Juver, 2019/12/20 */
	m_pPartyState = new CModernPartyState;
	m_pPartyState->Create ( PARTY_STATE_WINDOW, "MODERN_PARTY_STATE_WINDOW", UI_FLAG_LEFT );
	m_pPartyState->CreateSubControl ();
	RegisterControl ( m_pPartyState );
	ShowGroupBottom ( PARTY_STATE_WINDOW );
#endif

	/* party finder, Juver, 2019/12/28 */
	if( RPARAM::bUsePartyFinder )
	{ 
		m_pPartyFinderWindow = new CPartyFinderWindow;
		m_pPartyFinderWindow->Create( PARTY_FINDER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPartyFinderWindow->CreateBaseWindowLightGray( "PARTY_FINDER_WINDOW", NULL );
		m_pPartyFinderWindow->CreateSubControl();
		m_pPartyFinderWindow->ResetAlignFlag();
		m_pPartyFinderWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPartyFinderWindow->SetTitleName( ID2GAMEWORD("PARTY_FINDER_WINDOW", 0) );
		RegisterControl( m_pPartyFinderWindow, true );
		ShowGroupFocus( PARTY_FINDER_WINDOW );
		HideGroup( PARTY_FINDER_WINDOW );

		m_pPartyFinderQuestion = new CPartyFinderQuestion;
		m_pPartyFinderQuestion->Create ( PARTY_FINDER_QUESTION, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pPartyFinderQuestion->CreateBaseModal ( "PARTY_FINDER_QUESTION_WINDOW");
		m_pPartyFinderQuestion->CreateSubControl ();
		m_pPartyFinderQuestion->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPartyFinderQuestion );
		ShowGroupFocus ( PARTY_FINDER_QUESTION );
		HideGroup( PARTY_FINDER_QUESTION );
	}

	{	/* pvp club death match, Juver, 2020/11/26 */
		m_pRebirthDialogueClubDeathMatch = new CPVPClubDeathMatchRebirthDialogue;
		m_pRebirthDialogueClubDeathMatch->Create ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH, "PVP_CLUB_DEATH_MATCH_FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_BOTTOM );
		m_pRebirthDialogueClubDeathMatch->CreateSubControl ();
		RegisterControl ( m_pRebirthDialogueClubDeathMatch );
		ShowGroupBottom ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH );
		HideGroup ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH );

		m_pPVPClubDeathMatchBattleNoticeStart = new CPVPClubDeathMatchBattleNotice;
		m_pPVPClubDeathMatchBattleNoticeStart->Create( PVP_CLUB_DEATH_MATCH_BATTLE_START_NOTICE , "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_BACK_1" , UI_FLAG_CENTER_X );
		m_pPVPClubDeathMatchBattleNoticeStart->CreateSubControlStartImage();
		RegisterControl ( m_pPVPClubDeathMatchBattleNoticeStart );
		ShowGroupBottom ( PVP_CLUB_DEATH_MATCH_BATTLE_START_NOTICE );
		HideGroup ( PVP_CLUB_DEATH_MATCH_BATTLE_START_NOTICE );

		m_pPVPClubDeathMatchBattleNoticeEnd = new CPVPClubDeathMatchBattleNotice;
		m_pPVPClubDeathMatchBattleNoticeEnd->Create( PVP_CLUB_DEATH_MATCH_BATTLE_END_NOTICE , "PROGRESS_DISPLAY_PVP_CLUB_DEATH_MATCH_BACK_1" , UI_FLAG_CENTER_X );
		m_pPVPClubDeathMatchBattleNoticeEnd->CreateSubControlEndImage();
		RegisterControl ( m_pPVPClubDeathMatchBattleNoticeEnd );
		ShowGroupBottom ( PVP_CLUB_DEATH_MATCH_BATTLE_END_NOTICE );
		HideGroup ( PVP_CLUB_DEATH_MATCH_BATTLE_END_NOTICE );

		m_pPVPClubDeathMatchScore = new CPVPClubDeathMatchScore;
		m_pPVPClubDeathMatchScore->Create( PVP_CLUB_DEATH_MATCH_SCORE, "RNCDMPROGRESS_WINDOW", UI_FLAG_CENTER_X );
		m_pPVPClubDeathMatchScore->CreateSubControl ();
		RegisterControl ( m_pPVPClubDeathMatchScore );
		ShowGroupBottom ( PVP_CLUB_DEATH_MATCH_SCORE );
		HideGroup ( PVP_CLUB_DEATH_MATCH_SCORE );

		m_pPVPClubDeathMatchScoreDummy = new CUIControl;
		m_pPVPClubDeathMatchScoreDummy->Create ( NO_ID, "RNCDMPROGRESS_WINDOW", UI_FLAG_CENTER_X );
		RegisterControl ( m_pPVPClubDeathMatchScoreDummy );
		m_pPVPClubDeathMatchScoreDummy->SetVisibleSingle( FALSE );

		m_pPVPClubDeathMatchTopRankings = new CPVPClubDeathMatchTopRankings;
		m_pPVPClubDeathMatchTopRankings->Create ( PVP_CLUB_DEATH_MATCH_TOP_RANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPClubDeathMatchTopRankings->CreateBaseWindowLightGray ( "PVP_CLUB_DEATH_MATCH_TOP_RANKING", (char*)ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_TOP_RANKING", 1 ) );
		m_pPVPClubDeathMatchTopRankings->CreateSubControl ();
		m_pPVPClubDeathMatchTopRankings->ResetAlignFlag();
		m_pPVPClubDeathMatchTopRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPClubDeathMatchTopRankings );
		ShowGroupFocus ( PVP_CLUB_DEATH_MATCH_TOP_RANKINGS, true );
		HideGroup ( PVP_CLUB_DEATH_MATCH_TOP_RANKINGS );

		m_pPVPClubDeathMatchRankings = new CPVPClubDeathMatchRankings;
		m_pPVPClubDeathMatchRankings->Create ( PVP_CLUB_DEATH_MATCH_RANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPClubDeathMatchRankings->CreateBaseWindowLightGray ( "RNCDM_RANK_WINDOW", "" );
		m_pPVPClubDeathMatchRankings->CreateSubControl ();
		m_pPVPClubDeathMatchRankings->ResetAlignFlag();
		m_pPVPClubDeathMatchRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPClubDeathMatchRankings );
		ShowGroupFocus ( PVP_CLUB_DEATH_MATCH_RANKINGS, true );
		HideGroup ( PVP_CLUB_DEATH_MATCH_RANKINGS );

		m_pPVPClubDeathMatchDoublePointDisplay = new CPVPClubDeathMatchDoublePointDisplay;		
		m_pPVPClubDeathMatchDoublePointDisplay->Create ( PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_DISPLAY, "PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_DISPLAY" );
		m_pPVPClubDeathMatchDoublePointDisplay->CreateSubControl ();
		RegisterControl ( m_pPVPClubDeathMatchDoublePointDisplay );
		ShowGroupTop ( PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_DISPLAY );
		HideGroup ( PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_DISPLAY );

		m_pPVPClubDeathMatchRewardInfo = new CPVPClubDeathMatchRewardInfo;
		m_pPVPClubDeathMatchRewardInfo->Create ( PVP_CLUB_DEATH_MATCH_REWARD_INFO, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPClubDeathMatchRewardInfo->CreateBaseWindowLightGray ( "PVP_CLUB_DEATH_MATCH_REWARD_INFO", (char*)ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_REWARD_INFO", 0 ) );
		m_pPVPClubDeathMatchRewardInfo->CreateSubControl ();
		m_pPVPClubDeathMatchRewardInfo->ResetAlignFlag();
		m_pPVPClubDeathMatchRewardInfo->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPClubDeathMatchRewardInfo );
		ShowGroupFocus ( PVP_CLUB_DEATH_MATCH_REWARD_INFO, true );
		HideGroup ( PVP_CLUB_DEATH_MATCH_REWARD_INFO );
	}


	{	/* chaos machine, Juver, 2021/07/08 */
		m_pChaosMachineWindow = new CChaosMachineWindow;
		m_pChaosMachineWindow->Create( CHAOS_MACHINE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChaosMachineWindow->CreateBaseWindowLightGray( "CHAOS_MACHINE_WINDOW", NULL );
		m_pChaosMachineWindow->CreateSubControl();
		m_pChaosMachineWindow->ResetAlignFlag();
		m_pChaosMachineWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pChaosMachineWindow->SetTitleName( ID2GAMEWORD("CHAOS_MACHINE_WINDOW", 0 ) );
		RegisterControl( m_pChaosMachineWindow, true );
		ShowGroupFocus( CHAOS_MACHINE_WINDOW );

#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pChaosMachineInvenWindow = new CChaosMachineInvenWindow;
		m_pChaosMachineInvenWindow->Create( CHAOS_MACHINE_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChaosMachineInvenWindow->CreateBaseWindowLightGray( "CHAOS_MACHINE_INVEN_WINDOW", NULL );
		m_pChaosMachineInvenWindow->CreateSubControl();
		m_pChaosMachineInvenWindow->ResetAlignFlag();
		m_pChaosMachineInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pChaosMachineInvenWindow->SetTitleName( ID2GAMEWORD("CHAOS_MACHINE_INVEN_WINDOW", 0 ) );
		RegisterControl( m_pChaosMachineInvenWindow, true );
		ShowGroupFocus( CHAOS_MACHINE_INVEN_WINDOW );
		HideGroup( CHAOS_MACHINE_INVEN_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pChaosMachineInvenWindow = new CEP7ChaosMachineInvenWindow;
		m_pChaosMachineInvenWindow->Create( CHAOS_MACHINE_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pChaosMachineInvenWindow->CreateBaseWindowLightGray( "EP7_CHAOS_MACHINE_INVENTORY_WINDOW", NULL );
#else
		m_pChaosMachineInvenWindow->CreateBaseWindowLightGray( "EP7_CHAOS_MACHINE_INVENTORY_WINDOW2", NULL );
#endif

		m_pChaosMachineInvenWindow->CreateSubControl();
		m_pChaosMachineInvenWindow->ResetAlignFlag();
		m_pChaosMachineInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pChaosMachineInvenWindow->SetTitleName( ID2GAMEWORD("EP7_CHAOS_MACHINE_INVEN_WINDOW", 0 ) );
		RegisterControl( m_pChaosMachineInvenWindow, true );
		ShowGroupFocus( CHAOS_MACHINE_INVEN_WINDOW );
		HideGroup( CHAOS_MACHINE_INVEN_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pChaosMachineInvenWindow = new CModernChaosMachineInvenWindow;
		m_pChaosMachineInvenWindow->Create( CHAOS_MACHINE_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChaosMachineInvenWindow->CreateBaseWindowLightGray( "MODERN_CHAOS_MACHINE_INVENTORY_WINDOW", NULL );
		m_pChaosMachineInvenWindow->CreateSubControl();
		m_pChaosMachineInvenWindow->ResetAlignFlag();
		m_pChaosMachineInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pChaosMachineInvenWindow->SetTitleName( ID2GAMEWORD("MODERN_CHAOS_MACHINE_INVEN_WINDOW" ) );
		RegisterControl( m_pChaosMachineInvenWindow, true );
		ShowGroupFocus( CHAOS_MACHINE_INVEN_WINDOW );
		HideGroup( CHAOS_MACHINE_INVEN_WINDOW );

#endif // 

	}


	{   /*npc shop, Juver, 2017/07/25 */
		m_pContributionShopWindow = new CNPCContributionShopWindow;
		m_pContributionShopWindow->Create( CONTRIBUTION_SHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pContributionShopWindow->CreateBaseWindowLightGray( "NPC_CONTRIBUTION_SHOP_WINDOW", NULL );
		m_pContributionShopWindow->CreateSubControl();
		m_pContributionShopWindow->ResetAlignFlag();
		m_pContributionShopWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pContributionShopWindow->SetTitleName( ID2GAMEWORD("NPC_CONTRIBUTION_SHOP_TEXT", 0) );
		RegisterControl( m_pContributionShopWindow, true );
		ShowGroupFocus( CONTRIBUTION_SHOP_WINDOW );
		HideGroup( CONTRIBUTION_SHOP_WINDOW );
	}

	{
		m_pPKMarkDisplay = new CPKMarkDisplay;
		m_pPKMarkDisplay->Create ( PKMARK_DISPLAY, "PKRANK_DISPLAY", UI_FLAG_RIGHT );
		m_pPKMarkDisplay->CreateSubControl ();
		RegisterControl ( m_pPKMarkDisplay );
		ShowGroupBottom ( PKMARK_DISPLAY );
	}

	{
		m_pMaxRvPreviewWindow = new CMaxRvPreviewWindow;
		m_pMaxRvPreviewWindow->Create( MAXRV_PREVIEW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pMaxRvPreviewWindow->CreateBaseWindowLightGray( "MAXRV_PREVIEW_WINDOW", NULL );
		m_pMaxRvPreviewWindow->CreateSubControl();
		m_pMaxRvPreviewWindow->ResetAlignFlag();
		m_pMaxRvPreviewWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMaxRvPreviewWindow->SetTitleName( ID2GAMEWORD("MAXRV_PREVIEW_WINDOW", 0) );
		RegisterControl( m_pMaxRvPreviewWindow, true );
		ShowGroupFocus( MAXRV_PREVIEW_WINDOW );
		HideGroup( MAXRV_PREVIEW_WINDOW );
	}


	{
		m_pAutoPotionWindow = new CAutoPotionWindow;
		m_pAutoPotionWindow->Create( AUTO_POTION_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pAutoPotionWindow->CreateBaseWindowLightGray( "AUTO_POTION_WINDOW", NULL );
		m_pAutoPotionWindow->CreateSubControl();
		m_pAutoPotionWindow->ResetAlignFlag();
		m_pAutoPotionWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pAutoPotionWindow->SetTitleName( ID2GAMEWORD("AUTO_POTION_WINDOW", 0) );
		RegisterControl( m_pAutoPotionWindow, true );
		ShowGroupFocus( AUTO_POTION_WINDOW );
		HideGroup( AUTO_POTION_WINDOW );
	}

	{
		m_pAutoPilotWindow = new CAutoPilotWindow;
		m_pAutoPilotWindow->Create( AUTO_PILOT_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pAutoPilotWindow->CreateBaseWindowLightGray( "AUTO_PILOT_WINDOW", NULL );
		m_pAutoPilotWindow->CreateSubControl();
		m_pAutoPilotWindow->ResetAlignFlag();
		m_pAutoPilotWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pAutoPilotWindow->SetTitleName( ID2GAMEWORD("AUTO_PILOT_WINDOW", 0) );
		RegisterControl( m_pAutoPilotWindow, true );
		ShowGroupFocus( AUTO_PILOT_WINDOW );
		HideGroup( AUTO_PILOT_WINDOW );
	}

	{
		m_pCharacterRebornAWindow = new CCharacterRebornAWindow;
		m_pCharacterRebornAWindow->Create ( CHARACTER_REBORN_A_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCharacterRebornAWindow->CreateBaseWindowLightGray ( "CHARACTER_REBORN_A_WINDOW", (char*)ID2GAMEWORD ( "CHARACTER_REBORN_A_WINDOW", 0 ) );
		m_pCharacterRebornAWindow->CreateSubControl ();
		m_pCharacterRebornAWindow->ResetAlignFlag ();
		RegisterControl ( m_pCharacterRebornAWindow, true );
		ShowGroupFocus ( CHARACTER_REBORN_A_WINDOW );
	}

	{
		m_pExchangeItemWindow = new CExchangeItemWindow;
		m_pExchangeItemWindow->Create ( EXCHANGE_ITEM_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pExchangeItemWindow->CreateBaseWindowLightGray ( "EXCHANGE_ITEM_WINDOW", (char*)ID2GAMEWORD ( "EXCHANGE_ITEM_WINDOW", 0 ) );
		m_pExchangeItemWindow->CreateSubControl ();
		m_pExchangeItemWindow->ResetAlignFlag ();
		RegisterControl ( m_pExchangeItemWindow, true );
		ShowGroupFocus ( EXCHANGE_ITEM_WINDOW );

#if /*defined( BUILD_EP6 ) ||*/ defined( BUILD_EP4 )
		m_pExchangeItemInvenWindow = new CExchangeItemInventoryWindow;
		m_pExchangeItemInvenWindow->Create( EXCHANGE_ITEM_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pExchangeItemInvenWindow->CreateBaseWindowLightGray( "EXCHANGE_ITEM_INVEN_WINDOW", NULL );
		m_pExchangeItemInvenWindow->CreateSubControl();
		m_pExchangeItemInvenWindow->ResetAlignFlag();
		m_pExchangeItemInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pExchangeItemInvenWindow->SetTitleName( ID2GAMEWORD("EXCHANGE_ITEM_INVEN_WINDOW", 0 ) );
		RegisterControl( m_pExchangeItemInvenWindow, true );
		ShowGroupFocus( EXCHANGE_ITEM_INVEN_WINDOW );
		HideGroup( EXCHANGE_ITEM_INVEN_WINDOW );

#elif defined( BUILD_EP6 ) || defined( BUILD_EP7 )
		/*extended inventory window, EJCode, 2018/10/16 */
		m_pExchangeItemInvenWindow = new CEP7ExchangeItemInventoryWindow;
		m_pExchangeItemInvenWindow->Create( EXCHANGE_ITEM_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );

#if defined( BUILD_EP7 )
		m_pExchangeItemInvenWindow->CreateBaseWindowLightGray( "EP7_EXCHANGE_ITEM_INVENTORY_WINDOW", NULL );
#else
		m_pExchangeItemInvenWindow->CreateBaseWindowLightGray( "EP7_EXCHANGE_ITEM_INVENTORY_WINDOW2", NULL );
#endif

		m_pExchangeItemInvenWindow->CreateSubControl();
		m_pExchangeItemInvenWindow->ResetAlignFlag();
		m_pExchangeItemInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pExchangeItemInvenWindow->SetTitleName( ID2GAMEWORD("EP7_EXCHANGE_ITEM_INVEN_WINDOW", 0 ) );
		RegisterControl( m_pExchangeItemInvenWindow, true );
		ShowGroupFocus( EXCHANGE_ITEM_INVEN_WINDOW );
		HideGroup( EXCHANGE_ITEM_INVEN_WINDOW );

#else
		/*modern inventory window, Juver, 2018/04/06 */
		m_pExchangeItemInvenWindow = new CModernExchangeItemInventoryWindow;
		m_pExchangeItemInvenWindow->Create( EXCHANGE_ITEM_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pExchangeItemInvenWindow->CreateBaseWindowLightGray( "MODERN_EXCHANGE_ITEM_INVENTORY_WINDOW", NULL );
		m_pExchangeItemInvenWindow->CreateSubControl();
		m_pExchangeItemInvenWindow->ResetAlignFlag();
		m_pExchangeItemInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pExchangeItemInvenWindow->SetTitleName( ID2GAMEWORD("MODERN_EXCHANGE_ITEM_INVEN_WINDOW" ) );
		RegisterControl( m_pExchangeItemInvenWindow, true );
		ShowGroupFocus( EXCHANGE_ITEM_INVEN_WINDOW );
		HideGroup( EXCHANGE_ITEM_INVEN_WINDOW );

#endif // 
	}

	{
		m_pCaptchaWindow = new CCaptchaWindow;
		m_pCaptchaWindow->Create ( CAPTCHA_WINDOW, "CAPTCHA_WINDOW", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pCaptchaWindow->CreateSubControl ();
		RegisterControl ( m_pCaptchaWindow );
		ShowGroupFocus ( CAPTCHA_WINDOW );	
	}


	{
		m_pPVPPBGScore = new CPVPPBGScore;
		m_pPVPPBGScore->Create( PVP_PBG_SCORE, "COMPETITION_PBG_SCORE_WINDOW", UI_FLAG_CENTER_X );
		m_pPVPPBGScore->CreateSubControl ();
		RegisterControl ( m_pPVPPBGScore );
		ShowGroupBottom ( PVP_PBG_SCORE );
		HideGroup ( PVP_PBG_SCORE );

		m_pPVPPBGScoreDummy = new CUIControl;
		m_pPVPPBGScoreDummy->Create ( NO_ID, "COMPETITION_PBG_SCORE_WINDOW", UI_FLAG_CENTER_X );
		RegisterControl ( m_pPVPPBGScoreDummy );
		m_pPVPPBGScoreDummy->SetVisibleSingle( FALSE );

		m_pPVPPBGRebirthDialogue = new CPVPPBGRebirthDialogue;
		m_pPVPPBGRebirthDialogue->Create ( REBIRTH_DIALOGUE_PBG, "PVP_PBG_FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_BOTTOM );
		m_pPVPPBGRebirthDialogue->CreateSubControl ();
		RegisterControl ( m_pPVPPBGRebirthDialogue );
		ShowGroupBottom ( REBIRTH_DIALOGUE_PBG );
		HideGroup ( REBIRTH_DIALOGUE_PBG );

		m_pPVPPBGTopRankings = new CPVPPBGTopRankings;
		m_pPVPPBGTopRankings->Create ( PVP_PBG_TOPRANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPPBGTopRankings->CreateBaseWindowLightGray ( "PVP_PBG_TOPRANKING", (char*)ID2GAMEWORD("PVP_PBG_TOPRANKING", 1 ) );
		m_pPVPPBGTopRankings->CreateSubControl ();
		m_pPVPPBGTopRankings->ResetAlignFlag();
		m_pPVPPBGTopRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPPBGTopRankings );
		ShowGroupFocus ( PVP_PBG_TOPRANKINGS, true );
		HideGroup ( PVP_PBG_TOPRANKINGS );

		m_pPVPPBGRankings = new CPVPPBGRankings;
		m_pPVPPBGRankings->Create ( PVP_PBG_RANKINGS, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPPBGRankings->CreateBaseWindowLightGray ( "PVP_PBG_RESULT_WINDOW", "" );
		m_pPVPPBGRankings->CreateSubControl ();
		m_pPVPPBGRankings->ResetAlignFlag();
		m_pPVPPBGRankings->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPVPPBGRankings );
		ShowGroupFocus ( PVP_PBG_RANKINGS, true );
		HideGroup ( PVP_PBG_RANKINGS );
	}

	/* Gacha System, MontageDev 7/10/24 */

	if ( RPARAM::bFeatureGacha )
	{
		m_pGachaWindow = new CGachaWindow;
		m_pGachaWindow->Create( GACHA_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pGachaWindow->CreateBaseWindowLightGray( "GACHA_WINDOW", NULL );
		m_pGachaWindow->CreateSubControl();
		m_pGachaWindow->ResetAlignFlag();
		m_pGachaWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pGachaWindow->SetTitleName( "Lucky Gacha" );
		RegisterControl( m_pGachaWindow, true );
		ShowGroupFocus( GACHA_WINDOW );
		HideGroup( GACHA_WINDOW );
		
		m_pGachaButton = new CGachaButton;
		m_pGachaButton->Create ( GACHA_BUTTON, "GACHA_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pGachaButton->CreateSubControl ();
		RegisterControl ( m_pGachaButton );
		ShowGroupBottom ( GACHA_BUTTON );
		//HideGroup( GACHA_BUTTON );
	}
	
	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	{
		m_pItemShopWindow = new CItemShopWindow;		
		m_pItemShopWindow->Create ( ITEMSHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemShopWindow->CreateBaseWindowLightGray ( "NPCSHOP_WINDOW", (char*)ID2GAMEWORD("ITEMSHOP_NAME_STATIC") );
		m_pItemShopWindow->CreateSubControl ();
		m_pItemShopWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pItemShopWindow, true );
		HideGroup ( ITEMSHOP_WINDOW );

		/*m_pItemShopButton = new CItemShopButton;
		m_pItemShopButton->Create ( ITEMSHOP_BUTTON, "ITEM_SHOP_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pItemShopButton->CreateSubControl ();
		RegisterControl ( m_pItemShopButton );
		ShowGroupBottom(ITEMSHOP_BUTTON);*/
	}
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
	
	{
		/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
		m_pPVPTyrannyRankingsDisplayMain = new CPVPTyrannyRankingsDisplayMain;
		m_pPVPTyrannyRankingsDisplayMain->Create( PVP_TYRANNY_RANKING_DISPLAY, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPVPTyrannyRankingsDisplayMain->CreateBaseWindowLightGray( "PVP_TYRANNY_RANKING_DISPLAY", NULL );
		m_pPVPTyrannyRankingsDisplayMain->CreateSubControl();
		m_pPVPTyrannyRankingsDisplayMain->ResetAlignFlag();
		m_pPVPTyrannyRankingsDisplayMain->SetTitleName( ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 0) );
		RegisterControl ( m_pPVPTyrannyRankingsDisplayMain, true );
		ShowGroupBottom ( PVP_TYRANNY_RANKING_DISPLAY );	
		HideGroup( PVP_TYRANNY_RANKING_DISPLAY );
	}

	{
		/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
		m_pClubWarRankingsDisplayMain = new CClubWarRankingsDisplayMain;
		m_pClubWarRankingsDisplayMain->Create( CLUB_WAR_RANKING_DISPLAY, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubWarRankingsDisplayMain->CreateBaseWindowLightGray( "PVP_TYRANNY_RANKING_DISPLAY", NULL );
		m_pClubWarRankingsDisplayMain->CreateSubControl();
		m_pClubWarRankingsDisplayMain->ResetAlignFlag();
		m_pClubWarRankingsDisplayMain->SetTitleName( ID2GAMEWORD("PVP_TYRANNY_RANKING_DISPLAY", 4) );
		RegisterControl ( m_pClubWarRankingsDisplayMain, true );
		ShowGroupBottom ( CLUB_WAR_RANKING_DISPLAY );	
		HideGroup( CLUB_WAR_RANKING_DISPLAY );
	}

	CloseAllWindow ();

	DxSoundLib::GetInstance()->CreateSound ( "QuestAlarm", ID2GAMEWORD("QUEST_ALARM"), SFX_SOUND );

	return CUIMan::InitDeviceObjects ( pd3dDevice );
}

HRESULT CInnerInterface::DeleteDeviceObjects ()
{
	GLCharacter *pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( pCharacter ) pCharacter->UPDATE_RECORD_CHAT( TRUE );

	TEXTURE_PRE_UNLOAD ();

	ResetControl();

	return CUIMan::DeleteDeviceObjects ();
}

static LPDIRECT3DTEXTUREQ* ppPRELOAD_TEXTURE = NULL;
HRESULT	CInnerInterface::TEXTURE_PRE_LOAD ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	CString strTemp = ID2GAMEWORD("SKILL_TEXTURE",0);
	int nCOUNT = atoi(strTemp);

	if ( nCOUNT < 1 ) return S_FALSE;

	ppPRELOAD_TEXTURE = new LPDIRECT3DTEXTUREQ[nCOUNT];

	for ( int i = 0; i < nCOUNT; ++i )
	{
		CString strKeyword = ID2GAMEWORD("SKILL_TEXTURE",i+1);
		TextureManager::LoadTexture( strKeyword, pd3dDevice, ppPRELOAD_TEXTURE[i], 0, 0 );
	}

	return S_OK;
}

HRESULT	CInnerInterface::TEXTURE_PRE_UNLOAD ()
{
	HRESULT hr;
	CString strTemp = ID2GAMEWORD("SKILL_TEXTURE",0);
	int nCOUNT = atoi(strTemp);

	if ( nCOUNT < 1 ) return S_FALSE;
	
	for ( int i = 0; i < nCOUNT; ++i )
	{
		CString strKeyword = ID2GAMEWORD("SKILL_TEXTURE",i+1);
		// MEMO : ReleaseTexture에서 m_pTexture를 NULL로 만든다.
		hr = TextureManager::ReleaseTexture ( strKeyword, ppPRELOAD_TEXTURE[i] );
	}

	SAFE_DELETE_ARRAY ( ppPRELOAD_TEXTURE );

	return S_OK;
}

void CInnerInterface::ADD_FRIEND_NAME_TO_EDITBOX( const CString& strName )
{
	if( m_pFriendWindow ) 
		m_pFriendWindow->ADD_FRIEND_NAME_TO_EDITBOX( strName );
}

void CInnerInterface::ADD_FRIEND( const CString& strName )
{
	if( m_pFriendWindow )
		m_pFriendWindow->ADD_FRIEND( strName );
}

void CInnerInterface::FRIEND_LIST( CONST CString & strName, bool bOnline )
{
	if( m_pChat )
		m_pChat->FRIEND_LIST( strName, bOnline );
}

// void CInnerInterface::UpdateClubBattleTime( float fClubBattleTime )
// {
// 	if( m_pMiniMap )
// 		m_pMiniMap->UpdateClubTime( fClubBattleTime );
// }

bool CInnerInterface::IsCHAT_BEGIN()
{
	if( m_pChat )
		return m_pChat->IsCHAT_BEGIN();

	return false;
}

void CInnerInterface::ClearItemBank()
{
	if( m_pItemBankWindow )
		m_pItemBankWindow->ClearItemBank();
}

void CInnerInterface::ClearVNGainSys()
{
	if( m_pVNGainSysInventory )
		m_pVNGainSysInventory->ClearVNGainSys();
}

void CInnerInterface::DisableMinimapTarget()
{
	if( m_pLargeMapWindow )
		m_pLargeMapWindow->VISIBLE_TARGET_POS();
}

void CInnerInterface::SET_COUNT_MSG( INT nCount )
{
	if( m_pCountMsgWindow )
	{
		m_pCountMsgWindow->SetCount( nCount );
		ShowGroupTop( COUNT_MSG_WINDOW );
	}
}

void CInnerInterface::SetDefaultPosInterface(UIGUID ControlID)
{
	CUIControl * pControl = FindControl ( ControlID );

	if( pControl )
	{
		pControl->RestoreDeviceObjects(m_pd3dDevice);
	}
}

void CInnerInterface::ReqToggleRun()
{
	GLGaeaClient::GetInstance().GetCharacter()->ReqToggleRun ();
}

VOID CInnerInterface::SET_VNGAINTYPE_GAUGE( int nPos, int nLimit )
{
	m_pVNGainSysGauge->SetGauge( nPos, nLimit );
}


void CInnerInterface::BONUS_TIME_EVENT_START( bool bCharging )
{
	m_pBonusTimeGauge->BONUS_TIME_EVENT_START( bCharging );
}
void CInnerInterface::BONUS_TIME_EVENT_END()
{
	m_pBonusTimeGauge->BONUS_TIME_EVENT_END();
}
void CInnerInterface::BONUS_TIME_BUSTER_START()
{
	m_pBonusTimeGauge->BONUS_TIME_BUSTER_START();
	ShowGroupTop( BONUSTIME_EVENT_DISPLAY );
	m_pBonusTimeDisplay->START();
}

void CInnerInterface::BONUS_TIME_BUSTER_END()
{
	m_pBonusTimeGauge->BONUS_TIME_BUSTER_END();
	ShowGroupTop( BONUSTIME_EVENT_DISPLAY );
	m_pBonusTimeDisplay->STOP();
}

void CInnerInterface::SET_QUEST_HELPER( DWORD dwQuestID )
{
	m_pQuestHelper->SetQuestID( dwQuestID );

	if ( IsVisibleGroup(CDM_RANKING_DISPLAY) )
	{
		HideGroup( QUEST_HELPER_DISPLAY );
	}

}

void CInnerInterface::RESET_QUEST_HELPER( DWORD dwQuestID )
{
	m_pQuestHelper->ReSetQuestID( dwQuestID );
}

void CInnerInterface::SetThaiCCafeClass( DWORD dwClass )
{
	if ( m_pThaiCCafeMark )	m_pThaiCCafeMark->SetClass( dwClass );
}

void CInnerInterface::DoBattleModal( const CString strClubName, bool bAlliance )
{
	if ( !m_pClubBattleModalWindow ) return;
	
    m_pClubBattleModalWindow->SetClubName( strClubName );
	m_pClubBattleModalWindow->SetAlliance( bAlliance );
	ShowGroupFocus( CLUB_BATTLE_MODAL_WINDOW );
}

BOOL CInnerInterface::ReqClubBattle( DWORD dwTime, bool bAlliance )
{
	DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

	if ( bAlliance ) 
		return GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattle( dwGaeaID, dwTime );
	else 
		return GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattle( dwGaeaID, dwTime );
}

void CInnerInterface::OPEN_TAXI_WINDOW( WORD wPosX, WORD wPosY )
{
	if ( !m_pTaxiWindow ) return;
	m_pTaxiWindow->SetInvenPos( wPosX, wPosY );
	ShowGroupFocus( TAXI_WINDOW );
}

/*npc shop, Juver, 2017/07/25 */
void CInnerInterface::SetNPCShopWindowOpen ( SNATIVEID sNativeID )
{
	if ( m_pNPCShopWindow )
	{
		m_pNPCShopWindow->InitShop ( sNativeID );
		ShowGroupFocus ( NPC_SHOP_WINDOW );
	}
}

/*item transfer card, Juver, 2018/01/18 */
void CInnerInterface::OpenItemTransferWindow()
{
	if ( isReviveWaiting() )				return;

	if( !m_pItemTransferWindow || !m_pItemTransferInvenWindow )	return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	const UIRECT& rcInvenWindow = m_pItemTransferInvenWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pItemTransferInvenWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_TRANSFER_INVEN_WINDOW );

	const UIRECT& rcWindow = m_pItemTransferWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcWindow.sizeX;
	m_pItemTransferWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_TRANSFER_WINDOW );
}

/*item transfer card, Juver, 2018/01/18 */
void CInnerInterface::CloseItemTransferWindow()
{
	HideGroup( ITEM_TRANSFER_INVEN_WINDOW );
	HideGroup( ITEM_TRANSFER_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ResetItemTransferItem();
}

/*item random option rebuild, Juver, 2018/07/03 */
void CInnerInterface::OpenItemRandomOptionRebuildWindow()
{
	if ( isReviveWaiting() )				return;

	if( !m_pItemRandomOptionRebuildWindow || !m_pItemRandomOptionRebuildInventoryWindow )	return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	const UIRECT& rcInvenWindow = m_pItemRandomOptionRebuildInventoryWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pItemRandomOptionRebuildInventoryWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );

	const UIRECT& rcWindow = m_pItemRandomOptionRebuildWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcWindow.sizeX;
	m_pItemRandomOptionRebuildWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_RANDOM_OPTION_REBUILD_WINDOW );
}

/*item random option rebuild, Juver, 2018/07/03 */
void CInnerInterface::CloseItemRandomOptionRebuildWindow()
{
	HideGroup( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW );
	HideGroup( ITEM_RANDOM_OPTION_REBUILD_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ResetItemRandomOptionRebuildItemAll();
}

/* chaos machine, Juver, 2021/07/07 */
void CInnerInterface::OpenChaosMachineWindow( DWORD dwNPCID )
{
	if ( isReviveWaiting() )				return;

	if( !m_pChaosMachineWindow || !m_pChaosMachineInvenWindow )	return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	const UIRECT& rcInvenWindow = m_pChaosMachineInvenWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pChaosMachineInvenWindow->SetGlobalPos( vPos );
	ShowGroupFocus( CHAOS_MACHINE_INVEN_WINDOW );

	const UIRECT& rcWindow = m_pChaosMachineWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcWindow.sizeX;
	m_pChaosMachineWindow->SetGlobalPos( vPos );
	ShowGroupFocus( CHAOS_MACHINE_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->m_dwChaosMachineNPCID = dwNPCID;
}

/* chaos machine, Juver, 2021/07/07 */
void CInnerInterface::CloseChaosMachineWindow()
{
	HideGroup( CHAOS_MACHINE_INVEN_WINDOW );
	HideGroup( CHAOS_MACHINE_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ResetChaosMachineItem();
}

void CInnerInterface::OpenContributionShopWindow( SNATIVEID sNpcID )
{
	if ( m_pContributionShopWindow )
	{
		m_pContributionShopWindow->InitShop ( sNpcID );
		ShowGroupFocus ( CONTRIBUTION_SHOP_WINDOW );
	}
}

void CInnerInterface::ReqMaxRVShow( const SITEMCUSTOM& sItemCustom )
{
	if ( RPARAM::bMaxRvPreviewEnable == FALSE )
		return;

	GLGaeaClient::GetInstance().GetCharacter()->ReqMaxRVShow(sItemCustom);
}

void CInnerInterface::MaxRvPreviewDisplayShow()
{
	if ( IsVisibleGroup( MAXRV_PREVIEW_WINDOW ))
		HideGroup( MAXRV_PREVIEW_WINDOW );

	if ( RPARAM::bMaxRvPreviewEnable == FALSE )
		return;

	if ( !m_pMaxRvPreviewWindow )		
		return;

	ShowGroupFocus( MAXRV_PREVIEW_WINDOW );

	m_pMaxRvPreviewWindow->ShowItemData();
}