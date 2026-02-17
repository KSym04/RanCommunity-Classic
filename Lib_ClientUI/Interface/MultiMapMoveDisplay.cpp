#include "StdAfx.h"
#include "./MultiMapMoveDisplay.h"
#include "./BasicLineBox.h"
#include "./BasicLineBoxEx.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxOctree/DxLandGateMan.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/DxGlobalStage.h"

#include "../../Lib_Network/s_CRandomNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMultiMapMoveDisplay::CMultiMapMoveDisplay ()
	: m_pLineBoxBody(NULL)
	, m_pLineBoxList(NULL)
	, m_pTextBoxInfo(NULL)
	, m_pButtonMoveOK(NULL)
	, m_pScrollBarList(NULL)
{
	for ( int i=0; i<MAX_SELECTION; ++i )
	{
		m_pLineBoxSelection[i] = NULL;
		m_pTextBoxSelection[i] = NULL;
		m_dwSelectionIndex[i] = DxLandGate::GATE_OUT_INDEX_NULL;
	}

	m_dwSelected = 0;
}

CMultiMapMoveDisplay::~CMultiMapMoveDisplay ()
{
}

void CMultiMapMoveDisplay::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBoxBody = new CBasicLineBoxEx;
	m_pLineBoxBody->CreateSub ( this, "BASIC_LINE_BOX_PARTY_INFO", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxBody->CreateBaseBoxPartyInfoLineBox ( "MULTIMAPMOVE_WINDOW_BOX" );
	RegisterControl ( m_pLineBoxBody );	

	m_pLineBoxList = new CBasicLineBoxEx;
	m_pLineBoxList->CreateSub ( this, "BASIC_LINE_BOX_PARTY_INFO", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxList->CreateBaseBoxPartyInfoLineBox ( "MULTIMAPMOVE_LIST_BOX" );
	RegisterControl ( m_pLineBoxList );

	m_pTextBoxInfo = new CBasicTextBox;
	m_pTextBoxInfo->CreateSub ( this, "MULTIMAPMOVE_INFORMATION_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
    m_pTextBoxInfo->SetFont ( pFont9 );
	m_pTextBoxInfo->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextBoxInfo->SetText( ID2GAMEINTEXT( "MULTI_GATE_INFORMATIOIN_TEXT"), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextBoxInfo );

	m_pButtonMoveOK = new CBasicTextButton;
	m_pButtonMoveOK->CreateSub ( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, MULTIMAPMOVE_WINDOW_OK_BUTTON );
	m_pButtonMoveOK->CreateBaseButton ( "MULTIMAPMOVE_WINDOW_OK_BUTTON", CBasicTextButton::SIZE14, CBasicButton::CLICK_FLIP, ID2GAMEWORD ( "MAPMOVE_OKBUTTON" ) );
	m_pButtonMoveOK->SetShortcutKey ( DIK_SPACE );
	RegisterControl ( m_pButtonMoveOK );

	std::string strSelectionBox[CMultiMapMoveDisplay::MAX_SELECTION] = 
	{
		"MULTIMAPMOVE_SELECTBOX_0",
		"MULTIMAPMOVE_SELECTBOX_1",
		"MULTIMAPMOVE_SELECTBOX_2",
		"MULTIMAPMOVE_SELECTBOX_3",
	};

	std::string strSelectionText[CMultiMapMoveDisplay::MAX_SELECTION] = 
	{
		"MULTIMAPMOVE_TEXTBOX_0",
		"MULTIMAPMOVE_TEXTBOX_1",
		"MULTIMAPMOVE_TEXTBOX_2",
		"MULTIMAPMOVE_TEXTBOX_3",
	};

	for ( int i=0; i<MAX_SELECTION; ++i )
	{
		m_pLineBoxSelection[i] = new CBasicLineBoxEx;
		m_pLineBoxSelection[i]->CreateSub ( this, "BASIC_LINE_BOX_PARTY_INFO_SELECTBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pLineBoxSelection[i]->CreateBaseBoxPartyInfoSelectBox ( (char*)strSelectionBox[i].c_str() );
		RegisterControl ( m_pLineBoxSelection[i] );	

		m_pTextBoxSelection[i] = new CBasicTextBox;
		m_pTextBoxSelection[i]->CreateSub ( this, strSelectionText[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE, MULTIMAPMOVE_LIST_SELECT_0 + i );
		m_pTextBoxSelection[i]->SetFont ( pFont9 );
		m_pTextBoxSelection[i]->SetTextAlign ( TEXT_ALIGN_CENTER_Y );
		m_pTextBoxSelection[i]->SetText( "selection", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pTextBoxSelection[i] );
	}

	m_pScrollBarList = new CBasicScrollBarEx;
	m_pScrollBarList->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, MULTIMAPMOVE_LIST_SCROLLBAR );
	m_pScrollBarList->CreateBaseScrollBar ( "MULTIMAPMOVE_LIST_SCROLLBAR" );
	m_pScrollBarList->GetThumbFrame()->SetState ( 1, CMultiMapMoveDisplay::MAX_SELECTION );
	RegisterControl ( m_pScrollBarList );
}

void CMultiMapMoveDisplay::DoMAPMOVE ()
{
	GLGaeaClient::GetInstance().GetCharacter()->ReqGateOut( m_dwSelected );
}

void CMultiMapMoveDisplay::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case MULTIMAPMOVE_WINDOW_OK_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) || CHECK_KEYFOCUSED ( dwMsg ) )
			{
				DoMAPMOVE ();
			}
		}
        break;

	case MULTIMAPMOVE_LIST_SELECT_0:
	case MULTIMAPMOVE_LIST_SELECT_1:
	case MULTIMAPMOVE_LIST_SELECT_2:
	case MULTIMAPMOVE_LIST_SELECT_3:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				int nSelected = ControlID - MULTIMAPMOVE_LIST_SELECT_0;
				if ( nSelected >= CMultiMapMoveDisplay::MAX_SELECTION ) return;
				m_dwSelected = nSelected;
			}
		}break;

	}
}

void CMultiMapMoveDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pScrollBarList )
	{
		int nNum = (int)m_vecData.size();
		int nTotal = m_pScrollBarList->GetThumbFrame()->GetTotal ();
		float fPercent = m_pScrollBarList->GetThumbFrame()->GetPercent ();

		if ( nNum != nTotal )
		{
			m_pScrollBarList->GetThumbFrame()->SetState ( nNum, CMultiMapMoveDisplay::MAX_SELECTION );
			nTotal = m_pScrollBarList->GetThumbFrame()->GetTotal ();
			fPercent = m_pScrollBarList->GetThumbFrame()->GetPercent ();
		}

		BOOL bEnableScrollBar = ( nNum >= CMultiMapMoveDisplay::MAX_SELECTION );
		m_pScrollBarList->SetVisibleSingle( bEnableScrollBar );

		int nIncrease = 0;
		if ( CMultiMapMoveDisplay::MAX_SELECTION < nTotal )
		{
			nIncrease = (int)floor(fPercent * nTotal);
			if ( nIncrease + CMultiMapMoveDisplay::MAX_SELECTION >= nTotal )
			{
				nIncrease = nTotal - CMultiMapMoveDisplay::MAX_SELECTION;
			}
		}

		//CDebugSet::ToView( 20, "%d", nIncrease );

		for( int i=0; i<CMultiMapMoveDisplay::MAX_SELECTION; ++i )
		{
			if ( m_pLineBoxSelection[i] )
				m_pLineBoxSelection[i]->SetVisibleSingle( FALSE );

			if ( m_pTextBoxSelection[i] )
				m_pTextBoxSelection[i]->ClearText();

			m_dwSelectionIndex[i] = DxLandGate::GATE_OUT_INDEX_NULL;
		}

		for( int i=0; i<CMultiMapMoveDisplay::MAX_SELECTION; ++i )
		{
			int nIndex = nIncrease + i;

			if ( nIndex >= (int) m_vecData.size() )	continue;

			MULTI_MAP_MOVE_SELECT_DATA& sData = m_vecData[nIndex];

			if ( m_dwSelected == i )
			{
				if ( m_pTextBoxSelection[i] )
					m_pTextBoxSelection[i]->SetText(sData.strText.c_str(), NS_UITEXTCOLOR::ORANGE );

				if ( m_pLineBoxSelection[i] )
					m_pLineBoxSelection[i]->SetVisibleSingle( TRUE );
			}
			else
			{
				if ( m_pTextBoxSelection[i] )
					m_pTextBoxSelection[i]->SetText(sData.strText.c_str(), NS_UITEXTCOLOR::WHITE );
			}
			
			m_dwSelectionIndex[i] = sData.dwIndex;
		}
	}
	
}

void CMultiMapMoveDisplay::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );
	if ( bVisible )
	{
	}
	else
	{
		ResetSelectionData();
	}
}

void CMultiMapMoveDisplay::SetSelectionData()
{
	if ( m_vecData.size() == 0 )
	{
		DWORD dwGateID = GLGaeaClient::GetInstance().GetCharacter()->DetectGate ();
		if ( dwGateID==UINT_MAX )	
			return;

		PLANDMANCLIENT pLandMClient = GLGaeaClient::GetInstance().GetActiveMap();
		if ( !pLandMClient )								
			return;

		DxLandGateMan *pLandGateMan = &pLandMClient->GetLandGateMan();
		if ( !pLandGateMan )								
			return;

		PDXLANDGATE pLandGate = pLandGateMan->FindLandGate ( dwGateID );
		if ( !pLandGate )									
			return;

		if ( pLandGate->GetNewSystem() )
		{
			for ( DWORD i = 0; i<pLandGate->GetToMapNum(); ++i )
			{
				SNATIVEID sToMapID = pLandGate->GetToMapID(i);
				SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( sToMapID );
				if ( pMapNode )
				{
					DWORD dwNameINDEX = pLandGate->GetNameINDEX(i);
					std::string strNameKEY = pLandGate->GetNameKEY(i);

					MULTI_MAP_MOVE_SELECT_DATA sData;
					sData.dwIndex = i;

					if ( dwNameINDEX != DxLandGate::GATE_OUT_INDEX_NULL && strNameKEY.size() )
						sData.strText = ID2GAMEWORD( strNameKEY.c_str(), dwNameINDEX );

					if ( sData.strText.empty() )
						sData.strText = pMapNode->strMapName.c_str();

					
					m_vecData.push_back( sData );
				}
			}
		}
	}
}

void CMultiMapMoveDisplay::ResetSelectionData()
{
	m_vecData.clear();

	for( int i=0; i<CMultiMapMoveDisplay::MAX_SELECTION; ++i )
	{
		if ( m_pLineBoxSelection[i] )
			m_pLineBoxSelection[i]->SetVisibleSingle( FALSE );

		if ( m_pTextBoxSelection[i] )
			m_pTextBoxSelection[i]->ClearText();

		m_dwSelectionIndex[i] = DxLandGate::GATE_OUT_INDEX_NULL;
	}

	m_dwSelected = 0;
}