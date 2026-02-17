#include "stdafx.h"
#include "./NsOCTree.h"

#include "./Collision.h"
#include "./NsOptimizeMesh.h"

#include "../DxEffect/DxTexEffMan.h"
#include "../DxEffect/DxTexEffDiffuse.h"
#include "../DxEffect/DxTexEffFlowUV.h"
#include "../DxEffect/DxTexEffRotate.h"
#include "../DxEffect/DxTexEffSpecular.h"
#include "../DxEffect/DxTexEffVisualMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace NSOCTREE
{
	BOOL				g_bDynamicLoad = FALSE;	// Tool 에 따라 변함. Game 사용, Level Tool 미 사용.

	void OneTimeSceneInit()
	{
	}

	void CleanUp()
	{
	}

	BOOL IsDynamicLoad()
	{
		return g_bDynamicLoad;
	}

	void EnableDynamicLoad()
	{
		g_bDynamicLoad = TRUE; 
	}

	BOOL IsFaceFront ( BYTE* pSrc, const D3DXPLANE& sPlane, const UINT nSIZE )
	{
		float fResult;
		D3DXVECTOR3 *pVecter;
		for ( DWORD i=0; i<3; ++i )
		{
			pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*i );
			fResult = D3DXPlaneDotCoord ( &sPlane, pVecter );
			if ( fResult >= 0.f )	return TRUE;
		}
		return FALSE;
	}

	void DivideFace ( BYTE* pSrc, BYTE* pLeft, BYTE* pRight, DWORD dwFace, DWORD& dwLeft, DWORD& dwRight, const D3DXPLANE& sPlane, const UINT nSIZE )
	{
		D3DXVECTOR3 *pVecter;
		D3DXVECTOR3 *pDest;
		for ( DWORD i=0; i<dwFace; ++i )
		{
			// Note : 면이 앞에 있다.		 면 단위 계산
			if ( IsFaceFront ( &pSrc[nSIZE*i*3], sPlane, nSIZE ) )
			{
				pDest = (D3DXVECTOR3*)( pLeft + nSIZE*(dwLeft*3+0) );
				pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*(i*3+0) );
				*pDest = *pVecter;

				pDest = (D3DXVECTOR3*)( pLeft + nSIZE*(dwLeft*3+1) );
				pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*(i*3+1) );
				*pDest = *pVecter;

				pDest = (D3DXVECTOR3*)( pLeft + nSIZE*(dwLeft*3+2) );
				pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*(i*3+2) );
				*pDest = *pVecter;

				memcpy ( pLeft + (nSIZE*3)*dwLeft, pSrc + (nSIZE*3)*i, nSIZE*3 );

				++dwLeft;
			}
			else
			{
				pDest = (D3DXVECTOR3*)( pRight + nSIZE*(dwRight*3+0) );
				pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*(i*3+0) );
				*pDest = *pVecter;

				pDest = (D3DXVECTOR3*)( pRight + nSIZE*(dwRight*3+1) );
				pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*(i*3+1) );
				*pDest = *pVecter;

				pDest = (D3DXVECTOR3*)( pRight + nSIZE*(dwRight*3+2) );
				pVecter = (D3DXVECTOR3*)( pSrc + nSIZE*(i*3+2) );
				*pDest = *pVecter;

				memcpy ( pRight + (nSIZE*3)*dwRight, pSrc + (nSIZE*3)*i, nSIZE*3 );

				++dwRight;
			}
		}
	}

	float DISMAX = 0.f;
	DWORD FACEMAX = 0;

	void MakeOctree1 ( POBJOCTREE& pTree, const UINT nSIZE )
	{
		// Note : 이건 완료된 데이터
		float fDisX = pTree->vMax.x - pTree->vMin.x;
		float fDisY = pTree->vMax.y - pTree->vMin.y;
		float fDisZ = pTree->vMax.z - pTree->vMin.z;

		if ( (fDisX<DISMAX) && (fDisY<DISMAX) && (fDisZ<DISMAX) && (pTree->dwFace<FACEMAX) )
		{
			return;
		}

		// Note : 분할을 하자.
		DWORD	dwLeftFace = 0;
		DWORD	dwRightFace = 0;
		BYTE* pLeftVertex = new BYTE[nSIZE*pTree->dwFace*3];
		BYTE* pRightVertex = new BYTE[nSIZE*pTree->dwFace*3];

		D3DXPLANE sPlane;
		D3DXVECTOR3 vCenter = pTree->vMin;
		vCenter.x += (fDisX*0.5f);
		vCenter.y += (fDisY*0.5f);
		vCenter.z += (fDisZ*0.5f);
		if ( (fDisX>fDisY) && (fDisX>fDisZ) )	// X 길다.
		{
			D3DXVECTOR3	v0 = vCenter;
			D3DXVECTOR3	v1 = vCenter;
			D3DXVECTOR3	v2 = vCenter;
			v1.z += 1.f;
			v2.y += 1.f;
			D3DXPlaneFromPoints ( &sPlane, &v0, &v1, &v2 );
		}
		else if (fDisZ>fDisY)									// Z 길다.
		{
			D3DXVECTOR3	v0 = vCenter;
			D3DXVECTOR3	v1 = vCenter;
			D3DXVECTOR3	v2 = vCenter;
			v1.x += 1.f;
			v2.y += 1.f;
			D3DXPlaneFromPoints ( &sPlane, &v0, &v1, &v2 );
		}								
		else 
		{
			D3DXVECTOR3	v0 = vCenter;
			D3DXVECTOR3	v1 = vCenter;
			D3DXVECTOR3	v2 = vCenter;
			v1.x += 1.f;
			v2.z += 1.f;
			D3DXPlaneFromPoints ( &sPlane, &v0, &v1, &v2 );	
		}

		DivideFace ( pTree->pVertex, pLeftVertex, pRightVertex, pTree->dwFace, dwLeftFace, dwRightFace, sPlane, nSIZE );

		// Note : 
		if( !dwLeftFace || !dwRightFace )
		{
			if( pTree->dwFace>=FACEMAX )	MessageBox( NULL, "Static Mesh (Octree Action) pTree->dwFace>=FACEMAX ", "ERROR", MB_OK );

			// Normal - OK
			SAFE_DELETE_ARRAY ( pLeftVertex );
			SAFE_DELETE_ARRAY ( pRightVertex );
			return;
		}

		// Note : 필요 없어진것 삭제
		SAFE_DELETE_ARRAY ( pTree->pVertex );
		pTree->dwFace = 0;

		// Note : Left 생성
		if ( dwLeftFace )
		{
			D3DXVECTOR3 vMax = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
			D3DXVECTOR3 vMin = D3DXVECTOR3 ( FLT_MAX, FLT_MAX, FLT_MAX );
			for ( DWORD i=0; i<dwLeftFace*3; ++i )
			{
				D3DXVECTOR3 *pVecter = (D3DXVECTOR3*)( pLeftVertex + nSIZE*i );

				if ( vMax.x < pVecter->x )	vMax.x = pVecter->x;
				if ( vMax.y < pVecter->y )	vMax.y = pVecter->y;
				if ( vMax.z < pVecter->z )	vMax.z = pVecter->z;

				if ( vMin.x > pVecter->x )	vMin.x = pVecter->x;
				if ( vMin.y > pVecter->y )	vMin.y = pVecter->y;
				if ( vMin.z > pVecter->z )	vMin.z = pVecter->z;
			}

			// 충돌을 위해서
			vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
			vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

			pTree->pLeftChild = new OBJOCTree;
			pTree->pLeftChild->vMax = vMax;
			pTree->pLeftChild->vMin = vMin;
			pTree->pLeftChild->pVertex = pLeftVertex;
			pTree->pLeftChild->dwFace = dwLeftFace;

			MakeOctree1 ( pTree->pLeftChild, nSIZE );
		}
		else
		{
			SAFE_DELETE_ARRAY ( pLeftVertex );
		}

		// Note : Right 생성
		if ( dwRightFace )
		{
			D3DXVECTOR3 vMax = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
			D3DXVECTOR3 vMin = D3DXVECTOR3 ( FLT_MAX, FLT_MAX, FLT_MAX );
			for ( DWORD i=0; i<dwRightFace*3; ++i )
			{
				D3DXVECTOR3 *pVecter = (D3DXVECTOR3*)( pRightVertex + nSIZE*i );

				if ( vMax.x < pVecter->x )	vMax.x = pVecter->x;
				if ( vMax.y < pVecter->y )	vMax.y = pVecter->y;
				if ( vMax.z < pVecter->z )	vMax.z = pVecter->z;

				if ( vMin.x > pVecter->x )	vMin.x = pVecter->x;
				if ( vMin.y > pVecter->y )	vMin.y = pVecter->y;
				if ( vMin.z > pVecter->z )	vMin.z = pVecter->z;
			}

			// 충돌을 위해서
			vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
			vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

			pTree->pRightChild = new OBJOCTree;
			pTree->pRightChild->vMax = vMax;
			pTree->pRightChild->vMin = vMin;
			pTree->pRightChild->pVertex = pRightVertex;
			pTree->pRightChild->dwFace = dwRightFace;

			MakeOctree1 ( pTree->pRightChild, nSIZE );
		}
		else
		{
			SAFE_DELETE_ARRAY ( pRightVertex );
		}
	}

	void MakeOctree ( POBJOCTREE& pTree, BYTE* pVertex, const DWORD dwFace, float fDisMAX, DWORD dwFaceMAX, const UINT nSIZE )
	{
		DISMAX = fDisMAX;
		FACEMAX = dwFaceMAX;

		D3DXVECTOR3 vMax = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		D3DXVECTOR3 vMin = D3DXVECTOR3 ( FLT_MAX, FLT_MAX, FLT_MAX );

		for ( DWORD i=0; i<dwFace*3; ++i )
		{
			D3DXVECTOR3 *pVecter = (D3DXVECTOR3*)( pVertex + nSIZE*i );

			if ( vMax.x < pVecter->x )	vMax.x = pVecter->x;
			if ( vMax.y < pVecter->y )	vMax.y = pVecter->y;
			if ( vMax.z < pVecter->z )	vMax.z = pVecter->z;

			if ( vMin.x > pVecter->x )	vMin.x = pVecter->x;
			if ( vMin.y > pVecter->y )	vMin.y = pVecter->y;
			if ( vMin.z > pVecter->z )	vMin.z = pVecter->z;
		}

		// 충돌을 위해서
		vMax += D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );
		vMin -= D3DXVECTOR3( 0.0001f, 0.0001f, 0.0001f );

		pTree = new OBJOCTree;
		pTree->vMax = vMax;
		pTree->vMin = vMin;
		pTree->pVertex = new BYTE[nSIZE*dwFace*3];
		memcpy ( pTree->pVertex, pVertex, nSIZE*dwFace*3 );
		pTree->dwFace = dwFace;

		MakeOctree1 ( pTree, nSIZE );
	}




	void MakeOcMesh ( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, const DWORD dwFVF )
	{
		if ( pTree->pVertex )
		{
			SAFE_RELEASE ( pTree->pOcMesh );
			D3DXCreateMeshFVF ( pTree->dwFace, pTree->dwFace*3, D3DXMESH_MANAGED, dwFVF, pd3dDevice, &pTree->pOcMesh );
			if ( !pTree->pOcMesh )	return;

			if ( !pTree->pVertex )	return;

			UINT nSIZE = D3DXGetFVFVertexSize ( dwFVF );

			BYTE *pBuffer(NULL);
			pTree->pOcMesh->LockVertexBuffer ( 0L, (VOID**)&pBuffer );
			memcpy ( pBuffer, pTree->pVertex, nSIZE*pTree->dwFace*3 );
			pTree->pOcMesh->UnlockVertexBuffer();

			WORD* pIndices;
			pTree->pOcMesh->LockIndexBuffer ( 0L, (VOID**)&pIndices );
			for ( DWORD i=0; i<pTree->dwFace*3; ++i )
			{
				pIndices[i] = (WORD)i;
			}
			pTree->pOcMesh->UnlockIndexBuffer();

			SAFE_DELETE_ARRAY ( pTree->pVertex );
			//pTree->dwFace = 0;

			return;
		}

		if ( pTree->pLeftChild )	MakeOcMesh ( pd3dDevice, pTree->pLeftChild, dwFVF );
		if ( pTree->pRightChild )	MakeOcMesh ( pd3dDevice, pTree->pRightChild, dwFVF );
	}

	void MakeOptimizeMesh ( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree )
	{
		if ( pTree->pOcMesh )
		{
			// Note : 메쉬의 최적화
			NSOPTIMIZEMESH::MakeOptimizeMesh( pd3dDevice, pTree->pOcMesh );
			return;
		}

		if ( pTree->pLeftChild )	MakeOptimizeMesh ( pd3dDevice, pTree->pLeftChild );
		if ( pTree->pRightChild )	MakeOptimizeMesh ( pd3dDevice, pTree->pRightChild );
	}



	void MakeCollisionData2 ( LPD3DXMESH pMesh, PDXAABBNODE& pTree )
	{
		DWORD dwFaceNUM = pMesh->GetNumFaces();

		WORD *pLockIndices=NULL;
		PBYTE pbLockPoints = NULL;
		pMesh->LockIndexBuffer( 0L, (VOID**)&pLockIndices );
		pMesh->LockVertexBuffer( 0L, (VOID**)&pbLockPoints );

		WORD *pFaces;
		pFaces = new WORD[dwFaceNUM];
		if ( pFaces == NULL )	return;// E_OUTOFMEMORY;

		for ( WORD i=0; i<dwFaceNUM; i++ )
			pFaces[i] = i;

		D3DXMATRIX	matComb;
		D3DXMatrixIdentity ( &matComb );

		D3DXVECTOR3 vMax, vMin;
		COLLISION::GetSizeNode ( matComb, pLockIndices, pbLockPoints, pMesh->GetFVF(), pFaces, dwFaceNUM, vMax, vMin );

		DxAABBNode *pNode = new DxAABBNode;
		pNode->fMaxX = vMax.x;
		pNode->fMaxY = vMax.y;
		pNode->fMaxZ = vMax.z;
		pNode->fMinX = vMin.x;
		pNode->fMinY = vMin.y;
		pNode->fMinZ = vMin.z;

		COLLISION::MakeAABBNode ( pNode, matComb, pLockIndices, pbLockPoints, pMesh->GetFVF(), pFaces, dwFaceNUM, vMax, vMin );

		pTree = pNode;

		pMesh->UnlockIndexBuffer();
		pMesh->UnlockVertexBuffer();

		SAFE_DELETE_ARRAY( pFaces );
	}

	void MakeCollisionData ( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree )
	{
		if( !pTree )	return;

		if ( pTree->pOcMesh )
		{
			MakeCollisionData2 ( pTree->pOcMesh, pTree->pCollsionTREE );
			return;
		}

		if ( pTree->pLeftChild )	MakeCollisionData ( pd3dDevice, pTree->pLeftChild );
		if ( pTree->pRightChild )	MakeCollisionData ( pd3dDevice, pTree->pRightChild );
	}



	void MakeVBIBAndDelMesh( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree )
	{
		if( !pTree )	return;

		// Note : VertexBuffer, IndexBuffer, pNormal을 생성하고 Mesh는 삭제한다.
		//		Mesh는 Normal을 포함하고 있는 경우일 것이다. 분해 후 넣어야 한다.
		if( pTree->pOcMesh )
		{
			// Note : FVF를 얻는다.
			DWORD dwFVF = pTree->pOcMesh->GetFVF();
			DWORD dwVert = pTree->pOcMesh->GetNumVertices();
			DWORD dwFaces = pTree->pOcMesh->GetNumFaces();

			// Note : 기본적인 VB, IB, DIRECTPOINTCOLOR, 등을 생성한다.
			pTree->CreateBASE( pd3dDevice, dwVert, dwFaces );

			// Note : 데이터를 옮긴다.
			BYTE*	pVertices(NULL);
			pTree->pOcMesh->LockVertexBuffer( 0L, (VOID**)&pVertices );
			pTree->CreateVertSrc( pVertices, dwFVF, dwVert );				// VertSrc를 생성한다.
			pTree->CreateNormalColor( pVertices, dwFVF, dwVert );			// NormalColor를 생성한다.

			// Note : 렌더에 쓰일 VB를 셋팅한다.
			pTree->CopyVertSrcToVB( pVertices, pTree->m_pVertSrc, pTree->m_pVB, dwVert );
			pTree->pOcMesh->UnlockVertexBuffer();

			// Note : 렌더에 쓰일 IB를 셋팅한다.
			WORD* pIndexSRC(NULL);
			pTree->pOcMesh->LockIndexBuffer( 0L, (VOID**)&pIndexSRC );
			pTree->CopyMeshToIB( pIndexSRC, pTree->m_pIB, dwFaces );
			pTree->pOcMesh->UnlockIndexBuffer();

			// Note : 마지막으로 삭제한다.
			SAFE_RELEASE( pTree->pOcMesh );

			return;
		}

		if( pTree->pLeftChild )		MakeVBIBAndDelMesh( pd3dDevice, pTree->pLeftChild );
		if( pTree->pRightChild )	MakeVBIBAndDelMesh( pd3dDevice, pTree->pRightChild );
	}

	void DeleteNormalColor( OBJOCTree* pTree )
	{
		if( !pTree )	return;

		SAFE_DELETE_ARRAY( pTree->m_pColor );

		if( pTree->pLeftChild )		DeleteNormalColor( pTree->pLeftChild );
		if( pTree->pRightChild )	DeleteNormalColor( pTree->pRightChild );
	}

	void CovertPieceSetting( LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree )
	{
		if( !pTree )	return;

		pTree->CovertPieceSetting( pd3dDevice );

		if( pTree->pLeftChild )		CovertPieceSetting( pd3dDevice, pTree->pLeftChild );
		if( pTree->pRightChild )	CovertPieceSetting( pd3dDevice, pTree->pRightChild );
	}


	void MakeLoadCheckOK( OBJOCTree* pTree )
	{
		pTree->m_bLoad = TRUE;

		if ( pTree->pLeftChild )	MakeLoadCheckOK( pTree->pLeftChild );
		if ( pTree->pRightChild )	MakeLoadCheckOK( pTree->pRightChild );
	}




	void InsertColorList( OBJOCTree* pTree, LOADINGDATALIST &listColorData )
	{
		if( !pTree )	return;

		pTree->InsertColorList( listColorData );

		InsertColorList( pTree->pLeftChild, listColorData );
		InsertColorList( pTree->pRightChild, listColorData );
	}

	void UpdateLightMapUV( OBJOCTree* pTree, float fMultiX, float fMultiY )
	{
		if( !pTree )	return;

		pTree->UpdateLightMapUV( fMultiX, fMultiY );

		UpdateLightMapUV( pTree->pLeftChild, fMultiX, fMultiY );
		UpdateLightMapUV( pTree->pRightChild, fMultiX, fMultiY );
	}




	void RenderOctree ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree )
	{
		if ( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume ( sCV, pTree->vMax, pTree->vMin ) )	return;

		if ( pTree->IsLoad() )
		{
			//pTree->pOcMesh->DrawSubset(0);	// Modify
			pTree->DrawEff( pd3dDevice );
			return;
		}

		if ( pTree->pLeftChild )	RenderOctree ( pd3dDevice, sCV, pTree->pLeftChild );
		if ( pTree->pRightChild )	RenderOctree ( pd3dDevice, sCV, pTree->pRightChild );
	}

	void RenderOctree_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffFlowUV* pEff )
	{
		if ( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume ( sCV, pTree->vMax, pTree->vMin ) )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );			
			pEff->Render( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderOctree_FlowUV( pd3dDevice, sCV, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderOctree_FlowUV( pd3dDevice, sCV, pTree->pRightChild, pEff );
	}

	void RenderOctree_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffRotate* pEff )
	{
		if ( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume ( sCV, pTree->vMax, pTree->vMin ) )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );
			pEff->Render( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderOctree_Rotate( pd3dDevice, sCV, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderOctree_Rotate( pd3dDevice, sCV, pTree->pRightChild, pEff );
	}

	void RenderList ( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree )
	{
		if ( !pTree )	return;

		if ( pTree->IsLoad() )
		{
			pTree->DrawEff( pd3dDevice );
			return;
		}

		if ( pTree->pLeftChild )	RenderList( pd3dDevice, pTree->pLeftChild );
		if ( pTree->pRightChild )	RenderList( pd3dDevice, pTree->pRightChild );
	}

	void RenderList_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffFlowUV* pEff )
	{
		if ( !pTree )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );
			pEff->Render( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderList_FlowUV( pd3dDevice, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderList_FlowUV( pd3dDevice, pTree->pRightChild, pEff );
	}

	void RenderList_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffRotate* pEff )
	{
		if ( !pTree )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );
			pEff->Render( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderList_Rotate( pd3dDevice, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderList_Rotate( pd3dDevice, pTree->pRightChild, pEff );
	}


	void RenderList_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff )
	{
		if ( !pTree )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );
			pEff->RenderFlow( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderList_FlowUV( pd3dDevice, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderList_FlowUV( pd3dDevice, pTree->pRightChild, pEff );
	}

	void RenderOctree_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff )
	{
		if ( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume ( sCV, pTree->vMax, pTree->vMin ) )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );			
			pEff->RenderFlow( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderOctree_FlowUV( pd3dDevice, sCV, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderOctree_FlowUV( pd3dDevice, sCV, pTree->pRightChild, pEff );
	}


	void RenderList_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff )
	{
		if ( !pTree )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );
			pEff->RenderRotate( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderList_Rotate( pd3dDevice, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderList_Rotate( pd3dDevice, pTree->pRightChild, pEff );
	}

	void RenderOctree_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, OBJOCTree* pTree, DxTexEffVisualMaterial* pEff )
	{
		if ( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume ( sCV, pTree->vMax, pTree->vMin ) )	return;

		if ( pTree->IsLoad() )
		{
			pd3dDevice->SetIndices ( pTree->m_pIB );
			pEff->RenderRotate( pd3dDevice, pTree->m_dwVert, pTree->m_dwFace, pTree->m_pVertSrc );
			return;
		}

		if ( pTree->pLeftChild )	RenderOctree_Rotate( pd3dDevice, sCV, pTree->pLeftChild, pEff );
		if ( pTree->pRightChild )	RenderOctree_Rotate( pd3dDevice, sCV, pTree->pRightChild, pEff );
	}

	float	g_fCOLL_LENGTH = FLT_MAX;
	void IsCollision ( DxAABBNode *pAABBCur, OBJOCTree* pTree, const D3DXVECTOR3 &vStart, D3DXVECTOR3 &vEnd, 
		D3DXVECTOR3 &vCollision, D3DXVECTOR3 &vNormal, BOOL& bColl, const BOOL bFrontColl )
	{
		D3DXVECTOR3 vNewP1=vStart, vNewP2=vEnd;

		if ( pAABBCur->IsCollision(vNewP1,vNewP2) )
		{
			if ( pAABBCur->dwFace != AABB_NONINDEX)
			{
				PBYTE pbDataVB;
				PWORD pwIndexB;

				//if( FAILED( pTree->m_pVB->Lock( 0, 0, (VOID**)&pbDataVB, D3DLOCK_READONLY ) ) )
				//	return;
				pbDataVB = (PBYTE)pTree->m_pVertSrc;
				if( FAILED( pTree->m_pIB->Lock( 0, 0, (VOID**)&pwIndexB, D3DLOCK_READONLY ) ) )
				{
					//pTree->m_pVB->Unlock();
					return;
				}

				DWORD fvfsize = sizeof(VERTEXNORCOLORTEX_850);		// CAUTION : pTree->m_pVertSrc의 자료형이 바뀌었을 시 안 바꿔주면 큰 문제가 생긴다.
				PWORD pwIndex = pwIndexB + pAABBCur->dwFace*3;
				LPD3DXVECTOR3 pvT0 = (D3DXVECTOR3*) (pbDataVB+fvfsize*(*(pwIndex)));
				LPD3DXVECTOR3 pvT1 = (D3DXVECTOR3*) (pbDataVB+fvfsize*(*(pwIndex+1)));
				LPD3DXVECTOR3 pvT2 = (D3DXVECTOR3*) (pbDataVB+fvfsize*(*(pwIndex+2)));


				D3DXVECTOR3 vNewCollision;
				D3DXVECTOR3 vNewNormal;
				vNewP1=vStart;
				if ( COLLISION::IsLineTriangleCollision( pvT0, pvT1, pvT2, &vNewP1, &vNewP2, &vNewCollision, &vNewNormal, bFrontColl ) )
				{
					bColl = TRUE;		// 충돌 되었다는 것 확인.

					D3DXVECTOR3 vDxVect;
					vDxVect = vStart - vNewCollision;
					float fNewLength = D3DXVec3LengthSq ( &vDxVect );

					if ( fNewLength < g_fCOLL_LENGTH )
					{
						g_fCOLL_LENGTH = fNewLength;
						vCollision	= vNewCollision;
						vNormal		= vNewNormal;

						vEnd		= vCollision;
					}
				}

				//pTree->m_pVB->Unlock();
				pTree->m_pIB->Unlock();
			}
			else
			{
				if ( pAABBCur->pLeftChild )
					IsCollision ( pAABBCur->pLeftChild, pTree, vStart, vEnd, vCollision, vNormal, bColl, bFrontColl );

				if ( pAABBCur->pRightChild )
					IsCollision ( pAABBCur->pRightChild, pTree, vStart, vEnd, vCollision, vNormal, bColl, bFrontColl );
			}
		}
	}

	void CollisionLine2( OBJOCTree* pTree, const D3DXVECTOR3 &vStart, D3DXVECTOR3 &vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, 
		BOOL& bColl, const BOOL bFrontColl )
	{
		if ( !pTree )	return;

		if ( !COLLISION::IsCollisionLineToAABB( vStart, vEnd, pTree->vMax, pTree->vMin ) )	return;

		if ( pTree->pCollsionTREE )
		{
			IsCollision( pTree->pCollsionTREE, pTree, vStart, vEnd, vColl, vNor, bColl, bFrontColl );
		}

		if ( pTree->pLeftChild )	CollisionLine2( pTree->pLeftChild, vStart, vEnd, vColl, vNor, bColl, bFrontColl );
		if ( pTree->pRightChild )	CollisionLine2( pTree->pRightChild, vStart, vEnd, vColl, vNor, bColl, bFrontColl );
	}

	void CollisionLine ( OBJOCTree* pTree, const D3DXVECTOR3 &vStart, D3DXVECTOR3 &vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor,
		BOOL& bColl, const BOOL bFrontColl )
	{
		g_fCOLL_LENGTH	= FLT_MAX;
		CollisionLine2 ( pTree, vStart, vEnd, vColl, vNor, bColl, bFrontColl );
	}


	void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, OBJOCTree* pTree, VECDXMESH& vecMesh )
	{
		if ( pTree->m_pVB && pTree->m_pIB && pTree->m_dwVert && pTree->m_dwFace && pTree->m_dwFVF )
		{
			SCOPYMESHDATA meshData;
			meshData.pVB = pTree->m_pVB;
			meshData.pIB = pTree->m_pIB;
			meshData.dwVert = pTree->m_dwVert;
			meshData.dwFace = pTree->m_dwFace;
			meshData.dwFVF = pTree->m_dwFVF;

			vecMesh.push_back( meshData );
		}

		if ( pTree->pLeftChild )	CopyMesh ( pd3dDevice, pTree->pLeftChild, vecMesh );
		if ( pTree->pRightChild )	CopyMesh ( pd3dDevice, pTree->pRightChild, vecMesh );
	}
};


