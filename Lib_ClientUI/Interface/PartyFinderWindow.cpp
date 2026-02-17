#include "StdAfx.h"
#include "./PartyFinderWindow.h"

#include "./PartyFinderSlot.h"

#include "./BasicLineBoxEx.h"
#include "./BasicTextBoxEx.h"
#include "./BasicTextBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPartyFinder.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Helper/CommonContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPartyFinderWindow::CPartyFinderWindow () 
	: m_nSelectIndexType(-1)
	, m_nCurrentPage(1)
	, m_nMaxPage(1)
	, m_dwSelectedIndex(SPARTY_FINDER_SEARCH_INDEX_NULL)
	, m_pListTextBox(NULL)
	, m_pListScrollBar(NULL)
	, m_pButtonSearch(NULL)
	, m_pButtonPrev(NULL)
	, m_pButtonNext(NULL)
	, m_pTextPage(NULL)
{
	for( int i=0; i<PARTY_FINDER_MAX_SLOT; ++i )
	{
		m_pSlot[i] = NULL;
	}
}

CPartyFinderWindow::~CPartyFinderWindow ()
{
}

void CPartyFinderWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_SEARCH_REGION_LINEBOX" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_RESULT_REGION_LINEBOX" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_SEARCH_REGION_LINEBOX_HEAD" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_RESULT_REGION_LINEBOX_HEAD" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "PARTY_FINDER_RESULT_REGION_LINEBOX_PAGES" );
	RegisterControl ( pBasicLineBox );


	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "PARTY_FINDER_SEARCH_REGION_TEXT_HEAD" );
	pTextBox->SetFont ( pFont9 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetText( ID2GAMEWORD( "PARTY_FINDER_TEXT", 0), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pTextBox );

	pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "PARTY_FINDER_RESULT_REGION_TEXT_HEAD" );
	pTextBox->SetFont ( pFont9 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetText( ID2GAMEWORD( "PARTY_FINDER_TEXT", 1), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pTextBox );


	m_pListTextBox = new CBasicTextBoxEx;
	m_pListTextBox->CreateSub ( this, "PARTY_FINDER_SEARCH_REGION_TEXT_SELECT", UI_FLAG_DEFAULT, PARTY_FINDER_SELECTION_TEXTBOX );
	m_pListTextBox->SetFont ( pFont9 );		
	m_pListTextBox->SetLineInterval ( 3.0f );
	m_pListTextBox->SetSensitive ( true );
	m_pListTextBox->SetLimitLine ( 10000 );
	RegisterControl ( m_pListTextBox );

	int nTotalLine = m_pListTextBox->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, PARTY_FINDER_SELECTION_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "PARTY_FINDER_SEARCH_REGION_SCROLL_SELECT" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );

	m_pButtonSearch = new CBasicTextButton;
	m_pButtonSearch->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, PARTY_FINDER_BUTTON_SEARCH );
	m_pButtonSearch->CreateBaseButton ( "PARTY_FINDER_SEARCH_BUTTON_SEARCH", CBasicTextButton::SIZE40, CBasicButton::CLICK_FLIP, ID2GAMEWORD("PARTY_FINDER_TEXT", 2) );
	RegisterControl ( m_pButtonSearch );


	std::string strSlot[PARTY_FINDER_MAX_SLOT] = 
	{
		"PARTY_FINDER_SLOT_00",
		"PARTY_FINDER_SLOT_01",
		"PARTY_FINDER_SLOT_02",
		"PARTY_FINDER_SLOT_03",
		"PARTY_FINDER_SLOT_04",
		"PARTY_FINDER_SLOT_05",
		"PARTY_FINDER_SLOT_06",
		"PARTY_FINDER_SLOT_07",
	};

	for( int i=0; i<PARTY_FINDER_MAX_SLOT; ++i )
	{
		m_pSlot[i] = new CPartyFinderSlot;
		m_pSlot[i]->CreateSub ( this, strSlot[i].c_str(), UI_FLAG_DEFAULT, PARTY_FINDER_SLOT_00 + i );
		m_pSlot[i]->CreateSubControl ();
		m_pSlot[i]->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pSlot[i] );
	}

	m_pButtonPrev = new CBasicButton;
	m_pButtonPrev->CreateSub ( this, "PARTY_FINDER_RESULT_REGION_PRE_PAGE_BUTTON", UI_FLAG_DEFAULT, PARTY_FINDER_BUTTON_PREV );
	m_pButtonPrev->CreateFlip ( "PARTY_FINDER_RESULT_REGION_PRE_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonPrev->CreateMouseOver ( "PARTY_FINDER_RESULT_REGION_PRE_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonPrev );

	m_pButtonNext = new CBasicButton;
	m_pButtonNext->CreateSub ( this, "PARTY_FINDER_RESULT_REGION_NEXT_PAGE_BUTTON", UI_FLAG_DEFAULT, PARTY_FINDER_BUTTON_NEXT );
	m_pButtonNext->CreateFlip ( "PARTY_FINDER_RESULT_REGION_NEXT_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonNext->CreateMouseOver ( "PARTY_FINDER_RESULT_REGION_NEXT_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonNext );

	m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "PARTY_FINDER_RESULT_REGION_CUR_PAGE_TEXT" );
	m_pTextPage->SetFont ( pFont9 );
	m_pTextPage->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPage->SetText( "0" );
	RegisterControl ( m_pTextPage );
}

void CPartyFinderWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListTextBox->SetUseOverColor ( FALSE );
	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndexType )
		m_pListTextBox->SetUseTextColor ( m_nSelectIndexType, FALSE );

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndexType )
	{
		m_pListTextBox->SetUseTextColor ( m_nSelectIndexType, TRUE );
		m_pListTextBox->SetTextColor ( m_nSelectIndexType, NS_UITEXTCOLOR::ORANGE );
	}

	CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();
	const int nTotalLine = m_pListTextBox->GetTotalLine ();
	const int nLinePerOneView = m_pListTextBox->GetVisibleLine ();
	pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
	if ( nLinePerOneView < nTotalLine )
	{
		const int nMovableLine = nTotalLine - nLinePerOneView;
		float fPercent = pThumbFrame->GetPercent ();
		int nPos = (int)floor(fPercent * nMovableLine);
		m_pListTextBox->SetCurLine ( nPos );			
	}

	if ( m_pButtonSearch )
	{
		float fPartyFinderDelayTimer = GLGaeaClient::GetInstance().GetCharacter()->m_fPartyFinderSearchDelayTimer;
		BOOL bCooldown = BOOL( fPartyFinderDelayTimer < RPARAM::fPartyFinderSearchDelay );
	
		m_pButtonSearch->SetVisibleSingle( !bCooldown );
	}
}

void CPartyFinderWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case PARTY_FINDER_SELECTION_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pListTextBox->GetSelectPos ();
				if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0  ) return ;

				if ( UIMSG_LB_UP & dwMsg )
				{
					DWORD dwType = m_pListTextBox->GetTextData ( nIndex );
					m_nSelectIndexType = nIndex;
					SelectType( dwType );
				}

				m_pListTextBox->SetUseOverColor ( TRUE );
				m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
			}
		}
		break;

	case PARTY_FINDER_SLOT_00:
	case PARTY_FINDER_SLOT_01:
	case PARTY_FINDER_SLOT_02:
	case PARTY_FINDER_SLOT_03:
	case PARTY_FINDER_SLOT_04:
	case PARTY_FINDER_SLOT_05:
	case PARTY_FINDER_SLOT_06:
	case PARTY_FINDER_SLOT_07:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - PARTY_FINDER_SLOT_00;
				if ( m_pSlot[nSelect] )
				{
					if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
					{
						SelectSlot(nSelect);
					}
				}
			}
		}break;

	case PARTY_FINDER_BUTTON_PREV:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_nCurrentPage --;
					if ( m_nCurrentPage < 1 )
						m_nCurrentPage = 1;
					ShowSlot( m_nCurrentPage );
				}
			}
		}break;

	case PARTY_FINDER_BUTTON_NEXT:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					if ( m_nCurrentPage >= m_nMaxPage )	return;

					m_nCurrentPage ++;
					ShowSlot( m_nCurrentPage );
				}
			}
		}break;

	case PARTY_FINDER_BUTTON_SEARCH:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					SearchParty();
				}
			}
		}break;
	};
}

void CPartyFinderWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		InitData();
	}
	else
	{
		ResetData();
	}
}

