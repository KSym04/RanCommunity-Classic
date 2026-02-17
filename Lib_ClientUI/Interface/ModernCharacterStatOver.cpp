#include "StdAfx.h"
#include "./ModernCharacterStatOver.h"
#include "./BasicLineBox.h"
#include "./InnerInterface.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernCharacterStatOver::CModernCharacterStatOver ()
	: m_pLineBox(NULL)
	
{
}

CModernCharacterStatOver::~CModernCharacterStatOver ()
{
}

void CModernCharacterStatOver::CreateSubControl( std::string strMsg )
{
	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxQuestList( "BASIC_LINE_BOX_QUEST_LIST" );
	RegisterControl ( m_pLineBox );

	const UIRECT& rc_old_pos = m_pLineBox->GetLocalPos ();
	const UIRECT& rc_new_pos = GetLocalPos ();
	m_pLineBox->AlignSubControl ( rc_old_pos, rc_new_pos );
	m_pLineBox->SetLocalPos ( D3DXVECTOR2 ( rc_old_pos.left, rc_old_pos.top ) );

	m_pLineBox->SetVisibleSingle( FALSE );

	m_strMsg = strMsg;
}

void CModernCharacterStatOver::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_pLineBox->SetVisibleSingle( FALSE );

	if ( CHECK_MOUSE_IN ( GetMessageEx () ) )
	{
		m_pLineBox->SetVisibleSingle( TRUE );

		if ( m_strMsg.size() )
			CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine ( m_strMsg.c_str(), NS_UITEXTCOLOR::DARKLBUE );
	}
}


