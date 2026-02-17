#pragma	once

#include "../../Lib_Engine/GUInterface/UIMan.h"
#include "../../Lib_Client/G-Logic/GLCharClient.h"
#include "./InnerInterfaceGuid.h"

/*pvp tyranny, Juver, 2017/08/25 */
#include "../../Lib_Client/G-Logic/GLPVPTyrannyDefine.h" 

/* personal lock system, Juver, 2019/12/13 */
#include "../../Lib_Client/G-Logic/GLPersonalLock.h"

/*woe Arc Development 08-06-2024*/
#include "../Lib_Client/G-Logic/GLPVPWoeDefine.h"

/* party finder, Juver, 2020/01/16 */
#include "../../Lib_Client/G-Logic/GLPartyFinderDef.h"
//pandora
class CPandoraBoxWindow;
class CPandoraBoxButton;

class	CItemShopButton;

class	VoidMainNews;

class CPVPWoeTowerCaptureGuild;
class CPVPWoeTowerCapture;
class CPVPWoeTowerProgress;
class CPVPWoeRebirthDialogue;
class CWoeRanking;
class	CAcademyConftDisplay;	
class	CAdminMessageDisplay;	
class	CBasicButton;	
class	CBasicChat;	
class	CBasicGameMenu;	
class   CQBoxButton;
class	CBasicInfoView;	
class	CBasicPotionTray;	

class	CStudentCodexWindow;
		
class	CBasicVarTextBox;	
class	CBlockProgramAlarm;	
class	CBusWindow;	
	
class	CChatMacroWindow;	
class	CClubMake;	
class	CClubStorageWindow;	
class	CClubWindow;	
class	CConftConfirm;	
class	CConftDisplayMan;	
class	CConftModalWindow;	
class	CCountMsg;	
class	CDamageDisplayMan;	

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
class	CDialogueWindow;	
#else
/* modern dialogue window, Juver, 2020/02/03 */
class	CModernDialogueWindow;	
#endif

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
class	CFriendWindow;	
#else
/* modern friend window, Juver, 2020/03/02 */
class	CModernFriendWindow;	
#endif

class	CGambleAgainBox;	
class	CGambleAnimationBox;	
class   CPetSkinMixImage;
class	CGambleBox;	
class	CGambleResultEvenBox;	
class	CGambleResultOddBox;	
class	CGambleSelectBox;	
class	CHeadChatDisplayMan;	

class	CItemBankWindow;	
class	CVNGainSysInventory;
class   CVNGainSysGauge;
class	CItemMove;	
class	CItemRebuild;	// ITEMREBUILD_MARK
class	CKeySettingWindow;	
class	CLargeMapWindow;
class	CLargeMapWindowImage;
class	CMobPreviewWindow;
class	CMapMoveDisplay;	

/* multi gate out, Juver, 2020/11/19 */
class	CMultiMapMoveDisplay;

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
class	CMarketWindow;	
#else
/* modern market window, Juver, 2020/02/05 */
class	CModernMarketWindow;	
#endif

class	CMiniMap;	

class	CModalWindow;	
class	CNameDisplayMan;	
class	CPartyModalWindow;	

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
class	CMiniPartyWindow;	
class	CPartyWindow;	
#else
/* modern party window, Juver, 2019/12/19 */
class	CModernMiniPartyWindow;	
class	CModernPartyWindow;	
class	CModernPartyState;
#endif

class	CPetRebirthDialogue;// Monster7j	
class	CPetWindow;	
class	CPrivateMarketMake;	
class	CPrivateMarketSellWindow;	
class	CPrivateMarketShowMan;	
class	CPrivateMarketWindow;	
class	CPtoPWindow;	
class	CQuestAlarm;	
class	CQuestionItemDisplay;	

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 )
class	CQuestWindow;	
#else
/*quest ui, Juver, 2017/11/06 */
class	CModernQuestWindow;
#endif

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
class	CRebirthDialogue;	
#else
/* modern rebirth dialogue, Juver, 2020/03/31 */
class	CModernRebirthDialogue;
#endif

class	CReceiveNoteWindow;	
class	CSimpleHP;	
class	CSimpleMessageMan;	
class	CSkillTrayTab;	
class	CSkillWindowToTray;	
class	CSMSSendWindow;	
class	CStorageChargeCard;	
class	CStorageWindow;	
class	CSubMasterSet;	
class	CSystemMessageWindow;	
class	CTargetInfoDisplay;	

class	CTradeWindow;	
class	CUILeftTopGroup;	
class	CWaitServerDialogue;	
class	CWriteNoteWindow;	
struct	GLCHARLOGIC_CLIENT;	
class	GLCLUB;	
class	GLMapAxisInfo;	
struct	GLSKILL;	
struct	SCHARSKILL;	
struct	SITEMCUSTOM;	
struct	SNpcTalk;	
class	CBonusTimeGauge;
class	CBonusTimeDisplay;
class	CQuestHelper;
class	CVehicleWindow;
class	CThaiCCafeMark;
class	CItemGarbage;

class	CItemShopIconMan;
class	CShopItemSearchWindow;
class	CItemSearchResultWindow;
class	CSummonWindow;
class	CSummonPosionDisplay;
class	CAttendanceBookWindow;
class	CClubBattleModal;
class	CMapRequireCheck;
class	CTaxiWindow;
class	CItemMixWindow;

class	CGatherGauge;
class	CCdmRankingDisplay;

 /*game stats, Juver, 2017/06/22 */
class	CGameStats;

/*npc shop, Juver, 2017/07/25 */
class	CNPCShopWindow;

/*item preview, Juver, 2017/07/27 */
class	CItemPreviewWindow;

/*pet status, Juver, 2017/07/30 */
class	CPetStatus; 

 /*vehicle status, Juver, 2017/07/30 */
class	CVehicleStatus;

/*item link, Juver, 2017/07/31 */
class	CBasicVarTextBoxItemLink; 

/*vehicle booster system, Juver, 2017/08/11 */
class	CVehicleBoosterDisplay; 

/*box contents, Juver, 2017/08/29 */
class	CInfoBoxContents;			

/* Competition UI, Juver, 2017/08/24 */
class	CPVPCompetitionButton;
class	CPVPCompetitionNotify;
class	CPVPCompetitionWindow;
class	CPVPCompetitionNotice;

/* PK Streak Sigaw - Jxyyy */
class	CPKStreakKillNotice;
class	CPKStreakShutdownNotice;
class	CPVPNextBattleAndWinner; // PVP Next Battle UI - Jxyyy
/*pvp tyranny, Juver, 2017/08/25 */
class	CPVPTyrannyTowerCapture;
class	CPVPTyrannyTowerProgress;
class	CPVPTyrannyRebirthDialogue;
class	CPVPTyrannyCaptureNotice;
class	CPVPTyranny2CaptureNotice;
class	CPVPTyrannyRankings;
class	CPVPTyrannyTopRankings;
class	CPVPTyrannyBattleNotice;

/*item exchange, Juver, 2017/10/12 */
class	CNPCItemExchangeWindow;


/*product item, Juver, 2017/10/15 */
class	CProductButton;
class	CProductWindow;

/*student record ui, Juver, 2017/10/30 */
class	CStudentRecordButton;
class	CStudentRecordWindow;

/*activity system, Juver, 2017/11/05 */
class	CActivityItemWindow;

/*charinfoview , Juver, 2017/11/11 */
class	CCharacterInfoViewWindow;

/*bike color , Juver, 2017/11/12 */
class	CBikeColorWindow;

/*pet skill info display, Juver, 2017/12/01 */
/*dual pet skill, Juver, 2017/12/29 */
class	CPetSkillInfoDisplay;
class	CPetSkillInfoDisplay2;

/*change scale card, Juver, 2018/01/03 */
class	CChangeScaleWindow;

/*item color, Juver, 2018/01/10 */
class	CItemColorWindow;

/*change school card, Juver, 2018/01/12 */
class	CChangeSchoolWindow;

/*item transfer card, Juver, 2018/01/18 */
class	CItemTransferWindow;


/*school wars, Juver, 2018/01/19 */
class	CPVPSchoolWarsScore;
class	CPVPSchoolWarsRebirthDialogue;
class	CPVPSchoolWarsTopRankings;
class	CPVPSchoolWarsBattleNotice;
class	CPVPSchoolWarsRankings;

/*pvp capture the flag, Juver, 2018/02/01 */
class	CPVPCaptureTheFlagRebirthDialogue;
class	CPVPCaptureTheFlagTopRankings;
class	CPVPCaptureTheFlagBattleNotice;
class	CPVPCaptureTheFlagScore;
class	CPVPCaptureTheFlagRankings;
class	CPVPCaptureTheFlagHoldIcon;


/* car, cart color, Juver, 2018/02/14 */
class	CCarColorWindow;

/*info window, Juver, 2018/03/12 */
class	CInfoWindow;

/*item random option rebuild, Juver, 2018/07/03 */
class	CItemRandomOptionRebuildWindow;

/*pvp reward info, Juver, 2018/07/24 */
class	CPVPTyrannyRewardInfo;
class	CPVPSchoolWarsRewardInfo;
class	CPVPCaptureTheFlagRewardInfo;

/*modern skill tray, Juver, 2018/08/21 */
class	CSkillTrayTabMini;
class	CSkillTrayTab;
class	CSkillTrayTabExMini;
class	CSkillTrayTabEx;

/*specific item box, Juver, 2018/09/02 */
class	CItemBoxSpecificSelectionWindow;

/*pk combo, EJCode, 2018/11/15 */
class	CPKComboDisplay;

#if defined( BUILD_EP4 )

