#include "stdafx.h"
#include "./OBJOCTreeLightMap.h"

#include "./Collision.h"
#include "../G-Logic/GLPeriod.h"
#include "./DxLightMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD		OBJOCTreeLightMap::TYPEID = EMOBJOCTREE_TYPE_LIGHTMAP;

OBJOCTreeLightMap::OBJOCTreeLightMap () 
{
};

OBJOCTreeLightMap::~OBJOCTreeLightMap()
{
	CleanUpDerived();
}

void OBJOCTreeLightMap::CleanUpDerived()
{
}

void OBJOCTreeLightMap::CreateBASE( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces )
{
	m_dwVert = dwVert;
	m_dwFace = dwFaces;

	SAFE_DELETE_ARRAY( pNormalColor );
	pNormalColor = new NORMALCOLOR[m_dwVert];

	SAFE_DELETE_ARRAY( m_pVertSrc );
	m_pVertSrc = new VERTEXNORCOLORTEX_850[m_dwVert];
	
	SAFE_DELETE_ARRAY( m_pColor );
	m_pColor = new DIRECTPOINTCOLOR[m_dwVert];

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

void OBJOCTreeLightMap::CovertPieceSetting( LPDIRECT3DDEVICEQ pd3dDevice )
{
}

void OBJOCTreeLightMap::CreateVertSrc( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert )
{
	UINT nSize = D3DXGetFVFVertexSize( dwFVF );

	D3DXVECTOR3* pVector(NULL);
	D3DXVECTOR3* pNormal(NULL);
	DWORD* pDiffuse(NULL);
	D3DXVECTOR2* pTex1(NULL);
	D3DXVECTOR2* pTex2(NULL);

	if ( dwFVF == VERTEXNORCOLORTEX_338::FVF )
	{
		for( DWORD i=0; i<m_dwVert; ++i )
		{
			pVector = (D3DXVECTOR3*)( pByte + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pByte + (nSize*i) + 4*3 );
			pDiffuse =(DWORD*)( pByte + (nSize*i) + 4*6 );
			pTex1	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*7 );

			m_pVertSrc[i].vPos = *pVector;
			m_pVertSrc[i].vNor = *pNormal;
			m_pVertSrc[i].dwColor = *pDiffuse;
			m_pVertSrc[i].vTex1 = *pTex1;
		}
	}
	else if ( dwFVF == VERTEXNORTEX_530::FVF )
	{
		for( DWORD i=0; i<m_dwVert; ++i )
		{
			pVector = (D3DXVECTOR3*)( pByte + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pByte + (nSize*i) + 4*3 );
			pTex1	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*6 );
			pTex2	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*8 );

			m_pVertSrc[i].vPos = *pVector;
			m_pVertSrc[i].vNor = *pNormal;
			m_pVertSrc[i].dwColor = 0xffffffff;
			m_pVertSrc[i].vTex1 = *pTex1;
			m_pVertSrc[i].vTex2 = *pTex2;
		}
	}
	else if ( dwFVF == VERTEXNORCOLORTEX_594::FVF )
	{
		for( DWORD i=0; i<m_dwVert; ++i )
		{
			pVector = (D3DXVECTOR3*)( pByte + (nSize*i) );
			pNormal = (D3DXVECTOR3*)( pByte + (nSize*i) + 4*3 );
			pDiffuse =(DWORD*)( pByte + (nSize*i) + 4*6 );
			pTex1	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*7 );
			pTex2	= (D3DXVECTOR2*)( pByte + (nSize*i) + 4*9 );

			m_pVertSrc[i].vPos = *pVector;
			m_pVertSrc[i].vNor = *pNormal;
			m_pVertSrc[i].dwColor = *pDiffuse;
			m_pVertSrc[i].vTex1 = *pTex1;
			m_pVertSrc[i].vTex2 = *pTex2;
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

void OBJOCTreeLightMap::CreateNormalColor( BYTE* pByte, const DWORD dwFVF, const DWORD dwVert )
{
	BOOL bDiffuse = (BOOL)( dwFVF&D3DFVF_DIFFUSE );
	UINT nSize = D3DXGetFVFVertexSize( dwFVF );
	D3DXVECTOR3* pVector(NULL);
	DWORD* pDiffuse(NULL);
	for( DWORD i=0; i<m_dwVert; ++i )
	{
		pVector = (D3DXVECTOR3*)( pByte + (nSize*i) + sizeof(D3DXVECTOR3) );
		pNormalColor[i].vNor = *pVector;
		D3DXVec3Normalize( &pNormalColor[i].vNor, &pNormalColor[i].vNor );

		if( bDiffuse )
		{
			pDiffuse = (DWORD*)( pByte + (nSize*i) + sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) );

			pNormalColor[i].cColor.a = (((*pDiffuse)&0xff000000)>>24) / 255.f;
			pNormalColor[i].cColor.r = (((*pDiffuse)&0xff0000)>>16) / 255.f;
			pNormalColor[i].cColor.g = (((*pDiffuse)&0xff00)>>8) / 255.f;
			pNormalColor[i].cColor.b = ((*pDiffuse)&0xff) / 255.f;
		}
		else
		{
			pNormalColor[i].cColor.a = 1.f;
			pNormalColor[i].cColor.r = 1.f;
			pNormalColor[i].cColor.g = 1.f;
			pNormalColor[i].cColor.b = 1.f;
		}
	}

	float fColorR(0.f);
	float fColorG(0.f);
	float fColorB(0.f);
	float fDot(0.f);
	D3DXVECTOR3 vDirect(0.f,0.f,0.f);
	DWORD		dwColor(0L);

	for( DWORD i=0; i<m_dwVert; ++i )
	{
		dwColor = DxLightMan::GetInstance()->ComputeClosedPointLight( &m_pVertSrc[i].vPos, &pNormalColor[i].vNor );	// 가까운 빛들을 계산한 값을 얻는다.

		m_pColor[i].wNightR = (WORD)((dwColor&0xff0000)>>16);
		m_pColor[i].wNightG = (WORD)((dwColor&0xff00)>>8);
		m_pColor[i].wNightB = (WORD)(dwColor&0xff);
	}

	DXLIGHT* pLight = DxLightMan::GetInstance()->GetDirectNoonLight();
	vDirect.x = -pLight->m_Light.Direction.x;
	vDirect.y = -pLight->m_Light.Direction.y;
	vDirect.z = -pLight->m_Light.Direction.z;

	for( DWORD i=0; i<m_dwVert; ++i )
	{
		D3DXVec3Normalize( &vDirect, &vDirect );
		D3DXVec3Normalize( &pNormalColor[i].vNor, &pNormalColor[i].vNor );
		fDot = D3DXVec3Dot( &pNormalColor[i].vNor, &vDirect );
		if( fDot > 0.f )
		{
			fColorR = pLight->m_Light.Diffuse.r*fDot;
			fColorG = pLight->m_Light.Diffuse.g*fDot;
			fColorB = pLight->m_Light.Diffuse.b*fDot;

			fColorR += pLight->m_Light.Ambient.r;
			fColorG += pLight->m_Light.Ambient.g;
			fColorB += pLight->m_Light.Ambient.b;

			fColorR = (fColorR>1.f) ? 1.f : fColorR;
			fColorG = (fColorG>1.f) ? 1.f : fColorG;
			fColorB = (fColorB>1.f) ? 1.f : fColorB;

			m_pColor[i].wDayR = (WORD)(fColorR*255.f);
			m_pColor[i].wDayG = (WORD)(fColorG*255.f);
			m_pColor[i].wDayB = (WORD)(fColorB*255.f);
		}
		else
		{
			fColorR = pLight->m_Light.Ambient.r;
			fColorG = pLight->m_Light.Ambient.g;
			fColorB = pLight->m_Light.Ambient.b;

			m_pColor[i].wDayR = (WORD)(fColorR*255.f);
			m_pColor[i].wDayG = (WORD)(fColorG*255.f);
			m_pColor[i].wDayB = (WORD)(fColorB*255.f);
		}
	}

	// Note : 낮에도 PointLight를 끄지 않는다.
	if( !DxLightMan::GetInstance()->IsWhenDayPointOff() )
	{
		for( DWORD i=0; i<m_dwVert; ++i )
		{
			m_pColor[i].wDayR += m_pColor[i].wNightR;
			m_pColor[i].wDayG += m_pColor[i].wNightG;
			m_pColor[i].wDayB += m_pColor[i].wNightB;
		}
	}

	// Note : 밤일 경우의 컬러를 구한다.
	//
	pLight = DxLightMan::GetInstance()->GetDirectNightLight();
	vDirect.x = -pLight->m_Light.Direction.x;
	vDirect.y = -pLight->m_Light.Direction.y;
	vDirect.z = -pLight->m_Light.Direction.z;

	for( DWORD i=0; i<m_dwVert; ++i )
	{
		D3DXVec3Normalize( &vDirect, &vDirect );
		D3DXVec3Normalize( &pNormalColor[i].vNor, &pNormalColor[i].vNor );
		fDot = D3DXVec3Dot( &pNormalColor[i].vNor, &vDirect );
		if( fDot > 0.f )	// 빛과의 방향을 맞춘다.
		{
			// Note : Diffuse Color를 구한다.
			fColorR = pLight->m_Light.Diffuse.r*fDot;
			fColorG = pLight->m_Light.Diffuse.g*fDot;
			fColorB = pLight->m_Light.Diffuse.b*fDot;

			// Note : Ambient Color를 더한다.
			fColorR += pLight->m_Light.Ambient.r;
			fColorG += pLight->m_Light.Ambient.g;
			fColorB += pLight->m_Light.Ambient.b;

			fColorR = (fColorR>1.f) ? 1.f : fColorR;
			fColorG = (fColorG>1.f) ? 1.f : fColorG;
			fColorB = (fColorB>1.f) ? 1.f : fColorB;

			m_pColor[i].wNightR += (WORD)(fColorR*255.f);
			m_pColor[i].wNightG += (WORD)(fColorG*255.f);
			m_pColor[i].wNightB += (WORD)(fColorB*255.f);
		}
		else
		{
			// Note : Ambient Color를 구한다.
			fColorR = pLight->m_Light.Ambient.r;
			fColorG = pLight->m_Light.Ambient.g;
			fColorB = pLight->m_Light.Ambient.b;

			m_pColor[i].wNightR += (WORD)(fColorR*255.f);
			m_pColor[i].wNightG += (WORD)(fColorG*255.f);
			m_pColor[i].wNightB += (WORD)(fColorB*255.f);
		}
	}

	// Note : 최종 색 , 원래 가지고 있던 Diffuse 색을 곱하는 것이다.
	for( DWORD i=0; i<m_dwVert; ++i )
	{
		m_pColor[i].wAlpha = (WORD)( 255.f*pNormalColor[i].cColor.a );

		m_pColor[i].wDayR = (WORD)( m_pColor[i].wDayR * pNormalColor[i].cColor.r );
		m_pColor[i].wDayG = (WORD)( m_pColor[i].wDayG * pNormalColor[i].cColor.g );
		m_pColor[i].wDayB = (WORD)( m_pColor[i].wDayB * pNormalColor[i].cColor.b );

		m_pColor[i].wNightR = (WORD)( m_pColor[i].wNightR * pNormalColor[i].cColor.r );
		m_pColor[i].wNightG = (WORD)( m_pColor[i].wNightG * pNormalColor[i].cColor.g );
		m_pColor[i].wNightB = (WORD)( m_pColor[i].wNightB * pNormalColor[i].cColor.b );
	}

	// Note : 컬러를 임시로 복제
	WORD wTempColor(0);
	for( DWORD i=0; i<m_dwVert; ++i )
	{
		dwColor = (m_pColor[i].wAlpha<<24);

		switch( GLPeriod::GetInstance().m_sFGTime )
		{
		case FGT_DAYTIME:
			wTempColor = m_pColor[i].wDayR;
			if( wTempColor > 255 )	wTempColor = 255;
			dwColor += (wTempColor<<16);

			wTempColor = m_pColor[i].wDayG;
			if( wTempColor > 255 )	wTempColor = 255;
			dwColor += (wTempColor<<8);

			wTempColor = m_pColor[i].wDayB;
			if( wTempColor > 255 )	wTempColor = 255;
			dwColor += wTempColor;

			break;

		case FGT_DAWN:
		case FGT_SUNSET:
		case FGT_NIGHT:
			wTempColor = m_pColor[i].wNightR;
			if( wTempColor > 255 )	wTempColor = 255;
			dwColor += (wTempColor<<16);

			wTempColor = m_pColor[i].wNightG;
			if( wTempColor > 255 )	wTempColor = 255;
			dwColor += (wTempColor<<8);

			wTempColor = m_pColor[i].wNightB;
			if( wTempColor > 255 )	wTempColor = 255;
			dwColor += wTempColor;

			break;
		};

		m_pVertSrc[i].dwColor = dwColor;
	}

	SAFE_DELETE_ARRAY( pNormalColor );
}

