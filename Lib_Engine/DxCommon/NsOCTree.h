// NsOcTree.h: interface for the CCollision class.
//
//	class DxAABBOctree : 정점 데이터와 면 갯수를 넣으면 Octree 형식의 최적화된 OptimizeMesh 를 만든다.
//
//	Name : Sung-Hwan Han
//	Begin :2004/10/21
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSOCTREE_H__INCLUDED_)
#define AFX_NSOCTREE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./OBJOCTree.h"
#include "./OBJOCTreeOrig.h"
#include "./OBJOCTreeLightMap.h"
#include "./OBJOCTreeMaterial.h"
#include "../Common/SerialFile.h"
#include "./LandTypeDef.h"
#include "./DxShadowMeshData.h"

class DxTexEffBase;
class DxTexEffFlowUV;
class DxTexEffRotate;
class DxTexEffVisualMaterial;

namespace NSOCTREE
{
	void OneTimeSceneInit();
	void CleanUp();

	void EnableDynamicLoad();
	BOOL IsDynamicLoad();

	extern float g_fCOLL_LENGTH;
	void MakeOctree( POBJOCTREE& pTree, BYTE* pVertex, DWORD dwFace, float fDisMAX, DWORD dwFaceMAX, const UINT nSIZE );	// Tree 를 만든다.
	void MakeOcMesh( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, const DWORD dwFVF );								// Tree 안에 있는 Mesh 데이터를 생성한다.
	void MakeOptimizeMesh( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree );											// Tree 안에 있는 Mesh 데이터를 최적화 한다.
	void MakeCollisionData( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree );											// Tree 안에 Collsion 데이터를 만든다.
	void MakeLoadCheckOK( OBJOCTree* pTree );

	void MakeVBIBAndDelMesh( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree );	// Tree 안에 Mesh를 삭제하고 VB, IB, Normal을 빼낸다.
	void DeleteNormalColor( OBJOCTree* pTree );											// Normal을 삭제
	void CovertPieceSetting( LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree );			// Mesh를 Piece에 맞게 셋팅해준다.

	void InsertColorList( OBJOCTree* pTree, LOADINGDATALIST &listColorData );
	void UpdateLightMapUV( OBJOCTree* pTree, float fMultiX, float fMultiY );
	void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, VECDXMESH& vecMesh );	

	void RenderOctree( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree );
	void RenderOctree_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffFlowUV* pEff );
	void RenderOctree_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffRotate* pEff );
	void RenderList( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree );
	void RenderList_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffFlowUV* pEff );
	void RenderList_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffRotate* pEff );

	void RenderList_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff );
	void RenderOctree_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff );
	void RenderList_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff );
	void RenderOctree_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff );

	void AddNormalMesh( OBJOCTree* pTree );
	void CollisionLine( OBJOCTree* pTree, const D3DXVECTOR3 &vStart, D3DXVECTOR3 &vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, BOOL& bColl, const BOOL bFrontColl );

	void SaveOctree ( CSerialFile& SFile, OBJOCTree* pTree, BOOL bPiece );
	void LoadOctree_OLD ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece );
	void LoadOctree_TYPE_0( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType );
	void LoadOctree_TYPE_1( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType );
	void LoadOctree_TYPE_2( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType );
	void LoadOctree_TYPE_Select( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType );

	void FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, OBJOCTree* pTree, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin );
	void ThreadLoad( OBJOCTree* pTree, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading );
	void DynamicLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, OBJOCTree* pTree, const CLIPVOLUME &sCV );
	void LoadOctreeVER_100( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree );
};

#endif // !defined(AFX_NSOCTREE_H__INCLUDED_)
