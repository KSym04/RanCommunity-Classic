#include "StdAfx.h"
#include "./ModernFriendWindowBlockPage.h"
#include "./ModernFriendWindow.h"

#include "./BasicLineBox.h"
#include "./MultiModeButton.h"
#include "./BasicTextBoxEx.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./BasicTextButton.h"
#include "./UIEditBox.h"
#include "./ModalWindow.h"
#include "./UITextControl.h"
#include "./ModalCallerID.h"
#include "./BasicChat.h"
#include "./PartyModalWindow.h"
#include "./ModalCallerID.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"
#include "../../Lib_Engine/Common/DXInputString.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernFriendWindowBlockPage::nLIMIT_CHAR = 16;

CModernFriendWindowBlockPage::CModernFriendWindowBlockPage()
	: m_nSelectIndex( -1 )
	, m_pListText(NULL)
	, m_pListScrollBar(NULL)
	, m_pADD_BLOCK(NULL)
	, m_pDEL_BLOCK(NULL)
	, m_pEditBox(NULL)
{
}

CModernFriendWindowBlockPage::~CModernFriendWindowBlockPage ()
{
}

void CModernFriendWindowBlockPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_LIST_BG" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_LIST_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxEditBox( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_ROW_TOP" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxEditBox( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_ROW_BOTTOM" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_TEXTEDIT_POS_BG" );
	RegisterControl ( pBasicLineBox );

	m_pListText = new CBasicTextBoxEx;
	m_pListText->CreateSub ( this, "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_LIST", UI_FLAG_DEFAULT, FRIEND_LIST_TEXTBOX );
	m_pListText->SetFont ( pFont9 );		
	m_pListText->SetLineInterval ( 3.0f );
	m_pListText->SetSensitive ( true );
	m_pListText->SetLimitLine ( 10000 );
	RegisterControl ( m_pListText );

	int nTotalLine = m_pListText->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, FRIEND_LIST_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );

	m_pDEL_BLOCK = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_BUTTON_DELETE", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 1 ), FRIEND_LIST_DEL_BLOCK_BUTTON );
	m_pADD_BLOCK = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_BUTTON_BLOCK", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 2 ), FRIEND_LIST_ADD_BLOCK_BUTTON );

	CUIEditBox* pEditBox = new CUIEditBox;
	pEditBox->CreateSub ( this, "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_TEXTEDIT", UI_FLAG_XSIZE | UI_FLAG_BOTTOM, FRIEND_EDITBOX_BLOCK );
	pEditBox->CreateCarrat ( "MODERN_FRIEND_WINDOW_FRIEND_BLOCK_TEXTEDIT_CARRAT", TRUE, UINT_MAX );
	pEditBox->SetLimitInput ( nLIMIT_CHAR );
	pEditBox->SetFont ( pFont9 );		
	RegisterControl ( pEditBox );
	m_pEditBox = pEditBox;

	LoadBlockList ();
}

CMultiModeButton* CModernFriendWindowBlockPage::CreateButton( char* szControl, const char* szText, const UIGUID& cID /*= NO_ID*/ )
{
	CMultiModeButton* pControl = new CMultiModeButton;
	pControl->CreateSub ( this, "MODERN_BUTTON_BLUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, cID );
	pControl->CreateImageBaseMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_BASE_IMAGE_LEFT", "MODERN_BUTTON_BLUE_BASE_IMAGE_CENTER", "MODERN_BUTTON_BLUE_BASE_IMAGE_RIGHT" );
	pControl->CreateImageOverMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_OVER_IMAGE_LEFT", "MODERN_BUTTON_BLUE_OVER_IMAGE_CENTER", "MODERN_BUTTON_BLUE_OVER_IMAGE_RIGHT" );
	pControl->CreateImageHoldMulti( "MODERN_BUTTON_BLUE","MODERN_BUTTON_BLUE_CLICK_IMAGE_LEFT", "MODERN_BUTTON_BLUE_CLICK_IMAGE_CENTER", "MODERN_BUTTON_BLUE_CLICK_IMAGE_RIGHT" );
	pControl->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pControl->CreateTextBox( "MODERN_BUTTON_BLUE_TEXT_POS", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pControl->AlignToControl( szControl );
	pControl->SetOneLineText( szText, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pControl );

	return pControl;
}

void CModernFriendWindowBlockPage::LoadBlockList ()
{
	//	친구 목록을 갱신한다.
	m_pListText->ClearText ();

	GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();

	{
		GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
		GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			SFRIEND& sFriend = (*iter).second;			
			if ( sFriend.IsBLOCK() && sFriend.bONLINE )
				LoadBlockFriend ( sFriend );
		}
	}

	{
		GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
		GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			SFRIEND& sFriend = (*iter).second;			
			if ( sFriend.IsBLOCK() && !sFriend.bONLINE ) 
				LoadBlockFriend ( sFriend );
		}
	}

	m_pListText->SetCurLine ( 0 );

	const int nTotal = m_pListText->GetCount ();
	const int nViewPerPage = m_pListText->GetVisibleLine ();	
	m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
}

void CModernFriendWindowBlockPage::LoadBlockFriend ( const SFRIEND& sFriend )
{
	const CString& strName = sFriend.szCharName;
	const bool bOnline = sFriend.bONLINE;

	enum { ONLINE = 0, OFFLINE };

	static	CString strKeyword[] =
	{
		"FRIEND_ONLINE",
		"FRIEND_OFFLINE"
	};

	D3DCOLOR dwTextColor = NS_UITEXTCOLOR::DARKGRAY;
	int nIcon = OFFLINE;

	int nIndex = m_pListText->AddText ( strName, dwTextColor );	
	m_pListText->SetTextImage ( nIndex, strKeyword[nIcon] );
	m_pListText->SetTextData ( nIndex, sFriend.nCharID );
}

