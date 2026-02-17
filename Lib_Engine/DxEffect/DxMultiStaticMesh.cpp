#include "stdafx.h"
#include "DxMultiStaticMesh.h"
#include "../Common/StlFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxMultiStaticMesh::DxMultiStaticMesh() 
	: m_vMin(FLT_MAX,FLT_MAX,FLT_MAX)
	, m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX)
{
}

DxMultiStaticMesh::~DxMultiStaticMesh()
{
	CleanUp();

	m_SFile.CloseFile();
}

void DxMultiStaticMesh::StartThread( LPDIRECT3DDEVICEQ pd3dDevice )
{
	InitializeCriticalSection(&m_CSLockLoading);
	m_sThread.StartThread( pd3dDevice, &m_SFile, &m_listLoadingData, &m_CSLockLoading );
}

void DxMultiStaticMesh::EndThread()
{
	DxStaticMeshColor_THREAD::GetInstance().DeleteList();	//	관련 List를 지운다.

	m_sThread.EndThread();
	DeleteCriticalSection(&m_CSLockLoading);
}

void DxMultiStaticMesh::CleanUp()
{
	m_vMin = D3DXVECTOR3 (FLT_MAX,FLT_MAX,FLT_MAX);
	m_vMax = D3DXVECTOR3 (-FLT_MAX,-FLT_MAX,-FLT_MAX);

	std::for_each ( m_vecStaticMesh.begin(), m_vecStaticMesh.end(), std_afunc::DeleteObject() );
	m_vecStaticMesh.clear();
}

void DxMultiStaticMesh::CalculateAABB()
{
	m_vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	m_vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );

	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		D3DXVECTOR3	vMax = pmesh->GetAABBMax();
		D3DXVECTOR3	vMin = pmesh->GetAABBMin();

		if ( m_vMax.x < vMax.x )	m_vMax.x = vMax.x;
		if ( m_vMax.y < vMax.y )	m_vMax.y = vMax.y;
		if ( m_vMax.z < vMax.z )	m_vMax.z = vMax.z;

		if ( m_vMin.x > vMin.x )	m_vMin.x = vMin.x;
		if ( m_vMin.y > vMin.y )	m_vMin.y = vMin.y;
		if ( m_vMin.z > vMin.z )	m_vMin.z = vMin.z;
	}
}

void DxMultiStaticMesh::FrameMove( const float fTime, const float fElapsedTime )
{
	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->FrameMove( fTime, fElapsedTime );
	}
}

void DxMultiStaticMesh::Render_THREAD_( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_( pd3dDevice, sCV, m_listLoadingData, m_CSLockLoading );
	}
}

void DxMultiStaticMesh::Render_THREAD_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_Alpha( pd3dDevice, sCV, m_listLoadingData, m_CSLockLoading );
	}
}

void DxMultiStaticMesh::Render_THREAD_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_SoftAlpha( pd3dDevice, sCV, m_listLoadingData, m_CSLockLoading );
	}
}

BOOL DxMultiStaticMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl )
{
	BOOL bUse = FALSE;

	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		if ( pmesh->IsCollisionLine( vStart, vEnd, vColl, vNor, szName, bFrontColl ) ) 
		{
			bUse = TRUE;
			break;
		}
	}
	
	return bUse;
}

BOOL DxMultiStaticMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	BOOL bUse = FALSE;

	for( int i=0; i<(int)m_vecStaticMesh.size(); ++i )
	{
		DxStaticMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		if ( pmesh->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) ) 
		{
			bUse = TRUE;
			break;
		}
	}

	return bUse;
}

void DxMultiStaticMesh::MakeAABBOCTree( const LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* const pFrameMesh, const BOOL bProgress, BOOL bPiece )
{
	CleanUp();

	DxStaticMesh* pstatic_mesh = new DxStaticMesh;
	pstatic_mesh->MakeAABBOCTree( pd3dDevice, pFrameMesh, bProgress, bPiece );
	m_vecStaticMesh.push_back( pstatic_mesh );

	CalculateAABB();
}