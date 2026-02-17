#include "stdafx.h"
#include "./DxShadowMesh.h"
#include "./DxSoftShadowMap.h"
#include "./DxRenderStates.h"
#include "./DxSurfaceTex.h"
#include "./DxViewPort.h"
#include "./DxFogMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DxShadowMeshTree::DxShadowMeshTree ()
	: m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX)
	, m_vMin(FLT_MAX,FLT_MAX,FLT_MAX)
	, m_dwVert(0)
	, m_dwFace(0)
	, m_dwFVF(0)
	, m_nFVFSize(0)
	, m_pVertex(NULL)
	, m_pIndex(NULL)
	, m_pVB(NULL)
	, m_pIB(NULL)
	, m_pNext(NULL)
{
};

DxShadowMeshTree::~DxShadowMeshTree ()
{
	CleanUp();
	SAFE_DELETE(m_pNext);
};

void DxShadowMeshTree::CleanUp()
{
	SAFE_DELETE_ARRAY(m_pVertex);
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

BOOL DxShadowMeshTree::Create( LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwVert, DWORD dwFace )
{
	m_dwVert = dwVert;
	m_dwFace = dwFace;
	m_dwFVF = VERTEX_SHADOW::FVF;
	m_nFVFSize = D3DXGetFVFVertexSize( m_dwFVF );

	//create vertex buffer
	SAFE_RELEASE(m_pVB);
	pd3dDevice->CreateVertexBuffer( m_dwVert*m_nFVFSize, D3DUSAGE_WRITEONLY, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL );
	if(!m_pVB)
	{
		CDebugSet::MsgBox( "failed to create pVB!" );
		return FALSE;
	}

	//create index buffer
	SAFE_RELEASE(m_pIB);
	pd3dDevice->CreateIndexBuffer( m_dwFace*3*sizeof(WORD), 0L, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL );
	if(!m_pIB)
	{
		CDebugSet::MsgBox( "failed to create pIB!" );
		return FALSE;
	}

	//create vertex and index array to fill data
	SAFE_DELETE_ARRAY(m_pVertex);
	m_pVertex = new VERTEX_SHADOW[m_dwVert];

	SAFE_DELETE_ARRAY(m_pIndex);
	m_pIndex = new WORD[m_dwFace*3];

	return TRUE;
}

BOOL DxShadowMeshTree::CopyMesh( LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	DWORD dwvert(0);
	DWORD dwface(0);
	DWORD dwlastvert(0);

	for ( unsigned int i=0; i<vecMesh.size(); ++i )
	{
		dwvert += vecMesh[i].dwVert;
		dwface += vecMesh[i].dwFace;
	}

	if ( dwvert == 0 || dwvert >= 65535 )	return FALSE;
	if ( dwface == 0 || dwface >= 65535 )	return FALSE; 

	BOOL bCreate = Create( pd3dDevice, dwvert, dwface );
	if ( !bCreate )	return FALSE;

	dwvert = 0;
	dwface = 0;
	dwlastvert = 0;
	for ( unsigned int i=0; i<vecMesh.size(); ++i )
	{
		SCOPYMESHDATA data = vecMesh[i];

		if ( data.pVB && data.pIB && data.dwVert && data.dwFace && data.dwFVF )
		{
			int nfvf = D3DXGetFVFVertexSize( data.dwFVF );

			BYTE* pVertices;
			data.pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
			for( DWORD j=0; j<data.dwVert; ++j )
			{
				m_pVertex[dwvert++].vPos = *(D3DXVECTOR3*)( pVertices + (nfvf*j) );
			}
			data.pVB->Unlock();

			BYTE* pIndices;
			data.pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
			for( DWORD j=0; j<data.dwFace; ++j )
			{
				m_pIndex[dwface++] = *(WORD*)( pIndices + (6*j) ) + (WORD)dwlastvert;
				m_pIndex[dwface++] = *(WORD*)( pIndices + (6*j) + 2 ) + (WORD)dwlastvert;
				m_pIndex[dwface++] = *(WORD*)( pIndices + (6*j) + 4 ) + (WORD)dwlastvert;
			}
			data.pIB->Unlock();

			dwlastvert += data.dwVert;
		}
	}

	BYTE* pVertices;
	m_pVB->Lock( 0, 0, (VOID**)&pVertices, 0L );
	memcpy( pVertices, m_pVertex, sizeof(VERTEX_SHADOW) * m_dwVert );
	m_pVB->Unlock();

	BYTE* pIndices;
	m_pIB->Lock( 0, 0, (VOID**)&pIndices, 0L );
	memcpy( pIndices, m_pIndex, sizeof(WORD) * 3 * m_dwFace );
	m_pIB->Unlock();

	for ( DWORD i=0; i<m_dwVert; ++i )
	{
		if ( m_vMax.x < m_pVertex[i].vPos.x )	m_vMax.x = m_pVertex[i].vPos.x;
		if ( m_vMax.y < m_pVertex[i].vPos.y )	m_vMax.y = m_pVertex[i].vPos.y;
		if ( m_vMax.z < m_pVertex[i].vPos.z )	m_vMax.z = m_pVertex[i].vPos.z;

		if ( m_vMin.x > m_pVertex[i].vPos.x )	m_vMin.x = m_pVertex[i].vPos.x;
		if ( m_vMin.y > m_pVertex[i].vPos.y )	m_vMin.y = m_pVertex[i].vPos.y;
		if ( m_vMin.z > m_pVertex[i].vPos.z )	m_vMin.z = m_pVertex[i].vPos.z;
	}

	return TRUE;

}

void DxShadowMeshTree::Render( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( !m_pVB )	return;
	if ( !m_pIB )	return;

	pd3dDevice->SetFVF ( VERTEX_SHADOW::FVF );
	pd3dDevice->SetStreamSource ( 0, m_pVB, 0, sizeof(VERTEX_SHADOW) );
	pd3dDevice->SetIndices ( m_pIB );
	pd3dDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST, 0, 0, m_dwVert, 0, m_dwFace );
}

