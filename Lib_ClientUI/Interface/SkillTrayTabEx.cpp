#include "StdAfx.h"
#include "./SkillTrayTabEx.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"
#include "../../Lib_Engine/GUInterface/UIKeyCheck.h"

#include "./BasicTextButton.h"
#include "./BasicButton.h"
#include "./MultiModeButton.h"
#include "./UITextControl.h"
#include "./BasicTextBox.h"
#include "./BasicQuickSkillSlot2.h"
#include "./BasicSkillTrayEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

INT CSkillTrayTabEx::NEXT_INDEX[] = {1, 2, 3, 0};

CSkillTrayTabEx::CSkillTrayTabEx(void)
	: m_pBasicQuickSkillSlot(NULL)
	, m_pButtonClose(NULL)
	, m_pButtonRotate(NULL)
	, m_pButtonLock(NULL)
	, m_pButtonUnlock(NULL)
	, m_pButtonSetting(NULL)
	, m_nTabIndex(0)
	, m_nCount(0)
	, m_bFirstGap ( FALSE )
{
	memset( m_pSkillTray, 0, sizeof( CBasicSkillTray* ) * MAX_TAB_INDEX );
	memset( m_pTextButton, 0, sizeof( CBasicTextButton* ) * MAX_TAB_INDEX );
}

CSkillTrayTabEx::~CSkillTrayTabEx(void)
{
}

