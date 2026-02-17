#include "stdafx.h"
#include "./InnerInterface.h"

#include "./UIInfoLoader.h"

#include "./BasicVarTextBox.h"
#include "./BasicVarTextBoxItemLink.h"

/* modern info tooltip, Juver, 2020/04/09 */
#include "./BasicVarTextBoxEx.h"
#include "./BasicVarTextBoxExItemLink.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CInnerInterface::InfoDisplayReset ()
{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	NS_ITEMINFO::RESET ();
	NS_SKILLINFO::RESET();
#else
	/* modern info tooltip, Juver, 2020/04/09 */
	NS_ITEMINFO_EX::RESET ();
	NS_SKILLINFO_EX::RESET();
#endif

	m_strGeneralInfoBack.Empty ();

	if( m_pInfoDisplayMain ) 
		m_pInfoDisplayMain->ClearText ();

	/*modern character window, Juver, 2018/03/15 */
	m_sItemInfoTemp = SITEMCUSTOM();
}

void CInnerInterface::InfoDisplayShowItemSimple ( SITEMCUSTOM &sItemCustom )
{
	if ( IsMouseInControl () ) return ;

	//	NOTE
	//		이미 뿌리고 있다는 것은
	//		다른 창에서 먼저 데이타를 쓰고 있다는 이야기이다.
	if ( IsVisibleGroup ( INFO_DISPLAY_MAIN ) ) return ;
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		InfoDisplayReset ();
		m_etInfoType = ET_ITEM_INFO;
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	NS_ITEMINFO::LOAD_SIMPLE ( sItemCustom );
#else
	/* modern info tooltip, Juver, 2020/04/09 */
	NS_ITEMINFO_EX::LOAD_SIMPLE ( sItemCustom );
#endif

	/*modern character window, Juver, 2018/03/15 */
	m_sItemInfoTemp = sItemCustom;

	ShowGroupTop ( INFO_DISPLAY_MAIN );

	m_bInfoDisplayActive = true;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */
}

/*npc shop, Juver, 2017/07/26 */
/*product item, Juver, 2017/10/15 */
void CInnerInterface::InfoDisplayShowItemAdvanced ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID /* = SNATIVEID */,
	BOOL bNPCShop /*= FALSE*/, WORD wNPCShopType /*= 0*/, BOOL bInInventory /*= FALSE*/, BOOL bInCompoundResult/* = FALSE*/, BOOL bCompoundGenerateRandomOption/* = FALSE*/,
	BOOL bCurrencyShop /*= FALSE*/, SNATIVEID sCurrencyShopNpcID /*= SNATIVEID(false)*/, SNATIVEID sCurrencyShopItemID /*= SNATIVEID(false)*/, DWORD dwMarketChannel /*= 0*/ ) 
{	
	if ( IsMouseInControl () ) return ;

	//	NOTE
	//		이미 뿌리고 있다는 것은
	//		다른 창에서 먼저 데이타를 쓰고 있다는 이야기이다.
	if ( IsVisibleGroup ( INFO_DISPLAY_MAIN ) ) return ;
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		InfoDisplayReset ();
		m_etInfoType = ET_ITEM_INFO;
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	NS_ITEMINFO::LOAD ( sItemCustom, bShopOpen, bInMarket, bInPrivateMarket, FALSE, wPosX, wPosY, sNpcNativeID, 
		bNPCShop, wNPCShopType, FALSE, bInInventory, bInCompoundResult, bCompoundGenerateRandomOption,
		bCurrencyShop, sCurrencyShopNpcID.dwID, sCurrencyShopItemID.dwID, dwMarketChannel ); 
#else
	/*npc shop, Juver, 2017/07/26 */
	/*item link, Juver, 2017/07/31 */
	/*product item, Juver, 2017/10/15 */
	/* modern info tooltip, Juver, 2020/04/09 */
	NS_ITEMINFO_EX::LOAD ( sItemCustom, bShopOpen, bInMarket, bInPrivateMarket, FALSE, wPosX, wPosY, sNpcNativeID, 
		bNPCShop, wNPCShopType, FALSE, bInInventory, bInCompoundResult, bCompoundGenerateRandomOption,
		bCurrencyShop, sCurrencyShopNpcID.dwID, sCurrencyShopItemID.dwID, dwMarketChannel ); 
#endif

	/*modern character window, Juver, 2018/03/15 */
	m_sItemInfoTemp = sItemCustom;

	ShowGroupTop ( INFO_DISPLAY_MAIN );

	m_bInfoDisplayActive = true;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */

	//	비교 정보 ( 자기가 현재 착용하고 있는 아이템과 마우스 오버된 아이템의 정보출력 )
	BOOL bTRADE = IsVisibleGroup ( TRADE_WINDOW );

	/*npc shop, Juver, 2017/07/26 */
	if ( bTRADE || bInMarket || bInPrivateMarket || bNPCShop || bCurrencyShop )
	{
		for( int nSLOT = SLOT_HEADGEAR; nSLOT < SLOT_NSIZE_S_2; ++nSLOT )
		{			
			if ( GLGaeaClient::GetInstance().GetCharacter()->CHECKSLOT_ITEM ( sItemCustom.sNativeID, (EMSLOT) nSLOT ) )
			{
				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM ( (EMSLOT) nSLOT );
				if ( sItemCustom.sNativeID == NATIVEID_NULL () ) return ;

				m_bInfoDisplayModeSub = true;
				{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
					//	ITEM 정보 뽑기, 마지막 -1은 의미없는 값임
					//
					/*npc shop, Juver, 2017/07/26 */
					/*item link, Juver, 2017/07/31 */
					NS_ITEMINFO::LOAD ( sItemCustom, FALSE, FALSE, FALSE, TRUE, wPosX, wPosY, sNpcNativeID,
						FALSE, 0, FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0, 0 );
#else
					//	ITEM 정보 뽑기, 마지막 -1은 의미없는 값임
					//
					/*npc shop, Juver, 2017/07/26 */
					/*item link, Juver, 2017/07/31 */
					NS_ITEMINFO_EX::LOAD ( sItemCustom, FALSE, FALSE, FALSE, TRUE, wPosX, wPosY, sNpcNativeID,
						FALSE, 0, FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0, 0 );
#endif
				}
				m_bInfoDisplayModeSub = false;

				ShowGroupTop ( INFO_DISPLAY_SUB );
			}
		}		
	}
}

