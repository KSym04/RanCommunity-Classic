#include "stdafx.h"
#include "./DxSingleTexMesh.h"

#include "./DxLightMan.h"
#include "../DxEffect/DxTexEffMan.h"
#include "./TextureManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD	DxSingleTexMesh::VERSION_SINGLE_TEX_MESH_DATA		= 0x0200;

void DxSingleTexMesh::Save( CSerialFile& SFile, BOOL bPiece )
{
	SFile << VERSION_SINGLE_TEX_MESH_DATA;

	SFile.BeginBlock(EMBLOCK_01);
	{
		SFile << m_szTexName;
		SFile << m_dwFlag;
		SFile << unknown_var1;
		SFile << unknown_var2;
		SFile << unknown_var3;
		SFile << unknown_var4;
		SFile << unknown_var5;

		DWORD dwSize=0;
		DxTexEffBase* pCur = m_pTexEffList;
		while( pCur )
		{
			++dwSize;
			pCur = pCur->m_pNext;
		}
		SFile << dwSize;

		pCur = m_pTexEffList;
		while( pCur )
		{
			SFile << pCur->GetTypeID();

			pCur->SavePSF( SFile );
			pCur = pCur->m_pNext;
		}
	}
	SFile.EndBlock(EMBLOCK_01);

	BOOL bExist;
	bExist = m_pAABBOctree ? TRUE : FALSE;
	SFile << bExist;
	if ( m_pAABBOctree )
	{
		SFile << (DWORD)m_pAABBOctree->GetTypeID();
		m_pAABBOctree->Save( SFile, bPiece );
	}
}

void DxSingleTexMesh::Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece )
{
	CleanUp();

	DWORD dwVer;
	DWORD dwBufferSize;

	SFile >> dwVer;
	SFile >> dwBufferSize;

	if( dwVer==VERSION_SINGLE_TEX_MESH_DATA )
	{
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;
		SFile >> unknown_var2;
		SFile >> unknown_var3;
		SFile >> unknown_var4;
		SFile >> unknown_var5;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0106 )
	{
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var3;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0105 )
	{
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;
		SFile >> unknown_var2;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0104 )
	{
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0103 )
	{
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0102 )
	{
		SFile >> m_szTexName;
		SFile >> m_dwFlag;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0101 )
	{
		SFile >> m_dwFlag;
		SFile >> m_szTexName;

		DWORD dwSize;
		SFile >> dwSize;

		DxTexEffBase*	pTexEffBase;
		DWORD			dwTypeID;
		std::string		strName;
		for( DWORD i=0; i<dwSize; ++i )
		{
			SFile >> dwTypeID;

			pTexEffBase = DxTexEffMan::GetInstance().AdaptToDxFrame( dwTypeID, pd3dDevice, NULL );
			pTexEffBase->LoadPSF( pd3dDevice, SFile );
			pTexEffBase->m_pNext = m_pTexEffList;
			m_pTexEffList = pTexEffBase;
		}
	}
	else if( dwVer==0x0100 )
	{
		SFile >> m_szTexName;
	}
	else
	{
		CDebugSet::ErrorVersion( "DxSingleTexMesh::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwBufferSize );
	}

	BOOL bExist;
	SFile >> bExist;
	if ( bExist )
	{
		if ( dwVer <= 0x0103 )
		{
			DxAABBOctreeOrig* pAABBOctree = new DxAABBOctreeOrig;
			pAABBOctree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
			m_pAABBOctree = pAABBOctree;
		}
		else
		{
			DWORD _dwType(0);
			SFile >> _dwType;

			if ( _dwType == EMAABBCTREE_TYPE_ORIG )
			{
				m_pAABBOctree = new DxAABBOctreeOrig;
			}
			else if ( _dwType == EMAABBCTREE_TYPE_LIGHTMAP )
			{
				m_pAABBOctree = new DxAABBOctreeLightMap;	
			}
			else
			{
				CDebugSet::ErrorCode( "DxSingleTexMesh::Load", "unrecognized DxAABBOctree type :%d", _dwType );
			}

			if ( m_pAABBOctree )
				m_pAABBOctree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
		}
	}

	//lightmap test
	if ( m_pAABBOctree && m_pAABBOctree->GetTypeID() == EMAABBCTREE_TYPE_LIGHTMAP )
	{
		DxAABBOctreeLightMap* paabbtree = dynamic_cast<DxAABBOctreeLightMap*>(m_pAABBOctree);
		if ( paabbtree )
		{
			m_szTexNameLightMapD = paabbtree->strLightMapD0;
			m_szTexNameLightMapN = paabbtree->strLightMapN0;

			LoadLightMaps( pd3dDevice );
		}
	}

	// Note : Texture Loading		
	//	if ( m_szTexName.size() )	TextureManager::LoadTexture ( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L );
}

void DxSingleTexMesh::Load_VER100 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	CleanUp();

	std::string szName;

	DxAABBOctreeOrig* pAABBOctree = new DxAABBOctreeOrig;
	pAABBOctree->Load_VER100 ( pd3dDevice, SFile, szName );
	m_pAABBOctree = pAABBOctree;

	m_szTexName = szName.c_str();
	//	if ( m_szTexName.size() )	TextureManager::LoadTexture ( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L );
}
