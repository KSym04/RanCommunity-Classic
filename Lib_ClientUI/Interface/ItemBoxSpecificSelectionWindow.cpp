#include "StdAfx.h"
#include "./ItemBoxSpecificSelectionWindow.h"

#include "./InnerInterface.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./ItemImage.h"
#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CItemBoxSpecificSelectionWindow::CItemBoxSpecificSelectionWindow ()
	: m_pui_item_main_name(NULL)
	, m_pui_mouse_over(NULL)
	, m_pui_item_image_selected(NULL)
	, m_pui_item_name_selected(NULL)
	, m_pui_button_open(NULL)
	, m_pui_button_close(NULL)

	, m_item_id(false)
	, m_item_id_selected(false)
	, m_item_selected_num(0)
	, m_item_id_selected_old(false)
	, m_item_selected_num_old(0)
	, m_item_selected_index(-1)
{
	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_pui_item_image_box[i] = NULL;
		m_item_box_num[i] = 0;

		m_item_id_box[i] = NATIVEID_NULL();
	}
}

CItemBoxSpecificSelectionWindow::~CItemBoxSpecificSelectionWindow ()
{
}

void CItemBoxSpecificSelectionWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pui_linebox = new CBasicLineBox;
	pui_linebox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pui_linebox->CreateBaseBoxQuestList ( "ITEM_BOX_SPECIFIC_SELECTION_WINDOW_LINE" );
	RegisterControl ( pui_linebox );

	m_pui_item_main_name = new CBasicTextBox;
	m_pui_item_main_name->CreateSub ( this, "ITEM_BOX_SPECIFIC_SELECTION_ITEM_NAME" );
	m_pui_item_main_name->SetFont ( pFont9 );
	m_pui_item_main_name->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pui_item_main_name );

	std::string item_box[ITEM::SBOX::ITEM_SIZE] = 
	{
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_0",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_1",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_2",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_3",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_4",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_5",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_6",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_7",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_8",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_9",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_10",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_11",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_12",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_13",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_14",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_15",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_16",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_17",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_18",
		"ITEM_BOX_SPECIFIC_SELECTION_ITEM_19",
	};

	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_pui_item_image_box[i] = new CItemImage;
		m_pui_item_image_box[i]->CreateSub ( this, item_box[i].c_str(), UI_FLAG_DEFAULT, ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_0 + i );
		m_pui_item_image_box[i]->CreateSubControl ();
		m_pui_item_image_box[i]->CreateTextBoxDownRight();
		RegisterControl ( m_pui_item_image_box[i] );
	}

	m_pui_item_image_selected = new CItemImage;
	m_pui_item_image_selected->CreateSub ( this, "ITEM_BOX_SPECIFIC_SELECTION_ITEM_SELECTED", UI_FLAG_DEFAULT, ITEM_BOX_SPECIFIC_SELECTION_ITEM_SELECTED );
	m_pui_item_image_selected->CreateSubControl ();
	RegisterControl ( m_pui_item_image_selected );

	m_pui_item_name_selected = new CBasicTextBox;
	m_pui_item_name_selected->CreateSub ( this, "ITEM_BOX_SPECIFIC_SELECTION_ITEM_SELECTED_NAME" );
	m_pui_item_name_selected->SetFont ( pFont9 );
	m_pui_item_name_selected->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pui_item_name_selected );

	m_pui_mouse_over = new CUIControl;
	m_pui_mouse_over->CreateSub ( this, "ITEM_BOX_SPECIFIC_SELECTION_ITEM_MOUSE_OVER" );
	RegisterControl ( m_pui_mouse_over );

	m_pui_button_open = new CBasicTextButton;
	m_pui_button_open->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, ITEM_BOX_SPECIFIC_SELECTION_BUTTON_OPEN );
	m_pui_button_open->CreateBaseButton ( "ITEM_BOX_SPECIFIC_SELECTION_BUTTON_OPEN", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("ITEM_BOX_SPECIFIC_SELECTION_WINDOW", 1 ) );
	RegisterControl ( m_pui_button_open );

	m_pui_button_close = new CBasicTextButton;
	m_pui_button_close->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, ITEM_BOX_SPECIFIC_SELECTION_BUTTON_CLOSE );
	m_pui_button_close->CreateBaseButton ( "ITEM_BOX_SPECIFIC_SELECTION_BUTTON_CLOSE", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("ITEM_BOX_SPECIFIC_SELECTION_WINDOW", 2 ) );
	RegisterControl ( m_pui_button_close );
}

void CItemBoxSpecificSelectionWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( m_pui_mouse_over ) m_pui_mouse_over->SetVisibleSingle ( FALSE );

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_item_id_selected_old != m_item_id_selected || m_item_selected_num_old != m_item_selected_num )
	{
		m_item_id_selected_old = m_item_id_selected;
		m_item_selected_num_old = m_item_selected_num;

		if ( m_pui_item_name_selected )
			m_pui_item_name_selected->ClearText();

		if ( m_pui_item_image_selected )
		{
			m_pui_item_image_selected->ResetItem();
			m_pui_item_image_selected->SetTextDownRight( "", NS_UITEXTCOLOR::ORANGE );

			SITEM* pitem_data = GLItemMan::GetInstance().GetItem( m_item_id_selected );
			if ( pitem_data )
			{
				CString strText;
				strText.Format( "%u", m_item_selected_num );
				m_pui_item_image_selected->SetItem( pitem_data->sBasicOp.sICONID, pitem_data->GetInventoryFile() );

				if (m_item_selected_num > 1 )
					m_pui_item_image_selected->SetTextDownRight( strText.GetString(), NS_UITEXTCOLOR::ORANGE );

				if ( m_pui_item_name_selected )
					m_pui_item_name_selected->AddText( pitem_data->GetName(), COMMENT::ITEMCOLOR[pitem_data->sBasicOp.emLevel] );
			}
		}
	}
	
}

void CItemBoxSpecificSelectionWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) ){
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}break;

	case ITEM_BOX_SPECIFIC_SELECTION_BUTTON_CLOSE:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().HideGroup( GetWndID());
		break;

	case ITEM_BOX_SPECIFIC_SELECTION_BUTTON_OPEN:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			selection_open();
		break;

	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_0:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_1:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_2:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_3:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_4:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_5:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_6:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_7:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_8:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_9:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_10:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_11:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_12:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_13:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_14:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_15:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_16:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_17:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_18:
	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_19:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = ControlID - ITEM_BOX_SPECIFIC_SELECTION_ITEM_BOX_0;

				if ( nIndex < ITEM::SBOX::ITEM_SIZE )
				{
					if( m_item_id_box[nIndex] != NATIVEID_NULL() )
					{
						SITEMCUSTOM sitemcustom;
						sitemcustom.sNativeID = m_item_id_box[nIndex];
						sitemcustom.wTurnNum = (WORD)m_item_box_num[nIndex];

						CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE );

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

						if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
						{
							if ( dwMsg & UIMSG_LB_UP )
							{
								CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
								return;
							}
						}

						if ( m_pui_mouse_over && m_pui_item_image_box[nIndex] )
						{
							const UIRECT& rcImagePos = m_pui_item_image_box[nIndex]->GetGlobalPos();
							m_pui_mouse_over->SetGlobalPos ( rcImagePos );
							m_pui_mouse_over->SetVisibleSingle ( TRUE );
						}

						if( dwMsg & UIMSG_LB_UP )
						{
							m_item_id_selected = sitemcustom.sNativeID;
							m_item_selected_num = sitemcustom.wTurnNum;

							m_item_selected_index = nIndex;
						}
					}
				}
			}
		}break;


	case ITEM_BOX_SPECIFIC_SELECTION_ITEM_SELECTED:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if( m_item_id_selected != NATIVEID_NULL() )
				{
					SITEMCUSTOM sitemcustom;
					sitemcustom.sNativeID = m_item_id_selected;
					sitemcustom.wTurnNum = (WORD)m_item_selected_num;

					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE );

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

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
							return;
						}
					}

					if ( m_pui_mouse_over && m_pui_item_image_selected )
					{
						const UIRECT& rcImagePos = m_pui_item_image_selected->GetGlobalPos();
						m_pui_mouse_over->SetGlobalPos ( rcImagePos );
						m_pui_mouse_over->SetVisibleSingle ( TRUE );
					}
				}

			}
		}break;
	};
}

void CItemBoxSpecificSelectionWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}

void CItemBoxSpecificSelectionWindow::reset_info()
{
	m_item_id = NATIVEID_NULL();
	m_item_id_selected = NATIVEID_NULL();
	m_item_selected_num = 0;
	m_item_id_selected_old = NATIVEID_NULL();
	m_item_selected_num_old = 0;
	m_item_selected_index = -1;

	if ( m_pui_item_main_name )
		m_pui_item_main_name->ClearText();

	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_item_id_box[i] = NATIVEID_NULL();
		m_item_box_num[i] = 0;

		if ( m_pui_item_image_box[i] )	
		{
			m_pui_item_image_box[i]->ResetItem();
			m_pui_item_image_box[i]->SetTextDownRight( "", NS_UITEXTCOLOR::ORANGE );
		}
	}

	if ( m_pui_item_image_selected )
	{
		m_pui_item_image_selected->ResetItem();
		m_pui_item_image_selected->SetTextDownRight( "", NS_UITEXTCOLOR::ORANGE );
	}

	if ( m_pui_item_name_selected )
		m_pui_item_name_selected->ClearText();
}

void CItemBoxSpecificSelectionWindow::selection_show( SITEM* pitem_data )
{
	reset_info();

	if ( !pitem_data )	return;

	m_item_id = pitem_data->sBasicOp.sNativeID;

	if ( m_pui_item_main_name )
		m_pui_item_main_name->AddText( pitem_data->GetName(), COMMENT::ITEMCOLOR[pitem_data->sBasicOp.emLevel] );

	for( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		m_item_id_box[i] = pitem_data->sBox.sITEMS[i].nidITEM;
		m_item_box_num[i] = pitem_data->sBox.sITEMS[i].dwAMOUNT;

		SITEM* pitem_data = GLItemMan::GetInstance().GetItem( m_item_id_box[i] );
		if ( !pitem_data )	continue;;

		if ( m_pui_item_image_box[i] )
		{
			CString strText;
			strText.Format( "%u", m_item_box_num[i] );
			m_pui_item_image_box[i]->SetItem( pitem_data->sBasicOp.sICONID, pitem_data->GetInventoryFile() );

			if ( m_item_box_num[i] > 1 )
				m_pui_item_image_box[i]->SetTextDownRight( strText.GetString(), NS_UITEXTCOLOR::ORANGE );
		}
	}
}

void CItemBoxSpecificSelectionWindow::selection_open()
{
	if ( m_item_selected_index < 0 )						return;
	if ( m_item_selected_index >= ITEM::SBOX::ITEM_SIZE )	return;
	if ( m_item_id_selected == NATIVEID_NULL() )			return;

	SITEM* pitem_data_main = GLItemMan::GetInstance().GetItem( m_item_id );
	if ( !pitem_data_main )		return;

	SITEM* pitem_data_selected = GLItemMan::GetInstance().GetItem( m_item_id_selected );
	if ( !pitem_data_selected )	return;

	if ( pitem_data_main->sBox.sITEMS[m_item_selected_index].nidITEM != m_item_id_selected )
		return;

	GLGaeaClient::GetInstance().GetCharacter()->item_box_specific_select_item( m_item_id, m_item_selected_index, m_item_id_selected );

	CInnerInterface::GetInstance().HideGroup( GetWndID() );
}