void CSkillTrayTabEx::CreateSubControl()
{
	CD3DFontPar * m_pFont8 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	m_pBasicQuickSkillSlot = new CBasicQuickSkillSlot2;
	m_pBasicQuickSkillSlot->CreateSub ( this, "BASIC_QUICK_SKILL_SLOT_H", UI_FLAG_DEFAULT, BASIC_QUICK_SKILL_BASE_SLOT );
	m_pBasicQuickSkillSlot->CreateSubControl ();
	m_pBasicQuickSkillSlot->SetUseSkillImageTwinkle( false );
	RegisterControl ( m_pBasicQuickSkillSlot );

	CBasicSkillTrayEx * pSkillTray = new CBasicSkillTrayEx;
	pSkillTray->CreateSub( this, "BASIC_QUICK_SKILL_TRAY_H", UI_FLAG_DEFAULT, BASIC_QUICK_SKILL_TRAY_F1 );
	pSkillTray->CreateSubControl( 0 );
	RegisterControl( pSkillTray );
	m_pSkillTray[0] = pSkillTray;

	pSkillTray = new CBasicSkillTrayEx;
	pSkillTray->CreateSub( this, "BASIC_QUICK_SKILL_TRAY_H", UI_FLAG_DEFAULT, BASIC_QUICK_SKILL_TRAY_F2 );
	pSkillTray->CreateSubControl( 1 );
	pSkillTray->SetVisibleSingle( FALSE );
	RegisterControl( pSkillTray );
	m_pSkillTray[1] = pSkillTray;

	pSkillTray = new CBasicSkillTrayEx;
	pSkillTray->CreateSub( this, "BASIC_QUICK_SKILL_TRAY_H", UI_FLAG_DEFAULT, BASIC_QUICK_SKILL_TRAY_F3 );
	pSkillTray->CreateSubControl( 2 );
	pSkillTray->SetVisibleSingle( FALSE );
	RegisterControl( pSkillTray );
	m_pSkillTray[2] = pSkillTray;

	pSkillTray = new CBasicSkillTrayEx;
	pSkillTray->CreateSub( this, "BASIC_QUICK_SKILL_TRAY_H", UI_FLAG_DEFAULT, BASIC_QUICK_SKILL_TRAY_F4 );
	pSkillTray->CreateSubControl( 3 );
	pSkillTray->SetVisibleSingle( FALSE );
	RegisterControl( pSkillTray );
	m_pSkillTray[3] = pSkillTray;


	CMultiModeButton * pTextButton = new CMultiModeButton;
	pTextButton->CreateSub ( this, "QUICK_SKILL_TRAY_TAB_BUTTON_BASE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, BASIC_TEXT_BUTTON_F1 );
	pTextButton->CreateImageBase( "QUICK_SKILL_TRAY_TAB_BUTTON_DEFAULT" );
	pTextButton->CreateImageOver( "QUICK_SKILL_TRAY_TAB_BUTTON_CLICK" );
	pTextButton->CreateTextBox( "QUICK_SKILL_TRAY_TAB_BUTTON_TEXT", 8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pTextButton->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pTextButton->AlignToControl( "QUICK_SKILL_TRAY_TAB_BUTTON_H" );
	pTextButton->SetOneLineText( ID2GAMEWORD( "SKILL_TAB_INFO", 0 ), NS_UITEXTCOLOR::WHITE );
	pTextButton->SetUseGlobalAction ( TRUE );	
	RegisterControl( pTextButton );
	m_pTextButton[0] = pTextButton;
	SetFoldGroup( pTextButton, TABBUTTON_FOLDGROUP_ID );

	pTextButton = new CMultiModeButton;
	pTextButton->CreateSub ( this, "QUICK_SKILL_TRAY_TAB_BUTTON_BASE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, BASIC_TEXT_BUTTON_F2 );
	pTextButton->CreateImageBase( "QUICK_SKILL_TRAY_TAB_BUTTON_DEFAULT" );
	pTextButton->CreateImageOver( "QUICK_SKILL_TRAY_TAB_BUTTON_CLICK" );
	pTextButton->CreateTextBox( "QUICK_SKILL_TRAY_TAB_BUTTON_TEXT", 8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pTextButton->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pTextButton->AlignToControl( "QUICK_SKILL_TRAY_TAB_BUTTON_H" );
	pTextButton->SetOneLineText( ID2GAMEWORD( "SKILL_TAB_INFO", 1 ), NS_UITEXTCOLOR::WHITE );
	pTextButton->SetUseGlobalAction ( TRUE );	
	pTextButton->SetVisibleSingle( FALSE );
	RegisterControl( pTextButton );
	m_pTextButton[1] = pTextButton;
	SetFoldGroup( pTextButton, TABBUTTON_FOLDGROUP_ID );

	pTextButton = new CMultiModeButton;
	pTextButton->CreateSub ( this, "QUICK_SKILL_TRAY_TAB_BUTTON_BASE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, BASIC_TEXT_BUTTON_F3 );
	pTextButton->CreateImageBase( "QUICK_SKILL_TRAY_TAB_BUTTON_DEFAULT" );
	pTextButton->CreateImageOver( "QUICK_SKILL_TRAY_TAB_BUTTON_CLICK" );
	pTextButton->CreateTextBox( "QUICK_SKILL_TRAY_TAB_BUTTON_TEXT", 8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pTextButton->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pTextButton->AlignToControl( "QUICK_SKILL_TRAY_TAB_BUTTON_H" );
	pTextButton->SetOneLineText( ID2GAMEWORD( "SKILL_TAB_INFO", 2 ), NS_UITEXTCOLOR::WHITE );
	pTextButton->SetUseGlobalAction ( TRUE );	
	pTextButton->SetVisibleSingle( FALSE );
	RegisterControl( pTextButton );
	m_pTextButton[2] = pTextButton;
	SetFoldGroup( pTextButton, TABBUTTON_FOLDGROUP_ID );

	pTextButton = new CMultiModeButton;
	pTextButton->CreateSub ( this, "QUICK_SKILL_TRAY_TAB_BUTTON_BASE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, BASIC_TEXT_BUTTON_F4 );
	pTextButton->CreateImageBase( "QUICK_SKILL_TRAY_TAB_BUTTON_DEFAULT" );
	pTextButton->CreateImageOver( "QUICK_SKILL_TRAY_TAB_BUTTON_CLICK" );
	pTextButton->CreateTextBox( "QUICK_SKILL_TRAY_TAB_BUTTON_TEXT", 8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_SHADOW_FLAG );
	pTextButton->SetTextColor( NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	pTextButton->AlignToControl( "QUICK_SKILL_TRAY_TAB_BUTTON_H" );
	pTextButton->SetOneLineText( ID2GAMEWORD( "SKILL_TAB_INFO", 3 ), NS_UITEXTCOLOR::WHITE );
	pTextButton->SetUseGlobalAction ( TRUE );	
	pTextButton->SetVisibleSingle( FALSE );
	RegisterControl( pTextButton );
	m_pTextButton[3] = pTextButton;
	SetFoldGroup( pTextButton, TABBUTTON_FOLDGROUP_ID );

	m_pButtonClose = new CBasicButton;
	m_pButtonClose->CreateSub( this, "QUICK_SKILL_TRAY_CLOSE_BUTTON_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_CLOSE_BUTTON );
	m_pButtonClose->CreateFlip( "QUICK_SKILL_TRAY_CLOSE_BUTTON_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonClose->SetUseGlobalAction ( TRUE );	
	RegisterControl( m_pButtonClose );

	m_pButtonRotate = new CBasicButton;
	m_pButtonRotate->CreateSub( this, "QUICK_SKILL_TRAY_TURN_BUTTON_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_ROTATE_BUTTON );
	m_pButtonRotate->CreateFlip( "QUICK_SKILL_TRAY_TURN_BUTTON_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonRotate->SetUseGlobalAction ( TRUE );	
	RegisterControl( m_pButtonRotate );

	m_pButtonLock = new CBasicButton;
	m_pButtonLock->CreateSub( this, "QUICK_SKILL_TRAY_LOCK_BUTTON_OPEN_LOCK_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_LOCK_BUTTON );
	m_pButtonLock->CreateFlip( "QUICK_SKILL_TRAY_LOCK_BUTTON_OPEN_LOCK_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonLock->SetUseGlobalAction ( TRUE );	
	m_pButtonLock->SetVisibleSingle( FALSE );
	RegisterControl( m_pButtonLock );

	m_pButtonUnlock = new CBasicButton;
	m_pButtonUnlock->CreateSub( this, "QUICK_SKILL_TRAY_LOCK_BUTTON_OPEN_UNLOCK_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_UNLOCK_BUTTON );
	m_pButtonUnlock->CreateFlip( "QUICK_SKILL_TRAY_LOCK_BUTTON_OPEN_UNLOCK_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonUnlock->SetUseGlobalAction ( TRUE );	
	m_pButtonLock->SetVisibleSingle( TRUE );
	RegisterControl( m_pButtonUnlock );

	m_pButtonSetting = new CBasicButton;
	m_pButtonSetting->CreateSub( this, "QUICK_SKILL_TRAY_SETTING_BUTTON_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_SETTING_BUTTON );
	m_pButtonSetting->CreateFlip( "QUICK_SKILL_TRAY_SETTING_BUTTON_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonSetting->SetUseGlobalAction ( TRUE );	
	RegisterControl( m_pButtonSetting );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "QUICK_SKILL_TRAY_GAP_0_H", UI_FLAG_XSIZE | UI_FLAG_YSIZE  );
	pControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pControl );

	pControl = new CUIControl;
	pControl->CreateSub ( this, "QUICK_SKILL_TRAY_GAP_1_H", UI_FLAG_XSIZE | UI_FLAG_YSIZE  );
	pControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pControl );
}

void CSkillTrayTabEx::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_bCHECK_MOUSE_STATE = false;

	m_PosX = x;
	m_PosY = y;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() )
	{
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
		AddMessageEx( UIMSG_SKILLTRAY_TAB_LBDUP );
	}

	bool skill_tray_lock = CInnerInterface::GetInstance().skill_tray_get_lock();

	if ( m_pButtonLock )	m_pButtonLock->SetVisibleSingle( skill_tray_lock );
	if ( m_pButtonUnlock )	m_pButtonUnlock->SetVisibleSingle( !skill_tray_lock );

	if ( CInnerInterface::GetInstance().IsVisibleGroup( MODAL_WINDOW ) ) return ;

	INT nOldIndex = GetTabIndex();
	INT nNewIndex( nOldIndex );

	if( UIKeyCheck::GetInstance()->Check( DIK_F1, DXKEY_DOWN ) )	
		nNewIndex = 0;
	if( UIKeyCheck::GetInstance()->Check( DIK_F2, DXKEY_DOWN ) )	
		nNewIndex = 1;
	if( UIKeyCheck::GetInstance()->Check( DIK_F3, DXKEY_DOWN ) )	
		nNewIndex = 2;
	if( UIKeyCheck::GetInstance()->Check( DIK_F4, DXKEY_DOWN ) )	
		nNewIndex = 3;

	if( nNewIndex != nOldIndex )
	{
		m_pSkillTray[nOldIndex]->SetVisibleSingle( FALSE );
		m_pSkillTray[nNewIndex]->SetVisibleSingle( TRUE );
		m_pTextButton[nOldIndex]->SetVisibleSingle( FALSE );
		m_pTextButton[nNewIndex]->SetVisibleSingle( TRUE );
		SetTabIndex( nNewIndex );
	}
	
	
}

void CSkillTrayTabEx::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BASIC_QUICK_SKILL_BASE_SLOT:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( !m_bCHECK_MOUSE_STATE )
				{
					CheckMouseState ();
					m_bCHECK_MOUSE_STATE = true;
				}
			}
		}break;

	case QUICK_SKILL_TRAY_CLOSE_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
					CInnerInterface::GetInstance().skill_tray_open( TRUE );
			}
		}break;

	case QUICK_SKILL_TRAY_ROTATE_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
					CInnerInterface::GetInstance().skill_tray_rotate();
			}
		}break;

	case QUICK_SKILL_TRAY_LOCK_BUTTON:
	case QUICK_SKILL_TRAY_UNLOCK_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
					CInnerInterface::GetInstance().skill_tray_lock();
			}
		}break;

	case QUICK_SKILL_TRAY_SETTING_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
					CInnerInterface::GetInstance().OpenAutoPilotWindow();
			}
		}break;

	case BASIC_TEXT_BUTTON_F1:
	case BASIC_TEXT_BUTTON_F2:
	case BASIC_TEXT_BUTTON_F3:
	case BASIC_TEXT_BUTTON_F4:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE( dwMsg ) )
				{
					INT nIndex = GetTabIndex();
					INT nNextIndex = NEXT_INDEX[nIndex];

					m_pSkillTray[nIndex]->SetVisibleSingle( FALSE );
					m_pSkillTray[nNextIndex]->SetVisibleSingle( TRUE );

					m_pTextButton[nIndex]->SetVisibleSingle( FALSE );
					m_pTextButton[nNextIndex]->SetVisibleSingle( TRUE );

					SetTabIndex( nNextIndex );
				}
			}
		}
		break;

	case BASIC_QUICK_SKILL_TRAY_F1:
	case BASIC_QUICK_SKILL_TRAY_F2:
	case BASIC_QUICK_SKILL_TRAY_F3:
	case BASIC_QUICK_SKILL_TRAY_F4:
		{
			if( dwMsg & UIMSG_MOUSEIN_SKILLSLOT_EX )
			{
				AddMessageEx ( UIMSG_MOUSEIN_SKILLSLOT_EX );
			}
		}
		break;
	}
}

