#include "StdAfx.h"
#include "GachaPage.h"
#include "GachaPageRewardSlot.h"

#include "../Lib_Engine/Core/NSRGameGlobal.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../ItemImage.h"
#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../BasicTextBoxEx.h"
#include "../BasicScrollBarEx.h"
#include "../BasicScrollThumbFrame.h"
#include "../BasicButton.h"
#include "../BasicTextButton.h"
#include "../BasicProgressBar.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"
#include "../Lib_Client/G-Logic/GLItem.h"

#include "../Lib_Client/DxGlobalStage.h"

#include "../InnerInterface.h"
#include "../SkillImage.h"

#include "../Lib_ClientUI/Interface/ModalCallerID.h"
#include "../Lib_ClientUI/Interface/ModalWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGachaPage::CGachaPage ()	
	: m_pTextGachaTitle(NULL)
	, m_pTextIntroductionTitle(NULL)
	, m_pTextCostTitle(NULL)
	, m_pTextIntroductionText(NULL)
	, m_pButtonOpenGacha(NULL)

	, m_pGachaItemImage(NULL)
	, m_pTextGachaItemName(NULL)

	, m_pGachaRewardItemImage(NULL)
	, m_pTextGachaRewardItemName(NULL)

	, m_pGachaGuaranteedRewardItemImage(NULL)
	, m_pTextGachaGuaranteedRewardItemName(NULL)
	
	, m_fRefreshTimer(0.0f)
	, m_fLimitTimer(0.0f)
	, m_nSelected(0)
	, m_bDraw(false)

	, m_pConfirmButton(NULL)
	, m_pAutoButton(NULL)

	, m_pTextConfirm(NULL)
	, m_pTextAuto(NULL)
	, m_sidItem(NATIVEID_NULL())
	, m_sidItemMainReward(NATIVEID_NULL())
	, m_sidItemGuaranteedReward(NATIVEID_NULL())

	, m_pEXP ( NULL )
	, m_pEXPText ( NULL )
{
}

CGachaPage::~CGachaPage ()
{
	for( int i=0; i<GACHA_REWARD_SLOT_MAX; ++i )
	{
		m_pGachaRewardPageSlot[i] = NULL;
	}
}

void CGachaPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CreateLineBox();

	//left
	m_pTextGachaTitle = new CBasicTextBox;
	m_pTextGachaTitle->CreateSub ( this, "GACHA_WINDOW_GACHA_TITLE" );
	m_pTextGachaTitle->SetFont ( pFont9 );
	m_pTextGachaTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGachaTitle->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 1 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextGachaTitle );


	//right up
	m_pTextIntroductionTitle = new CBasicTextBox;
	m_pTextIntroductionTitle->CreateSub ( this, "GACHA_WINDOW_INTRODUCTION_TITLE" );
	m_pTextIntroductionTitle->SetFont ( pFont9 );
	m_pTextIntroductionTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIntroductionTitle->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 2 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextIntroductionTitle );

	m_pTextIntroductionText = new CBasicTextBox;
	m_pTextIntroductionText->CreateSub ( this, "GACHA_WINDOW_INTRODUCTION_TEXT" );
	m_pTextIntroductionText->SetFont ( pFont9 );
	m_pTextIntroductionText->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextIntroductionText->SetText( ID2GAMEWORD( "GACHA_WINDOW_TEXT", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextIntroductionText );

	m_pTextIntroductionText->AddText( ID2GAMEWORD( "GACHA_WINDOW_TEXT", 1 ), NS_UITEXTCOLOR::WHITE );
	m_pTextIntroductionText->AddText( ID2GAMEWORD( "GACHA_WINDOW_TEXT", 2 ), NS_UITEXTCOLOR::WHITE );

	//right down
	m_pTextCostTitle = new CBasicTextBox;
	m_pTextCostTitle->CreateSub ( this, "GACHA_WINDOW_COST_TITLE" );
	m_pTextCostTitle->SetFont ( pFont9 );
	m_pTextCostTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextCostTitle->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 3 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextCostTitle );

	m_pConfirmButton = new CBasicButton;
	m_pConfirmButton->CreateSub ( this, "CONFIRM_DRAW_BUTTON", UI_FLAG_DEFAULT, CONFIRM_DRAW_CHECK_BUTTON );
	m_pConfirmButton->CreateFlip ( "CONFIRM_DRAW_BUTTON_ON", CBasicButton::RADIO_FLIP );
	m_pConfirmButton->SetControlNameEx ( "CONFIRM_DRAW_BUTTON" );
	m_pConfirmButton->SetFlip( RGAME_GLOBAL::bConfirmDraw );
	RegisterControl ( m_pConfirmButton );

	m_pAutoButton = new CBasicButton;
	m_pAutoButton->CreateSub ( this, "AUTO_DRAW_BUTTON", UI_FLAG_DEFAULT, AUTO_DRAW_CHECK_BUTTON );
	m_pAutoButton->CreateFlip ( "AUTO_DRAW_BUTTON_ON", CBasicButton::RADIO_FLIP );
	m_pAutoButton->SetControlNameEx ( "AUTO_DRAW_BUTTON" );
	m_pAutoButton->SetFlip( RGAME_GLOBAL::bAutoDraw );
	RegisterControl ( m_pAutoButton );

	m_pTextConfirm = new CBasicTextBox;
	m_pTextConfirm->CreateSub ( this, "GACHA_WINDOW_CONFIRM_TEXT" );
	m_pTextConfirm->SetFont ( pFont9 );
	m_pTextConfirm->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextConfirm->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 5 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextConfirm );

	m_pTextAuto = new CBasicTextBox;
	m_pTextAuto->CreateSub ( this, "GACHA_WINDOW_AUTO_TEXT" );
	m_pTextAuto->SetFont ( pFont9 );
	m_pTextAuto->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextAuto->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 6 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextAuto );

	m_pGachaItemImage = new CItemImage;
	m_pGachaItemImage->CreateSub ( this, "GACHA_ITEM_SLOT_IMAGE", UI_FLAG_DEFAULT, GACHA_ITEM_SLOT_IMAGE );
	m_pGachaItemImage->CreateSubControl ();
	m_pGachaItemImage->SetUseRender ( TRUE );
	m_pGachaItemImage->CreateTextBoxDownRight();
    RegisterControl ( m_pGachaItemImage );

	m_pTextGachaItemName = new CBasicTextBox;
	m_pTextGachaItemName->CreateSub ( this, "GACHA_ITEM_NAME_TEXT" );
	m_pTextGachaItemName->SetFont ( pFont9 );
	m_pTextGachaItemName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGachaItemName->SetText( "" , NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextGachaItemName );

	m_pGachaRewardItemImage = new CItemImage;
	m_pGachaRewardItemImage->CreateSub ( this, "GACHA_REWARD_ITEM_SLOT_IMAGE", UI_FLAG_DEFAULT, GACHA_REWARD_ITEM_SLOT_IMAGE );
	m_pGachaRewardItemImage->CreateSubControl ();
	m_pGachaRewardItemImage->SetUseRender ( TRUE );
	m_pGachaRewardItemImage->CreateTextBoxDownRight();
    RegisterControl ( m_pGachaRewardItemImage );

	m_pTextGachaRewardItemName = new CBasicTextBox;
	m_pTextGachaRewardItemName->CreateSub ( this, "GACHA_REWARD_ITEM_NAME_TEXT" );
	m_pTextGachaRewardItemName->SetFont ( pFont9 );
	m_pTextGachaRewardItemName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGachaRewardItemName->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 8 ) , NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextGachaRewardItemName );


	//Guaranteed reward

	m_pGachaGuaranteedRewardItemImage = new CItemImage;
	m_pGachaGuaranteedRewardItemImage->CreateSub ( this, "GACHA_GUARANTEED_REWARD_ITEM_SLOT_IMAGE", UI_FLAG_DEFAULT, GACHA_GUARANTEED_REWARD_ITEM_SLOT_IMAGE );
	m_pGachaGuaranteedRewardItemImage->CreateSubControl ();
	m_pGachaGuaranteedRewardItemImage->SetUseRender ( TRUE );
	m_pGachaGuaranteedRewardItemImage->CreateTextBoxDownRight();
    RegisterControl ( m_pGachaGuaranteedRewardItemImage );

	m_pTextGachaGuaranteedRewardItemName = new CBasicTextBox;
	m_pTextGachaGuaranteedRewardItemName->CreateSub ( this, "GACHA_GUARANTEED_REWARD_ITEM_NAME_TEXT" );
	m_pTextGachaGuaranteedRewardItemName->SetFont ( pFont9 );
	m_pTextGachaGuaranteedRewardItemName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGachaGuaranteedRewardItemName->SetText( ID2GAMEWORD( "GACHA_WINDOW_TITLE", 8 ) , NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextGachaGuaranteedRewardItemName );

	m_pButtonOpenGacha = new CBasicTextButton;
	m_pButtonOpenGacha->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, OPEN_GACHA );
	m_pButtonOpenGacha->CreateBaseButton ( "GACHA_OPEN_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("GACHA_WINDOW_TITLE", 4 ) );
	RegisterControl ( m_pButtonOpenGacha );

	std::string strslot[GACHA_REWARD_SLOT_MAX] = 
	{
		"GACHA_REWARD_LIST_SLOT0",
		"GACHA_REWARD_LIST_SLOT1",
		"GACHA_REWARD_LIST_SLOT2",
		"GACHA_REWARD_LIST_SLOT3",
		"GACHA_REWARD_LIST_SLOT4",
		"GACHA_REWARD_LIST_SLOT5",
		"GACHA_REWARD_LIST_SLOT6",
		"GACHA_REWARD_LIST_SLOT7",
		"GACHA_REWARD_LIST_SLOT8",
		"GACHA_REWARD_LIST_SLOT9",
		"GACHA_REWARD_LIST_SLOT10",
		"GACHA_REWARD_LIST_SLOT11",
		"GACHA_REWARD_LIST_SLOT12",
		"GACHA_REWARD_LIST_SLOT13",
		"GACHA_REWARD_LIST_SLOT14",
		"GACHA_REWARD_LIST_SLOT15",
		"GACHA_REWARD_LIST_SLOT16",
		"GACHA_REWARD_LIST_SLOT17",
		//"GACHA_REWARD_LIST_SLOT18",
		//"GACHA_REWARD_LIST_SLOT19",
	};
	for( int i=0; i<GACHA_REWARD_SLOT_MAX; ++i )
	{
		m_pGachaRewardPageSlot[i] = new CGachaPageRewardSlot;
		m_pGachaRewardPageSlot[i]->CreateSub( this, strslot[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, GACHA_REWARD_PAGE_SLOT_0 + i );
		m_pGachaRewardPageSlot[i]->CreateSubControl ();
		m_pGachaRewardPageSlot[i]->SetVisibleSingle( TRUE );
		RegisterControl ( m_pGachaRewardPageSlot[i] );
	}

	m_pEXP = new CBasicProgressBar;
	m_pEXP->CreateSub ( this, "GACHA_EXP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pEXP->CreateOverImage ( "GACHA_EXP_OVERIMAGE" );
	m_pEXP->SetControlNameEx ( "EXP" );
	RegisterControl ( m_pEXP );

	m_pEXPText = new CBasicTextBox;
	m_pEXPText->CreateSub ( this, "GACHA_EXP_TEXT", TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
    m_pEXPText->SetFont ( pFont9 );
	m_pEXPText->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pEXPText->SetControlNameEx ( "EXP" );	
	RegisterControl ( m_pEXPText );
}
CUIControl* CGachaPage::CreateUIControl( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pControl->SetVisibleSingle( TRUE );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextBox* CGachaPage::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}
CBasicTextBox* CGachaPage::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CGachaPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
		case OPEN_GACHA:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
					{	
						//m_bDraw = true;
						CString strCombine = CInnerInterface::GetInstance().MakeString ( "Are you sure you want to draw?" );
						if( RGAME_GLOBAL::bConfirmDraw )	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_OPEN_GACHA );
						else	GLGaeaClient::GetInstance().GetCharacter()->ReqOpenGacha(GLCONST_CHAR::nidGachaItem, false);
					}
			}
		}break;
		case CONFIRM_DRAW_CHECK_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RGAME_GLOBAL::bConfirmDraw = !RGAME_GLOBAL::bConfirmDraw;
				m_pConfirmButton->SetFlip( RGAME_GLOBAL::bConfirmDraw );
			}
		}break;
		case AUTO_DRAW_CHECK_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				RGAME_GLOBAL::bAutoDraw = !RGAME_GLOBAL::bAutoDraw;
				m_pAutoButton->SetFlip( RGAME_GLOBAL::bAutoDraw );
			}
		}break;
		case GACHA_ITEM_SLOT_IMAGE:
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
		case GACHA_REWARD_ITEM_SLOT_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sidItemMainReward;

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

		case GACHA_GUARANTEED_REWARD_ITEM_SLOT_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sidItemGuaranteedReward;

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

void CGachaPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );


	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( pCharacter )	SetEXP(pCharacter->m_wGachaPromisePoint);
	if ( m_bDraw )
	{
		m_fLimitTimer+= fElapsedTime;
		m_fRefreshTimer+= fElapsedTime;
		if ( m_fRefreshTimer >= 0.05f)
		{
			ResetRewardSlot();
			//int nRandom = rand() % GACHA_REWARD_SLOT_MAX;
			//m_pGachaRewardPageSlot[nRandom]->SetOverVisible();
			m_nSelected++;
			if ( m_nSelected >= GACHA_REWARD_SLOT_MAX ) m_nSelected = 0;
			m_pGachaRewardPageSlot[m_nSelected]->SetOverVisible();
			m_fRefreshTimer = 0.0f;
		}
		if ( m_fLimitTimer >= 3.0f || RGAME_GLOBAL::bAutoDraw )
		{
			m_fLimitTimer = 0;
			m_bDraw = false;
			GLGaeaClient::GetInstance().GetCharacter()->ReqOpenGacha(GLCONST_CHAR::nidGachaItem, true);
		}
	}
}

void CGachaPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		LoadData();
	}
}
void CGachaPage::ResetRewardSlot()
{
	for (int i = 0; i<GACHA_REWARD_SLOT_MAX; i++ )
	{
		m_pGachaRewardPageSlot[i]->SetOverVisibleOFF();
	}
}
void CGachaPage::ResetData()
{
	
}
void CGachaPage::SetItem( WORD nSelected )
{
	ResetRewardSlot();
	m_pGachaRewardPageSlot[nSelected]->SetOverVisible();

	SITEM* pItem = GLItemMan::GetInstance().GetItem(GLCONST_CHAR::nidGachaItem);
	if ( !pItem ) return;
	if (pItem->sBasicOp.emItemType == ITEM_GACHA_COUPON)
	{

		for (DWORD i = 0; i<pItem->sRandomBox.vecBOX.size(); ++i)
		{
			SITEMCUSTOM sCUSTOM;
			ITEM::SRANDOMITEM sITEMBOX = pItem->sRandomBox.vecBOX[i];
			sCUSTOM.sNativeID = sITEMBOX.nidITEM;
			if ( i >= GACHA_REWARD_SLOT_MAX ) continue;
			if ( i == nSelected ) 
			{
				m_sidItemMainReward = sITEMBOX.nidITEM;
				break;
			}
		}

	}

	SITEM* pItemReward = GLItemMan::GetInstance().GetItem(m_sidItemMainReward);
	if ( !pItemReward ) return;
	if ( m_pTextGachaRewardItemName )
	{
		m_pTextGachaRewardItemName->SetText( pItemReward->GetName(), NS_UITEXTCOLOR::ORANGE);
	}

	m_pGachaRewardItemImage->SetItem ( pItemReward->sBasicOp.sICONID, pItemReward->GetInventoryFile() );
	m_pGachaRewardItemImage->SetVisibleSingle( TRUE );

}
void CGachaPage::LoadData()
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem(GLCONST_CHAR::nidGachaItem);
	if (!pItem)
	{
		return;
	}
	m_sidItem = pItem->sBasicOp.sNativeID;
	if ( m_pTextGachaItemName )
	{
		m_pTextGachaItemName->SetText( pItem->GetName(), NS_UITEXTCOLOR::ORANGE);
		m_pTextGachaItemName->AddText(  ID2GAMEWORD( "GACHA_WINDOW_TITLE", 7 ), NS_UITEXTCOLOR::WHITE);
	}

	m_pGachaItemImage->SetItem ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
	m_pGachaItemImage->SetVisibleSingle( TRUE );

	if (pItem->sBasicOp.emItemType == ITEM_GACHA_COUPON)
	{

		for (DWORD i = 0; i<pItem->sRandomBox.vecBOX.size(); ++i)
		{
			SITEMCUSTOM sCUSTOM;
			ITEM::SRANDOMITEM sITEMBOX = pItem->sRandomBox.vecBOX[i];
			sCUSTOM.sNativeID = sITEMBOX.nidITEM;

			if (sCUSTOM.sNativeID == SNATIVEID(false))				continue;
			if ( i >= GACHA_REWARD_SLOT_MAX ) continue;

			if ( i==0 )	m_sidItemGuaranteedReward = sITEMBOX.nidITEM;
			
			SITEM* pItemData = GLItemMan::GetInstance().GetItem(sCUSTOM.sNativeID);
			if (pItemData)
			{
				m_pGachaRewardPageSlot[i]->DataSet(sITEMBOX.nidITEM, sITEMBOX.fRATE);
			}
		}
	}

	SITEM* pItemReward = GLItemMan::GetInstance().GetItem(m_sidItemGuaranteedReward);
	if ( !pItemReward ) return;
	if ( m_pTextGachaGuaranteedRewardItemName )
	{
		m_pTextGachaGuaranteedRewardItemName->SetText( pItemReward->GetName(), NS_UITEXTCOLOR::ORANGE);
	}

	m_pGachaGuaranteedRewardItemImage->SetItem ( pItemReward->sBasicOp.sICONID, pItemReward->GetInventoryFile() );
	m_pGachaGuaranteedRewardItemImage->SetVisibleSingle( TRUE );
}
void CGachaPage::CreateLineBox()
{
	//back
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhite ( "GACHA_OBJECT_LINE" );
	RegisterControl ( pLineBox );


	//left side (summon)
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_MAIN_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_MAIN_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_MAIN_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_MAIN_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_MAIN_CONTENT" );
	RegisterControl ( pLineBox );

	//right side upper
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_SUB_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_SUB_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_SUB_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_SUB_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_SUB_CONTENT" );
	RegisterControl ( pLineBox );

	//right side lower
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_SUBDOWN_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_SUBDOWN_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_LINEBOX_SUBDOWN_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_SUBDOWN_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_LINEBOX_SUBDOWN_CONTENT" );
	RegisterControl ( pLineBox );


	//Reward Image linebox
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "GACHA_REWARD_ITEM_SLOT_IMAGE_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "GACHA_REWARD_ITEM_SLOT_IMAGE_LINEBOX" );
	RegisterControl ( pLineBox );
}
void CGachaPage::SetEXP( WORD wExp )
{
	{

		CString strTemp;
		float fPercent(0.0f);
		if( wExp )
		{
			fPercent = float(wExp) / float(GLCONST_CHAR::wGachaMaxPoint);	
		}

		strTemp.Format ( "Promise Points: %d/%d", wExp, GLCONST_CHAR::wGachaMaxPoint );
		m_pEXPText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::WHITE );

		m_pEXP->SetPercent ( fPercent );
	}
}