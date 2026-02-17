#ifndef NSRPARAM_H__INCLUDED
#define NSRPARAM_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./NSRParamDef.h"
#include "./RegisterRefferalDef.h"
#include "../DxCommon/DxFontDefine.h"

#include "../../Lib_Network/LoginDefine.h"

namespace RPARAM
{
	BOOL Init ( bool bServer = false );
	BOOL Load();	

	extern EMSERVICE_TYPE	emSERVICE_TYPE;

	extern SLOGIN_ADDRESS	sLoginAddress[MAX_LOGIN_IP];
	

	extern TCHAR		HttpAddressTable[MAX_HTTP][RANPARAM_STRING_NUM_128];	
	extern TCHAR		FtpAddressTable[MAX_FTP][RANPARAM_STRING_NUM_128];

	extern CString		strNewsURL;
	extern CString		strConnectionURL;
	extern BOOL			bUsePassiveDN;

	extern SERVER_SET	China_Region[MAX_CHINA_REGION];

	extern language::LANGFLAG emLangSet;
	extern CString		strGDIFont;

	extern BOOL			bXML_USE;
	extern CString		strCountry;
	extern CString		strLauncherText;
	extern CString		strGameInText;
	extern CString		strGameExText;
	extern CString		strGameWord;
	extern CString		strServerText;

	extern CString		strUIOuterCfg01;
	extern CString		strUIOuterCfg02;
	extern CString		strUIOuterCfg03;

	extern CString		strUIInnerCfg01;
	extern CString		strUIInnerCfg02;
	extern CString		strUIInnerCfg03;
	extern CString		strUIInnerCfg04;
	extern CString		strUIInnerCfg05;
	extern CString		strUIInnerCfg06;

	extern CString		strUIExtCfg01;
	extern CString		strUIExtCfg02;

	extern CString      strUITextureList;   // New UI texture management
	extern CString      strUIXmlList;       // New UI control management

	extern TCHAR		HelpAddress[RANPARAM_STRING_NUM_128];
	extern TCHAR		ItemShopAddress[RANPARAM_STRING_NUM_128];
	extern TCHAR		RPFDataPath[RANPARAM_STRING_NUM_128];


	//check
	extern BOOL bQuickSlotDelay;
	extern DWORD dwQuickSlotDelay;

	extern DWORD hs_dll_version;
	extern float hs_close_timer;

	extern BOOL client_hash_check_use;
	extern std::string client_hash_check_game_file_1;
	extern std::string client_hash_check_game_file_2;

	extern BOOL client_tick_count_check;
	extern float client_tick_count_timer;
	extern DWORD client_tick_count_max_diff;
	extern WORD client_tick_count_max_error;

	extern std::string strMShieldDLL1;
	extern std::string strMShieldDLL2;

	//PCID
	extern bool bPCIDSingleLogin;
	extern bool bPCIDCheckLogin;
	
	//lobby
	extern BOOL bRegister;
	extern BOOL bRegisterUseMD5;

	extern BOOL register_email_check;
	extern UINT register_user_min;
	extern UINT register_user_max;
	extern UINT register_pass_min;
	extern UINT register_pass_max;
	extern UINT register_pass2_min;
	extern UINT register_pass2_max;
	extern UINT register_mail_min;
	extern UINT register_mail_max;
	extern UINT register_fullname_min;
	extern UINT register_fullname_max;
	extern UINT register_sa_min;
	extern UINT register_sa_max;
	extern UINT register_captcha_min;
	extern UINT register_captcha_max;

	//PCID
	extern BOOL bRegisterPCIDCheck;

	extern EMREFERRAL_REGISTER_TYPE emRegisterReferral;

	extern BOOL bCharacterDelete;
	extern BOOL bCharacterDeleteMD5;

	extern BOOL use_login_control;
	extern BOOL use_login_control_md5;

	extern BOOL bCreateGunner;
	extern BOOL bCreateAssassin;
	extern BOOL bCreateMagician;