class	CCharacterWindow;
class	CInventoryWindow;
class	CGarbageInventoryWindow;
class	CItemMixInvenWindow;
class	CRebuildInventoryWindow;
class	CTradeInventoryWindow;	
class	CItemTransferInvenWindow;						/*item transfer card, Juver, 2018/01/18 */
class	CItemRandomOptionRebuildInventoryWindow;		/*item random option rebuild, Juver, 2018/07/03 */
class	CChaosMachineInvenWindow;						/* chaos machine, Juver, 2021/07/08 */
class	CExchangeItemInventoryWindow;

class	CSkillWindow;

#elif defined( BUILD_EP6 )

class	CCharacterWindow;

class	CEP7InventoryWindow;			
class	CEP7GarbageInventoryWindow;
class	CEP7ItemMixInvenWindow;
class	CEP7RebuildInventoryWindow;
class	CEP7TradeInventoryWindow;
class	CEP7ItemTransferInvenWindow;					/*item transfer card, Juver, 2018/01/18 */
class	CEP7ItemRandomOptionRebuildInventoryWindow;		/*item random option rebuild, Juver, 2018/07/03 */
class	CEP7ChaosMachineInvenWindow;					/* chaos machine, Juver, 2021/07/08 */
class	CEP7ExchangeItemInventoryWindow;

class	CSkillWindow;

#elif defined ( BUILD_EP7 )

class	CCharacterWindow;

/*extended inventory window, EJCode, 2018/10/16 */
class	CEP7InventoryWindow;			
class	CEP7GarbageInventoryWindow;
class	CEP7ItemMixInvenWindow;
class	CEP7RebuildInventoryWindow;
class	CEP7TradeInventoryWindow;
class	CEP7ItemTransferInvenWindow;					/*item transfer card, Juver, 2018/01/18 */
class	CEP7ItemRandomOptionRebuildInventoryWindow;		/*item random option rebuild, Juver, 2018/07/03 */
class	CEP7ChaosMachineInvenWindow;					/* chaos machine, Juver, 2021/07/08 */
class	CEP7ExchangeItemInventoryWindow;

class	CSkillWindow;

#else

class	CModernCharacterWindow;							/*modern character window, Juver, 2018/03/12 */

/*modern inventory window, Juver, 2018/04/06 */
class	CModernInventoryWindow;							
class	CModernGarbageInventoryWindow;
class	CModernItemMixInvenWindow;
class	CModernRebuildInventoryWindow;
class	CModernTradeInventoryWindow;
class	CModernItemTransferInvenWindow;					/*item transfer card, Juver, 2018/01/18 */
class	CModernItemRandomOptionRebuildInventoryWindow;	/*item random option rebuild, Juver, 2018/07/03 */
class	CModernChaosMachineInvenWindow;					/* chaos machine, Juver, 2021/07/08 */
class	CModernExchangeItemInventoryWindow;

class	CModernSkillWindow;

#endif // 

/*target information display, Juver, 2018/08/22 */
class	CNpcDialogueSet;
class	GLCharacter;

class	CPVPClubWarRankingWindow;

class CCurrencyShopWindow;

/* personal lock system, Juver, 2019/12/06 */
class CPersonalLockInfo;
class CPersonalLockPinCreate;
class CPersonalLockPinInput;
class CPersonalLockPinReset;
class CPersonalLockPinChange;
class CPersonalLockPinRecover;
class CPersonalLockPinRecoverResult;

/* party finder, Juver, 2019/12/28 */
class CPartyFinderWindow;
class CPartyFinderQuestion;

/* modern info tooltip, Juver, 2020/04/09 */
class CBasicVarTextBoxEx;
class CBasicVarTextBoxExItemLink;

/* pvp club death match, Juver, 2020/11/26 */
class CPVPClubDeathMatchRebirthDialogue;
class CPVPClubDeathMatchBattleNotice;
class CPVPClubDeathMatchScore;
class CPVPClubDeathMatchTopRankings;
class CPVPClubDeathMatchRankings;
class CPVPClubDeathMatchDoublePointDisplay;
class CPVPClubDeathMatchRewardInfo;
/* Add ItemShop, Ssodomain, 11-09-2023, Start */
class	CItemShopWindow;
class	CItemShopButton;
/* Add ItemShop, Ssodomain, 11-09-2023, End */
/* chaos machine, Juver, 2021/07/08 */
class CChaosMachineWindow;

class CPKMarkDisplay;

class CNPCContributionShopWindow;

class CMaxRvPreviewWindow;

class CAutoPotionWindow;
class CAutoPilotWindow;

/* Boss Spawn Viewer, Review000 */
class	CBossViewerButton;
class	CBossDetailsWindow;

class CCharacterRebornAWindow;

class CExchangeItemWindow;

class CCaptchaWindow;

class CPVPPBGScore;
class CPVPPBGRebirthDialogue;
class CPVPPBGTopRankings;
class CPVPPBGRankings;
/* Gacha System, MontageDev 7/10/24 */
class	CGachaButton;
class	CGachaWindow;

class	CTGRankingDisplay;
class	CBattleRankingDisplay;

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
class   CPVPTyrannyRankingsDisplayMain;
/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
class	CClubWarRankingsDisplayMain;

/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
class	CGlobalRankingWindow;
/////////////////////////////////////////////////////////////////////////////
//	NOTE
//		대련시에 키워드
enum
{
	CONFT_WIN_INDEX = 0,
	CONFT_LOSE_INDEX,
	CONFT_DRAW_INDEX
};

//	NOTE
//		학교
enum
{	
	SUNGMOON = 0,
	HYUNAM,
	BONGHWANG,
	INIT_SCHOOL = 100,
};

const float fDEFAULT_WAITTIME_LEFT = 5.0f;

class	CInnerInterface : public CUIMan
{
private:
	//	정보 표시 타입
	enum ET_INFOTYPE
	{		
		ET_ITEM_INFO,
		ET_SKILL_INFO,
		ET_GENERAL_INFO,
		ET_ITEM_INFO_LINK, /*item link, Juver, 2017/07/31 */
	};

	/*target information display, Juver, 2018/08/22 */
public:
	typedef std::vector<CString>		INTERFACE_CSTRING_VEC;
	typedef std::vector<std::string>	INTERFACE_STDSTRING_VEC;

private:
    //pandora kuno
	CPandoraBoxWindow*			m_pPandoraBoxWindow;
    CPandoraBoxButton*			m_pPandoraBoxButton;
public:	
	//pandora
    void	SetPandoraBoxInfo();
	void	ClosePandoraBoxWindow();
public:
	static const int nOUTOFRANGE;
	static const float fMENU_LIFTUP;

private: //	등록되는 모든 컨트롤
	/////////////////////////////////////////////////////////////////////////////
	/*woe Arc Development 08-06-2024*/
	CWoeRanking* m_pWoeRanking;
	CPVPWoeTowerCaptureGuild* m_pPVPWoeTowerCaptureGuild;
	CPVPWoeTowerCapture* m_pPVPWoeTowerCapture;
	CUIControl* m_pPVPWoeTowerCaptureDummy;
	CPVPWoeTowerProgress* m_pPVPWoeTowerProgress;
	CPVPWoeRebirthDialogue* m_pRebirthDialogueWoe;
	// LG-7 GlobalRanking
	CGlobalRankingWindow*		m_pGlobalRankingWindow;
	/////////////////////////////////////////////////////////////////////////////
	CAcademyConftDisplay*		m_pAcademyConftDisplay;				
	CAdminMessageDisplay*		m_pAdminMessageDisplay;				
	CBasicChat*					m_pChat;				
	CBasicGameMenu*				m_pGameMenu;		
	CQBoxButton*				m_pQBoxButton;
	CBasicInfoView*				m_pBasicInfoView;		
		
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CBasicVarTextBox*			m_pInfoDisplayMain;			
	CBasicVarTextBox*			m_pInfoDisplaySub;		

	/*item link, Juver, 2017/07/31 */
	CBasicVarTextBoxItemLink*	m_pInfoDisplayItemLink;	
#else
	/* modern info tooltip, Juver, 2020/04/09 */
	CBasicVarTextBoxEx*			m_pInfoDisplayMain;			
	CBasicVarTextBoxEx*			m_pInfoDisplaySub;	
	/*item link, Juver, 2017/07/31 */
	CBasicVarTextBoxExItemLink*	m_pInfoDisplayItemLink;		
#endif

	CBlockProgramAlarm*			m_pBlockProgramAlarm;				
	CBusWindow*					m_pBusWindow;				
		
	CChatMacroWindow*			m_pChatMacroWindow;			
	CClubMake*					m_pClubMake;	
	CClubStorageWindow*			m_pClubStorageWindow;			
	CClubWindow*				m_pClubWindow;		
	CConftConfirm*				m_pConftConfirm;		
	CConftDisplayMan*			m_pConftDisplayMan;			
	CConftModalWindow*			m_pConftModalWindow;			
	CCountMsg*					m_pCountMsgWindow;	
	CDamageDisplayMan*			m_pDamageDisplayMan;	

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CDialogueWindow*			m_pDialogueWindow;		
#else
	/* modern dialogue window, Juver, 2020/02/03 */
	CModernDialogueWindow*		m_pDialogueWindow;	
#endif
	
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CFriendWindow*				m_pFriendWindow;	
#else
	/* modern friend window, Juver, 2020/03/02 */
	CModernFriendWindow*		m_pFriendWindow;	
#endif

	CGambleAgainBox*			m_pGambleAgainBox;			
	CGambleAnimationBox*		m_pGambleAnimationBox;				
	CPetSkinMixImage*			m_pPetSkinMixImage;
	CGambleBox*					m_pGambleBox;	
	CGambleResultEvenBox*		m_pGambleResultEvenBox;				
	CGambleResultOddBox*		m_pGambleResultOddBox;				
	CGambleSelectBox*			m_pGambleSelectBox;			
	CHeadChatDisplayMan*		m_pHeadChatDisplayMan;				
			
