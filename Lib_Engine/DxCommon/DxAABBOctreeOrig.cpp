#include "stdafx.h"
#include "./DxAABBOctreeOrig.h"

#include "./OBJOCTree.h"
#include "./NsOCTree.h"


#include "../DxEffect/DxTexEffMan.h"
#include "../DxEffect/DxTexEffDiffuse.h"
#include "../DxEffect/DxTexEffFlowUV.h"
#include "../DxEffect/DxTexEffRotate.h"
#include "../DxEffect/DxTexEffSpecular.h"
#include "../DxEffect/DxTexEffVisualMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD DxAABBOctreeOrig::TYPEID = EMAABBCTREE_TYPE_ORIG;

DxAABBOctreeOrig::DxAABBOctreeOrig()
{
};

DxAABBOctreeOrig::~DxAABBOctreeOrig()
{
	CleanUpDerived();
}

void DxAABBOctreeOrig::CleanUpDerived()
{
}

void DxAABBOctreeOrig::Create ( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, EM_LAND_TYPE emLandType )
{
	// Note : 처음 다 삭제
	CleanUpBase();
	CleanUpDerived();

	// Note : 정점 데이터 얻기
	if ( !pVertices )	return;

	UINT nVERSIZE = D3DXGetFVFVertexSize ( dwFVF );

	BYTE*	pVertex = new BYTE[nVERSIZE*dwFaceNUM*3];
	memcpy ( pVertex, pVertices, nVERSIZE*dwFaceNUM*3 );

	for ( DWORD i=0; i<dwFaceNUM*3; ++i )
	{
		D3DXVECTOR3 *pVecter = (D3DXVECTOR3*)( pVertices + nVERSIZE*i );

		if ( m_vMax.x < pVecter->x )	m_vMax.x = pVecter->x;
		if ( m_vMax.y < pVecter->y )	m_vMax.y = pVecter->y;
		if ( m_vMax.z < pVecter->z )	m_vMax.z = pVecter->z;

		if ( m_vMin.x > pVecter->x )	m_vMin.x = pVecter->x;
		if ( m_vMin.y > pVecter->y )	m_vMin.y = pVecter->y;
		if ( m_vMin.z > pVecter->z )	m_vMin.z = pVecter->z;
	}

	// Note : Octree 형식으로 분할
	NSOCTREE::MakeOctree( m_pOcTree, pVertex, dwFaceNUM, 1500.f, 20000, nVERSIZE );
	SAFE_DELETE_ARRAY( pVertex );

	// Note : OcMesh 만들기
	NSOCTREE::MakeOcMesh( pd3dDevice, m_pOcTree, dwFVF );

	// Note : OcMesh 최적화
	NSOCTREE::MakeOptimizeMesh( pd3dDevice, m_pOcTree );

	// Note : Collision 만들기
	NSOCTREE::MakeCollisionData( pd3dDevice, m_pOcTree );

	// Note : 현 Object의 Type을 설정한다.
	switch( emLandType )
	{
	case EMLR_DAYNIGHT_ON:
		NSOCTREE::MakeVBIBAndDelMesh( pd3dDevice, m_pOcTree );	// OcMesh를 m_pVB, m_pIB, pNormalColor쪽으로 옮기고, 자신은 삭제한다.
		break;

	case EMLR_DAYNIGHT_OFF:
		NSOCTREE::MakeVBIBAndDelMesh( pd3dDevice, m_pOcTree );	// OcMesh를 m_pVB, m_pIB, pNormalColor쪽으로 옮기고, 자신은 삭제한다.
		NSOCTREE::DeleteNormalColor( m_pOcTree );				// 여기서 pNormalColor은 쓰이지 않는다.
		break;

	case EMLR_PIECE:
		NSOCTREE::CovertPieceSetting( pd3dDevice, m_pOcTree );
		break;
	};

	NSOCTREE::MakeLoadCheckOK( m_pOcTree );					// Load 다 되있다고 설정하자. 안하면 기본 파일이 없어서 로딩완료를 알지 못한다.
}

