#include "stdafx.h"
#include "./DxPieceAnimation.h"
#include "./DxAnimationManager.h"
#include "../DxCommon/DxSingleTexMaterialMesh.h"
#include "../DxEffect/DxStaticMaterialMesh.h"
#include "../DxEffect/DxPieceMultiStaticMaterialMesh.h"
#include "../DxEffect/DxStaticMaterialMesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const DWORD DxPieceAnimation::DXPIECE_ANIMATION_VERSION = 0x0100;

DxPieceAnimation::DxPieceAnimation ()
	: m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX)
	, m_vMin(FLT_MAX,FLT_MAX,FLT_MAX)
	, m_pDxAnimation(NULL)
	, m_pLeft(NULL)
	, m_pRight(NULL)
	, m_dwVar(UINT_MAX)
{
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matResult );
}

DxPieceAnimation::~DxPieceAnimation()
{
	SAFE_DELETE (m_pDxAnimation);
	SAFE_DELETE (m_pLeft);
	SAFE_DELETE (m_pRight);
}

void DxPieceAnimation::FrameMove ( float fCurTime, const D3DXMATRIX& mat )
{
	if ( m_pDxAnimation )
	{
		m_matWorld = *m_pDxAnimation->SetTime ( fCurTime );
	}

	D3DXMatrixMultiply( &m_matResult, &m_matWorld, &mat );

	DxPieceAnimation* p = m_pRight;
	while( p )
	{
		p->FrameMove( fCurTime, m_matResult );

		p = p->m_pLeft;
	}
}