	CItemBankWindow*			m_pItemBankWindow;				
	CVNGainSysInventory*		m_pVNGainSysInventory;				
	CVNGainSysGauge*			m_pVNGainSysGauge;
	CItemMove*					m_pItemMove;		
	CItemRebuild*				m_pItemRebuildWindow;	// ITEMREBUILD_MARK	
	CKeySettingWindow*			m_pKeySettingWindow;			
	CLargeMapWindow*			m_pLargeMapWindow;	
	CLargeMapWindowImage*		m_pLargeMapWindowImage;			
	CMobPreviewWindow*			m_pMobPreviewWindow;		
	CMapMoveDisplay*			m_pMapMoveDisplay;	

	/* multi gate out, Juver, 2020/11/19 */
	CMultiMapMoveDisplay*		m_pMultiMapMoveDisplay;
	CUIControl*					m_pMultiMapMoveDisplayDummy;

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CMarketWindow*				m_pMarketWindow;		
#else
	/* modern market window, Juver, 2020/02/05 */
	CModernMarketWindow*		m_pMarketWindow;	
#endif
	

	CMiniMap*					m_pMiniMap;				
	CModalWindow*				m_pModalWindow;		
	CNameDisplayMan*			m_pNameDisplayMan;				
	CPartyModalWindow*			m_pPartyModalWindow;			
	

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CPartyWindow*				m_pPartyWindow;		
	CMiniPartyWindow*			m_pMiniPartyWindow;		
#else
	/* modern party window, Juver, 2019/12/19 */
	CModernMiniPartyWindow*		m_pMiniPartyWindow;	
	CModernPartyWindow*			m_pPartyWindow;	
	CModernPartyState*			m_pPartyState;	
#endif

	CPetRebirthDialogue*		m_pPetRebirthDialogue;// Monster7j				
	CPetWindow*					m_pPetWindow;	
	CPrivateMarketMake*			m_pPrivateMarketMake;			
	CPrivateMarketSellWindow*	m_pPrivateMarketSellWindow;				
	CPrivateMarketShowMan*		m_pPrivateMarketShowMan;				
	CPrivateMarketWindow*		m_pPrivateMarketWindow;				
	CPtoPWindow*				m_pPtoPWindow;			
	CQuestAlarm*				m_pQuestAlarm;				
	CQuestionItemDisplay*		m_pQuestionItemDisplay;		

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 )
	CQuestWindow*				m_pQuestWindow;		
#else
	/*quest ui, Juver, 2017/11/06 */
	CModernQuestWindow*			m_pQuestWindow;
#endif
	
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CRebirthDialogue*			m_pRebirthDialogue;			
#else
	/* modern rebirth dialogue, Juver, 2020/03/31 */
	CModernRebirthDialogue*		m_pRebirthDialogue;			
#endif

			
	CSimpleHP*					m_pSimpleHP;				
	CSimpleHP*					m_pSummonHP;				
			
	CSkillWindowToTray*			m_pSkillWindowToTray;				
	CStorageChargeCard*			m_pStorageChargeCard;			
	CStorageWindow*				m_pStorageWindow;			
	CSubMasterSet*				m_pSubMasterSet;		// 부마스터 설정 창
	CSystemMessageWindow*		m_pSystemMessageWindow;				
	CTargetInfoDisplay*			m_pTargetInfoDisplay;				
					
	CTradeWindow*				m_pTradeWindow;			
	CUILeftTopGroup*			m_pUILeftTopGroup;			
	CWaitServerDialogue*		m_pWaitServerDisplay;
	CBonusTimeGauge*			m_pBonusTimeGauge;
	CBonusTimeDisplay*			m_pBonusTimeDisplay;
	CQuestHelper*				m_pQuestHelper;
	CVehicleWindow*				m_pVehicleWindow;
	CThaiCCafeMark*				m_pThaiCCafeMark;
	CItemGarbage*				m_pItemGarbageWindow;
	
	CItemShopIconMan*			m_pItemShopIconMan;
	CShopItemSearchWindow*		m_pShopItemSearchWindow;
	CItemSearchResultWindow*	m_pItemSearchResultWindow;
	CSummonWindow*				m_pSummonWindow;
	CAttendanceBookWindow*		m_pAttendanceBookWindow;
	CClubBattleModal*			m_pClubBattleModalWindow;
	CTaxiWindow*				m_pTaxiWindow;
	CMapRequireCheck*			m_pMapRequireCheckWindow;
	CItemMixWindow*				m_pItemMixWindow;
	
	CGatherGauge*				m_pGatherGauge;
	CCdmRankingDisplay*			m_pCdmRankingDisplay;

	/*game stats, Juver, 2017/06/22 */
	CGameStats*					m_pGameStats; 

	/*npc shop, Juver, 2017/07/25 */
	CNPCShopWindow*				m_pNPCShopWindow;

	/*item preview, Juver, 2017/07/27 */
	CItemPreviewWindow*			m_pItemPreviewWindow;

	/*pet status, Juver, 2017/07/30 */
	CPetStatus*					m_pPetStatus; 

	/*vehicle status, Juver, 2017/07/30 */
	CVehicleStatus*				m_pVehicleStatus; 

	/*vehicle booster system, Juver, 2017/08/11 */
	CVehicleBoosterDisplay*		m_pVehicleBoosterDisplay; 

	/*box contents, Juver, 2017/08/29 */
	CInfoBoxContents*			m_pInfoBoxContents;		

	VoidMainNews*				m_pVoidMainNews;

	/* Competition UI, Juver, 2017/08/24 */
	CPVPCompetitionButton*		m_pPVPCompetitionButton;
	CPVPCompetitionNotify*		m_pPVPCompetitionNotify;
	CPVPCompetitionWindow*		m_pPVPCompetitionWindow;
	CPVPCompetitionNotice*		m_pPVPCompetitionNotice; 

	CPVPNextBattleAndWinner*	m_pPVPNextBattleAndWinner; // PVP Next Battle UI - Jxyyy
	/* PK Streak Sigaw - Jxyyy */
	CPKStreakKillNotice*				m_pPKStreakKillNotice; 
	CPKStreakShutdownNotice*			m_pPKStreakShutdownNotice; 
	/*pvp tyranny, Juver, 2017/08/25 */
	CPVPTyrannyTowerCapture*	m_pPVPTyrannyTowerCapture;
	CUIControl*					m_pPVPTyrannyTowerCaptureDummy;
	CPVPTyrannyTowerProgress*	m_pPVPTyrannyTowerProgress;
	CPVPTyrannyRebirthDialogue*	m_pRebirthDialogueTyranny;
	CPVPTyrannyCaptureNotice*	m_pPVPTyrannyNoticeCapture[TYRANNY_SCHOOL_SIZE];
	CPVPTyranny2CaptureNotice*	m_pPVPTyranny2Capture[TYRANNY_SCHOOL_SIZE];
	CPVPTyranny2CaptureNotice*	m_pPVPTyranny2CaptureOwn;			
	CPVPTyrannyRankings*		m_pPVPTyrannyRankings;
	CPVPTyrannyTopRankings*		m_pPVPTyrannyTopRankings;
	CPVPTyrannyBattleNotice*	m_pPVPTyrannyBattleNoticeStart;
	CPVPTyrannyBattleNotice*	m_pPVPTyrannyBattleNoticeEnd;

	/*item exchange, Juver, 2017/10/12 */
	CNPCItemExchangeWindow*		m_pNPCItemExchangeWindow;

	/*product item, Juver, 2017/10/15 */
	CProductButton*				m_pProductButton;
	CProductWindow*				m_pProductWindow;


	/*student record ui, Juver, 2017/10/30 */
	CStudentRecordButton*		m_pStudentRecordButton;
	CStudentRecordWindow*		m_pStudentRecordWindow;

	CItemShopButton*	m_pItemShopButton; // Item Mall - JX

	/*activity system, Juver, 2017/11/05 */
	CActivityItemWindow*		m_pActivityItemWindow;

	/*charinfoview , Juver, 2017/11/11 */
	CCharacterInfoViewWindow*	m_pCharacterInfoViewWindow;

	/*bike color , Juver, 2017/11/12 */
	CBikeColorWindow*			m_pBikeColorWindow;

	/*pet skill info display, Juver, 2017/12/01 */
	/*dual pet skill, Juver, 2017/12/29 */
	CPetSkillInfoDisplay*		m_pPetSkillInfoDisplay;
	CPetSkillInfoDisplay2*		m_pPetSkillInfoDisplay2;

	/*change scale card, Juver, 2018/01/03 */
	CChangeScaleWindow*			m_pChangeScaleWindow;

	/*item color, Juver, 2018/01/10 */
	CItemColorWindow*			m_pItemColorWindow;

	/*change school card, Juver, 2018/01/12 */
	CChangeSchoolWindow*		m_pChangeSchoolWindow;

	/*item transfer card, Juver, 2018/01/18 */
	CItemTransferWindow*		m_pItemTransferWindow;
	

	/*school wars, Juver, 2018/01/19 */
	CPVPSchoolWarsScore*				m_pPVPSchoolWarsScore;
	CUIControl*							m_pPVPSchoolWarsScoreDummy;
	CPVPSchoolWarsRebirthDialogue*		m_pRebirthDialogueSchoolWars;
	CPVPSchoolWarsTopRankings*			m_pPVPSchoolWarsTopRankings;
	CPVPSchoolWarsBattleNotice*			m_pPVPSchoolWarsBattleNoticeStart;
	CPVPSchoolWarsBattleNotice*			m_pPVPSchoolWarsBattleNoticeEnd;
	CPVPSchoolWarsRankings*				m_pPVPSchoolWarsRankings;

	/*pvp capture the flag, Juver, 2018/02/01 */
	CPVPCaptureTheFlagRebirthDialogue*	m_pRebirthDialogueCaptureTheFlag;
	CPVPCaptureTheFlagTopRankings*		m_pPVPCaptureTheFlagTopRankings;
	CPVPCaptureTheFlagBattleNotice*		m_pPVPCaptureTheFlagBattleNoticeStart;
	CPVPCaptureTheFlagBattleNotice*		m_pPVPCaptureTheFlagBattleNoticeEnd;
	CPVPCaptureTheFlagScore*			m_pPVPCaptureTheFlagScore;
	CUIControl*							m_pPVPCaptureTheFlagScoreDummy;
	CPVPCaptureTheFlagRankings*			m_pPVPCaptureTheFlagRankings;
	CPVPCaptureTheFlagHoldIcon*			m_pPVPCaptureTheFlagHoldIcon;


	/* car, cart color, Juver, 2018/02/14 */
	CCarColorWindow*					m_pCarColorWindow;
	/* Boss Spawn Viewer, Review000 */
	CBossDetailsWindow*			m_pBossDetailsWindow;
	CBossViewerButton*			m_pBossViewerButton;

	CStudentCodexWindow*		m_pStudentCodexWindow;

	/*info window, Juver, 2018/03/12 */
	CInfoWindow*						m_pInfoWindow;

	/*item random option rebuild, Juver, 2018/07/03 */
	CItemRandomOptionRebuildWindow*		m_pItemRandomOptionRebuildWindow;

	/*pvp reward info, Juver, 2018/07/25 */
	CPVPTyrannyRewardInfo*				m_pPVPTyrannyRewardInfo;
	CPVPSchoolWarsRewardInfo*			m_pPVPSchoolWarsRewardInfo;
	CPVPCaptureTheFlagRewardInfo*		m_pPVPCaptureTheFlagRewardInfo;

	/*modern skill tray, Juver, 2018/08/21 */	
	CSkillTrayTab*						m_pSkillTrayTab;
	CSkillTrayTabMini*					m_pSkillTrayTabMini;
	CSkillTrayTabEx*					m_pSkillTrayTabEx;
	CSkillTrayTabExMini*				m_pSkillTrayTabExMini;

	/*target information display, Juver, 2018/08/22 */
	CBasicVarTextBox*					m_ptarget_information_display;
	CUIControl*							m_ptarget_information_display_dummy;

	/*specific item box, Juver, 2018/09/02 */
	CItemBoxSpecificSelectionWindow*	m_pitem_box_specific_selection_window;

	/*pk combo, EJCode, 2018/11/15 */
	CPKComboDisplay*					m_ppk_combo_display;

