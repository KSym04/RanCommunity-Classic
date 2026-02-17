#include "StdAfx.h"
#include "PVPNextBattleAndWinner.h" // PVP Next Battle UI - Jxyyy

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "../../Lib_Engine/Core/NSROption.h"

#include "../../Lib_Engine/Common/CommonWeb.h"
#include "../../Lib_Engine/Common/DxInputString.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"

#include "../../Lib_Client/DxGlobalStage.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPartyClient.h"
/* PK Streak Sigaw - Jxyyy */
#include "./PKStreakKillNotice.h" 
#include "./PKStreakShutdownNotice.h" 
/*pk combo, EJCode, 2018/11/15 */
#include "../../Lib_Client/G-Logic/GLPKCombo.h"

#include "../../Lib_Network/s_CHttpPatch.h"

#include "./AdminMessageDisplay.h"
#include "./BasicChat.h"
#include "./BasicInfoView.h"
#include "./BasicVarTextBox.h"
#include "./BlockProgramAlarm.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "./InventoryWindow.h"
#include "./ItemShopWindowWeb.h"
#include "./MapMoveDisplay.h"
#include "./ModalCallerID.h"
#include "./ModalWindow.h"
#include "./NameDisplayMan.h"
#include "./PrivateMarketWindow.h"
#include "./QuestAlarm.h"
#include "./QuestWindow.h"
#include "./RebirthDialogue.h"
#include "./ShopItemSearchWindow.h"
#include "./ItemSearchResultWindow.h"
#include "./MapRequireCheck.h"
#include "./SkillTrayTab.h"
#include "./SystemMessageWindow.h"
#include "./UILeftTopGroup.h"
#include "./UITextControl.h"

#include "./ItemMixWindow.h"
#include "./GatherGauge.h"
#include "./CdmRankingDisplay.h"
#include "./LargeMapWindow.h"

/*game stats, Juver, 2017/06/22 */
#include "./GameStats.h" 

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

/* Competition UI, Juver, 2017/08/24 */
#include "./PVPCompetitionButton.h"
#include "./PVPCompetitionNotify.h"
#include "./PVPCompetitionWindow.h"
#include "./PVPCompetitionNotice.h"

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

/*activity system, Juver, 2017/11/05 */
#include "./ActivityItemWindow.h"

/* Gacha System, MontageDev 7/10/24 */
#include "./GachaSystem/GachaWindow.h"

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
#include "./PVPSchoolWarsTopRankings.h"
#include "./PVPSchoolWarsBattleNotice.h"
#include "./PVPSchoolWarsRankings.h"

/*pvp capture the flag, Juver, 2018/02/01 */
#include "./PVPCaptureTheFlagTopRankings.h"
#include "./PVPCaptureTheFlagRebirthDialogue.h"
#include "./PVPCaptureTheFlagBattleNotice.h"
#include "./PVPCaptureTheFlagScore.h"
#include "./PVPCaptureTheFlagRankings.h"
#include "./PVPCaptureTheFlagHoldIcon.h"

/* car, cart color, Juver, 2018/02/14 */
#include "./CarColorWindow.h"

/*info window, Juver, 2018/03/12 */
#include "./InfoWindow.h"
#include "./BasicTextBox.h"

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

#include "./SkillWindow.h"
#include "./ModernSkillWindow.h"

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

#include "./PVPPBGScore.h"
#include "./PVPPBGRebirthDialogue.h"
#include "./PVPPBGTopRankings.h"
#include "./PVPPBGRankings.h"

#include "./TGRankingDisplay.h"
#include "./BattleRankingDisplay.h"
/* Add ItemShop, Ssodomain, 11-09-2023, Start */
#include "./ItemShop/ItemShopWindow.h"
/* Add ItemShop, Ssodomain, 11-09-2023, End */

/* Gacha System, MontageDev 7/10/24 */
#include "./GachaSystem/GachaButton.h"

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
#include "_TyrannyWarRankingDisplay/PVPTyrannyRankingsDisplayMain.h"

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
#include "_ClubWarRankingDisplay/ClubWarRankingsDisplayMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHAT_MACRO_SIZE	10

CInnerInterface& CInnerInterface::GetInstance()
{
	static CInnerInterface Instance;
	return Instance;
}

const float	CInnerInterface::fMENU_LIFTUP = 90.0f;
const int CInnerInterface::nOUTOFRANGE = -1;

CInnerInterface::CInnerInterface () :
	m_emConflictReqType ( EMCONFT_ONE ),
	m_ConflictReqID ( GAEAID_NULL ),
	m_bPartyStateBack ( FALSE ),
	m_bBlockProgramFound ( false ),
	m_dwEventQuestID ( NATIVEID_NULL().dwID ),
	m_dwEventQuestStep ( UINT_MAX ),
	m_bBlockProgramAlarm ( false ),
	m_bInfoDisplayActive(false),
	m_bInfoDisplayModeSub(false),
	m_bInfoDisplayModeItemLink(false), /*item link, Juver, 2017/07/31 */
	m_bCHANNEL ( false ),
	m_bFirstVNGainSysCall ( true ),
	m_bTabReserve ( false ),
	m_bItemShopLoad ( false ),
	m_bPVPTyrannyShowRanking(false),		/*pvp tyranny, Juver, 2017/08/25 */
	m_bPVPSchoolWarsShowRanking(false),		/*school wars, Juver, 2018/01/20 */
	m_bPVPCaptureTheFlagShowRanking(false),	/*pvp capture the flag, Juver, 2018/02/02 */
	m_bPVPClubDeathMatchShowRanking(false),	/* pvp club death match, Juver, 2020/11/29 */
	m_bPVPPBGShowRanking(false),
	
	m_bPVPWoeShowRanking(false), /*woe Arc Development 08-06-2024*/

	/*modern skill tray, Juver, 2018/08/21 */
	m_skill_tray_lock(false),
	m_skill_tray_mini(false),
	m_skill_tray_rotate(false),

	m_bPVPClubWarShowRanking(false)
{
	/*target information display, Juver, 2018/08/22 */
	m_target_id_information.RESET();

	ResetControl();
}

CInnerInterface::~CInnerInterface ()
{
}

void CInnerInterface::ResetControl()
{
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	m_pGlobalRankingWindow	= NULL;
	/////////////////////////////////////////////////////////////////////////////
	
	m_pPandoraBoxButton = NULL; //pandora
	m_pSimpleHP = NULL;
	m_pBusWindow = NULL;
	m_pMiniMap = NULL;
	m_pChat = NULL;
	m_pQuestWindow = NULL;
	m_pQuestAlarm = NULL;	
	m_pFriendWindow = NULL;	
	m_pSkillWindowToTray = NULL;	
	m_pInventoryWindow = NULL;
	m_pPartyWindow = NULL;
	m_pMiniPartyWindow = NULL;
	m_pTradeWindow = NULL;	
	m_pPtoPWindow = NULL;
	m_pDamageDisplayMan = NULL;
	m_pTargetInfoDisplay = NULL;
	m_pNameDisplayMan = NULL;
	m_pDialogueWindow = NULL;
	m_pMarketWindow = NULL;
	m_pStorageWindow = NULL;
	m_pItemMove = NULL;
	m_pBlockProgramAlarm = NULL;
	m_pItemBankWindow = NULL;
	m_pVNGainSysInventory = NULL;
	m_pTradeInventoryWindow = NULL;
	m_pHeadChatDisplayMan = NULL;	
	m_pAdminMessageDisplay = NULL;	
	m_pConftModalWindow = NULL;	
	m_pPartyModalWindow = NULL;
	m_pConftDisplayMan = NULL;

	m_pInfoDisplayMain = NULL;
	m_pInfoDisplaySub = NULL;

	/*item link, Juver, 2017/07/31 */
	m_pInfoDisplayItemLink = NULL; 

	m_pMapMoveDisplay = NULL;
	m_pWaitServerDisplay = NULL;
	m_pAcademyConftDisplay = NULL;
	m_pLargeMapWindow = NULL;
	m_pStorageChargeCard = NULL;
	m_pRebirthDialogue = NULL;
	m_pPrivateMarketWindow = NULL;
	m_pPrivateMarketMake = NULL;
	m_pPrivateMarketSellWindow = NULL;
	m_pPrivateMarketShowMan = NULL;
	m_pClubWindow = NULL;
	m_pSubMasterSet = NULL;
	m_pClubMake = NULL;
	m_pSystemMessageWindow = NULL;
	m_pQuestionItemDisplay = NULL;
	
	m_pClubStorageWindow = NULL;
	m_pConftConfirm = NULL;
	m_pBasicInfoView = NULL;
	m_pUILeftTopGroup = NULL;
	
	m_pCountMsgWindow = NULL;
	m_pGameMenu = NULL;
	m_pQBoxButton = NULL;
	m_pItemRebuildWindow = NULL;	// ITEMREBUILD_MARK
	//m_pRebuildInventoryWindow = NULL;
	m_pPetRebirthDialogue = NULL; // monster7j
	m_pGambleBox = NULL;
	m_pGambleSelectBox = NULL;
	m_pGambleAgainBox = NULL;
	m_pGambleResultOddBox = NULL;
	m_pGambleResultEvenBox = NULL;
	m_pPetWindow = NULL;
	m_pCharacterWindow = NULL;

	m_pModalWindow = NULL;

	m_pSystemMessageWindowDummy = NULL;
	m_pMapMoveDisplayDummy = NULL;
	m_pBlockProgramAlarmDummy = NULL;
	m_pLeftTopGroupDummy = NULL;
	m_pBasicInfoViewDummy = NULL;
	m_pBasicPotionTrayDummy = NULL;
	m_pQuestAlarmDummy = NULL;

	m_pKeySettingWindow = NULL;
	m_pChatMacroWindow = NULL;

	m_pBonusTimeGauge = NULL;
	m_pBonusTimeDisplay = NULL;
	m_pQuestHelper = NULL;
	m_pVehicleWindow = NULL;
	m_pThaiCCafeMark = NULL;

	m_pItemGarbageWindow = NULL;
	m_pGarbageInventoryWindow = NULL;

	m_pItemShopIconMan = NULL;
	m_pNameDisplayMan = NULL;

	m_pShopItemSearchWindow   = NULL;
	m_pItemSearchResultWindow = NULL;

	m_pAttendanceBookWindow = NULL;
	m_pClubBattleModalWindow = NULL;

	m_pMapRequireCheckWindow = NULL;
	m_pTaxiWindow = NULL;

	m_bFirstVNGainSysCall = true;	

	m_fItemBankDelay = 5.0f;

	m_pItemMixWindow = NULL;
	m_pItemMixInvenWindow = NULL;
	m_pGatherGauge = NULL;
	m_pCdmRankingDisplay = NULL;

	/*game stats, Juver, 2017/06/22 */
	m_pGameStats = NULL; 

	 /*npc shop, Juver, 2017/07/25 */
	m_pNPCShopWindow = NULL;

	/*item preview, Juver, 2017/07/27 */
	m_pItemPreviewWindow = NULL; 

	/*pet status, Juver, 2017/07/30 */
	m_pPetStatus = NULL; 

	/*vehicle status, Juver, 2017/07/30 */
	m_pVehicleStatus = NULL; 

	/*vehicle booster system, Juver, 2017/08/11 */
	m_pVehicleBoosterDisplay = NULL; 

	/*box contents, Juver, 2017/08/30 */
	m_pInfoBoxContents = NULL;

	/* Competition UI, Juver, 2017/08/24 */
	m_pPVPCompetitionButton = NULL;
	m_pPVPCompetitionNotify = NULL;
	m_pPVPCompetitionWindow = NULL;
	m_pPVPCompetitionNotice = NULL;

	/* PK Streak Sigaw - Jxyyy */
	m_pPKStreakKillNotice = NULL;
	m_pPKStreakShutdownNotice = NULL;	
	// PVP Next Battle UI - Jxyyy
	m_pPVPNextBattleAndWinner = NULL;
	/*pvp tyranny, Juver, 2017/08/25 */
	m_pPVPTyrannyTowerCapture = NULL;
	m_pPVPTyrannyTowerCaptureDummy = NULL;
	m_pPVPTyrannyTowerProgress = NULL;
	m_pRebirthDialogueTyranny = NULL;
	m_pPVPTyranny2CaptureOwn = NULL;
	m_pPVPTyrannyRankings = NULL;
	m_pPVPTyrannyTopRankings = NULL;
	m_pPVPTyrannyBattleNoticeStart = NULL;
	m_pPVPTyrannyBattleNoticeEnd = NULL;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	m_pClubWarRankingsDisplayMain = NULL;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	m_pPVPTyrannyRankingsDisplayMain = NULL;
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		m_pPVPTyrannyNoticeCapture[i] = NULL;
		m_pPVPTyranny2Capture[i] = NULL;
	}

	/*woe Arc Development 08-06-2024*/
	m_pPVPWoeTowerCapture = NULL;
	m_pPVPWoeTowerCaptureDummy = NULL;
	m_pPVPWoeTowerProgress = NULL;
	m_pRebirthDialogueWoe = NULL;
	/*item exchange, Juver, 2017/10/12 */
	m_pNPCItemExchangeWindow = NULL;

	/*product item, Juver, 2017/10/15 */
	m_pProductButton = NULL;
	m_pProductWindow = NULL;

	/*student record ui, Juver, 2017/10/30 */
	m_pStudentRecordButton = NULL;
	m_pStudentRecordWindow = NULL;

	/*activity system, Juver, 2017/11/05 */
	m_pActivityItemWindow = NULL;

	/*charinfoview , Juver, 2017/11/11 */
	m_pCharacterInfoViewWindow = NULL;

	/*bike color , Juver, 2017/11/12 */
	m_pBikeColorWindow = NULL;

	/*pet skill info display, Juver, 2017/12/01 */
	m_pPetSkillInfoDisplay = NULL;
	m_pPetSkillInfoDisplay2 = NULL;

	/*change scale card, Juver, 2018/01/03 */
	m_pChangeScaleWindow = NULL;

	/*item color, Juver, 2018/01/10 */
	m_pItemColorWindow = NULL;

	/*change school card, Juver, 2018/01/12 */
	m_pChangeSchoolWindow = NULL;

	/*item transfer card, Juver, 2018/01/18 */
	m_pItemTransferInvenWindow = NULL;
	m_pItemTransferWindow = NULL;

	/*school wars, Juver, 2018/01/19 */
	m_pPVPSchoolWarsScore = NULL;
	m_pPVPSchoolWarsScoreDummy = NULL;
	m_pRebirthDialogueSchoolWars = NULL;
	m_pPVPSchoolWarsTopRankings = NULL;
	m_pPVPSchoolWarsBattleNoticeStart = NULL;
	m_pPVPSchoolWarsBattleNoticeEnd = NULL;
	m_pPVPSchoolWarsRankings = NULL;

	/*pvp capture the flag, Juver, 2018/02/01 */
	m_pRebirthDialogueCaptureTheFlag = NULL;
	m_pPVPCaptureTheFlagTopRankings = NULL;
	m_pPVPCaptureTheFlagBattleNoticeStart = NULL;
	m_pPVPCaptureTheFlagBattleNoticeEnd = NULL;
	m_pPVPCaptureTheFlagScore = NULL;
	m_pPVPCaptureTheFlagRankings = NULL;
	m_pPVPCaptureTheFlagHoldIcon = NULL;

	/* car, cart color, Juver, 2018/02/14 */
	m_pCarColorWindow = NULL;

	/*info window, Juver, 2018/03/12 */
	m_pInfoWindow = NULL;

	/*item random option rebuild, Juver, 2018/07/03 */
	m_pItemRandomOptionRebuildInventoryWindow = NULL;
	m_pItemRandomOptionRebuildWindow = NULL;

	/*pvp reward info, Juver, 2018/07/25 */
	m_pPVPTyrannyRewardInfo = NULL;
	m_pPVPSchoolWarsRewardInfo = NULL;
	m_pPVPCaptureTheFlagRewardInfo = NULL;

	/*modern skill tray, Juver, 2018/08/21 */
	m_pSkillTrayTab = NULL;
	m_pSkillTrayTabMini = NULL;
	m_pSkillTrayTabEx = NULL;
	m_pSkillTrayTabExMini = NULL;

	/*target information display, Juver, 2018/08/22 */
	m_ptarget_information_display = NULL;
	m_ptarget_information_display_dummy = NULL;

	/*specific item box, Juver, 2018/09/02 */
	m_pitem_box_specific_selection_window = NULL;

	/*pk combo, EJCode, 2018/11/15 */
	m_ppk_combo_display = NULL;

	m_pPVPClubWarRankingWindow = NULL;

	m_pCurrencyShopWindow = NULL;

	m_pCharacterSkillWindow = NULL;

	/* personal lock system, Juver, 2019/12/10 */
	m_pPersonalLockInfo = NULL;
	m_pPersonalLockPinCreate = NULL;
	m_pPersonalLockPinInput = NULL;
	m_pPersonalLockPinReset = NULL;
	m_pPersonalLockPinChange = NULL;
	m_pPersonalLockPinRecover = NULL;
	m_pPersonalLockPinRecoverResult = NULL;

	/* party finder, Juver, 2019/12/28 */
	m_pPartyFinderWindow = NULL;
	m_pPartyFinderQuestion = NULL;

	/* multi gate out, Juver, 2020/11/19 */
	m_pMultiMapMoveDisplay = NULL;
	m_pMultiMapMoveDisplayDummy = NULL;

	/* pvp club death match, Juver, 2020/11/26 */
	m_pRebirthDialogueClubDeathMatch = NULL;
	m_pPVPClubDeathMatchBattleNoticeStart = NULL;
	m_pPVPClubDeathMatchBattleNoticeEnd = NULL;
	m_pPVPClubDeathMatchScore = NULL;
	m_pPVPClubDeathMatchScoreDummy = NULL;
	m_pPVPClubDeathMatchTopRankings = NULL;
	m_pPVPClubDeathMatchRankings = NULL;
	m_pPVPClubDeathMatchDoublePointDisplay = NULL;
	m_pPVPClubDeathMatchRewardInfo = NULL;

	/* chaos machine, Juver, 2021/07/08 */
	m_pChaosMachineWindow = NULL;
	m_pChaosMachineInvenWindow = NULL;


	m_pPKMarkDisplay = NULL;

	m_pContributionShopWindow = NULL; 
	m_pMaxRvPreviewWindow = NULL;

	m_pAutoPotionWindow = NULL;

	m_pAutoPilotWindow = NULL;

	m_pCharacterRebornAWindow = NULL;

	m_pExchangeItemWindow = NULL;
	m_pExchangeItemInvenWindow = NULL;

	m_pCaptchaWindow = NULL;
	/* Gacha System, MontageDev 7/10/24 */
	m_pGachaWindow = NULL;
	m_pGachaButton = NULL;

	m_pPVPPBGScore = NULL;
	m_pPVPPBGScoreDummy = NULL;
	m_pPVPPBGRebirthDialogue = NULL;
	m_pPVPPBGTopRankings = NULL;
	m_pPVPPBGRankings = NULL;

	m_pTGRankingDisplay = NULL;

}

