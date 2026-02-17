#include "stdafx.h"
#include "DxMultiStaticMaterialMesh.h"
#include "../Common/StlFunctions.h"

#include "../DxCommon/MaterialShaderParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxMultiStaticMaterialMesh::DxMultiStaticMaterialMesh() 
	: m_vMin(FLT_MAX,FLT_MAX,FLT_MAX)
	, m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX)
{
}

DxMultiStaticMaterialMesh::~DxMultiStaticMaterialMesh()
{
	CleanUp();

	m_SFile.CloseFile();
}

void DxMultiStaticMaterialMesh::StartThread( LPDIRECT3DDEVICEQ pd3dDevice )
{
	InitializeCriticalSection(&m_CSLockLoading);
	m_sThread.StartThread( pd3dDevice, &m_SFile, &m_listLoadingData, &m_CSLockLoading );
}

void DxMultiStaticMaterialMesh::EndThread()
{
	DxStaticMaterialMeshColor_THREAD::GetInstance().DeleteList();	//	관련 List를 지운다.

	m_sThread.EndThread();
	DeleteCriticalSection(&m_CSLockLoading);
}

void DxMultiStaticMaterialMesh::CleanUp()
{
	m_vMin = D3DXVECTOR3 (FLT_MAX,FLT_MAX,FLT_MAX);
	m_vMax = D3DXVECTOR3 (-FLT_MAX,-FLT_MAX,-FLT_MAX);

	std::for_each ( m_vecStaticMesh.begin(), m_vecStaticMesh.end(), std_afunc::DeleteObject() );
	m_vecStaticMesh.clear();
}

void DxMultiStaticMaterialMesh::CalculateAABB()
{
	m_vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	m_vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );

	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
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

void DxMultiStaticMaterialMesh::FrameMove( const float fTime, const float fElapsedTime )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->FrameMove( fTime, fElapsedTime );
	}
}

void DxMultiStaticMaterialMesh::Render_THREAD_Normal( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_Normal( pd3dDevice, sCV, material );
	}
}

void DxMultiStaticMaterialMesh::Render_THREAD_Alpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_Alpha( pd3dDevice, sCV, material );
	}
}

void DxMultiStaticMaterialMesh::Render_THREAD_SoftAlpha( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_SoftAlpha( pd3dDevice, sCV, material );
	}
}

void DxMultiStaticMaterialMesh::Render_THREAD_Blending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_Blending( pd3dDevice, sCV, material );
	}
}

void DxMultiStaticMaterialMesh::Render_THREAD_SpecularBlending( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_SpecularBlending( pd3dDevice, sCV, material );
	}
}

void DxMultiStaticMaterialMesh::Render_THREAD_MaterialEffect( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->Render_THREAD_MaterialEffect( pd3dDevice, sCV, material );
	}
}

BOOL DxMultiStaticMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, LPCSTR& szName, const BOOL bFrontColl )
{
	BOOL bUse = FALSE;

	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		if ( pmesh->IsCollisionLine( vStart, vEnd, vColl, vNor, szName, bFrontColl ) ) 
		{
			bUse = TRUE;
			break;
		}
	}
	
	return bUse;
}

BOOL DxMultiStaticMaterialMesh::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	BOOL bUse = FALSE;

	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		if ( pmesh->IsCollisionLine( vStart, vEnd, vColl, vNor, bFrontColl ) ) 
		{
			bUse = TRUE;
			break;
		}
	}

	return bUse;
}

void DxMultiStaticMaterialMesh::CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	for( unsigned int i=0; i<m_vecStaticMesh.size(); ++i )
	{
		DxStaticMaterialMesh* pmesh = m_vecStaticMesh[i];
		if ( !pmesh )	continue;

		pmesh->CopyMesh( pd3dDevice, vecMesh );

	}
}