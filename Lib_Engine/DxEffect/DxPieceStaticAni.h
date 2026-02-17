#pragma once

#include "./DxStaticMesh.h"
#include "./DxStaticMaterialMesh.h"
#include "../DxOctree/CollisionMap.h"
#include "./DxEffectFrame.h"
#include "../DxCommon/MaterialShaderParam.h"

class DxFrameAniMan;
class DxFrameMesh;
class DxPieceAnimation;
class DxPieceMultiStaticMaterialMesh;


//----------------------------------------------------------------------------------------------------------------------
//								D	X		R	E	P	L	A	C	E	P	I	E	C	E		E	X
//----------------------------------------------------------------------------------------------------------------------
class DxStaticAniFrame
{
protected:
	static DWORD		VERSION_STATIC_ANI;

protected:
	BOOL			m_bFrameMoveLock;

protected:
	DWORD			m_dwEffectCount;
	PDXEFFECTBASE	m_pEffectHead;

protected:
	DxStaticMesh*			m_pStaticMesh;	// 저장
	DxStaticMaterialMesh*	m_pStaticMaterialMesh;
	DxFrameAniMan*			m_pFrameAniMan;	// 저장
	DxPieceAnimation*		m_pPieceAnimation;
	DxPieceMultiStaticMaterialMesh*	m_pMultiStaticMaterialMesh;

protected:
	int					m_nRefCount;
	DxStaticAniFrame*	m_pNext;

public:
	DxFrame*		GetAniFrame( const char* szName );
	DxAnimationMan*	GetAniManHead();

public:
	void SetTime( const float fAniTime );
	void MakeAniMan( LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* pFrameMesh );

protected:
	HRESULT EffectAdaptToList( DxFrame *pFrameRoot, DWORD TypeID, char* szFrame, LPDIRECT3DDEVICEQ pd3dDevice,
									PBYTE pProperty, DWORD dwSize, DWORD dwVer, LPDXAFFINEPARTS pAffineParts, DxEffectBase* pEffectSrc );
	HRESULT EffectLoadToList( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile &SFile, PDXEFFECTBASE &pEffList );

	// Edit
public:
	void GetAABBBox( D3DXVECTOR3& vMax, D3DXVECTOR3&  vMin );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3& vCollision );
	void UpdateLightMapUV( float fMultiX, float fMultiY );

public:
	void OnResetDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	void OnLostDevice();

public:
	void FrameMove( const float fTime, const float fElapsedTime );
	void Render( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, const float fAniTime, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material );
	void Render_Alpha( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material );
	void Render_Reflect( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, const float fAniTime, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material );
	void Render_Reflect_Alpha( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, DxTextureEffMan::MAPTEXEFF* pmapTexEff, SMATERIAL_SETTING& material );
	void Render_EDIT( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld, SMATERIAL_SETTING& material );

public:
	void Save( CSerialFile& SFile );	// Note : Save, Load 는 DxPieceEdit 에서만 쓰인다. 
	void Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Export( LPDIRECT3DDEVICEQ pd3dDevice, DxFrameMesh* pFrameMesh );

protected:
	void CleanUp();

public:
	DxStaticAniFrame();
	~DxStaticAniFrame();
};