void CSkillTrayTabEx::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		m_pSkillTray[0]->SetVisibleSingle( FALSE );
		m_pSkillTray[1]->SetVisibleSingle( FALSE );
		m_pSkillTray[2]->SetVisibleSingle( FALSE );
		m_pSkillTray[3]->SetVisibleSingle( FALSE );

		m_pTextButton[0]->SetVisibleSingle( FALSE );
		m_pTextButton[1]->SetVisibleSingle( FALSE );
		m_pTextButton[2]->SetVisibleSingle( FALSE );
		m_pTextButton[3]->SetVisibleSingle( FALSE );

		INT nIndex = GetTabIndex();
		m_pSkillTray[nIndex]->SetVisibleSingle( TRUE );
		m_pTextButton[nIndex]->SetVisibleSingle( TRUE );
	}
}

void CSkillTrayTabEx::SetShotcutText ( DWORD nID, CString& strTemp )
{
	m_pSkillTray[0]->SetShotcutText( nID, strTemp );
	m_pSkillTray[1]->SetShotcutText( nID, strTemp );
	m_pSkillTray[2]->SetShotcutText( nID, strTemp );
	m_pSkillTray[3]->SetShotcutText( nID, strTemp );
}

void CSkillTrayTabEx::CheckMouseState ()
{
	if ( CInnerInterface::GetInstance().skill_tray_get_lock() )	return;

	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if ( dwMsg & UIMSG_LB_DRAG)
		{
			SetExclusiveControl();	

			if ( !m_bFirstGap )
			{
				UIRECT rcPos = GetGlobalPos ();
				m_vGap.x = m_PosX - rcPos.left;
				m_vGap.y = m_PosY - rcPos.top;
				m_bFirstGap = TRUE;
			}
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			ResetExclusiveControl();
			m_bFirstGap = FALSE;				
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
	{								
		ResetExclusiveControl();
		m_bFirstGap = FALSE;					
	}
}
