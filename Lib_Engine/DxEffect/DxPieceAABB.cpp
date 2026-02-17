#include "stdafx.h"
#include "./DxPieceAABB.h"

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
// -----------------------------------------------------------------------------------------------------------------------------------------
//											D	x		P	i	e	c	e		A	A	B	B
// -----------------------------------------------------------------------------------------------------------------------------------------

const DWORD DxPieceAABB::VERSION_PIECE_AABB	= 0x0100;

DxPieceAABB::DxPieceAABB() :
	m_pPieceHead(NULL),
	m_pPieceAABBTree(NULL),
	m_dwPieceNUM(0L)
{
}

DxPieceAABB::~DxPieceAABB()
{
	m_dwPieceNUM = 0L;
	m_pPieceHead = NULL;
	SAFE_DELETE( m_pPieceAABBTree );
}

void DxPieceAABB::ClearTree()
{	
	m_pPieceAABBTree->CleanUp(); 
}

void DxPieceAABB::ClearOnlyTree()
{	
	SAFE_DELETE( m_pPieceAABBTree ); 
}

void DxPieceAABB::SetPiece( DxPiece* pPiece )
{
	// List로 연결
	pPiece->m_pNext = m_pPieceHead;
	m_pPieceHead = pPiece;
}

void DxPieceAABB::FrameMove( const float fTime, const float fElapsedTime )
{
}

void DxPieceAABB::Render( LPDIRECT3DDEVICEQ pd3dDevice, DxPiece* pPiecePick, const CLIPVOLUME &sCV, const float fTime, const float fElapsedTime, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxPieceAABB::Render");

	float fDistance = DxViewPort::GetInstance().GetDistance();
	fDistance = fDistance*fDistance;

	// Note : Quick Sort 를 한다.		이넘을 저기서도 쓰려면... 데이터를 공유하면 안될 듯 하네.
	m_sQuickSort.Reset();
	NSPIECETREE::InsertQuickSort( pd3dDevice, m_pPieceAABBTree, &m_sQuickSort, pPiecePick, sCV, 
		DxViewPort::GetInstance().GetFromPt(), DxViewPort::GetInstance().GetLookatPt(), fDistance );
	m_sQuickSort.QuickSortProcess();

	m_sQuickSort.FrameMove( fTime, fElapsedTime );
	m_sQuickSort.Render( pd3dDevice, sCV, material );

	PROFILE_END("DxPieceAABB::Render");
}

void DxPieceAABB::RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxPieceAABB::RenderAlpha");
	m_sQuickSort.RenderAlpha( pd3dDevice, sCV, material );
	PROFILE_END("DxPieceAABB::RenderAlpha");
}

void DxPieceAABB::RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
							DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxPieceAABB::RenderEff");
	m_sQuickSort.RenderEff( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
	PROFILE_END("DxPieceAABB::RenderEff");
}

void DxPieceAABB::RenderPickAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
								  DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material )
{
	PROFILE_BEGIN("DxPieceAABB::RenderPickAlpha");
	m_sQuickSort.RenderPickAlpha( pd3dDevice, sCV, pStaticMesh, pFrameMesh, pMultiStaticMesh, pMultiStaticMaterialMesh, material );
	PROFILE_END("DxPieceAABB::RenderPickAlpha");
}

void DxPieceAABB::Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material )
{
	NSPIECETREE::Render_Reflect( pd3dDevice, m_pPieceAABBTree, sCV, material );
}

void DxPieceAABB::MakeTree()
{
	// Note : 현재 조각들의 AABB Box를 구한다.
	DxPiece* pCur = m_pPieceHead;
	while( pCur )
	{
		pCur->SetAABBBox();
		pCur = pCur->m_pNext;
	}

	// Note : 현재 조각파일의 전체 갯수
	DWORD dwCount = 0;
	pCur = m_pPieceHead;
	while( pCur )
	{
		++dwCount;
		pCur = pCur->m_pNext;
	}

	m_dwPieceNUM = dwCount;

	// Note : 전체 조각들이 모두 사용 할수 있도록 최대 배열을 잡을수 있게 해 준다.
	m_sQuickSort.SetArrayMaxSize( dwCount );

	// Note : Tree 를 만든다.
	SAFE_DELETE( m_pPieceAABBTree );
	NSPIECETREE::MakeTree( m_pPieceAABBTree, m_pPieceHead );
}

void DxPieceAABB::CloneData( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceAABB* pSrc )
{
	SAFE_DELETE( m_pPieceAABBTree );
	SAFE_DELETE( m_pPieceHead );

	DxPiece* pCur = pSrc->m_pPieceHead;

	while( pCur )
	{
		DxPiece* pNew = new DxPiece;
		pNew->CloneData( pd3dDevice, pCur );

		pCur = pCur->m_pNext;

		// 이제 연결한다.
		pNew->m_pNext = m_pPieceHead;
		m_pPieceHead = pNew;
	}

	// Note : Tree를 만든다.
	MakeTree();
}

BOOL DxPieceAABB::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision )
{
	BOOL bCollision(FALSE);
	CLIPVOLUME cv = DxViewPort::GetInstance().GetClipVolume();
	NSPIECETREE::CollisionLine( m_pPieceAABBTree, cv, vStart, vEnd, vCollision, bCollision );

	return bCollision;
}

void DxPieceAABB::Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const char* pPieceName )
{
	SFile >> m_dwPieceNUM;

	// Note : 전체 조각들이 모두 사용 할수 있도록 최대 배열을 잡을수 있게 해 준다.
	m_sQuickSort.SetArrayMaxSize( m_dwPieceNUM );

	DxPieceEdit* pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, pPieceName );

	NSPIECETREE::Load_OLD( pd3dDevice, m_pPieceAABBTree, SFile, pPieceEdit );
}

void DxPieceAABB::Save( CSerialFile& SFile )
{
	SFile << (DWORD)VERSION_PIECE_AABB;

	SFile.BeginBlock(EMBLOCK_02);
	{
		SFile << m_dwPieceNUM;

		SFile << BOOL(FALSE);

		NSPIECETREE::Save( m_pPieceAABBTree, SFile );
	}
	SFile.EndBlock(EMBLOCK_02);
}

void DxPieceAABB::Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const char* pPieceName )
{
	DWORD dwVER;
	DWORD dwSIZE;

	SFile >> dwVER;
	SFile >> dwSIZE;

	if ( dwVER == VERSION_PIECE_AABB )
	{
		SFile >> m_dwPieceNUM;

		m_sQuickSort.SetArrayMaxSize( m_dwPieceNUM );
		DxPieceEdit* pPieceEdit = DxPieceContainer::GetInstance().LoadPiece( pd3dDevice, pPieceName );

		BOOL bEXIST(FALSE);
		SFile >> bEXIST;
		if ( bEXIST )
		{
			//jdevtodo
			//lightmaps for pie
			//bypass for now
			DWORD _dwBlockSize(0);
			SFile >> _dwBlockSize;
			DWORD dwCur = SFile.GetfTell();
			SFile.SetOffSet( dwCur+_dwBlockSize );
		}

		NSPIECETREE::Load_STABLE( pd3dDevice, m_pPieceAABBTree, SFile, pPieceEdit );
	}
	else
	{
		CDebugSet::ErrorVersion( "DxPieceAABB::Load_STABLE", SFile.GetFileName(), dwVER );
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+dwSIZE );
	}
}
