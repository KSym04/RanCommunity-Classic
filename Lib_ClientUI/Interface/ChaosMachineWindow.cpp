#include "stdafx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLChaosMachine.h"

#include "./BasicLineBox.h"
#include "./ItemImage.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./ItemMove.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"


#include "./ChaosMachineWindow.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CChaosMachineWindow::CChaosMachineWindow ()
	: m_pItemImageMain(NULL)
	, m_pMainItemInfo(NULL)
	, m_pMainItemCost(NULL)
	, m_pMainItemRate(NULL)
	, m_pReqItemInfo(NULL)
	, m_pStartButton(NULL)
	, m_pCancelButton(NULL)
{
	for( int i=0; i<CHAOS_MACHINE_MATERIAL_NUM; ++i )
	{
		m_pItemImageRequire[i] = NULL;
		m_pItemReqCountText[i] = NULL;
	}
}

CChaosMachineWindow::~CChaosMachineWindow ()
{
}

void CChaosMachineWindow::CreateSubControl ()
{
	CD3DFontPar* pFont08 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite ( "CHAOS_MACHINE_WINDOW_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHAOS_MACHINE_MAIN_ITEM_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHAOS_MACHINE_REQ_ITEM_LINE" );
	RegisterControl ( pBasicLineBox );


	m_pItemImageMain = new CItemImage;
	m_pItemImageMain->CreateSub( this, "CHAOS_MACHINE_ITEM_IMAGE_MAIN", UI_FLAG_DEFAULT, CHAOS_MACHINE_ITEM_IMAGE_MAIN );
	m_pItemImageMain->CreateSubControl();
	RegisterControl( m_pItemImageMain );


	std::string strItemSlotReq[CHAOS_MACHINE_MATERIAL_NUM] = 
	{
		"CHAOS_MACHINE_ITEM_IMAGE_REQ_0",
		"CHAOS_MACHINE_ITEM_IMAGE_REQ_1",
		"CHAOS_MACHINE_ITEM_IMAGE_REQ_2",
	};

	std::string strItemSlotReqLine[CHAOS_MACHINE_MATERIAL_NUM] = 
	{
		"CHAOS_MACHINE_REQ_ITEM_0_COUNT_LINE",
		"CHAOS_MACHINE_REQ_ITEM_1_COUNT_LINE",
		"CHAOS_MACHINE_REQ_ITEM_2_COUNT_LINE",
	};

	for ( int i=0; i<CHAOS_MACHINE_MATERIAL_NUM; ++i )
	{
		m_pItemImageRequire[i] = new CItemImage;
		m_pItemImageRequire[i]->CreateSub( this, strItemSlotReq[i].c_str(), UI_FLAG_DEFAULT, CHAOS_MACHINE_ITEM_IMAGE_REQ_0 + i );
		m_pItemImageRequire[i]->CreateSubControl();
		RegisterControl( m_pItemImageRequire[i] );

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxQuestList ( (char*)strItemSlotReqLine[i].c_str() );
		RegisterControl ( pBasicLineBox );


		m_pItemReqCountText[i] = new CBasicTextBox;
		m_pItemReqCountText[i]->CreateSub ( this, strItemSlotReqLine[i].c_str(), UI_FLAG_DEFAULT );
		m_pItemReqCountText[i]->SetFont ( pFont08 );
		m_pItemReqCountText[i]->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
		m_pItemReqCountText[i]->SetText( "0/0", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pItemReqCountText[i] );


	}


	m_pMainItemInfo = new CBasicTextBox;
	m_pMainItemInfo->CreateSub ( this, "CHAOS_MACHINE_MAIN_ITEM_INFO", UI_FLAG_DEFAULT );
	m_pMainItemInfo->SetFont ( pFont08 );
	m_pMainItemInfo->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pMainItemInfo->SetText( "", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pMainItemInfo );

	m_pMainItemCost = new CBasicTextBox;
	m_pMainItemCost->CreateSub ( this, "CHAOS_MACHINE_MAIN_ITEM_COST", UI_FLAG_DEFAULT );
	m_pMainItemCost->SetFont ( pFont08 );
	m_pMainItemCost->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pMainItemCost->SetText( "", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pMainItemCost );

	m_pMainItemRate = new CBasicTextBox;
	m_pMainItemRate->CreateSub ( this, "CHAOS_MACHINE_MAIN_ITEM_RATE", UI_FLAG_DEFAULT );
	m_pMainItemRate->SetFont ( pFont08 );
	m_pMainItemRate->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pMainItemRate->SetText( "", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pMainItemRate );

	m_pReqItemInfo = new CBasicTextBox;
	m_pReqItemInfo->CreateSub ( this, "CHAOS_MACHINE_REQ_ITEM_INFO", UI_FLAG_DEFAULT );
	m_pReqItemInfo->SetFont ( pFont08 );
	m_pReqItemInfo->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pReqItemInfo->SetText( "", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pReqItemInfo );


	m_pStartButton = new CBasicTextButton;
	m_pStartButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHAOS_MACHINE_BUTTON_START );
	m_pStartButton->CreateBaseButton ( "CHAOS_MACHINE_WINDOW_BUTTON_START", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("CHAOS_MACHINE_WINDOW", 1 ) );
	RegisterControl ( m_pStartButton );

	m_pCancelButton = new CBasicTextButton;
	m_pCancelButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHAOS_MACHINE_BUTTON_CANCEL );
	m_pCancelButton->CreateBaseButton ( "CHAOS_MACHINE_WINDOW_BUTTON_CANCEL", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, ID2GAMEWORD("CHAOS_MACHINE_WINDOW", 2 ) );
	RegisterControl ( m_pCancelButton );
}


void CChaosMachineWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	UpdateInfo();
}

void CChaosMachineWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case CHAOS_MACHINE_BUTTON_CANCEL:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseChaosMachineWindow();
		break;

	case ET_CONTROL_BUTTON:
		if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			CInnerInterface::GetInstance().CloseChaosMachineWindow();
		break;

	case CHAOS_MACHINE_ITEM_IMAGE_MAIN:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )	break;

				SNATIVEID sItem = pItemMove->GetItem();
				if( sItem != NATIVEID_NULL() )
				{
					const UIRECT& rcSlotPos = m_pItemImageMain->GetGlobalPos();
					pItemMove->SetGlobalPos( rcSlotPos );
					CInnerInterface::GetInstance().SetSnapItem();
				}

				if( dwMsg & UIMSG_LB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->SetChaosItemMoveMainItem();
				}

				if( dwMsg & UIMSG_RB_UP )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ResetChaosItemMoveMainItem();
				}

				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GetChaosMachineItem();
				if( sItemCustom.sNativeID != NATIVEID_NULL() )
					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, 0, 0 );

				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sItemCustom );
						return;
					}
				}

				///*box contents, Juver, 2017/08/30 */
				//if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				//{
				//	if ( dwMsg & UIMSG_RB_UP )
				//	{
				//		CInnerInterface::GetInstance().ShowBoxContents( sItemCustom.sNativeID );
				//		return;
				//	}
				//}

// 				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
// 				{
// 					if ( dwMsg & UIMSG_LB_UP )
// 					{
// 						CInnerInterface::GetInstance().ShowMaxRV( sItemCustom );
// 						return;
// 					}
// 				}
			}
		}break;

	case CHAOS_MACHINE_ITEM_IMAGE_REQ_0:
	case CHAOS_MACHINE_ITEM_IMAGE_REQ_1:
	case CHAOS_MACHINE_ITEM_IMAGE_REQ_2:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nSelect = ControlID - CHAOS_MACHINE_ITEM_IMAGE_REQ_0;

				if ( nSelect >= CHAOS_MACHINE_MATERIAL_NUM )	return;

				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GetChaosMachineItem();
				SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
				if ( pItem )
				{
					SCHAOS_MACHINE_DATA* pChaosMachineData = GLChaosMachine::GetInstance().GetData( pItem->sBasicOp.dwChaosMachineID );
					if ( pChaosMachineData )
					{
						SITEMCUSTOM sitemcustom;
						sitemcustom.sNativeID = pChaosMachineData->sMaterial[nSelect].sItemID;
						sitemcustom.wTurnNum = pChaosMachineData->sMaterial[nSelect].wNum;

						if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

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

						///*box contents, Juver, 2017/08/30 */
						//if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
						//{
						//	if ( dwMsg & UIMSG_RB_UP )
						//	{
						//		CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						//		return;
						//	}
						//}

// 						if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
// 						{
// 							if ( dwMsg & UIMSG_LB_UP )
// 							{
// 								CInnerInterface::GetInstance().ShowMaxRV( sItemCustom );
// 								return;
// 							}
// 						}
					}
				}
			}
			
		}break;

	case CHAOS_MACHINE_BUTTON_START:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				ChaosMachineStart();
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

}

void CChaosMachineWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
	else
	{
	}
}