#if defined ( BUILD_EP4 )

	CCharacterWindow*					m_pCharacterWindow;		

	CInventoryWindow*					m_pInventoryWindow;	
	CGarbageInventoryWindow*			m_pGarbageInventoryWindow;
	CItemMixInvenWindow*				m_pItemMixInvenWindow;
	CRebuildInventoryWindow*			m_pRebuildInventoryWindow;	
	CTradeInventoryWindow*				m_pTradeInventoryWindow;
	CItemTransferInvenWindow*			m_pItemTransferInvenWindow;								/*item transfer card, Juver, 2018/01/18 */
	CItemRandomOptionRebuildInventoryWindow*		m_pItemRandomOptionRebuildInventoryWindow;	/*item random option rebuild, Juver, 2018/07/03 */
	CChaosMachineInvenWindow*			m_pChaosMachineInvenWindow;								/* chaos machine, Juver, 2021/07/08 */
	CExchangeItemInventoryWindow*		m_pExchangeItemInvenWindow;

	CSkillWindow*						m_pCharacterSkillWindow; 

#elif defined ( BUILD_EP6 )

	CCharacterWindow*					m_pCharacterWindow;		

	/*extended inventory window, EJCode, 2018/10/16 */
	CEP7InventoryWindow*				m_pInventoryWindow;			
	CEP7GarbageInventoryWindow*			m_pGarbageInventoryWindow;
	CEP7ItemMixInvenWindow*				m_pItemMixInvenWindow;
	CEP7RebuildInventoryWindow*			m_pRebuildInventoryWindow;
	CEP7TradeInventoryWindow*			m_pTradeInventoryWindow;
	CEP7ItemTransferInvenWindow*		m_pItemTransferInvenWindow;								/*item transfer card, Juver, 2018/01/18 */
	CEP7ItemRandomOptionRebuildInventoryWindow*		m_pItemRandomOptionRebuildInventoryWindow;	/*item random option rebuild, Juver, 2018/07/03 */
	CEP7ChaosMachineInvenWindow*		m_pChaosMachineInvenWindow;								/* chaos machine, Juver, 2021/07/08 */
	CEP7ExchangeItemInventoryWindow*	m_pExchangeItemInvenWindow;

	CSkillWindow*						m_pCharacterSkillWindow; 

#elif defined ( BUILD_EP7 )

	CCharacterWindow*					m_pCharacterWindow;		

	/*extended inventory window, EJCode, 2018/10/16 */
	CEP7InventoryWindow*				m_pInventoryWindow;			
	CEP7GarbageInventoryWindow*			m_pGarbageInventoryWindow;
	CEP7ItemMixInvenWindow*				m_pItemMixInvenWindow;
	CEP7RebuildInventoryWindow*			m_pRebuildInventoryWindow;
	CEP7TradeInventoryWindow*			m_pTradeInventoryWindow;
	CEP7ItemTransferInvenWindow*		m_pItemTransferInvenWindow;								/*item transfer card, Juver, 2018/01/18 */
	CEP7ItemRandomOptionRebuildInventoryWindow*		m_pItemRandomOptionRebuildInventoryWindow;	/*item random option rebuild, Juver, 2018/07/03 */
	CEP7ChaosMachineInvenWindow*		m_pChaosMachineInvenWindow;								/* chaos machine, Juver, 2021/07/08 */
	CEP7ExchangeItemInventoryWindow*	m_pExchangeItemInvenWindow;

	CSkillWindow*						m_pCharacterSkillWindow; 

#else

	CModernCharacterWindow*				m_pCharacterWindow;										/*modern character window, Juver, 2018/03/12 */
	CModernInventoryWindow*				m_pInventoryWindow;										/*modern inventory window, Juver, 2018/04/06 */
	CModernGarbageInventoryWindow*		m_pGarbageInventoryWindow;
	CModernItemMixInvenWindow*			m_pItemMixInvenWindow;
	CModernRebuildInventoryWindow*		m_pRebuildInventoryWindow;	
	CModernTradeInventoryWindow*		m_pTradeInventoryWindow;
	CModernItemTransferInvenWindow*		m_pItemTransferInvenWindow;

	CModernItemRandomOptionRebuildInventoryWindow*	m_pItemRandomOptionRebuildInventoryWindow;	/*item random option rebuild, Juver, 2018/07/03 */

	CModernChaosMachineInvenWindow*		m_pChaosMachineInvenWindow;								/* chaos machine, Juver, 2021/07/08 */
	CModernExchangeItemInventoryWindow*	m_pExchangeItemInvenWindow;

	CModernSkillWindow*					m_pCharacterSkillWindow; 

#endif // 
	
	CPVPClubWarRankingWindow*			m_pPVPClubWarRankingWindow;

	CCurrencyShopWindow*				m_pCurrencyShopWindow;

	/* personal lock system, Juver, 2019/12/06 */
	CPersonalLockInfo*					m_pPersonalLockInfo;
	CPersonalLockPinCreate*				m_pPersonalLockPinCreate;
	CPersonalLockPinInput*				m_pPersonalLockPinInput;
	CPersonalLockPinReset*				m_pPersonalLockPinReset;
	CPersonalLockPinChange*				m_pPersonalLockPinChange;
	CPersonalLockPinRecover*			m_pPersonalLockPinRecover;
	CPersonalLockPinRecoverResult*		m_pPersonalLockPinRecoverResult;

	/* party finder, Juver, 2019/12/28 */
	CPartyFinderWindow*					m_pPartyFinderWindow;
	CPartyFinderQuestion*				m_pPartyFinderQuestion;
	
	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	CItemShopWindow*			m_pItemShopWindow;
	//CItemShopButton*			m_pItemShopButton;
	/* Add ItemShop, Ssodomain, 11-09-2023, End */



	/* pvp club death match, Juver, 2020/11/26 */
	CPVPClubDeathMatchRebirthDialogue*		m_pRebirthDialogueClubDeathMatch;
	CPVPClubDeathMatchBattleNotice*			m_pPVPClubDeathMatchBattleNoticeStart;
	CPVPClubDeathMatchBattleNotice*			m_pPVPClubDeathMatchBattleNoticeEnd;
	CPVPClubDeathMatchScore*				m_pPVPClubDeathMatchScore;
	CUIControl*								m_pPVPClubDeathMatchScoreDummy;
	CPVPClubDeathMatchTopRankings*			m_pPVPClubDeathMatchTopRankings;
	CPVPClubDeathMatchRankings*				m_pPVPClubDeathMatchRankings;
	CPVPClubDeathMatchDoublePointDisplay*	m_pPVPClubDeathMatchDoublePointDisplay;
	CPVPClubDeathMatchRewardInfo*			m_pPVPClubDeathMatchRewardInfo;

	/* chaos machine, Juver, 2021/07/08 */
	CChaosMachineWindow*					m_pChaosMachineWindow;

	CPKMarkDisplay*							m_pPKMarkDisplay;

	CNPCContributionShopWindow*				m_pContributionShopWindow;

	CMaxRvPreviewWindow*					m_pMaxRvPreviewWindow;

	CAutoPotionWindow*						m_pAutoPotionWindow;

	CAutoPilotWindow*						m_pAutoPilotWindow;

	CCharacterRebornAWindow*				m_pCharacterRebornAWindow;

	CExchangeItemWindow*					m_pExchangeItemWindow;

	CCaptchaWindow*							m_pCaptchaWindow;


	CPVPPBGScore*							m_pPVPPBGScore;
	CUIControl*								m_pPVPPBGScoreDummy;
	CPVPPBGRebirthDialogue*					m_pPVPPBGRebirthDialogue;
	CPVPPBGTopRankings*						m_pPVPPBGTopRankings;
	CPVPPBGRankings*						m_pPVPPBGRankings;

	CTGRankingDisplay*						m_pTGRankingDisplay;
	CBattleRankingDisplay*					m_pBattleRankingDisplay;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	CPVPTyrannyRankingsDisplayMain*		m_pPVPTyrannyRankingsDisplayMain;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	CClubWarRankingsDisplayMain*		m_pClubWarRankingsDisplayMain;

	/* Gacha System, MontageDev 7/10/24 */
	CGachaWindow*				m_pGachaWindow;
	CGachaButton*				m_pGachaButton;
