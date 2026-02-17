#include "stdafx.h"
#include "./NsOCTree.h"

#include "./Collision.h"


#ifdef _DEBUG                                                                                                                                                                                                 
#define new DEBUG_NEW
#endif


namespace NSOCTREE
{

	void SaveOctree ( CSerialFile& SFile, OBJOCTree* pTree, BOOL bPiece )
	{
		if ( pTree )
		{
			SFile << (BOOL)TRUE;
			pTree->Save ( SFile, bPiece );
		}
		else
		{
			SFile << (BOOL)FALSE;
			return;
		}

		SaveOctree ( SFile, pTree->pLeftChild, bPiece );
		SaveOctree ( SFile, pTree->pRightChild, bPiece );
	}

	void LoadOctree_OLD ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece )
	{
		BOOL bUse = TRUE;
		SFile >> bUse;

		if ( bUse )
		{
			pTree = new OBJOCTreeOrig;
			pTree->Load_OLD( pd3dDevice, SFile, bDynamicLoad, bPiece );

			LoadOctree_OLD( pd3dDevice, SFile, pTree->pLeftChild, bDynamicLoad, bPiece );
			LoadOctree_OLD( pd3dDevice, SFile, pTree->pRightChild, bDynamicLoad, bPiece );
		}
	}

	void LoadOctree_TYPE_0 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType )
	{
		BOOL bUse = TRUE;
		SFile >> bUse;

		if ( bUse )
		{
			pTree = new OBJOCTreeOrig; //type 0
			pTree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );

			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pLeftChild, bDynamicLoad, bPiece, dwType );
			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pRightChild, bDynamicLoad, bPiece, dwType );
		}
	}

	void LoadOctree_TYPE_1 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType )
	{
		BOOL bUse = TRUE;
		SFile >> bUse;

		if ( bUse )
		{
			pTree = new OBJOCTreeLightMap;
			pTree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );

			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pLeftChild, bDynamicLoad, bPiece, dwType );
			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pRightChild, bDynamicLoad, bPiece, dwType );
		}
	}

	void LoadOctree_TYPE_2 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType )
	{
		BOOL bUse = TRUE;
		SFile >> bUse;

		if ( bUse )
		{
			pTree = new OBJOCTreeMaterial;
			pTree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );

			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pLeftChild, bDynamicLoad, bPiece, dwType );
			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pRightChild, bDynamicLoad, bPiece, dwType );
		}
	}

	void LoadOctree_TYPE_Select ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree, const BOOL bDynamicLoad, BOOL bPiece, DWORD dwType )
	{
		BOOL bUse = TRUE;
		SFile >> bUse;

		if ( bUse )
		{
			//add type option
			if ( dwType == EMOBJOCTREE_TYPE_ORIG )
			{
				pTree = new OBJOCTreeOrig;
				pTree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
			}
			else if ( dwType == EMOBJOCTREE_TYPE_LIGHTMAP )
			{
				pTree = new OBJOCTreeLightMap;
				pTree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
			}
			else if ( dwType == EMOBJOCTREE_TYPE_MATERIAL )
			{
				pTree = new OBJOCTreeMaterial;
				pTree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
			}
			
			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pLeftChild, bDynamicLoad, bPiece, dwType );
			LoadOctree_TYPE_Select( pd3dDevice, SFile, pTree->pRightChild, bDynamicLoad, bPiece, dwType );
		}
	}


	void FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, OBJOCTree* pTree, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin )
	{
		if( !pTree )	return;

		if ( !COLLISION::IsCollisionAABBToAABB( vMax, vMin, pTree->vMax, pTree->vMin ) )	return;

		pTree->DynamicLoad( pd3dDevice, SFile, FALSE );

		FirstLoad( pd3dDevice, SFile, pTree->pLeftChild, vMax, vMin );
		FirstLoad( pd3dDevice, SFile, pTree->pRightChild, vMax, vMin );
	}

	void DynamicLoad ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, OBJOCTree* pTree, const CLIPVOLUME &sCV )
	{
		if( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume( sCV, pTree->vMax, pTree->vMin ) )	return;

		pTree->DynamicLoad( pd3dDevice, SFile, FALSE );

		DynamicLoad ( pd3dDevice, SFile, pTree->pLeftChild, sCV );
		DynamicLoad ( pd3dDevice, SFile, pTree->pRightChild, sCV );
	}

	void ThreadLoad ( OBJOCTree* pTree, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading )
	{
		if( !pTree )	return;

		if ( !COLLISION::IsCollisionVolume( sCV, pTree->vMax, pTree->vMin ) )	return;

		pTree->ThreadLoad( listLoadingData, CSLockLoading );

		ThreadLoad( pTree->pLeftChild, sCV, listLoadingData, CSLockLoading );
		ThreadLoad( pTree->pRightChild, sCV, listLoadingData, CSLockLoading );
	}

	void LoadOctreeVER_100 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, POBJOCTREE& pTree )
	{
		BOOL bUse = TRUE;
		SFile >> bUse;

		if ( bUse )
		{
			OBJOCTreeOrig* pNew = new OBJOCTreeOrig;
			pNew->Load_VER100 ( pd3dDevice, SFile );
			pTree = pNew;
		}
		else
		{
			return;
		}

		LoadOctreeVER_100 ( pd3dDevice, SFile, pTree->pLeftChild );
		LoadOctreeVER_100 ( pd3dDevice, SFile, pTree->pRightChild );
	}
};