HRESULT CInnerInterface::OneTimeSceneInit ()
{
	switch ( RPARAM::emSERVICE_TYPE )
	{
	case EMSERVICE_DEFAULT:
	case EMSERVICE_KOREA:
	case EMSERVICE_FEYA:
	case EMSERVICE_MALAYSIA_CN:
	case EMSERVICE_MALAYSIA_EN:
	case EMSERVICE_JAPAN:
	case EMSERVICE_THAILAND:
	case EMSERVICE_CHINA:
	case EMSERVICE_INDONESIA:
	case EMSERVICE_PHILIPPINES:
	case EMSERVICE_VIETNAM:
	case EMSERVICE_GLOBAL:
		m_bCHANNEL = true;
		break;

	default:
		m_bCHANNEL = false;
		break;
	};

	return S_OK;
}

HRESULT CInnerInterface::FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime )
{
	GASSERT( pd3dDevice );

	// 탈것 딜레이 측정
	m_fVehicleDelay += fElapsedTime;

	//	무조건 안 보이게 만듦.
	//	사용시에 Update()에서 켜고 동작.
	HideGroup ( INFO_DISPLAY_MAIN );
	HideGroup ( INFO_DISPLAY_SUB );
	HideGroup ( INFO_DISPLAY_ITEM_LINK ); /*item link, Juver, 2017/07/31 */

	target_information_reset();		/*target information display, Juver, 2018/08/22 */

	/*pvp tyranny, Juver, 2017/08/25 */
	HideGroup ( PVP_TYRANNY_TOWER_PROGRESS ); 

	/*woe Arc Development 08-06-2024*/
	HideGroup(PVP_WOE_TOWER_PROGRESS);
	ResetTargetInfo ();		//	타겟 정보창 리셋
	ResetCharMoveBlock ();	//	캐릭터 움직임 잠금 풀기	
	ResetSnapItem ();		//	아이템 스냅 리셋
	ResetFirstItemSlot ();	//	아이템슬롯 붙기 리셋

	m_bInfoDisplayActive = false;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */

	// 아이템 뱅크창 딜레이 측정
	if( !IsVisibleGroup( ITEMBANK_WINDOW ) && m_fItemBankDelay < 5.0f )
	{
		m_fItemBankDelay += fElapsedTime;
	}

	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	if( !IsVisibleGroup( ITEMSHOP_WINDOW ) && m_fItemShopDelay < 5.0f )
	{
		m_fItemShopDelay += fElapsedTime;
	}
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
	bool bKEYBOARD_BLOCK = false;
	if( IsVisibleGroup ( MODAL_WINDOW ) ||
		IsVisibleGroup ( REBIRTH_DIALOGUE ) ||
		IsVisibleGroup ( REBIRTH_DIALOGUE_TYRANNY ) ||			/*pvp tyranny, Juver, 2017/08/25 */
		IsVisibleGroup ( REBIRTH_DIALOGUE_SCHOOLWARS ) ||		/*school wars, Juver, 2018/01/20 */
		IsVisibleGroup ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ) || /*pvp capture the flag, Juver, 2018/02/01 */
		IsVisibleGroup ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH ) ||	/* pvp club death match, Juver, 2020/11/26 */
		IsVisibleGroup ( REBIRTH_DIALOGUE_PBG ) ||
		IsVisibleGroup ( REBIRTH_DIALOGUE_WOE ) ||	/*woe Arc Development 08-06-2024*/
		//IsVisibleGroup ( HELP_WINDOW ) ||	// 웹 브라우저용, 아직 사용 안함 ( 준혁 )
		IsVisibleGroup ( ITEMSHOP_WINDOW ) ||
		DXInputString::GetInstance().IsOn () )
	{
		bKEYBOARD_BLOCK = true;
	}

	// WebBrowser가 반투명이 지원되지 않으므로
	// 필요한 상황에서는 HELP_WINDOW를 닫는다 ( 준혁 )
	if( CCommonWeb::Get()->GetCreate() )
	{
		if( IsVisibleGroup ( MODAL_WINDOW ) ||
			IsVisibleGroup ( REBIRTH_DIALOGUE ) ||
			IsVisibleGroup ( REBIRTH_DIALOGUE_TYRANNY ) ||		/*pvp tyranny, Juver, 2017/08/25 */
			IsVisibleGroup ( REBIRTH_DIALOGUE_SCHOOLWARS ) ||	/*school wars, Juver, 2018/01/20 */
			IsVisibleGroup ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ) || /*pvp capture the flag, Juver, 2018/02/01 */
			IsVisibleGroup ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH) || /* pvp club death match, Juver, 2020/11/26 */
			IsVisibleGroup ( REBIRTH_DIALOGUE_PBG ) ||
			IsVisibleGroup ( REBIRTH_DIALOGUE_WOE ) ||	/*woe Arc Development 08-06-2024*/
			IsVisibleGroup( ESC_MENU ) )
		{
			HideGroup( HELP_WINDOW );
			HideGroup( ITEMSHOP_WINDOW );
		}
	}

	if ( !bKEYBOARD_BLOCK ) UpdateShortcutBefore ();

	/*item link, Juver, 2017/07/31 */
	if ( m_sItemInfoLinkTemp.sNativeID != NATIVEID_NULL() )
	{
		InfoDisplayItemLinkShow( m_sItemInfoLinkTemp );
	}

	/*target information display, Juver, 2018/08/22 */
	if ( m_target_id_information.dwID != EMTARGET_NULL )
	{
		target_information_show();
		m_target_id_information.RESET();
	}

	HRESULT hr = S_OK;
	hr = CUIMan::FrameMove ( pd3dDevice, fTime, fElapsedTime );
	if ( FAILED ( hr ) ) return hr;

	/*item link, Juver, 2017/07/31 */
	if ( IsVisibleGroup ( INFO_DISPLAY_ITEM_LINK ) )
	{
		UIRECT rcGlobalPos1 = GetChat()->GetGlobalPos();
		UIRECT rcGlobalPos2 = m_pInfoDisplayItemLink->GetGlobalPos();

		float x = rcGlobalPos1.right + 2.0f;
		float y = rcGlobalPos1.bottom - rcGlobalPos2.sizeY;

		m_pInfoDisplayItemLink->SetGlobalPos(D3DXVECTOR2(x,y));
	}

	/*target information display, Juver, 2018/08/22 */
	if ( IsVisibleGroup ( TARGET_INFORMATION_DISPLAY ) )
	{
		if ( m_ptarget_information_display && m_ptarget_information_display_dummy )
		{
			UIRECT rcGlobalPos1 = m_ptarget_information_display_dummy->GetGlobalPos();
			UIRECT rcGlobalPos2 = m_ptarget_information_display->GetGlobalPos();

			float x = rcGlobalPos1.left + 2.0f;
			float y = rcGlobalPos1.bottom - rcGlobalPos2.sizeY;

			if ( IsVisibleGroup( MINIPARTY_WINDOW ) )
			{
				CUIControl* mini_party = FindControl( MINIPARTY_WINDOW );
				if ( mini_party )
				{
					if ( x + rcGlobalPos2.sizeX >= mini_party->GetGlobalPos().left )
					{
						x = mini_party->GetGlobalPos().left - rcGlobalPos2.sizeX - 2.0f;
					}
				}
			}

			m_ptarget_information_display->SetGlobalPos(D3DXVECTOR2(x,y));
		}
	}

	if( IsOpenWindowToMoveBlock() )
	{
		SetCharMoveBlock ();
	}

	UpdateStatus ();

	if ( !m_bInfoDisplayActive ) InfoDisplayReset ();
	if ( !bKEYBOARD_BLOCK )	UpdateShortcutAfter ();

	if ( IsExclusiveControl() ) SetCharMoveBlock ();

	int LB = (int)DxInputDevice::GetInstance().GetMouseState(DXMOUSE_LEFT);
	if ( (LB & DXKEY_UP) || (LB & DXKEY_DUP) )
	{
		ResetExclusiveControl();
	}

	MoveBasicInfoWindow ();

	CUIFocusContainer::UICONTROL_FOCUSLIST focus_list = GetFocusList()->GetFocusList();
	CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter = focus_list.rbegin ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter_end = focus_list.rend ();

	bool bBasicChatBoxShow = false;
	for ( ; riter != riter_end; ++riter )
	{
		UIGUID cID = riter->cID;
		if ( BASIC_INFO_VIEW == cID )
		{
			if ( bBasicChatBoxShow )
			{
				if ( !GetChat()->IsCHAT_BEGIN() )
				{
					GetFocusList()->InsertAfter ( BASIC_CHAT_BOX, BASIC_INFO_VIEW, riter->pUIControl );
				}
			}
			break;
		}
		else if ( BASIC_CHAT_BOX == cID )
		{
			bBasicChatBoxShow = true;
		}
		/*info window, Juver, 2018/03/12 */
		else if ( UI_INFO_WINDOW == cID )
		{
			CUIControl* pcontrol_tail = GetFocusList()->GetTail();
			if ( pcontrol_tail )
			{
				if ( pcontrol_tail->GetWndID() != cID )
				{
					GetFocusList()->InsertTail( cID, riter->pUIControl );
				}
			}
		}
		else if ( REBIRTH_DIALOGUE == cID )
		{
			CUIControl* pcontrol_tail = GetFocusList()->GetTail();
			if ( pcontrol_tail )
			{
				if ( pcontrol_tail->GetWndID() != cID )
				{
					GetFocusList()->InsertTail( cID, riter->pUIControl );
				}
			}
		}
	}

	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
		Actor& actor = pCharacter->GetActorMove ();
		if ( actor.PathIsActive () ) m_vCharDir = actor.Movement ();

		if ( IsVisibleGroup( PVP_COMPETITION_WINDOW ) )
		{
			if ( pCharacter->isPetLastUseTime() )
				SetCompetitionJoin( false );
			else
				SetCompetitionJoin( true );
		}
	}

	{
		if ( IsVisibleGroup ( INFO_DISPLAY_SUB ) )
		{
			UIRECT rcGlobalPos = m_pInfoDisplayMain->GetGlobalPos();
			UIRECT rcGlobalPosEx = m_pInfoDisplaySub->GetGlobalPos();
			float x = rcGlobalPos.right + 2.0f;
			float y = min(rcGlobalPos.top,rcGlobalPosEx.top);

			const long lResolution = GetResolution ();
			WORD X_RES = HIWORD ( lResolution );
			WORD Y_RES = LOWORD ( lResolution );

			if ( X_RES < x + rcGlobalPosEx.sizeX )
			{
				x = X_RES - rcGlobalPosEx.sizeX;

				m_pInfoDisplayMain->SetGlobalPos(D3DXVECTOR2(x-2.0f-rcGlobalPos.sizeX,y));
				m_pInfoDisplaySub->SetGlobalPos(D3DXVECTOR2(x,y));
			}
			else
			{
				m_pInfoDisplayMain->SetGlobalPos(D3DXVECTOR2(rcGlobalPos.left,y));
				m_pInfoDisplaySub->SetGlobalPos(D3DXVECTOR2(x,y));
			}
		}
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )	
#else
	/* modern party window, Juver, 2019/12/20 */
	if ( RPROFILE::bPartyWindowShowInfo )
	{
		if ( !IsVisibleGroup( PARTY_STATE_WINDOW ))
			ShowGroupBottom( PARTY_STATE_WINDOW );
	}
	else
	{
		if ( IsVisibleGroup( PARTY_STATE_WINDOW ))
			HideGroup( PARTY_STATE_WINDOW );
	}
#endif

	
	return hr;
}

void CInnerInterface::UpdateShortcutBefore ()
{	
	{
		if ( !m_pNameDisplayMan ) return ;		

		if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_LMENU, DXKEY_PRESSED ) )
		{
			for (int i= 0; i < CHAT_MACRO_SIZE; ++i){
				if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_1+i, DXKEY_DOWN ) )
				{			
					AddChatMacro(i);
					break;
				}
			}
		}



		if( RPROFILE::bNAME_DISPLAY )
		{
			if( IsVisibleGroup( NAME_DISPLAY_MAN ) )
			{
				if( m_pNameDisplayMan->GetNameType() != NAME_DISPLAY_MOBCHAR )
				{
					HideGroup( NAME_DISPLAY_MAN );
					m_pNameDisplayMan->SetNameType( NAME_DISPLAY_MOBCHAR );
					ShowGroupBottom( NAME_DISPLAY_MAN );
				}
			}
			else
			{
				m_pNameDisplayMan->SetNameType( NAME_DISPLAY_MOBCHAR );
				ShowGroupBottom( NAME_DISPLAY_MAN );
			}

			return ;
		}

		HideGroup ( NAME_DISPLAY_MAN );		
		if ( UIKeyCheck::GetInstance()->Check ( DIK_LMENU, DXKEY_PRESSED ) )
		{
			m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_ITEMMONEY );
			ShowGroupBottom ( NAME_DISPLAY_MAN );
		}
		else if ( UIKeyCheck::GetInstance()->Check ( DIK_LCONTROL, DXKEY_PRESSED ) )
		{
			m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_MOBCHAR );
			ShowGroupBottom ( NAME_DISPLAY_MAN );
		}
		else if ( UIKeyCheck::GetInstance()->Check ( DIK_LSHIFT, DXKEY_PRESSED ) )
		{
			m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_ALL );
			ShowGroupBottom ( NAME_DISPLAY_MAN );
		}
		else
		{
			//	BEGIN : CLUB CONFT SETTING
			const EMCONFT_TYPE emTYPE = GLGaeaClient::GetInstance().GetCharacter()->GetConfting().emTYPE;
			if ( emTYPE == EMCONFT_GUILD )
			{
				m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_CLUB_CONFT );
				ShowGroupBottom ( NAME_DISPLAY_MAN );                
				return ;
			}

			//	파티
			GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();	//	마스터
			if ( pMaster )
			{
				GLCharacter *pMyCharacter = GLGaeaClient::GetInstance().GetCharacter();			

				if ( pMyCharacter->GetConfting().IsCONFRONTING() )	//	파티 대련 중인가?
				{
					m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_PARTY_CONFT );			
				}
				else	//	파티 구성 된 경우
				{
					m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_PARTY );		
				}
				ShowGroupBottom ( NAME_DISPLAY_MAN );
			}
			else
			{
				//	무조건 뿌림
				m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_DEFAULT );
				ShowGroupBottom ( NAME_DISPLAY_MAN );
			}
		}
	}	
}