private:
	CUIControl*	m_pSystemMessageWindowDummy;
	CUIControl*	m_pMapMoveDisplayDummy;
	CUIControl*	m_pBlockProgramAlarmDummy;
	CUIControl*	m_pLeftTopGroupDummy;
	CUIControl*	m_pBasicInfoViewDummy;
	CUIControl* m_pBasicPotionTrayDummy;
	CUIControl* m_pQuestAlarmDummy;

	void ResetControl();

private:
	bool m_bFirstVNGainSysCall;

public:
	CInnerInterface ();
	virtual	~CInnerInterface ();

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT OneTimeSceneInit ();
	virtual	HRESULT FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT FinalCleanup ();

public:
	void SetMiniMapInfo ( GLMapAxisInfo &sInfo, CString strMapName );
	void SetFirstVNGainSysCall ( bool bFirstCall ) { m_bFirstVNGainSysCall = bFirstCall; }

public:
	/* Boss Spawn Viewer, Review000 */
	CBossDetailsWindow*	GetBossDetailsWindow()				{ return m_pBossDetailsWindow; }	
	
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	CGlobalRankingWindow*	GetGlobalRankingWindow()		{ return m_pGlobalRankingWindow; }
	/////////////////////////////////////////////////////////////////////////////
	CMiniMap*				GetMiniMap()					{ return m_pMiniMap; }
	CSkillWindowToTray*		GetSkillWindowToTray()			{ return m_pSkillWindowToTray; }
	CBattleRankingDisplay*	GetBattleRankingDisplay()		{ return m_pBattleRankingDisplay; }

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CPartyWindow*			GetPartyWindow()				{ return m_pPartyWindow; }
#else
	CModernPartyWindow*		GetPartyWindow()				{ return m_pPartyWindow; }
#endif
	
	CBasicChat*				GetChat()						{ return m_pChat; }
	CNameDisplayMan*		GetDispName()					{ return m_pNameDisplayMan; }
	CPrivateMarketShowMan*	GetPMarketShow()				{ return m_pPrivateMarketShowMan; }
	CTradeWindow*			GetTradeWindow()				{ return m_pTradeWindow; }
	
	CPtoPWindow*			GetPtoPWindow()					{ return m_pPtoPWindow; }
	
	CItemMove*				GetItemMove()					{ return m_pItemMove; }

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CMarketWindow*			GetMarketWindow()				{ return m_pMarketWindow; }
#else
	/* modern market window, Juver, 2020/02/05 */
	CModernMarketWindow*	GetMarketWindow()				{ return m_pMarketWindow; }
#endif
	
	CHeadChatDisplayMan*	GetHeadChatDisplayMan()			{ return m_pHeadChatDisplayMan; }
	CConftModalWindow*		GetConftModalWindow()			{ return m_pConftModalWindow; }
	CPartyModalWindow*		GetPartyModalWindow()			{ return m_pPartyModalWindow; }
	CConftDisplayMan*		GetConftDisplayMan()			{ return m_pConftDisplayMan; }
	
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	CFriendWindow*			GetFriendWindow()				{ return m_pFriendWindow; }
#else
	/* modern friend window, Juver, 2020/03/02 */
	CModernFriendWindow*	GetFriendWindow()				{ return m_pFriendWindow; }
#endif

	CBasicGameMenu*			GetGameMenu()					{ return m_pGameMenu; }
	CQBoxButton*			GetQBoxButton()					{ return m_pQBoxButton; }
	CPetWindow*				GetPetWindow()					{ return m_pPetWindow; }

	CLargeMapWindow*		GetLargeMapWindow()				{ return m_pLargeMapWindow; }
	CLargeMapWindowImage*	GetLargeMapWindowImage()		{ return m_pLargeMapWindowImage; }
	CMobPreviewWindow*		GetMobPreviewWindow()			{ return m_pMobPreviewWindow; }

	CGambleBox*				GetGambleBox()					{ return m_pGambleBox; }
	CGambleSelectBox*		GetGambleSelectBox()			{ return m_pGambleSelectBox; }
	CGambleAgainBox*		GetGambleAgainBox()				{ return m_pGambleAgainBox; }
	CGambleAnimationBox*	GetGambleAnimationBox()			{ return m_pGambleAnimationBox; }
	CPetSkinMixImage*		GetPetSkinMixImage()			{ return m_pPetSkinMixImage; }
	CGambleResultEvenBox*	GetGambleResultEvenBox()		{ return m_pGambleResultEvenBox; }
	CGambleResultOddBox*	GetGambleResultOddBox()			{ return m_pGambleResultOddBox; }
	CModalWindow*			GetModalWindow()				{ return m_pModalWindow; }
	CKeySettingWindow*		GetKeySettingWindow()			{ return m_pKeySettingWindow; }
	CChatMacroWindow*		GetChatMacroWindow()			{ return m_pChatMacroWindow; }
	CItemShopIconMan*		GetItemShopIconMan()			{ return m_pItemShopIconMan; }
	CShopItemSearchWindow*	GetShopItemSearchWindow()		{ return m_pShopItemSearchWindow; }
	CItemSearchResultWindow* GetItemSearchResultWindow()	{ return m_pItemSearchResultWindow; }
	CSummonWindow*			GetSummonWindow()				{ return m_pSummonWindow; }
	CMapRequireCheck*		GetRequireCheck()				{ return m_pMapRequireCheckWindow; }
	CGameStats*				GetGameStatsDisplay()			{ return m_pGameStats; }				/*game stats, Juver, 2017/06/22 */
	CNPCShopWindow*			GetNPCShopWindow()				{ return m_pNPCShopWindow; }			/*npc shop, Juver, 2017/07/25 */
	CNPCItemExchangeWindow*	GetNPCItemExchangeWindow()		{ return m_pNPCItemExchangeWindow; }	/*item exchange, Juver, 2017/10/12 */
	
	CQuestHelper*			GetQuestHelper()				{ return m_pQuestHelper;	}			/*quest ui, Juver, 2017/11/11 */
	CCharacterInfoViewWindow*	GetCharInfoViewWindow()		{ return m_pCharacterInfoViewWindow; }	/*charinfoview , Juver, 2017/11/11 */
	CPVPSchoolWarsScore*	GetSchoolWarsScoreWindow()		{ return m_pPVPSchoolWarsScore; }		/*school wars, Juver, 2018/01/20 */
	CInfoWindow*			GetInfoWindow()					{ return m_pInfoWindow;	}				/*info window, Juver, 2018/03/12 */
	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	CItemShopWindow*		GetItemShop()					{ return m_pItemShopWindow; }
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
	
	/*woe Arc Development 08-06-2024*/
	CPVPWoeTowerCapture* GetTowerCapture() { return m_pPVPWoeTowerCapture; }
	
	CStudentRecordWindow*	GetStudentRecordWindow()		{ return m_pStudentRecordWindow; }

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 )
	CQuestWindow*				GetQuestWindow()				{ return m_pQuestWindow; }
#else
	CModernQuestWindow*			GetQuestWindow()				{ return m_pQuestWindow; }
#endif

#if defined( BUILD_EP4 )

	CCharacterWindow*			GetCharacterWindow()			{ return m_pCharacterWindow; }
	CInventoryWindow*			GetInventoryWindow()			{ return m_pInventoryWindow; }
	CTradeInventoryWindow*		GetTradeInventoryWindow()		{ return m_pTradeInventoryWindow; }

#elif defined ( BUILD_EP6 )

	CCharacterWindow*			GetCharacterWindow()			{ return m_pCharacterWindow; }

	CEP7InventoryWindow*		GetInventoryWindow()			{ return m_pInventoryWindow; }			/*extended inventory window, EJCode, 2018/10/16 */
	CEP7TradeInventoryWindow*	GetTradeInventoryWindow()		{ return m_pTradeInventoryWindow; }		/*extended inventory window, EJCode, 2018/10/16 */

#elif defined ( BUILD_EP7 )

	CCharacterWindow*			GetCharacterWindow()			{ return m_pCharacterWindow; }
	CEP7InventoryWindow*		GetInventoryWindow()			{ return m_pInventoryWindow; }			/*extended inventory window, EJCode, 2018/10/16 */
	CEP7TradeInventoryWindow*	GetTradeInventoryWindow()		{ return m_pTradeInventoryWindow; }		/*extended inventory window, EJCode, 2018/10/16 */

#else

	CModernCharacterWindow*	GetModernCharacterWindow()			{ return m_pCharacterWindow; }	/*modern character window, Juver, 2018/03/12 */

	/*modern inventory window, Juver, 2018/04/07 */
	CModernInventoryWindow*	GetInventoryWindow()				{ return m_pInventoryWindow; }	
	CModernTradeInventoryWindow*	GetTradeInventoryWindow()	{ return m_pTradeInventoryWindow; }

#endif // 

	/* chaos machine, Juver, 2021/07/08 */
	CChaosMachineWindow*	GetChaosMachineWindow()				{ return m_pChaosMachineWindow; }

	CNPCContributionShopWindow* GetContributionShopWindow()		{ return m_pContributionShopWindow; }

	CMaxRvPreviewWindow*	GetMaxRvPreviewWindow()		{ return m_pMaxRvPreviewWindow; }


