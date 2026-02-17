#include "stdafx.h"
#include "./DxAABBOctreeOrig.h"

#include "./NsOCTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const DWORD	DxAABBOctreeOrig::VERSION = 0x0200;

void DxAABBOctreeOrig::Save ( CSerialFile& SFile, BOOL bPiece )
{
	SFile << VERSION;

	SFile.BeginBlock(EMBLOCK_01);
	{
		SFile << m_vMax;
		SFile << m_vMin;

		BOOL bExist;
		bExist = m_pOcTree ? TRUE : FALSE;
		SFile << bExist;
		if ( m_pOcTree )	NSOCTREE::SaveOctree ( SFile, m_pOcTree, bPiece );
	}
	SFile.EndBlock(EMBLOCK_01);
}

void DxAABBOctreeOrig::Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece )
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

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			NSOCTREE::LoadOctree_TYPE_0( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_ORIG );
		}
	}
	else if( dwVer==0x0101 )
	{
		SFile >> m_vMax;
		SFile >> m_vMin;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			NSOCTREE::LoadOctree_TYPE_0( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece, EMOBJOCTREE_TYPE_ORIG );
		}
	}
	else if( dwVer==0x0100 )
	{
		SFile >> m_vMax;
		SFile >> m_vMin;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			NSOCTREE::LoadOctree_OLD( pd3dDevice, SFile, m_pOcTree, bDynamicLoad, bPiece );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxAABBOctreeOrig::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwBufferSize );
	}
}

void DxAABBOctreeOrig::Load_VER100 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, std::string& szName )
{
	CleanUpBase();
	CleanUpDerived();

	SFile >> m_vMax;
	SFile >> m_vMin;
	SFile >> szName;

	BOOL bUse = TRUE;
	SFile >> bUse;
	if ( bUse )
	{
		NSOCTREE::LoadOctreeVER_100( pd3dDevice, SFile, m_pOcTree );
	}
}