void CInnerInterface::UpdateShortcutAfter ()
{
	if ( UIKeyCheck::GetInstance()->Check ( DIK_ESCAPE, DXKEY_DOWN ) )
	{
		bool bALLHIDE = true;

		//	NOTE
		//		동시에 두 곳에서 FocusList에
		//		접근할 가능성이 없다는 가정이 있어야 한다.
		//		위험하군 ㅡ.ㅡ
		CUIFocusContainer::UICONTROL_FOCUSLIST list = GetFocusList()->GetFocusList ();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter = list.rbegin ();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter_end = list.rend ();
		for ( ; riter != riter_end; ++riter )
		{
			bool bAnotherProcess = false;
			UIGUID cID = riter->cID;

			/*pvp tyranny, Juver, 2017/08/25 */
			/*school wars, Juver, 2018/01/20 */
			/*pvp capture the flag, Juver, 2018/02/02 */
			/* modern party window, Juver, 2019/12/20 */
			/* pvp club death match, Juver, 2020/11/29 */
			if ( IsVisibleGroup ( cID ) && 
				cID != BASIC_CHAT_BOX && 
				cID != BASIC_INFO_VIEW &&
				cID != PRIVATE_MARKET_SHOW_MAN && 
				cID != MINIMAP &&
				cID != PVP_TYRANNY_RANKINGS &&
				cID != PVP_WOE_RANKINGS &&	
				cID != PVP_SCHOOLWARS_RANKINGS &&
				cID != PVP_CAPTURE_THE_FLAG_RANKINGS &&
				cID != PVP_CLUB_DEATH_MATCH_RANKINGS &&
				cID != PARTY_STATE_WINDOW &&
				cID != PVP_PBG_RANKINGS )		
			{
				if ( cID == INVENTORY_WINDOW || cID == MARKET_WINDOW ||
					cID == STORAGE_WINDOW || cID == CLUB_STORAGE_WINDOW )
				{
					SetMarketWindowClose ();
					SetStorageWindowClose ();
					SetClubStorageWindowClose ();
				}
				else if ( cID == TRADEINVENTORY_WINDOW )
				{
					if ( IsVisibleGroup ( TRADE_WINDOW ) )
					{
						SetTradeWindowCloseReq ();
					}
					else if ( IsVisibleGroup ( PRIVATE_MARKET_WINDOW ) )
					{
						if ( m_pPrivateMarketWindow->IsOPENER () )
						{
							DoModal ( ID2GAMEINTEXT("PRIVATE_MARKET_QUIT"), MODAL_QUESTION, YESNO, MODAL_PRIVATE_MARKET_QUIT );
						}
						else
						{
							SetPrivateMarketClose ();
						}
						bAnotherProcess = true;
					}
				}
				else if ( cID == TRADE_WINDOW )
				{							
					SetTradeWindowCloseReq ();
				}
				else if ( cID == MINIPARTY_WINDOW )
				{
					ShowGroupBottom ( MINIPARTY_OPEN );
// #ifdef CH_PARAM // 중국 인터페이스 변경
//					UpdatePotionTrayPosition();
// #endif
				}
				else if ( cID == PRIVATE_MARKET_WINDOW )
				{
					if ( m_pPrivateMarketWindow->IsOPENER () )
					{
						DoModal ( ID2GAMEINTEXT("PRIVATE_MARKET_QUIT"), MODAL_QUESTION, YESNO, MODAL_PRIVATE_MARKET_QUIT );
					}
					else
					{
						SetPrivateMarketClose ();
					}
					bAnotherProcess = true;					
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
				else if( cID == EXCHANGE_ITEM_WINDOW || cID == EXCHANGE_ITEM_INVEN_WINDOW )
				{
					CloseExchangeItemWindow();
				}
				else if( cID == GAMBLE_DIALOGUE || cID == GAMBLE_SELECT_DIALOGUE || 
						cID == GAMBLE_AGAIN_DIALOGUE || cID == GAMBLE_ANIMATION_DIALOGUE || 
						cID == GAMBLE_RESULT_EVEN_DIALOGUE || cID == GAMBLE_RESULT_ODD_DIALOGUE  )
				{
					HideGroup( INVENTORY_WINDOW );
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if( pCharacter )
					{
						pCharacter->ReqMGameOddEvenCancel();
					}
				}
				else if ( cID == QUEST_WINDOW )
				{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 )
					int nPage = m_pQuestWindow->GET_SHOW_PAGE ();
					switch ( nPage )
					{
					case CQuestWindow::QUEST_STEP:
						{							
							m_pQuestWindow->SET_SHOW_PAGE ( CQuestWindow::QUEST_MAIN );
							bAnotherProcess = true;
						}
						break;

					case CQuestWindow::QUEST_MAIN:
						{												
							m_pQuestWindow->SET_SHOW_PAGE ( CQuestWindow::QUEST_LIST );
							bAnotherProcess = true;
						}
						break;
					}	
#endif
				}
				/*pvp tyranny, Juver, 2017/08/25 */
				/*school wars, Juver, 2018/01/20 */
				/*pvp capture the flag, Juver, 2018/02/01 */
				/* pvp club death match, Juver, 2020/11/26 */
				else if ( cID == WAITSERVER_DISPLAY || 
					cID == REBIRTH_DIALOGUE || 
					cID == MAP_REQUIRE_CHECK ||
					cID == REBIRTH_DIALOGUE_TYRANNY || 
					cID == REBIRTH_DIALOGUE_SCHOOLWARS || 
					cID == REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ||
					cID == REBIRTH_DIALOGUE_CLUB_DEATH_MATCH ||
					cID == REBIRTH_DIALOGUE_PBG ||
					cID == REBIRTH_DIALOGUE_WOE )
				{
					bAnotherProcess = true;
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
				
				/////////////////////////////////////////////////////////////////////////////
				// LG-7 GlobalRanking
				else if ( cID == GLOBAL_RANKING_WINDOW )	SetGlobalRankingWindowOpen(FALSE);
				/////////////////////////////////////////////////////////////////////////////

				else if ( (MODAL_WINDOW + 1) <= cID )
				{
					//					CLOSE_MODAL ( cID, false );
					bAnotherProcess = true;
				}
				

				if ( !bAnotherProcess )	
				{
					HideGroup ( cID );
				}

				bALLHIDE = false;
				break;
			}
		}

		/*item link, Juver, 2017/07/31 */
		if ( bALLHIDE )
		{
			if( IsVisibleGroup( INFO_DISPLAY_ITEM_LINK ) )
			{
				InfoDisplayItemLinkClearText ();
				bALLHIDE = FALSE;
			}
		}

		if ( bALLHIDE )
		{
			if ( !IsVisibleGroup ( ESC_MENU ) )
				ShowGroupFocus ( ESC_MENU );
		}
	}

	// Tab Key 예약중이라면
	if ( m_bTabReserve ) 
	{
		if ( !GLGaeaClient::GetInstance().GetCharacter()->IsACTION(GLAT_SKILL) )
		{
			GLGaeaClient::GetInstance().GetCharacter()->ReqSlotChange();
			m_bTabReserve = false;
		}
	}	
	else 
	{
		if ( UIKeyCheck::GetInstance()->Check( DIK_TAB, DXKEY_DOWN ) )
		{
			EMCHARCLASS emClass = GLGaeaClient::GetInstance().GetCharacter()->m_emClass;

			if( emClass&GLCC_EXTREME_M || emClass&GLCC_EXTREME_W )
			{
				// 스킬 사용중으로 실패하면 Tab Key 예약
				if ( E_FAIL == GLGaeaClient::GetInstance().GetCharacter()->ReqSlotChange() )
					m_bTabReserve = true;
			}
				
		}
	}



	if ( IsVisibleGroup ( ESC_MENU ) )				return ;
	if ( IsVisibleGroup ( HELP_WINDOW ) )			return ;
	if ( IsVisibleGroup ( ITEMSHOP_WINDOW ) )		return ;
	if ( IsVisibleGroup ( KEY_SETTING_WINDOW ) )	return ;

	if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_V, DXKEY_DOWN ) )
	{
		if ( GLTradeClient::GetInstance().Valid() ) return;
		GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter ();

		if ( pCharacter->m_sVehicle.IsActiveValue() && m_fVehicleDelay < 1.0f )	
		{
			PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_DELAY") );
			return;
		}

		pCharacter->ReqSetVehicle( !pCharacter->m_bVehicle ); // 활성 비활성
		m_fVehicleDelay = 0.0f;
	}

	/*vehicle booster system, Juver, 2017/08/11 */
	if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_Z, DXKEY_DOWN ) )
	{
		GLGaeaClient::GetInstance().GetCharacter ()->VehicleBoosterStart();
	}

	if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_LMENU, DXKEY_PRESSED ) )
	{
		for (int i= 0; i < CHAT_MACRO_SIZE; ++i){
			if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_1+i, DXKEY_DOWN ) )
			{			
				return ;
			}
		}
	}

	GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
	if ( pCharacter )
	{
		//	퀵 스킬
		for( int i = 0; i < QUICK_SKILL_SLOT_MAX; ++i)
		{
            if ( UIKeyCheck::GetInstance()->Check (RPROFILE::SkillSlot[i], DXKEY_DOWN ) )
			{
				INT nIndex = skill_tray_get_tab_index() * QUICK_SKILL_SLOT_MAX + i;
				pCharacter->ReqSkillRunSet( nIndex );
			}
		}
	}
	
	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_INVEN], DXKEY_DOWN ) )
	{		
		/*modern inventory window, Juver, 2018/04/07 */
		if ( !IsVisibleGroup ( INVENTORY_WINDOW ) && !IsInventoryBlocked () )
		{
			ShowGroupFocus( INVENTORY_WINDOW );
		}
		else
		{
			HideGroup ( INVENTORY_WINDOW );
		}
	}

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_SKILL], DXKEY_DOWN ) )
	{		
		if ( !IsVisibleGroup ( SKILL_WINDOW ) )
			ShowGroupFocus ( SKILL_WINDOW );
		else
			HideGroup ( SKILL_WINDOW );
	}

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_PARTY], DXKEY_DOWN ) )
	{		
		if ( !IsVisibleGroup ( PARTY_WINDOW ) )
			ShowGroupFocus ( PARTY_WINDOW );
		else
			HideGroup ( PARTY_WINDOW );
	}

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_CHAR], DXKEY_DOWN ) )
	{		
		if ( !IsVisibleGroup ( CHARACTER_WINDOW ) )
			ShowGroupFocus ( CHARACTER_WINDOW );
		else
			HideGroup ( CHARACTER_WINDOW );
	}

	if ( UIKeyCheck::GetInstance()->CheckSimple (RPROFILE::MenuShortcut[SHORTCUT_MAP], DXKEY_DOWN ) )
	{
		if ( !IsVisibleGroup ( LARGEMAP_WINDOW ) )
			ShowGroupFocus ( LARGEMAP_WINDOW );
		else
			HideGroup ( LARGEMAP_WINDOW );
	}

	if ( UIKeyCheck::GetInstance()->CheckSimple (RPROFILE::MenuShortcut[SHORTCUT_CLUB], DXKEY_DOWN ) )
	{
		if ( !IsVisibleGroup ( CLUB_WINDOW ) )
			ShowGroupFocus ( CLUB_WINDOW );
		else
			HideGroup ( CLUB_WINDOW );
	}

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_QUEST], DXKEY_DOWN ) )
	{
		if ( !IsVisibleGroup ( QUEST_WINDOW ) )
		{			
			if ( NATIVEID_NULL().dwID == m_dwEventQuestID )
			{
				ShowGroupFocus ( QUEST_WINDOW );
				REFRESH_QUEST_WINDOW ();				
			}
			else
			{
				SetQuestWindowOpen ( m_dwEventQuestID );
			}
		}
		else
		{
			HideGroup ( QUEST_WINDOW );
		}	
	}

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_HELP], DXKEY_DOWN) )
	{
		if ( !IsVisibleGroup ( HELP_WINDOW ) )
		{
			if ( RPARAM::bUseHelpWindow )
			{
				HideGroup ( HELP_WINDOW );
				ShowGroupFocus ( HELP_WINDOW );
			}
			else
			{
				HideGroup ( HELP_WINDOW );
			}
		}

	}

#if defined ( CH_PARAM ) || defined ( TH_PARAM ) || defined( PH_PARAM ) || defined ( JP_PARAM ) || defined( _RELEASED ) // 필리핀 아이템샵
	if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_ITEMSHOP], DXKEY_DOWN) )
	{
		if ( !IsVisibleGroup ( ITEMSHOP_WINDOW ) )
		{
			//	ItemShopAuth
			if ( ItemShopAuth() )
			{
				ShowGroupFocus ( ITEMSHOP_WINDOW );
			}

			HideGroup ( HELP_WINDOW );
		}
		else
		{
			HideGroup ( ITEMSHOP_WINDOW );
		}
	}
	
	//	ItemShopAuth	
	ItemShopVisible();

#endif	

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_FRIEND], DXKEY_DOWN ) )
	{
		if ( IsVisibleGroup ( FRIEND_WINDOW ) )		HideGroup ( FRIEND_WINDOW );			
		else										ShowGroupFocus ( FRIEND_WINDOW );
	}

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_ITEMBANK], DXKEY_DOWN ) )
	{
		if ( IsVisibleGroup ( ITEMBANK_WINDOW ) )	HideGroup ( ITEMBANK_WINDOW );
		else										SetItemBankWindowOpen ();

		/*if ( !IsVisibleGroup ( ITEMBANK_WINDOW ) )	
		{
			SetItemBankWindowOpen();			
		}*/
	}

	/*if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_VIETNAMGAINSYS], DXKEY_DOWN ) )
	{
		if ( IsVisibleGroup ( VNGAINSYS_WINDOW ) )	HideGroup ( VNGAINSYS_WINDOW );
		else										SetVNGainSysWindowOpen();
	}*/

	if ( UIKeyCheck::GetInstance()->Check (RPROFILE::MenuShortcut[SHORTCUT_CHATMACRO], DXKEY_DOWN ) )
	{
		if ( IsVisibleGroup ( CHATMACRO_WINDOW ) )
		{
			HideGroup ( CHATMACRO_WINDOW );
		}
		else
		{
			GetChatMacro();
			ShowGroupFocus ( CHATMACRO_WINDOW );
		}
	}



	if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_LMENU, DXKEY_PRESSED ) )
	{
		if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_F4, DXKEY_DOWN ) )
		{			
			DoModal ( ID2GAMEINTEXT("CLOSE_GAME"), MODAL_QUESTION, OKCANCEL, MODAL_CLOSEGAME );
		}
	}

//#ifndef CH_PARAM // 중국 인터페이스 변경
	if( UIKeyCheck::GetInstance()->Check ( DIK_GRAVE, DXKEY_DOWN ) ||
		UIKeyCheck::GetInstance()->Check ( DIK_Y, DXKEY_DOWN ) )
	{
		/*modern skill tray, Juver, 2018/08/21 */
		m_skill_tray_mini = !m_skill_tray_mini;
		skill_tray_switch();
	}
