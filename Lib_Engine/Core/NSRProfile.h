#ifndef NSRPROFILE_H__INCLUDED
#define NSRPROFILE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./NSRProfileDef.h"

namespace RPROFILE
{
	void  Init();
	BOOL Load();
	BOOL Save();

	extern BOOL				bDIS_CONFT;	
	extern BOOL				bDIS_TRADE;	
	extern BOOL				bDIS_PARTY;
	extern BOOL				bDIS_FRIEND;
	extern BOOL				bSHOW_SIMPLEHP;
	extern BOOL				bSHOW_TIP;
	extern BOOL				bMOVABLE_ON_CHAT;
	extern BOOL				bFORCED_ATTACK;
	extern BOOL				bNAME_DISPLAY;
	extern BOOL				bNON_Rebirth;
	extern BOOL				bPrivateStats;

	extern BOOL				bCONFT_BOUND;
	extern WORD				wCONFT_RECOVER;	
	extern float			fCONFT_HP_RATE;	
	extern float			fCONFT_TAR_HP_RATE;	

	extern DWORD			dwPARTY_GET_ITEM;
	extern DWORD			dwPARTY_GET_MONEY;
	extern bool				bPARTY_PRIVATE;

	extern int				DefaultShortcutKey[QUICK_ALL_NUM];
	extern int				QuickSlot[QUICK_SLOT_NUM];
	extern int				SkillSlot[QUICK_SKILL_NUM];
	extern int				MenuShortcut[QUICK_MENU_NUM]; 

	extern int				KeySettingToRanparam[QUICK_MENU_NUM];
	extern int				BasicMenuToRanparam[BASICMENU_NUM];

	extern CString			ChatMacro[CHAT_MACRO_NUM];

	/*extern ERENDER_SETTING_TYPE	emRenderHead;
	extern ERENDER_SETTING_TYPE	emRenderUpper;
	extern ERENDER_SETTING_TYPE	emRenderLower;
	extern ERENDER_SETTING_TYPE	emRenderGloves;
	extern ERENDER_SETTING_TYPE	emRenderShoes;
	extern ERENDER_SETTING_TYPE	emRenderWeapon;
	extern ERENDER_SETTING_TYPE	emRenderNeckLace;
	extern ERENDER_SETTING_TYPE	emRenderBracelet;
	extern ERENDER_SETTING_TYPE	emRenderRing;
	extern ERENDER_SETTING_TYPE	emRenderBelt;
	extern ERENDER_SETTING_TYPE	emRenderEarring;
	extern ERENDER_SETTING_TYPE	emRenderAccessory;
	extern ERENDER_SETTING_TYPE	emRenderDecoration;

	extern BOOL					bApplySelf;
	extern BOOL					bVehicleSimple;*/

	extern EMCHARACTER_VIEW		emCharacterView;					

	/* modern party window, Juver, 2019/12/20 */
	extern BOOL	bPartyWindowShowBuff;
	extern BOOL	bPartyWindowShowInfo;

	/* modern market window, Juver, 2020/02/10 */
	extern BOOL bModernMarketWindowSettingUsable;
	extern BOOL bModernMarketWindowSettingBuyable;

	/* modern rebirth dialogue, Juver, 2020/03/31 */
	extern BOOL bModernReviveWindowAutoRevive;

	/* award title, Juver, 2021/02/13 */
	extern DWORD			dwAwardTitleSelected0;
	extern DWORD			dwAwardTitleSelected1;
	extern DWORD			dwAwardTitleSelected2;
};

#endif // NSRPROFILE_H__INCLUDED
