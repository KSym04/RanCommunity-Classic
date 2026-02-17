#include "StdAfx.h"
#include "./ModernSkillWindow.h"
#include "./ModernSkillPage.h"
#include "./ModernSkillSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextButton.h"
#include "./BasicTextBox.h"
#include "./MultiModeButton.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/DxGlobalStage.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"

#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernSkillWindow::CModernSkillWindow ()
	: m_pRestPoint ( NULL )
	, m_dwSkillPointBACK ( 0 )
	, m_pHelp( NULL )
{
	memset ( m_pPage, 0, sizeof ( m_pPage ) );
	memset ( m_pPageButton, 0, sizeof ( m_pPageButton ) );
}

CModernSkillWindow::~CModernSkillWindow ()
{
}

void CModernSkillWindow::SetVisiblePage ( int nPage )
{
	if ( nPage < SKILL_WINDOW_PAGE0 || SKILL_WINDOW_PAGE3 < nPage )
	{
		GASSERT ( 0 && "CModernSkillWindow::SetVisiblePage incorrect page index" );
		return ;
	}

	for ( int i = 0; i < nMAXPAGE; i++ )
	{
		m_pPage[i]->SetVisibleSingle ( FALSE );
		m_pPageButton[i]->DoImageFlip( FALSE );
	}

	int nIndex = nPage - SKILL_WINDOW_PAGE0;
	m_pPage[nIndex]->SetVisibleSingle ( TRUE );
	m_pPageButton[nIndex]->DoImageFlip( TRUE );
}

void CModernSkillWindow::CreateSubControl ()
{
	EMCHARCLASS emCharClass = DxGlobalStage::GetInstance().GetGameStage()->GetCharJoinData().m_CharData2.m_emClass;
	const int nCLASSTYPE = CharClassToIndex ( emCharClass );
	int newClassType = nCLASSTYPE;

	int nSkillCountAll[EMSKILL_PC_CLASSNUM] = { 0, 0, 0, 0, 0, 0 };

	SNATIVEID sNativeIDArrayAll[EMSKILL_PC_CLASSNUM][GLSkillMan::MAX_CLASSSKILL];
	{	
		for ( int i = 0; i < EMSKILL_PC_CLASSNUM; i ++ )
		{
			for ( int j = 0; j < GLSkillMan::MAX_CLASSSKILL; j++ )
			{
				sNativeIDArrayAll[i][j] = NATIVEID_NULL();
			}
		}

		SortSkill ( emCharClass, sNativeIDArrayAll, nSkillCountAll );
	}

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "RAN_CHARACTER_SKILL_WINDOW_BACK" );
	RegisterControl ( pControl );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxSkill ( "RAN_CHARACTER_SKILL_LIST_REGION" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxSkill ( "RAN_CHARACTER_SKILL_POINT_REGION" );
	RegisterControl ( pLineBox );

	{
		const CString strClassTextKeyword[GLCI_NUM_8CLASS] = 
		{
			"SKILL_TAB_TITLE_FIGHTER",
			"SKILL_TAB_TITLE_ARMS",
			"SKILL_TAB_TITLE_ARCHER",
			"SKILL_TAB_TITLE_SPIRIT",
			"SKILL_TAB_TITLE_EXTREME_M",
			"SKILL_TAB_TITLE_EXTREME_W",
			"SKILL_TAB_TITLE_FIGHTER",
			"SKILL_TAB_TITLE_ARMS",
			"SKILL_TAB_TITLE_ARCHER",
			"SKILL_TAB_TITLE_SPIRIT",
			"SKILL_TAB_TITLE_SCIENTIST",
			"SKILL_TAB_TITLE_SCIENTIST",
			"SKILL_TAB_TITLE_ASSASSIN",
			"SKILL_TAB_TITLE_ASSASSIN",
			"SKILL_TAB_TITLE_TRICKER",
			"SKILL_TAB_TITLE_TRICKER",
		};
		
		CString strClassText = strClassTextKeyword[newClassType ];

		CString	strPageKeyword[nMAXPAGE] = 
		{
			"RAN_CHARACTER_SKILL_TAB_1",
			"RAN_CHARACTER_SKILL_TAB_2",
			"RAN_CHARACTER_SKILL_TAB_3",
			"RAN_CHARACTER_SKILL_TAB_4",
		};		

		for ( int i = 0; i < nMAXPAGE; i++ )
		{			
			m_pPage[i] = CreateSkillPage ( SKILL_WINDOW_PAGE0 + i, sNativeIDArrayAll[i], nSkillCountAll[i] );

			m_pPageButton[i] = CreateTabButton( strPageKeyword[i].GetString(), ID2GAMEWORD ( strClassText.GetString(), i ), SKILL_WINDOW_PAGE_BUTTON0 + i);
		}	
	}

	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	{
		m_pRestPoint = CreateStaticControl ( "RAN_CHARACTER_SKILL_POINT_REGION", pFont9, dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
		CString strTemp;
		strTemp.Format ( "%s:%d", ID2GAMEWORD ( "SKILL_WINDOW_REST_POINT_STATIC", 0 ), 0 );
		m_pRestPoint->AddText ( strTemp );
	}

	m_pHelp = new CBasicButton;
	m_pHelp->CreateSub ( this, "RAN_CHARACTER_SKILL_HELP_POS" , UI_FLAG_DEFAULT, SKILL_WINDOW_HELP );
	m_pHelp->CreateFlip ( "RAN_CHARACTER_SKILL_HELP_POS_F", CBasicButton::MOUSEIN_FLIP );
	m_pHelp->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelp );

	SetVisiblePage ( SKILL_WINDOW_PAGE0 );
}