void CInnerInterface::InfoDisplayShowSkillAdvanced ( SNATIVEID sNativeID, const BOOL bNextLevel )
{
	if ( IsMouseInControl () ) return ;

	if ( IsVisibleGroup ( INFO_DISPLAY_MAIN ) ) return ;
	if ( m_etInfoType != ET_SKILL_INFO )
	{
		InfoDisplayReset ();
		m_etInfoType = ET_SKILL_INFO;
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	NS_SKILLINFO::LOAD ( sNativeID, bNextLevel );
#else
	/* modern info tooltip, Juver, 2020/04/11 */
	NS_SKILLINFO_EX::LOAD(sNativeID, bNextLevel);
#endif

	ShowGroupTop ( INFO_DISPLAY_MAIN );

	m_bInfoDisplayActive = true;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */

	/*multi skill Juver, 2018/09/05 */
	PGLSKILL pskill_main = GLSkillMan::GetInstance().GetData( sNativeID );
	if ( pskill_main )
	{
		PGLSKILL pskill_sub = GLSkillMan::GetInstance().GetData( pskill_main->m_sEXT_DATA.idMultiTargetSkill );
		if ( pskill_sub )
		{
			m_bInfoDisplayModeSub = true;

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
			NS_SKILLINFO::LOAD_SUB ( pskill_main->m_sBASIC.sNATIVEID, pskill_sub->m_sBASIC.sNATIVEID, bNextLevel );
#else
			/* modern info tooltip, Juver, 2020/04/11 */
			NS_SKILLINFO_EX::LOAD_SUB(pskill_main->m_sBASIC.sNATIVEID, pskill_sub->m_sBASIC.sNATIVEID, bNextLevel);
#endif

			m_bInfoDisplayModeSub = false;
			ShowGroupTop ( INFO_DISPLAY_SUB );
		}
	}
}

void CInnerInterface::InfoDisplayShowCommonSingleLine ( const CString& strText, D3DCOLOR dwColor )
{
	if ( IsMouseInControl () ) return ;
	if ( IsVisibleGroup ( INFO_DISPLAY_MAIN ) ) return ;		
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		InfoDisplayReset ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	if ( !strText.GetLength() ) return ;

	if ( m_strGeneralInfoBack != strText )
	{
		m_pInfoDisplayMain->ClearText ();
		m_pInfoDisplayMain->AddTextNoSplit ( strText, dwColor );

		m_strGeneralInfoBack = strText;
	}

	ShowGroupTop ( INFO_DISPLAY_MAIN );

	m_bInfoDisplayActive = true;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */
}

bool CInnerInterface::InfoDisplayShowCommonMultiLineBegin  ()
{
	if ( IsMouseInControl () ) return false;
	if ( IsVisibleGroup ( INFO_DISPLAY_MAIN ) ) return false;
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		InfoDisplayReset ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	m_strGeneralInfoBack.Empty ();
	m_pInfoDisplayMain->ClearText ();

	m_bInfoDisplayActive = true;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */

	return true;
}

bool CInnerInterface::InfoDisplayShowCommonMultiLineInsert ( const CString& strText, D3DCOLOR dwColor )
{
	if ( IsVisibleGroup ( INFO_DISPLAY_MAIN ) ) return false;

	m_pInfoDisplayMain->AddTextNoSplit ( strText, dwColor );

	return true;
}

void CInnerInterface::InfoDisplayShowCommonMultiLineEnd ()
{
	ShowGroupTop ( INFO_DISPLAY_MAIN );
}

/*item link, Juver, 2017/07/31 */
void CInnerInterface::InfoDisplayItemLinkReset ()
{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	NS_ITEMINFO::RESET ();
#else
	/* modern info tooltip, Juver, 2020/04/09 */
	NS_ITEMINFO_EX::RESET ();
#endif

	m_strGeneralInfoBack.Empty ();

	if( m_pInfoDisplayItemLink ) 
		m_pInfoDisplayItemLink->ClearText ();

	/*modern character window, Juver, 2018/03/15 */
	m_sItemInfoTemp = SITEMCUSTOM();
}

/*item link, Juver, 2017/07/31 */
void CInnerInterface::InfoDisplayItemLinkClearText ()
{
	m_sItemInfoLinkTemp = SITEMCUSTOM();

	InfoDisplayItemLinkReset ();
	HideGroup( INFO_DISPLAY_ITEM_LINK );
}

/*item link, Juver, 2017/07/31 */
void CInnerInterface::InfoDisplayItemLinkSet ( SITEMCUSTOM sITEM )
{
	m_sItemInfoLinkTemp = sITEM;
	InfoDisplayItemLinkReset ();
	InfoDisplayItemLinkShow( m_sItemInfoLinkTemp );
}

/*item link, Juver, 2017/07/31 */
void CInnerInterface::InfoDisplayItemLinkShow ( SITEMCUSTOM &sItemCustom )
{	
	if ( IsVisibleGroup ( INFO_DISPLAY_ITEM_LINK ) )	 return ;

	if ( m_etInfoType != ET_ITEM_INFO_LINK )
	{
		InfoDisplayItemLinkReset ();
		m_etInfoType = ET_ITEM_INFO_LINK;
	}

	m_bInfoDisplayModeItemLink = true;

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	NS_ITEMINFO::LOAD ( sItemCustom, FALSE, FALSE, FALSE, FALSE, 0, 0, SNATIVEID( false ), FALSE, 0, TRUE );
#else
	/* modern info tooltip, Juver, 2020/04/09 */
	NS_ITEMINFO_EX::LOAD ( sItemCustom, FALSE, FALSE, FALSE, FALSE, 0, 0, SNATIVEID( false ), FALSE, 0, TRUE );
#endif

	/*modern character window, Juver, 2018/03/15 */
	m_sItemInfoTemp = sItemCustom;

	m_bInfoDisplayModeItemLink = false;

	ShowGroupTop ( INFO_DISPLAY_ITEM_LINK );
}

void CInnerInterface::InfoDisplayClearText ()
{
	/*item link, Juver, 2017/07/31 */
	if ( m_bInfoDisplayModeItemLink )
	{
		m_pInfoDisplayItemLink->ClearText ();
	}
	else if ( m_bInfoDisplayModeSub )
	{
		m_pInfoDisplaySub->ClearText ();
	}
	else
	{
		m_pInfoDisplayMain->ClearText ();	
	}
}

int CInnerInterface::InfoDisplayAddTextNoSplit ( const CString& strText, const D3DCOLOR& dwColor )
{
	/*item link, Juver, 2017/07/31 */
	if ( m_bInfoDisplayModeItemLink )
	{
		return m_pInfoDisplayItemLink->AddTextNoSplit ( strText, dwColor );
	}
	else if ( m_bInfoDisplayModeSub )
	{
		return m_pInfoDisplaySub->AddTextNoSplit ( strText, dwColor );
	}
	else
	{
		return m_pInfoDisplayMain->AddTextNoSplit ( strText, dwColor );
	}
}

void CInnerInterface::InfoDisplayAddTextLongestLineSplit  ( const CString& strText, const D3DCOLOR& dwColor )
{
	/*item link, Juver, 2017/07/31 */
	if ( m_bInfoDisplayModeItemLink )
	{
		m_pInfoDisplayItemLink->AddTextLongestLineSplit ( strText, dwColor );
	}
	else if ( m_bInfoDisplayModeSub )
	{
		m_pInfoDisplaySub->AddTextLongestLineSplit ( strText, dwColor );
	}
	else
	{
		m_pInfoDisplayMain->AddTextLongestLineSplit ( strText, dwColor );
	}
}

void CInnerInterface::InfoDisplayAddStringNoSplit( const int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	/*item link, Juver, 2017/07/31 */
	if ( m_bInfoDisplayModeItemLink )
	{
		m_pInfoDisplayItemLink->AddStringNoSplit ( nIndex, strText, dwColor );
	}
	else if ( m_bInfoDisplayModeSub )
	{
		m_pInfoDisplaySub->AddStringNoSplit ( nIndex, strText, dwColor );
	}
	else
	{
		m_pInfoDisplayMain->AddStringNoSplit ( nIndex, strText, dwColor );
	}
}

/* modern info tooltip, Juver, 2020/04/09 */
void CInnerInterface::InfoDisplayAddTitle ( const CString& strText, const D3DCOLOR& dwColor )
{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
#else
	if ( m_bInfoDisplayModeItemLink )
	{
		m_pInfoDisplayItemLink->AddTitleText ( strText, dwColor );
	}
	else if ( m_bInfoDisplayModeSub )
	{
		m_pInfoDisplaySub->AddTitleText ( strText, dwColor );
	}
	else
	{
		m_pInfoDisplayMain->AddTitleText ( strText, dwColor );
	}
#endif
}

/* modern info tooltip, Juver, 2020/04/09 */
void CInnerInterface::InfoDisplaySetIcon( SNATIVEID sICONINDEX, const char* szTexture )
{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
#else
	if ( m_bInfoDisplayModeItemLink )
	{
		m_pInfoDisplayItemLink->SetIcon( sICONINDEX, szTexture );
	}
	else if ( m_bInfoDisplayModeSub )
	{
		m_pInfoDisplaySub->SetIcon( sICONINDEX, szTexture );
	}
	else
	{
		m_pInfoDisplayMain->SetIcon( sICONINDEX, szTexture );
	}
#endif
}

void CInnerInterface::SHOW_ITEM_INFO ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID /* = SNATIVEID */,
	BOOL bNPCShop /*= FALSE*/, WORD wNPCShopType /*= 0*/, BOOL bInInventory /*= FALSE*/, BOOL bInCompoundResult/* = FALSE*/, BOOL bCompoundGenerateRandomOption/* = FALSE*/,
	BOOL bCurrencyShop /*= FALSE*/, SNATIVEID sCurrencyShopNpcID /*= SNATIVEID(false)*/, SNATIVEID sCurrencyShopItemID /*= SNATIVEID(false)*/, DWORD dwMarketChannel /*= 0*/ ) 
{	
	InfoDisplayShowItemAdvanced ( sItemCustom, bShopOpen, bInMarket, bInPrivateMarket, wPosX, wPosY, sNpcNativeID,
		bNPCShop, wNPCShopType, bInInventory, bInCompoundResult, bCompoundGenerateRandomOption, 
		bCurrencyShop, sCurrencyShopNpcID, sCurrencyShopItemID, dwMarketChannel ) ;
}

