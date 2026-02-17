#include "stdafx.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "./GLAutoPotion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLAutoPotion::GLAutoPotion()
	: m_fHPThreshold(1.0f)
	, m_fMPThreshold(1.0f)
	, m_fSPThreshold(1.0f)
{
	for ( int i=0; i<AUTO_POTION_COOLDOWN_TYPE_SIZE; ++i )
	{
		m_fCoolDown[i] = 0.0f;
	}
}

GLAutoPotion::~GLAutoPotion()
{

}

void GLAutoPotion::Reset()
{
	m_fHPThreshold = 1.0f;
	m_fMPThreshold = 1.0f;
	m_fSPThreshold = 1.0f;

	for ( int i=0; i<AUTO_POTION_COOLDOWN_TYPE_SIZE; ++i )
	{
		m_fCoolDown[i] = 0.0f;
	}
}

void GLAutoPotion::FrameMove( float fElapsedTime )
{
	if ( RPARAM::bUseAutoPotion == FALSE )
		return;

	for ( int i=0; i<AUTO_POTION_COOLDOWN_TYPE_SIZE; ++i )
	{
		if ( m_fCoolDown[i] > 0.0f ) 
			m_fCoolDown[i] -= fElapsedTime;
	}
}

BOOL GLAutoPotion::isReady( int nIndex )
{
	if ( nIndex < 0 )
		return FALSE;

	if ( nIndex >= AUTO_POTION_COOLDOWN_TYPE_SIZE )
		return FALSE;

	return BOOL( m_fCoolDown[nIndex] <= 0.0f );
}

void GLAutoPotion::applyDelay( int nIndex )
{
	if ( nIndex < 0 )
		return;

	if ( nIndex >= AUTO_POTION_COOLDOWN_TYPE_SIZE )
		return;

	m_fCoolDown[nIndex] = RPARAM::fAutoPotionDelay;
}