//#endif

	if( UIKeyCheck::GetInstance()->Check( DIK_F1, DXKEY_DOWN ) )	
	{
		m_pSkillTrayTab->SetTabIndex( 0 );
		m_pSkillTrayTabEx->SetTabIndex( 0 );
	}

	if( UIKeyCheck::GetInstance()->Check( DIK_F2, DXKEY_DOWN ) )
	{
		m_pSkillTrayTab->SetTabIndex( 1 );
		m_pSkillTrayTabEx->SetTabIndex( 1 );
	}

	if( UIKeyCheck::GetInstance()->Check( DIK_F3, DXKEY_DOWN ) )	
	{
		m_pSkillTrayTab->SetTabIndex( 2 );
		m_pSkillTrayTabEx->SetTabIndex( 2 );
	}

	if( UIKeyCheck::GetInstance()->Check( DIK_F4, DXKEY_DOWN ) )	
	{
		m_pSkillTrayTab->SetTabIndex( 3 );
		m_pSkillTrayTabEx->SetTabIndex( 3 );
	}

	if( UIKeyCheck::GetInstance()->Check(RPROFILE::MenuShortcut[SHORTCUT_PET], DXKEY_DOWN ) )
	{
		if( GLGaeaClient::GetInstance().GetPetClient()->IsVALID() )
		{
			if ( GLTradeClient::GetInstance().Valid() ) return;
			
			if ( IsVisibleGroup( PET_WINDOW ) )		HideGroup ( PET_WINDOW );
			else									ShowGroupFocus( PET_WINDOW );

			
		}
	}


	if( UIKeyCheck::GetInstance()->Check( DIK_N, DXKEY_DOWN ) )
	{
		if( GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.IsActiveValue() )
		{
			if ( GLTradeClient::GetInstance().Valid() ) return;
			
			if ( IsVisibleGroup( VEHICLE_WINDOW ) )	HideGroup ( VEHICLE_WINDOW );
			else									ShowGroupFocus( VEHICLE_WINDOW );

			
		}
	}

	/*skill effect setting, Juver, 2017/10/01 */
	if( UIKeyCheck::GetInstance()->Check( DIK_F10, DXKEY_DOWN ) )	
	{
		ROPTION::bHideSkillEffect = !ROPTION::bHideSkillEffect;

		if (ROPTION::bHideSkillEffect )
			PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("HIDE_SKILL_EFFECT_ON") );
		else
			PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("HIDE_SKILL_EFFECT_OFF") );
	}

	if( UIKeyCheck::GetInstance()->Check( DIK_F11, DXKEY_DOWN ) )	
	{
		GLLandManClient* pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
		if ( pLandClient && pLandClient->IsAllowHideDead() == false )
		{
			//warn player that hide dead has no effect for the current map
			PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("HIDE_DEAD_NOT_ALLOWED") );
		}
		else
		{
			int nIndex = ROPTION::emHideDead;
			nIndex ++;
			if ( nIndex >= ROPTION::EMHIDE_DEAD_SIZE )
				nIndex = 0;

			ROPTION::emHideDead = (ROPTION::EMHIDE_DEAD)nIndex;

			if (ROPTION::emHideDead == ROPTION::EMHIDE_DEAD_NONE)
				PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMHIDE_DEAD_NONE") );
			else if (ROPTION::emHideDead == ROPTION::EMHIDE_DEAD_ALL)
				PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMHIDE_DEAD_ALL") );
			else if (ROPTION::emHideDead == ROPTION::EMHIDE_DEAD_OTHER_TEAM)
				PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMHIDE_DEAD_OTHER_TEAM") );
		}
	}


	///*hide costume, EJCode, 2018/11/18 */
	//if( UIKeyCheck::GetInstance()->Check( DIK_F11, DXKEY_DOWN ) )	
	//{
	//	GLGaeaClient::GetInstance().GetCharacter()->hide_costume_set();
	//}

	/*if( UIKeyCheck::GetInstance()->Check( DIK_F5, DXKEY_DOWN ) )	
	{
		PVPClubWarRankingWindowOpen();
	}*/

	/*if ( UIKeyCheck::GetInstance()->Check ( DIK_F7, DXKEY_DOWN) )
	{
		if (IsVisibleGroup(BATTLE_RANKING_DISPLAY))	SetBattleRankingDisplayOpen(FALSE);
		else										SetBattleRankingDisplayOpen(TRUE);
	}*/
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	if( UIKeyCheck::GetInstance()->Check( DIK_F6, DXKEY_DOWN ) )
	{
		PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
		if( pLand->m_bClubBattle || pLand->m_bClubBattleHall )
		{
			RPARAM::bHideMiniRank = !RPARAM::bHideMiniRank;
			if( RPARAM::bHideMiniRank )
			{
				PrintMsgText ( NS_UITEXTCOLOR::YELLOW, "Show Clubwar Ranking");
				ShowGroupBottom ( CLUB_WAR_RANKING_DISPLAY );
			}
			else if( !RPARAM::bHideMiniRank )
			{
				PrintMsgText ( NS_UITEXTCOLOR::RED, "Hide Clubwar Ranking");
				HideGroup( CLUB_WAR_RANKING_DISPLAY );
			}
		}

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
		else if ( pLand->m_bPVPTyrannyMap )
		{
			RPARAM::bHideMiniRank = !RPARAM::bHideMiniRank;
			if( RPARAM::bHideMiniRank )
			{
				PrintMsgText ( NS_UITEXTCOLOR::YELLOW, "Show Tyranny Ranking");
				ShowGroupBottom ( PVP_TYRANNY_RANKING_DISPLAY );
			}
			else if( !RPARAM::bHideMiniRank )
			{
				PrintMsgText ( NS_UITEXTCOLOR::RED, "Hide Tyranny Ranking");
				HideGroup( PVP_TYRANNY_RANKING_DISPLAY );
			}
		}
		else if ( pLand->m_bPVPWoeMap )
		{
			RPARAM::bHideMiniRank = !RPARAM::bHideMiniRank;
			if( RPARAM::bHideMiniRank )
			{
				PrintMsgText ( NS_UITEXTCOLOR::YELLOW, "Show Woe Ranking");
				ShowGroupBottom ( WOE_RANKING_WINDOW );
			}
			else if( !RPARAM::bHideMiniRank )
			{
				PrintMsgText ( NS_UITEXTCOLOR::RED, "Hide Woe Ranking");
				HideGroup( WOE_RANKING_WINDOW );
			}
		}
		else 
		{
			PrintMsgText ( NS_UITEXTCOLOR::DISABLE, "You're not in the War-Maps to show Rankings");
		}
	}

	/* party finder, Juver, 2019/12/29 */
	if( UIKeyCheck::GetInstance()->Check( DIK_F8, DXKEY_DOWN ) )	
	{
		if ( IsVisibleGroup ( PARTY_FINDER_WINDOW ) )		
			HideGroup ( PARTY_FINDER_WINDOW );			
		else										
			OpenPartyFinderWindow();
	}

	if( UIKeyCheck::GetInstance()->Check( DIK_F9, DXKEY_DOWN ) )	
	{
		CharacterSimplifyChange();
	}

	/* Combined Ranking Button Hide - Show TW CW - Jxyy */
	/*if (UIKeyCheck::GetInstance()->Check(DIK_F6, DXKEY_DOWN))
	{
    	PLANDMANCLIENT plandmanclient = GLGaeaClient::GetInstance().GetActiveMap();
    
    	if (plandmanclient)
    	{
        	bool bClubWars = plandmanclient->m_bClubBattle;
        	bool bPVPTyranny = plandmanclient->m_bPVPTyrannyMap;
			bool bPVPWoe = plandmanclient->m_bPVPWoeMap;

        	// Handle Club Wars
        	if (bClubWars)
        	{
            	bool isVisibleCW = m_pClubWarRankingDisplay->IsVisible();
            	if (isVisibleCW)
            	{
            	    HideGroup(CLUBWAR_RANKING_DISPLAY);
            	    PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("JXYY_CWRANKING_OFF"));
            	}
            	else
            	{
                	ShowGroupBottom(CLUBWAR_RANKING_DISPLAY);
                	PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("JXYY_CWRANKING_ON"));
            	}
        	}
        	// Handle Tyranny Wars
        	else if (bPVPTyranny)
        	{
            	bool isVisibleTW = m_pTyrannyWarRankingDisplay->IsVisible();
            	if (isVisibleTW)
            	{
            	    HideGroup(TYRANNY_RANKING_DISPLAY);
            	    PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("JXYY_TWRANKING_OFF"));
            	}
            	else
            	{
                	ShowGroupBottom(TYRANNY_RANKING_DISPLAY);
                	PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("JXYY_TWRANKING_ON"));
            	}
        	}
			else if (bPVPWoe)
        	{
            	bool isVisibleWOE = m_pWoeRanking->IsVisible();
            	if (isVisibleWOE)
            	{
            	    HideGroup(WOE_RANKING_WINDOW);
            	    PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("JXYY_WOERANKING_OFF"));
            	}
            	else
            	{
                	ShowGroupBottom(WOE_RANKING_WINDOW);
                	PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("JXYY_WOERANKING_ON"));
            	}
        	}
        	// Not in either Club War or Tyranny War
        	else
        	{
        	    PrintMsgText(NS_UITEXTCOLOR::ORNAGERED, ID2GAMEINTEXT("JXYY_NOT_IN_CW_OR_TW"));
        	}
    	}
    	else
    	{
    	    PrintMsgText(NS_UITEXTCOLOR::ORNAGERED, ID2GAMEINTEXT("JXYY_NOT_IN_CW_OR_TW"));
    	}
	}*/
}

