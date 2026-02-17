#include "stdafx.h"
#include "./NSRProfile.h"
#include "./NSRGameGlobal.h"
#include "./NSRPath.h"
#include "./NSRInfo.h"
#include "./NSRLog.h"

#include "../Lib_Engine/Common/gltexfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPROFILE
{
	BOOL Load()
	{
		Init();
		std::string strUSERID = RGAME_GLOBAL::GETCHARID();
		if ( strUSERID.empty() )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_RPROFILE_LOAD_CHAR_NAME ) );
			return FALSE;
		}

		std::string strOPTFILE = RPATH::getSaveProfilePath();
		strOPTFILE += strUSERID;
		strOPTFILE += _T(".gameopt");

		gltexfile cFILE;
		cFILE.reg_sep ( '\t' );

		if ( !cFILE.open(strOPTFILE.c_str(),false) )	return FALSE;


		cFILE.getflag ( "bDIS_CONFT", 1, 1, bDIS_CONFT );
		cFILE.getflag ( "bDIS_TRADE", 1, 1, bDIS_TRADE );
		cFILE.getflag ( "bDIS_PARTY", 1, 1, bDIS_PARTY );
		cFILE.getflag ( "bDIS_FRIEND", 1, 1, bDIS_FRIEND );		
		cFILE.getflag ( "bSHOW_SIMPLEHP", 1, 1, bSHOW_SIMPLEHP );
		cFILE.getflag ( "bSHOW_TIP", 1, 1, bSHOW_TIP );
		cFILE.getflag ( "bMOVABLE_ON_CHAT", 1, 1, bMOVABLE_ON_CHAT );
		cFILE.getflag ( "bFORCED_ATTACK", 1, 1, bFORCED_ATTACK );
		cFILE.getflag ( "bNAME_DISPLAY", 1, 1, bNAME_DISPLAY );
		cFILE.getflag ( "bNON_Rebirth", 1, 1, bNON_Rebirth );
		cFILE.getflag ( "bPrivateStats", 1, 1, bPrivateStats );

		cFILE.getflag ( "bCONFT_BOUND", 1, 1, bCONFT_BOUND );
		cFILE.getflag ( "wCONFT_RECOVER", 1, 1, wCONFT_RECOVER );
		cFILE.getflag ( "fCONFT_HP_RATE", 1, 1, fCONFT_HP_RATE );
		cFILE.getflag ( "fCONFT_TAR_HP_RATE", 1, 1, fCONFT_TAR_HP_RATE );

		cFILE.getflag ( "dwPARTY_GET_ITEM", 1, 1, dwPARTY_GET_ITEM );
		cFILE.getflag ( "dwPARTY_GET_MONEY", 1, 1, dwPARTY_GET_MONEY );
		cFILE.getflag ( "bPARTY_PRIVATE", 1, 1, bPARTY_PRIVATE );

		CString strTemp;

		// QUICK_POTION_SHOTCUT
		for ( int i = 0; i< QUICK_SLOT_NUM; ++i)
		{
			strTemp.Format("QUICK_SLOT%d", i);
			cFILE.getflag ( (LPCTSTR)strTemp, 1, 1, QuickSlot[i] );
		}

		// QUICK_SKILL_SHOTCUT
		for ( int i = 0; i< QUICK_SKILL_NUM; ++i)
		{
			strTemp.Format("SKILL_SLOT%d", i);
			cFILE.getflag ( (LPCTSTR)strTemp, 1, 1, SkillSlot[i] );
		}

		// MENU_SHOTCUT
		cFILE.getflag ("SHORTCUT_INVEN", 1, 1, MenuShortcut[SHORTCUT_INVEN] );
		cFILE.getflag ("SHORTCUT_CHAR", 1, 1, MenuShortcut[SHORTCUT_CHAR] );
		cFILE.getflag ("SHORTCUT_SKILL", 1, 1, MenuShortcut[SHORTCUT_SKILL] );
		cFILE.getflag ("SHORTCUT_PARTY", 1, 1, MenuShortcut[SHORTCUT_PARTY] );
		cFILE.getflag ("SHORTCUT_QUEST", 1, 1, MenuShortcut[SHORTCUT_QUEST] );
		cFILE.getflag ("SHORTCUT_CLUB", 1, 1, MenuShortcut[SHORTCUT_CLUB] );
		cFILE.getflag ("SHORTCUT_FRIEND", 1, 1, MenuShortcut[SHORTCUT_FRIEND] );
		cFILE.getflag ("SHORTCUT_MAP", 1, 1, MenuShortcut[SHORTCUT_MAP] );
		cFILE.getflag ("SHORTCUT_ITEMBANK", 1, 1, MenuShortcut[SHORTCUT_ITEMBANK] );
		cFILE.getflag ("SHORTCUT_RUN", 1, 1, MenuShortcut[SHORTCUT_RUN] );
		cFILE.getflag ("SHORTCUT_HELP", 1, 1, MenuShortcut[SHORTCUT_HELP] );
		cFILE.getflag ("SHORTCUT_PET", 1, 1, MenuShortcut[SHORTCUT_PET] );
		cFILE.getflag ("SHORTCUT_ATTACKMODE", 1, 1, MenuShortcut[SHORTCUT_ATTACKMODE] );
		cFILE.getflag ("SHORTCUT_PKMODE", 1, 1, MenuShortcut[SHORTCUT_PKMODE] );
		cFILE.getflag ("SHORTCUT_ITEMSHOP", 1, 1, MenuShortcut[SHORTCUT_ITEMSHOP] );
		cFILE.getflag ("SHORTCUT_CHATMACRO", 1, 1, MenuShortcut[SHORTCUT_CHATMACRO] );
		cFILE.getflag ("SHORTCUT_SUMMON", 1, 1, MenuShortcut[SHORTCUT_SUMMON] );

		for ( int i = 0; i< CHAT_MACRO_NUM; ++i)
		{
			strTemp.Format("CHATMACRO%d", i);
			cFILE.getflag ( (LPCTSTR)strTemp, 2, 2, ChatMacro[i] );
		}
		
		/*cFILE.getflag ( "emRenderHead", 1, 1, emRenderHead );
		cFILE.getflag ( "emRenderUpper", 1, 1, emRenderUpper );
		cFILE.getflag ( "emRenderLower", 1, 1, emRenderLower );
		cFILE.getflag ( "emRenderGloves", 1, 1, emRenderGloves );
		cFILE.getflag ( "emRenderShoes", 1, 1, emRenderShoes );
		cFILE.getflag ( "emRenderWeapon", 1, 1, emRenderWeapon );
		cFILE.getflag ( "emRenderNeckLace", 1, 1, emRenderNeckLace );
		cFILE.getflag ( "emRenderBracelet", 1, 1, emRenderBracelet );
		cFILE.getflag ( "emRenderRing", 1, 1, emRenderRing );
		cFILE.getflag ( "emRenderBelt", 1, 1, emRenderBelt );
		cFILE.getflag ( "emRenderEarring", 1, 1, emRenderEarring );
		cFILE.getflag ( "emRenderAccessory", 1, 1, emRenderAccessory );
		cFILE.getflag ( "emRenderDecoration", 1, 1, emRenderDecoration );
		cFILE.getflag ( "bVehicleSimple", 1, 1, bVehicleSimple );
		cFILE.getflag ( "bApplySelf", 1, 1, bApplySelf );*/

		cFILE.getflag ( "emCharacterView", 1, 1, emCharacterView );

		/* modern party window, Juver, 2019/12/20 */
		cFILE.getflag ( "bPartyWindowShowBuff", 1, 1, bPartyWindowShowBuff );
		cFILE.getflag ( "bPartyWindowShowInfo", 1, 1, bPartyWindowShowInfo );

		/* modern market window, Juver, 2020/02/10 */
		cFILE.getflag ( "bModernMarketWindowSettingUsable", 1, 1, bModernMarketWindowSettingUsable );
		cFILE.getflag ( "bModernMarketWindowSettingBuyable", 1, 1, bModernMarketWindowSettingBuyable );

		/* modern rebirth dialogue, Juver, 2020/03/31 */
		cFILE.getflag ( "bModernReviveWindowAutoRevive", 1, 1, bModernReviveWindowAutoRevive );

		/* award title, Juver, 2021/02/13 */
		cFILE.getflag ( "dwAwardTitleSelected0", 1, 1, dwAwardTitleSelected0 );
		cFILE.getflag ( "dwAwardTitleSelected1", 1, 1, dwAwardTitleSelected1 );
		cFILE.getflag ( "dwAwardTitleSelected2", 1, 1, dwAwardTitleSelected2 );

		return TRUE;
	};

	BOOL Save()
	{
		std::string strUSERID = RGAME_GLOBAL::GETCHARID();
		if ( strUSERID.empty() )
		{
			return FALSE;
		}

		TCHAR szPROFILE[MAX_PATH]={0};
		SHGetSpecialFolderPath( NULL, szPROFILE, CSIDL_PERSONAL, FALSE );

		std::string strOPTFILE = RPATH::getSaveProfilePath();
		strOPTFILE += strUSERID;
		strOPTFILE += _T(".gameopt");

		FILE* file=NULL;
		file = fopen ( strOPTFILE.c_str(), "wt" );
		if ( !file )	return FALSE;

		fprintf ( file, "\n" );
		fprintf ( file, "// GAME OPTION\n" );

		fprintf ( file, "bDIS_CONFT\t%d\n", bDIS_CONFT );
		fprintf ( file, "bDIS_TRADE\t%d\n", bDIS_TRADE );
		fprintf ( file, "bDIS_PARTY\t%d\n", bDIS_PARTY );
		fprintf ( file, "bDIS_FRIEND\t%d\n", bDIS_FRIEND );	
		fprintf ( file, "bSHOW_SIMPLEHP\t%d\n", bSHOW_SIMPLEHP );	
		fprintf ( file, "bSHOW_TIP\t%d\n", bSHOW_TIP );
		fprintf ( file, "bMOVABLE_ON_CHAT\t%d\n", bMOVABLE_ON_CHAT );
		fprintf ( file, "bFORCED_ATTACK\t%d\n", bFORCED_ATTACK );
		fprintf ( file, "bNAME_DISPLAY\t%d\n", bNAME_DISPLAY );
		fprintf ( file, "bNON_Rebirth\t%d\n", bNON_Rebirth );
		fprintf ( file, "bPrivateStats\t%d\n", bPrivateStats );

		fprintf ( file, "bCONFT_BOUND\t%d\n", bCONFT_BOUND );
		fprintf ( file, "wCONFT_RECOVER\t%d\n", wCONFT_RECOVER );
		fprintf ( file, "fCONFT_HP_RATE\t%1.1f\n", fCONFT_HP_RATE );
		fprintf ( file, "fCONFT_TAR_HP_RATE\t%1.1f\n", fCONFT_TAR_HP_RATE );

		fprintf ( file, "dwPARTY_GET_ITEM\t%d\n", dwPARTY_GET_ITEM );
		fprintf ( file, "dwPARTY_GET_MONEY\t%d\n", dwPARTY_GET_MONEY );
		fprintf ( file, "bPARTY_PRIVATE\t%d\n", bPARTY_PRIVATE );

		CString strTemp;
		for ( int i=0; i< QUICK_SLOT_NUM; ++i)
		{
			if(QuickSlot[i] > 0)
			{
				strTemp.Format("QUICK_SLOT%d\t%d\n", i, QuickSlot[i]);			
				fprintf ( file, strTemp);
			}
		}

		for ( int i=0; i< QUICK_SKILL_NUM; ++i)
		{
			if(SkillSlot[i] > 0)
			{
				strTemp.Format("SKILL_SLOT%d\t%d\n", i, SkillSlot[i]);
				fprintf ( file, strTemp);
			}
		}

		fprintf ( file, "SHORTCUT_INVEN\t%d\n", MenuShortcut[SHORTCUT_INVEN] );
		fprintf ( file, "SHORTCUT_CHAR\t%d\n", MenuShortcut[SHORTCUT_CHAR] );
		fprintf ( file, "SHORTCUT_SKILL\t%d\n", MenuShortcut[SHORTCUT_SKILL] );
		fprintf ( file, "SHORTCUT_PARTY\t%d\n", MenuShortcut[SHORTCUT_PARTY] );
		fprintf ( file, "SHORTCUT_QUEST\t%d\n", MenuShortcut[SHORTCUT_QUEST] );
		fprintf ( file, "SHORTCUT_CLUB\t%d\n", MenuShortcut[SHORTCUT_CLUB] );
		fprintf ( file, "SHORTCUT_FRIEND\t%d\n", MenuShortcut[SHORTCUT_FRIEND] );
		fprintf ( file, "SHORTCUT_MAP\t%d\n", MenuShortcut[SHORTCUT_MAP] );
		fprintf ( file, "SHORTCUT_ITEMBANK\t%d\n", MenuShortcut[SHORTCUT_ITEMBANK] );
		fprintf ( file, "SHORTCUT_RUN\t%d\n", MenuShortcut[SHORTCUT_RUN] );
		fprintf ( file, "SHORTCUT_HELP\t%d\n", MenuShortcut[SHORTCUT_HELP] );
		fprintf ( file, "SHORTCUT_PET\t%d\n", MenuShortcut[SHORTCUT_PET] );
		fprintf ( file, "SHORTCUT_ATTACKMODE\t%d\n", MenuShortcut[SHORTCUT_ATTACKMODE] );
		fprintf ( file, "SHORTCUT_PKMODE\t%d\n", MenuShortcut[SHORTCUT_PKMODE] );
		fprintf ( file, "SHORTCUT_ITEMSHOP\t%d\n", MenuShortcut[SHORTCUT_ITEMSHOP] );
		fprintf ( file, "SHORTCUT_CHATMACRO\t%d\n", MenuShortcut[SHORTCUT_CHATMACRO] );
		fprintf ( file, "SHORTCUT_SUMMON\t%d\n", MenuShortcut[SHORTCUT_SUMMON] );

		CString strMacro;
		for ( int i=0; i< CHAT_MACRO_NUM; ++i)
		{
			if(ChatMacro[i].GetLength() > 0)
			{
				strMacro = ChatMacro[i];
				strMacro.Replace("%", "%%");
				strTemp.Format("CHATMACRO%d\t=\t%s\n", i, strMacro);
				fprintf ( file, strTemp);
			}
		}

		/*fprintf ( file, "emRenderHead\t%d\n", emRenderHead );
		fprintf ( file, "emRenderUpper\t%d\n", emRenderUpper );
		fprintf ( file, "emRenderLower\t%d\n", emRenderLower );
		fprintf ( file, "emRenderGloves\t%d\n", emRenderGloves );
		fprintf ( file, "emRenderShoes\t%d\n", emRenderShoes );
		fprintf ( file, "emRenderWeapon\t%d\n", emRenderWeapon );
		fprintf ( file, "emRenderNeckLace\t%d\n", emRenderNeckLace );
		fprintf ( file, "emRenderBracelet\t%d\n", emRenderBracelet );
		fprintf ( file, "emRenderRing\t%d\n", emRenderRing );
		fprintf ( file, "emRenderBelt\t%d\n", emRenderBelt );
		fprintf ( file, "emRenderEarring\t%d\n", emRenderEarring );
		fprintf ( file, "emRenderAccessory\t%d\n", emRenderAccessory );
		fprintf ( file, "emRenderDecoration\t%d\n", emRenderDecoration );
		fprintf ( file, "bVehicleSimple\t%d\n", bVehicleSimple );
		fprintf ( file, "bApplySelf\t%d\n", bApplySelf );*/

		fprintf ( file, "emCharacterView\t%d\n", emCharacterView );

		/* modern party window, Juver, 2019/12/20 */
		fprintf ( file, "bPartyWindowShowBuff\t%d\n", bPartyWindowShowBuff );
		fprintf ( file, "bPartyWindowShowInfo\t%d\n", bPartyWindowShowInfo );

		/* modern market window, Juver, 2020/02/10 */
		fprintf ( file, "bModernMarketWindowSettingUsable\t%d\n", bModernMarketWindowSettingUsable );
		fprintf ( file, "bModernMarketWindowSettingBuyable\t%d\n", bModernMarketWindowSettingBuyable );

		/* modern rebirth dialogue, Juver, 2020/03/31 */
		fprintf ( file, "bModernReviveWindowAutoRevive\t%d\n", bModernReviveWindowAutoRevive );

		/* award title, Juver, 2021/02/13 */
		fprintf ( file, "dwAwardTitleSelected0\t%d\n", dwAwardTitleSelected0 );
		fprintf ( file, "dwAwardTitleSelected1\t%d\n", dwAwardTitleSelected1 );
		fprintf ( file, "dwAwardTitleSelected2\t%d\n", dwAwardTitleSelected2 );

		fclose ( file );

		return TRUE;
	};
};