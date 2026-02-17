#include "StdAfx.h"
#include "./CurrencyShopWindow.h"
#include "./CurrencyShopSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicButton.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/G-Logic/GLCrowData.h"
#include "../../Lib_Client/G-Logic/GLCurrencyShop.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCurrencyShopWindow::CCurrencyShopWindow ()
	: m_sNpcID(false)
	, m_nCurrentPage(1)
	, m_nMaxPage(1)
	, m_pTextPage(NULL)
	, m_pButtonPrev(NULL)
	, m_pButtonNext(NULL)

{
	for ( int i=0; i<CURRENCY_SHOP_MAX_SLOT; ++i )
		m_pSlot[i] = NULL;
}

CCurrencyShopWindow::~CCurrencyShopWindow ()
{
}

void CCurrencyShopWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhite ( "CURRENCY_SHOP_WINDOW_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CURRENCY_SHOP_SLOT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CURRENCY_SHOP_PAGE_LINE" );
	RegisterControl ( pBasicLineBox );

	std::string strSlot[CURRENCY_SHOP_MAX_SLOT] = 
	{
		"CURRENCY_SHOP_SLOT_0",
		"CURRENCY_SHOP_SLOT_1",
		"CURRENCY_SHOP_SLOT_2",
		"CURRENCY_SHOP_SLOT_3",
		"CURRENCY_SHOP_SLOT_4",
		"CURRENCY_SHOP_SLOT_5",
		"CURRENCY_SHOP_SLOT_6",
		"CURRENCY_SHOP_SLOT_7",
		"CURRENCY_SHOP_SLOT_8",
		"CURRENCY_SHOP_SLOT_9",
	};

	for ( int i=0; i<CURRENCY_SHOP_MAX_SLOT; ++i )
	{
		m_pSlot[i] = new CCurrencyShopSlot;
		m_pSlot[i]->CreateSub ( this, strSlot[i].c_str(), UI_FLAG_DEFAULT, CURRENCY_SHOP_ITEM00 + i );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSlot[i] );
	}

	m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "CURRENCY_SHOP_CUR_PAGE_TEXT" );
	m_pTextPage->SetFont ( pFont9 );
	m_pTextPage->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPage->SetText( "0" );
	RegisterControl ( m_pTextPage );

	m_pButtonPrev = new CBasicButton;
	m_pButtonPrev->CreateSub ( this, "CURRENCY_SHOP_PREV_PAGE_BUTTON", UI_FLAG_DEFAULT, CURRENCY_SHOP_PREV_BUTTON );
	m_pButtonPrev->CreateFlip ( "CURRENCY_SHOP_PREV_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonPrev->CreateMouseOver ( "CURRENCY_SHOP_PREV_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonPrev );

	m_pButtonNext = new CBasicButton;
	m_pButtonNext->CreateSub ( this, "CURRENCY_SHOP_NEXT_PAGE_BUTTON", UI_FLAG_DEFAULT, CURRENCY_SHOP_NEXT_BUTTON );
	m_pButtonNext->CreateFlip ( "CURRENCY_SHOP_NEXT_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonNext->CreateMouseOver ( "CURRENCY_SHOP_NEXT_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonNext );
}

void CCurrencyShopWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCurrencyShopWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{

	case CURRENCY_SHOP_PREV_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_nCurrentPage --;
					if ( m_nCurrentPage < 1 )
						m_nCurrentPage = 1;
					ShowPage( m_nCurrentPage );
				}
			}
		}break;

	case CURRENCY_SHOP_NEXT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					if ( m_nCurrentPage >= m_nMaxPage )	return;

					m_nCurrentPage ++;
					ShowPage( m_nCurrentPage );
				}
			}
		}break;
	};
}

void CCurrencyShopWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
	else
	{
		ResetList();
	}
}

void CCurrencyShopWindow::LoadList( SNATIVEID sNpcID )
{
	ResetList();

	SCROWDATA* pCrow = GLCrowDataMan::GetInstance().GetCrowData( sNpcID );
	if ( !pCrow )	return;

	SCURRENCY_SHOP_DATA* pShop = GLCrowDataMan::GetInstance().CurrencyShopFind( pCrow->m_sAction.m_strCurrencyShop );
	if ( !pShop )	return;

	if ( pShop->m_strTitle.size() )
		SetTitleName( pShop->m_strTitle.c_str() );

	m_sNpcID = pCrow->m_sBasic.sNativeID;

	SCURRENCY_SHOP_ITEM_POINTER_MAP_ITER it_b = pShop->m_mapData.begin();
	SCURRENCY_SHOP_ITEM_POINTER_MAP_ITER it_e = pShop->m_mapData.end();

	for ( ; it_b != it_e; ++it_b )
	{
		const SCURRENCY_SHOP_ITEM& sData = *(*it_b).second;
		m_vecList.push_back( sData );
	}

	std::sort( m_vecList.begin(), m_vecList.end() );

	m_nMaxPage = 1 + ( (int)m_vecList.size() / CURRENCY_SHOP_MAX_SLOT );

	m_nCurrentPage = 1;
	ShowPage( m_nCurrentPage );
}

void CCurrencyShopWindow::ResetList()
{
	m_nCurrentPage = 1;
	m_nMaxPage = 1;

	m_vecList.clear();
	m_sNpcID = NATIVEID_NULL();
	
	for ( int i=0; i<CURRENCY_SHOP_MAX_SLOT; ++i )
	{
		m_pSlot[i]->DataReset();
		m_pSlot[i]->SetVisibleSingle( FALSE );
	}	

	if ( m_pTextPage )
	{
		CString strTemp;
		strTemp.Format( "%d", m_nCurrentPage );
		m_pTextPage->ClearText();
		m_pTextPage->SetText( strTemp.GetString() );
	}
}

void CCurrencyShopWindow::ShowPage( int nPage )
{
	for ( int i=0; i< CURRENCY_SHOP_MAX_SLOT; ++ i )
	{
		m_pSlot[i]->DataReset();
		m_pSlot[i]->SetVisibleSingle( FALSE );
	}

	if ( nPage < 1 )	return;

	int nEnd = nPage * CURRENCY_SHOP_MAX_SLOT;
	int nStart = nEnd - CURRENCY_SHOP_MAX_SLOT;
	int nSlot = 0;

	for ( int i=nStart; i<nEnd; ++i )
	{
		if ( i >= (int)m_vecList.size() )	continue;

		if ( m_pSlot[nSlot] )
		{
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->DataSet( m_sNpcID, &m_vecList[i] );
			nSlot ++;
		}
	}

	if ( m_pTextPage )
	{
		CString strTemp;
		strTemp.Format( "%d", nPage );
		m_pTextPage->ClearText();
		m_pTextPage->SetText( strTemp.GetString() );
	}
}

void CCurrencyShopWindow::UpdatePrice()
{
	for ( int i=0; i< CURRENCY_SHOP_MAX_SLOT; ++ i )
	{
		if ( m_pSlot[i] &&  m_pSlot[i]->IsVisible() )
		{
			m_pSlot[i]->UpdatePrice();
		}
	}
}