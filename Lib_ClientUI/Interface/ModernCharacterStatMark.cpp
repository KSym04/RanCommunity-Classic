#include "StdAfx.h"
#include "./ModernCharacterStatMark.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernCharacterStatMark::CModernCharacterStatMark ()
	: m_pUpImage(NULL)
	, m_pDownImage(NULL)
{
}

CModernCharacterStatMark::~CModernCharacterStatMark ()
{
}

void CModernCharacterStatMark::CreateSubControl ()
{
	m_pUpImage = new CUIControl;
	m_pUpImage->CreateSub ( this, "UPPER_ARROW", UI_FLAG_DEFAULT, MARK_IMAGE_UP );
	m_pUpImage->SetVisibleSingle( FALSE );
	RegisterControl ( m_pUpImage );

	m_pDownImage = new CUIControl;
	m_pDownImage->CreateSub ( this, "LOWER_ARROW", UI_FLAG_DEFAULT, MARK_IMAGE_DOWN );
	m_pDownImage->SetVisibleSingle( FALSE );
	RegisterControl ( m_pDownImage );
}

void CModernCharacterStatMark::Update ( int x ,int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernCharacterStatMark::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( cID, dwMsg );
}

void CModernCharacterStatMark::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );
	if ( bVisible )
	{
		Reset();
	}
}

void CModernCharacterStatMark::Reset()
{
	if ( !m_pUpImage || !m_pDownImage )	return;

	m_pUpImage->SetVisibleSingle( FALSE );
	m_pDownImage->SetVisibleSingle( FALSE );
}

void CModernCharacterStatMark::ShowImage( bool bUp )
{
	if ( !m_pUpImage || !m_pDownImage )	return;
	Reset();

	m_pUpImage->SetVisibleSingle( bUp );
	m_pDownImage->SetVisibleSingle( !bUp );
}

bool CModernCharacterStatMark::CheckValue( int nOrig, int nToCheck )
{
	if ( nToCheck == nOrig )
	{
		Reset();
		return false;
	}else{
		ShowImage( nToCheck > nOrig );
		return true;
	}

	return false;
}

bool CModernCharacterStatMark::CheckValue( float fOrig, float fToCheck )
{
	if ( fToCheck == fOrig )
	{
		Reset();
		return false;
	}else{
		ShowImage( fToCheck > fOrig );
		return true;
	}

	return false;
}

bool CModernCharacterStatMark::CheckValue( int nCheck )
{
	ShowImage( nCheck > 0 );
	return true;
}