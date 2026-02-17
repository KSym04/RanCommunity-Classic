#include "stdafx.h"
#include "./DxMultiPieceAABB.h"

#include "../DxCommon/DxViewPort.h"

#include "./DxPiece.h"
#include "./DxPieceNode.h"
#include "./DxPieceQuickSort.h"
#include "./DxPieceContainer.h"

#include "./DxStaticMesh.h"
#include "./DxMultiStaticMesh.h"
#include "./DxMultiStaticMaterialMesh.h"

#include "./PIECETREE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD DxMultiPieceAABB::VERSION_MULTI_PIECE_AABB	= 0x0200;

DxMultiPieceAABB::DxMultiPieceAABB() :
	m_pPieceHead(NULL),
	m_dwPieceNUM(0L)
{
	m_vecPiece.clear();
}

DxMultiPieceAABB::~DxMultiPieceAABB()
{
	m_dwPieceNUM = 0L;

	DxPiece* pDel;
	DxPiece* pCur = m_pPieceHead;
	while( pCur )
	{
		pDel = pCur;
		pCur = pCur->m_pTreeNext;

		SAFE_DELETE( pDel );
	}

	m_pPieceHead = NULL;

	m_vecPiece.clear();
}

void DxMultiPieceAABB::SetPiece( DxPiece* pPiece )
{
	pPiece->SetAABBBox();
	pPiece->m_pTreeNext = m_pPieceHead;
	m_pPieceHead = pPiece;
}

void DxMultiPieceAABB::FrameMove( const float fTime, const float fElapsedTime )
{
	const CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume();
	const D3DXVECTOR3 vFrom  = DxViewPort::GetInstance().GetFromPt();
	const D3DXVECTOR3 vTo    = DxViewPort::GetInstance().GetLookatPt();

	m_vecPiece.clear();

	DxPiece* pCur = m_pPieceHead;
	while( pCur )
	{
		if ( pCur->IsRender( CV, vFrom, vTo ) )
			m_vecPiece.push_back( pCur );

		pCur = pCur->m_pTreeNext;
	}

	for( unsigned int i=0; i<m_vecPiece.size(); ++i )
	{
		m_vecPiece[i]->FrameMove( fTime, fElapsedTime );
	}
}

void DxMultiPieceAABB::Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxMultiPieceAABB::Render");

	for( unsigned int i=0; i<m_vecPiece.size(); ++i )
	{
		m_vecPiece[i]->Render( pd3dDevice, sCV, material );
	}

	PROFILE_END("DxMultiPieceAABB::Render");
}

void DxMultiPieceAABB::RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxMultiPieceAABB::RenderAlpha");

	for( unsigned int i=0; i<m_vecPiece.size(); ++i )
	{
		m_vecPiece[i]->RenderAlpha( pd3dDevice, sCV, material );
	}

	PROFILE_END("DxMultiPieceAABB::RenderAlpha");
}

void DxMultiPieceAABB::RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
								 DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxMultiPieceAABB::RenderEff");
	
	for( unsigned int i=0; i<m_vecPiece.size(); ++i )
	{
		m_vecPiece[i]->RenderEff( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
	}

	PROFILE_END("DxMultiPieceAABB::RenderEff");
}

void DxMultiPieceAABB::Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxMultiPieceAABB::Render_Reflect");

	for( unsigned int i=0; i<m_vecPiece.size(); ++i )
	{
		m_vecPiece[i]->Render_Reflect( pd3dDevice, sCV, material );
	}

	PROFILE_END("DxMultiPieceAABB::Render_Reflect");
}

BOOL DxMultiPieceAABB::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision )
{
	CLIPVOLUME cv = DxViewPort::GetInstance().GetClipVolume();

	BOOL bCollision(FALSE);
	DxPiece* pCur = m_pPieceHead;
	while( pCur )
	{
		if ( COLLISION::IsCollisionVolume( cv, pCur->m_vMax, pCur->m_vMin ) )
		{
			if( pCur->IsCollisionLine( vStart, vEnd, vCollision ) )
			{
				bCollision = TRUE;
				break;
			}
		}

		pCur = pCur->m_pTreeNext;
	}

	return bCollision;
}

