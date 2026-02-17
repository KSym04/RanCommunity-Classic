#include "StdAfx.h"
#include "./ModernFriendWindowNormalPage.h"
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

const int CModernFriendWindowNormalPage::nLIMIT_CHAR = 16;

CModernFriendWindowNormalPage::CModernFriendWindowNormalPage ()
	: m_nSelectIndex ( -1 )
	, m_bINIT( FALSE )
	, m_pListText(NULL)
	, m_pListScrollBar(NULL)
	, m_pADD(NULL)
	, m_pDEL(NULL)
	, m_pBLOCK(NULL)
	, m_pMOVE(NULL)
	, m_pPARTY(NULL)
	, m_pWHISPER(NULL)
	, m_pEditBox(NULL)
{
}

CModernFriendWindowNormalPage::~CModernFriendWindowNormalPage ()
{
}

HRESULT CModernFriendWindowNormalPage::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	m_bINIT = TRUE;

	LoadFriendList();

	return S_OK;
}

void CModernFriendWindowNormalPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_BG" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_LIST_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxEditBox( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_ROW_TOP" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxEditBox( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_ROW_BOTTOM" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_TEXTEDIT_POS_BG" );
	RegisterControl ( pBasicLineBox );

	m_pListText = new CBasicTextBoxEx;
	m_pListText->CreateSub ( this, "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_LIST", UI_FLAG_DEFAULT, FRIEND_LIST_TEXTBOX );
	m_pListText->SetFont ( pFont9 );		
	m_pListText->SetLineInterval ( 3.0f );
	m_pListText->SetSensitive ( true );
	m_pListText->SetLimitLine ( 10000 );
	RegisterControl ( m_pListText );

	int nTotalLine = m_pListText->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, FRIEND_LIST_SCROLLBAR );
	m_pListScrollBar->CreateBaseScrollBar ( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );

	m_pADD = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON_ADD", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 0 ), FRIEND_LIST_ADDBUTTON );
	m_pADD->SetShortcutKey ( DIK_RETURN );
	m_pDEL = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON_DELETE", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 1 ), FRIEND_LIST_DELBUTTON );
	m_pBLOCK = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON_BLOCK", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 2 ), FRIEND_LIST_BLOCKBUTTON );
	m_pMOVE = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON_MOVE", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 3 ), FRIEND_LIST_MOVEBUTTON );
	m_pPARTY = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON_ADDPARTY", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 4 ), FRIEND_LIST_PARTYBUTTON );
	m_pWHISPER = CreateButton( "MODERN_FRIEND_WINDOW_FRIEND_BUTTON_WHISPER", ID2GAMEWORD( "MODERN_FRIEND_WINDOW_BUTTON", 5 ), FRIEND_LIST_WHISPERBUTTON );

	CUIEditBox* pEditBox = new CUIEditBox;
	pEditBox->CreateSub ( this, "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_TEXTEDIT", UI_FLAG_XSIZE | UI_FLAG_BOTTOM, FRIEND_EDITBOX );
	pEditBox->CreateCarrat ( "MODERN_FRIEND_WINDOW_FRIEND_FRIEND_TEXTEDIT_CARRAT", TRUE, UINT_MAX );
	pEditBox->SetLimitInput ( nLIMIT_CHAR );
	pEditBox->SetFont ( pFont9 );		
	RegisterControl ( pEditBox );
	m_pEditBox = pEditBox;
}

CMultiModeButton* CModernFriendWindowNormalPage::CreateButton( char* szControl, const char* szText, const UIGUID& cID /*= NO_ID*/ )
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

void CModernFriendWindowNormalPage::LoadFriendList ()
{
	if( !m_bINIT ) return; // InitDeviceObject() 호출 후에 실행 되어야 한다.

	//	친구 목록을 갱신한다.
	m_pListText->ClearText ();

	GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();

	{
		GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
		GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			SFRIEND& sFriend = (*iter).second;
			if ( sFriend.IsBLOCK() ) continue;
			if ( sFriend.bONLINE ) LoadFriend ( sFriend );
		}
	}

	{
		GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
		GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			SFRIEND& sFriend = (*iter).second;
			if ( sFriend.IsBLOCK() ) continue;
			if ( !sFriend.bONLINE ) LoadFriend ( sFriend );
		}
	}

	m_pListText->SetCurLine ( 0 );

	const int nTotal = m_pListText->GetCount ();
	const int nViewPerPage = m_pListText->GetVisibleLine ();	
	m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
}

