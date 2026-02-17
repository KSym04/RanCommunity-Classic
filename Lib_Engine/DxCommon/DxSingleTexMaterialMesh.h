/*!
 * \file DxSingleTexMaterialMesh.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma once

#include <map>
#include "../Common/SerialFile.h"
#include "./Collision.h"
#include "../DxEffect/DxTextureEffMan.h"
#include "./DxAABBOctree.h"
#include "./DxAABBOctreeLightMap.h"
#include "./DxShadowMeshData.h"

class DxTexEffBase;
struct SMATERIAL_STATIC;

class DxSingleTexMaterialMesh : public OBJAABB
{
protected:
	static const DWORD	VERSION_SINGLE_TEX_MATERIAL_MESH_DATA;

protected:
	DWORD				m_dwFlag;
	DxAABBOctree*		m_pAABBOctree;
	std::string			m_szMaterial;
	std::string			m_szTexName;
	LPDIRECT3DTEXTUREQ	m_pTexture;

public:
	BOOL				unknown_var1;
	float				m_fCubeMapValue;
	float				m_fSpecularPower;
	float				m_fSpecularIntensity;
	DWORD				unknown_var5;
	DWORD				unknown_var6;

	SMATERIAL_STATIC*	m_pMaterial;
	ID3DXEffect*		m_pEffect;

	BOOL				m_bUseLightMap;

public:
	DxSingleTexMaterialMesh*	m_pNext;		// Tree 만들기 위한 임시 포인터

public:
	virtual void GetAABBSize ( D3DXVECTOR3& vMax, D3DXVECTOR3& vMin )
	{ 
		if( m_pAABBOctree )
		{
			m_pAABBOctree->GetAABBSize( vMax, vMin );
		}
		else
		{
			vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
			vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );
		}
	}
public:
	virtual BOOL IsDetectDivision ( D3DXVECTOR3 &vDivMax, D3DXVECTOR3 &vDivMin )
	{
		D3DXVECTOR3 vMax, vMin;
		GetAABBSize( vMax, vMin );
		return COLLISION::IsWithInPoint ( vDivMax, vDivMin, (vMax+vMin)/2 );
	}
	virtual OBJAABB* GetNext () { return m_pNext; }

	virtual HRESULT RenderTHREAD ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading );
	virtual HRESULT RenderDYNAMIC ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, CSerialFile* const SFile );
	virtual HRESULT FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile );
	virtual void	FrameMoveAABB( const float fTime, const float fElapsedTime );
	HRESULT Render ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME& sCV );
	void	Render ( const LPDIRECT3DDEVICEQ pd3dDevice );

	virtual HRESULT Render_THREAD( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, SMATERIAL_SETTING& material );
	virtual HRESULT Render_PIECE( const LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING& material );

	void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );

	// Note : Color Thread 를 위한 작업
public:
	virtual void InsertColorList( LOADINGDATALIST &listColorData );

public:
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, 
		LPCSTR& szName, const BOOL bFrontColl );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );
	void Create( const LPDIRECT3DDEVICEQ pd3dDevice, const char* szName, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, BOOL bPiece );

	std::string GetTextureName() { return m_szTexName; }
	LPDIRECT3DTEXTUREQ GetTexture() { return m_pTexture; }
	DWORD	GetFlag() { return m_dwFlag; }

	BOOL	ApplyShader( LPDIRECT3DDEVICEQ pd3dDevice, SMATERIAL_SETTING* pmaterial );
	void	UpdateLightMapUV( float fMultiX, float fMultiY );

public:
	void Save ( CSerialFile& SFile, BOOL bPiece );
	void Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece );


protected:
	void CleanUp();

public:
	DxSingleTexMaterialMesh();
	~DxSingleTexMaterialMesh();
};
typedef DxSingleTexMaterialMesh* PDXSINGLETEXMATERIALMESH;

typedef std::map<std::string,DxSingleTexMaterialMesh*>				MAPSINGLETEXMATERIALMESH;
typedef std::map<std::string,DxSingleTexMaterialMesh*>::iterator	MAPSINGLETEXMATERIALMESH_ITER;
typedef std::vector<DxSingleTexMaterialMesh*>						VECSINGLETEXMATERIALMESH;