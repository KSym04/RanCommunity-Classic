#include "stdafx.h"
#include "./OBJOCTree.h"

#include "./Collision.h"
#include "../G-Logic/GLPeriod.h"
#include "./DxLightMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


OBJOCTree::OBJOCTree () :
	vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX),
	vMin(FLT_MAX,FLT_MAX,FLT_MAX),
	pLeftChild(NULL),
	pRightChild(NULL),
	pVertex(NULL),
	dwFace(0),
	pOcMesh(NULL),
	pNormalColor(NULL),
	pCollsionTREE(NULL),
	m_dwFVF(VERTEXNORCOLORTEX_850::FVF),
	m_nFVFSize(0),
	m_dwVert(0L),
	m_dwFace(0L),
	m_pVertSrc(NULL),
	m_pColor(NULL),
	m_pVB(NULL),
	m_pIB(NULL),

	m_bCheck(FALSE),
	m_bLoad(FALSE)
{
	InitializeCriticalSection(&m_pCSLockVB);
};

OBJOCTree::~OBJOCTree()
{
	CleanUpBase();

	DeleteCriticalSection(&m_pCSLockVB);

	SAFE_DELETE(pLeftChild);
	SAFE_DELETE(pRightChild);
}

void OBJOCTree::CleanUpBase()
{
	dwFace = 0;
	SAFE_DELETE_ARRAY ( pVertex );

	m_dwVert = 0L;
	m_dwFace = 0L;
	SAFE_DELETE_ARRAY( m_pVertSrc );
	SAFE_DELETE_ARRAY( m_pColor );
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );

	SAFE_RELEASE( pOcMesh );
	SAFE_DELETE_ARRAY( pNormalColor );
	SAFE_DELETE( pCollsionTREE );

	m_bCheck = FALSE;
	m_bLoad = FALSE;
}

void OBJOCTree::DrawEff( const LPDIRECT3DDEVICEQ pd3dDevice )
{
	pd3dDevice->SetFVF( m_dwFVF );
	pd3dDevice->SetStreamSource( 0, m_pVB, 0, m_nFVFSize );
	pd3dDevice->SetIndices( m_pIB );

	EnterCriticalSection(&m_pCSLockVB);

	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_dwVert, 0, m_dwFace );

	LeaveCriticalSection(&m_pCSLockVB);
}

