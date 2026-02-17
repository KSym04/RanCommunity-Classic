#include "stdafx.h"
#include "./OBJOCTreeMaterial.h"

#include "./Collision.h"
#include "../G-Logic/GLPeriod.h"
#include "./DxLightMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD		OBJOCTreeMaterial::TYPEID = EMOBJOCTREE_TYPE_MATERIAL;

OBJOCTreeMaterial::OBJOCTreeMaterial ()
{
};

OBJOCTreeMaterial::~OBJOCTreeMaterial()
{
	CleanUpDerived();
}

void OBJOCTreeMaterial::CleanUpDerived()
{
}

void OBJOCTreeMaterial::CreateBASE( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces )
{
	m_dwVert = dwVert;
	m_dwFace = dwFaces;

	SAFE_DELETE_ARRAY( m_pVertSrc );
	m_pVertSrc = new VERTEXNORCOLORTEX_850[m_dwVert];
	m_dwFVF = VERTEXNORCOLORTEX_850::FVF;
	m_nFVFSize = D3DXGetFVFVertexSize( m_dwFVF );

	EnterCriticalSection(&m_pCSLockVB);
	{		
		SAFE_RELEASE( m_pVB );
		pd3dDevice->CreateVertexBuffer( m_dwVert*m_nFVFSize, D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL );
		if( !m_pVB )
		{
			LeaveCriticalSection(&m_pCSLockVB);
			return;
		}
	}
	LeaveCriticalSection(&m_pCSLockVB);

	SAFE_RELEASE( m_pIB );
	pd3dDevice->CreateIndexBuffer( m_dwFace*3*sizeof(WORD), 0L, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL );
	if( !m_pIB )
	{
		return;
	}
}

void OBJOCTreeMaterial::CovertPieceSetting( LPDIRECT3DDEVICEQ pd3dDevice )
{
}

void OBJOCTreeMaterial::CreateVertSrc( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert )
{
	UINT nSize = D3DXGetFVFVertexSize( dwFVF );

	D3DXVECTOR3* pVector(NULL);
	D3DXVECTOR3* pNormal(NULL);
	DWORD* pDiffuse(NULL);
	D3DXVECTOR2* pTex1(NULL);
	D3DXVECTOR2* pTex2(NULL);
	D3DXVECTOR2* pTex3(NULL);
	D3DXVECTOR2* pTex4(NULL);
	D3DXVECTOR2* pTex5(NULL);

	if ( dwFVF == VERTEXNORCOLORTEX_1362::FVF )
	{
		for( DWORD i=0; i<m_dwVert; ++i )
		{
			pVector = (D3DXVECTOR3*)( pByte + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pByte + (nSize*i) + 4*3 );
			pDiffuse =(DWORD*)( pByte + (nSize*i) + 4*6 );
			pTex1	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*7 );
			//pTex2	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*9 );
			//pTex3	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*11 );
			//pTex4	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*13 );
			//pTex5	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*15 );

			pTex2	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*15 );

			m_pVertSrc[i].vPos = *pVector;
			m_pVertSrc[i].vNor = *pNormal;
			m_pVertSrc[i].dwColor = *pDiffuse;
			m_pVertSrc[i].vTex1 = *pTex1;
			m_pVertSrc[i].vTex2 = *pTex2;

			//backup
			m_pVertSrc[i].vTex3 = m_pVertSrc[i].vTex2;
		}
	}
	else
	{
		DWORD dwFVFTemp = dwFVF & (~D3DFVF_TEXCOUNT_MASK);
		UINT nSize = D3DXGetFVFVertexSize( dwFVF );
		UINT nOffSet = D3DXGetFVFVertexSize( dwFVFTemp );

		dwFVFTemp = dwFVF & D3DFVF_POSITION_MASK;
		UINT nPositionMarkSize = D3DXGetFVFVertexSize( dwFVFTemp );

		D3DXVECTOR3* pVector(NULL);
		D3DXVECTOR3* pNormal(NULL);
		D3DXVECTOR2* pTex(NULL);
		for( DWORD i=0; i<m_dwVert; ++i )
		{
			pVector = (D3DXVECTOR3*)( pByte + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pByte + (nSize*i) + nPositionMarkSize );
			pTex = (D3DXVECTOR2*)( pByte + (nSize*i) + nOffSet );

			m_pVertSrc[i].vPos = *pVector;
			m_pVertSrc[i].vNor = *pNormal;
			m_pVertSrc[i].dwColor = 0xffffffff;
			m_pVertSrc[i].vTex1 = *pTex;
		}
	}
}

void OBJOCTreeMaterial::CreateNormalColor( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert )
{
}

void OBJOCTreeMaterial::CopyVertSrcToVB( BYTE* pByte, VERTEXNORCOLORTEX_850* pVertSrc, LPDIRECT3DVERTEXBUFFERQ pVB, const DWORD dwVert )
{
	if( !pVertSrc )	return;
	if( !pVB )		return;

	EnterCriticalSection(&m_pCSLockVB);
	{
		VERTEXNORCOLORTEX_850* pVertices;
		pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
		memcpy( pVertices, pVertSrc, sizeof(VERTEXNORCOLORTEX_850)*dwVert );
		pVB->Unlock();
	}
	LeaveCriticalSection(&m_pCSLockVB);
}

void OBJOCTreeMaterial::CopyMeshToIB( WORD* pSrc, LPDIRECT3DINDEXBUFFERQ pIB, const DWORD dwFaces )
{
	if( !pSrc )		return;
	if( !pIB )		return;

	WORD* pIndices(NULL);
	pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
	memcpy( pIndices, pSrc, sizeof(WORD)*3*dwFaces );
	pIB->Unlock();
}

void OBJOCTreeMaterial::InsertColorList( LOADINGDATALIST &listColorData )
{
}

void OBJOCTreeMaterial::FrameMoveCOLOR()
{
}

void OBJOCTreeMaterial::UpdateLightMapUV( float fMultiX, float fMultiY )
{
	if ( m_dwVert == 0 )	return;
	if ( !m_pVB )			return;

	//restore orig
	for( DWORD i=0; i<m_dwVert; ++i )
	{
		m_pVertSrc[i].vTex2 = m_pVertSrc[i].vTex3;
	}

	for( DWORD i=0; i<m_dwVert; ++i )
	{
		m_pVertSrc[i].vTex2.x *= fMultiX;
		m_pVertSrc[i].vTex2.y *= fMultiY;
	}

	EnterCriticalSection(&m_pCSLockVB);
	{
		VERTEXNORCOLORTEX_850* pVertices(NULL);
		m_pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
		memcpy( pVertices, m_pVertSrc, sizeof(VERTEXNORCOLORTEX_850)*m_dwVert );
		m_pVB->Unlock();
	}
	LeaveCriticalSection(&m_pCSLockVB);
}