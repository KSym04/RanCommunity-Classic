#include "stdafx.h"
#include "DxSingleTexMaterialMesh.h"

#include "./DxLightMan.h"
#include "../DxEffect/DxTexEffMan.h"
#include "./TextureManager.h"
#include "./DxMaterialManager.h"
#include "./DxShaderManager.h"
#include "./MaterialShaderParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD	DxSingleTexMaterialMesh::VERSION_SINGLE_TEX_MATERIAL_MESH_DATA		= 0x0200;

void DxSingleTexMaterialMesh::Save( CSerialFile& SFile, BOOL bPiece )
{
	SFile << VERSION_SINGLE_TEX_MATERIAL_MESH_DATA;

	SFile.BeginBlock(EMBLOCK_01);
	{
		SFile << m_szMaterial;
		SFile << m_szTexName;
		SFile << m_dwFlag;
		SFile << unknown_var1;
		SFile << m_fCubeMapValue;
		SFile << m_fSpecularPower;
		SFile << m_fSpecularIntensity;
		SFile << unknown_var5;
		SFile << unknown_var6;
		SFile << m_bUseLightMap;
	}
	SFile.EndBlock(EMBLOCK_01);

	BOOL bExist;
	bExist = m_pAABBOctree ? TRUE : FALSE;
	SFile << bExist;
	if ( m_pAABBOctree )	m_pAABBOctree->Save( SFile, bPiece );
}

void DxSingleTexMaterialMesh::Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece )
{
	CleanUp();

	DWORD dwVer;
	DWORD dwBufferSize;

	SFile >> dwVer;
	SFile >> dwBufferSize;

	if( dwVer==VERSION_SINGLE_TEX_MATERIAL_MESH_DATA )
	{
		SFile >> m_szMaterial;
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;
		SFile >> m_fCubeMapValue;
		SFile >> m_fSpecularPower;
		SFile >> m_fSpecularIntensity;
		SFile >> unknown_var5;
		SFile >> unknown_var6;
		SFile >> m_bUseLightMap;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			m_pAABBOctree = new DxAABBOctreeLightMap;
			m_pAABBOctree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
		}
	}
	else if( dwVer==0x0107 || dwVer==0x0108 )
	{
		SFile >> m_szMaterial;
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;
		SFile >> m_fCubeMapValue;
		SFile >> m_fSpecularPower;
		SFile >> m_fSpecularIntensity;
		SFile >> unknown_var5;
		SFile >> unknown_var6;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			m_pAABBOctree = new DxAABBOctreeLightMap;
			m_pAABBOctree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
		}
	}
	else if( dwVer==0x0105 )
	{
		SFile >> m_szMaterial;
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;
		SFile >> m_fCubeMapValue;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			m_pAABBOctree = new DxAABBOctreeLightMap;
			m_pAABBOctree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
		}
	}
	else if( dwVer==0x0104 )
	{
		SFile >> m_szMaterial;
		SFile >> m_szTexName;
		SFile >> m_dwFlag;
		SFile >> unknown_var1;

		BOOL bExist;
		SFile >> bExist;
		if ( bExist )
		{
			m_pAABBOctree = new DxAABBOctreeLightMap;
			m_pAABBOctree->Load( pd3dDevice, SFile, bDynamicLoad, bPiece );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxSingleTexMaterialMesh::Load", SFile.GetFileName(), dwVer );
		DWORD dwCur = SFile.GetfTell();
		SFile.SetOffSet( dwCur+dwBufferSize );
	}

	if ( m_szTexName.size() )
		TextureManager::LoadTexture( m_szTexName.c_str(), pd3dDevice, m_pTexture, 0L, 0L, FALSE );

	if ( m_szMaterial.size() )
	{
		m_pMaterial = NULL;
		m_pMaterial = DxMaterialManager::GetInstance().GetMaterial( m_szMaterial.c_str() );

		if ( m_pMaterial )
		{
			m_pMaterial->LoadTexture( pd3dDevice );
			m_pMaterial->LoadEffect( pd3dDevice );
		}
		
	}

	//CDebugSet::ToFile( "check.txt", "%s %s %d", m_szMaterial.c_str(), m_szTexName.c_str(), m_dwFlag );
}