void CPartyFinderWindow::InitData()
{
	ResetData();

	SPARTY_FINDER_MAP_DATA_MAP& mapData = GLPartyFinder::GetInstance().m_mapPartyFinderMap;

	SPARTY_FINDER_MAP_DATA_MAP_ITER it_b = mapData.begin();
	SPARTY_FINDER_MAP_DATA_MAP_ITER it_e = mapData.end();
	for ( ; it_b != it_e; ++it_b )
	{
		const SPARTY_FINDER_MAP_DATA& sData = (*it_b).second;

		int nIndex = m_pListTextBox->AddText ( sData.strMapName.c_str(), NS_UITEXTCOLOR::WHITE );
		m_pListTextBox->SetTextData ( nIndex, sData.dwIndex );
	}

	//select first line 
	if ( m_pListTextBox->GetCount () > 0 )
	{
		m_pListTextBox->SetSelectPos( 0 );
		int nIndex = m_pListTextBox->GetSelectPos ();
		if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0 ) return;
		DWORD dwType = m_pListTextBox->GetTextData ( nIndex );
		m_nSelectIndexType = nIndex;
		SelectType( dwType );
		m_pListTextBox->SetUseOverColor ( TRUE );
		m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
	}

	UpdateSearchResult();
}

void CPartyFinderWindow::ResetData()
{
	common_vector_free( m_vecResult );

	m_dwSearchType = 0;

	m_nCurrentPage = 1;
	m_nSelectIndexType = -1;

	m_nMaxPage = 1;
	
	m_dwSelectedIndex = SPARTY_FINDER_SEARCH_INDEX_NULL;

	if ( m_pListTextBox )
		m_pListTextBox->ClearText();

	for ( int i=0; i<PARTY_FINDER_MAX_SLOT; ++i )
	{
		if( m_pSlot[i] )
		{
			m_pSlot[i]->DataReset();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}

	if ( m_pTextPage )
	{
		CString strTemp;
		strTemp.Format( "%d", m_nCurrentPage );
		m_pTextPage->ClearText();
		m_pTextPage->SetText( strTemp.GetString() );
	}
}

void CPartyFinderWindow::ShowSlot( int nPage )
{
	for ( int i=0; i< PARTY_FINDER_MAX_SLOT; ++ i )
	{
		if( m_pSlot[i] )
		{
			m_pSlot[i]->DataReset();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}

	SelectSlot( PARTY_FINDER_MAX_SLOT );

	if ( nPage < 1 )	return;

	int nEnd = nPage * PARTY_FINDER_MAX_SLOT;
	int nStart = nEnd - PARTY_FINDER_MAX_SLOT;
	int nSlot = 0;

	for ( int i=nStart; i<nEnd; ++i )
	{
		if ( i >= m_vecResult.size() )	continue;

		if ( m_pSlot[nSlot] )
		{
			m_pSlot[nSlot]->SetVisibleSingle( TRUE );
			m_pSlot[nSlot]->DataSet( &m_vecResult[i] );
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

void CPartyFinderWindow::SelectSlot( int nSelect )
{
	for ( int i=0; i< PARTY_FINDER_MAX_SLOT; ++ i )
	{
		if ( m_pSlot[i] )
			m_pSlot[i]->SlotSelect( FALSE );
	}

	if ( nSelect >= 0 && nSelect < PARTY_FINDER_MAX_SLOT )
	{
		if ( m_pSlot[nSelect] )
		{
			if ( m_pSlot[nSelect]->IsVisible() )
				m_pSlot[nSelect]->SlotSelect( TRUE );
		}
	}
}

void CPartyFinderWindow::SelectType( DWORD dwIndex )
{
	m_dwSelectedIndex = dwIndex;
}

void CPartyFinderWindow::SearchParty()
{
	GLGaeaClient::GetInstance().GetCharacter()->PartyFinderSearch( m_dwSelectedIndex );
}

void CPartyFinderWindow::UpdateSearchResult()
{
	common_vector_free( m_vecResult );
	m_vecResult = GLGaeaClient::GetInstance().m_vecPartyFinderResult;

	m_nMaxPage = 1 + ( (int)m_vecResult.size() / PARTY_FINDER_MAX_SLOT );
	m_nCurrentPage = 1;

	ShowSlot( m_nCurrentPage );
}