void CInnerInterface::UpdateStatus ()
{
	GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter();

	if ( IsVisibleGroup ( SYSTEM_MESSAGE_WINDOW ) )
	{
		UIRECT rcGlobalPos = m_pSystemMessageWindowDummy->GetGlobalPos ();
		//rcGlobalPos.top += fMENU_LIFTUP;
		m_pSystemMessageWindow->SetGlobalPos ( rcGlobalPos );
	}

	if ( IsBlockProgramAlarm () )
	{		
		if ( !IsVisibleGroup ( BLOCK_PROGRAM_ALARM ) )
		{
			UIRECT rcGlobalPos = m_pBlockProgramAlarmDummy->GetGlobalPos ();
			rcGlobalPos.top += float((rand()%200) - 100);
			rcGlobalPos.left += float((rand()%200) - 100);
			m_pBlockProgramAlarm->SetGlobalPos ( rcGlobalPos );

			GetPositionKeeper()->SetPosition ( BLOCK_PROGRAM_ALARM, rcGlobalPos );

			ShowGroupFocus ( BLOCK_PROGRAM_ALARM );
		}
	}
	else
	{
		HideGroup ( BLOCK_PROGRAM_ALARM );
	}
	
	
	//	캐릭터 생존 상태 확인
	if ( pCharacter->IsDie () )
	{
		bool bPVPTyranny = false;			/*pvp tyranny, Juver, 2017/08/25 */
		bool bPVPSchoolWars = false;		/*school wars, Juver, 2018/01/19 */
		bool bPVCaptureTheFlag = false;		/*pvp capture the flag, Juver, 2018/01/31 */
		bool bPVPClubDeathMatch = false;	/* pvp club death match, Juver, 2020/11/26 */
		bool bPVPPBG = false;
		bool bPVPWoe = false;	/*woe Arc Development 08-06-2024*/

		PLANDMANCLIENT plandmanclient = GLGaeaClient::GetInstance().GetActiveMap();
		if ( plandmanclient )
		{
			bPVPTyranny = plandmanclient->m_bPVPTyrannyMap;
			bPVPSchoolWars = plandmanclient->m_bPVPSchoolWarsMap;
			bPVCaptureTheFlag = plandmanclient->m_bPVPCaptureTheFlagMap;
			bPVPWoe = plandmanclient->m_bPVPWoeMap;	/*woe Arc Development 08-06-2024*/
			
			/* pvp club death match, Juver, 2020/11/26 */
			if ( plandmanclient->m_bPVPClubDeathMatchLobbyMap || plandmanclient->m_bPVPClubDeathMatchBattleMap )
				bPVPClubDeathMatch = true;

			if ( plandmanclient->m_bPVPPBGLobbyMap || plandmanclient->m_bPVPPBGBattleMap )
				bPVPPBG = true;
		}

		/*pvp tyranny, Juver, 2017/08/25 */
		if ( bPVPTyranny )
		{
			if ( !IsVisibleGroup( REBIRTH_DIALOGUE_TYRANNY ) )
			{
				if ( m_pRebirthDialogueTyranny )
					m_pRebirthDialogueTyranny->timeStart = CTime::GetCurrentTime().GetTime();

				ShowGroupFocus ( REBIRTH_DIALOGUE_TYRANNY, true );
				ShowGroupFocus ( BASIC_CHAT_BOX, true );

				if ( IsVisibleGroup( PVP_TYRANNY_RANKINGS ) )
				{
					HideGroup( PVP_TYRANNY_RANKINGS );
					m_bPVPTyrannyShowRanking = TRUE;
				}
			}
		}
		/*school wars, Juver, 2018/01/19 */
		else if ( bPVPSchoolWars )
		{
			if ( !IsVisibleGroup( REBIRTH_DIALOGUE_SCHOOLWARS ) )
			{
				if ( m_pRebirthDialogueSchoolWars )
					m_pRebirthDialogueSchoolWars->timeStart = CTime::GetCurrentTime().GetTime();

				ShowGroupFocus ( REBIRTH_DIALOGUE_SCHOOLWARS, true );
				ShowGroupFocus ( BASIC_CHAT_BOX, true );

				if ( IsVisibleGroup( PVP_SCHOOLWARS_RANKINGS ) )
				{
					HideGroup( PVP_SCHOOLWARS_RANKINGS );
					m_bPVPSchoolWarsShowRanking = TRUE;
				}
			}
		}
		/*pvp capture the flag, Juver, 2018/02/01 */
		else if ( bPVCaptureTheFlag )
		{
			if ( !IsVisibleGroup( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ) )
			{
				if ( m_pRebirthDialogueCaptureTheFlag )
					m_pRebirthDialogueCaptureTheFlag->timeStart = CTime::GetCurrentTime().GetTime();

				ShowGroupFocus ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG, true );
				ShowGroupFocus ( BASIC_CHAT_BOX, true );

				if ( IsVisibleGroup( PVP_CAPTURE_THE_FLAG_RANKINGS ) )
				{
					HideGroup( PVP_CAPTURE_THE_FLAG_RANKINGS );
					m_bPVPCaptureTheFlagShowRanking = TRUE;
				}
			}
		}
		/* pvp club death match, Juver, 2020/11/26 */
		else if ( bPVPClubDeathMatch )
		{
			if ( !IsVisibleGroup( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH ) )
			{
				if ( m_pRebirthDialogueClubDeathMatch )
					m_pRebirthDialogueClubDeathMatch->timeStart = CTime::GetCurrentTime().GetTime();

				ShowGroupFocus ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH, true );
				ShowGroupFocus ( BASIC_CHAT_BOX, true );

				if ( IsVisibleGroup( PVP_CLUB_DEATH_MATCH_RANKINGS ) )
				{
					HideGroup( PVP_CLUB_DEATH_MATCH_RANKINGS );
					m_bPVPClubDeathMatchShowRanking = TRUE;
				}
			}
		}
		else if ( bPVPPBG )
		{
			if ( !IsVisibleGroup( REBIRTH_DIALOGUE_PBG ) )
			{
				if ( m_pPVPPBGRebirthDialogue )
					m_pPVPPBGRebirthDialogue->timeStart = CTime::GetCurrentTime().GetTime();

				ShowGroupFocus ( REBIRTH_DIALOGUE_PBG, true );
				ShowGroupFocus ( BASIC_CHAT_BOX, true );

				if ( IsVisibleGroup( PVP_PBG_RANKINGS ) )
				{
					HideGroup( PVP_PBG_RANKINGS );
					m_bPVPPBGShowRanking = TRUE;
				}
			}
		}
		/*woe Arc Development 08-06-2024*/
		else if (bPVPWoe)
		{
			if (!IsVisibleGroup(REBIRTH_DIALOGUE_WOE))
			{
				if (m_pRebirthDialogueWoe)
					m_pRebirthDialogueWoe->timeStart = CTime::GetCurrentTime().GetTime();

				ShowGroupFocus(REBIRTH_DIALOGUE_WOE, true);
				ShowGroupFocus(BASIC_CHAT_BOX, true);

				if (IsVisibleGroup(PVP_WOE_RANKINGS))
				{
					HideGroup(PVP_WOE_RANKINGS);
					m_bPVPWoeShowRanking = TRUE;
				}
			}
		}
		else if ( !IsVisibleGroup ( REBIRTH_DIALOGUE ) )
		{
			bool bItemRebirth = (pCharacter->ISREVIVE ())?true:false;
			bool bAutoRebirth = false;
			bool bCantUseRebirth = FALSE;

			//	귀혼주 쿨 타임 체크
			if ( bItemRebirth ) 
			{
				/*ep8 puton slots, Juver, 2018/03/30 */
				SITEM* pITEM = pCharacter->GET_SLOT_ITEMDATA(SLOT_LOGIC_NECKLACE);
				if ( pITEM && pCharacter->IsCoolTime( pITEM->sBasicOp.sNativeID ) )
				{
					bItemRebirth = FALSE;
				}
			}

			if( pCharacter->IsRestartPossible() == FALSE )
			{
				bItemRebirth = FALSE;
				bCantUseRebirth = TRUE;
			}

			if ( bItemRebirth )
			{
				// 소모성 귀혼주이면 자동 귀혼주 사용
				/*ep8 puton slots, Juver, 2018/03/30 */
				SITEM* pITEM = pCharacter->GET_SLOT_ITEMDATA(SLOT_LOGIC_NECKLACE);
				if ( pITEM && !pITEM->ISINSTANCE() ) bAutoRebirth = true;
			}
			
			m_pRebirthDialogue->SET_ITEMREBIRTH ( bItemRebirth, bAutoRebirth, bCantUseRebirth );
			m_pRebirthDialogue->SetCallerID ( MODAL_REBIRTH );
			ShowGroupFocus ( REBIRTH_DIALOGUE, true );
			ShowGroupFocus ( BASIC_CHAT_BOX, true );

			if ( IsVisibleGroup( PVP_CLUB_WAR_RANKING_WINDOW ) )
			{
				HideGroup( PVP_CLUB_WAR_RANKING_WINDOW );
				m_bPVPClubWarShowRanking = TRUE;
			}
		}
	}
	else
	{
		HideGroup ( REBIRTH_DIALOGUE, true );
		HideGroup ( REBIRTH_DIALOGUE_TYRANNY, true );			/*pvp tyranny, Juver, 2017/08/25 */
		HideGroup ( REBIRTH_DIALOGUE_SCHOOLWARS, true );		/*school wars, Juver, 2018/01/20 */
		HideGroup ( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG, true );	/*pvp capture the flag, Juver, 2018/02/01 */
		HideGroup ( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH, true );	/* pvp club death match, Juver, 2020/11/26 */
		HideGroup ( REBIRTH_DIALOGUE_PBG, true );
		HideGroup ( REBIRTH_DIALOGUE_WOE, true );	/*woe Arc Development 08-06-2024*/

		//dont show map move ui when pet is in cooldown
		if ( IsGateOpen ()  && !pCharacter->isPetLastUseTime() )
		{
			/* multi gate out, Juver, 2020/11/17 */
			std::vector<std::string> vecString;
			pCharacter->DetectGateToMapName(vecString);

			std::string strTEMP("");
			for ( size_t i=0; i<vecString.size(); ++i )
			{
				if ( strTEMP.size() )	
					strTEMP += ", ";

				strTEMP += vecString[i];
			}


			if ( vecString.size() == 1 )
			{
				//	만약 맵 이름이 없다면, 이동할 수 없다.
				if ( m_pMapMoveDisplay )
				{
					std::string strMapName = strTEMP + ID2GAMEINTEXT ( "MAPMOVE_MESSAGE" );

					m_pMapMoveDisplay->SetMapName ( strMapName.c_str()  );

					UIRECT rcGlobalPos = m_pMapMoveDisplayDummy->GetGlobalPos ();
					rcGlobalPos.top -= fMENU_LIFTUP;
					m_pMapMoveDisplay->SetGlobalPos ( rcGlobalPos );

					ShowGroupFocus ( MAPMOVE_DISPLAY );
				}		
			}
			else if ( vecString.size() > 1 )
			{
				/* multi gate out, Juver, 2020/11/19 */
				if ( m_pMultiMapMoveDisplay )
				{
					m_pMultiMapMoveDisplay->SetSelectionData();

					UIRECT rcGlobalPos = m_pMultiMapMoveDisplayDummy->GetGlobalPos ();
					rcGlobalPos.top -= fMENU_LIFTUP;
					m_pMultiMapMoveDisplay->SetGlobalPos ( rcGlobalPos );

					ShowGroupFocus ( MULTI_MAPMOVE_DISPLAY );
				}
			}
			else
			{
				HideGroup ( MAPMOVE_DISPLAY );
				HideGroup ( MULTI_MAPMOVE_DISPLAY );
			}
			
		}
		else
		{
			HideGroup ( MAPMOVE_DISPLAY );
			HideGroup ( MULTI_MAPMOVE_DISPLAY );
		}
	}

	if ( pCharacter->m_bCaptchaWait )
	{
		//force show captcha window when waiting for input
		//this will override focus windows
		if ( IsVisibleGroup(CAPTCHA_WINDOW) == FALSE)
		{
			ShowGroupFocus(CAPTCHA_WINDOW);

			if(m_pCaptchaWindow)
				m_pCaptchaWindow->BeginEdit();
		}
	}
	else
	{
		HideGroup(CAPTCHA_WINDOW);
	}

	//	스핵, 오토 프로세스 체크
	if ( IsBlockProgramFound () )
	{
		if ( !IsVisibleGroup ( WAITSERVER_DISPLAY ) )
			DoModal ( ID2GAMEINTEXT("BLOCK_PROGRAM_FOUND"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );
	}

	if ( !DxGlobalStage::GetInstance().IsEmulator() )		//네트워크 연결이 끊어진 경우
	{
		if ( !DxGlobalStage::GetInstance().IsSTATE( DxGlobalStage::EM_CHANGE ) )
		{
			if ( !DxGlobalStage::GetInstance().GetNetClient()->IsOnline() )
			{
				if ( !IsVisibleGroup ( WAITSERVER_DISPLAY ) )
					DoModal ( ID2GAMEINTEXT("PLAYINTERFACE_5"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );				
			}
		}
	}

	if ( m_pAdminMessageDisplay )
	{
		if ( IsVisibleGroup ( ADMIN_MESSAGE_DISPLAY ) )
		{
			if ( m_pAdminMessageDisplay->GetLifeTime () < 0.0f )
			{
				HideGroup ( ADMIN_MESSAGE_DISPLAY );			
			}
		}
	}

	//if ( m_pSimpleMessageMan )
	//{
	//if ( IsVisibleGroup ( SIMPLE_MESSAGE_MAN ) )
	//{
	//	if ( m_pSimpleMessageMan->GetLifeTime () < 0.0f )
	//	{
	//		HideGroup ( SIMPLE_MESSAGE_MAN );			
	//	}
	//}
	//	}

	BOOL bPartyState = FALSE;
	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();	//	마스터
	if ( pMaster )
	{
		bPartyState = TRUE;
	}

	if ( bPartyState != m_bPartyStateBack )
	{
		if ( GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster() )
		{
			ShowGroupFocus ( MINIPARTY_WINDOW );
			HideGroup ( MINIPARTY_OPEN );
		}
		else
		{
			ShowGroupBottom ( MINIPARTY_OPEN );
			HideGroup ( MINIPARTY_WINDOW );
		}
		m_bPartyStateBack = bPartyState;

// #ifdef CH_PARAM // 중국 인터페이스 변경
//		UpdatePotionTrayPosition();
// #endif
	}

	UpdateStateQuestAlarm ();
	UpdateStateSimpleHP ();	

	if ( IsVisibleGroup ( ACADEMY_CONFT_DISPLAY ) || IsVisibleGroup ( SYSTEM_MESSAGE_WINDOW ) )
	{
		HideGroup ( SIMPLE_MESSAGE_MAN );
	}
	else
	{
		if ( RPROFILE::bSHOW_TIP )
		{
			ShowGroupBottom ( SIMPLE_MESSAGE_MAN, true );
		}
		else
		{
			HideGroup ( SIMPLE_MESSAGE_MAN, true );
		}
	}
}

void CInnerInterface::MoveBasicInfoWindow ()
{
	CUIControl * pControl(NULL);

//#ifdef CH_PARAM // 중국 인터페이스 변경
//	pControl = m_pBasicQuickSkillSlot;
//#else
	pControl = m_pUILeftTopGroup;
//#endif

	if ( !pControl ) return;

	const UIRECT& rcBasicInfoViewDummy = m_pBasicInfoViewDummy->GetGlobalPos ();
	const UIRECT& rcLeftTopGroupDummy = m_pLeftTopGroupDummy->GetGlobalPos ();
	const UIRECT& rcBasicInfoView = m_pBasicInfoView->GetGlobalPos ();
	const UIRECT& rcLeftTopGroup = pControl->GetGlobalPos ();

	const D3DXVECTOR2 vLeftTopGroup ( rcLeftTopGroup.left, rcLeftTopGroup.top );
	const D3DXVECTOR2 vLeftTopGroupDummy ( rcLeftTopGroupDummy.left, rcLeftTopGroupDummy.top );

	const D3DXVECTOR2 vBasicInfoViewDummy (rcBasicInfoViewDummy.left,rcBasicInfoViewDummy.top);
	const D3DXVECTOR2 vBasicInfoView (rcBasicInfoView.left,rcBasicInfoView.top);
	const D3DXVECTOR2 vDist = vBasicInfoViewDummy - vBasicInfoView;
	const float fDist = D3DXVec2LengthSq ( &vDist );

	if ( fDist > 900.0f )	//	컨트롤이 떨어졌나?
	{	
		if ( vBasicInfoViewDummy != vLeftTopGroup ) pControl->SetGlobalPos ( vBasicInfoViewDummy );
	}
	else
	{
		if ( vLeftTopGroupDummy != vLeftTopGroup )		pControl->SetGlobalPos ( vLeftTopGroupDummy );
		if ( vBasicInfoViewDummy != vBasicInfoView )	m_pBasicInfoView->SetGlobalPos( vBasicInfoViewDummy );
	}
}

void CInnerInterface::BasicInfoViewDoubleClick()
{
	CUIControl * pControl(NULL);

//#ifdef CH_PARAM // 중국 인터페이스 변경
//	pControl = m_pBasicQuickSkillSlot;
//#else
	pControl = m_pUILeftTopGroup;
//#endif

	const UIRECT& rcBasicInfoViewDummy = m_pBasicInfoViewDummy->GetGlobalPos ();
	const UIRECT& rcLeftTopGroupDummy = m_pLeftTopGroupDummy->GetGlobalPos ();

	const D3DXVECTOR2 vLeftTopGroupDummy ( rcLeftTopGroupDummy.left, rcLeftTopGroupDummy.top );
	const D3DXVECTOR2 vBasicInfoViewDummy (rcBasicInfoViewDummy.left,rcBasicInfoViewDummy.top);

	pControl->SetGlobalPos ( vLeftTopGroupDummy );
	m_pBasicInfoView->SetGlobalPos( vBasicInfoViewDummy );
}

//************************************
// Method:    IsOpenWindowToMoveBlock
// FullName:  CInnerInterface::IsOpenWindowToMoveBlock
// Access:    public 
// Returns:   BOOL
// Qualifier: 
//************************************
BOOL CInnerInterface::IsOpenWindowToMoveBlock()
{
	// 여기에 추가되는 컨트롤은 반듯이 InitDeviceObjects에서 생성시 SetVisibleSingle( FALSE )를 해줘야 한다.
	if( IsVisibleGroup( TRADE_WINDOW ) || 
		IsVisibleGroup( TRADEINVENTORY_WINDOW ) || 
		IsVisibleGroup( MARKET_WINDOW ) || 
		IsVisibleGroup( STORAGE_WINDOW ) || 
		IsVisibleGroup( ITEMBANK_WINDOW ) || 
		IsVisibleGroup( MAP_REQUIRE_CHECK ) ||
		IsVisibleGroup( REBIRTH_DIALOGUE ) ||
		IsVisibleGroup( REBIRTH_DIALOGUE_TYRANNY ) ||			/*pvp tyranny, Juver, 2017/08/25 */
		IsVisibleGroup( REBIRTH_DIALOGUE_SCHOOLWARS ) ||		/*school wars, Juver, 2018/01/20 */
		IsVisibleGroup( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ) ||	/*pvp capture the flag, Juver, 2018/02/01 */
		IsVisibleGroup( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH ) ||	/* pvp club death match, Juver, 2020/11/26 */
		IsVisibleGroup( REBIRTH_DIALOGUE_PBG ) ||
		IsVisibleGroup( REBIRTH_DIALOGUE_WOE ) ||	/*woe Arc Development 08-06-2024*/
		IsVisibleGroup( DIALOGUE_WINDOW ) || 
		IsVisibleGroup( CLUB_STORAGE_WINDOW ) || 
		IsVisibleGroup( PRIVATE_MARKET_MAKE ) || 
		IsVisibleGroup( GAMBLE_DIALOGUE ) || 
		IsVisibleGroup( GAMBLE_SELECT_DIALOGUE ) || 
		IsVisibleGroup( GAMBLE_AGAIN_DIALOGUE ) ||
		IsVisibleGroup( GAMBLE_ANIMATION_DIALOGUE ) || 
		IsVisibleGroup( GAMBLE_RESULT_EVEN_DIALOGUE ) ||
		IsVisibleGroup( GAMBLE_RESULT_ODD_DIALOGUE ) ||
		IsVisibleGroup( ITEM_REBUILD_WINDOW ) || 
		IsVisibleGroup( REBUILDINVENTORY_WINDOW ) ||	// ITEMREBUILD_MARK
		IsVisibleGroup( ITEM_GARBAGE_WINDOW ) || 
		IsVisibleGroup( GARBAGEINVENTORY_WINDOW ) ||	// 휴지통
		IsVisibleGroup( ITEMSHOP_WINDOW ) ||
		IsVisibleGroup( ITEM_SHOP_SEARCH_WINDOW ) ||
		IsVisibleGroup( ITEM_SEARCH_RESULT_WINDOW ) || // 아이템 검색 창
		IsVisibleGroup( PETSKIN_MIX_IMAGE_WINDOW ) ||
		IsVisibleGroup( ITEM_MIX_WINDOW ) ||
		IsVisibleGroup( ITEM_MIX_INVEN_WINDOW ) ||
		IsVisibleGroup( PVP_TYRANNY_RANKINGS ) ||			/*pvp tyranny, Juver, 2017/08/25 */
		IsVisibleGroup( PRODUCT_WINDOW ) ||					/*product item, Juver, 2017/10/17 */
		IsVisibleGroup( BIKE_COLOR_WINDOW )	||				/*bike color , Juver, 2017/11/16 */
		IsVisibleGroup( CHANGE_SCALE_WINDOW ) ||			/*change scale card, Juver, 2018/01/03 */
		IsVisibleGroup( ITEM_COLOR_WINDOW )	||				/*item color, Juver, 2018/01/10 */
		IsVisibleGroup( ITEM_TRANSFER_WINDOW ) ||			/*item transfer card, Juver, 2018/01/18 */
		IsVisibleGroup( ITEM_TRANSFER_INVEN_WINDOW ) ||		/*item transfer card, Juver, 2018/01/18 */
		IsVisibleGroup( PVP_SCHOOLWARS_RANKINGS )  ||		/*school wars, Juver, 2018/01/20 */
		IsVisibleGroup( PVP_CAPTURE_THE_FLAG_RANKINGS )	||	/*pvp capture the flag, Juver, 2018/02/02 */
		IsVisibleGroup( PVP_CLUB_DEATH_MATCH_RANKINGS )	||	/* pvp club death match, Juver, 2020/11/29 */
		IsVisibleGroup( CAR_COLOR_WINDOW ) ||				/* car, cart color, Juver, 2018/02/14 */
		IsVisibleGroup( ITEM_RANDOM_OPTION_REBUILD_WINDOW ) ||			/*item random option rebuild, Juver, 2018/07/03 */
		IsVisibleGroup( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW ) || 	/*item random option rebuild, Juver, 2018/07/03 */
		IsVisibleGroup( CHAOS_MACHINE_WINDOW ) ||			/* chaos machine, Juver, 2021/07/08 */
		IsVisibleGroup( CHAOS_MACHINE_INVEN_WINDOW ) ||		/* chaos machine, Juver, 2021/07/08 */
		IsVisibleGroup( EXCHANGE_ITEM_WINDOW ) ||
		IsVisibleGroup( EXCHANGE_ITEM_INVEN_WINDOW ) ||
		IsVisibleGroup( CAPTCHA_WINDOW ) ||
		IsVisibleGroup( PVP_WOE_RANKINGS ) ||					/*woe Arc Development 08-06-2024*/
		IsVisibleGroup( PVP_PBG_RANKINGS )
		)

	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
#ifdef CH_PARAM // 중국 인터페이스 변경
void CInnerInterface::UpdatePotionTrayPosition()
{
	m_pUILeftTopGroup->SetGlobalPos( m_pBasicPotionTrayDummy->GetGlobalPos() );
	m_pQuestAlarm->SetGlobalPos( m_pQuestAlarmDummy->GetGlobalPos() );

	BOOL bVisibleQuest = IsVisibleGroup( QUEST_ALARM);
	BOOL bVisibleMini = IsVisibleGroup( MINIPARTY_OPEN );

	if( bVisibleQuest && bVisibleMini ) return;

	UIRECT rcDummy = m_pUILeftTopGroup->GetGlobalPos();
	rcDummy.left += 41; // 퀘스트, 미니파티 버튼의 가로 크기가 41
	rcDummy.right = rcDummy.left + rcDummy.sizeX;
	m_pUILeftTopGroup->SetGlobalPos( rcDummy );

	CUIControl * pUIControl(NULL);

	if( !bVisibleQuest && !bVisibleMini )
	{
		pUIControl = m_pUILeftTopGroup;
	}
	else if( !bVisibleMini )
	{
		pUIControl = m_pQuestAlarm;
	}

	if( pUIControl != NULL )
	{
		rcDummy = pUIControl->GetGlobalPos();
		rcDummy.left += 41;
		rcDummy.right = rcDummy.left + rcDummy.sizeX;
		pUIControl->SetGlobalPos( rcDummy );
	}
}
#endif
*/
void CInnerInterface::SetArmSwap( BOOL bSwap )
{
#if defined( BUILD_EP7 ) || defined( BUILD_EP6 ) || defined( BUILD_EP4 )
	m_pInventoryWindow->SetArmSwapTabButton( bSwap );
#else
	/*modern character window, Juver, 2018/03/13 */
	if ( m_pCharacterWindow )
		m_pCharacterWindow->SetArmSwapTabButton( bSwap );
#endif // 
}

bool CInnerInterface::ItemShopAuth ()
{
	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	if( m_fItemShopDelay >= 5.0f )
	{
		m_fItemShopDelay = 0.0f;
	}else{
		PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("ITEM_SHOP_DELAY") );
		return false;
	}

	if( m_pItemShopWindow )
		m_pItemShopWindow->ResetShop();

	//Add Refresh After Buy
	m_bItemShopLoad = true;
	GLGaeaClient::GetInstance().GetCharacter()->ReqItemShopOpen( true );
	return true;
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
}

void CInnerInterface::ItemShopVisible()
{
	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	if ( m_pItemShopWindow )
	{
		ShowGroupFocus ( ITEMSHOP_WINDOW );
		m_pItemShopWindow->InitShop ();
	}
	//Add Refresh After Buy
	m_bItemShopLoad = false;
	return;
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
}

void CInnerInterface::SetItemMixResult( CString strMsg, bool bSuccess, bool bFail )
{
	m_pItemMixWindow->SetItemMixResult( strMsg, bSuccess, bFail );
}

void CInnerInterface::SET_GATHER_GAUGE( DWORD dwGaeaID, float fTime )
{
	m_pGatherGauge->SetTime( fTime );
	m_pGatherGauge->SetMaterial ( dwGaeaID );
}

void CInnerInterface::VisibleCDMRanking( bool bVisible )
{
	if ( bVisible )
	{
		HideGroup( QUEST_HELPER_DISPLAY );
		ShowGroupBottom ( CDM_RANKING_DISPLAY );
	}
	else
	{
		HideGroup( CDM_RANKING_DISPLAY );
		ShowGroupBottom ( QUEST_HELPER_DISPLAY );
	}
}

void CInnerInterface::RefreshCDMRanking()
{
	if ( m_pCdmRankingDisplay && m_pCdmRankingDisplay->IsVisible() )
		m_pCdmRankingDisplay->RefreshCdmRanking();
}

/*woe Arc Development 08-06-2024*/
void CInnerInterface::RefreshWoeRanking()
{
	m_pWoeRanking->RefreshRanking();
}
/*item preview, Juver, 2017/07/27 */
bool CInnerInterface::PreviewItemCheckSimple( SNATIVEID sID )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem( sID );
	if ( !pItem )	return false;

	if ( pItem->sBasicOp.emItemType == ITEM_SUIT )
	{
		return PreviewItemCheckWear( sID );
	}
	else if ( pItem->sBasicOp.emItemType == ITEM_BOX )
	{
		for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
		{
			SNATIVEID sNativeID = pItem->sBox.sITEMS[i].nidITEM;
			SITEM* pItemBox = GLItemMan::GetInstance().GetItem( sNativeID );
			if( pItemBox )
			{
				bool bCan = PreviewItemCheckWear( sNativeID );
				if ( bCan )	return true;
			}
		}
	}

	return false;
}

