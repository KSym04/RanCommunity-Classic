#include "stdafx.h"
#include "./DxPieceAnimation.h"
#include "../Common/SerialFile.h"
#include "./DxAnimationManager.h"
#include "../DxEffect/DxPieceMultiStaticMaterialMesh.h"
#include "../DxEffect/DxStaticMaterialMesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void	DxPieceAnimation::SaveFile ( CSerialFile &SFile )
{
	SFile << (DWORD)DXPIECE_ANIMATION_VERSION;

	SFile.BeginBlock(EMBLOCK_04);

	Save(SFile);
	
	SFile.EndBlock(EMBLOCK_04);
}

void DxPieceAnimation::LoadFile ( CSerialFile &SFile, LPDIRECT3DDEVICEQ pd3dDevice, DxPieceMultiStaticMaterialMesh* pMesh )
{
	DWORD dwVer;
	DWORD dwSize;

	SFile >> dwVer;
	SFile >> dwSize;

	if ( dwVer == DXPIECE_ANIMATION_VERSION )
	{
		Load_0100( SFile, pd3dDevice, pMesh );
	}
	else
	{
		CDebugSet::ErrorVersion( "DxPieceAnimation::LoadFile", SFile.GetFileName(), dwVer );
		SFile.SetOffSet ( SFile.GetfTell() + dwSize );
	}
}

void DxPieceAnimation::Save( CSerialFile &SFile )
{
	SFile << m_vMax;
	SFile << m_vMin;
	SFile.WriteBuffer ( &m_matWorld, sizeof(D3DXMATRIX) );

	if ( m_pDxAnimation )
	{
		SFile << BOOL(TRUE);
		m_pDxAnimation->SaveFile( SFile );
	}else{
		SFile << BOOL(FALSE);
	}

	SFile << m_dwVar;

	if ( m_pLeft )
	{
		SFile << BOOL(TRUE);
		m_pLeft->Save( SFile );
	}else{
		SFile << BOOL(FALSE);
	}

	if ( m_pRight )
	{
		SFile << BOOL(TRUE);
		m_pRight->Save( SFile );
	}else{
		SFile << BOOL(FALSE);
	}
}

void DxPieceAnimation::Load_0100 ( CSerialFile &SFile, LPDIRECT3DDEVICEQ pd3dDevice, DxPieceMultiStaticMaterialMesh* pMesh )
{
	SFile >> m_vMax;
	SFile >> m_vMin;
	SFile.ReadBuffer ( &m_matWorld, sizeof(D3DXMATRIX) );

	BOOL	bExist = FALSE;

	SFile >> bExist;
	if ( bExist )
	{
		SAFE_DELETE( m_pDxAnimation );
		m_pDxAnimation = new DxAnimation;
		m_pDxAnimation->LoadFile ( SFile, NULL );
	}

	SFile >> m_dwVar;

	if ( pMesh )
	{
		if( m_dwVar < (DWORD)pMesh->m_vecStaticMesh.size() )
		{
			pMesh->m_vecStaticMesh[m_dwVar]->m_pmatPieceFrame = &m_matResult;
		}
	}

	SFile >> bExist;
	if ( bExist )
	{
		m_pLeft = new DxPieceAnimation;
		m_pLeft->Load_0100 ( SFile, pd3dDevice, pMesh );
	}

	SFile >> bExist;
	if ( bExist )
	{
		m_pRight = new DxPieceAnimation;
		m_pRight->Load_0100 ( SFile, pd3dDevice, pMesh );
	}
}