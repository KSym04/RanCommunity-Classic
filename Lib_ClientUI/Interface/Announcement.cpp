#include "StdAfx.h"
#include "Announcement.h"

#include "BasicTextButton.h"
#include "BasicTextBox.h"
#include "BasicLineBox.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"

#include "UIEditBoxMan.h"
#include "../Lib_Engine/GUInterface/UIKeyCheck.h"

#include "BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "../Lib_Engine/Common/SUBPATH.h"
#include "InnerInterface.h"
#include "../Lib_Engine/Common/BaseString.h"
#include "../Lib_Engine/G-Logic/GLogic.h"

#include "OuterInterface.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "ModalWindow.h"
#include "OuterInterface.h"
#include "../Lib_Network/s_NetClient.h"
#include "../Lib_Client/DxGlobalStage.h"
CAnnouncementWindow::CAnnouncementWindow ()
: m_pInfo ( NULL )
, m_pScrollBar( NULL )
{
}

CAnnouncementWindow::~CAnnouncementWindow ()
{
}

void CAnnouncementWindow::CreateSubControl()
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE18;
	CUIEditBox* pEditBox = NULL;
	CBasicLineBox* pBasicLineBox = NULL;

	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	
	DWORD dwFontColor = NS_UITEXTCOLOR::DEFAULT;
	int nAlign = TEXT_ALIGN_LEFT;

	//	배경
	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxWhiteBlankBody ( "ANNOUNCEMENT_WINDOW_BACK_BOX" );
	RegisterControl ( pBasicLineBox );

	// 확인 버튼 
	CBasicTextButton* pOKButton = new CBasicTextButton;
	pOKButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHAT_MACRO_OK );
	pOKButton->CreateBaseButton ( "ANNOUNCEMENT_WINDOW_OKBUTTON", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "CHATMACRO_OKBUTTON", 0 ) );
	RegisterControl ( pOKButton );


	
	m_pInfo = new CBasicTextBox;
	m_pInfo->CreateSub ( this, "ANNOUNCEMENT_INFO_BOX" );
	m_pInfo->SetFont ( m_pFont );
	m_pInfo->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pInfo->AddText( ID2GAMEEXTEXT ("ANNOUNCEMENT_INFO"), NS_UITEXTCOLOR::DEFAULT );
	RegisterControl ( m_pInfo );

	int nTotalLine = m_pInfo->GetVisibleLine ();

	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, ANNOUNCEMENT_SCROLLBAR );
	pScrollBar->CreateBaseScrollBar ( "ANNOUNCEMENT_SCROLL_BAR" );
	pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( pScrollBar );
	m_pScrollBar = pScrollBar;

	

}
void CAnnouncementWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIOuterWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_pInfo->SetTextAlign ( TEXT_ALIGN_LEFT );

	if (m_pScrollBar)
	{

		CBasicScrollThumbFrame* const pThumbFrame = m_pScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pInfo->GetTotalLine ();
		const int nLinePerOneView = m_pInfo->GetVisibleLine ();	
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pInfo->SetCurLine ( nPos );			
		}


	}

}
void CAnnouncementWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
		switch ( ControlID )
		{
		case ET_CONTROL_TITLE:
		case ET_CONTROL_TITLE_F:
			{
				if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
				{
					//COuterInterface::GetInstance().SetDefaultPosInterface( ANNOUNCEMENT_WINDOW );
				}
				break;
			}			
		case CHAT_MACRO_OK:
			{
				if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					
					CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
					if ( !pNetClient->IsOnline() )
					{	
						DoModalOuter ( ID2GAMEEXTEXT("SERVERSTAGE_4"), MODAL_QUESTION, OKCANCEL, OUTER_MODAL_CLOSEGAME );
						COuterInterface::GetInstance().HideGroup ( GetWndID () );
					}
					else
					{
						COuterInterface::GetInstance().HideGroup ( GetWndID () );
					}
				}
				break;
			}		
		case ET_CONTROL_BUTTON:
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					COuterInterface::GetInstance().HideGroup ( GetWndID () );
				}
				break;
			}
		}
	
	CUIOuterWindow::TranslateUIMessage ( ControlID, dwMsg );
}