/*item preview, Juver, 2017/07/27 */
bool CInnerInterface::PreviewItemCheckWear( SNATIVEID sID )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem( sID );
	if ( !pItem )	return false;

	EMCHARCLASS	emClass = GLGaeaClient::GetInstance().GetCharacter()->m_emClass;
	WORD wSchool = GLGaeaClient::GetInstance().GetCharacter()->m_wSchool;
	EMCHARINDEX emIndex = CharClassToIndex( emClass );

	if ( !(pItem->sBasicOp.dwReqCharClass&emClass) )			return false;
	if ( !(pItem->sBasicOp.dwReqSchool&index2school(wSchool)) ) return false;

	if ( pItem->sBasicOp.emItemType == ITEM_SUIT )
	{
		if ( pItem->sSuitOp.emSuit > SUIT_HANDHELD )	return false;

		std::string strR = pItem->GetWearingFileR( emIndex );
		std::string strL = pItem->GetWearingFileL( emIndex );

		if ( strR.size() > 0 || strL.size() > 0 )	return true;
	}

	return false;
}

/*item preview, Juver, 2017/07/27 */
void CInnerInterface::PreviewItem( SITEMCUSTOM sItem )
{
	if ( !m_pItemPreviewWindow )	return;

	/*item wrapper, Juver, 2018/01/12 */
	SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( sItem.sNativeID );
	if ( pitem_data_check && pitem_data_check->sBasicOp.emItemType == ITEM_WRAPPER_BOX && sItem.nidDISGUISE != NATIVEID_NULL() )
	{
		SITEMCUSTOM sitem_new = sItem;
		sitem_new.sNativeID = sitem_new.nidDISGUISE;
		sitem_new.nidDISGUISE = NATIVEID_NULL();

		if ( !PreviewItemCheckSimple( sitem_new.sNativeID ) )	return;
		m_pItemPreviewWindow->StartPreview( sitem_new );
	}
	else
	{
		if ( !PreviewItemCheckSimple( sItem.sNativeID ) )	return;
		m_pItemPreviewWindow->StartPreview( sItem );
	}
	
	ShowGroupFocus( ITEMPREVIEW_WINDOW );
}

/*pet status, Juver, 2017/07/30 */
void CInnerInterface::ShowPetStatus( bool bVisible )
{
	if ( m_pPetStatus )	
	{
		if ( bVisible )	ShowGroupBottom ( PET_STATUS_INFO );
		else	HideGroup( PET_STATUS_INFO );
	}

	/*pet skill info display, Juver, 2017/12/01 */
	if ( m_pPetSkillInfoDisplay )
	{
		if ( bVisible )	ShowGroupBottom ( PET_SKILL_INFO_DISPLAY );
		else	HideGroup( PET_SKILL_INFO_DISPLAY );
	}

	/*dual pet skill, Juver, 2017/12/29 */
	if ( m_pPetSkillInfoDisplay2 )
	{
		if ( bVisible )	ShowGroupBottom ( PET_SKILL_INFO_DISPLAY2 );
		else	HideGroup( PET_SKILL_INFO_DISPLAY2 );
	}
}

/*vehicle status, Juver, 2017/07/30 */
void CInnerInterface::ShowVehicleStatus( bool bVisible )
{
	if ( !m_pVehicleStatus )	return;

	if ( bVisible )	ShowGroupBottom ( VEHICLE_STATUS_INFO );
	else	HideGroup( VEHICLE_STATUS_INFO );
}

/*item link, Juver, 2017/07/31 */
void CInnerInterface::LinkItem( SITEMCUSTOM* pItem )
{
	if ( GetChat() && pItem )
	{
		GetChat()->AddItemLink( pItem );
	}
}

/*vehicle booster system, Juver, 2017/08/11 */
void CInnerInterface::VehicleBoosterVisible( bool bVisible )
{
	if ( !m_pVehicleBoosterDisplay )	return;

	if (bVisible)	ShowGroupBottom ( VEHICLE_BOOSTER_DISPLAY );
	else	HideGroup( VEHICLE_BOOSTER_DISPLAY );
}

/*box contents, Juver, 2017/08/30 */
void CInnerInterface::ShowBoxContents( SNATIVEID sidItem )
{
	if ( IsVisibleGroup( INFO_BOX_CONTENTS ))
		HideGroup( INFO_BOX_CONTENTS );

	if ( !m_pInfoBoxContents )			return;
	if ( sidItem == NATIVEID_NULL() )	return;

	SITEM* pitem = GLItemMan::GetInstance().GetItem( sidItem );
	if ( !pitem )	return;

	/*specific item box, Juver, 2018/09/02 */
	if ( !( pitem->sBasicOp.emItemType == ITEM_BOX || pitem->sBasicOp.emItemType == ITEM_PREMIUMSET || pitem->sBasicOp.emItemType == ITEM_BOX_SPECIFIC ) )	return;

	ShowGroupFocus( INFO_BOX_CONTENTS );

	m_pInfoBoxContents->DataShow( pitem );

}

/* Competition UI, Juver, 2017/08/24 */
void CInnerInterface::PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE emType )
{
	switch ( emType )
	{
	case EMCOMPETITION_NOTIFY_TYPE_NONE:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( FALSE );

			if ( m_pPVPCompetitionNotify )	
				m_pPVPCompetitionNotify->SetVisibleSingle( FALSE );
		}break;

	case EMCOMPETITION_NOTIFY_TYPE_TYRANNY_REGISTER:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( TRUE );

			if ( m_pPVPCompetitionNotify )	
			{
				m_pPVPCompetitionNotify->SetVisibleSingle( TRUE );
				m_pPVPCompetitionNotify->SetNotify( ID2GAMEWORD( "PVP_COMPETITION_NOTIFY_TEXT", 0 ), NS_UITEXTCOLOR::BLACK );
			}
		}break;

	case EMCOMPETITION_NOTIFY_TYPE_SCHOOL_WAR_REGISTER:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( TRUE );

			if ( m_pPVPCompetitionNotify )	
			{
				m_pPVPCompetitionNotify->SetVisibleSingle( TRUE );
				m_pPVPCompetitionNotify->SetNotify( ID2GAMEWORD( "PVP_COMPETITION_NOTIFY_TEXT", 1 ), NS_UITEXTCOLOR::BLACK );
			}
		}break;

		case EMCOMPETITION_NOTIFY_TYPE_WOE_REGISTER:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( TRUE );

			if ( m_pPVPCompetitionNotify )	
			{
				m_pPVPCompetitionNotify->SetVisibleSingle( TRUE );
				m_pPVPCompetitionNotify->SetNotify( ID2GAMEWORD( "PVP_COMPETITION_NOTIFY_TEXT", 5 ), NS_UITEXTCOLOR::BLACK );
			}
		}break;

	case EMCOMPETITION_NOTIFY_TYPE_CAPTURE_THE_FLAG_REGISTER:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( TRUE );

			if ( m_pPVPCompetitionNotify )	
			{
				m_pPVPCompetitionNotify->SetVisibleSingle( TRUE );
				m_pPVPCompetitionNotify->SetNotify( ID2GAMEWORD( "PVP_COMPETITION_NOTIFY_TEXT", 2 ), NS_UITEXTCOLOR::BLACK );
			}
		}break;

	case EMCOMPETITION_NOTIFY_TYPE_CLUB_DEATH_MATCH_REGISTER:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( TRUE );

			if ( m_pPVPCompetitionNotify )	
			{
				m_pPVPCompetitionNotify->SetVisibleSingle( TRUE );
				m_pPVPCompetitionNotify->SetNotify( ID2GAMEWORD( "PVP_COMPETITION_NOTIFY_TEXT", 3 ), NS_UITEXTCOLOR::BLACK );
			}
		}break;

	case EMCOMPETITION_NOTIFY_TYPE_PVP_PBG_REGISTER:
		{
			if ( m_pPVPCompetitionButton )	
				m_pPVPCompetitionButton->SetButtonAlarm( TRUE );

			if ( m_pPVPCompetitionNotify )	
			{
				m_pPVPCompetitionNotify->SetVisibleSingle( TRUE );
				m_pPVPCompetitionNotify->SetNotify( ID2GAMEWORD( "PVP_COMPETITION_NOTIFY_TEXT", 4 ), NS_UITEXTCOLOR::BLACK );
			}
		}break;
	}
}

/* Competition UI, Juver, 2017/08/24 */
void CInnerInterface::PVPCompetitionWindowShow()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))
	{
		SetDefaultPosInterface( PVP_COMPETITION_WINDOW );
		ShowGroupFocus( PVP_COMPETITION_WINDOW );
	}

	PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE_NONE );
}

int CInnerInterface::PVPCompetitionShowNoticeMultiColor(DWORD dwTextColor, const char* szFormat, ...)
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop(PKSTREAK_KILL_NOTICE);

	if (m_pPKStreakKillNotice)
	{
		int line = m_pPKStreakKillNotice->AddText(szBuffer, dwTextColor);
		return line;
	}
	return 0;
}

int CInnerInterface::PVPCompetitionShowNoticeAddString(int index, DWORD dwTextColor, const char* szFormat, ...)
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop(PKSTREAK_KILL_NOTICE);

	if (m_pPKStreakKillNotice)
	{
		int line = m_pPKStreakKillNotice->AddString(index, szBuffer, dwTextColor);
		return line;
	}
	return 0;
}

int CInnerInterface::PVPCompetitionShowNoticeMultiColorPKDead(DWORD dwTextColor, const char* szFormat, ...)
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop(PKSTREAK_SHUTDOWN_NOTICE);

	if (m_pPKStreakShutdownNotice)
	{
		int line = m_pPKStreakShutdownNotice->AddText(szBuffer, dwTextColor);
		return line;
	}
	return 0;
}

int CInnerInterface::PVPCompetitionShowNoticeAddStringPKDead(int index, DWORD dwTextColor, const char* szFormat, ...)
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop(PKSTREAK_SHUTDOWN_NOTICE);

	if (m_pPKStreakShutdownNotice)
	{
		int line = m_pPKStreakShutdownNotice->AddString(index, szBuffer, dwTextColor);
		return line;
	}
	return 0;
}

/* Competition UI, Juver, 2017/08/24 */
void CInnerInterface::PVPCompetitionShowNotice( DWORD dwTextColor, const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop ( PVP_COMPETITION_NOTICE );

	if ( m_pPVPCompetitionNotice )
	{
		m_pPVPCompetitionNotice->AddText( szBuffer, dwTextColor );
	}
}
/* PK Streak Sigaw - Jxyyy */
void CInnerInterface::ShowPKStreakKillNotice( DWORD dwTextColor, const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop ( PKSTREAK_KILL_NOTICE );

	if ( m_pPKStreakKillNotice && GetChat() )
	{
		m_pPKStreakKillNotice->AddText( szBuffer, dwTextColor );
	//	GetChat()->AddStringToSystemMessage ( szBuffer, dwTextColor, NULL );
	}
}
void CInnerInterface::ShowPKStreakShutdownNotice( DWORD dwTextColor, const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 1024;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	ShowGroupTop ( PKSTREAK_SHUTDOWN_NOTICE );

	if ( m_pPKStreakShutdownNotice && GetChat() )
	{
		m_pPKStreakShutdownNotice->AddText( szBuffer, dwTextColor );
		GetChat()->AddStringToSystemMessage ( szBuffer, dwTextColor, NULL);
	}
}
/* PK Streak Sigaw - Jxyyy End */
/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyTowerCaptureResetPos()
{
	if ( !m_pPVPTyrannyTowerCapture )		return;
	if ( !m_pPVPNextBattleAndWinner )		return; // PVP Next Battle UI - Jxyyy
	if ( !m_pPVPTyrannyTowerCaptureDummy )	return;

	m_pPVPTyrannyTowerCapture->SetGlobalPos( 
		D3DXVECTOR2( m_pPVPTyrannyTowerCaptureDummy->GetGlobalPos ().left, m_pPVPTyrannyTowerCaptureDummy->GetGlobalPos ().top ) );
	// PVP Next Battle UI - Jxyyy
	m_pPVPNextBattleAndWinner->SetGlobalPos( 
		D3DXVECTOR2( m_pPVPTyrannyTowerCaptureDummy->GetGlobalPos ().left, m_pPVPTyrannyTowerCaptureDummy->GetGlobalPos ().top ) );
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShowCaptureNotice( WORD wSchool, DWORD dwClubID, DWORD dwClubMarkVer )
{
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		if ( m_pPVPTyrannyNoticeCapture[i] )
			m_pPVPTyrannyNoticeCapture[i]->SetVisibleSingle( FALSE );
	}

	if ( wSchool < TYRANNY_SCHOOL_SIZE )
	{
		if ( m_pPVPTyrannyNoticeCapture[wSchool] )
		{
			m_pPVPTyrannyNoticeCapture[wSchool]->SetData( dwClubID, dwClubMarkVer );
			m_pPVPTyrannyNoticeCapture[wSchool]->SetVisibleSingle( TRUE );
		}
	}
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShow2CaptureNotice( WORD wSchool )
{
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		if ( m_pPVPTyranny2Capture[i] )
			m_pPVPTyranny2Capture[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pPVPTyranny2CaptureOwn )
		m_pPVPTyranny2CaptureOwn->SetVisibleSingle( FALSE );

	if ( wSchool < TYRANNY_SCHOOL_SIZE )
	{
		if ( m_pPVPTyranny2Capture[wSchool] )
			m_pPVPTyranny2Capture[wSchool]->SetVisibleSingle( TRUE );
	}
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShow2CaptureOwnNotice()
{
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		if ( m_pPVPTyranny2Capture[i] )
			m_pPVPTyranny2Capture[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pPVPTyranny2CaptureOwn )
		m_pPVPTyranny2CaptureOwn->SetVisibleSingle( FALSE );

	if ( m_pPVPTyranny2CaptureOwn )
		m_pPVPTyranny2CaptureOwn->SetVisibleSingle( TRUE );
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShowRanking()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_TYRANNY ))
	{
		//character is dead show rank window later when revived!
		m_bPVPTyrannyShowRanking = true;
		return;
	}

	if ( !IsVisibleGroup( PVP_TYRANNY_RANKINGS )){
		SetDefaultPosInterface( PVP_TYRANNY_RANKINGS );
		ShowGroupFocus( PVP_TYRANNY_RANKINGS );
	}

	if ( m_pPVPTyrannyRankings ){
		m_pPVPTyrannyRankings->UpdateRankings();
	}

	m_bPVPTyrannyShowRanking = false;
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShowTopRanking()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	/*pvp reward info, Juver, 2018/07/25 */
	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_TYRANNY_TOPRANKINGS );

	if ( m_pPVPTyrannyTopRankings )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;

		m_pPVPTyrannyTopRankings->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShowStartNotice()
{
	ShowGroupFocus( PVP_TYRANNY_BATTLE_START_NOTICE );
}

/*pvp tyranny, Juver, 2017/08/25 */
void CInnerInterface::PVPTyrannyShowEndNotice()
{
	ShowGroupFocus( PVP_TYRANNY_BATTLE_END_NOTICE );
}
/*woe Arc Development 08-06-2024*/
void CInnerInterface::PVPWoeTowerCaptureResetPos()
{
	if (!m_pPVPWoeTowerCapture)		return;
	if (!m_pPVPWoeTowerCaptureDummy)	return;

	m_pPVPWoeTowerCapture->SetGlobalPos(
		D3DXVECTOR2(m_pPVPWoeTowerCaptureDummy->GetGlobalPos().left, m_pPVPWoeTowerCaptureDummy->GetGlobalPos().top));
}

/*item exchange, Juver, 2017/10/12 */
void CInnerInterface::SetNPCItemExchangeWindowOpen( SNATIVEID sid_npc )
{
	if ( m_pNPCItemExchangeWindow )
	{
		m_pNPCItemExchangeWindow->LoadList ( sid_npc );
		ShowGroupFocus ( NPC_ITEM_EXCHANGE_WINDOW );
	}
}

/*product item, Juver, 2017/10/15 */
void CInnerInterface::OpenProductWindow()
{
	if ( isReviveWaiting() )				return;

	if ( IsVisibleGroup( PRODUCT_WINDOW ) )	return;

	CloseAllWindow();

	ShowGroupFocus( PRODUCT_WINDOW );

	if ( m_pProductWindow )
		m_pProductWindow->LoadData();
}

/*student record ui, Juver, 2017/10/30 */
void CInnerInterface::OpenStudentRecord()
{
	if ( !RPARAM::bStudentRecord )	return;

	if ( IsVisibleGroup( STUDENT_RECORD_WINDOW ) )	return;

	ShowGroupFocus( STUDENT_RECORD_WINDOW );
}

/*activity system, Juver, 2017/11/05 */
void CInnerInterface::OpenActivityItemWindow()
{
	if ( !RPARAM::bActivity )	return;

	if ( IsVisibleGroup( ACTIVITY_ITEM_WINDOW ) )	return;

	ShowGroupFocus( ACTIVITY_ITEM_WINDOW );
}
/* Codex, Archie 02/16/24 */
void CInnerInterface::OpenStudentCodex()
{

	if ( !IsVisibleGroup( STUDENT_CODEX_WINDOW ))	
	{
		ShowGroupFocus( STUDENT_CODEX_WINDOW );
		//m_pStudentRecordWindow->LoadData();
	}
	else
	{
		HideGroup ( STUDENT_CODEX_WINDOW );
	}
}

/*school wars, Juver, 2018/01/20 */
void CInnerInterface::PVPSchoolWarsScoreResetPos()
{
	if ( !m_pPVPSchoolWarsScore )		return;
	if ( !m_pPVPSchoolWarsScoreDummy )	return;

	m_pPVPSchoolWarsScore->SetGlobalPos( 
		D3DXVECTOR2( m_pPVPSchoolWarsScoreDummy->GetGlobalPos ().left, m_pPVPSchoolWarsScoreDummy->GetGlobalPos ().top ) );
}

