#pragma once

#include "../DxCommon/DxVertexFVF.h"

#include "../Common/SerialFile.h"
#include "../DxCommon/MaterialShaderParam.h"

class DxPieceEdit;
class DxStaticAniFrame;
class DxPieceTexEff;
class DxFrameMesh;
class DxStaticMesh;
class DxMultiStaticMesh;
class DxMultiStaticMaterialMesh;
class DxPieceEff;

//----------------------------------------------------------------------------------------------------------------------
//											D	x		P	i	e	c	e
//	Note : 
//----------------------------------------------------------------------------------------------------------------------
class DxPiece
{
private:
	static const DWORD VERSION_PIECE;

private:
	float		m_fTime;
	float		m_fAniTime;
	float		m_ElapsedTime;

private:
	DxStaticAniFrame*	m_pStaticAniFrame;	// RefCount로 1개만 생성후 Pointer 사용으로 중복생성을 안함. Container에서 관리
	DxPieceEff*			m_pPieceEff;		// RefCount 없음. 여러개 생성. 확실히 지워줘야 함.
	DxPieceTexEff*		m_pAniTexEff;		// Animation용 Texture Effect. 여러개 생성. 확실히 지워줘야 함.

public:
	D3DXVECTOR3	m_vStaticMax;	// StaticMesh 만의 한정. < 캐릭터를 가릴경우를 체크하기 위함 >
	D3DXVECTOR3	m_vStaticMin;	// StaticMesh 만의 한정. < 캐릭터를 가릴경우를 체크하기 위함 >
	D3DXVECTOR3 m_vCenter;		// 물체의 중점. 						< 1차 체크를 위함 >
	float		m_fLength;		// 중점에서 원을 그렸을 경우의 반지름.	< 1차 체크를 위함 >
	float		m_fLengthOrig;

public:
	float		m_fAlpha;		// 픽킹이 되었을 때 Alpha 가 변화한다.		// 매번 변하는 값.
	BOOL		m_bAplhaBlend;	// Picking에 의한 AlphaBlending을 하는가?	// 매번 변하는 값.

private:
	VEC_DWORD	m_vecPointIndex;	// 자신에게 영향을 주는 Point Light Index

public:
	D3DXVECTOR3	m_vMax;			// 전체의 Max
	D3DXVECTOR3	m_vMin;			// 전체의 Min
	D3DXMATRIX	m_matWorld;

	DxPiece*	m_pNext;		// List
	DxPiece*	m_pTreeNext;	// Tree 안에서의 List 이다.
	DxPiece*	m_pSortNext;	// Quick Sort 할 때의 List 이다.

	std::string m_strPiece;

	DWORD		m_dwLightMapOffset1;
	DWORD		m_dwLightMapOffset2;
	std::string	m_strLightMapDay;
	std::string	m_strLightMapDS1;
	std::string	unknown_var5;
	std::string	unknown_var6;

	float		m_fLightMapUVX;
	float		m_fLightMapUVY;
	std::string m_strLightMapDS2;
	std::string m_strLightMapShadow;
	float		m_fDrawDistance1;
	float		m_fDrawDistance2;
	DWORD		unknown_var13;
	DWORD		unknown_var14;
	DWORD		unknown_var15;

	float			m_fLightMapOffsetX;
	float			m_fLightMapOffsetY;
	DWORD			m_dwIndex;
	BOOL			m_bLightMapOffSet;

public:
	D3DXVECTOR3		m_vMaxCol;
	D3DXVECTOR3		m_vMinCol;
	D3DXVECTOR3		m_vPos;

public:
	LPDIRECT3DTEXTUREQ	m_pTextureLightMapDay;

		
public:
	void FrameMove( const float fTime, const float fElapsedTime );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderAlpha( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	void RenderEff( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxStaticMesh* pStaticMesh, DxFrameMesh* pFrameMesh, 
		DxMultiStaticMesh* pMultiStaticMesh, DxMultiStaticMaterialMesh* pMultiStaticMaterialMesh, SMATERIAL_SETTING& material );

	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );

	// Edit
	void SetAABBBox();
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision );

public:
	void Save( CSerialFile& SFile );	// Tree 에서의 Save Load
	void Load_OLD( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxPieceEdit* pSrc );
	void Load_STABLE( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, DxPieceEdit* pSrc );

public:
	void Import( LPDIRECT3DDEVICEQ pd3dDevice, DxPieceEdit* pSrc, const D3DXMATRIX& matWorld, const D3DXMATRIX& matFrame );	// DxPieceEdit에서 값을 가져올 때 호출
	void CloneData( LPDIRECT3DDEVICEQ pd3dDevice, DxPiece* pSrc );															// DxPiece 녀석들 끼리의 복제
	void CleanUp();

private:
	void CheckPointLight();									// 자신과 맞닿아 있는 빛을 체크를 한다.	< 기본적으로 빛은 로딩되어 있어야 함 >
	void RenderPointLight( LPDIRECT3DDEVICEQ pd3dDevice );	// Point Light를 적용시킨다.

	void LoadLightMaps( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	BOOL IsRender( const CLIPVOLUME &sCV, const D3DXVECTOR3& vFromPt, const D3DXVECTOR3& vLookatPt );
	void UpdateLightMapUV();

public:
	DxPiece();
	~DxPiece();
};