public:
	void	CloseAllWindow ();
	void 	PreviewMob( SNATIVEID sMobID );

public:
	BOOL	PrintMsgText ( D3DCOLOR dwColor, const char* szFormat, ... );

	/*link drop item, EJCode, 2018/10/20 */
	BOOL	PrintMsgText ( D3DCOLOR dwColor, SITEMLINK* pItemLink, const char* szFormat, ... );

	BOOL	PrintConsoleText ( const char* szFormat, ... );

	BOOL	PrintMsgTextDlg ( D3DCOLOR dwColor, const char* szFormat, ... );
	BOOL	PrintConsoleTextDlg ( const char* szFormat, ... );

	BOOL	PrintMsgDlg ( D3DCOLOR dwColor, const char* szFormat, ... );

public:
	const char* MakeString ( const char* szFormat, ... );

public:
	BOOL IsGateOpen()							{ return m_bGateOpen; }
	void SetGateOpen( BOOL bGateOpen )			{ m_bGateOpen = bGateOpen; }
	void SetDamage( D3DXVECTOR3 vPos, int nDamage, DWORD dwDamageFlag, BOOL bAttack );
	void SetTargetInfo( STARGETID sTargetID );
	void ResetTargetInfo()						{ HideGroup ( TARGETINFO_DISPLAY ); }

	void SetFightBegin ( const int nIndex );
	void SetFightEnd ( const int nResult );

	void SetAcademyFightBegin ( const int& nLMARK, const int& nLNUMBER, const int& nRMARK, const int& nRNUMBER );
	void SetAcademyUpdateNumber ( const int& nLNUMBER, const int& nRNUMBER );
	void SetAcademyFightEnd ();

	void SetBlockProgramFound( bool bFOUND )	{ m_bBlockProgramFound = bFOUND; }
	bool IsBlockProgramFound()					{ return m_bBlockProgramFound; }

	void SetBlockProgramAlarm( bool bAlarm )	{ m_bBlockProgramAlarm = bAlarm; }
	bool IsBlockProgramAlarm()					{ return m_bBlockProgramAlarm; }

public:	
	void	SetBattleRankingDisplayOpen(BOOL bOpen);

	void	SetDialogueWindowOpen ( STARGETID sCrowID, GLCHARLOGIC_CLIENT* pCHAR );
	
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	void	SetGlobalRankingWindowOpen(BOOL bOpen);
	/////////////////////////////////////////////////////////////////////////////
	void	SetPtoPWindowOpen ( PGLCHARCLIENT pCharClient, DWORD dwGaeaID, GLCLUB & sClub );
	void	SetQuestWindowOpen ( DWORD dwQuestID );
	void	SetBusWindowOpen ( const DWORD dwGaeaID, SNpcTalk* pNpcTalk );

	void	SetTradeWindowOpen ( CString szPlayerName, DWORD dwTargetID );
	void	SetTradeWindowClose ();
	void	SetStorageWindowOpen ( DWORD dwNPCID );
	void	SetStorageWindowClose ();
	void	SetClubStorageWindowOpen ();
	void	SetClubStorageWindowClose ();
	void	SetMarketWindowOpen ( SNATIVEID sNativeID );
	void	SetMarketWindowClose ();
	void	SetStorageChargeOpen ( const WORD& wPosX, const WORD& wPosY );
	void	SetItemBankWindowOpen ();
	void	SetItemBankInfo ();
	void	SetVNGainSysWindowOpen ();
	void	SetVNGainSysInfo ();
	void	SetDefaultPosInterface(UIGUID ControlID);

	void	OpenItemRebuildWindow();	// ITEMREBUILD_MARK
	void	CloseItemRebuildWindow();

	void	OpenItemGarbageWindow( BOOL bUseCard );	// 휴지통
	void	CloseItemGarbageWindow();

	void	OpenItemMixWindow( DWORD dwNpcID );		// 아이템 조합
	void	CloseItemMixWindow();
	void	SetItemMixResult( CString strMsg, bool bSuccess = false, bool bFail = false );

	void	OPEN_TAXI_WINDOW( WORD wPosX, WORD wPosY );			// 택시 카드

	void	SetPrivateMarketMake ();
	void	SetPrivateMarketOpen ( const bool& bOPENER, const DWORD& dwGaeaID );
	void	GetPrivateMarketInfo ( bool& bOPENER, DWORD& dwGaeaID );
	void	SetPrivateMarketClose ();	

	void	MODAL_PRIVATE_MARKET_SELLITEM ( const bool& bCOUNTABLE, const WORD& wPosX, const WORD& wPosY );

	void	SetClubMake ( const DWORD& dwNpcID );

	void	SetShotcutText ( DWORD nID, CString& strTemp );
	void	GetShotCutKey();
	void	GetChatMacro();
	void	AddChatMacro(int nIndex);
	void	ChatLog( bool bChatLogt, int nChatLogType );
	CString GET_RECORD_CHAT();
	CString GetdwKeyToString(int dwKey);

	void	SetArmSwap( BOOL bSwap );

	/*npc shop, Juver, 2017/07/25 */
	void	SetNPCShopWindowOpen ( SNATIVEID sNativeID );


public:
	void	SetLottoOpen ( const DWORD& dwGaeaID );

public:
	void	DOMODAL_ADD_FRIEND_REQ( const CString& strName );
	void	DOMODAL_CLUB_JOIN_ASK( const DWORD& dwMasterID, const CString& strClubName, const CString& strClubMaster );
	void	DOMODAL_UNION_JOIN_ASK( const DWORD& dwMasterID, const CString& strClubMaster );
	void	DOMODAL_CLUB_BATTLE_ASK( const DWORD& dwMasterID, const CString& strClubMaster, DWORD dwBattleTime, bool bAlliance = false );
	void	DOMODAL_CLUB_BATTLE_ARMISTICE_ASK( const DWORD& dwClubID, const CString& strClubName, bool bAlliance = false );
	void	DOMODAL_CLUB_AUTHORITY_ASK( const CString& strClubName );

	void	OPEN_MODAL ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID = NO_ID, BOOL bHide = FALSE );
	void	OPEN_MODALESS ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID = NO_ID, BOOL bHide = FALSE );
	void	CLOSE_MODAL ( const UIGUID cID, bool bMakeMsg );

public:
	DWORD GetEventQuestID()							{ return m_dwEventQuestID; }
	void SetEventQuestID( DWORD dwQuestID )			{ m_dwEventQuestID = dwQuestID; }

public:
	void	WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer = fDEFAULT_WAITTIME_LEFT );
	void	WAITSERVER_DIALOGUE_CLOSE();

public:
	void	ClearNameList ();

public:
	void	SetTradeWindowCloseReq ();

private:
	HRESULT	TEXTURE_PRE_LOAD ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT	TEXTURE_PRE_UNLOAD ();

public:
	BOOL IsInventoryWindowOpen()			{ return IsVisibleGroup( INVENTORY_WINDOW ); }
	BOOL IsStorageWindowOpen()				{ return IsVisibleGroup( STORAGE_WINDOW ); }
	BOOL IsClubStorageWindowOpen()			{ return IsVisibleGroup( CLUB_STORAGE_WINDOW ); }
	BOOL IsMarketWindowOpen()				{ return IsVisibleGroup( MARKET_WINDOW ); }

public:
	// 인터페이스 사용시 캐릭터의 움직임을 제어
	BOOL IsCharMoveBlock()							{ return m_bCharMoveBlock; }
	void SetCharMoveBlock()							{ m_bCharMoveBlock = TRUE; }
	void ResetCharMoveBlock()						{ m_bCharMoveBlock = FALSE; }
	BOOL IsOpenWindowToMoveBlock();

	BOOL IsSnapItem()								{ return m_bSnapItem; }
	void SetSnapItem()								{ m_bSnapItem = TRUE; }
	void ResetSnapItem()							{ m_bSnapItem = FALSE; }

	void SetFirstItemSlot()							{ m_bFirstItemSlot = false;	}
	void ResetFirstItemSlot()						{ m_bFirstItemSlot = true; }
	bool IsFirstItemSlot()							{ return m_bFirstItemSlot; }

	void SetFriendName( const CString& strName )	{ m_strMoveFriendName = strName; }
	const CString& GetFriendName() const			{ return m_strMoveFriendName; }

	void SetThaiCCafeClass( DWORD dwClass );
	void SetMyCCafeClass( int nClass );				// 말레이시아 PC방 이벤트

public:
	bool	SET_QUESTION_ITEM_ID ( int nID );

	bool	SET_KEEP_QUESTION_ITEM_ID ( int nID );
	void	RESET_KEEP_QUESTION_ITEM ();

	void	BONUS_TIME_EVENT_START( bool bCharging ); // 이벤트 시작
	void	BONUS_TIME_EVENT_END(); // 이벤트 종료
	void	BONUS_TIME_BUSTER_START(); // 경험치 시작
	void	BONUS_TIME_BUSTER_END(); // 경험치 종료

	// 베트남 탐닉 방지 시스템 게이지
	void	SET_VNGAINTYPE_GAUGE( int nPos, int nLimit );

	void	SET_QUEST_HELPER( DWORD dwQuestID );
	void	RESET_QUEST_HELPER( DWORD dwQuestID );

public:
	void	SET_CONFT_CONFIRM_VISIBLE ( bool bVisible );
	void	SET_CONFT_CONFIRM_PERCENT ( float fPercent );

public:
	void SetSkillUpID( const DWORD& dwID )			{ m_dwSkillUpID = dwID; }
	DWORD GetSkillUpID()							{ return m_dwSkillUpID; }
//	BOOL ISMASTER_SKILL( SNATIVEID skill_id );

private:
	void MoveBasicInfoWindow ();
	void BasicInfoViewDoubleClick();

private:
	BOOL	IsPartyMember ( const char* szName );

