#include "StdAfx.h"
#include "./ModernPartyStateSlot.h"
#include "./ModernPartyStateClassIcon.h"
#include "./ModernPartyStateInfo.h"

#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./BasicLineBox.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyStateSlot::CModernPartyStateSlot () :
	m_pInfo( NULL ),
	m_pIcon( NULL )
{
}

CModernPartyStateSlot::~CModernPartyStateSlot ()
{
}

void CModernPartyStateSlot::CreateSubControl ()
{
	m_pInfo = new CModernPartyStateInfo;
	m_pInfo->CreateSub ( this, "MODERN_MEMBER_STATE_SLOT_INFORMATION" );
	m_pInfo->CreateSubControl ();
	RegisterControl ( m_pInfo );

	m_pIcon = new CModernPartyStateClassIcon;
	m_pIcon->CreateSub ( this, "MODERN_MEMBER_STATE_SLOT_PORTRAIT", UI_FLAG_DEFAULT, PARTY_STATE_SLOT_ICON );
	m_pIcon->CreateSubControl ();
	RegisterControl ( m_pIcon );
}

void CModernPartyStateSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernPartyStateSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case PARTY_STATE_SLOT_ICON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MOUSEIN_ICON );
			}
		}break;
	};
}

void CModernPartyStateSlot::SetClass ( int nClassType, BOOL bSameMap )
{
	if ( m_pIcon )	m_pIcon->SetClass( nClassType, bSameMap );
}

void CModernPartyStateSlot::SetDie( bool bDIE )
{
	if ( m_pIcon )	m_pIcon->SetDie( bDIE );
	if ( m_pInfo )	m_pInfo->SetDie( bDIE );
}

void CModernPartyStateSlot::SetSchool ( int nSchoolType )
{
	if ( m_pInfo ) m_pInfo->SetSchool ( nSchoolType );
}

void CModernPartyStateSlot::SetPlayerName ( CString strName )
{
	if ( m_pInfo ) m_pInfo->SetPlayerName ( strName );
}

void CModernPartyStateSlot::SetHP ( float fPercent, int nClassType )
{
	if ( m_pInfo ) m_pInfo->SetHP ( fPercent, nClassType );
}

void CModernPartyStateSlot::SetHP ( CString str )
{
	if ( m_pInfo ) m_pInfo->SetHP ( str );
}

void CModernPartyStateSlot::SetMaster( bool bMaster )
{
	if ( m_pIcon ) m_pIcon->SetMaster ( bMaster );
}