void CInnerInterface::SHOW_SKILL_INFO ( SNATIVEID sNativeID, const BOOL bNextLevel )
{
	InfoDisplayShowSkillAdvanced ( sNativeID, bNextLevel );
}

/*item link, Juver, 2017/07/31 */
void CInnerInterface::SET_INFO_ITEM_LINK ( SITEMCUSTOM sITEM )
{
	InfoDisplayItemLinkSet( sITEM );
}

/*extended buff info, EJCode, 2018/10/06 */
void CInnerInterface::SHOW_SKILL_INFO_SIMPLE(SNATIVEID sSkillID, WORD wLevel)
{
	if (IsMouseInControl()) return;

	if (IsVisibleGroup(INFO_DISPLAY_MAIN)) return;

	if (m_etInfoType != ET_SKILL_INFO)
	{
		InfoDisplayReset();
		m_etInfoType = ET_SKILL_INFO;
	}

#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
	NS_SKILLINFO::LOAD_SIMPLE(sSkillID, wLevel);
#else
	/* modern info tooltip, Juver, 2020/04/11 */
	NS_SKILLINFO_EX::LOAD_SIMPLE(sSkillID, wLevel);
#endif

	ShowGroupTop(INFO_DISPLAY_MAIN);

	m_bInfoDisplayActive = true;
	m_bInfoDisplayModeItemLink = false; /*item link, Juver, 2017/07/31 */
	m_bInfoDisplayModeSub = false;
}