/*school wars, Juver, 2018/01/20 */
void CInnerInterface::PVPSchoolWarsShowTopRanking()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	/*pvp reward info, Juver, 2018/07/25 */
	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_SCHOOLWARS_TOPRANKINGS );

	if ( m_pPVPSchoolWarsTopRankings )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPSchoolWarsTopRankings->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/*school wars, Juver, 2018/01/20 */
void CInnerInterface::PVPSchoolWarsShowStartNotice()
{
	ShowGroupFocus( PVP_SCHOOLWARS_BATTLE_START_NOTICE );
}

/*school wars, Juver, 2018/01/20 */
void CInnerInterface::PVPSchoolWarsShowEndNotice()
{
	ShowGroupFocus( PVP_SCHOOLWARS_BATTLE_END_NOTICE );
}

/*school wars, Juver, 2018/01/20 */
void CInnerInterface::PVPSchoolWarsShowRanking()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_SCHOOLWARS ))
	{
		//character is dead show rank window later when revived!
		m_bPVPSchoolWarsShowRanking = true;
		return;
	}

	if ( !IsVisibleGroup( PVP_SCHOOLWARS_RANKINGS )){
		SetDefaultPosInterface( PVP_SCHOOLWARS_RANKINGS );
		ShowGroupFocus( PVP_SCHOOLWARS_RANKINGS );
	}

	if ( m_pPVPSchoolWarsRankings ){
		m_pPVPSchoolWarsRankings->UpdateRankings();
	}

	m_bPVPSchoolWarsShowRanking = false;
}

/*pvp capture the flag, Juver, 2018/02/01 */
void CInnerInterface::PVPCaptureTheFlagShowTopRanking()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	/*pvp reward info, Juver, 2018/07/25 */
	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_CAPTURE_THE_FLAG_TOPRANKINGS );

	if ( m_pPVPCaptureTheFlagTopRankings )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;

		m_pPVPCaptureTheFlagTopRankings->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/*pvp capture the flag, Juver, 2018/02/02 */
void CInnerInterface::PVPCaptureTheFlagShowStartNotice()
{
	ShowGroupFocus( PVP_CAPTURE_THE_FLAG_BATTLE_START_NOTICE );
}

/*pvp capture the flag, Juver, 2018/02/02 */
void CInnerInterface::PVPCaptureTheFlagShowEndNotice()
{
	ShowGroupFocus( PVP_CAPTURE_THE_FLAG_BATTLE_END_NOTICE );
}

/*pvp capture the flag, Juver, 2018/02/02 */
void CInnerInterface::PVPCaptureTheFlagScoreResetPos()
{
	if ( !m_pPVPCaptureTheFlagScore )		return;
	if ( !m_pPVPCaptureTheFlagScoreDummy )	return;

	m_pPVPCaptureTheFlagScore->SetGlobalPos( D3DXVECTOR2( m_pPVPCaptureTheFlagScoreDummy->GetGlobalPos ().left, m_pPVPCaptureTheFlagScoreDummy->GetGlobalPos ().top ) );
}

/*pvp capture the flag, Juver, 2018/02/02 */
void CInnerInterface::PVPCaptureTheFlagShowRanking()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ))
	{
		//character is dead show rank window later when revived!
		m_bPVPCaptureTheFlagShowRanking = true;
		return;
	}

	if ( !IsVisibleGroup( PVP_CAPTURE_THE_FLAG_RANKINGS )){
		SetDefaultPosInterface( PVP_CAPTURE_THE_FLAG_RANKINGS );
		ShowGroupFocus( PVP_CAPTURE_THE_FLAG_RANKINGS );
	}

	if ( m_pPVPCaptureTheFlagRankings ){
		m_pPVPCaptureTheFlagRankings->UpdateRankings();
	}

	m_bPVPCaptureTheFlagShowRanking = false;
}

/*info window, Juver, 2018/03/12 */
void CInnerInterface::InfoWindowOpen( UIGUID uid_caller /*= NO_ID*/ )
{
	SetDefaultPosInterface( UI_INFO_WINDOW );
	ShowGroupFocus( UI_INFO_WINDOW );

	if ( m_pInfoWindow )
	{
		m_pInfoWindow->SetCallerID( uid_caller );
		m_pInfoWindow->GetTextBox()->ClearText();
	}
}

/*info window, Juver, 2018/03/12 */
void CInnerInterface::InfoWindowSetText( CString strText, DWORD dwColor )
{
	if ( !m_pInfoWindow )	return;
	m_pInfoWindow->GetTextBox()->SetText( strText, dwColor );
}

/*info window, Juver, 2018/03/12 */
int CInnerInterface::InfoWindowAddText( CString strText, DWORD dwColor )
{
	if ( !m_pInfoWindow )	return -1;
	return m_pInfoWindow->GetTextBox()->AddText( strText, dwColor );
}

/*info window, Juver, 2018/03/12 */
int CInnerInterface::InfoWindowAddString( int nIndex, CString strText, DWORD dwColor )
{
	if ( !m_pInfoWindow )	return -1;
	return m_pInfoWindow->GetTextBox()->AddString( nIndex, strText, dwColor );
}

/*modern inventory window, Juver, 2018/04/07 */
BOOL CInnerInterface::IsInventoryBlocked()
{
	if ( IsVisibleGroup ( GARBAGEINVENTORY_WINDOW ) )		return TRUE;
	if ( IsVisibleGroup ( ITEM_MIX_INVEN_WINDOW ) )			return TRUE;
	if ( IsVisibleGroup ( REBUILDINVENTORY_WINDOW ) )		return TRUE;
	if ( IsVisibleGroup ( TRADEINVENTORY_WINDOW ) )			return TRUE;
	if ( IsVisibleGroup ( ITEM_TRANSFER_INVEN_WINDOW ) )	return TRUE;
	if ( IsVisibleGroup ( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW ) )	return TRUE;	/*item random option rebuild, Juver, 2018/07/03 */
	if ( IsVisibleGroup ( CHAOS_MACHINE_INVEN_WINDOW ) )	return TRUE;					/* chaos machine, Juver, 2021/07/08 */

	if ( IsVisibleGroup ( EXCHANGE_ITEM_INVEN_WINDOW ) )		
		return TRUE;

	return FALSE;
}

/*pvp reward info, Juver, 2018/07/25 */
void CInnerInterface::PVPCloseRewardInfo()
{
	HideGroup( PVP_TYRANNY_TOPRANKINGS );
	HideGroup( PVP_SCHOOLWARS_TOPRANKINGS );
	HideGroup( PVP_CAPTURE_THE_FLAG_TOPRANKINGS );

	/* pvp club death match, Juver, 2020/11/28 */
	HideGroup( PVP_CLUB_DEATH_MATCH_TOP_RANKINGS );

	HideGroup( PVP_TYRANNY_REWARD_INFO );
	HideGroup( PVP_SCHOOLWARS_REWARD_INFO );
	HideGroup( PVP_CAPTURE_THE_FLAG_REWARD_INFO );

	/* pvp club death match, Juver, 2020/12/03 */
	HideGroup( PVP_CLUB_DEATH_MATCH_REWARD_INFO );

	HideGroup( PVP_PBG_TOPRANKINGS );
}

/*pvp reward info, Juver, 2018/07/25 */
void CInnerInterface::PVPTyrannyShowRewardInfo()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_TYRANNY_REWARD_INFO );

	if ( m_pPVPTyrannyRewardInfo )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPTyrannyRewardInfo->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/*pvp reward info, Juver, 2018/07/25 */
void CInnerInterface::PVPSchoolWarsShowRewardInfo()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_SCHOOLWARS_REWARD_INFO );

	if ( m_pPVPSchoolWarsRewardInfo )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPSchoolWarsRewardInfo->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/*pvp reward info, Juver, 2018/07/25 */
void CInnerInterface::PVPCaptureTheFlagShowRewardInfo()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_CAPTURE_THE_FLAG_REWARD_INFO );

	if ( m_pPVPCaptureTheFlagRewardInfo )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPCaptureTheFlagRewardInfo->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/*modern skill tray, Juver, 2018/08/21 */
void CInnerInterface::skill_tray_open( bool use_mini )
{
	m_skill_tray_mini = use_mini;
	skill_tray_switch();
}

/*modern skill tray, Juver, 2018/08/21 */
void CInnerInterface::skill_tray_lock()
{
	m_skill_tray_lock = !m_skill_tray_lock;
}

/*modern skill tray, Juver, 2018/08/21 */
void CInnerInterface::skill_tray_rotate()
{
	m_skill_tray_rotate = !m_skill_tray_rotate;
	skill_tray_switch();
}

/*modern skill tray, Juver, 2018/08/21 */
void CInnerInterface::skill_tray_switch()
{
	HideGroup ( QUICK_SKILL_TRAY_TAB_MINI );
	HideGroup ( QUICK_SKILL_TRAY_TAB_FULL );
	HideGroup ( QUICK_SKILL_TRAY_TAB_EX_MINI );
	HideGroup ( QUICK_SKILL_TRAY_TAB_EX_FULL );

	if ( m_skill_tray_mini && m_skill_tray_rotate )
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_EX_MINI );
	else if ( m_skill_tray_mini && !m_skill_tray_rotate )
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_MINI );
	else if ( !m_skill_tray_mini && m_skill_tray_rotate )
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_EX_FULL );
	else if ( !m_skill_tray_mini && !m_skill_tray_rotate )
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_FULL );
	else
	{
		//code gone wrong use defaults instead
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_FULL );
	}
	
}

/*modern skill tray, Juver, 2018/08/21 */
INT CInnerInterface::skill_tray_get_tab_index()
{
	if ( !m_pSkillTrayTab )		return 0;
	if ( !m_pSkillTrayTabEx )	return 0;

	if ( m_skill_tray_rotate )	
		return m_pSkillTrayTabEx->GetTabIndex();

	return m_pSkillTrayTab->GetTabIndex();
}

/*specific item box, Juver, 2018/09/02 */
void CInnerInterface::item_box_specific_selection_window_show( SNATIVEID item_id )
{
	if ( !m_pitem_box_specific_selection_window	)	return;

	ShowGroupFocus( ITEM_BOX_SPECIFIC_SELECTION_WINDOW );

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( item_id );
	if ( !pitem_data )	return;

	if ( pitem_data->sBasicOp.emItemType != ITEM_BOX_SPECIFIC )	return;

	if ( !pitem_data->sBox.VALID() )	return;

	if ( m_pitem_box_specific_selection_window )
		m_pitem_box_specific_selection_window->selection_show( pitem_data );
}

/*pk combo, EJCode, 2018/11/15 */
bool CInnerInterface::PKComboSet ( int id )
{
	UIGUID cID = NO_ID;

	switch ( id )
	{
	case EMPK_COMBO_TYPE_00:			cID = CPKComboDisplay::PK_COMBO_IMAGE_00;		break;
	case EMPK_COMBO_TYPE_01:			cID = CPKComboDisplay::PK_COMBO_IMAGE_01;		break;
	case EMPK_COMBO_TYPE_02:			cID = CPKComboDisplay::PK_COMBO_IMAGE_02;		break;
	case EMPK_COMBO_TYPE_03:			cID = CPKComboDisplay::PK_COMBO_IMAGE_03;		break;
	case EMPK_COMBO_TYPE_04:			cID = CPKComboDisplay::PK_COMBO_IMAGE_04;		break;
	case EMPK_COMBO_TYPE_05:			cID = CPKComboDisplay::PK_COMBO_IMAGE_05;		break;
	case EMPK_COMBO_TYPE_06:			cID = CPKComboDisplay::PK_COMBO_IMAGE_06;		break;
	case EMPK_COMBO_TYPE_07:			cID = CPKComboDisplay::PK_COMBO_IMAGE_07;		break;
	case EMPK_COMBO_TYPE_08:			cID = CPKComboDisplay::PK_COMBO_IMAGE_08;		break;
	case EMPK_COMBO_TYPE_09:			cID = CPKComboDisplay::PK_COMBO_IMAGE_09;		break;

	case EMPK_COMBO_TYPE_10:			cID = CPKComboDisplay::PK_COMBO_IMAGE_10;		break;
	case EMPK_COMBO_TYPE_11:			cID = CPKComboDisplay::PK_COMBO_IMAGE_11;		break;
	case EMPK_COMBO_TYPE_12:			cID = CPKComboDisplay::PK_COMBO_IMAGE_12;		break;
	case EMPK_COMBO_TYPE_13:			cID = CPKComboDisplay::PK_COMBO_IMAGE_13;		break;
	case EMPK_COMBO_TYPE_14:			cID = CPKComboDisplay::PK_COMBO_IMAGE_14;		break;
	case EMPK_COMBO_TYPE_15:			cID = CPKComboDisplay::PK_COMBO_IMAGE_15;		break;
	case EMPK_COMBO_TYPE_16:			cID = CPKComboDisplay::PK_COMBO_IMAGE_16;		break;
	case EMPK_COMBO_TYPE_17:			cID = CPKComboDisplay::PK_COMBO_IMAGE_17;		break;
	case EMPK_COMBO_TYPE_18:			cID = CPKComboDisplay::PK_COMBO_IMAGE_18;		break;
	case EMPK_COMBO_TYPE_19:			cID = CPKComboDisplay::PK_COMBO_IMAGE_19;		break;
	}

	if ( cID == NO_ID ) return false;

	ShowGroupTop ( PK_COMBO_DISPLAY );

	if( m_ppk_combo_display )
		m_ppk_combo_display->START ( cID );

	//PrintMsgText ( NS_UITEXTCOLOR::GOLD, ID2GAMEWORD("PK_COMBO_STATE", id ) );

	return true;
}

void CInnerInterface::SetCompetitionJoin( bool bEnable )
{
	if ( m_pPVPCompetitionWindow )
		m_pPVPCompetitionWindow->SetCompetitionJoin( bEnable );
}

void CInnerInterface::PVPClubWarRankingWindowClose()
{
	HideGroup( PVP_CLUB_WAR_RANKING_WINDOW );
}

void CInnerInterface::PVPClubWarRankingWindowRefresh()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE ))
	{
		//character is dead show rank window later when revived!
		m_bPVPClubWarShowRanking = true;
		return;
	}

	if ( !IsVisibleGroup( PVP_CLUB_WAR_RANKING_WINDOW ) )
	{
		SetDefaultPosInterface( PVP_CLUB_WAR_RANKING_WINDOW );
		ShowGroupFocus( PVP_CLUB_WAR_RANKING_WINDOW );
	}

	if ( m_pPVPClubWarRankingWindow )
		m_pPVPClubWarRankingWindow->SetData();

	m_bPVPClubWarShowRanking = false;
}

void CInnerInterface::PVPClubWarRankingWindowOpen()
{
	if ( GLGaeaClient::GetInstance().PVPClubWarsRankValid() )
		PVPClubWarRankingWindowRefresh();
	else
		PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEWORD("PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_INFO", 2 ) );
}

void CInnerInterface::CurrencyShopOpen( SNATIVEID sNpcID )
{
	if ( m_pCurrencyShopWindow )
	{
		m_pCurrencyShopWindow->LoadList ( sNpcID );
		ShowGroupFocus ( CURRENCY_SHOP_WINDOW );
	}
}

void CInnerInterface::CurrencyShopUpdatePrice()
{
	if ( m_pCurrencyShopWindow )
		m_pCurrencyShopWindow->UpdatePrice();
}

/* personal lock system, Juver, 2020/01/25 */
void CInnerInterface::PersonalLockCloseWindow()
{
	HideGroup( PERSONAL_LOCK_PIN_CREATE );
	HideGroup( PERSONAL_LOCK_PIN_INPUT );

	HideGroup( PERSONAL_LOCK_PIN_RESET );
	HideGroup( PERSONAL_LOCK_PIN_CHANGE );
	HideGroup( PERSONAL_LOCK_PIN_RECOVER );
}

/* personal lock system, Juver, 2020/01/25 */
void CInnerInterface::PersonalLockPinCreate( EMPERSONAL_LOCK emLock )
{
	PersonalLockCloseWindow();
	ShowGroupFocus ( PERSONAL_LOCK_PIN_CREATE );

	if ( m_pPersonalLockPinCreate )
		m_pPersonalLockPinCreate->SetLockType( emLock );
}

/* personal lock system, Juver, 2020/01/25 */
void CInnerInterface::PersonalLockPinInput( EMPERSONAL_LOCK emLock )
{
	PersonalLockCloseWindow();
	ShowGroupFocus ( PERSONAL_LOCK_PIN_INPUT );

	if ( m_pPersonalLockPinInput )
		m_pPersonalLockPinInput->SetLockType( emLock );
}

/* personal lock system, Juver, 2020/01/25 */
void CInnerInterface::PersonalLockPinReset()
{
	PersonalLockCloseWindow();
	ShowGroupFocus ( PERSONAL_LOCK_PIN_RESET );
}

/* personal lock system, Juver, 2020/01/25 */
void CInnerInterface::PersonalLockPinChange()
{
	PersonalLockCloseWindow();
	ShowGroupFocus ( PERSONAL_LOCK_PIN_CHANGE );
}