void CModernFriendWindowNormalPage::LoadFriend ( const SFRIEND& sFriend )
{
	const CString& strName = sFriend.szCharName;
	bool bOnline = sFriend.bONLINE;

	static CString strKeyword[] =
	{
		"FRIEND_ONLINE",
		"FRIEND_OFFLINE"
	};

	D3DCOLOR dwTextColor;
	int nIcon;

	if ( bOnline && sFriend.nCharFlag != EMFRIEND_BETRAY)
	{
		dwTextColor = NS_UITEXTCOLOR::WHITE;
		nIcon = ONLINE;
	}
	else
	{
		dwTextColor = NS_UITEXTCOLOR::DARKGRAY;
		nIcon = OFFLINE;
	}

	if( sFriend.nCharFlag == EMFRIEND_BETRAY ) // Note : 상대방이 나를 친구에서 삭제한 경우
	{
		dwTextColor = NS_UITEXTCOLOR::FRIEND_OFF;
		nIcon = OFFLINE;
		bOnline = FALSE;
	}

	CString strTemp;
	
	if( CInnerInterface::GetInstance().IsCHANNEL () && bOnline )
	{
		strTemp.Format( "[%d]%s", sFriend.nCHANNEL, strName );
	}
	else
	{
		strTemp.Format( "%s", strName );
	}

	int nIndex = m_pListText->AddText ( strTemp, dwTextColor );	
	m_pListText->SetTextImage ( nIndex, strKeyword[nIcon] );	
	m_pListText->SetTextData ( nIndex, sFriend.nCharID );
}

BOOL CModernFriendWindowNormalPage::GetFriend ( const int nCharID, CString & strName )
{
	GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();

	GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
	GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
	for ( ; iter != iter_end; ++iter )
	{
		SFRIEND& sFriend = (*iter).second;
		if ( sFriend.nCharID == nCharID )
		{
			strName = sFriend.szCharName;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CModernFriendWindowNormalPage::SetFriendInfo( const int nCharID )
{
	GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();

	GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
	GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
	for ( ; iter != iter_end; ++iter )
	{
		if( (*iter).second.nCharID == nCharID )
		{
			SFRIEND& sFriend = (*iter).second;
			m_sFriend = sFriend;

			return TRUE;
		}
	}

	return FALSE;
}

void CModernFriendWindowNormalPage::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
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
					if( !GetFriend( nCharID, m_strSelectName ) ) m_strSelectName.Empty();
				}
				
				if ( UIMSG_LB_DUP & dwMsg )
				{
					SFRIEND* pFriend = GLFriendClient::GetInstance().GetFriend ( m_strSelectName.GetString() );
					if ( pFriend && pFriend->bONLINE )
					{
						//CInnerInterface::GetInstance().PostUIMessage ( FRIEND_WINDOW, UIMSG_ADDNAME_TO_CHATBOX );
						GetParent()->AddMessageEx( UIMSG_ADDNAME_TO_CHATBOX );
					}
				}
			}
		}
		break;

	case FRIEND_EDITBOX:
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

	case FRIEND_LIST_ADDBUTTON:
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
				ADD_FRIEND ( strName );
				
				SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus 문제로 메세지 삭제

				//CInnerInterface::GetInstance().PostUIMessage ( GetWndID (), UIMSG_EDIT_END );
				GetParent()->AddMessageEx( UIMSG_EDIT_END );
			}
		}
		break;

	case FRIEND_LIST_DELBUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelectIndex < 0 )
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("FRIEND_DEL_INCORRECT") );
					return ;
				}

				int nCharID = m_pListText->GetTextData ( m_nSelectIndex );

				CString strName;
				if( GetFriend( nCharID, strName ) ) DEL_FRIEND ( strName );
			}
		}
		break;

	case FRIEND_LIST_MOVEBUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( m_nSelectIndex < 0 ) return ;

				int nCharID = m_pListText->GetTextData ( m_nSelectIndex );

				CString strName;
				if( GetFriend( nCharID, strName ) ) MOVETO_FRIEND ( strName ); //	친구에게 이동
			}
		}
		break;

	case FRIEND_LIST_BLOCKBUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if( m_nSelectIndex < 0 ) return ;

				int nCharID = m_pListText->GetTextData( m_nSelectIndex );

				CString strName;
				if( GetFriend( nCharID, strName ) )
				{
					ADD_BLOCK( strName );
				}
			}
		}
		break;

	case FRIEND_LIST_PARTYBUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if( m_nSelectIndex < 0 ) return ;
				int nCharID = m_pListText->GetTextData( m_nSelectIndex );
				CString strName;
				if( GetFriend( nCharID, strName ) )
				{
					CString strTemp = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("PARTY_QUESTION"), strName.GetString() );

					CPartyModalWindow* pPartyModalWindow = CInnerInterface::GetInstance().GetPartyModalWindow ();
					if ( pPartyModalWindow )
					{
						pPartyModalWindow->DoModal ( strTemp, MODAL_PARTY_REQ_PARTY_WINDOW );
						CInnerInterface::GetInstance().ShowGroupFocus ( PARTY_MODAL_WINDOW );
					}
				}
			}
		}
		break;

	case FRIEND_LIST_WHISPERBUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if( m_nSelectIndex < 0 ) return ;
				int nCharID = m_pListText->GetTextData ( m_nSelectIndex );
				if( GetFriend( nCharID, m_strSelectName ) )
				{
					GetParent()->AddMessageEx( UIMSG_ADDNAME_TO_CHATBOX );
				}
			}
		}
		break;
	}
}

