#include "stdafx.h"
#include "./GLAutoPilot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLAutoPilot::GLAutoPilot()
	: m_bActive(FALSE)
	, m_fRange(100.0f)
	, m_sMap(false)
	, m_vPos(0.0f,0.0f,0.0f)
{

}

GLAutoPilot::~GLAutoPilot()
{

}

void GLAutoPilot::FrameMove( float fElapsedTime )
{

}

void GLAutoPilot::Reset()
{
	m_bActive = FALSE;
	//m_fScanRange = 100.0f;
	m_sMap.ResetNativeID();
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
}