void DxMultiPieceAABB::Save( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_MULTI_PIECE_AABB;

	SFile.BeginBlock(EMBLOCK_02);
	{
		SFile << m_dwPieceNUM;

		DxPiece* pCur = m_pPieceHead;
		while( pCur )
		{
			pCur->Save( SFile );
			pCur = pCur->m_pTreeNext;
		}
		
	}
	SFile.EndBlock(EMBLOCK_02);
}

void DxMultiPieceAABB::Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile )
{
	DWORD dwVER;
	DWORD dwSIZE;

	SFile >> dwVER;
	SFile >> dwSIZE;

	if ( dwVER == VERSION_MULTI_PIECE_AABB )
	{
		SFile >> m_dwPieceNUM;
		m_vecPiece.reserve( m_dwPieceNUM );

		for ( DWORD i=0; i<m_dwPieceNUM; ++i )
		{
			DxPiece* ppiece = new DxPiece;
			ppiece->Load_STABLE( pd3dDevice, SFile, NULL );
			SetPiece( ppiece );
		}

		DxPiece* pCur = m_pPieceHead;
		while( pCur )
		{
			if ( pCur->m_bLightMapOffSet )
				pCur->UpdateLightMapUV();

			pCur = pCur->m_pTreeNext;
		}
	}
	else if ( dwVER == 0x0102 )
	{
		SFile >> m_dwPieceNUM;
		m_vecPiece.reserve( m_dwPieceNUM );

		for ( DWORD i=0; i<m_dwPieceNUM; ++i )
		{
			DxPiece* ppiece  = new DxPiece;
			ppiece->Load_STABLE( pd3dDevice, SFile, NULL );
			ppiece->m_dwIndex = i;
			SetPiece( ppiece );
		}

		DWORD dwlinknum(0);
		SFile >> dwlinknum;
		for ( DWORD i=0; i<dwlinknum; ++i )
		{
			DWORD dwval;
			SFile >> dwval;

			DxPiece* pCur = m_pPieceHead;
			while( pCur )
			{
				if ( pCur->m_dwIndex == dwval )
					pCur->UpdateLightMapUV();

				pCur = pCur->m_pTreeNext;
			}
		}
	}
	else if ( dwVER == 0x0101 )
	{
		SFile >> m_dwPieceNUM;
		m_vecPiece.reserve( m_dwPieceNUM );

		for ( DWORD i=0; i<m_dwPieceNUM; ++i )
		{
			DxPiece* ppiece  = new DxPiece;
			ppiece->Load_STABLE( pd3dDevice, SFile, NULL );
			ppiece->m_dwIndex = i;
			SetPiece( ppiece );
		}

		DWORD dwlinknum(0);
		SFile >> dwlinknum;
		for ( DWORD i=0; i<dwlinknum; ++i )
		{
			DWORD dwval;
			SFile >> dwval;

			DxPiece* pCur = m_pPieceHead;
			while( pCur )
			{
				if ( pCur->m_dwIndex == dwval )
					pCur->UpdateLightMapUV();

				pCur = pCur->m_pTreeNext;
			}
		}

		//not used
		BOOL bExist(FALSE);
		SFile >> bExist;
		if ( bExist )
		{
			DWORD _dwVER2(0);
			DWORD _dwSIZE2(0);

			SFile >> _dwVER2;
			SFile >> _dwSIZE2;

			DWORD dwCur = SFile.GetfTell ();
			SFile.SetOffSet ( dwCur+_dwSIZE2 );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxMultiPieceAABB::Load", SFile.GetFileName(), dwVER );
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+dwSIZE );
	}
}
