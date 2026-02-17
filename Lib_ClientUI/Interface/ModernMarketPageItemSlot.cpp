#include "StdAfx.h"
#include "./ModernMarketPageItemSlot.h"

#include "./ItemImage.h"
#include "./InnerInterface.h"
#include "./ItemMove.h"
#include "./BasicTextBox.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/Core/NSRProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernMarketPageItemSlot::nOUTOFRANGE = -1;

CModernMarketPageItemSlot::CModernMarketPageItemSlot () :
	m_pMouseOver ( NULL ),
	m_nMaxColumn ( 0 ),
	m_sNPCCrowID(false),
	m_dwNPCChannel(0)
{
	SecureZeroMemory ( m_pItemImageArray, sizeof( m_pItemImageArray ) );	
	SecureZeroMemory ( m_pNumberBoxArray, sizeof( m_pNumberBoxArray ) );
}

CModernMarketPageItemSlot::~CModernMarketPageItemSlot ()
{
}

void CModernMarketPageItemSlot::CreateSubControl ( int nMaxColumn, BOOL bNumberUse )
{
	m_nMaxColumn = nMaxColumn;

	{
		CString	strSlotBack[nLIMIT_COLUMN] = 
		{
			"ITEM_IMAGE_BACK0",
			"ITEM_IMAGE_BACK1",
			"ITEM_IMAGE_BACK2",
			"ITEM_IMAGE_BACK3",
			"ITEM_IMAGE_BACK4",
			"ITEM_IMAGE_BACK5",
			"ITEM_IMAGE_BACK6",
			"ITEM_IMAGE_BACK7",
			"ITEM_IMAGE_BACK8",
			"ITEM_IMAGE_BACK9"
		};

		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			m_pItemImageArray[i] = CreateItemImage ( strSlotBack[i].GetString (), ITEM_IMAGE0 + i );			
		}
	}

	if ( bNumberUse )
	{
		CreateNumberBox ();
	}

	m_pMouseOver = new CUIControl;
	m_pMouseOver->CreateSub ( this, "ITEM_MOUSE_OVER" );
	RegisterControl ( m_pMouseOver );
}

CItemImage*	CModernMarketPageItemSlot::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

void CModernMarketPageItemSlot::ResetItemImage ( int nIndex )
{
	if ( nIndex < 0 || m_nMaxColumn <= nIndex )
	{
		return ;
	}

	CItemImage* pItem = m_pItemImageArray[nIndex];
	pItem->ResetItem ();
	
	m_InvenItem[nIndex].sItemCustom.sNativeID = NATIVEID_NULL ();
}

void CModernMarketPageItemSlot::SetItemImage ( int nIndex, SINVENITEM& ref_InvenItem )
{
	if ( nIndex < 0 || m_nMaxColumn <= nIndex )
	{
		return ;
	}

	SITEMCUSTOM &sItemCustom = ref_InvenItem.sItemCustom;
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
	if ( !pItem )
	{
		GASSERT ( 0 && "등록되지 않은 ID입니다." );
		return ;
	}
    
	CItemImage* pItemImage = m_pItemImageArray[nIndex];
	pItemImage->SetItem ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile(), pItem->sBasicOp.sNativeID );

	/*item wrapper, Juver, 2018/01/12 */
	if ( pItem->sBasicOp.emItemType == ITEM_WRAPPER_BOX )
	{
		SITEM* pitem_disguise = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
		if ( pitem_disguise )
			pItemImage->SetBackItem( pitem_disguise->sBasicOp.sICONID, pitem_disguise->GetInventoryFile() );
	}

	m_InvenItem[nIndex] = ref_InvenItem;
}

void CModernMarketPageItemSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
    SetItemIndex ( nOUTOFRANGE );
	m_pMouseOver->SetVisibleSingle ( FALSE );
    
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	for ( int i=0; i<nLIMIT_COLUMN; ++i )
	{
		if ( m_pItemImageArray[i] )
		{
			BOOL bCheck = NPCItemCheck( m_InvenItem[i].sItemCustom.sNativeID, m_InvenItem[i].sItemCustom.wTurnNum );
			m_pItemImageArray[i]->SetDiffuse ( bCheck? D3DCOLOR_ARGB(125,125,125,125):D3DCOLOR_ARGB(255,255,255,255) );
		}
	}

	if ( m_pNumberBoxArray[0] )
	{
		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			ResetNumber ( i );

			SNATIVEID sNativeID = m_InvenItem[i].sItemCustom.sNativeID;
			if ( sNativeID != NATIVEID_NULL () )
			{	
				SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
				if ( pItemData )
				{
					const WORD wPileNum = pItemData->sDrugOp.wPileNum;
					const WORD wTurnNum = m_InvenItem[i].sItemCustom.wTurnNum;						

					if ( wPileNum>1 || wTurnNum>1 )
					{
						SetNumber ( i, wTurnNum, wPileNum );
					}
				}
			}
		}
	}
}

void CModernMarketPageItemSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ITEM_IMAGE0:
	case ITEM_IMAGE1:
	case ITEM_IMAGE2:
	case ITEM_IMAGE3:
	case ITEM_IMAGE4:
	case ITEM_IMAGE5:
	case ITEM_IMAGE6:
	case ITEM_IMAGE7:
	case ITEM_IMAGE8:
	case ITEM_IMAGE9:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_nIndex = ControlID - ITEM_IMAGE0;			

				if ( !CInnerInterface::GetInstance().IsFirstItemSlot () ) return ;								

				if ( m_pItemImageArray[m_nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pItemImageArray[m_nIndex]->GetGlobalPos ();
					const UIRECT& rcImageLocalPos = m_pItemImageArray[m_nIndex]->GetLocalPos ();
					m_pMouseOver->SetLocalPos ( rcImageLocalPos );
					m_pMouseOver->SetGlobalPos ( rcImagePos );
					m_pMouseOver->SetVisibleSingle ( TRUE );
				}

				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
				if ( !pItemMove )
				{
					GASSERT ( 0 && "CItemMove가 널입니다." );
					return ;
				}

				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pItemImageArray[m_nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						

					AddMessageEx ( UIMSG_MOUSEIN_MODERN_MARKET_WINDOW_PAGE_ITEMSLOT | UIMSG_TOTOPPARENT );
				}
			}
		}
		break;
	};
}

CBasicTextBox* CModernMarketPageItemSlot::CreateNumberBox ( const char* szControl )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
    pTextBox->SetFont ( pFont8 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( pTextBox );

	return pTextBox;
}

void CModernMarketPageItemSlot::SetNumber ( int nIndex, int nNumber, int nMaxNumber )
{
	CString strNumber;
	strNumber.Format ( "%d", nNumber );
	DWORD dwColor = NS_UITEXTCOLOR::GREENYELLOW;
	if ( nNumber == nMaxNumber ) dwColor = NS_UITEXTCOLOR::ORANGE;
	m_pNumberBoxArray[nIndex]->SetOneLineText ( strNumber, dwColor );
	m_pNumberBoxArray[nIndex]->SetVisibleSingle ( TRUE );
}

void CModernMarketPageItemSlot::ResetNumber ( int nIndex )
{
	m_pNumberBoxArray[nIndex]->ClearText ();
	m_pNumberBoxArray[nIndex]->SetVisibleSingle ( FALSE );
}

void CModernMarketPageItemSlot::CreateNumberBox ()
{
	{
		CString	strSlotBack[nLIMIT_COLUMN] = 
		{
			"ITEM_NUMBER0",
			"ITEM_NUMBER1",
			"ITEM_NUMBER2",
			"ITEM_NUMBER3",
			"ITEM_NUMBER4",
			"ITEM_NUMBER5",
			"ITEM_NUMBER6",
			"ITEM_NUMBER7",
			"ITEM_NUMBER8",
			"ITEM_NUMBER9"
		};

		for ( int i = 0; i < m_nMaxColumn; i++ )
		{
			m_pNumberBoxArray[i] = CreateNumberBox ( strSlotBack[i].GetString () );			
		}
	}
}

BOOL CModernMarketPageItemSlot::NPCItemCheck( SNATIVEID sID, WORD wNum )
{
	SITEM* sItemCheck = GLItemMan::GetInstance().GetItem( sID );
	if ( !sItemCheck )	return FALSE;

	if ( !RPROFILE::bModernMarketWindowSettingUsable )
	{
		if ( ! GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM( sID ) )
			return TRUE;
	}

	if ( !RPROFILE::bModernMarketWindowSettingBuyable )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem( sID );
		if ( pItemData )
		{
			LONGLONG dwNpcSellPrice = 0;

			WORD wItemNum = 1;

			if( pItemData->ISPILE() && (pItemData->GETAPPLYNUM()==1) )
			{
				wItemNum = wNum;
				if ( wItemNum == 0 )	wItemNum = 1;
			}

			volatile LONGLONG dwPrice = 0;
			volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
			volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;

			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( m_sNPCCrowID );
			if( pCrowData != NULL )
			{
				std::string strCrowSale = pCrowData->GetCrowSale( m_dwNPCChannel );
				if ( strCrowSale.size() )
				{
					SCROWSALE_DATA* pCrowSale = GLCrowDataMan::GetInstance().CrowSaleFind( strCrowSale );
					if ( pCrowSale )
					{
						LONGLONG dwNpcPrice = pCrowSale->GetNpcSellPrice( pItemData->sBasicOp.sNativeID.dwID );
						if( dwNpcPrice == 0 )
						{								
							dwNpcSellPrice = pItemData->sBasicOp.dwBuyPrice;
							dwPrice = DWORD ( (float)dwNpcSellPrice * fSHOP_RATE_C );
						}else{
							dwNpcSellPrice = dwNpcPrice;
							dwPrice = dwNpcSellPrice;								
						}
					}
				}
			}

			if( dwPrice * wItemNum > GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
				return TRUE;	

		}
	}

	return FALSE;
}

void CModernMarketPageItemSlot::NPCCrowIDSet( SNATIVEID sID )
{
	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sID );
	if( pCrowData )
	{
		m_sNPCCrowID = pCrowData->m_sBasic.sNativeID;
	}
}