/* personal lock system, Juver, 2020/01/31 */
void CInnerInterface::PersonalLockPinRecover()
{
	PersonalLockCloseWindow();
	ShowGroupFocus ( PERSONAL_LOCK_PIN_RECOVER );
}

/* personal lock system, Juver, 2020/02/01 */
void CInnerInterface::PersonalLockPinRecoverResultOpen()
{
	ShowGroupFocus ( PERSONAL_LOCK_PIN_RECOVER_RESULT );

	if ( m_pPersonalLockPinRecoverResult )
		m_pPersonalLockPinRecoverResult->InfoClear();
}

/* personal lock system, Juver, 2020/02/01 */
void CInnerInterface::PersonalLockPinRecoverResultUpdate( SPERSONAL_LOCK_PIN* pPin, EMPERSONAL_LOCK emLock )
{
	if ( m_pPersonalLockPinRecoverResult )
		m_pPersonalLockPinRecoverResult->InfoAdd( pPin, emLock );
}

/* party finder, Juver, 2019/12/28 */
void CInnerInterface::OpenPartyFinderWindow()
{
	if ( !RPARAM::bUsePartyFinder )	return;

	//dummy code to test the window functions and pages
	/*for ( int i=0; i<100; ++i )
	{
		SPARTY_FINDER_RESULT sResult;
		sResult.dwPartyID = i*2;
		sResult.dwPartyMembers =  i%8;

		sResult.dwLeaderCharID = i*3;
		sResult.wLeaderSchool = i%3;
		sResult.wLeaderClass = i%14;

		sResult.wPartyItemOption = i%EMPTYITEMOPT_SIZE;
		sResult.wPartyMoneyOption = i%EMPTYMONEYOPT_SIZE;

		StringCchCopy ( sResult.szLeaderName, CHAR_SZNAME, "test1" );
		StringCchCopy ( sResult.szMapName, MAP_NAME_MAX, "test2" );

		GLGaeaClient::GetInstance().m_vecPartyFinderResult.push_back( sResult );
	}*/
	
	ShowGroupFocus( PARTY_FINDER_WINDOW );
}

/* party finder, Juver, 2019/12/28 */
void CInnerInterface::RefreshPartyFinderWindow()
{
	if ( m_pPartyFinderWindow )
		m_pPartyFinderWindow->UpdateSearchResult();
}

/* party finder, Juver, 2020/01/17 */
void CInnerInterface::OpenPartyFinderQuestion( SPARTY_FINDER_QUESTION* pQuestion )
{
	if ( IsVisibleGroup( PARTY_FINDER_QUESTION ) )
	{
		//auto decline when window is still active
		if ( pQuestion )
		{
			GLGaeaClient::GetInstance().GetCharacter()->PartyFinderQuestionDecline( pQuestion->dwPartyID, pQuestion->dwCharID );
		}
		
		return;
	}

	if ( m_pPartyFinderQuestion )
	{
		m_pPartyFinderQuestion->SetCallerID( MODAL_PARTY_FINDER_JOIN_QUESTION );
		m_pPartyFinderQuestion->SetQuestionData( pQuestion );
	}

	ShowGroupFocus( PARTY_FINDER_QUESTION );
}

bool CInnerInterface::isReviveWaiting()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE ) )			
		return true;

	/*pvp tyranny, Juver, 2017/08/25 */
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_TYRANNY ) )	
		return true;

	/*school wars, Juver, 2018/01/19 */
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_SCHOOLWARS ) )	
		return true;

	/*pvp capture the flag, Juver, 2018/02/01 */
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_CAPTURE_THE_FLAG ) )	
		return true;

	/* pvp club death match, Juver, 2020/11/26 */
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH ) )	
		return true;

	if ( IsVisibleGroup( REBIRTH_DIALOGUE_PBG ) )	
		return true;

	return false;
}

/* map window mark, Juver, 2020/03/31 */
void CInnerInterface::LargeMapWindowRefresh()
{
	if ( m_pLargeMapWindow && m_pLargeMapWindow->IsVisible() )
	{
		m_pLargeMapWindow->MAP_MARK_RESET();
		m_pLargeMapWindow->MAP_MARK_INIT();
	}
}

/* pvp club death match, Juver, 2020/11/23 */
void CInnerInterface::PVPClubDeathMatchShowStartNotice()
{
	ShowGroupFocus( PVP_CLUB_DEATH_MATCH_BATTLE_START_NOTICE );
}

/* pvp club death match, Juver, 2020/11/23 */
void CInnerInterface::PVPClubDeathMatchShowEndNotice()
{
	ShowGroupFocus( PVP_CLUB_DEATH_MATCH_BATTLE_END_NOTICE );
}

/* pvp club death match, Juver, 2020/11/27 */
void CInnerInterface::PVPClubDeathMatchScoreResetPos()
{
	if ( !m_pPVPClubDeathMatchScore )		return;
	if ( !m_pPVPClubDeathMatchScoreDummy )	return;

	m_pPVPClubDeathMatchScore->SetGlobalPos( D3DXVECTOR2( m_pPVPClubDeathMatchScoreDummy->GetGlobalPos ().left, m_pPVPClubDeathMatchScoreDummy->GetGlobalPos ().top ) );
}

/* pvp club death match, Juver, 2020/11/28 */
void CInnerInterface::PVPClubDeathMatchShowTopRanking()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	/*pvp reward info, Juver, 2018/07/25 */
	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_CLUB_DEATH_MATCH_TOP_RANKINGS );

	if ( m_pPVPClubDeathMatchTopRankings )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPClubDeathMatchTopRankings->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

/* pvp club death match, Juver, 2020/11/29 */
void CInnerInterface::PVPClubDeathMatchShowRanking()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_CLUB_DEATH_MATCH ))
	{
		//character is dead show rank window later when revived!
		m_bPVPClubDeathMatchShowRanking = true;
		return;
	}

	if ( !IsVisibleGroup( PVP_CLUB_DEATH_MATCH_RANKINGS ))
	{
		SetDefaultPosInterface( PVP_CLUB_DEATH_MATCH_RANKINGS );
		ShowGroupFocus( PVP_CLUB_DEATH_MATCH_RANKINGS );
	}

	if ( m_pPVPClubDeathMatchRankings )
	{
		m_pPVPClubDeathMatchRankings->UpdateRankings();
	}

	m_bPVPClubDeathMatchShowRanking = false;
}

/* pvp club death match, Juver, 2020/12/01 */
void CInnerInterface::PVPClubDeathMatchShowDoublePoint( BOOL bShow )
{
	if ( bShow )
	{
		ShowGroupTop ( PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_DISPLAY );

		if( m_pPVPClubDeathMatchDoublePointDisplay )
		{
			UIGUID cID = CPVPClubDeathMatchDoublePointDisplay::PVP_CLUB_DEATH_MATCH_DOUBLE_POINT_IMAGE_00;

			m_pPVPClubDeathMatchDoublePointDisplay->START( cID );
			m_pPVPClubDeathMatchDoublePointDisplay->KEEP_START ( cID );
		}
	}
	else
	{
		if( m_pPVPClubDeathMatchDoublePointDisplay )
			m_pPVPClubDeathMatchDoublePointDisplay->ALLSTOP ();
	}
}

/* pvp club death match, Juver, 2020/12/03 */
void CInnerInterface::PVPClubDeathMatchShowRewardInfo()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	return;
	if ( !m_pPVPCompetitionWindow )	return;

	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_CLUB_DEATH_MATCH_REWARD_INFO );

	if ( m_pPVPClubDeathMatchRewardInfo )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPClubDeathMatchRewardInfo->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

void CInnerInterface::CharacterSimplifyChange()
{
	//cast to int for increment, cannot increment an enum
	int nCharacterView = static_cast<int>(RPROFILE::emCharacterView);
	nCharacterView ++;
	
	//validity check
	if ( nCharacterView >= EMCHARACTER_VIEW_SIZE )
		nCharacterView = EMCHARACTER_VIEW_ALL;
	
	RPROFILE::emCharacterView = static_cast<EMCHARACTER_VIEW>(nCharacterView);

	switch ( RPROFILE::emCharacterView )
	{
	case EMCHARACTER_VIEW_ALL:
		PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CHANGE_SIMPLYFY_ALL") );
		break;

	case EMCHARACTER_VIEW_SIMPLIFY:
		PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CHANGE_SIMPLYFY_SIMPLE") );
		break;;
	}
	
	GLGaeaClient::GetInstance().UpdateSuitClientActor();

}

void CInnerInterface::OpenAutoPotionWindow()
{
	if ( IsVisibleGroup( AUTO_POTION_WINDOW ) )
	{
		HideGroup( AUTO_POTION_WINDOW );
	}
	else
	{
		//function is disabled
		//just return
		if ( RPARAM::bUseAutoPotion == FALSE )
			return;

		//not allowed for this map
		if ( GLGaeaClient::GetInstance().GetActiveMap()->isUseAutoPotion() == FALSE )
		{
			PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_POTION_FB_NOT_MAP") );
			return;
		}
		
		ShowGroupFocus( AUTO_POTION_WINDOW );
	}
}

void CInnerInterface::OpenAutoPilotWindow()
{
	if ( IsVisibleGroup( AUTO_PILOT_WINDOW ) )
	{
		HideGroup( AUTO_PILOT_WINDOW );
	}
	else
	{
		//function is disabled
		//just return
		if ( RPARAM::bUseAutoPilot == FALSE )
			return;

		//not allowed for this map
		if ( GLGaeaClient::GetInstance().GetActiveMap()->isUseAutoPilot() == FALSE )
		{
			PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_NOT_MAP") );
			return;
		}

		ShowGroupFocus( AUTO_PILOT_WINDOW );
	}
}

void CInnerInterface::OpenCardRebornA( WORD wPosX, WORD wPosY )
{
	if ( IsVisibleGroup ( CHARACTER_REBORN_A_WINDOW ) ) 
		return;

	if ( !m_pCharacterRebornAWindow )	
		return;

	m_pCharacterRebornAWindow->SetCard ( wPosX, wPosY );

	const long lResolution = GetResolution ();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	const UIRECT& rcWindow = m_pCharacterRebornAWindow->GetGlobalPos ();

	D3DXVECTOR2 vPos;
	vPos.x = (X_RES - rcWindow.sizeX) / 2.0f;
	vPos.y = (Y_RES - rcWindow.sizeY) / 2.0f;
	m_pCharacterRebornAWindow->SetGlobalPos ( vPos );

	ShowGroupFocus ( CHARACTER_REBORN_A_WINDOW );
}

void CInnerInterface::OpenExchangeItemWindow( WORD wPosX, WORD wPosY )
{
	if ( IsVisibleGroup ( EXCHANGE_ITEM_WINDOW ) ) 
		return ;

	if ( !m_pExchangeItemWindow  )
		return;

	if ( !m_pExchangeItemInvenWindow )
		return;

	CloseAllWindow();

	m_pExchangeItemWindow->SetCard ( wPosX, wPosY );

	const long lResolution = GetResolution ();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	D3DXVECTOR2 vPos;
	const UIRECT& rcInvenWindow = m_pExchangeItemInvenWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pExchangeItemInvenWindow->SetGlobalPos( vPos );
	ShowGroupFocus( EXCHANGE_ITEM_INVEN_WINDOW );

	const UIRECT& rcWindow = m_pExchangeItemWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcWindow.sizeX;
	m_pExchangeItemWindow->SetGlobalPos( vPos );
	ShowGroupFocus( EXCHANGE_ITEM_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqExchangeItemOpen();
}

void CInnerInterface::CloseExchangeItemWindow()
{
	HideGroup( EXCHANGE_ITEM_WINDOW );
	HideGroup( EXCHANGE_ITEM_INVEN_WINDOW );
	GLGaeaClient::GetInstance().GetCharacter()->ReqExchangeItemClose();	
	//GLGaeaClient::GetInstance().GetCharacter()->PreviewItemCancel();
}

void CInnerInterface::ShowCaptcha( DWORD dwValue )
{
	ShowGroupFocus(CAPTCHA_WINDOW);

	if ( m_pCaptchaWindow )
		m_pCaptchaWindow->ShowCaptcha(dwValue);
}

void CInnerInterface::HideCaptcha()
{
	//EndEdit should be automatically be called when window is hidden by HideGroup
	//but we make a call here to make sure
	//having an invisible edit window running is a hard bug to find
	if (m_pCaptchaWindow)
		m_pCaptchaWindow->EndEdit();

	HideGroup(CAPTCHA_WINDOW);
}

void CInnerInterface::CaptchaStartEdit()
{
	if (m_pCaptchaWindow)
		m_pCaptchaWindow->BeginEdit();
}

void CInnerInterface::PVPPBGScoreResetPos()
{
	if ( !m_pPVPPBGScore )	
		return;

	if ( !m_pPVPPBGScoreDummy )	
		return;

	m_pPVPPBGScore->SetGlobalPos( D3DXVECTOR2( m_pPVPPBGScoreDummy->GetGlobalPos ().left, m_pPVPPBGScoreDummy->GetGlobalPos ().top ) );
}

void CInnerInterface::PVPPBGShowTopRanking()
{
	if ( !IsVisibleGroup( PVP_COMPETITION_WINDOW ))	
		return;

	if ( !m_pPVPCompetitionWindow )	
		return;

	/*pvp reward info, Juver, 2018/07/25 */
	PVPCloseRewardInfo();

	ShowGroupFocus( PVP_PBG_TOPRANKINGS );

	if ( m_pPVPPBGTopRankings )
	{
		UIRECT rect = m_pPVPCompetitionWindow->GetGlobalPos();
		rect.left = rect.left + rect.sizeX;
		m_pPVPPBGTopRankings->SetGlobalPos( D3DXVECTOR2(rect.left, rect.top) );
	}
}

void CInnerInterface::PVPPBGShowRanking()
{
	if ( IsVisibleGroup( REBIRTH_DIALOGUE_PBG ))
	{
		//character is dead show rank window later when revived!
		m_bPVPPBGShowRanking = true;
		return;
	}

	if ( !IsVisibleGroup( PVP_PBG_RANKINGS ))
	{
		SetDefaultPosInterface( PVP_PBG_RANKINGS );
		ShowGroupFocus( PVP_PBG_RANKINGS );
	}

	if ( m_pPVPPBGRankings )
	{
		m_pPVPPBGRankings->UpdateRankings();
	}

	m_bPVPPBGShowRanking = false;
}

void CInnerInterface::VisibleTGRanking( bool bVisible )
{
	if ( bVisible )
	{
		HideGroup( QUEST_HELPER_DISPLAY );
		ShowGroupBottom ( TG_RANKING_DISPLAY );
	}
	else
	{
		HideGroup( TG_RANKING_DISPLAY );
		ShowGroupBottom ( QUEST_HELPER_DISPLAY );

	}
}

void CInnerInterface::RefreshTGRanking()
{
	if ( m_pTGRankingDisplay && m_pTGRankingDisplay->IsVisible() )
		m_pTGRankingDisplay->RefreshTGRanking();
}

void CInnerInterface::SetBattleRankingDisplayOpen(BOOL bOpen)
{
	if (m_pBattleRankingDisplay)
	{
		if (bOpen)
		{
			ShowGroupFocus(BATTLE_RANKING_DISPLAY);
		}
		else
		{
			HideGroup(BATTLE_RANKING_DISPLAY);
			m_pBattleRankingDisplay->ResetAll();
		}
	}
}
/* Gacha System, MontageDev 7/10/24 */

void CInnerInterface::OpenGachaWindow()
{
	if ( !IsVisibleGroup( GACHA_WINDOW ))	
	{
		ShowGroupFocus( GACHA_WINDOW );
		m_pGachaWindow->LoadData();
	}
	else
	{
		HideGroup ( GACHA_WINDOW );
	}
}
/* Gacha System, MontageDev 7/10/24 */

void CInnerInterface::GachaWindowSelect(WORD nSelected)
{
	if ( m_pGachaWindow && IsVisibleGroup( GACHA_WINDOW ) ) 
		m_pGachaWindow->SetItem(nSelected);
}
/* Gacha System, MontageDev 7/10/24 */

void CInnerInterface::SetGachaDraw()
{
	if ( m_pGachaWindow && IsVisibleGroup( GACHA_WINDOW ) ) 
		m_pGachaWindow->SetDraw();
}
/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void CInnerInterface::PVPTyrannyRankingDisplayShow( bool bVisible )
{
	if ( bVisible )
	{
		ShowGroupBottom ( PVP_TYRANNY_RANKING_DISPLAY );
	}
	else
	{
		HideGroup( PVP_TYRANNY_RANKING_DISPLAY );
	}
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void CInnerInterface::PVPTyrannyRankingDisplayRefresh()
{
	if ( m_pPVPTyrannyRankingsDisplayMain && m_pPVPTyrannyRankingsDisplayMain->IsVisible() )
		m_pPVPTyrannyRankingsDisplayMain->RefreshPage();
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void CInnerInterface::ClubWarRankingDisplayShow( bool bVisible )
{
	if ( bVisible )
	{
		ShowGroupBottom ( CLUB_WAR_RANKING_DISPLAY );
	}
	else
	{
		HideGroup( CLUB_WAR_RANKING_DISPLAY );
	}
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
void CInnerInterface::ClubWarRankingDisplayRefresh()
{
	if ( m_pClubWarRankingsDisplayMain && m_pClubWarRankingsDisplayMain->IsVisible() )
		m_pClubWarRankingsDisplayMain->RefreshPage();
}