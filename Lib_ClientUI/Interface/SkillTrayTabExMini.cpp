#include "StdAfx.h"
#include "./SkillTrayTabExMini.h"

#include "./InnerInterface.h"
#include "./BasicButton.h"
#include "./BasicQuickSkillSlot2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSkillTrayTabExMini::CSkillTrayTabExMini(void)
	: m_pBasicQuickSkillSlot(NULL)
	, m_pButtonOpen(NULL)
	, m_pButtonLock(NULL)
	, m_pButtonUnlock(NULL)
	, m_bFirstGap ( FALSE )
{
}

CSkillTrayTabExMini::~CSkillTrayTabExMini(void)
{
}

void CSkillTrayTabExMini::CreateSubControl()
{
	m_pBasicQuickSkillSlot = new CBasicQuickSkillSlot2;
	m_pBasicQuickSkillSlot->CreateSub ( this, "BASIC_QUICK_SKILL_SLOT_H", UI_FLAG_DEFAULT, BASIC_QUICK_SKILL_BASE_SLOT );
	m_pBasicQuickSkillSlot->CreateSubControl ();
	m_pBasicQuickSkillSlot->SetUseSkillImageTwinkle( false );
	RegisterControl ( m_pBasicQuickSkillSlot );

	m_pButtonOpen = new CBasicButton;
	m_pButtonOpen->CreateSub( this, "QUICK_SKILL_TRAY_OPEN_BUTTON_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_OPEN_BUTTON );
	m_pButtonOpen->CreateFlip( "QUICK_SKILL_TRAY_OPEN_BUTTON_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonOpen->SetUseGlobalAction ( TRUE );	
	RegisterControl( m_pButtonOpen );

	m_pButtonLock = new CBasicButton;
	m_pButtonLock->CreateSub( this, "QUICK_SKILL_TRAY_LOCK_BUTTON_CLOSE_LOCK_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_LOCK_BUTTON );
	m_pButtonLock->CreateFlip( "QUICK_SKILL_TRAY_LOCK_BUTTON_CLOSE_LOCK_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonLock->SetUseGlobalAction ( TRUE );	
	m_pButtonLock->SetVisibleSingle( FALSE );
	RegisterControl( m_pButtonLock );

	m_pButtonUnlock = new CBasicButton;
	m_pButtonUnlock->CreateSub( this, "QUICK_SKILL_TRAY_LOCK_BUTTON_CLOSE_UNLOCK_H", UI_FLAG_DEFAULT, QUICK_SKILL_TRAY_UNLOCK_BUTTON );
	m_pButtonUnlock->CreateFlip( "QUICK_SKILL_TRAY_LOCK_BUTTON_CLOSE_UNLOCK_F_H", CBasicButton::MOUSEIN_FLIP );
	m_pButtonUnlock->SetUseGlobalAction ( TRUE );	
	m_pButtonLock->SetVisibleSingle( TRUE );
	RegisterControl( m_pButtonUnlock );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, "QUICK_SKILL_TRAY_GAP_2_H", UI_FLAG_XSIZE | UI_FLAG_YSIZE  );
	pControl->SetVisibleSingle ( TRUE );
	RegisterControl ( pControl );
}

void CSkillTrayTabExMini::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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
}

void CSkillTrayTabExMini::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
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

	case QUICK_SKILL_TRAY_OPEN_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
					CInnerInterface::GetInstance().skill_tray_open( FALSE );
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
	}
}

void CSkillTrayTabExMini::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}


void CSkillTrayTabExMini::CheckMouseState ()
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
