#include "stdafx.h"
#include "./NSRProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPROFILE
{
	BOOL				bDIS_CONFT			= FALSE;
	BOOL				bDIS_TRADE			= FALSE;
	BOOL				bDIS_PARTY			= FALSE;
	BOOL				bDIS_FRIEND			= FALSE;
	BOOL				bSHOW_SIMPLEHP		= TRUE;
	BOOL				bSHOW_TIP			= TRUE;
	BOOL				bMOVABLE_ON_CHAT	= FALSE;
	BOOL				bFORCED_ATTACK		= TRUE;
	BOOL				bNAME_DISPLAY		= TRUE;
	BOOL				bNON_Rebirth		= FALSE;
	BOOL				bPrivateStats		= FALSE;

	BOOL				bCONFT_BOUND		= TRUE;
	WORD				wCONFT_RECOVER		= 0;
	float				fCONFT_HP_RATE		= 1.0f;
	float				fCONFT_TAR_HP_RATE	= 1.0f;

	DWORD				dwPARTY_GET_ITEM	= 3;
	DWORD				dwPARTY_GET_MONEY	= 3;
	bool				bPARTY_PRIVATE		= false;


	int	DefaultShortcutKey[QUICK_ALL_NUM]	= 
	{ 
		DIK_Q, DIK_W, DIK_E, DIK_A, DIK_S, DIK_D,
		DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, DIK_9, DIK_0,	
		DIK_I, DIK_C, DIK_K, DIK_P, DIK_T, DIK_G, DIK_F, DIK_M, DIK_B, DIK_R, DIK_H, DIK_L, DIK_X, DIK_Z, DIK_U, DIK_J, DIK_O 
	};

	int	QuickSlot[QUICK_SLOT_NUM] = 
	{ 
		DIK_Q, DIK_W, DIK_E, DIK_A, DIK_S, DIK_D ,
	};

	int	SkillSlot[QUICK_SKILL_NUM] = 
	{
		DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6,
		DIK_7, DIK_8, DIK_9, DIK_0,
	};

	int	MenuShortcut[QUICK_MENU_NUM]	= 
	{ 
		DIK_I, DIK_C, DIK_K, DIK_P, DIK_T, DIK_G, 
		DIK_F, DIK_M, DIK_B, DIK_R, DIK_H, DIK_L,
		DIK_X, DIK_Z ,DIK_U, DIK_J, DIK_O,
	};

	int	KeySettingToRanparam[QUICK_MENU_NUM] = 
	{ 
		SHORTCUT_INVEN, SHORTCUT_CHAR, SHORTCUT_SKILL, SHORTCUT_PARTY,
		SHORTCUT_QUEST, SHORTCUT_CLUB, SHORTCUT_FRIEND, SHORTCUT_MAP,
		SHORTCUT_CHATMACRO, SHORTCUT_ITEMBANK, SHORTCUT_ITEMSHOP, SHORTCUT_RUN,
		SHORTCUT_HELP, SHORTCUT_PET, SHORTCUT_ATTACKMODE, SHORTCUT_PKMODE, SHORTCUT_SUMMON
	};

	int	BasicMenuToRanparam[BASICMENU_NUM] = 
	{ 
		SHORTCUT_INVEN, SHORTCUT_CHAR,	SHORTCUT_SKILL, SHORTCUT_PARTY,
		SHORTCUT_QUEST, SHORTCUT_CLUB, SHORTCUT_FRIEND, SHORTCUT_MAP,
		SHORTCUT_ITEMBANK, SHORTCUT_RUN, SHORTCUT_ITEMSHOP, SHORTCUT_CHATMACRO
	};

	CString		ChatMacro[CHAT_MACRO_NUM];


	/*ERENDER_SETTING_TYPE	emRenderHead = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderUpper = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderLower = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderGloves = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderShoes = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderWeapon = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderNeckLace = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderBracelet = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderRing = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderBelt = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderEarring = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderAccessory = ERENDER_SETTING_TYPE_DEFAULT;
	ERENDER_SETTING_TYPE	emRenderDecoration = ERENDER_SETTING_TYPE_DEFAULT;

	BOOL					bApplySelf = FALSE;
	BOOL					bVehicleSimple = FALSE;*/

	EMCHARACTER_VIEW		emCharacterView = EMCHARACTER_VIEW_ALL;

	/* modern party window, Juver, 2019/12/20 */
	BOOL bPartyWindowShowBuff = TRUE;
	BOOL bPartyWindowShowInfo = TRUE;

	/* modern market window, Juver, 2020/02/10 */
	BOOL bModernMarketWindowSettingUsable = TRUE;
	BOOL bModernMarketWindowSettingBuyable = TRUE;

	/* modern rebirth dialogue, Juver, 2020/03/31 */
	BOOL bModernReviveWindowAutoRevive = FALSE;

	/* award title, Juver, 2021/02/13 */
	DWORD	dwAwardTitleSelected0 = 0;
	DWORD	dwAwardTitleSelected1 = 0;
	DWORD	dwAwardTitleSelected2 = 0;

	void Init()
	{
		bDIS_CONFT			= FALSE;
		bDIS_TRADE			= FALSE;
		bDIS_PARTY			= FALSE;
		bDIS_FRIEND			= FALSE;
		bSHOW_SIMPLEHP		= TRUE;

		bSHOW_TIP			= TRUE;	
		bMOVABLE_ON_CHAT	= FALSE;
		bFORCED_ATTACK		= TRUE;
		bNAME_DISPLAY		= TRUE;	
		bNON_Rebirth		= FALSE;
		bPrivateStats		= TRUE;

		bCONFT_BOUND		= TRUE;
		wCONFT_RECOVER		= 0;
		fCONFT_HP_RATE		= 1.0f;
		fCONFT_TAR_HP_RATE	= 1.0f;

		dwPARTY_GET_ITEM	= 3;
		dwPARTY_GET_MONEY	= 3;
		bPARTY_PRIVATE		= false;

		for( int i = 0; i < QUICK_SLOT_NUM; ++i)
		{
			QuickSlot[i]  =	DefaultShortcutKey[i];
		}

		for( int i = 0; i < QUICK_SKILL_NUM; ++i)
		{
			SkillSlot[i]  =	DefaultShortcutKey[i+QUICK_SLOT_NUM];
		}

		for( int i = 0; i < QUICK_MENU_NUM; ++i)
		{
			MenuShortcut[i] = DefaultShortcutKey[i+QUICK_SLOT_NUM+QUICK_SKILL_NUM];
		}

		for( int i = 0; i < CHAT_MACRO_NUM; ++i)
		{
			ChatMacro[i] = "";
		}

		/*emRenderHead = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderUpper = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderLower = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderGloves = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderShoes = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderWeapon = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderNeckLace = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderBracelet = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderRing = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderBelt = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderEarring = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderAccessory = ERENDER_SETTING_TYPE_DEFAULT;
		emRenderDecoration = ERENDER_SETTING_TYPE_DEFAULT;

		bApplySelf = FALSE;
		bVehicleSimple = FALSE;*/

		emCharacterView			= EMCHARACTER_VIEW_ALL;

		/* modern party window, Juver, 2019/12/20 */
		bPartyWindowShowBuff = TRUE;
		bPartyWindowShowInfo = TRUE;

		/* modern market window, Juver, 2020/02/10 */
		bModernMarketWindowSettingUsable = TRUE;
		bModernMarketWindowSettingBuyable = TRUE;

		/* modern rebirth dialogue, Juver, 2020/03/31 */
		bModernReviveWindowAutoRevive = FALSE;

		/* award title, Juver, 2021/02/13 */
		dwAwardTitleSelected0 = 0;
		dwAwardTitleSelected1 = 0;
		dwAwardTitleSelected2 = 0;
	};

};