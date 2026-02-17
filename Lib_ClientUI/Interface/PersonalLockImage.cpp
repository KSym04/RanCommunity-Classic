#include "StdAfx.h"
#include "./PersonalLockImage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPersonalLockImage::CPersonalLockImage ()
	: m_pImageLock(NULL)
	, m_pImageHover(NULL)
{
}

CPersonalLockImage::~CPersonalLockImage ()
{
}

void CPersonalLockImage::CreateSubControl ()
{
	m_pImageLock = new CUIControl;
	m_pImageLock->CreateSub ( this, "PERSONAL_LOCK_IMAGE_LOCK" );
	m_pImageLock->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageLock );

	m_pImageHover = new CUIControl;
	m_pImageHover->CreateSub ( this, "PERSONAL_LOCK_IMAGE_HOVER" );
	m_pImageHover->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageHover );
}

void CPersonalLockImage::SetImageLock( BOOL bSet )
{
	if ( m_pImageLock )
		m_pImageLock->SetVisibleSingle( bSet );
}

void CPersonalLockImage::SetImageHover( BOOL bSet )
{
	if ( m_pImageHover )
		m_pImageHover->SetVisibleSingle( bSet );
}