public:
	void	REFRESH_QUEST_WINDOW ();

	void	REFRESH_FRIEND_LIST ();
	void	REFRESH_FRIEND_STATE();

	void	REFRESH_CLUB_LIST ();
	void	REFRESH_CLUB_STATE ();
	void	REFRESH_CLUB_NOTICE();
	void	REFRESH_CLUB_ALLIANCE_LIST();
	void	REFRESH_CLUB_BATTLE_LIST();

	void	REFRESH_ITEMBANK ();
	void	REFRESH_VNGAINSYSTEM ();

	void	REFRESH_ATTENDBOOK();

	void	SET_GATHER_GAUGE( DWORD dwGaeaID, float fTime );

public:
	void	SetInventorySlotViewSize ( const int& nSLOT_VIEW_SIZE );
	int GetInventorySlotViewSize () const;

public:
	void ADD_FRIEND_NAME_TO_EDITBOX( const CString& strName );
	void ADD_FRIEND( const CString& strName );
	void FRIEND_LIST( CONST CString & strName, bool bOnline );

	const CString& GetPrivateMarketTitle ();

	void DoBattleModal( const CString strClubName, bool bAlliance = false );
	BOOL ReqClubBattle( DWORD dwTime, bool bAlliance );

private:
	ET_INFOTYPE		m_etInfoType;

public:
	void	InfoDisplayReset ();

	void	InfoDisplayShowItemSimple ( SITEMCUSTOM &sItemCustom );

	void	InfoDisplayShowItemAdvanced ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID = SNATIVEID(),
		BOOL bNPCShop = FALSE, WORD wNPCShopType = 0, BOOL bInInventory = FALSE, BOOL bInCompoundResult = FALSE, BOOL bCompoundGenerateRandomOption = FALSE,
		BOOL bCurrencyShop = FALSE, SNATIVEID sCurrencyShopNpcID = SNATIVEID(false), SNATIVEID sCurrencyShopItemID = SNATIVEID(false), DWORD dwMarketChannel = 0 );	

	void	InfoDisplayShowSkillAdvanced ( SNATIVEID sNativeID, const BOOL bNextLevel );

	void	InfoDisplayShowCommonSingleLine ( const CString& strText, D3DCOLOR dwColor );

	bool	InfoDisplayShowCommonMultiLineBegin  ();
	bool	InfoDisplayShowCommonMultiLineInsert ( const CString& strText, D3DCOLOR dwColor );
	void	InfoDisplayShowCommonMultiLineEnd ();

	void	InfoDisplayItemLinkReset ();
	void	InfoDisplayItemLinkClearText ();
	void	InfoDisplayItemLinkSet ( SITEMCUSTOM sITEM );
	void	InfoDisplayItemLinkShow ( SITEMCUSTOM &sItemCustom );	

	void	InfoDisplayClearText ();	
	int		InfoDisplayAddTextNoSplit ( const CString& strText, const D3DCOLOR& dwColor );
	void	InfoDisplayAddTextLongestLineSplit  ( const CString& strText, const D3DCOLOR& dwColor );
	void	InfoDisplayAddStringNoSplit( const int nIndex, const CString& strText, const D3DCOLOR& dwColor );

	/* modern info tooltip, Juver, 2020/04/09 */
	void	InfoDisplayAddTitle( const CString& strText, const D3DCOLOR& dwColor );
	void	InfoDisplaySetIcon( SNATIVEID sICONINDEX, const char* szTexture );

	/*npc shop, Juver, 2017/07/26 */
	/*product item, Juver, 2017/10/15 */
	void	SHOW_ITEM_INFO ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID = SNATIVEID(),
		BOOL bNPCShop = FALSE, WORD wNPCShopType = 0, BOOL bInInventory = FALSE, BOOL bInCompoundResult = FALSE, BOOL bCompoundGenerateRandomOption = FALSE,
		BOOL bCurrencyShop = FALSE, SNATIVEID sCurrencyShopNpcID = SNATIVEID(false), SNATIVEID sCurrencyShopItemID = SNATIVEID(false), DWORD dwMarketChannel = 0 );	

	void	SHOW_SKILL_INFO ( SNATIVEID sNativeID, const BOOL bNextLevel );

	void	SET_INFO_ITEM_LINK ( SITEMCUSTOM sITEM );

	/*extended buff info, EJCode, 2018/10/06 */
	void	SHOW_SKILL_INFO_SIMPLE(SNATIVEID sSkillID, WORD wLevel);

public:
	void	WARNING_MSG_ON()			{ ShowGroupBottom( WARNING_MSG_WINDOW ); }
	void	WARNING_MSG_OFF()			{ HideGroup( WARNING_MSG_WINDOW ); }

	void	SET_COUNT_MSG( INT nCount );

public:
	void	ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg );	
	void	MsgProcess ( LPVOID msgBuffer );

public:
	const	D3DXVECTOR3&	GetCharDir () const				{ return m_vCharDir; }

public:
	void	SetAniPic(int nPic);
	void	SetOddEven(BOOL bOdd);

public:
	void	DisplayChatMessage ( int nType, const char *szName, const char *szMsg );
	/* Chat Color, Mhundz 02/22/25 */
	void	DisplayChatMessage ( int nType, const char *szName, const char *szMsg, SITEMLINK sLINK, bool bStaff = false ); /*item link, Juver, 2017/07/31 */
	void	DisplayEnchantMessage ( const char *szName, SITEMCUSTOM sItemCustom );
	void	DisplayComboWhoreMessage ( const char *szName );

	//void	UpdateClubBattleTime( float fClubBattleTime );
	void	ClearItemBank();
	void	ClearVNGainSys();
	void	DisableMinimapTarget();

private:
	void	UpdateStatus ();
	void	UpdateShortcutBefore ();
	void	UpdateShortcutAfter ();

private:
	void	UpdateStateQuestAlarm ();
	void	UpdateStateSimpleHP ();
	void	UpdateSimpleMessage ();
public:
	void	ReqToggleRun ();
	bool	ItemShopAuth ();		// 일본 커스텀 브라우저 인증 모듈 //	ItemShopAuth
	void	ItemShopVisible();		// 일본 커스텀 브라우저 로딩 대기 //	ItemShopAuth


	void	VisibleCDMRanking( bool bVisible );
	void	RefreshCDMRanking();


	/*woe Arc Development 08-06-2024*/
	void VisibleWoeRanking(bool bVisible);
	void RefreshWoeRanking();
	