	extern std::string strLoginMapFile;
	extern std::string strLoginMapCameraPos;
	extern std::string strLoginMapCameraAni;

	extern std::string strSelectCharMapFile;
	extern std::string strSelectCharMapCameraPos;
	extern std::string strSelectCharMapCameraAni;

	extern std::string strCreateCharMapFile;
	extern std::string strCreateCharMapCameraPos;
	extern std::string strCreateCharMapCameraAni;

	extern BOOL bLoginPageControl;
	extern UINT nLoginPageUserMaxLength;
	extern UINT nLoginPagePassMaxLength;
	extern UINT nLoginPageRPMaxLength;
	extern bool bLoginPageUseChannel;

	//feature
	extern TCHAR	szClientWindowTitle[RANPARAM_STRING_NUM_128];

	extern BOOL bProduct;
	extern BOOL bProductIcon;

	extern BOOL bStudentRecord;
	extern BOOL bActivity;
	extern BOOL bFeatureDailyLogin;
	extern BOOL bFeatureCodex;
	extern BOOL bFeatureGacha;
	extern BOOL bFeatureBossViewer;
	extern BOOL bOpenSkillsMax9;
	extern BOOL bFeatureDisplayCP;
	extern BOOL bBypassLateRegTW;
	extern BOOL bAutoInviGM;
	extern BOOL bFeatureBattlePass;
	extern BOOL bFeaturePandora;
	extern DWORD dwGlobalRankingClass;
	extern BOOL bFeaturePlatBuff;

	extern DWORD dwNewsWindowButtons;

	extern BOOL bWoeGVG;

	extern BOOL bPVPWoeWar;

	/* charinfoview, Juver, 2020/03/03 */
	extern BOOL bViewCharInfo;
	extern float fViewCharInfoDelay;
	extern BOOL bViewCharAllowPrivate;

	extern BOOL bUseRecoveryEXP;

	extern BOOL bHideGMInfo;

	extern WORD wSkillRangeCheckAdd;

	extern BOOL bPetSkillDisplay;

	extern BOOL bRegionalChatColor;
	extern BYTE nRegionalChatColorR;
	extern BYTE nRegionalChatColorG;
	extern BYTE nRegionalChatColorB;
	extern DWORD dwRegionalChatCost;

	extern BOOL bBlockDamageBack;
	extern float fBlockDamageBackTimer;

	extern BOOL bClubWarVerifyDisablePushPull;

	extern BOOL bChatShowStaffName;

	extern BOOL bChangeSchoolReqCleanQuest;
	extern BOOL bChangeSchoolReqCleanGuild;
	extern BOOL bChangeSchoolReqMaxLevel;
	extern BOOL bChangeSchoolWipeQuest;

	extern DWORD dwItemTransferCost;

	extern BOOL	bEventTyranny;
	extern BOOL bEventCaptureTheFlag;
	extern BOOL bEventSchoolWars;
	extern BOOL bEventClubDeathMatch;	/* pvp club death match, Juver, 2020/11/26 */
	extern BOOL bEventPartyBattleGrounds;

	extern BOOL bUseClubAlliance;

	extern BOOL use_inven_sort;

	extern float force_revive_time;

	extern BOOL allow_buff_remove;

	extern BOOL use_target_information;

	extern BOOL allow_skill_tab_change;
	extern BOOL allow_skill_tab_change_buff_reset;

	extern BOOL allow_lunchbox_remove;

	extern std::string game_launch_param;

	extern BOOL extended_buff_info;

	extern BOOL link_drop_item;

	extern BOOL block_locker_money_add;
	extern BOOL block_locker_money_take;
	extern BOOL block_guild_locker_money_add;
	extern BOOL block_guild_locker_money_take;

	extern BOOL damage_reduction_limit;

	extern BOOL item_random_option_rebuild_use_delay;
	extern float item_random_option_rebuild_delay_time;

