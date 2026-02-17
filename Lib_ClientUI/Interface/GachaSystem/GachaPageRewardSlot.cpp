#include "StdAfx.h"
#include "GachaPageRewardSlot.h"

#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../BasicTextButton.h"
#include "../ItemImage.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"

#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../InnerInterface.h"

#include "../Lib_Client/G-Logic/GLQuestMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"


#include "../Lib_ClientUI/Interface/ModalCallerID.h"
#include "../Lib_ClientUI/Interface/ModalWindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGachaPageRewardSlot::CGachaPageRewardSlot ()	
	:  m_pTextRate(NULL)
	
	, m_pBlackLineBox(NULL)
	, m_pBlueLineBox(NULL)
	
	, m_pItemImage(NULL)
	, m_sidItem(NATIVEID_NULL())
	, m_fRATE(0.0f)

	//select highlight
	, m_pSelectImage(NULL)
{
}

CGachaPageRewardSlot::~CGachaPageRewardSlot ()
{
}

void CGachaPageRewardSlot::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	
	/*m_pBlueLineBox = new CBasicLineBox;
	m_pBlueLineBox->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_BLUE_BASE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pBlueLineBox->CreateBaseBlueBase ( "GACHA_LINEBOX_REWARD" );
	m_pBlueLineBox->SetVisibleSingle(FALSE);
	RegisterControl ( m_pBlueLineBox );*/
	
	
	m_pBlackLineBox = new CBasicLineBox;
	m_pBlackLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pBlackLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_REWARD" );
	m_pBlackLineBox->SetVisibleSingle(TRUE);
	RegisterControl ( m_pBlackLineBox );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_REWARD" );
	pLineBox->SetVisibleSingle(TRUE);
	RegisterControl ( pLineBox );

	
	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub ( this, "GACHA_SLOT_IMAGE", UI_FLAG_DEFAULT, ITEM_SLOT_IMAGE );
	m_pItemImage->CreateSubControl ();
	m_pItemImage->SetUseRender ( TRUE );
	m_pItemImage->CreateTextBoxDownRight();
    RegisterControl ( m_pItemImage );

	m_pTextRate = new CBasicTextBox;
	m_pTextRate->CreateSub ( this, "GACHA_REWARD_RATE" );
	m_pTextRate->SetFont ( pFont );
	m_pTextRate->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRate->SetText( "", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextRate );

	//Select Highlight
	m_pSelectImage = new CUIControl;
	m_pSelectImage->CreateSub ( this, "GACHA_LINEBOX_REWARD_SELECT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pSelectImage->SetVisibleSingle( FALSE );
	RegisterControl ( m_pSelectImage );
}

void CGachaPageRewardSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ITEM_SLOT_IMAGE:
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
				if ( dwMsg & UIMSG_LB_UP )
				{				
				}
			}
		}
		break;
	};
}

void CGachaPageRewardSlot::DataReset()
{
}

void CGachaPageRewardSlot::DataSet(SNATIVEID sItemID, float fRATE)
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )	return;

	SITEM* pItemData0 = GLItemMan::GetInstance().GetItem ( sItemID );
	if( pItemData0 )
	{
		m_sidItem = sItemID;
		m_fRATE = fRATE;
		m_pItemImage->SetItem ( pItemData0->sBasicOp.sICONID, pItemData0->GetInventoryFile() );
		m_pItemImage->SetVisibleSingle( TRUE );

		CString strText;
		strText.Format ( "%2.2f", m_fRATE );
		m_pTextRate->SetText(strText, NS_UITEXTUTIL::WHITE);

	}
}
void CGachaPageRewardSlot::SetOverVisible()
{
	m_pSelectImage->SetVisibleSingle( TRUE );
}
void CGachaPageRewardSlot::SetOverVisibleOFF()
{
	m_pSelectImage->SetVisibleSingle( FALSE );
}