#pragma once

#include <vector>
#include <map>
#include <string>

#include "../Common/SerialFile.h"
#include "../DxCommon/DxCustomTypes.h"
#include "./DxPieceQuickSort.h"
#include "../DxCommon/MaterialShaderParam.h"

struct DxFrame;
class DxPiece;
class DxFrameMesh;
class DxStaticMesh;
class DxMultiStaticMesh;
class DxMultiStaticMaterialMesh;
class DxPieceAABB;
class DxMultiPieceAABB;

// -----------------------------------------------------------------------------------------------------------------------------------------
//										D	x		P	i	e	c	e		M	a	n	a	g	e	r
// -----------------------------------------------------------------------------------------------------------------------------------------
class DxPieceManager
{
private:
	static const DWORD VERSION_PSF;
	static const DWORD VERSION_PIECE_MANAGER;

protected:
	float m_fTime;
	float m_fElapsedTime;

protected:
	struct MATRIXPISNAME
	{
		D3DXMATRIX	m_matWorld;		// 자신의 Matrix
		char*		m_pPisName;
		DxPiece*	m_pPiece;

		void Save( CSerialFile& SFile, const D3DXMATRIX& matLocal, const D3DXMATRIX& matFrame );
		void Load( CSerialFile& SFile );

		void CleanUp();

		MATRIXPISNAME();
		~MATRIXPISNAME();
	};

protected:
	// FrameName, Piece
	typedef std::map<std::string,MATRIXPISNAME*>	MAPCHANGEPIS;
	typedef MAPCHANGEPIS::iterator					MAPCHANGEPIS_ITER;
	MAPCHANGEPIS	m_mapChangePis;

	// Point, Piece
	typedef std::vector<MATRIXPISNAME*>		VECPOINTPIS;
	typedef VECPOINTPIS::iterator			VECPOINTPIS_ITER;
	VECPOINTPIS	m_vecPointPis;

	// PieceName, PieceAABB
	typedef std::map<std::string,DxPieceAABB*>	MAPNAMEPIECE;
	typedef MAPNAMEPIECE::iterator				MAPNAMEPIECE_ITER;
	MAPNAMEPIECE	m_mapNamePiece;

protected:
	DxPiece*			m_pPiecePick;		// Pick Piece
	DxPieceQuickSort	m_sPickSort;		// Pick Quick Sort
	DxMultiPieceAABB*	m_pMultiPieceAABB;

	BOOL	unknown_var1;
	float	unknown_var2;

public:
	DWORD GetSize()		{ return (DWORD)(m_mapChangePis.size()+m_vecPointPis.size()); }
	BOOL IsGetName( DWORD nNUM, LPCSTR& pPieceName, LPCSTR& pFrameName );
	void SetPiece( LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* pFrameMesh, const char* szFrameName, const char* szPisName );
	void DeletePiece( DxFrameMesh* pFrameMesh, const char* szFrameName );
	void SetPiece( LPDIRECT3DDEVICEQ pd3dDevice, const char* szPisName, const D3DXMATRIX& matWorld );
	void DeletePiece( DWORD dwNumber );
	D3DXMATRIX* MoveCamera( DxFrame* pFrameRoot, const char* szFrameName );
	D3DXMATRIX* MoveCamera( DWORD dwNumber );

	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision );

protected:
	void MakeTree();		// Note : Render를 하기위해 구조 변환 및 Tree 생성

public:
	void FrameMove( const float fTime, const float fElapsedTime );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	void RenderPickAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material );

public:
	void SavePSF( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxFrameMesh* pFrameMesh );
	void LoadPSF( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxFrameMesh* pFrameMesh );
	void LoadPSF_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxFrameMesh* pFrameMesh );

	void SaveWLD( CSerialFile& SFile );
	void LoadWLD_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void LoadWLD_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

	void ClonePSFtoWLD( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceManager* pSrc );	// Game에 맞춘 복제
	void CleanUp();

public:
	DxPieceManager();
	~DxPieceManager();
};