void OBJOCTreeLightMap::CopyVertSrcToVB( BYTE* pByte, VERTEXNORCOLORTEX_850* pVertSrc, LPDIRECT3DVERTEXBUFFERQ pVB, const DWORD dwVert )
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

void OBJOCTreeLightMap::CopyMeshToIB( WORD* pSrc, LPDIRECT3DINDEXBUFFERQ pIB, const DWORD dwFaces )
{
	if( !pSrc )		return;
	if( !pIB )		return;

	WORD* pIndices(NULL);
	pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
	memcpy( pIndices, pSrc, sizeof(WORD)*3*dwFaces );
	pIB->Unlock();
}

void OBJOCTreeLightMap::InsertColorList( LOADINGDATALIST &listColorData )
{
	if( !IsLoad() )		return;
	if( !m_pColor )		return;

	DWORD dwData = (DWORD)(this);
	listColorData.push_back( dwData );
}

void OBJOCTreeLightMap::FrameMoveCOLOR()
{
	DWORD dwSub = 0;
	DWORD dwAdd = 200;
	for( DWORD i=0; i<m_dwVert; )
	{
		dwSub = m_dwVert - i;
		dwSub = (dwSub>dwAdd) ? dwAdd : dwSub;

		ComputeCOLOR( i, i+dwSub );

		i += dwSub;
		Sleep(0);
	}

	EnterCriticalSection(&m_pCSLockVB);
	{
		dwSub = 0;
		dwAdd = 1600;
		for( DWORD i=0; i<m_dwVert; )
		{
			dwSub = m_dwVert - i;
			dwSub = (dwSub>dwAdd) ? dwAdd : dwSub;

			BYTE* pVertices(NULL);
			HRESULT hr = m_pVB->Lock( m_nFVFSize*i, m_nFVFSize*dwSub, (VOID**)&pVertices, 0L );
			if( FAILED( hr ) )
			{
				LeaveCriticalSection(&m_pCSLockVB);
				return;
			}
			memcpy( pVertices, &m_pVertSrc[i], m_nFVFSize*dwSub );
			m_pVB->Unlock();

			i += dwSub;
			Sleep(0);
		}
	}
	LeaveCriticalSection(&m_pCSLockVB);
}