void CModernFriendWindowNormalPage::ADD_FRIEND ( const CString& strName )
{
	if ( !strName.GetLength () ) return ;

	m_strSelectName = strName;

	SFRIEND* pFriend = GLFriendClient::GetInstance().GetFriend (m_strSelectName.GetString());
	if ( pFriend && !pFriend->IsBLOCK () )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("FRIEND_ADD_ALREADY"), strName );
		return ;
	}

	INT nTotalLine = GLFriendClient::GetInstance().GetFriendSize( EMFRIEND_VALID );
	nTotalLine += GLFriendClient::GetInstance().GetFriendSize( EMFRIEND_BETRAY );
	if( nTotalLine >= GLCONST_CHAR::nMAX_FRIEND_NUMBER )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("FRIEND_ADD_MAX"), GLCONST_CHAR::nMAX_FRIEND_NUMBER );
		return ;
	}

	CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("FRIEND_ADD"), strName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_FRIEND_ADD );
}

void CModernFriendWindowNormalPage::DEL_FRIEND ( const CString& strName )
{
	if ( !strName.GetLength () ) return ;

	m_strSelectName = strName;

	CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("FRIEND_DEL"), strName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_FRIEND_DEL );
}

void CModernFriendWindowNormalPage::MOVETO_FRIEND ( const CString& strName )
{
	if ( !strName.GetLength () ) return ;

	m_strSelectName = strName;

	GLFriendClient::FRIENDMAP& FriendMap = GLFriendClient::GetInstance().GetFriend();

	//	온라인이 아니면 처리하지 않음
	{
		GLFriendClient::FRIENDMAP_ITER iter = FriendMap.begin ();
		GLFriendClient::FRIENDMAP_ITER iter_end = FriendMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			SFRIEND& sFriend = (*iter).second;
			if ( !strcmp ( sFriend.szCharName, m_strSelectName.GetString() ) )
			{
				if ( !sFriend.bONLINE ) return ;
			}
		}
	}

	CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("FRIEND_MOVE"), strName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_FRIEND_MOVE );
}

void CModernFriendWindowNormalPage::EDIT_BEGIN ( const CString& strName )
{
	if( m_pEditBox )
	{
		m_pEditBox->EndEdit ();
		m_pEditBox->ClearEdit ();

		m_pEditBox->SetEditString ( strName );
		m_pEditBox->BeginEdit ();	
	}
}

void CModernFriendWindowNormalPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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

void CModernFriendWindowNormalPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadFriendList ();

		m_nSelectIndex = -1;

		if( GLCONST_CHAR::bBATTLEROYAL )
		{
			m_pADD->SetVisibleSingle( FALSE );
		}
	}
	else
	{
		EDIT_END ();
	}
}

void CModernFriendWindowNormalPage::EDIT_END ()
{
	if( m_pEditBox ) m_pEditBox->EndEdit();
}

void CModernFriendWindowNormalPage::ADD_FRIEND_NAME_TO_EDITBOX ( const CString& strName )
{
	if( m_pEditBox ) m_pEditBox->SetEditString( strName );
}

void CModernFriendWindowNormalPage::ADD_BLOCK ( const CString& strName )
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
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_FRIEND_BLOCK_ADD_N );
}