void CModernFriendWindowBlockPage::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case FRIEND_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKORANGE );

				if( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;
				
					int nCharID = m_pListText->GetTextData ( m_nSelectIndex );
					m_strSelectName = GetFriend ( nCharID );
				}
			}
		}
		break;

	case FRIEND_EDITBOX_BLOCK:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					CString strName = m_pEditBox->GetEditString ();
					EDIT_BEGIN ( strName );				
				}
			}
			else
			{
				if ( UIMSG_LB_DOWN & dwMsg || UIMSG_LB_UP & dwMsg )
					EDIT_END ();
			}
		}
		break;

	case FRIEND_LIST_ADD_BLOCK_BUTTON:
		{
			bool bKEY_FOCUSED = false;
			if ( CHECK_KEYFOCUSED ( dwMsg ) )
			{
				if ( !m_pEditBox->IsBegin () )
				{
					//	RETURN 무시
					UIKeyCheck::GetInstance()->RemoveCheck ( DIK_RETURN, DXKEY_DOWN );

					dwMsg &= ~UIMSG_KEY_FOCUSED;
					SetMessageEx ( dwMsg );
					return ;
				}

				bKEY_FOCUSED = true;
			}

			if ( bKEY_FOCUSED || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( CHECK_KEYFOCUSED(dwMsg) &&
					!DXInputString::GetInstance().CheckEnterKeyDown() )	return;

				if ( !m_pEditBox->GetEditLength () ) return;

				CString strName = m_pEditBox->GetEditString();
				ADD_BLOCK ( strName );

				SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus 문제로 메세지 삭제

				//	FriendWindowNormalPage에 선언된,
				//	UIMSG_EDIT_END과 같은 메시지를 보내준다.
				//	이 메시지는 에디트가 종료(캐럿을 죽임)되는 것을 목적으로 한다.
				//CInnerInterface::GetInstance().PostUIMessage ( FRIEND_WINDOW, UIMSG_EDIT_END );
				GetParent()->AddMessageEx( UIMSG_EDIT_END );
			}
		}
		break;

	case FRIEND_LIST_DEL_BLOCK_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelectIndex < 0 )
				{
					CInnerInterface::GetInstance().PrintMsgText (
						NS_UITEXTCOLOR::RED,
						ID2GAMEINTEXT("FRIEND_BLOCK_DEL_INCORRECT") );
					return ;
				}

				int nCharID = m_pListText->GetTextData ( m_nSelectIndex );
				CString strName = GetFriend ( nCharID );

				DEL_BLOCK ( strName );
			}
		}
		break;
	}
}


void CModernFriendWindowBlockPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );


	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::ORANGE );
	}

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		CDebugSet::ToView ( 1, 10, "보이는 라인 %d", nLinePerOneView );
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}
}

void CModernFriendWindowBlockPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if ( bVisible )
	{
		LoadBlockList ();
		m_nSelectIndex = -1;
	}
	else
	{
		EDIT_END ();
	}
}

CString	CModernFriendWindowBlockPage::GetFriend ( const int nCharID )
{
	GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();

	GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
	GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
	for ( ; iter != iter_end; ++iter )
	{
		SFRIEND& sFriend = (*iter).second;
		if ( sFriend.nCharID == nCharID )
		{
			return sFriend.szCharName;
		}
	}

	static CString strNULL;
	return strNULL;
}


void CModernFriendWindowBlockPage::EDIT_BEGIN ( const CString& strName )
{
	m_pEditBox->EndEdit ();
	m_pEditBox->ClearEdit ();

	m_pEditBox->SetEditString ( strName );
	m_pEditBox->BeginEdit ();	
}

void CModernFriendWindowBlockPage::EDIT_END ()
{
	m_pEditBox->EndEdit ();
}

void CModernFriendWindowBlockPage::ADD_NAME_TO_EDITBOX ( const CString& strName )
{
	m_pEditBox->SetEditString ( strName );
}

void CModernFriendWindowBlockPage::ADD_BLOCK ( const CString& strName )
{
	if ( !strName.GetLength () ) return ;

	m_strSelectName = strName;

	SFRIEND* pFriend = GLFriendClient::GetInstance().GetFriend (m_strSelectName.GetString());
	if ( pFriend && pFriend->IsBLOCK () )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("FRIEND_BLOCK_ADD_ALREADY"), strName );
		return ;
	}

	INT nTotalLine = GLFriendClient::GetInstance().GetFriendSize( EMFRIEND_BLOCK );
	if( nTotalLine >= GLCONST_CHAR::nMAX_FRIEND_NUMBER )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("FRIEND_ADD_MAX"), GLCONST_CHAR::nMAX_FRIEND_NUMBER );
		return ;
	}

	CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("FRIEND_BLOCK_ADD"), strName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_FRIEND_BLOCK_ADD_B );
}

void CModernFriendWindowBlockPage::DEL_BLOCK ( const CString& strName )
{
	if ( !strName.GetLength () ) return ;

	m_strSelectName = strName;

	CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("FRIEND_BLOCK_DEL"), strName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_FRIEND_BLOCK_DEL );
}