public:
	/*item preview, Juver, 2017/07/27 */
	bool	PreviewItemCheckSimple( SNATIVEID sID );
	bool	PreviewItemCheckWear( SNATIVEID sID );
	void	PreviewItem( SITEMCUSTOM sItem );

	/*pet status, Juver, 2017/07/30 */
	void	ShowPetStatus( bool bVisible );

	/*vehicle status, Juver, 2017/07/30 */
	void	ShowVehicleStatus( bool bVisible );

	/*item link, Juver, 2017/07/31 */
	void	LinkItem( SITEMCUSTOM* pItem );

	/*vehicle booster system, Juver, 2017/08/11 */
	void	VehicleBoosterVisible( bool bVisible );

	/*box contents, Juver, 2017/08/29 */
	void	ShowBoxContents( SNATIVEID sidItem );

	/* Competition UI, Juver, 2017/08/24 */
	void	PVPCompetitionButtonAlarm( EMCOMPETITION_NOTIFY_TYPE emType );
	void	PVPCompetitionWindowShow();
	void	PVPCompetitionShowNotice( DWORD dwTextColor, const char* szFormat, ... );

	int		PVPCompetitionShowNoticeMultiColor(DWORD dwTextColor, const char* szFormat, ...);
	int		PVPCompetitionShowNoticeAddString(int index, DWORD dwTextColor, const char* szFormat, ...);

	int		PVPCompetitionShowNoticeMultiColorPKDead(DWORD dwTextColor, const char* szFormat, ...);
	int		PVPCompetitionShowNoticeAddStringPKDead(int index, DWORD dwTextColor, const char* szFormat, ...);

	/* PK Streak Sigaw - Jxyyy */
	void	ShowPKStreakKillNotice( DWORD dwTextColor, const char* szFormat, ... );
	void	ShowPKStreakShutdownNotice( DWORD dwTextColor, const char* szFormat, ... );

	/*pvp tyranny, Juver, 2017/08/25 */
	void	PVPTyrannyTowerCaptureResetPos();
	void	PVPTyrannyShowCaptureNotice( WORD wSchool, DWORD dwClubID, DWORD dwClubMarkVer );
	void	PVPTyrannyShow2CaptureNotice( WORD wSchool );
	void	PVPTyrannyShow2CaptureOwnNotice();
	void	PVPTyrannyShowRanking();
	bool	PVPTyrannyToShowRanking()	{ return m_bPVPTyrannyShowRanking; }
	void	PVPTyrannyShowTopRanking();
	void	PVPTyrannyShowStartNotice();
	void	PVPTyrannyShowEndNotice();

	/*woe Arc Development 08-06-2024*/
	void	PVPWoeTowerCaptureResetPos();
	bool	PVPWoeToShowRanking() { return m_bPVPTyrannyShowRanking; }
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	void	PVPTyrannyRankingDisplayShow(bool bShow);
	void	PVPTyrannyRankingDisplayRefresh();

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	void	ClubWarRankingDisplayShow(bool bShow);
	void	ClubWarRankingDisplayRefresh();
	/*item exchange, Juver, 2017/10/12 */
	void	SetNPCItemExchangeWindowOpen( SNATIVEID sid_npc );

	/*product item, Juver, 2017/10/15 */
	void	OpenProductWindow();

	/*student record ui, Juver, 2017/10/30 */
	void	OpenStudentRecord();
	void	OpenStudentCodex(); /* Codex, Archie 02/16/24 */

	/*activity system, Juver, 2017/11/05 */
	void	OpenActivityItemWindow();


	/*item transfer card, Juver, 2018/01/18 */
	void	OpenItemTransferWindow();
	void	CloseItemTransferWindow();

	/*school wars, Juver, 2018/01/20 */
	void	PVPSchoolWarsScoreResetPos();
	void	PVPSchoolWarsShowTopRanking();
	void	PVPSchoolWarsShowStartNotice();
	void	PVPSchoolWarsShowEndNotice();
	void	PVPSchoolWarsShowRanking();
	bool	PVPSchoolWarsToShowRanking()	{ return m_bPVPSchoolWarsShowRanking; }

	/*pvp capture the flag, Juver, 2018/02/01 */
	void	PVPCaptureTheFlagShowTopRanking();
	void	PVPCaptureTheFlagShowStartNotice();
	void	PVPCaptureTheFlagShowEndNotice();
	void	PVPCaptureTheFlagScoreResetPos();
	void	PVPCaptureTheFlagShowRanking();
	bool	PVPCaptureTheFlagToShowRanking()	{ return m_bPVPCaptureTheFlagShowRanking; }

	/*info window, Juver, 2018/03/12 */
	void	InfoWindowOpen( UIGUID uid_caller = NO_ID );
	void	InfoWindowSetText( CString strText, DWORD dwColor );
	int		InfoWindowAddText( CString strText, DWORD dwColor );
	int		InfoWindowAddString( int nIndex, CString strText, DWORD dwColor );


	/*modern character window, Juver, 2018/03/15 */
	void SetItemInfoTemp( const SITEMCUSTOM& sID )		{ m_sItemInfoTemp = sID; }
	SITEMCUSTOM GetItemInfoTemp()						{ return m_sItemInfoTemp; }

	/*modern inventory window, Juver, 2018/04/07 */
	BOOL IsInventoryBlocked();

	/*item random option rebuild, Juver, 2018/07/02 */
	void	OpenItemRandomOptionRebuildWindow();
	void	CloseItemRandomOptionRebuildWindow();

	/*pvp reward info, Juver, 2018/07/24 */
	void	PVPCloseRewardInfo();
	void	PVPTyrannyShowRewardInfo();
	void	PVPSchoolWarsShowRewardInfo();
	void	PVPCaptureTheFlagShowRewardInfo();

	/*modern skill tray, Juver, 2018/08/20 */
	void	skill_tray_open( bool use_mini );
	void	skill_tray_lock();
	void	skill_tray_rotate();
	bool	skill_tray_get_lock()	{ return m_skill_tray_lock; }
	void	skill_tray_switch();
	INT		skill_tray_get_tab_index();

	/*target information display, Juver, 2018/08/22 */
	void	target_information_set( STARGETID target_id );
	void	target_information_show();
	void	target_information_reset();
	void	target_information_load_talk_node( CNpcDialogueSet* pdialogue_set, GLCharacter* pcharacter, DWORD talk_id, 
		INTERFACE_CSTRING_VEC& vec_npc_function, INTERFACE_CSTRING_VEC& vec_quest_start, INTERFACE_CSTRING_VEC& vec_quest_step, INTERFACE_CSTRING_VEC& vec_quest_complete );

	/*specific item box, Juver, 2018/09/02 */
	void	item_box_specific_selection_window_show( SNATIVEID item_id );

	/*pk combo, EJCode, 2018/11/15 */
	bool	PKComboSet ( int id );

	void	SetCompetitionJoin( bool bEnable );
	
	void PVPClubWarRankingWindowClose();
	void PVPClubWarRankingWindowRefresh();
	bool PVClubWarToShowRanking()	{ return m_bPVPClubWarShowRanking; }
	void PVPClubWarRankingWindowOpen();

	void CurrencyShopOpen( SNATIVEID sNpcID );
	void CurrencyShopUpdatePrice();

	/* personal lock system, Juver, 2019/12/06 */
	void PersonalLockCloseWindow();
	void PersonalLockPinCreate( EMPERSONAL_LOCK emLock );
	void PersonalLockPinInput( EMPERSONAL_LOCK emLock );
	void PersonalLockPinReset();

	void PersonalLockPinChange();
	void PersonalLockPinRecover();
	void PersonalLockPinRecoverResultOpen();
	void PersonalLockPinRecoverResultUpdate( SPERSONAL_LOCK_PIN* pPin, EMPERSONAL_LOCK emLock );

	/* party finder, Juver, 2019/12/28 */
	void OpenPartyFinderWindow();
	void RefreshPartyFinderWindow();
	void OpenPartyFinderQuestion( SPARTY_FINDER_QUESTION* pQuestion );

	bool isReviveWaiting();

	/* map window mark, Juver, 2020/03/31 */
	void LargeMapWindowRefresh();

	/* pvp club death match, Juver, 2020/11/23 */
	void PVPClubDeathMatchShowStartNotice();
	void PVPClubDeathMatchShowEndNotice();
	void PVPClubDeathMatchScoreResetPos();
	void PVPClubDeathMatchShowTopRanking();
	void PVPClubDeathMatchShowRanking();
	bool PVPClubDeathMatchToShowRanking()	{ return m_bPVPClubDeathMatchShowRanking; }
	void PVPClubDeathMatchShowDoublePoint( BOOL bShow );
	void PVPClubDeathMatchShowRewardInfo();
	/* Gacha System, MontageDev 7/10/24 */
	void	OpenGachaWindow();
	void	SetGachaDraw();
	void	GachaWindowSelect(WORD nSelected);

//#ifdef CH_PARAM // 중국 인터페이스 변경
//public:
//	void UpdatePotionTrayPosition();
//#endif

public:	//	ETC Func.
	SCONFT_OPTION*	GetConftOption ()			{ return &m_sReqConflictOption; }
	SPARTY_OPT*		GetPartyOption ()			{ return &m_sReqPartyOption; }

public:
	bool IsCHANNEL() { return m_bCHANNEL; }
	bool IsCHAT_BEGIN();

public:
	/* chaos machine, Juver, 2021/07/07 */
	void	OpenChaosMachineWindow( DWORD dwNPCID );
	void	CloseChaosMachineWindow();

	void	OpenContributionShopWindow( SNATIVEID sNpcID );

	void	ReqMaxRVShow( const SITEMCUSTOM& sItemCustom );
	void	MaxRvPreviewDisplayShow();

	void CharacterSimplifyChange();

	void OpenAutoPotionWindow();

	void OpenAutoPilotWindow();

	void OpenCardRebornA( WORD wPosX, WORD wPosY );

	void OpenExchangeItemWindow( WORD wPosX, WORD wPosY );
	void CloseExchangeItemWindow();

	void ShowCaptcha( DWORD dwValue );
	void HideCaptcha();
	void CaptchaStartEdit();

	void PVPPBGScoreResetPos();
	void PVPPBGShowTopRanking();

	void PVPPBGShowRanking();
	bool PVPPBGToShowRanking()	{ return m_bPVPSchoolWarsShowRanking; }

	void VisibleTGRanking( bool bVisible );
	void RefreshTGRanking();

private:
	BOOL			m_bGateOpen;
	BOOL			m_bCharMoveBlock;
	bool			m_bBlockProgramFound;
	bool			m_bBlockProgramAlarm;

	bool			m_bInfoDisplayActive;
	bool			m_bInfoDisplayModeSub;

	/*item link, Juver, 2017/07/31 */
	bool			m_bInfoDisplayModeItemLink;

	DWORD			m_dwEventQuestStep;
	DWORD			m_dwEventQuestID;
	EMCONFT_TYPE	m_emConflictReqType;
	DWORD			m_ConflictReqID;
	SCONFT_OPTION	m_sReqConflictOption;
	SPARTY_OPT		m_sReqPartyOption;
	BOOL			m_bSnapItem;
	BOOL			m_bPartyStateBack;
	CString			m_strFriendName;
	D3DXVECTOR3		m_vCharDir;
	DWORD			m_dwSkillUpID;
	
	bool			m_bACADEMY_FIGHT_READY;
	CString			m_strGeneralInfoBack;
	float			m_fMoveWindowDistBack;
	bool			m_bFirstItemSlot;
	DWORD			m_dwClubMasterID;
	CString			m_strMoveFriendName;
	bool			m_bCHANNEL;
	bool			m_bTabReserve;
	float			m_fVehicleDelay;
	float			m_fItemBankDelay;
	float			m_fItemShopDelay; // Ssod
	bool			m_bItemShopLoad;	//	ItemShopAuth

	/*item link, Juver, 2017/07/31 */
	SITEMCUSTOM		m_sItemInfoLinkTemp;

	/*pvp tyranny, Juver, 2017/08/25 */
	bool			m_bPVPTyrannyShowRanking;	

	/*woe Arc Development 08-06-2024*/
	bool m_bPVPWoeShowRanking;
	
	/*school wars, Juver, 2018/01/20 */
	bool			m_bPVPSchoolWarsShowRanking;	

	/*pvp capture the flag, Juver, 2018/02/02 */
	bool			m_bPVPCaptureTheFlagShowRanking;	

	/*modern character window, Juver, 2018/03/15 */
	SITEMCUSTOM		m_sItemInfoTemp;

	/*modern skill tray, Juver, 2018/08/21 */
	bool			m_skill_tray_lock;
	bool			m_skill_tray_mini;
	bool			m_skill_tray_rotate;

	/*target information display, Juver, 2018/08/22 */
	STARGETID		m_target_id_information;

	bool			m_bPVPClubWarShowRanking;

	/* pvp club death match, Juver, 2020/11/29 */
	bool			m_bPVPClubDeathMatchShowRanking;	

	bool			m_bPVPPBGShowRanking;	
	
/* Add ItemShop, Ssodomain, 11-09-2023, Start */
public:
	void SetItemShopDelay (float fData) {m_fItemShopDelay = fData;}
/* Add ItemShop, Ssodomain, 11-09-2023, Start */
public:
	static CInnerInterface& GetInstance();
};