CMultiModeButton* CModernSkillWindow::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID );
	pButton->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
	pButton->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
	pButton->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pButton->AlignToControl( szButtonBox );
	pButton->SetOneLineText( szButtonText, NS_UITEXTCOLOR::WHITE );
	pButton->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pButton );
	return pButton;
}


CModernSkillPage*	CModernSkillWindow::CreateSkillPage ( UIGUID ControlID, SNATIVEID sNativeIDArray[GLSkillMan::MAX_CLASSSKILL], int nSkillCount )
{
	CModernSkillPage* pSkillPage = new CModernSkillPage;
	pSkillPage->CreateSub ( this, "RAN_CHARACTER_SKILL_LIST_REGION", UI_FLAG_DEFAULT, ControlID );
	pSkillPage->CreateSubControl ( sNativeIDArray, nSkillCount );
	RegisterControl ( pSkillPage );
	return pSkillPage;
}

CUIControl*	CModernSkillWindow::CreateControl ( char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

CBasicTextBox* CModernSkillWindow::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );

	return pStaticText;
}

void CModernSkillWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( GetWndID() );
			}
		}
		break;
	case SKILL_WINDOW_PAGE_BUTTON0:
	case SKILL_WINDOW_PAGE_BUTTON1:
	case SKILL_WINDOW_PAGE_BUTTON2:	
	case SKILL_WINDOW_PAGE_BUTTON3:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					int nIndex = ControlID - SKILL_WINDOW_PAGE_BUTTON0;
					SetVisiblePage ( SKILL_WINDOW_PAGE0 + nIndex );
				}
			}
		}
		break;

	case SKILL_WINDOW_HELP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					CInnerInterface::GetInstance().InfoWindowOpen( GetWndID() );
					CInnerInterface::GetInstance().InfoWindowAddText( ID2GAMEINTEXT("MODERN_CHARACTER_SKILL_TOOLTIP_HELP_TEXT" ), NS_UITEXTCOLOR::DARKLBUE );
				}
			}
		}break;

	}
    
	if ( SKILL_WINDOW_PAGE0 <= ControlID && ControlID <= SKILL_WINDOW_PAGE3 )
	{
		int nIndex = ControlID - SKILL_WINDOW_PAGE0;
		int nSkillIndex = m_pPage[nIndex]->GetSkillIndex ();		

		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{			
			if ( nSkillIndex < 0 ) return;
			SNATIVEID sNativeID = m_pPage[nIndex]->GetSkillID ( nSkillIndex );
			if ( sNativeID != NATIVEID_NULL() )
			{
				if ( dwMsg & MODERN_SKILL_SLOT_UIMSG_MOUSEIN_BUTTON )
				{
					dwMsg &= ~UIMSG_TOTOPPARENT;
					dwMsg &= ~MODERN_SKILL_SLOT_UIMSG_MOUSEIN_BUTTON;
					SetMessageEx ( dwMsg );

					if ( dwMsg & MODERN_SKILL_SLOT_UIMSG_LEVEL_UP )
					{							
						dwMsg &= ~MODERN_SKILL_SLOT_UIMSG_LEVEL_UP;
						SetMessageEx ( dwMsg );
						CInnerInterface::GetInstance().InfoDisplayReset ();
					}						
					
					CInnerInterface::GetInstance().SHOW_SKILL_INFO ( sNativeID, TRUE );
				}
				
				else if ( dwMsg & MODERN_SKILL_SLOT_UIMSG_MOUSEIN_ICON )
				{
					dwMsg &= ~UIMSG_TOTOPPARENT;
					dwMsg &= ~MODERN_SKILL_SLOT_UIMSG_MOUSEIN_ICON;
					SetMessageEx ( dwMsg );

					CInnerInterface::GetInstance().SHOW_SKILL_INFO ( sNativeID, FALSE );
				}
			}
		}
	}
}

