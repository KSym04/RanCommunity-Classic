#include "stdafx.h"
#include "./NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPARAM
{
	EMSERVICE_TYPE		emSERVICE_TYPE		= EMSERVICE_DEFAULT;

	
	SLOGIN_ADDRESS		sLoginAddress[MAX_LOGIN_IP] = 
	{
		SLOGIN_ADDRESS("127.0.0.1", 5001),
		SLOGIN_ADDRESS("127.0.0.1", 5002),
		SLOGIN_ADDRESS("127.0.0.1", 5003),
		SLOGIN_ADDRESS("127.0.0.1", 5004),
		SLOGIN_ADDRESS("127.0.0.1", 5005),
	};

	TCHAR	HttpAddressTable[MAX_HTTP][RANPARAM_STRING_NUM_128] =
	{
		"127.0.0.1", "", "", "", "",
		"", "", "", "", "",
		"", "", "", "", "",
		"", "", "", "", "",
		"", "", "", "",
	};

	TCHAR	FtpAddressTable[MAX_FTP][RANPARAM_STRING_NUM_128] =
	{
		"", "", "", "", "",
		"", "", "", "", "",
		"", "", "", "", "",
		"", "", "", "", "",
		"", "", "", "",
	};


	CString		strNewsURL			= "127.0.0.1";
	CString		strConnectionURL	= "127.0.0.1";
	BOOL		bUsePassiveDN		= FALSE;

	
	SERVER_SET	China_Region[MAX_CHINA_REGION];

	language::LANGFLAG emLangSet	= language::DEFAULT;
	CString		strGDIFont			= "Tahoma";

	BOOL		bXML_USE			= TRUE;
	CString		strCountry			= "common";
	CString		strLauncherText		= "Launcher.xml";
	CString		strGameInText		= "gameintext.xml";
	CString		strGameExText		= "gameextext.xml";
	CString		strGameWord			= "gameword.xml";
	CString		strServerText		= "servertext.xml";

	CString		strUIOuterCfg01		= "uioutercfg01.xml";
	CString		strUIOuterCfg02		= "uioutercfg02.xml";
	CString		strUIOuterCfg03		= "uioutercfg03.xml";

	CString		strUIInnerCfg01		= "uiinnercfg01.xml";
	CString		strUIInnerCfg02		= "uiinnercfg02.xml";
	CString		strUIInnerCfg03		= "uiinnercfg03.xml";
	CString		strUIInnerCfg04		= "uiinnercfg04.xml";
	CString		strUIInnerCfg05		= "uiinnercfg05.xml";
	CString		strUIInnerCfg06		= "uiinnercfg06.xml";

	CString		strUIExtCfg01		= "uiextcfg01.xml";	
	CString		strUIExtCfg02		= "uiextcfg02.xml";	

	CString     strUITextureList    = "uiinnerTextureList.xml"; 
	CString     strUIXmlList        = "UIXmlList.xml";

	TCHAR		HelpAddress[RANPARAM_STRING_NUM_128]			= _T("\\data\\help\\help.htm");
	TCHAR		ItemShopAddress[RANPARAM_STRING_NUM_128]		= _T("127.0.0.1");
	TCHAR		RPFDataPath[RANPARAM_STRING_NUM_128]			= _T("\\package\\data.rpf");


	//check
	BOOL bQuickSlotDelay = FALSE;	
	DWORD dwQuickSlotDelay = 200;

	DWORD hs_dll_version = 0;
	float hs_close_timer = 10.0f;

	BOOL client_hash_check_use = FALSE;
	std::string client_hash_check_game_file_1 = "";
	std::string client_hash_check_game_file_2 = "";

	BOOL client_tick_count_check = FALSE;
	float client_tick_count_timer = 30.0f;
	DWORD client_tick_count_max_diff = 60000;
	WORD client_tick_count_max_error = 3;

	std::string strMShieldDLL1 = "";
	std::string strMShieldDLL2 = "";

	//PCID
	bool bPCIDSingleLogin = false;
	bool bPCIDCheckLogin = false;

	//lobby
	BOOL bRegister = FALSE;
	BOOL bRegisterUseMD5 = FALSE;

	BOOL register_email_check	= FALSE;
	UINT register_user_min		= 4;
	UINT register_user_max		= 11;
	UINT register_pass_min		= 4;
	UINT register_pass_max		= 11;
	UINT register_pass2_min		= 4;
	UINT register_pass2_max		= 11;
	UINT register_mail_min		= 4;
	UINT register_mail_max		= 50;
	UINT register_fullname_min	= 4;
	UINT register_fullname_max	= 50;
	UINT register_sa_min		= 4;
	UINT register_sa_max		= 11;
	UINT register_captcha_min	= 4;
	UINT register_captcha_max	= 7;

	//PCID
	BOOL bRegisterPCIDCheck		= FALSE;

	EMREFERRAL_REGISTER_TYPE emRegisterReferral = EMREFERRAL_REGISTER_TYPE_NONE;

	BOOL bCharacterDelete = FALSE;
	BOOL bCharacterDeleteMD5 = FALSE;

	BOOL use_login_control = FALSE;
	BOOL use_login_control_md5 = FALSE;

	BOOL bCreateGunner = FALSE;
	BOOL bCreateAssassin = FALSE;
	BOOL bCreateMagician = FALSE;

	std::string strLoginMapFile = "log_in.wld";
	std::string strLoginMapCameraPos = "login";
	std::string strLoginMapCameraAni = "login";

	std::string strSelectCharMapFile = "cha_select.wld";
	std::string strSelectCharMapCameraPos = "select_character";
	std::string strSelectCharMapCameraAni = "select_character";

	std::string strCreateCharMapFile = "cha_select.wld";
	std::string strCreateCharMapCameraPos = "create_camera";
	std::string strCreateCharMapCameraAni = "create_camera";

	//default from LoginPage.cpp
	BOOL bLoginPageControl = FALSE;
	UINT nLoginPageUserMaxLength = 12;
	UINT nLoginPagePassMaxLength = 12;
	UINT nLoginPageRPMaxLength = 6;
	bool bLoginPageUseChannel = true;

	//feature
	TCHAR	szClientWindowTitle[RANPARAM_STRING_NUM_128] = _T("GameClient");

	BOOL bProduct = FALSE;	
	BOOL bProductIcon = FALSE;

	BOOL bStudentRecord = FALSE;

	BOOL bActivity = FALSE;

	BOOL bFeatureDailyLogin = FALSE;

	BOOL bWoeGVG = FALSE;

	BOOL bPVPWoeWar = FALSE;

	DWORD dwGlobalRankingClass = 4;

	DWORD dwNewsWindowButtons = 1;

	BOOL bFeatureBattlePass = FALSE;

	BOOL bFeatureCodex = FALSE;

	BOOL bFeatureGacha = FALSE;

	BOOL bFeatureBossViewer = FALSE;

	BOOL bOpenSkillsMax9 = FALSE;

	BOOL bFeatureDisplayCP = FALSE;

	BOOL bBypassLateRegTW = FALSE;

	BOOL bAutoInviGM = FALSE;

	BOOL bFeaturePandora = FALSE;

	BOOL bFeaturePlatBuff = FALSE;

	/* charinfoview, Juver, 2020/03/03 */
	BOOL bViewCharInfo = FALSE;
	float fViewCharInfoDelay = 5.0f;
	BOOL bViewCharAllowPrivate = TRUE;

	BOOL bUseRecoveryEXP = FALSE;

	BOOL bHideGMInfo = FALSE;

	WORD wSkillRangeCheckAdd = 50;

	BOOL bPetSkillDisplay = FALSE;

	BOOL bRegionalChatColor = FALSE;
	BYTE nRegionalChatColorR = 0x00;
	BYTE nRegionalChatColorG = 0xCE;
	BYTE nRegionalChatColorB = 0xD1;
	DWORD dwRegionalChatCost = 50000;

	BOOL bBlockDamageBack = FALSE;
	float fBlockDamageBackTimer = 1.0f;

	BOOL bClubWarVerifyDisablePushPull = FALSE;
	BOOL bChatShowStaffName = FALSE;

	BOOL bChangeSchoolReqCleanQuest = FALSE;
	BOOL bChangeSchoolReqCleanGuild = FALSE;
	BOOL bChangeSchoolReqMaxLevel = FALSE;
	BOOL bChangeSchoolWipeQuest = FALSE;

	DWORD dwItemTransferCost = 0;

	BOOL bEventTyranny = FALSE;
	BOOL bEventCaptureTheFlag = FALSE;
	BOOL bEventSchoolWars = FALSE;
	BOOL bEventClubDeathMatch = FALSE;		/* pvp club death match, Juver, 2020/11/26 */
	BOOL bEventPartyBattleGrounds = FALSE;

	BOOL bUseClubAlliance = TRUE;

	BOOL use_inven_sort = TRUE;

	float force_revive_time = 10.0f;

	BOOL allow_buff_remove = FALSE;

	BOOL use_target_information = FALSE;

	BOOL allow_skill_tab_change = FALSE;

	BOOL allow_skill_tab_change_buff_reset = FALSE;

	BOOL allow_lunchbox_remove = FALSE;

	std::string game_launch_param = "test";

	BOOL extended_buff_info = FALSE;

	BOOL link_drop_item = FALSE;

	BOOL block_locker_money_add = FALSE;
	BOOL block_locker_money_take = FALSE;
	BOOL block_guild_locker_money_add = FALSE;
	BOOL block_guild_locker_money_take = FALSE;

	BOOL damage_reduction_limit = FALSE;

	BOOL item_random_option_rebuild_use_delay = FALSE;
	float item_random_option_rebuild_delay_time = 2.0f;

	//BOOL hide_costume_use = FALSE;
	//float hide_costume_delay_time = 4.0f;

	float pet_call_delay = 4.0f;

	float fRenderSettingChangeTime = 5.0f;

	BOOL bTextClassConst = TRUE;

	BOOL bClubWarRankings = FALSE;

	float fCurrencyShopPurchaseDelayTime = 1.0f;

	BOOL bCheckWearUse = FALSE;
	BOOL bCheckWearSlot = FALSE;

	DWORD dwChatBlockMaxMinute = 43200; //defaults to 30 days limit

	BOOL bClubWarGMMapStay = FALSE;

	float fKickOutMinTime = 5.0f;		//5 seconds
	float fKickOutMaxTime = 3600.0f;	//1 hour

	/* personal lock system, Juver, 2020/06/20 */
	BOOL bUsePersonalLock = FALSE;
	float fPersonalLockCoolDownTime = 1.0f;
	WORD wPersonalLockMaxAttempt = 5;

	/* party finder, Juver, 2019/12/28 */
	BOOL bUsePartyFinder = FALSE;
	float fPartyFinderSearchDelay = 10.0f;
	float fPartyFinderJoinDelay = 5.0f;

	BOOL bGameItemShop = FALSE;

	/* megaphone user verfied, Juver, 2020/02/27 */
	BOOL bUserVerifiedLoudSpeaker = FALSE;

	/* mob item drop direct +, Juver, 2020/10/16 */
	BOOL bMobItemDropDirectPlus = FALSE;

	/* force auto revive, Juver, 2020/12/23 */
	float fForceAutoReviveTime = 300.0f;

	/* post system, Juver, 2021/02/05 */
	BOOL bUsePost = FALSE;

	float fSRC1 = 30;
	float fSRC2 = 30;

	/* skill collision check, Juver, 2021/07/10 */
	BOOL bSCCC = FALSE;
	BOOL bSCCS = FALSE;
	//BOOL bSCC = FALSE;
	//float fSCC1 = 0.0f;
	//float fSCC2 = 10.0f;
	//float fSCC3 = 0.0f;


	BOOL bShowGameStats = FALSE;

	/* tyranny AFK disconnect, Juver, 2021/08/06 */
	BOOL bTyrannyAFKDC = FALSE;
	float fTyrannyAFKDCTime = 300.0f;		//5 minutes

	/* reborn system, Juver, 2021/09/17 */
	BOOL bRBSystem = FALSE;


	BOOL bCaptureTheFlagAFKDC = FALSE;
	float fCaptureTheFlagAFKDCTime = 300.0f;		//5 minutes

	BOOL bDiscardItemQuestion = TRUE; //defaults to true 

	BOOL bMaxRvPreviewEnable = FALSE;
	BOOL bMaxRvPreviewMobGen = FALSE;
	BOOL bMaxRvPreviewRebuild = FALSE;

	//defaults to true for original behavior
	BOOL bUseHelpWindow = TRUE;

	BOOL bEnchantNotice = TRUE;
	WORD wEnchantNoticeMinLevel = 7;

	BOOL bComboWhoreNotice = TRUE;

	BOOL bShowPingFPS = FALSE;

	BOOL bShowNextTyranny = FALSE;

	BOOL bUseAutoPotion = FALSE;
	float fAutoPotionDelay = 0.1f;

	BOOL bUseAutoPilot = FALSE;
	float fAutoPilotRangeMax = 200.0f;

	BOOL bVIPSystem = FALSE;

	BOOL bAllowBTGParty = FALSE;
	BOOL bMultipleCaptchaShowTimer = FALSE;
	float fCaptchaShowTimer = 300.0f;
	float fCaptchaWaitTimer = 120.0f;

	float fDailyLoginTimeMax = 30.0f;

	bool bCTFGVG = false;
	bool bSkillResetReturnSphere = false;
	BOOL bHideMiniRank = FALSE; /* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */

	//

	// Logging Controls
	BOOL bCaptchaDcLogs = FALSE;
	BOOL bTyrannyAfkDcLogs = FALSE;
	BOOL bCharacterDcLogs = FALSE;
	BOOL bTyrannyRandomMapLogs = FALSE;
	BOOL bRandomOptionLogs = FALSE;
	BOOL bItemSortLogs = FALSE;
};
