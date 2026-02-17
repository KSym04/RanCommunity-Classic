#include "StdAfx.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "./UIInfoLoader.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLItemDef.h"
#include "../../Lib_Client/G-Logic/GLItem.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

/*item user requirement, Juver, 2018/05/24 */
#include "../../Lib_Client/G-Logic/GLItemUser.h"
#include "../../Lib_Client/G-Logic/UserTypeDefine.h"
#include "../../Lib_Client/G-Logic/GLCrowData.h"
#include "../../Lib_Client/G-Logic/GLCurrencyShop.h"
#include "../../Lib_Client/G-Logic/GLSetOption.h"

#include "./UIInfoLoaderDefine.h"
#include <boost/format.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NS_UIINFO_BASIC;

namespace NS_ITEMINFO_EX
{
	SITEMCUSTOM	m_sItemCustomBACK;
	BOOL		m_bShopOpenBACK;
	BOOL		m_bInMarketBACK;
	BOOL		m_bInPrivateMarketBACK;
	BOOL		m_bIsWEAR_ITEMBACK;
	WORD		m_wPosXBACK;
	WORD		m_wPosYBACK;

	/*npc shop, Juver, 2017/07/26 */
	BOOL		m_bNPCShopBACK;
	WORD		m_wNPCShopTypeBACK;

	/*item link, Juver, 2017/07/31 */
	BOOL		m_bItemLinkBACK;
	BOOL		m_bInInventoryBACK;

	/*product item, Juver, 2017/10/15 */
	BOOL		m_bInCompoundResultBACK;
	BOOL		m_bCompoundGenerateRandomOptionBACK;

	BOOL		m_bCurrencyShopBACK;
	DWORD		m_dwCurrencyShopNpcIDBACK; 
	DWORD		m_dwCurrencyShopItemIDBACK;

	DWORD		m_dwMarketChannelBACK;

	void	RESET ()
	{
		m_sItemCustomBACK.sNativeID = NATIVEID_NULL ();
		m_bShopOpenBACK = FALSE;
		m_bInMarketBACK = FALSE;
		m_bInPrivateMarketBACK = FALSE;
		m_bIsWEAR_ITEMBACK = FALSE;

		/*npc shop, Juver, 2017/07/26 */
		m_bNPCShopBACK = FALSE;
		m_wNPCShopTypeBACK = 0;

		/*item link, Juver, 2017/07/31 */
		m_bItemLinkBACK = FALSE;
		m_bInInventoryBACK = FALSE;

		/*product item, Juver, 2017/10/15 */
		m_bInCompoundResultBACK = FALSE;
		m_bCompoundGenerateRandomOptionBACK = FALSE;

		m_bCurrencyShopBACK = FALSE;
		m_dwCurrencyShopNpcIDBACK = 0;
		m_dwCurrencyShopItemIDBACK = 0;

		m_dwMarketChannelBACK = 0;
	}

	int AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
	{
		return CInnerInterface::GetInstance().InfoDisplayAddTextNoSplit ( strText, dwColor );
	}