void CModernSkillWindow::GetSkillIndexRange ( EMCHARCLASS emCharClass, int* pnBeginIndex, int* pnEndIndex )
{	
	int nBeginIndex = 0;
	int nEndIndex = 0;

	switch ( emCharClass )
	{
	case GLCC_BRAWLER_M:
	case GLCC_BRAWLER_W:
		{
			nBeginIndex = EMSKILL_BRAWLER_01;
			nEndIndex = EMSKILL_BRAWLER_04;
		}
		break;

	case GLCC_SWORDSMAN_M:
	case GLCC_SWORDSMAN_W:
		{
			nBeginIndex = EMSKILL_SWORDSMAN_01;
			nEndIndex = EMSKILL_SWORDSMAN_04;
		}
		break;

	case GLCC_ARCHER_M:
	case GLCC_ARCHER_W:
		{
			nBeginIndex = EMSKILL_ARCHER_01;
			nEndIndex = EMSKILL_ARCHER_04;
		}
		break;

	case GLCC_SHAMAN_M:
	case GLCC_SHAMAN_W:
		{
			nBeginIndex = EMSKILL_SHAMAN_01;
			nEndIndex = EMSKILL_SHAMAN_04;
		}
		break;	

	case GLCC_EXTREME_M:
	case GLCC_EXTREME_W:
		{
			nBeginIndex = EMSKILL_EXTREME_01;
			nEndIndex = EMSKILL_EXTREME_04;
		}
		break;

	case GLCC_GUNNER_M:
	case GLCC_GUNNER_W:
		{
			nBeginIndex = EMSKILL_GUNNER_01;
			nEndIndex = EMSKILL_GUNNER_04;
		}
		break;

	case GLCC_ASSASSIN_M:
	case GLCC_ASSASSIN_W:
		{
			nBeginIndex = EMSKILL_ASSASSIN_01;
			nEndIndex = EMSKILL_ASSASSIN_04;
		}
		break;

	case GLCC_TRICKER_M:
	case GLCC_TRICKER_W:
		{
			nBeginIndex = EMSKILL_TRICKER_01;
			nEndIndex = EMSKILL_TRICKER_04;
		}
		break;

	};

	*pnBeginIndex = nBeginIndex;
	*pnEndIndex = nEndIndex;
}

void CModernSkillWindow::SortSkill ( EMCHARCLASS emCharClass, SNATIVEID sNativeIDArrayAll[EMSKILL_PC_CLASSNUM][GLSkillMan::MAX_CLASSSKILL], int nSkillCountAll[EMSKILL_PC_CLASSNUM] )
{
	int nBeginIndex = 0;
	int nEndIndex = 0;

	GetSkillIndexRange ( emCharClass, &nBeginIndex, &nEndIndex );
	
	WORD wClass, wIndex;
	GLSkillMan::GetInstance().GetMaxSkill ( wClass, wIndex );

	for ( int i = nBeginIndex; i <= nEndIndex; i++ )
	{
		int nSkillCount = 0;
		SNATIVEID sNativeIDArray[GLSkillMan::MAX_CLASSSKILL];
		for ( int k = 0; k < GLSkillMan::MAX_CLASSSKILL; k++ )
		{
			sNativeIDArray[k] = NATIVEID_NULL();
		}
		
		for ( int j = 0; j < wIndex; j++ )
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( i, j );
			if ( pSkill )
			{
				sNativeIDArray[nSkillCount] = SNATIVEID(i,j);
				nSkillCount++;
			}
		}

		SNATIVEID sTempID;
		for ( int x = 0; x < nSkillCount; x++ )
		for ( int y = 0; y < nSkillCount; y++ )
		{
			PGLSKILL pA = GLSkillMan::GetInstance().GetData ( sNativeIDArray[x].wMainID, sNativeIDArray[x].wSubID );
			PGLSKILL pB = GLSkillMan::GetInstance().GetData ( sNativeIDArray[y].wMainID, sNativeIDArray[y].wSubID );
			if ( pA->m_sBASIC.dwGRADE < pB->m_sBASIC.dwGRADE )
			{
				sTempID = sNativeIDArray[x];
				sNativeIDArray[x] = sNativeIDArray[y];
				sNativeIDArray[y] = sTempID;
			}
		}

        const int nIndex = i - nBeginIndex;
		nSkillCountAll[nIndex] = nSkillCount;
		memmove( sNativeIDArrayAll[nIndex], sNativeIDArray, sizeof ( SNATIVEID ) * GLSkillMan::MAX_CLASSSKILL );
	}
}

void CModernSkillWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pRestPoint )
	{
		const DWORD dwSkillPoint = GLGaeaClient::GetInstance().GetCharacter ()->m_dwSkillPoint;
		if ( dwSkillPoint != m_dwSkillPointBACK )
		{
			CString strTemp;
			strTemp.Format ( "%s:%d", ID2GAMEWORD ( "SKILL_WINDOW_REST_POINT_STATIC", 0 ), dwSkillPoint );
			m_pRestPoint->SetText ( strTemp );
			m_dwSkillPointBACK = dwSkillPoint;
		}
	}
}