void OBJOCTreeLightMap::ComputeCOLOR( const DWORD& dwStart, const DWORD& dwEnd )
{
	if( !m_pColor )	return;

	DWORD dwColor(0L);
	WORD wColorR(0);
	WORD wColorG(0);
	WORD wColorB(0);
	float fBlendFact = GLPeriod::GetInstance().GetBlendFact();
	float fNightFact = GLPeriod::GetInstance().GetNightFact();
	float fBlueFact = GLPeriod::GetInstance().GetBlueFact();
	float fRedFact = GLPeriod::GetInstance().GetRedFact();

	for( DWORD i=dwStart; i<dwEnd; ++i )
	{
		dwColor = (m_pColor[i].wAlpha<<24);

		wColorR = (WORD)(m_pColor[i].wDayR*fBlendFact);
		wColorG = (WORD)(m_pColor[i].wDayG*fBlendFact);
		wColorB = (WORD)(m_pColor[i].wDayB*fBlendFact);

		wColorR += (WORD)(m_pColor[i].wNightR*fNightFact);
		wColorG += (WORD)(m_pColor[i].wNightG*fNightFact);
		wColorB += (WORD)(m_pColor[i].wNightB*fNightFact);

		if( wColorR > 255 )	wColorR = 255;
		if( wColorG > 255 )	wColorG = 255;
		if( wColorB > 255 )	wColorB = 255;

		dwColor += (wColorR<<16);
		dwColor += (wColorG<<8);
		dwColor += wColorB;

		m_pVertSrc[i].dwColor = dwColor;
	}
}

void OBJOCTreeLightMap::UpdateLightMapUV( float fMultiX, float fMultiY )
{
}