void CChaosMachineWindow::UpdateInfo()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )	return;

	CString strTemp;

	//reset
	m_pItemImageMain->ResetItem();
	m_pMainItemInfo->SetText( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 0 ), NS_UITEXTCOLOR::ORNAGERED );
	m_pMainItemCost->ClearText();
	m_pMainItemRate->ClearText();
	m_pReqItemInfo->ClearText();

	for ( int i=0; i<CHAOS_MACHINE_MATERIAL_NUM; ++i )
	{
		m_pItemImageRequire[i]->ResetItem();

		strTemp.Format( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 3 ), 0, 0 );
		m_pItemReqCountText[i]->SetText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	
	const SITEMCUSTOM sitemcustom = pCharacter->GetChaosMachineItem();
	SITEM* pItem = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
	if ( pItem )
	{
		m_pItemImageMain->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
		
		SCHAOS_MACHINE_DATA* pChaosMachineData = GLChaosMachine::GetInstance().GetData( pItem->sBasicOp.dwChaosMachineID );
		if ( pChaosMachineData )
		{
			if ( pChaosMachineData->bConsumeMainItem )
				m_pMainItemInfo->SetText( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 7 ), NS_UITEXTCOLOR::WHITE );
			else
				m_pMainItemInfo->SetText( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 8 ), NS_UITEXTCOLOR::WHITE );
			

			strTemp.Format( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 1 ), pChaosMachineData->llCost );
			m_pMainItemCost->SetText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );

			strTemp.Format( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 2 ), pChaosMachineData->fChance /** 100.0f*/ );
			m_pMainItemRate->SetText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );

			BOOL bReqFailed = FALSE;

			for ( int i=0; i<CHAOS_MACHINE_MATERIAL_NUM; ++i )
			{
				SITEM* pItemReq = GLItemMan::GetInstance().GetItem( pChaosMachineData->sMaterial[i].sItemID );
				if ( pItemReq )
				{
					m_pItemImageRequire[i]->SetItem( pItemReq->sBasicOp.sICONID, pItemReq->GetInventoryFile() );

					DWORD dwReqCount = pChaosMachineData->sMaterial[i].wNum;

					BOOL bCompleteReq = FALSE;

					if ( pItemReq->ISPILE() )
					{
						DWORD dwItemNum = pCharacter->m_cInventory.CountTurnItem( pItemReq->sBasicOp.sNativeID );
						bCompleteReq = BOOL( dwItemNum >= dwReqCount );

						strTemp.Format( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 3 ), dwItemNum, dwReqCount );
						m_pItemReqCountText[i]->SetText( strTemp.GetString(), bCompleteReq? NS_UITEXTCOLOR::BRIGHTGREEN : NS_UITEXTCOLOR::RED );
					}
					else
					{
						SINVENITEM* pinvenitem = pCharacter->m_cInventory.FindItem( pItemReq->sBasicOp.sNativeID );
						if ( pinvenitem )
						{
							bCompleteReq = TRUE;
						}

						strTemp.Format( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 3 ), bCompleteReq? dwReqCount:0, dwReqCount );
						m_pItemReqCountText[i]->SetText( strTemp.GetString(), bCompleteReq? NS_UITEXTCOLOR::BRIGHTGREEN : NS_UITEXTCOLOR::RED );
					}

					if ( !bCompleteReq )	
						bReqFailed = TRUE;

					
				}
			}

			if ( bReqFailed )
				m_pReqItemInfo->SetText( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 4 ), NS_UITEXTCOLOR::RED );
			else
				m_pReqItemInfo->SetText( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 5 ), NS_UITEXTCOLOR::BRIGHTGREEN );

		}
		else
		{
			m_pMainItemInfo->SetText( ID2GAMEWORD( "CHAOS_MACHINE_WINDOW_INFO", 6 ), NS_UITEXTCOLOR::ORNAGERED );
		}
	}
	

}

void CChaosMachineWindow::ChaosMachineStart()
{
	const SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GetChaosMachineItem();

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_INVALID_ITEM") );
		return;
	}

	SCHAOS_MACHINE_DATA* pChaosMachineData = GLChaosMachine::GetInstance().GetData( pItem->sBasicOp.dwChaosMachineID );
	if ( !pChaosMachineData )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_INVALID_ITEM") );
		return;
	}

	if ( sItemCustom.IsGM_GENITEM() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_NON_DROP") );
		return;
	}

	if ( pChaosMachineData->llCost != 0 && GLGaeaClient::GetInstance().GetCharacter()->m_lnMoney < pChaosMachineData->llCost ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_COST") );
		return;
	}

	GLGaeaClient::GetInstance().GetCharacter()->ChaosMachineStart();
}