	//extern BOOL hide_costume_use;
	//extern float hide_costume_delay_time;

	extern float pet_call_delay;

	extern float fRenderSettingChangeTime;

	extern BOOL	bTextClassConst;

	extern BOOL bClubWarRankings;
	
	extern float fCurrencyShopPurchaseDelayTime;

	extern BOOL bCheckWearUse;
	extern BOOL bCheckWearSlot;

	extern DWORD dwChatBlockMaxMinute;

	extern BOOL bClubWarGMMapStay;

	extern float fKickOutMinTime;
	extern float fKickOutMaxTime;

	/* personal lock system, Juver, 2020/06/20 */
	extern BOOL bUsePersonalLock;
	extern float fPersonalLockCoolDownTime;
	extern WORD wPersonalLockMaxAttempt;

	/* party finder, Juver, 2019/12/28 */
	extern BOOL bUsePartyFinder;
	extern float fPartyFinderSearchDelay;
	extern float fPartyFinderJoinDelay;

	extern BOOL bGameItemShop;

	/* megaphone user verfied, Juver, 2020/02/27 */
	extern BOOL bUserVerifiedLoudSpeaker;

	/* mob item drop direct +, Juver, 2020/10/16 */
	extern BOOL bMobItemDropDirectPlus;

	/* force auto revive, Juver, 2020/12/23 */
	extern float fForceAutoReviveTime;

	/* post system, Juver, 2021/02/05 */
	extern BOOL bUsePost;

	//#define SKILL_REVISION_RANGE 20
	//4-24-2021 increased to 30 to compensate client position desync
	//#define SKILL_REVISION_RANGE 30
	//5-30-2021 moved to rparam
	extern float fSRC1;
	extern float fSRC2;

	/* skill collision check, Juver, 2021/07/10 */
	extern BOOL bSCCC;
	extern BOOL bSCCS;
	//extern BOOL bSCC;
	//extern float fSCC1;
	//extern float fSCC2;
	//extern float fSCC3;

	extern BOOL bShowGameStats;

	/* tyranny AFK disconnect, Juver, 2021/08/06 */
	extern BOOL bTyrannyAFKDC;
	extern float fTyrannyAFKDCTime;

	/* reborn system, Juver, 2021/09/17 */
	extern BOOL bRBSystem;

	extern BOOL bCaptureTheFlagAFKDC;
	extern float fCaptureTheFlagAFKDCTime;

	extern BOOL bDiscardItemQuestion;

	extern BOOL bMaxRvPreviewEnable;
	extern BOOL bMaxRvPreviewMobGen;
	extern BOOL bMaxRvPreviewRebuild;

	extern BOOL bUseHelpWindow;

	extern BOOL bEnchantNotice;
	extern WORD wEnchantNoticeMinLevel;

	extern BOOL bComboWhoreNotice;

	extern BOOL bShowPingFPS;

	extern BOOL bShowNextTyranny;

	extern BOOL bUseAutoPotion;
	extern float fAutoPotionDelay;

	extern BOOL bUseAutoPilot;
	extern float fAutoPilotRangeMax;

	extern BOOL bVIPSystem;

	extern BOOL bAllowBTGParty;

	extern BOOL bMultipleCaptchaShowTimer;
	extern float fCaptchaShowTimer;
	extern float fCaptchaWaitTimer;

	extern float fDailyLoginTimeMax;

	extern bool bCTFGVG;
	extern bool bSkillResetReturnSphere;

	extern BOOL bHideMiniRank; /* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */

	// Logging Controls
	extern BOOL bCaptchaDcLogs;
	extern BOOL bTyrannyAfkDcLogs;
	extern BOOL bCharacterDcLogs;
	extern BOOL bTyrannyRandomMapLogs;
	extern BOOL bRandomOptionLogs;
	extern BOOL bItemSortLogs;
};

#endif // NSRPARAM_H__INCLUDED