void DxShadowMeshTree::RenderTest( LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD	dwWireFrame, dwAmbient;
	pd3dDevice->GetRenderState ( D3DRS_AMBIENT,		&dwAmbient );
	pd3dDevice->GetRenderState ( D3DRS_FILLMODE,	&dwWireFrame );

	pd3dDevice->SetRenderState ( D3DRS_AMBIENT,		0xffffffff );
	pd3dDevice->SetRenderState ( D3DRS_FILLMODE,	D3DFILL_WIREFRAME );

	pd3dDevice->SetTexture( 0, NULL );
	pd3dDevice->SetFVF( m_dwFVF );
	pd3dDevice->SetStreamSource( 0, m_pVB, 0, m_nFVFSize );
	pd3dDevice->SetIndices( m_pIB );
	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_dwVert, 0, m_dwFace );

	pd3dDevice->SetRenderState ( D3DRS_AMBIENT,		dwAmbient );
	pd3dDevice->SetRenderState ( D3DRS_FILLMODE,	dwWireFrame );
}

void DxShadowMeshTree::ShadowMakeUV ( const D3DXMATRIX& matDirect )
{
	if ( !m_pVB )	return;

	VERTEX_SHADOW*	pVertices;
	m_pVB->Lock ( 0, 0, (VOID**)&pVertices, D3DLOCK_DISCARD );
	
	//calculations only work with shadows calculated using directional lights
	for ( DWORD i=0; i<m_dwVert; ++i )
	{
		D3DXVECTOR2	pD;
		float		fDis;
		DWORD		dwColor;

		pD.x = matDirect._11*m_pVertex[i].vPos.x + matDirect._21*(m_pVertex[i].vPos.y-0.05f) + matDirect._31*m_pVertex[i].vPos.z + matDirect._41;
		pD.y = matDirect._12*m_pVertex[i].vPos.x + matDirect._22*(m_pVertex[i].vPos.y-0.05f) + matDirect._32*m_pVertex[i].vPos.z + matDirect._42;
		fDis = matDirect._14*m_pVertex[i].vPos.x + matDirect._24*(m_pVertex[i].vPos.y-0.05f) + matDirect._34*m_pVertex[i].vPos.z + matDirect._44;

		pD.x = pD.x/fDis;
		pD.y = pD.y/fDis;

		pVertices[i].vTex = pD;

		if ( (pD.x>=0.f) && (pD.x<=1.f) && (pD.y>=0.f) && (pD.y<=1.f) )
		{
			pD.x = (pD.x - 0.5f) * 2.f;
			pD.y = (pD.y - 0.5f) * 2.f;

			pD.x = fabsf ( pD.x );
			pD.y = fabsf ( pD.y );

			pD.x = 1.f - pD.x;
			pD.y = 1.f - pD.y;

			pD.x = (pD.x<=pD.y) ? pD.x: pD.y;
			if ( pD.x <= 0.f )	pD.x = 0.f;

			dwColor = (DWORD)(pD.x*255.f);

			pVertices[i].dwColor = 0xff000000 + (dwColor<<16) + (dwColor<<8) + dwColor;
		}
		else
		{
			pVertices[i].dwColor = 0xff000000;
		}
	}

	m_pVB->Unlock();
}