	void AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().InfoDisplayAddTextLongestLineSplit  ( strText, dwColor );
	}

	void AddStringNoSplit ( int nIndex, CString strText, D3DCOLOR dwColor )
	{
		return CInnerInterface::GetInstance().InfoDisplayAddStringNoSplit ( nIndex, strText, dwColor );
	}

	void AddTitle ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().InfoDisplayAddTitle ( strText, dwColor );
	}

	void SetIconImage( SNATIVEID sICONINDEX, const char* szTexture )
	{
		CInnerInterface::GetInstance().InfoDisplaySetIcon( sICONINDEX, szTexture );
	}

	void RemoveAllInfo ()
	{
		CInnerInterface::GetInstance().InfoDisplayClearText ();
	}

	char*	GetNumberWithSign ( int nValue )
	{
		static	char szNumber[128];

		if ( 0 < nValue )
		{
			StringCchPrintf ( szNumber, 128, "(+%d)", nValue );
		}
		else if ( nValue < 0 )
		{
			StringCchPrintf ( szNumber, 128, "(-%d)", nValue );
		}

		return szNumber;
	}

	char*	GetNumberWithSignEx ( int nValue )
	{
		static	char szNumber[128];

		if ( 0 < nValue )
		{
			StringCchPrintf ( szNumber, 128, "+%d", nValue );
		}
		else if ( nValue < 0 )
		{
			StringCchPrintf ( szNumber, 128, "-%d", nValue );
		}

		return szNumber;
	}

	void APPEND_ITEM_GRADE ( CString& strOrigin, BYTE uGRADE )
	{
		if ( !uGRADE ) return ;

		CString strGRADE;
		strGRADE.Format ( "[+%d]", uGRADE );
		strOrigin += strGRADE;
	}

	boost::format  CreateFormat(const std::string & f_string)
	{
		boost::format fmter(f_string);
		fmter.exceptions(boost::io::all_error_bits ^ (boost::io::too_many_args_bit | boost::io::too_few_args_bit));
		return fmter;
	}

	void AddItemTurnInfo ( const SITEMCUSTOM &sItemCustom, const BOOL bInMarket, const BOOL bInPrivateMarket )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		CString strText;

		if ( pItemData->sDrugOp.wPileNum > 1 )
		{
			WORD wPileNum = pItemData->sDrugOp.wPileNum;
			WORD wTurnNum = sItemCustom.wTurnNum;				
			if ( bInPrivateMarket )
			{
				strText.Format("%s : %d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::CHARTREUSE);
				return ;
			}

			if ( bInMarket )	wTurnNum = pItemData->GETAPPLYNUM();
			strText.Format("%s : %d/%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum, wPileNum);
			AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
		}
	}

	void AddInfoItemAddonInt( int nBase, int nTotal, int nAddon, CString strFormat )
	{
		CString strText;
		strText.Format("%s :", strFormat.GetString() );
		int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format("%d", nBase );
		AddStringNoSplit( nIndex, strText, NS_UITEXTCOLOR::WHITE );

		int nDiff = std::abs(nTotal-nBase);
		if ( nDiff )
			AddStringNoSplit( nIndex, GetNumberWithSign(nDiff), NS_UITEXTCOLOR::UIINFO_OPTION1 );

		if ( nAddon )
			AddStringNoSplit( nIndex, GetNumberWithSign(nAddon), NS_UITEXTCOLOR::YELLOW );
	}

	void AddInfoItemAddonInt2( int nBase, int nTotal, int nAddon, CString strFormat )
	{
		CString strText;
		strText.Format("%s :", strFormat.GetString() );
		int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format("%d", nBase );
		AddStringNoSplit( nIndex, strText, NS_UITEXTCOLOR::WHITE );

		int nDiff = std::abs(nTotal-nBase);
		if ( nDiff )
			AddStringNoSplit( nIndex, GetNumberWithSign(nDiff), NS_UITEXTCOLOR::UIINFO_OPTION2 );

		if ( nAddon )
			AddStringNoSplit( nIndex, GetNumberWithSign(nAddon), NS_UITEXTCOLOR::YELLOW );
	}

	void AddInfoItemAddonWORD( WORD wBase, WORD wTotal, WORD wAddon, CString strFormat )
	{
		CString strText;
		strText.Format("%s :", strFormat.GetString() );
		int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE );

		strText.Format("%u", wBase );
		AddStringNoSplit( nIndex, strText, NS_UITEXTCOLOR::WHITE );

		int nDiff = std::abs(wTotal-wBase);
		if ( nDiff )
			AddStringNoSplit( nIndex, GetNumberWithSign(nDiff), NS_UITEXTCOLOR::UIINFO_OPTION1 );

		if ( wAddon )
			AddStringNoSplit( nIndex, GetNumberWithSign(wAddon), NS_UITEXTCOLOR::YELLOW );
	}

	void AddInfoItemAddonRangeInt ( int nBaseMin, int nBaseMax, int nTotalMin, int nTotalMax, int nAddon, CString strFormat )
	{
		CString strText;

		strText.Format("%s :", strFormat.GetString() );
		int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE );
		strText.Format("%d", nBaseMin );
		AddStringNoSplit( nIndex, strText, NS_UITEXTCOLOR::WHITE );

		int nDiff = std::abs(nTotalMin-nBaseMin);
		if ( nDiff )
			AddStringNoSplit( nIndex, GetNumberWithSign(nDiff), NS_UITEXTCOLOR::UIINFO_OPTION1 );

		if ( nAddon )
			AddStringNoSplit( nIndex, GetNumberWithSign(nAddon), NS_UITEXTCOLOR::YELLOW );

		AddStringNoSplit( nIndex, " ~ ", NS_UITEXTCOLOR::WHITE );

		strText.Format("%u", nBaseMax );
		AddStringNoSplit( nIndex, strText, NS_UITEXTCOLOR::WHITE );

		nDiff = std::abs(nTotalMax-nBaseMax);
		if ( nDiff )
			AddStringNoSplit( nIndex, GetNumberWithSign(nDiff), NS_UITEXTCOLOR::UIINFO_OPTION1 );

		if ( nAddon )
			AddStringNoSplit( nIndex, GetNumberWithSign(nAddon), NS_UITEXTCOLOR::YELLOW );
	}

	void AddTextRandomValue( int nType, float fValue, BOOL bPercentage )
	{
		if( (EMR_OPT_NULL < nType) && (nType < EMR_OPT_SIZE) )
		{
			CString strText;
			strText.Format( "%s :", ID2GAMEWORD("ITEM_RANDOM_OPTION", nType ) );

			if( fValue != 0.0f )
			{
				if( fValue > 0.0f) strText += _T('+');
				if( fValue < 0.0f) strText += _T('-');

				strText.AppendFormat( "%.2f", fValue );

				if( bPercentage ) 
					strText += _T('%');

				AddTextNoSplit( strText, NS_UITEXTCOLOR::UIINFO_OPTION1 );
			}
		}
	}

	void AddTextAddValue( bool& bSpace, const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT )
	{
		CString strText;
		/*official random value, Juver, 2018/06/11 */
		BOOL bADD_HP(FALSE), bADD_MP(FALSE), bADD_SP(FALSE), bADD_MA(FALSE), bADD_PA(FALSE), bADD_SA(FALSE);
		INT arrVALUE[EMADD_SIZE];
		SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );

		for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;

			if ( emTYPE != EMADD_NONE )
			{
				if ( !bSpace )
				{
					AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE);
					bSpace = true;
				}

				int nVALUE = sSUIT.sADDON[i].nVALUE;

				switch ( emTYPE )
				{
				case EMADD_HP:
					if( !bADD_HP )
					{
						arrVALUE[EMADD_HP] = sItemCustom.GETADDHP();
						bADD_HP = TRUE;
					}
					break;
				case EMADD_MP:
					if( !bADD_MP )
					{
						arrVALUE[EMADD_MP] = sItemCustom.GETADDMP();
						bADD_MP = TRUE;
					}
					break;
				case EMADD_SP:
					if( !bADD_SP )
					{
						arrVALUE[EMADD_SP] = sItemCustom.GETADDSP();
						bADD_SP = TRUE;
					}
					break;
				case EMADD_MA:
					if( !bADD_MA )
					{
						arrVALUE[EMADD_MA] = sItemCustom.GETADDMA();
						bADD_MA = TRUE;
					}
					break;

					/*official random value, Juver, 2018/06/11 */
				case EMADD_PA:
					if( !bADD_PA )
					{
						arrVALUE[EMADD_PA] = sItemCustom.GETADDPA();
						bADD_PA = TRUE;
					}
					break;

					/*official random value, Juver, 2018/06/11 */
				case EMADD_SA:
					if( !bADD_SA )
					{
						arrVALUE[EMADD_SA] = sItemCustom.GETADDSA();
						bADD_SA = TRUE;
					}
					break;

				case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
				case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
				case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
				case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
				case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
				case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
				case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
				case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
				case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
				case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
				case EMADD_MAX_CP:		arrVALUE[EMADD_MAX_CP] += nVALUE;		break;		/*item max CP, Juver, 2018/07/09 */
				};
			}
		}

		for( int i=1; i<EMADD_SIZE; ++i)
		{
			if( arrVALUE[i] != 0 )
			{
				strText.Format( "%s :%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSignEx( arrVALUE[i] ) );
				AddTextNoSplit( strText, NS_UITEXTCOLOR::UIINFO_OPTION2 );
			}
		}
	}

	void AddTextAddValueCostume( bool& bSpace, const ITEM::SSUIT & sSUIT, EMSUIT emSuitBase )
	{
		CString strText;
		INT arrVALUE[EMADD_SIZE];
		SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );

		for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;

			if ( emTYPE != EMADD_NONE )
			{
				if ( !bSpace )
				{
					AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE);
					bSpace = true;
				}

				int nVALUE = sSUIT.sADDON[i].nVALUE;

				switch ( emTYPE )
				{
				case EMADD_HP:			arrVALUE[EMADD_HP] += nVALUE;		break;
				case EMADD_MP:			arrVALUE[EMADD_MP] += nVALUE;		break;
				case EMADD_SP:			arrVALUE[EMADD_SP] += nVALUE;		break;

				case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
				case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
				case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
				case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
				case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
				case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
				case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
				case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
				case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
				case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
				case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
				case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
				case EMADD_MA:			arrVALUE[EMADD_MA] += nVALUE;			break;
				case EMADD_MAX_CP:		arrVALUE[EMADD_MAX_CP] += nVALUE;		break;		/*item max CP, Juver, 2018/07/09 */
				};
			}
		}

		for( int i=1; i<EMADD_SIZE; ++i)
		{
			if( arrVALUE[i] != 0 )
			{
				strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSign( arrVALUE[i] ) );

				//suit type check
				if ( emSuitBase == sSUIT.emSuit )
					AddTextNoSplit( strText, NS_UITEXTCOLOR::UIINFO_COSNAME );
				else
					AddTextNoSplit( strText, NS_UITEXTCOLOR::RED );
			}
		}
	}

	void AddTextSpecialFunction( bool& bSpace, const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT, const float fExpMultiple = 1.0f )
	{
		CString strText("");

		BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
		BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

		EMITEM_VAR emITEM_VAR = sSUIT.sVARIATE.emTYPE;
		EMITEM_VAR emITEM_VOL = sSUIT.sVOLUME.emTYPE;

		if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC || (fExpMultiple!=1.0f) )
		{
			if ( !bSpace )
			{
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
				bSpace = true;
			}
		}

		float fVAR_SCALE(0);
		if ( emITEM_VAR != EMVAR_NONE )
		{
			switch ( emITEM_VAR )
			{
			case EMVAR_HP:
				bVAR_HP = TRUE;
				fVAR_SCALE = sItemCustom.GETINCHP();
				break;
			case EMVAR_MP:
				bVAR_MP = TRUE;
				fVAR_SCALE = sItemCustom.GETINCMP();
				break;
			case EMVAR_SP:
				bVAR_SP = TRUE;
				fVAR_SCALE = sItemCustom.GETINCSP();
				break;
			case EMVAR_AP:
				bVAR_AP = TRUE;
				fVAR_SCALE = sItemCustom.GETINCAP();
				break;
			default:
				fVAR_SCALE = sSUIT.sVARIATE.fVariate;
				break;
			};

			fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
			strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
			if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
			AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
		}

		if ( !bVAR_HP && sItemCustom.GETINCHP() )
		{
			fVAR_SCALE = sItemCustom.GETINCHP();
			emITEM_VAR = EMVAR_HP;
			fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
			strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
			if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
			AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
		}

		if ( !bVAR_MP && sItemCustom.GETINCMP() )
		{
			fVAR_SCALE = sItemCustom.GETINCMP();
			emITEM_VAR = EMVAR_MP;
			fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
			strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
			if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
			AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
		}

		if ( !bVAR_SP && sItemCustom.GETINCSP() )
		{
			fVAR_SCALE = sItemCustom.GETINCSP();
			emITEM_VAR = EMVAR_SP;
			fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
			strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
			if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
			AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
		}

		if ( !bVAR_AP && sItemCustom.GETINCAP() )
		{
			fVAR_SCALE = sItemCustom.GETINCAP();
			emITEM_VAR = EMVAR_AP;
			fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
			strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
			if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
			AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
		}

		if( fExpMultiple != 1.0f )
		{
			float fPercent  = fExpMultiple * 100.0f - 100.0f;

			boost::format fmt = CreateFormat("%1% :+%2%%%") 
				% ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ) 
				% fPercent 
				;
			
			AddTextNoSplit( fmt.str().c_str(), NS_UITEXTCOLOR::UIINFO_OPTION2 );
		}

		if ( emITEM_VOL != EMVAR_NONE)
		{
			if ( emITEM_VOL == EMVAR_MOVE_SPEED )
			{
				float fVOLUME = sItemCustom.GETMOVESPEED();
				strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
				AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
			}
			else
			{
				float fVOLUME = sSUIT.sVOLUME.fVolume;
				strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
				AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_OPTION2);
			}
		}

	}

	void AddTextSpecialFunctionCostume( bool& bSpace, const ITEM::SSUIT & sSUIT, EMSUIT emSuitBase, bool bCostumeCombineRestrictMoveSpeed, bool bCostumeCombineRestrictAttackSpeed )
	{
		EMITEM_VAR emITEM_VAR = sSUIT.sVARIATE.emTYPE;
		EMITEM_VAR emITEM_VOL = sSUIT.sVOLUME.emTYPE;
		if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE))
		{
			if ( !bSpace )
			{
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
				bSpace = true;
			}
		}

		CString strText;

		if ( emITEM_VAR != EMVAR_NONE )
		{
			float  fVAR_SCALE = sSUIT.sVARIATE.fVariate;
			fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
			strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
			if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";

			/* costume combine map restriction, Juver, 2020/09/14 */
			if ( ( emITEM_VAR == EMVAR_MOVE_SPEED && bCostumeCombineRestrictMoveSpeed ) || 
				( emITEM_VAR == EMVAR_ATTACK_SPEED && bCostumeCombineRestrictAttackSpeed ) )
			{
				AddTextNoSplit(strText,NS_UITEXTCOLOR::RED );
			}
			else
			{
				//suit type check
				if ( emSuitBase == sSUIT.emSuit )
					AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_COSNAME );
				else
					AddTextNoSplit(strText,NS_UITEXTCOLOR::RED );
			}
			
		}

		if ( emITEM_VOL != EMVAR_NONE)
		{
			float fVOLUME = sSUIT.sVOLUME.fVolume;
			strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );

			/* costume combine map restriction, Juver, 2020/09/14 */
			if ( ( emITEM_VAR == EMVAR_MOVE_SPEED && bCostumeCombineRestrictMoveSpeed ) || 
				( emITEM_VAR == EMVAR_ATTACK_SPEED && bCostumeCombineRestrictAttackSpeed ) )
			{
				AddTextNoSplit(strText,NS_UITEXTCOLOR::RED );	
			}
			else
			{
				//suit type check
				if ( emSuitBase == sSUIT.emSuit )
					AddTextNoSplit(strText,NS_UITEXTCOLOR::UIINFO_COSNAME );	
				else
					AddTextNoSplit(strText,NS_UITEXTCOLOR::RED );	
			}
		}
	}

	void AddTextResistance( bool& bSpace, const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT )
	{
		CString strText("");

		const int nELEC   = sItemCustom.GETRESIST_ELEC();
		const int nFIRE   = sItemCustom.GETRESIST_FIRE();
		const int nICE    = sItemCustom.GETRESIST_ICE();
		const int nPOISON = sItemCustom.GETRESIST_POISON();
		const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();
		
		if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
		{
			if ( !bSpace )
			{
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
				bSpace = true;
			}
			
			std::vector<SINFO_TEXT> vecText;
			if ( nSPIRIT )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_OPTION2 ) );
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
				if ( uGRADE )
				{
					strText.Format("(+%d)", uGRADE );
					vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_PLUS ) );
				}
			}

			if ( nFIRE )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_OPTION2 ) );
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
				if ( uGRADE )
				{
					strText.Format("(+%d)", uGRADE );
					vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_PLUS ) );
				}
			}
			if ( nICE )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_OPTION2 ) );
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
				if ( uGRADE )
				{
					strText.Format("(+%d)", uGRADE );
					vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_PLUS ) );
				}
			}

			if ( nELEC )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_OPTION2 ) );
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
				if ( uGRADE )
				{
					strText.Format("(+%d)", uGRADE );
					vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_PLUS ) );
				}
			}

			if ( nPOISON )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_OPTION2 ) );
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
				if ( uGRADE )
				{
					strText.Format("(+%d)", uGRADE );
					vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_PLUS ) );
				}
			}

			int nIndex = 0;
			for ( size_t i=0; i<vecText.size(); ++i )
			{
				SINFO_TEXT& sData = vecText[i];
				if ( i == 0 )
				{
					nIndex = AddTextNoSplit( sData.strText, sData.dwTextColor );
				}else{
					AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}
			}
			
		}
	}

	void AddTextSpecialEffect( const ITEM::SSUIT & sSUIT )
	{
		EMSTATE_BLOW emBLOW = sSUIT.sBLOW.emTYPE;
		if ( emBLOW !=EMBLOW_NONE )
		{
			AddTextNoSplit ( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
		
			float fVAR1_SCALE = sSUIT.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];
			float fVAR2_SCALE = sSUIT.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];
			float fRate = sSUIT.sBLOW.fRATE;
			float fLife = sSUIT.sBLOW.fLIFE;

			BOOL bValidVar1 = ( fVAR1_SCALE != 0.0 );
			BOOL bValidVar2 = ( fVAR2_SCALE != 0.0 );
			BOOL bValidVar3 = bValidVar1 && bValidVar2;

			if ( fVAR1_SCALE < 0.0 )	fVAR1_SCALE = -fVAR1_SCALE;
			if ( fVAR2_SCALE < 0.0 )	fVAR2_SCALE = -fVAR2_SCALE;

			boost::format fmt;

			switch ( emBLOW )
			{
			case EMBLOW_NUMB:
				{
					if ( bValidVar3 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_NUMB")) % fRate % fLife % fVAR1_SCALE % fVAR2_SCALE ;
					else if ( bValidVar1 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_NUMB_1")) % fRate % fLife % fVAR1_SCALE  ;
					else if ( bValidVar2 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_NUMB_2")) % fRate % fLife % fVAR2_SCALE ;
				}break;

			case EMBLOW_STUN:
				{
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STUN")) % fRate % fLife ;
				}break;

			case EMBLOW_STONE:
				{
					if ( bValidVar3 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STONE")) % fRate % fLife % fVAR1_SCALE % fVAR2_SCALE ;
					else if ( bValidVar1 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STONE_1")) % fRate % fLife % fVAR1_SCALE  ;
					else if ( bValidVar2 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STONE_2")) % fRate % fLife % fVAR2_SCALE ;
				}break;

			case EMBLOW_BURN:
				{
					if ( bValidVar2 )
					{
						if ( fVAR2_SCALE < 0.0 )	fVAR2_SCALE = -fVAR2_SCALE;
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_BURN")) % fRate % fLife % fVAR2_SCALE ;
					}
					else if ( bValidVar1 )
					{
						if ( fVAR1_SCALE < 0.0 )	fVAR1_SCALE = -fVAR1_SCALE;
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_BURN")) % fRate % fLife % fVAR1_SCALE ;
					}
				}break;

			case EMBLOW_FROZEN:
				{
					if ( bValidVar3 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_FROZEN")) % fRate % fLife % fVAR1_SCALE % fVAR2_SCALE ;
					else if ( bValidVar1 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_FROZEN_1")) % fRate % fLife % fVAR1_SCALE  ;
					else if ( bValidVar2 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_FROZEN_2")) % fRate % fLife % fVAR2_SCALE ;
				}break;

			case EMBLOW_MAD:
				{
					if ( bValidVar3 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_MAD")) % fRate % fLife % fVAR1_SCALE % fVAR2_SCALE ;
					else if ( bValidVar1 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_MAD_1")) % fRate % fLife % fVAR1_SCALE  ;
					else if ( bValidVar2 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_MAD_2")) % fRate % fLife % fVAR2_SCALE ;
				}break;

			case EMBLOW_POISON:
				{
					if ( bValidVar2 )
					{
						if ( fVAR2_SCALE < 0.0 )	fVAR2_SCALE = -fVAR2_SCALE;
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_POISON")) % fRate % fLife % fVAR2_SCALE ;
					}
					else if ( bValidVar1 )
					{
						if ( fVAR1_SCALE < 0.0 )	fVAR1_SCALE = -fVAR1_SCALE;
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_POISON")) % fRate % fLife % fVAR1_SCALE ;
					}

				}break;

			case EMBLOW_CURSE:
				{
					if ( bValidVar2 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_CURSE")) % fRate % fLife % fVAR2_SCALE  ;
					else if ( bValidVar1 )
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_CURSE")) % fRate % fLife % fVAR1_SCALE  ;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_CURSE_1")) % fRate  ;
				}break;
			};

			CString strText = fmt.str().c_str();
			if (strText.GetLength())
				AddTextLongestLineSplit(strText, NS_UITEXTCOLOR::UIINFO_OPTION2);
		}
	}

	void AddInfoSkillItem ( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		if ( !pItemData )	return;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pItemData->sSkillBookOp.sSkill_ID );
		if ( !pSkill )		return;

		AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE );

		AddTextNoSplit( ID2GAMEWORD("ITEM_ADDITIONAL_INFO_SKILL",0), NS_UITEXTCOLOR::WHITE );

		const SKILL::SSKILLBASIC& sBASIC = pSkill->m_sBASIC;		

		if ((sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != SKILLATT_NOCARE)) || (sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != SKILLATT_NOCARE)))
		{
			strText.Format("%s :", ID2GAMEWORD("SKILL_BASIC_INFO", 9));
			int nIndex = AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

			if (sBASIC.emUSE_LITEM)
			{
				if (sBASIC.emUSE_LITEM != SKILLATT_NOCARE)
				{
					AddStringNoSplit(nIndex, COMMENT::SKILLATTACK[sBASIC.emUSE_LITEM].c_str(), NS_UITEXTCOLOR::WHITE);
					AddStringNoSplit(nIndex, " ", NS_UITEXTCOLOR::WHITE);
				}
			}

			if (sBASIC.emUSE_RITEM)
			{
				if (sBASIC.emUSE_RITEM != SKILLATT_NOCARE)
				{
					AddStringNoSplit(nIndex, COMMENT::SKILLATTACK[sBASIC.emUSE_RITEM].c_str(), NS_UITEXTCOLOR::WHITE);
				}
			}
		}

		if ( pCharacter->GETLEARNED_SKILL ( pSkill->m_sBASIC.sNATIVEID ) )
		{
			AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
			return ;
		}

		
		const WORD wLevel = 0;
		SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
		SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wLevel];

		SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
		if ( NeedSkillID != NATIVEID_NULL() )
		{
			BOOL bVALID = FALSE;
			BOOL bNeedSkillLevel = FALSE;

			CString strNeedSkillName;
			CString strNeedSkillLevel;

			PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
			strNeedSkillName.Format("%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName() );
			bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

			if ( 0 < sLVL.dwSKILL_LVL )
			{
				strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
				bNeedSkillLevel = TRUE;

				SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
				SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
				if ( iter != map.end() )
				{
					SCHARSKILL& rCharSkill = (*iter).second;
					bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
				}
			}

			int nIndex = AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColorEx( bVALID ));

			if ( bNeedSkillLevel )
			{
				AddStringNoSplit(nIndex, " / ", NS_UITEXTCOLOR::WHITE );
				AddStringNoSplit(nIndex, strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColorEx ( bVALID ) );
			}
		}

		std::vector<SINFO_TEXT> vecText;
		DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;

		if ( 0 < sLVL.dwSKP )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.dwLEVEL )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.sSTATS.wPow )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.wPow);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_sSUMSTATS.wPow >= sLVL.sSTATS.wPow );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.sSTATS.wStr )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.wStr);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_sSUMSTATS.wStr >= sLVL.sSTATS.wStr );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.sSTATS.wSpi )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.wSpi);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_sSUMSTATS.wSpi >= sLVL.sSTATS.wSpi );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.sSTATS.wDex )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.wDex);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_sSUMSTATS.wDex >= sLVL.sSTATS.wDex );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.sSTATS.wInt )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.wInt);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_sSUMSTATS.wInt >= sLVL.sSTATS.wInt );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( 0 < sLVL.sSTATS.wSta )
		{
			strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.wSta);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_sSUMSTATS.wSta >= sLVL.sSTATS.wSta );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}
	
		if ( RPARAM::bVIPSystem && pSkill->m_sLEARN.emVIPLevel != EMVIP_LEVEL_NONE )
		{
			strText.Format("%s:%s",ID2GAMEWORD( "SKILL_BASIC_INFO_EX", 0), COMMENT::CHAR_VIP[pSkill->m_sLEARN.emVIPLevel].c_str());
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( static_cast<int>(pCharacter->m_emVIPLevel) >= static_cast<int>(sLEARN.emVIPLevel) );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		if ( RPARAM::bRBSystem && pSkill->m_sLEARN.dwRebornReq != 0 )
		{
			strText.Format("%s:%d",ID2GAMEWORD("SKILL_BASIC_INFO_EX", 1), pSkill->m_sLEARN.dwRebornReq);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->m_dwReborn >= sLEARN.dwRebornReq );
			vecText.push_back( SINFO_TEXT(strText, dwTextColor ) );
		}

		int nIndex = 0;
		for ( size_t i=0; i<vecText.size(); ++i )
		{
			SINFO_TEXT& sData = vecText[i];
			if ( i == 0 )
			{
				nIndex = AddTextNoSplit( sData.strText, sData.dwTextColor );
			}else{
				AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
				AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
			}
		}

	}

	void AddInfoPetSkillItem( const SITEMCUSTOM &sItemCustom )
	{
		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLPetClient* pPetClient = GLGaeaClient::GetInstance().GetPetClient();
		if ( !pPetClient )	return;

		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		if ( !pItemData )	return;

		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			if ( pPetClient->ISLEARNED_SKILL( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}
		}
	}

	void AddInfoItemDefaultTimeLimit( SITEM* pItemData )
	{
		if ( !pItemData )	return;

		if ( pItemData->IsTIMELMT () )
		{
			CString strText;
			std::vector<SINFO_TEXT> vecText;

			strText.Format( "%s :",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 0 ) );
			int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::UIINFO_EXPIRE );

			CTimeSpan cSPAN(pItemData->sDrugOp.tTIME_LMT);
			if ( cSPAN.GetDays() > 0 )
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 1 ), cSPAN.GetDays() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			if ( cSPAN.GetHours() > 0 )
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 2 ), cSPAN.GetHours() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			if ( cSPAN.GetMinutes() > 0 )
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 3 ), cSPAN.GetMinutes() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			for ( size_t i=0; i<vecText.size(); ++i )
			{
				SINFO_TEXT& sData = vecText[i];
				if ( i == 0 )
				{
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}else{
					AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}
			}
		}
	}

	void AddInfoItemCooltime( SITEM* pItemData )
	{
		if ( !pItemData )	return;

		if ( pItemData->sBasicOp.IsCoolTime() )
		{
			CString strText;
			std::vector<SINFO_TEXT> vecText;

			strText.Format( "%s :",ID2GAMEWORD( "ITEM_BASIC_INFO", 8 ) );
			int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::UIINFO_EXPIRE );

			CTimeSpan cCoolTime( pItemData->sBasicOp.dwCoolTime );

			if ( cCoolTime.GetHours() > 0 )	
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 2 ), cCoolTime.GetHours() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			if ( cCoolTime.GetMinutes() > 0 )	
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 3 ), cCoolTime.GetMinutes() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			if ( cCoolTime.GetSeconds() > 0 )	
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 4 ), cCoolTime.GetSeconds() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			for ( size_t i=0; i<vecText.size(); ++i )
			{
				SINFO_TEXT& sData = vecText[i];
				if ( i == 0 )
				{
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}else{
					AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}
			}
		}

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if ( pCharacter && pCharacter->IsCoolTime( pItemData->sBasicOp.sNativeID ) )
		{
			CString strTime = "";
			__time64_t tCurTime =  GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
			__time64_t tCoolTime = pCharacter->GetMaxCoolTime( pItemData->sBasicOp.sNativeID );				

			CString strText;
			std::vector<SINFO_TEXT> vecText;

			strText.Format( "%s :",ID2GAMEWORD( "ITEM_BASIC_INFO", 9 ) );
			int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::UIINFO_EXPIRE );

			CTimeSpan cReTime( tCoolTime - tCurTime );

			if ( cReTime.GetHours() > 0 )	
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 2 ), cReTime.GetHours() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			if ( cReTime.GetMinutes() > 0 )	
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 3 ), cReTime.GetMinutes() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			if ( cReTime.GetSeconds() > 0 )	
			{
				strText.Format( "%s %d",ID2GAMEWORD( "ITEM_ADDITIONAL_INFO_ETC", 4 ), cReTime.GetSeconds() );
				vecText.push_back( SINFO_TEXT(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE) );
			}

			for ( size_t i=0; i<vecText.size(); ++i )
			{
				SINFO_TEXT& sData = vecText[i];
				if ( i == 0 )
				{
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}else{
					AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}
			}
		}
	}

	void AddInfoDesc( bool& bSpace, SITEM* pItemData )
	{
		if ( !pItemData )	return;

		const TCHAR * pszComment(NULL);

		if ( pItemData->sBasicOp.emItemType == ITEM_SKILL || pItemData->sBasicOp.emItemType == ITEM_PET_SKILL )
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pItemData->sSkillBookOp.sSkill_ID );
			if ( pSkill )	
				pszComment = pSkill->GetDesc();
		}

		if ( !pszComment )
			pszComment = pItemData->GetComment();

		if ( pszComment && strlen(pszComment) )
		{
			if ( !bSpace )
			{
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
				bSpace = true;
			}

			AddTextLongestLineSplit( pszComment, NS_UITEXTCOLOR::WHITE );
		}
	}

	void AddInfoFlags( bool& bSpace, const SITEMCUSTOM &sItemCustom , SITEM* pItemData )
	{
		if ( !pItemData )	return;

		if ( !bSpace )
		{
			AddTextNoSplit( " ", NS_UITEXTCOLOR::WHITE );
			bSpace = true;
		}

		CString strText;
		std::vector<SINFO_TEXT> vecText;

		bool bGMGenItem = sItemCustom.IsGM_GENITEM();
		if ( bGMGenItem )
		{
			vecText.push_back( SINFO_TEXT( ID2GAMEINTEXT( "ITEM_INFO_GM_GEN"), NS_UITEXTCOLOR::GOLD ) );
		}


		DWORD dwFlags = pItemData->sBasicOp.dwFlags;
		CString strFlag; 
		
		bool bSale = pItemData->sBasicOp.IsSALE();
		bool bExchange = pItemData->sBasicOp.IsEXCHANGE();
		bool bLocker = pItemData->sBasicOp.IsLocker();
		bool bClubLocker = pItemData->sBasicOp.IsClubLocker();
		bool bThrow = pItemData->sBasicOp.IsTHROW();
		bool bCostume = pItemData->sBasicOp.IsDISGUISE();
		bool bTrashCan = pItemData->sBasicOp.IsGarbage();
		BOOL bRVCard = pItemData->sBasicOp.bUseRVCard;
		
		SITEM* pitem_costume = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
		if( pitem_costume )
		{
			if ( !pitem_costume->sBasicOp.IsSALE() )		bSale = false;
			if ( !pitem_costume->sBasicOp.IsEXCHANGE() )	bExchange = false;
			if ( !pitem_costume->sBasicOp.IsLocker() )		bLocker = false;
			if ( !pitem_costume->sBasicOp.IsClubLocker() )	bClubLocker = false;
			if ( !pitem_costume->sBasicOp.IsTHROW() )		bThrow = false;
		}

		if ( bGMGenItem )
		{
			bSale			= false;
			bExchange		= false;
			bThrow			= false;
		}
		
		vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_TRADE", 0 ), bSale? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_TRADE", 1 ), bThrow? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_TRADE", 3 ), bExchange? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_TRADE", 2 ), bLocker? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_TRADE_EX", 0 ), bClubLocker? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_TRADE", 4 ), bTrashCan? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		
		if( pItemData->sBasicOp.emItemType == ITEM_SUIT )
		{
			CString strFlag;
			/*use rebuild flag, Juver, 2019/02/18 */
			bool bUseRebuild = ( strlen( pItemData->sRandomOpt.szNAME ) > 3 && pItemData->sBasicOp.bUseRebuild );
			if ( bUseRebuild )
			{
				/* Item Reform use limit, Juver, 2021/07/23 */
				if ( pItemData->sBasicOp.GetMaxItemReform() == ITEM_REFORM_INFINITE )
				{
					strFlag.Format( "%s(%s)", ID2GAMEWORD ( "ITEM_INFO_EXTRA_INFO", 0 ), ID2GAMEWORD ( "ITEM_INFO_EXTRA_INFO", 1 ) );
					vecText.push_back( SINFO_TEXT( strFlag.GetString(), NS_UITEXTCOLOR::WHITE ) );
				}
				else
				{
					strFlag.Format( "%s(%d/%d)", ID2GAMEWORD ( "ITEM_INFO_EXTRA_INFO", 0 ), sItemCustom.wReformUseCount, pItemData->sBasicOp.GetMaxItemReform() );
					vecText.push_back( SINFO_TEXT( strFlag.GetString(), sItemCustom.wReformUseCount >= pItemData->sBasicOp.GetMaxItemReform() ? NS_UITEXTCOLOR::RED:NS_UITEXTCOLOR::BRIGHTGREEN ) );
				}

				
			}
			else
			{
				vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_EXTRA_INFO", 0 ), NS_UITEXTCOLOR::DISABLE ) );
			}
			
			vecText.push_back( SINFO_TEXT( ID2GAMEWORD ( "ITEM_INFO_EXTRA_INFO", 2 ), bRVCard? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DISABLE ) );
		}
		
		int nIndex = 0;
		for ( size_t i=0; i<vecText.size(); ++i )
		{
			SINFO_TEXT& sData = vecText[i];
			if ( nIndex == 0 )
			{
				nIndex = AddTextNoSplit( sData.strText, sData.dwTextColor );
			}else{
				AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
				AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
			}

			if( i && i % 4 == 0 )
			{
				AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
				nIndex = 0;
			}
		}

	}

	/* set item option, Juver, 2021/09/02 */
	void AddInfoSetOption( SITEM* pItemData, GLCharacter* pCharacter, const SITEMCUSTOM& sItemCustom )
	{
		if ( sItemCustom.wSetItemID != SET_OPTION_NULL )
		{
			SSET_OPTION_DATA* pSetOptionData = GLSetOptionData::GetInstance().GetData( sItemCustom.wSetItemID );
			if ( pSetOptionData )
			{
				//add space
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );

				//add title
				AddTextNoSplit ( pSetOptionData->strName.c_str(), NS_UITEXTCOLOR::HOTPINK );

				//set status
				std::vector<NS_UIINFO_BASIC::SINFO_TEXT> vecText;
				vecText.reserve( EMSET_OPTION_SLOT_SIZE );
				for ( int nSetSlot = 0; nSetSlot < EMSET_OPTION_SLOT_SIZE; ++nSetSlot )
				{
					if ( pSetOptionData->bUseParts[nSetSlot] )
					{
						bool bUseState = pCharacter->SetOptionIsSuit( pItemData->sSuitOp.emSuit );

						NS_UIINFO_BASIC::SINFO_TEXT sInfo;
						sInfo.strText = COMMENT::SET_OPTION_SLOT[nSetSlot].c_str();
						sInfo.dwTextColor = bUseState? NS_UITEXTCOLOR::WHITE:NS_UITEXTCOLOR::RED;
						vecText.push_back( sInfo );
					}
				}

				int nIndex = 0;
				for ( size_t i=0; i<vecText.size(); ++i )
				{
					NS_UIINFO_BASIC::SINFO_TEXT& sData = vecText[i];
					if ( nIndex == 0 )
					{
						nIndex = AddTextNoSplit( sData.strText, sData.dwTextColor );
					}else{
						AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
						AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
					}

					if( i && i % 4 == 0 )
					{
						AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
						nIndex = 0;
					}
				}

				//option status
				for ( int nSetNum = 0; nSetNum < SSET_OPTION_DATA::VAR_STAGES; ++ nSetNum )
				{
					EMSET_OPTION_VAR emVar = pSetOptionData->emOptionType[nSetNum];
					float fVar = pSetOptionData->fOptionVar[nSetNum];

					if ( emVar > EMSET_OPTION_VAR_NONE && emVar < EMSET_OPTION_VAR_SIZE )
					{
						//index - value type string, value
						bool bPercent = COMMENT::bSET_OPTION_VAR_PERCENT[emVar];

						CString strTemp;
						if ( bPercent )
						{
							strTemp.Format( "%s %s +%g%%", ID2GAMEWORD( "SET_OPTION_STAGES", nSetNum ), 
								COMMENT::SET_OPTION_VAR[emVar].c_str(), fVar * COMMENT::fSET_OPTION_VAR_MULTIPLIER[emVar] );
						}
						else
						{
							strTemp.Format( "%s %s +%g", ID2GAMEWORD( "SET_OPTION_STAGES", nSetNum ), 
								COMMENT::SET_OPTION_VAR[emVar].c_str(), fVar * COMMENT::fSET_OPTION_VAR_MULTIPLIER[emVar] );
						}

						AddTextNoSplit ( strTemp.GetString(), NS_UITEXTCOLOR::HOTPINK );
					}
				}

				//add space
				//AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
			}
			


		}
	}

	void AddInfoRecall(SITEM* pItemData )
	{
		if (!pItemData)	return;
		if (pItemData->sBasicOp.emItemType == ITEM_PET_SKILL)	return;

		CString strText("");

		if (pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_REGEN)
		{
			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

			CString strName(GLGaeaClient::GetInstance().GetMapName(pCharacter->m_sStartMapID));
			if (strName == _T("(null)")) strName.Empty();

			strText.Format("%s :%s", ID2GAMEWORD("ITEM_CALL_MAP", 0), strName);
			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

			SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode(pCharacter->m_sStartMapID);
			if (pMapNode)
			{
				GLLevelFile cLevelFile;
				BOOL bOk = cLevelFile.LoadFile(pMapNode->strFile.c_str(), TRUE, NULL);
				if (bOk)
				{
					D3DXVECTOR3 vStartPos;
					PDXLANDGATE pGate = cLevelFile.GetLandGateMan().FindLandGate(pCharacter->m_dwStartGate);
					if (pGate)
					{
						vStartPos = pGate->GetGenPos(DxLandGate::GEN_RENDUM);
					}
					else
					{
						pGate = cLevelFile.GetLandGateMan().FindLandGate(DWORD(0));
						if (pGate) vStartPos = pGate->GetGenPos(DxLandGate::GEN_RENDUM);
					}

					int nPosX(0), nPosY(0);
					cLevelFile.GetMapAxisInfo().Convert2MapPos(vStartPos.x, vStartPos.z, nPosX, nPosY);

					strText.Format("%s :%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1), nPosX, nPosY);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
				}
			}
		}
		else if (pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_LASTCALL)
		{
			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

			if (pCharacter->m_sLastCallMapID.IsValidNativeID())
			{
				CString strName(GLGaeaClient::GetInstance().GetMapName(pCharacter->m_sLastCallMapID));
				if (strName == _T("(null)")) strName.Empty();

				strText.Format("%s :%s", ID2GAMEWORD("ITEM_CALL_MAP", 0), strName);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

				SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode(pCharacter->m_sLastCallMapID);
				if (pMapNode)
				{
					GLLevelFile cLevelFile;
					BOOL bOk = cLevelFile.LoadFile(pMapNode->strFile.c_str(), TRUE, NULL);
					if (bOk)
					{
						int nPosX(0), nPosY(0);
						cLevelFile.GetMapAxisInfo().Convert2MapPos(pCharacter->m_vLastCallPos.x, pCharacter->m_vLastCallPos.z, nPosX, nPosY);

						strText.Format("%s :%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1), nPosX, nPosY);
						AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
					}
				}
			}
			else
			{
				AddTextNoSplit(ID2GAMEWORD("ITEM_CALL_MAP", 2), NS_UITEXTCOLOR::RED);
			}
		}
		else if (pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_TELEPORT)
		{
			CString strName(GLGaeaClient::GetInstance().GetMapName(pItemData->sBasicOp.sSubID));
			if (strName == _T("(null)")) strName.Empty();

			strText.Format("%s :%s", ID2GAMEWORD("ITEM_TELEPORT_MAP", 0), strName);
			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

			strText.Format("%s :%d,%d", ID2GAMEWORD("ITEM_TELEPORT_MAP", 1), pItemData->sBasicOp.wPosX, pItemData->sBasicOp.wPosY);
			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
		}
	}

	void AddInfoItemExpiration(SITEM* pItemData, const SITEMCUSTOM sItemCustom)
	{
		if (!pItemData)	return;

		if (pItemData->IsTIMELMT())
		{
			CTime cTime(sItemCustom.tBORNTIME);
			if (cTime.GetYear() != 1970)
			{
				CTimeSpan sLMT(pItemData->sDrugOp.tTIME_LMT);
				cTime += sLMT;

				CString strExpireDate;
				strExpireDate = CInnerInterface::GetInstance().MakeString(ID2GAMEWORD("ITEM_EXPIRE_DATE"),
					(cTime.GetYear() % 2000), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute());

				CString strText;
				strText.Format("%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 4), strExpireDate);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE);
			}
		}
	}

	void AddInfoItemExpirationCostume(const SITEMCUSTOM sItemCustom)
	{
		if (sItemCustom.tDISGUISE != 0)
		{
			CTime cTime(sItemCustom.tDISGUISE);
			if (cTime.GetYear() != 1970)
			{
				CString strExpireDate;
				strExpireDate = CInnerInterface::GetInstance().MakeString(ID2GAMEWORD("ITEM_EXPIRE_DATE"),
					(cTime.GetYear() % 2000), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute());

				CString strText;
				strText.Format("%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 7), strExpireDate);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::UIINFO_COSNAME);
			}
		}
	}

	void LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom_base )
	{
		if ( m_sItemCustomBACK == sItemCustom_base ) return ;

		m_sItemCustomBACK = sItemCustom_base;

		RemoveAllInfo();

		/*item wrapper, Juver, 2018/01/12 */
		SITEMCUSTOM sItemCustom = sItemCustom_base;

		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		if ( !pItemData ) return ;

		/*item wrapper, Juver, 2018/01/12 */
		BOOL bWrapped = FALSE;
		std::string strorigname = "";
		if ( pItemData && pItemData->sBasicOp.emItemType == ITEM_WRAPPER_BOX && sItemCustom.nidDISGUISE != NATIVEID_NULL() )
		{
			strorigname = pItemData->GetName();
			bWrapped = TRUE;
		}

		if ( bWrapped )
		{
			pItemData = NULL;
			pItemData = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
			sItemCustom.sNativeID = sItemCustom.nidDISGUISE;
			sItemCustom.nidDISGUISE = NATIVEID_NULL();
			sItemCustom.tDISGUISE = 0;
		}

		if ( !pItemData )
		{
			bWrapped = FALSE;
			pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		}

		SetIconImage( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );

		//name
		CString strBLANKSPACE("       ");
		CString strName("");
		CString strText("");
		
		/*item wrapper, Juver, 2018/01/12 */
		if ( bWrapped && strorigname.size() )
			strName.Format ( "%s %s", strorigname.c_str(), pItemData->GetName() );
		else
			strName.Format ( "%s", pItemData->GetName() );

		BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_NONE);
		if ( uGRADE > 0 )
			strText.Format ( "%s+%d %s", strBLANKSPACE.GetString(), uGRADE, strName.GetString() );
		else
			strText.Format ( "%s%s", strBLANKSPACE.GetString(), strName.GetString() );

		const int nNameMinSize = 50;
		if ( strText.GetLength() < nNameMinSize )
		{
			std::string strBlank( nNameMinSize - strText.GetLength (), ' ' );
			strText.Format( "%s%s",strText.GetString(), strBlank.c_str() );
		}

		DWORD dwLevel = pItemData->sBasicOp.emLevel;
		AddTitle ( strText.GetString(), COMMENT::ITEMCOLOR[dwLevel] );
	}

	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	void LOAD ( const SITEMCUSTOM &sItemCustom_base, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID,
		const BOOL bNPCShop /*= FALSE*/, const WORD wNPCShopType /*= 0*/, const BOOL bItemLink /*= FALSE*/, const BOOL bInInventory /*= FALSE*/, BOOL bInCompoundResult /*= FALSE*/, BOOL bCompoundGenerateRandomOption /*= FALSE*/,
		BOOL bCurrencyShop /*= FALSE*/, DWORD dwCurrencyShopNpcID /*= 0*/, DWORD dwCurrencyShopItemID /*= 0*/, DWORD dwMarketChannel /*= 0*/ )  
	{	
		/*npc shop, Juver, 2017/07/26 */
		/*item link, Juver, 2017/07/31 */
		/*product item, Juver, 2017/10/15 */
		if ( m_sItemCustomBACK == sItemCustom_base && m_bShopOpenBACK == bShopOpen
			&& m_bInMarketBACK == bInMarket && m_bInPrivateMarketBACK == bInPrivateMarket
			&& m_bIsWEAR_ITEMBACK == bIsWEAR_ITEM && m_wPosXBACK == wPosX && m_wPosYBACK == wPosY
			&& m_bNPCShopBACK == bNPCShop && m_wNPCShopTypeBACK == wNPCShopType
			&& m_bItemLinkBACK == bItemLink && m_bInInventoryBACK == bInInventory
			&& m_bInCompoundResultBACK == bInCompoundResult &&  m_bCompoundGenerateRandomOptionBACK == bCompoundGenerateRandomOption 
			&& m_bCurrencyShopBACK == bCurrencyShop && m_dwCurrencyShopItemIDBACK == dwCurrencyShopItemID && m_dwCurrencyShopNpcIDBACK == dwCurrencyShopNpcID 
			&& m_dwMarketChannelBACK == dwMarketChannel )	
			return ;

		m_sItemCustomBACK = sItemCustom_base;
		m_bShopOpenBACK = bShopOpen;
		m_bInMarketBACK = bInMarket;
		m_bInPrivateMarketBACK = bInPrivateMarket;
		m_bIsWEAR_ITEMBACK = bIsWEAR_ITEM;
		m_wPosXBACK = wPosX;
		m_wPosYBACK = wPosY;

		/*npc shop, Juver, 2017/07/26 */
		m_bNPCShopBACK = bNPCShop; 
		m_wNPCShopTypeBACK = wNPCShopType;

		/*item link, Juver, 2017/07/31 */
		m_bItemLinkBACK = bItemLink;
		m_bInInventoryBACK = bInInventory;

		/*product item, Juver, 2017/10/15 */
		m_bInCompoundResultBACK = bInCompoundResult;
		m_bCompoundGenerateRandomOptionBACK = bCompoundGenerateRandomOption;

		m_bCurrencyShopBACK = bCurrencyShop;
		m_dwCurrencyShopItemIDBACK = dwCurrencyShopItemID;
		m_dwCurrencyShopNpcIDBACK = dwCurrencyShopNpcID;

		m_dwMarketChannelBACK = dwMarketChannel;

		RemoveAllInfo();

		/*item wrapper, Juver, 2018/01/12 */
		SITEMCUSTOM sItemCustom = sItemCustom_base;


		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		/*item wrapper, Juver, 2018/01/12 */
		BOOL bWrapped = FALSE;
		std::string strorigname = "";
		if ( pItemData && pItemData->sBasicOp.emItemType == ITEM_WRAPPER_BOX && sItemCustom.nidDISGUISE != NATIVEID_NULL() )
		{
			strorigname = pItemData->GetName();
			bWrapped = TRUE;
		}

		if ( bWrapped )
		{
			pItemData = NULL;
			pItemData = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
			sItemCustom.sNativeID = sItemCustom.nidDISGUISE;
			sItemCustom.nidDISGUISE = NATIVEID_NULL();
			sItemCustom.tDISGUISE = 0;
		}

		if ( !pItemData )
		{
			bWrapped = FALSE;
			pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		}

		SetIconImage( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );

		//name
		{
			CString strBLANKSPACE("       ");
			CString strName("");
			CString strText("");

			/*item wrapper, Juver, 2018/01/12 */
			if ( bWrapped && strorigname.size() )
				strName.Format ( "%s %s", strorigname.c_str(), pItemData->GetName() );
			else
				strName.Format ( "%s", pItemData->GetName() );

			BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_NONE);
			if ( uGRADE > 0 )
				strText.Format ( "%s+%d %s", strBLANKSPACE.GetString(), uGRADE, strName.GetString() );
			else
				strText.Format ( "%s%s", strBLANKSPACE.GetString(), strName.GetString() );

			const int nNameMinSize = 50;
			if ( strText.GetLength() < nNameMinSize )
			{
				std::string strBlank( nNameMinSize - strText.GetLength (), ' ' );
				strText.Format( "%s%s",strText.GetString(), strBlank.c_str() );
			}

			DWORD dwLevel = pItemData->sBasicOp.emLevel;
			AddTitle ( strText.GetString(), COMMENT::ITEMCOLOR[dwLevel] );
		}
		
		//move 1 line
		{	
			AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
		}

		//requirements
		{
			std::vector<SINFO_TEXT> vecText;
			CString strText;
			CString strBLANKSPACE("         ");

			DWORD dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) );

			if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
			{
				CString strText;
				if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
					vecText.push_back( SINFO_TEXT( GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str(), dwTextColor ) );

				if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
					vecText.push_back( SINFO_TEXT( GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str(), dwTextColor ) );

				if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
					vecText.push_back( SINFO_TEXT( GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str(), dwTextColor ) );
			}
			else
			{
				vecText.push_back( SINFO_TEXT( ID2GAMEWORD("ITEM_ALL_SCHOOL", 0 ), dwTextColor ) );
			}

			DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( dwReqCharClass & pCharacter->m_emClass );

			if( dwReqCharClass )
			{
				if ( dwReqCharClass==GLCC_ALL_8CLASS)
				{
					vecText.push_back( SINFO_TEXT( ID2GAMEWORD("ITEM_ALL_CLASS", 0 ), dwTextColor ) );
				}
				else if ( dwReqCharClass==GLCC_MAN_8CLASS)
				{
					vecText.push_back( SINFO_TEXT( ID2GAMEWORD("ITEM_ALL_MALE_CLASS", 0 ), dwTextColor ) );
				}
				else if ( dwReqCharClass==GLCC_WOMAN_8CLASS)
				{
					vecText.push_back( SINFO_TEXT( ID2GAMEWORD("ITEM_ALL_FEMALE_CLASS", 0 ), dwTextColor ) );
				}
				else
				{
					if ( ( dwReqCharClass & GLCC_BRAWLER_M ) && ( dwReqCharClass & GLCC_BRAWLER_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_BRAWLER].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_BRAWLER_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_BRAWLER_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_BRAWLER_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_SWORDSMAN_M ) && ( dwReqCharClass & GLCC_SWORDSMAN_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_SWORDSMAN].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_SWORDSMAN_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_SWORDSMAN_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_SWORDSMAN_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_ARCHER_M ) && ( dwReqCharClass & GLCC_ARCHER_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_ARCHER].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_ARCHER_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_ARCHER_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_SHAMAN_M ) && ( dwReqCharClass & GLCC_SHAMAN_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_SHAMAN].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_SHAMAN_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_SHAMAN_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAMAN_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_EXTREME_M ) && ( dwReqCharClass & GLCC_EXTREME_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_EXTREME].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_EXTREME_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_EXTREME_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_GUNNER_M ) && ( dwReqCharClass & GLCC_GUNNER_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_GUNNER].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_GUNNER_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_GUNNER_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_GUNNER_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_ASSASSIN_M ) && ( dwReqCharClass & GLCC_ASSASSIN_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_ASSASSIN].c_str(), dwTextColor ) );
					}else{
						if ( dwReqCharClass & GLCC_ASSASSIN_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_ASSASSIN_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str(), dwTextColor ) );
					}

					if ( ( dwReqCharClass & GLCC_TRICKER_M ) && ( dwReqCharClass & GLCC_TRICKER_W ) )
					{
						vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS2[GLCI2_TRICKER].c_str(), dwTextColor ) );
					}else{

						if ( dwReqCharClass & GLCC_TRICKER_M )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str(), dwTextColor ) );

						if ( dwReqCharClass & GLCC_TRICKER_W )
							vecText.push_back( SINFO_TEXT( COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str(), dwTextColor ) );
					}
				}
			}

			int nIndex = 0;
			for ( size_t i=0; i<vecText.size(); ++i )
			{
				SINFO_TEXT& sData = vecText[i];
				if ( i == 0 )
				{
					nIndex = AddTextNoSplit( strBLANKSPACE, sData.dwTextColor );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}else{
					AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}
			}

			vecText.clear();
			strText = "";
			nIndex = 0;

			if ( pItemData->sBasicOp.wReqLevelDW && pItemData->sBasicOp.wReqLevelUP )
			{
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( (pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel) && 
					(pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel) ); 

				strText.Format( "%s:%d~%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW, pItemData->sBasicOp.wReqLevelUP );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}
			else
			{
				if ( pItemData->sBasicOp.wReqLevelDW )
				{
					dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel ); 
					strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
					vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
				}
			}

			SCHARSTATS& rItemStats = pItemData->sBasicOp.sReqStats;
			SCHARSTATS& rCharStats = pCharacter->m_sSUMSTATS;

			if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
			{
				strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pCharacter->GETBRIGHT() == pItemData->sBasicOp.emReqBright );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( pItemData->sBasicOp.wReqPA )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( pItemData->sBasicOp.wReqSA )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( rItemStats.wPow )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.wPow );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( rItemStats.wPow <= rCharStats.wPow );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( rItemStats.wStr )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.wStr );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( rItemStats.wStr <= rCharStats.wStr );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( rItemStats.wSpi)
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.wSpi );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( rItemStats.wSpi <= rCharStats.wSpi );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( rItemStats.wDex )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.wDex );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( rItemStats.wDex <= rCharStats.wDex );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( rItemStats.wInt  )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.wInt );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( rItemStats.wInt <= rCharStats.wInt );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( rItemStats.wSta )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.wSta );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( rItemStats.wSta <= rCharStats.wSta );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			/*UserNum ItemReq, Juver, 2017/06/27 */
			if ( pItemData->sBasicOp.dwReqUserNum != 0 )
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 0 ), pItemData->sBasicOp.dwReqUserNum );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.dwReqUserNum == pCharacter->GetUserID() );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			/*contribution point, Juver, 2017/08/23 */
			if( pItemData->sBasicOp.dwReqContributionPoint )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 4), pItemData->sBasicOp.dwReqContributionPoint );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.dwReqContributionPoint <= GLGaeaClient::GetInstance().GetCharacterLogic().m_dwContributionPoint );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}
			
			/*activity point, Juver, 2017/08/23 */
			if( pItemData->sBasicOp.dwReqActivityPoint )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 3), pItemData->sBasicOp.dwReqActivityPoint );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.dwReqActivityPoint <= GLGaeaClient::GetInstance().GetCharacterLogic().m_dwActivityPoint );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			/* play time system, Juver, 2021/01/27 */
			if( pItemData->sBasicOp.llPlayTimeReq )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 1), pItemData->sBasicOp.llPlayTimeReq );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.llPlayTimeReq <= GLGaeaClient::GetInstance().GetCharacterLogic().m_llPlayTime );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( RPARAM::bVIPSystem && pItemData->sBasicOp.emVIPLevel != EMVIP_LEVEL_NONE )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 2), pItemData->sBasicOp.emVIPLevel );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( 
					static_cast<int>( pItemData->sBasicOp.emVIPLevel ) <= static_cast<int>( GLGaeaClient::GetInstance().GetCharacterLogic().m_emVIPLevel ) );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			if ( RPARAM::bRBSystem && pItemData->sBasicOp.dwRebornReq != 0 )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 3), pItemData->sBasicOp.dwRebornReq );
				dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx ( pItemData->sBasicOp.dwRebornReq <= GLGaeaClient::GetInstance().GetCharacterLogic().m_dwReborn );
				vecText.push_back( SINFO_TEXT( strText, dwTextColor ) );
			}

			for ( size_t i=0; i<vecText.size(); ++i )
			{
				SINFO_TEXT& sData = vecText[i];
				if ( i == 0 )
				{
					nIndex = AddTextNoSplit( strBLANKSPACE, sData.dwTextColor );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}else{
					AddStringNoSplit( nIndex, "/", NS_UITEXTCOLOR::WHITE );
					AddStringNoSplit( nIndex, sData.strText, sData.dwTextColor );
				}
			}


		}

		if ( bIsWEAR_ITEM )
		{
			AddTextNoSplit( ID2GAMEWORD ("WEAR_ITEM" ), NS_UITEXTCOLOR::RED );
		}

		//item type and costume info
		{	
			SCHARSTATS& rItemStats = pItemData->sBasicOp.sReqStats;
			SCHARSTATS& rCharStats = pCharacter->m_sSUMSTATS;
			EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;
			DWORD dwLevel = pItemData->sBasicOp.emLevel;

			CString strItemID,strGen;
			strItemID.Format("ID:[%03d_%03d]", sItemCustom.sNativeID.wMainID, sItemCustom.sNativeID.wSubID  );
			strGen.Format("GenID:[%d] ", pItemData->sGenerateOp.dwSpecID  );

			DWORD dwUSERLVL = GLGaeaClient::GetInstance().GetCharacter()->m_dwUserLvl;
			if ( dwUSERLVL > NSUSER_TYPE::USER_TYPE_GM3  )
			{
				AddTextNoSplit ( strItemID, NS_UITEXTCOLOR::PRIVATE );
				if ( pItemData->sGenerateOp.dwSpecID ) AddTextNoSplit ( strGen, pItemData->sGenerateOp.bGenEnable ? NS_UITEXTCOLOR::ENABLE : NS_UITEXTCOLOR::DISABLE );
			
			}

			CString strText("None");
			DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
			int nIndex = -1;

			if ( emItemType == ITEM_SUIT )
			{
				if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
				{
					strText.Format("[%s]%s", COMMENT::ITEMLEVEL[dwLevel].c_str(), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );

				}else{
					strText.Format("[%s]%s", COMMENT::ITEMLEVEL[dwLevel].c_str(), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
				}

				dwTextColor = COMMENT::ITEMCOLOR[dwLevel];
				nIndex = AddTextNoSplit( strText, dwTextColor );

				if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) || pItemData->sBasicOp.IsDISGUISE() )
				{
					dwTextColor = NS_UITEXTCOLOR::UIINFO_COSNAME;
					strText.Format( "(%s)", ID2GAMEWORD ( "ITEM_BASIC_INFO", 3 ) );
					AddStringNoSplit( nIndex, strText, dwTextColor );
				}

				SITEM* pDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
				if ( pDisguiseData )
				{
					DWORD dwReqCharClass_Disguise = pDisguiseData->sBasicOp.dwReqCharClass;
					dwTextColor = (dwReqCharClass_Disguise&pCharacter->m_emClass) ? NS_UITEXTCOLOR::UIINFO_COSNAME:NS_UITEXTCOLOR::RED;
					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 3 ), pDisguiseData->GetName() );			
					nIndex = AddTextNoSplit( strText, dwTextColor );
				}
			}
			else
			{
				strText.Format("[%s]%s", COMMENT::ITEMLEVEL[dwLevel].c_str(), COMMENT::ITEMTYPE[pItemData->sBasicOp.emItemType].c_str() );
				dwTextColor = COMMENT::ITEMCOLOR[dwLevel];
				nIndex = AddTextNoSplit( strText, dwTextColor );
			}
		}

		//turn num
		AddItemTurnInfo ( sItemCustom, bInMarket, bInPrivateMarket );

		EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;
		//item suit info
		{
			if ( emItemType == ITEM_SUIT || emItemType == ITEM_REVIVE || emItemType == ITEM_ANTI_DISAPPEAR )
			{
				CString strText("");
				//BYTE uGRADE = 0;
				int nExtraValue = 0;

				//attack
				GLPADATA &sDamage = sItemCustom.getdamage();
				nExtraValue = sItemCustom.GETGRADE_DAMAGE();

				if ( nExtraValue || sDamage.wLow || sDamage.wHigh )
				{
					AddInfoItemAddonRangeInt( pItemData->sSuitOp.gdDamage.wLow, pItemData->sSuitOp.gdDamage.wHigh, 
						sDamage.wLow, sDamage.wHigh, nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );
				}

				//defense
				short nDefense = sItemCustom.getdefense();
				nExtraValue = sItemCustom.GETGRADE_DEFENSE();

				if ( nDefense || nExtraValue )
					AddInfoItemAddonInt( pItemData->sSuitOp.nDefense, nDefense, nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

				//attack range
				if ( sItemCustom.GETATTRANGE() )
					AddInfoItemAddonWORD( pItemData->sSuitOp.wAttRange, sItemCustom.GETATTRANGE(), 0, ID2GAMEWORD("ITEM_ADVANCED_INFO", 2) );

				//hitrate
				if ( sItemCustom.GETHITRATE() )
					AddInfoItemAddonInt( pItemData->sSuitOp.nHitRate, sItemCustom.GETHITRATE(), 0, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );

				//avoid rate
				if ( sItemCustom.GETAVOIDRATE() )
					AddInfoItemAddonInt( pItemData->sSuitOp.nAvoidRate, sItemCustom.GETAVOIDRATE(), 0, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );

				//sp usage
				const WORD wReqSP = sItemCustom.GETREQ_SP();
				if ( 0 < wReqSP )
				{
					strText.Format("%s :", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7) );
					int nIndex = AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE );

					strText.Format("%u", wReqSP );
					AddStringNoSplit( nIndex, strText, NS_UITEXTCOLOR::WHITE );
				}

				//random value
				if( sItemCustom.IsSetRandOpt() )
				{
					int nRandomOptionType = 0;
					float fRandomOptionValue = 0.0f;
					BOOL bRandomOptionPercentage =  FALSE;

					nRandomOptionType = sItemCustom.GETOptTYPE1();
					fRandomOptionValue = sItemCustom.GETOptVALUE1();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );

					nRandomOptionType = sItemCustom.GETOptTYPE2();
					fRandomOptionValue = sItemCustom.GETOptVALUE2();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );

					nRandomOptionType = sItemCustom.GETOptTYPE3();
					fRandomOptionValue = sItemCustom.GETOptVALUE3();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );

					nRandomOptionType = sItemCustom.GETOptTYPE4();
					fRandomOptionValue = sItemCustom.GETOptVALUE4();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );
				}

				bool bSpace = false;

				//add value
				AddTextAddValue( bSpace, sItemCustom, pItemData->sSuitOp );
				
				//special function
				AddTextSpecialFunction( bSpace, sItemCustom, pItemData->sSuitOp, pItemData->GetExpMultiple() );

				//resistance
				AddTextResistance( bSpace, sItemCustom, pItemData->sSuitOp );

				//special effect
				AddTextSpecialEffect( pItemData->sSuitOp );

				bSpace = false;
				/*costume combine stats, Juver, 2017/09/01 */
				SITEM*	pItemDisguiseCombine = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
				if ( pItemDisguiseCombine && pItemDisguiseCombine->sBasicOp.IsDISGUISE() )
				{
					AddTextAddValueCostume( bSpace, pItemDisguiseCombine->sSuitOp, pItemData->sSuitOp.emSuit );

					//special function
					AddTextSpecialFunctionCostume( bSpace, pItemDisguiseCombine->sSuitOp, pItemData->sSuitOp.emSuit, 
						pCharacter->m_bCostumeCombineRestrictMoveSpeed, pCharacter->m_bCostumeCombineRestrictAttackSpeed );
				}


			}

			/*gun-bullet logic, Juver, 2017/05/27 */
			if ( emItemType == ITEM_CHARM || emItemType == ITEM_ARROW || emItemType == ITEM_BULLET )
			{
				CString strText("");
				GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
				if ( sDAMAGE.wLow || sDAMAGE.wHigh  )
				{
					strText.Format("%s:%s~%s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), GetNumberWithSign ( sDAMAGE.wLow ), GetNumberWithSign ( sDAMAGE.wHigh ) );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE );
				}

				bool bSpace = false;
				//add value
				AddTextAddValue( bSpace, sItemCustom, pItemData->sSuitOp );		
			}

			if ( emItemType == ITEM_CURE )
			{
				CString strText("");
				if( pItemData->sDrugOp.wCureVolume == 0 )
				{
					strText.Format("%s",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str());
				}else{
					strText.Format("%s:%d",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str(), pItemData->sDrugOp.wCureVolume);
				}

				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
			}

			if ( emItemType == ITEM_SKILL )
			{
				AddInfoSkillItem ( sItemCustom );
			}

			if ( emItemType == ITEM_PET_SKILL )
			{
				AddInfoPetSkillItem ( sItemCustom );
			}

			if ( emItemType == ITEM_GRINDING )
			{

			}

			if ( emItemType == ITEM_VIETNAM_EXPGET )
			{
				CString strText("");
				if ( pItemData->sDrugOp.bRatio )
					strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 1) , pItemData->sDrugOp.wCureVolume );						
				else
					strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 0), pItemData->sDrugOp.wCureVolume );

				AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
			}

			if ( emItemType == ITEM_VIETNAM_ITEMGET )
			{
				CString strText("");
				strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_ITEM", 0) , pItemData->sDrugOp.wCureVolume );
				AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
			}

			if ( emItemType == ITEM_VEHICLE )
			{
				CString strText("");
				
				AddTextNoSplit(" ",NS_UITEXTCOLOR::WHITE);
				
				/*vehicle system, Juver, 2017/08/09 */
				if ( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_BIKE )
				{
					strText.Format( "%s%s(%s)", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), 
						COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str(),
						COMMENT::VEHICLE_SUBTYPE_BIKE[pItemData->sVehicle.emVehicleSubType].c_str());
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
				}
				/* vehicle car, cart, Juver, 2018/02/13 */
				else if( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_CAR )
				{
					strText.Format( "%s%s(%s)", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), 
						COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str(),
						COMMENT::VEHICLE_SUBTYPE_CAR[pItemData->sVehicle.emVehicleSubType].c_str());
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
				}
				/* vehicle car, cart, Juver, 2018/02/13 */
				else if( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_CART )
				{
					strText.Format( "%s%s(%s)", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), 
						COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str(),
						COMMENT::VEHICLE_SUBTYPE_CART[pItemData->sVehicle.emVehicleSubType].c_str());
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
				}
				/* vehicle bike v2, Juver, 2020/09/15 */
				else if( pItemData->sVehicle.emVehicleType == VEHICLE_TYPE_BIKE_V2 )
				{
					strText.Format( "%s%s(%s)", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), 
						COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str(),
						COMMENT::VEHICLE_SUBTYPE_BIKE_V2[pItemData->sVehicle.emVehicleSubType].c_str());
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
				}
				else
				{
					strText.Format( "%s%s(%s)", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), 
						COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str(),
						COMMENT::VEHICLE_SUBTYPE_HOVER[pItemData->sVehicle.emVehicleSubType].c_str());

					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
				}

				int nExtraValue = 0;

				//attack
				GLPADATA &sDamage = sItemCustom.getdamage();
				nExtraValue = sItemCustom.GETGRADE_DAMAGE();

				if ( nExtraValue || sDamage.wLow || sDamage.wHigh )
				{
					AddInfoItemAddonRangeInt( pItemData->sSuitOp.gdDamage.wLow, pItemData->sSuitOp.gdDamage.wHigh, 
						sDamage.wLow, sDamage.wHigh, nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );
				}

				//defense
				short nDefense = sItemCustom.getdefense();
				nExtraValue = sItemCustom.GETGRADE_DEFENSE();

				if ( nDefense || nExtraValue )
					AddInfoItemAddonInt( pItemData->sSuitOp.nDefense, nDefense, nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

				//attack range
				if ( sItemCustom.GETATTRANGE() )
					AddInfoItemAddonWORD( pItemData->sSuitOp.wAttRange, sItemCustom.GETATTRANGE(), 0, ID2GAMEWORD("ITEM_ADVANCED_INFO", 2) );

				//hitrate
				if ( sItemCustom.GETHITRATE() )
					AddInfoItemAddonInt( pItemData->sSuitOp.nHitRate, sItemCustom.GETHITRATE(), 0, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );

				//avoid rate
				if ( sItemCustom.GETAVOIDRATE() )
					AddInfoItemAddonInt( pItemData->sSuitOp.nAvoidRate, sItemCustom.GETAVOIDRATE(), 0, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );

				//random value
				if( sItemCustom.IsSetRandOpt() )
				{
					int nRandomOptionType = 0;
					float fRandomOptionValue = 0.0f;
					BOOL bRandomOptionPercentage =  FALSE;

					nRandomOptionType = sItemCustom.GETOptTYPE1();
					fRandomOptionValue = sItemCustom.GETOptVALUE1();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );

					nRandomOptionType = sItemCustom.GETOptTYPE2();
					fRandomOptionValue = sItemCustom.GETOptVALUE2();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );

					nRandomOptionType = sItemCustom.GETOptTYPE3();
					fRandomOptionValue = sItemCustom.GETOptVALUE3();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );

					nRandomOptionType = sItemCustom.GETOptTYPE4();
					fRandomOptionValue = sItemCustom.GETOptVALUE4();
					bRandomOptionPercentage = sItemCustom.IsPerRandOpt( nRandomOptionType );
					AddTextRandomValue( nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage );
				}

				bool bSpace = false;

				//add value
				AddTextAddValue( bSpace, sItemCustom, pItemData->sSuitOp );

				//special function
				AddTextSpecialFunction( bSpace, sItemCustom, pItemData->sSuitOp, pItemData->GetExpMultiple() );

				//resistance
				AddTextResistance( bSpace, sItemCustom, pItemData->sSuitOp );


				bool bInfo(true);

				VEHICLEITEMINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.find ( sItemCustom.dwVehicleID );
				if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.end() )
				{
					iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.find ( sItemCustom.dwVehicleID );
					if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.end() )	bInfo = false;
				}

				if ( bInfo ) 
				{
					AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

					SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;
					for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i ) 
					{
						SITEM* pItem = GLItemMan::GetInstance().GetItem ( sVehicleItemInfo.m_PutOnItems[i].sNativeID );
						if ( pItem )
						{
							strText.Format("%s %s", ID2GAMEWORD ("ITEM_VEHICLE_SLOT",i), pItem->GetName () );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
						}
					}
					
					strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_VEHICLE_BATTERY",0), sVehicleItemInfo.m_nFull/10.0f );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					
					if ( sVehicleItemInfo.m_emTYPE != VEHICLE_TYPE_BOARD )
					{
						/*vehicle booster system, Juver, 2017/08/10 */
						strText.Format( "%s", ID2GAMEWORD ("ITEM_VEHICLE_BOOST_LEARN", sVehicleItemInfo.m_bBooster ) );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}
				}

			}

			if ( emItemType == ITEM_PET_CARD )
			{
				if (sItemCustom.dwPetID != 0)
				{
					CString strText("");

					bool bInfo(true);

					PETCARDINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.find(sItemCustom.dwPetID);
					if (iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.end())
					{
						iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.find(sItemCustom.dwPetID);
						if (iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.end()) bInfo = false;
					}

					if (bInfo)
					{
						AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

						SPETCARDINFO sPetCardInfo = (*iter).second;

						strText.Format("%s %s", ID2GAMEWORD("ITEM_PET_NAME", 0), sPetCardInfo.m_szName);
						AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

						strText.Format("%s %s", ID2GAMEWORD("ITEM_PET_TYPE", 0), COMMENT::PET_TYPE[sPetCardInfo.m_emTYPE].c_str());
						AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

						strText.Format("%s %.2f%%", ID2GAMEWORD("ITEM_PET_FULL", 0), sPetCardInfo.m_nFull / 10.0f);
						AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

						/*dual pet skill, Juver, 2017/12/29 */
						AddTextNoSplit(ID2GAMEWORD("ITEM_PET_DUALSKILL", sPetCardInfo.m_bDualSkill), NS_UITEXTCOLOR::WHITE);

						SITEM* pItem = GLItemMan::GetInstance().GetItem(sPetCardInfo.m_PutOnItems[PET_ACCETYPE_A].sNativeID);
						if (pItem)
						{
							strText.Format("%s %s", ID2GAMEWORD("ITEM_PET_SLOTA", 0), pItem->GetName());
							AddTextNoSplit(strText, NS_UITEXTCOLOR::UIINFO_COSNAME);
						}

						pItem = GLItemMan::GetInstance().GetItem(sPetCardInfo.m_PutOnItems[PET_ACCETYPE_B].sNativeID);
						if (pItem)
						{
							strText.Format("%s %s", ID2GAMEWORD("ITEM_PET_SLOTB", 0), pItem->GetName());
							AddTextNoSplit(strText, NS_UITEXTCOLOR::UIINFO_COSNAME);
						}

						if (sPetCardInfo.m_ExpSkills.size() > 0)
						{
							AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
							AddTextNoSplit(ID2GAMEWORD("ITEM_PET_SKILLS", 0), NS_UITEXTCOLOR::WHITE);

							PETSKILL_MAP_CITER pos = sPetCardInfo.m_ExpSkills.begin();
							PETSKILL_MAP_CITER pos_end = sPetCardInfo.m_ExpSkills.end();
							for (; pos != pos_end; ++pos)
							{
								const PETSKILL& sPetSkill = (*pos).second;
								PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData(sPetSkill.sNativeID);
								if (pSKILL)
								{
									strText.Format("%s", pSKILL->GetName());
									AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
								}
							}
						}


						if (sItemCustom.tDISGUISE != 0)
						{
							AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

							CTime currentTime = GLGaeaClient::GetInstance().GetCurrentTime();
							CTime startTime = sItemCustom.tBORNTIME;
							CTimeSpan timeSpan = currentTime - startTime;
							if (timeSpan.GetTotalSeconds() < sItemCustom.tDISGUISE)
							{
								strText.Format("[%s]", ID2GAMEWORD("ITEM_PET_USE_SKINPACK", 0));
								AddTextNoSplit(strText, NS_UITEXTCOLOR::GOLD);
								strText.Format("%s : %dsec", ID2GAMEWORD("CLUB_BATTLE_TIME", 0), sItemCustom.tDISGUISE - timeSpan.GetTotalSeconds());
								AddTextNoSplit(strText, NS_UITEXTCOLOR::UIINFO_EXPIRE);
							}
						}
						
					}
				}
			}

			if (emItemType == ITEM_RANDOM_OPTION_CARD)
			{
				AddTextNoSplit(ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 0), NS_UITEXTCOLOR::LIGHTSKYBLUE);

				SITEMCUSTOM sitem_custom_copy = sItemCustom;
				sitem_custom_copy.cOptTYPE1 = (BYTE)pItemData->sRvCard.emOption;
				sitem_custom_copy.nOptVALUE1 = (short)pItemData->sRvCard.wValue;

				INT nRandOptType = sitem_custom_copy.GETOptTYPE1();
				if ((EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE))
				{
					CString strOptionText;
					if (pItemData->sRvCard.bReplaceOpt)
						strOptionText.Format("%s %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 3), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType));
					else
						strOptionText.Format("%s %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 2), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType));

					int nRandomOptionType = sitem_custom_copy.GETOptTYPE1();
					float fRandomOptionValue = sitem_custom_copy.GETOptVALUE1();
					BOOL bRandomOptionPercentage = sitem_custom_copy.IsPerRandOpt(nRandomOptionType);
					AddTextRandomValue(nRandomOptionType, fRandomOptionValue, bRandomOptionPercentage);
				}

				if (pItemData->sRvCard.bCheckExist)
					AddTextNoSplit(ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 4), NS_UITEXTCOLOR::LIGHTSKYBLUE);

				AddTextNoSplit(ID2GAMEWORD("ITEM_RANDOM_OPTION_CARD", 1), NS_UITEXTCOLOR::LIGHTSKYBLUE);
				for (DWORD nSuit = 0; nSuit < SUIT_NSIZE; ++nSuit)
				{
					if (pItemData->sRvCard.bUseSuit[nSuit])
						AddTextNoSplit(COMMENT::ITEMSUIT[nSuit].c_str(), NS_UITEXTCOLOR::PRIVATE);
				}
			}

			if (emItemType == ITEM_REBORD_CARD_B )
			{
				CString strRebornNum;
				strRebornNum.Format("%s %d", ID2GAMEWORD("CHARACTER_REBORN_B_INFO", 0), pItemData->sSuitOp.wReModelNum );
				AddTextNoSplit(strRebornNum.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN);
			}

			if ( emItemType == ITEM_EXCHANGE_ITEM_POINT )
			{
				CString strChangeItemPointNum;
				strChangeItemPointNum.Format("%s %d", ID2GAMEWORD("EXCHANGE_ITEM_POINT_INCREASE", 0), pItemData->sSuitOp.wReModelNum );
				AddTextNoSplit(strChangeItemPointNum.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN);
			}

		}


		/* set item option, Juver, 2021/09/02 */
		if (emItemType == ITEM_SUIT )
		{
			AddInfoSetOption( pItemData, pCharacter, sItemCustom );
		}
		
		//recall card information
		AddInfoRecall(pItemData);

		bool bSpace = false;

		//desc
		AddInfoDesc( bSpace, pItemData );

		//flag
		AddInfoFlags( bSpace, sItemCustom, pItemData );

		//etc info
		AddInfoItemDefaultTimeLimit( pItemData );

		//item expiration
		AddInfoItemExpiration(pItemData, sItemCustom);

		//costume expiration
		AddInfoItemExpirationCostume(sItemCustom);

		AddInfoItemCooltime( pItemData );


		{
			std::vector<SINFO_TEXT> vecText;

			/*rightclick wear/unwear, Juver, 2017/06/24 */
			if (pCharacter && !pCharacter->ValidWindowOpen() && (emItemType == ITEM_SUIT || emItemType == ITEM_ARROW || emItemType == ITEM_CHARM || emItemType == ITEM_BULLET))
			{
				BOOL bUseItem = pCharacter->ACCEPT_ITEM(sItemCustom.sNativeID);

				if (sItemCustom.nidDISGUISE != NATIVEID_NULL())
					bUseItem = pCharacter->ACCEPT_ITEM(sItemCustom.sNativeID, sItemCustom.nidDISGUISE);

				if (bUseItem)
					vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("INVENTOWEAR_ITEM_INFO"), NS_UITEXTCOLOR::WHITE));
			}

			/*item preview, Juver, 2017/07/27 */
			bool bCanPreview = CInnerInterface::GetInstance().PreviewItemCheckSimple(pItemData->sBasicOp.sNativeID);
			if (bCanPreview)
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEWORD("ITEM_USEPREVIEW", 0), NS_UITEXTCOLOR::WHITE));
			}

			/*item link, Juver, 2017/07/31 */
			if (bInInventory)
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("HELP_SHOW_ITEM_LINK"), NS_UITEXTCOLOR::WHITE));
			}

			/*box contents, Juver, 2017/08/30 */
			/*specific item box, Juver, 2018/09/02 */
			if (emItemType == ITEM_BOX || emItemType == ITEM_PREMIUMSET || emItemType == ITEM_BOX_SPECIFIC)
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("HELP_SHOW_ITEM_TOOLTIP"), NS_UITEXTCOLOR::WHITE));
			}

			if( std::string(pItemData->GetRandomOptFile()).empty() == false && RPARAM::bMaxRvPreviewEnable == TRUE )
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("ITEM_USE_MAXRV_PREVIEW"), NS_UITEXTCOLOR::WHITE));
			}

			/*product item, Juver, 2017/10/15 */
			if (bInCompoundResult)
			{
				if (bCompoundGenerateRandomOption)
					vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("COMPOUND_RANDOM_VALUE_POSSIBLE"), NS_UITEXTCOLOR::DODGERBLUE));
				else
					vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("COMPOUND_RANDOM_VALUE_NOT_POSSIBLE"), NS_UITEXTCOLOR::DODGERBLUE));
			}

			if (bCurrencyShop)
			{
				PCROWDATA pCrow = GLCrowDataMan::GetInstance().GetCrowData(SNATIVEID(dwCurrencyShopNpcID));
				if (pCrow)
				{
					SCURRENCY_SHOP_DATA* pShop = GLCrowDataMan::GetInstance().CurrencyShopFind(pCrow->m_sAction.m_strCurrencyShop);
					if (pShop)
					{
						SCURRENCY_SHOP_ITEM* pShopItem = pShop->FindData(SNATIVEID(dwCurrencyShopItemID));
						if (pShopItem)
						{
							if (pShopItem->bGenerateRandomValue)
								vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("CURRENCY_SHOP_RANDOM_VALUE_POSSIBLE"), NS_UITEXTCOLOR::DODGERBLUE));
							else
								vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("CURRENCY_SHOP_RANDOM_VALUE_NOT_POSSIBLE"), NS_UITEXTCOLOR::DODGERBLUE));
						}
					}
				}
			}

			/*item wrapper, Juver, 2018/01/12 */
			if (!bWrapped)
			{
				BOOL bWrappable = pItemData->isWrappable();
				if (sItemCustom.nidDISGUISE != NATIVEID_NULL())	bWrappable = FALSE;
				if (!sItemCustom.IsGM_GENITEM() && pItemData->sBasicOp.IsEXCHANGE())	bWrappable = FALSE;

				if (bWrappable)
				{
					vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("ITEM_INFO_POSSIBLE_WRAP"), NS_UITEXTCOLOR::UIINFO_PLUS));
				}
			}
			else
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("ITEM_INFO_POSSIBLE_UNWRAP"), NS_UITEXTCOLOR::UIINFO_PLUS));
			}


			/*item transfer card, Juver, 2018/01/18 */
			if (pItemData->sBasicOp.bItemTransfer)
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("ITEM_INFO_ITEM_TRANSFER"), NS_UITEXTCOLOR::WHITE));
			}

			/*item user requirement, Juver, 2018/05/24 */
			if (!GLItemUser::GetInstance().CharTest(sItemCustom.sNativeID.dwID, pCharacter->GetUserID(), pCharacter->m_dwCharID, pCharacter->m_dwGuild))
			{
				vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("ITEM_USER_REQUIREMENT"), NS_UITEXTCOLOR::DISABLE));
			}

			if (sItemCustom.nidDISGUISE != NATIVEID_NULL())
			{
				if (!GLItemUser::GetInstance().CharTest(sItemCustom.nidDISGUISE.dwID, pCharacter->GetUserID(), pCharacter->m_dwCharID, pCharacter->m_dwGuild))
				{
					vecText.push_back(SINFO_TEXT(ID2GAMEINTEXT("ITEM_USER_REQUIREMENT"), NS_UITEXTCOLOR::DISABLE));
				}
			}

			for (size_t i = 0; i < vecText.size(); ++i)
			{
				SINFO_TEXT& sData = vecText[i];

				if (i == 0)
					AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

				AddTextLongestLineSplit(sData.strText, sData.dwTextColor);
			}

		}
		

		
		
		

		/*item color, Juver, 2018/01/10 */
		bool bColoring = pItemData->sBasicOp.bItemColor;
		SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
		if ( pitem_data_disguise && pitem_data_disguise->sBasicOp.bItemColor )
			bColoring = true;
		
		if ( bColoring )
		{
			AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );

			CString strText("");
			strText.Format( "%s :", ID2GAMEWORD ( "ITEM_INFO_COLOR", 0 ) );
			int nIndex = AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
		
			{
				WORD wCOLOR = sItemCustom.wColor1;
				WORD wR, wG, wB;
				wR = wCOLOR >> 10;
				wG = wCOLOR >> 5;
				wG = wG&0x1f;
				wB = wCOLOR&0x1f;
		
				wR = (WORD)( wR * 8.225806f );
				wG = (WORD)( wG * 8.225806f );
				wB = (WORD)( wB * 8.225806f );
		
				DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;
				AddStringNoSplit ( nIndex, ID2GAMEWORD ( "ITEM_INFO_COLOR", 1 ), dwColor );
			}
		
			{
				WORD wCOLOR = sItemCustom.wColor2;
				WORD wR, wG, wB;
				wR = wCOLOR >> 10;
				wG = wCOLOR >> 5;
				wG = wG&0x1f;
				wB = wCOLOR&0x1f;
		
				wR = (WORD)( wR * 8.225806f );
				wG = (WORD)( wG * 8.225806f );
				wB = (WORD)( wB * 8.225806f );
		
				DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;
				AddStringNoSplit ( nIndex, ID2GAMEWORD ( "ITEM_INFO_COLOR", 2 ), dwColor );
			}
		}
		
		//price
		if ( pItemData->sBasicOp.dwBuyPrice || bInPrivateMarket )
		{
			
			CString strText("");

			if ( bInPrivateMarket )
			{
				bool bOPENER;
				DWORD dwPrivateMarketID;
				CInnerInterface::GetInstance().GetPrivateMarketInfo ( bOPENER, dwPrivateMarketID );
				if ( bOPENER )
				{
					GLPrivateMarket &sPMarket = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket;
	
					const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
					if ( pSALE )
					{
						CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
						strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );	

						AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
					}						
				}
				else
				{
					PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwPrivateMarketID );
					if ( !pCLIENT ) return ;
	
					GLPrivateMarket &sPMarket = pCLIENT->m_sPMarket;
	
					const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
					if ( pSALE )
					{
						CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
						strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );	

						AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
					}
				}
			}				
			else if ( bShopOpen && bInMarket )
			{
				LONGLONG llNpcSellPrice = 0;
	
				volatile LONGLONG llPrice = 0;
				volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
				volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;
	
				//if( sNpcNativeID.wMainID != 0 && sNpcNativeID.wSubID != 0 )
				{
					PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNpcNativeID );
					if( pCrowData != NULL )
					{
						std::string strCrowSale = pCrowData->GetCrowSale( dwMarketChannel );
						if ( strCrowSale.size() )
						{
							SCROWSALE_DATA* pCrowSale = GLCrowDataMan::GetInstance().CrowSaleFind( strCrowSale );
							if ( pCrowSale )
							{
								LONGLONG llNpcPrice = pCrowSale->GetNpcSellPrice( pItemData->sBasicOp.sNativeID.dwID );
								if( llNpcPrice == 0 )
								{								
									llNpcSellPrice = pItemData->sBasicOp.dwBuyPrice;
									llPrice = DWORD ( (float)llNpcSellPrice * fSHOP_RATE_C );
								}else{
									llNpcSellPrice = llNpcPrice;
									llPrice = llNpcSellPrice;								
								}
							}
						}
					}
				}
	
				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( llPrice, 3, "," );
	
				strText.Format( "%s : %s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );
	
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( llPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
				AddTextNoSplit ( strText, dwColor );
			}
			else if ( bShopOpen )
			{
				volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetSaleRate();
				volatile float fSALE_DISCOUNT = fSHOP_RATE * 0.01f;
	
				volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
				volatile DWORD dwSALE_PRICE = DWORD ( dwPrice * fSALE_DISCOUNT );					
	
				volatile DWORD dwDISPRICE = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );

				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwSALE_PRICE, 3, "," );
				strText.Format( "%s : %s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );
	
				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE);
			}
			/*npc shop, Juver, 2017/07/26 */
			else if( bNPCShop )
			{
				//from npc shop
				volatile LONGLONG llPrice = pItemData->sBasicOp.dwBuyPrice;
	
				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( llPrice, 3, "," );
				strText.Format( "%s : %s", ID2GAMEWORD("NPC_SHOP_TYPE_PRICE", wNPCShopType ), strMoney );
	
				D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
				if( llPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
				{
					dwColor = NS_UITEXTCOLOR::WHITE;
				}

				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
				AddTextNoSplit ( strText, dwColor );
			}
		}
	

		/* item refines ID, Juver, 2021/07/30 */
		if( pItemData->sBasicOp.emItemType == ITEM_SUIT || pItemData->sBasicOp.emItemType == ITEM_GRINDING )
		{
			if ( pItemData->sBasicOp.wItemRefinesID != ITEM_REFINE_ID_DEFAULT )
			{
				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
				CString strTemp;
				strTemp.Format( ID2GAMEINTEXT( "GRINDING_ITEM_INFO_ID" ), pItemData->sBasicOp.wItemRefinesID );
				AddTextNoSplit ( strTemp.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
			}
		}

	}



};
