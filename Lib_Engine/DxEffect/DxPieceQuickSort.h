#pragma once

#include "../Common/SerialFile.h"
#include "../DxCommon/DxCustomTypes.h"
#include "../DxCommon/MaterialShaderParam.h"

class DxPiece;
class DxFrameMesh;
class DxStaticMesh;
class DxMultiStaticMesh;
class DxMultiStaticMaterialMesh;

//----------------------------------------------------------------------------------------------------------------------
//								D	x		P	i	e	c	e		Q	u	i	c	k		S	o	r	t
//	Note : 괜히 소트한다음 뿌리면 느릴까봐 걱정. ^^;
//			한곳에 집어넣고 배열로 정렬을 하는 것 인가 ?
//----------------------------------------------------------------------------------------------------------------------
class DxPieceQuickSort
{
protected:
	struct PIECEDISTANCE
	{
		float		m_fDistance;
		DxPiece*	m_pPiece;
	};
	PIECEDISTANCE*	m_pArrayPiece;	// 배열
	DWORD			m_dwArraySize;	// 최대 가능한 배열 사이즈
	DWORD			m_dwPieceCount;	// 현재 배열 사이즈
	float			m_fElapsedTime;	// ElapsedTime,

protected:
	static PIECEDISTANCE*	m_pArrayPieceONLY;	// 배열
	static DWORD			m_dwArraySizeONLY;	// 최대 가능한 배열 사이즈
	static DWORD			m_dwPieceCountONLY;	// 현재 배열 사이즈

public:
	void SetArrayMaxSize( DWORD dwCount );																// 최대 배열 사이즈 셋팅
	void Reset();																						// 현재 배열 사이즈 초기화.
	void InsertData( DxPiece* pPiece, DxPiece* pPiecePick, 
		const D3DXVECTOR3& vFromPt, const D3DXVECTOR3& vLookatPt, const float& fLength );	// 무작위 Piece 데이타
	void QuickSortProcess();																			// Quick Sort 작업
	void FrameMove( const float fTime, const float fElapsedTime );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	static void InitPickAlpha();
	static void FrameMoveONLY( const float fTime, const float fElapsedTime );
	static void RenderPickAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

protected:
	void DxQuickSort( PIECEDISTANCE pArray[], int m, int n );
	void Swap( PIECEDISTANCE& pData1, PIECEDISTANCE& pData2 );

public:
	DxPieceQuickSort();
	~DxPieceQuickSort();
};