DxShadowMesh::DxShadowMesh ()
	: m_pHead(NULL)
{
};

DxShadowMesh::~DxShadowMesh ()
{
	CleanUp();
};

void DxShadowMesh::CleanUp()
{
	if ( m_pHead )
	{
		m_pHead->CleanUp();
		SAFE_DELETE( m_pHead );
	}
}

BOOL DxShadowMesh::CopyMesh( LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	if ( vecMesh.empty() )	return FALSE;

	DWORD dwvert(0);
	DWORD dwface(0);
	VECDXMESH vecmesh_new;

	SAFE_DELETE(m_pHead);
	m_pHead = new DxShadowMeshTree;

	for ( unsigned int i=0; i<vecMesh.size(); ++i )
	{
		if ( dwvert + vecMesh[i].dwVert >= 65000 || dwface + vecMesh[i].dwFace >= 65000 )
		{
			m_pHead->CopyMesh( pd3dDevice, vecmesh_new );

			DxShadowMeshTree* ptree = new DxShadowMeshTree;
			ptree->m_pNext = m_pHead;
			m_pHead = ptree;

			vecmesh_new.clear();
			dwvert = 0;
			dwface = 0;
		}

		vecmesh_new.push_back( vecMesh[i] );
		dwvert += vecMesh[i].dwVert;
		dwface += vecMesh[i].dwFace;
	}

	if ( vecmesh_new.size() )
	{
		m_pHead->CopyMesh( pd3dDevice, vecmesh_new );
	}

	return TRUE;
}

void DxShadowMesh::Render( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld )
{
	if ( DxSoftShadowMap::GetInstance().GetShadowDetail() == SOFT_SHADOW_DISABLE )	return;


	DxRenderStates::GetInstance().SetShadowColor ( pd3dDevice );

	DWORD dwState0, dwState1;
	pd3dDevice->GetRenderState ( D3DRS_SRCBLEND, &dwState0 );
	pd3dDevice->GetRenderState ( D3DRS_DESTBLEND, &dwState1 );

	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND,			D3DBLEND_ZERO );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND,			D3DBLEND_INVSRCCOLOR );

	D3DXMATRIX texMat;
	D3DXMATRIX	matLastWVP_D;
	matLastWVP_D = DxSoftShadowMap::GetInstance().GetMatrix_D ();

	//set special texture matrix for shadow mapping
	float fOffset = 0.5f + ( 0.5f/2048.f );

	D3DXMATRIX texScaleBiasMat
		( 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,  0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		fOffset, fOffset, 0.0f, 1.0f );

	D3DXMatrixMultiply(&texMat, &matLastWVP_D, &texScaleBiasMat);

	pd3dDevice->SetTexture ( 0, DxSurfaceTex::GetInstance().m_pShadowTex2048 );

	DxShadowMeshTree* pmesh = m_pHead;
	while( pmesh )
	{
		pmesh->ShadowMakeUV( texMat );
		pmesh->Render( pd3dDevice );
		pmesh = pmesh->m_pNext;
	}

	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, dwState0 );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, dwState1 );

	DxRenderStates::GetInstance().ReSetShadowColor ( pd3dDevice );
}

void DxShadowMesh::RenderTest( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld )
{
	D3DXMATRIX matW;
	pd3dDevice->GetTransform( D3DTS_WORLD, &matW );

	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	DxShadowMeshTree* pmesh = m_pHead;
	while( pmesh )
	{
		pmesh->RenderTest( pd3dDevice );
		pmesh = pmesh->m_pNext;
	}

	pd3dDevice->SetTransform( D3DTS_WORLD, &matW );
}