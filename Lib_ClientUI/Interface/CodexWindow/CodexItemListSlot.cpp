#include "StdAfx.h"
#include "CodexItemListSlot.h"

#include "../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../Lib_ClientUI/Interface/ItemImage.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"

#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCodexItemListSlot::CCodexItemListSlot ()	
	: m_pItemImage(NULL)
	, m_pTextName(NULL)
	, m_pTextPrice(NULL)
	, m_pTextPoint(NULL)
	, m_sidItem(NATIVEID_NULL())
{
}

CCodexItemListSlot::~CCodexItemListSlot ()
{
}

void CCodexItemListSlot::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_CODEX_SALESITEM_ITEM_LINEBOX" );
	RegisterControl ( pLineBox );

	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub ( this, "RAN_CODEX_SALESITEM_ITEM_ITEMIMAGE", UI_FLAG_DEFAULT, ACTIVITY_ITEM_LIST_SLOT_ITEM_IMAGE );
	m_pItemImage->CreateSubControl ();
	m_pItemImage->CreateTextBoxDownRight();
	RegisterControl ( m_pItemImage );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "RAN_CODEX_SALESITEM_ITEM_ITEMNAME" );
	m_pTextName->SetFont ( pFont );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextName );

	m_pTextPrice = new CBasicTextBox;
	m_pTextPrice->CreateSub ( this, "RAN_CODEX_SALESITEM_ITEM_PRICE" );
	m_pTextPrice->SetFont ( pFont );
	m_pTextPrice->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );
	m_pTextPrice->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextPrice );

	m_pTextPoint = new CBasicTextBox;
	m_pTextPoint->CreateSub ( this, "RAN_CODEX_SALESITEM_ITEM_REQPOINT" );
	m_pTextPoint->SetFont ( pFont );
	m_pTextPoint->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextPoint->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextPoint );
}

void CCodexItemListSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ACTIVITY_ITEM_LIST_SLOT_ITEM_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sidItem;

				if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

				SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
				if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0 );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sitemcustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						return;
					}
				}
			}
		}break;
	};
}

void CCodexItemListSlot::DataReset()
{
	if ( m_pItemImage )	
	{
		m_pItemImage->ResetItem();
		m_pItemImage->SetTextDownRight( "", NS_UITEXTCOLOR::GOLD );
	}

	if ( m_pTextName )	m_pTextName->ClearText();
	if ( m_pTextPrice )	m_pTextPrice->ClearText();
	if ( m_pTextPoint )	m_pTextPoint->ClearText();
}

void CCodexItemListSlot::DataSet( SNATIVEID sidItem )
{
	m_sidItem = sidItem;

	DataReset();

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( m_sidItem );
	if ( pitem_data )
	{
		if ( m_pTextName )		
			m_pTextName->SetText( pitem_data->GetName(), COMMENT::ITEMCOLOR[pitem_data->sBasicOp.emLevel] );

		if ( m_pTextPrice )		
		{
			CString strPrice = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT( (LONGLONG)pitem_data->sBasicOp.dwBuyPrice, 3, "," );
			CString strCombine;
			strCombine.Format( ID2GAMEWORD( "RAN_CODEX_SALESITEM_ITEM_TEXT", 0 ), strPrice.GetString() );	
			m_pTextPrice->SetText( strCombine.GetString(), NS_UITEXTCOLOR::GOLD );
		}

		if ( m_pTextPoint )		
		{
			DWORD dwActivity = GLGaeaClient::GetInstance().GetCharacter()->m_dwActivityPoint;
			DWORD dwActivityReq = pitem_data->sBasicOp.dwReqActivityPoint;

			CString strCombine;
			strCombine.Format( ID2GAMEWORD( "RAN_CODEX_SALESITEM_ITEM_TEXT", 1 ), dwActivityReq );	
			
			if ( dwActivity >= dwActivityReq )
				m_pTextPoint->SetText( strCombine.GetString(), NS_UITEXTCOLOR::WHITE );
			else
				m_pTextPoint->SetText( strCombine.GetString(), NS_UITEXTCOLOR::RED );
		}

		if ( m_pItemImage )
		{
			m_pItemImage->SetItem( pitem_data->sBasicOp.sICONID, pitem_data->GetInventoryFile() );

			if ( pitem_data->ISPILE() )
			{
				CString strNum;
				strNum.Format( "%d", pitem_data->GETAPPLYNUM() );
				m_pItemImage->SetTextDownRight( strNum.GetString(), NS_UITEXTCOLOR::GOLD );
			}
		}
	}	
}