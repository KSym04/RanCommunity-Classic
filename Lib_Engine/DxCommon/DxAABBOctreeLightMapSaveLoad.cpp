#include "stdafx.h"
#include "./DxAABBOctreeLightMap.h"

#include "./NsOCTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const DWORD	DxAABBOctreeLightMap::VERSION = 0x0200;

void DxAABBOctreeLightMap::Save ( CSerialFile& SFile, BOOL bPiece )
{
	SFile << VERSION;

	SFile.BeginBlock(EMBLOCK_01);
	{
		SFile << m_vMax;
		SFile << m_vMin;

		SFile << unknown_var1;
		SFile << strLightMapD0;
		SFile << strLightMapN0;
		SFile << strLightMapD1;
		SFile << strLightMapN1;

		if ( m_pOcTree && m_pOcTree->GetTypeID() == EMOBJOCTREE_TYPE_LIGHTMAP )
		{
			SFile << BOOL(TRUE);
			NSOCTREE::SaveOctree ( SFile, m_pOcTree, bPiece );
		}
		else
			SFile << BOOL(FALSE);

		if ( m_pOcTree && m_pOcTree->GetTypeID() == EMOBJOCTREE_TYPE_MATERIAL )
		{
			SFile << BOOL(TRUE);
			NSOCTREE::SaveOctree ( SFile, m_pOcTree, bPiece );
		}
		else
			SFile << BOOL(FALSE);
	}
	SFile.EndBlock(EMBLOCK_01);
}

void DxAABBOctreeLightMap::Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece )
{
	CleanUpBase();
	CleanUpDerived();

	DWORD dwVer;
	DWORD dwBufferSize;

	SFile >> dwVer;
	SFile >> dwBufferSize;

	if( dwVer==VERSION )
	{
		SFile >> m_vMax;
		SFile >> m_vMin;

		SFile >> unknown_var1;
		SFile >> strLightMapD0;
		SFile >> strLightMapN0;
		SFile >> strLightMapD1;
		SFile >> strLightMapN1;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			NSOCTREE::LoadOctree_TYPE_1( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_LIGHTMAP );
		}

		SFile >> bExist;
		if ( bExist )
		{
			NSOCTREE::LoadOctree_TYPE_2( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_MATERIAL );
		}
	}
	else if( dwVer==0x0104 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			SFile >> unknown_var1;

			SFile >> m_vMax;
			SFile >> m_vMin;

			SFile >> strLightMapD0;
			SFile >> strLightMapN0;

			SFile >> bExist;
			if ( bExist )
			{
				NSOCTREE::LoadOctree_TYPE_1( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_LIGHTMAP );
			}

			SFile >> bExist;
			if ( bExist )
			{
				NSOCTREE::LoadOctree_TYPE_2( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_MATERIAL );
			}
		}
	}
	else if( dwVer==0x0103 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			SFile >> unknown_var1;

			SFile >> m_vMax;
			SFile >> m_vMin;

			SFile >> strLightMapD0;
			SFile >> strLightMapN0;
			SFile >> strLightMapD1;
			SFile >> strLightMapN1;

			SFile >> bExist;
			if ( bExist )
			{
				NSOCTREE::LoadOctree_TYPE_1( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_LIGHTMAP );
			}

			SFile >> bExist;
			if ( bExist )
			{
				NSOCTREE::LoadOctree_TYPE_2( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_MATERIAL );
			}
		}
	}
	else if( dwVer==0x0101 )
	{
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			SFile >> unknown_var1;

			SFile >> m_vMax;
			SFile >> m_vMin;

			SFile >> strLightMapD0;
			SFile >> strLightMapN0;
			SFile >> strLightMapD1;
			SFile >> strLightMapN1;

			NSOCTREE::LoadOctree_TYPE_1( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_LIGHTMAP );
			NSOCTREE::MakeCollisionData( pd3dDevice, m_pOcTree );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxAABBOctreeLightMap::Load_STABLE", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwBufferSize );
	}
}
