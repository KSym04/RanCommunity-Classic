/*!
 * \file DxSingleTexMesh.h
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
#include "./DxAABBOctreeOrig.h"
#include "./DxAABBOctreeLightMap.h"

class DxTexEffBase;

class DxSingleTexMesh : public OBJAABB
{
protected:
	static const DWORD	VERSION_SINGLE_TEX_MESH_DATA;

protected:
	DWORD				m_dwFlag;
	DxAABBOctree*		m_pAABBOctree;
	std::string			m_szTexName;
	LPDIRECT3DTEXTUREQ	m_pTexture;
	BOOL				m_bCheckTex;	// Texture 로딩은 1번만 시도하도록 한다.

	LPDIRECT3DTEXTUREQ	m_pTextureLightMapD;
	LPDIRECT3DTEXTUREQ	m_pTextureLightMapN;
	BOOL				m_bCheckTexLightMapD;
	BOOL				m_bCheckTexLightMapN;
	std::string			m_szTexNameLightMapD;
	std::string			m_szTexNameLightMapN;

public:
	BOOL				unknown_var1;
	BOOL				unknown_var2;
	DWORD				unknown_var3;

	//wld2 load
	std::string			unknown_var4;
	BOOL				unknown_var5;

protected:
	DxTexEffBase*		m_pTexEffList;	// Texture Effect List.

public:
	DxSingleTexMesh*	m_pNext;		// Tree 만들기 위한 임시 포인터

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
	void	Render ( const LPDIRECT3DDEVICEQ pd3dDevice, DxTextureEffMan::MAPTEXEFF* pmapTexEff );

	// Note : Color Thread 를 위한 작업
public:
	virtual void InsertColorList( LOADINGDATALIST &listColorData );

public:
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, 
		LPCSTR& szName, const BOOL bFrontColl );
	BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );
	void Create( const LPDIRECT3DDEVICEQ pd3dDevice, const char* szName, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, BOOL bPiece );
	void ConvertTexEffMesh( const LPDIRECT3DDEVICEQ pd3dDevice, DxTextureEffMan* pTexEff );

	std::string GetTextureName() { return m_szTexName; }
	LPDIRECT3DTEXTUREQ GetTexture() { return m_pTexture; }

	void LoadLightMaps( const LPDIRECT3DDEVICEQ pd3dDevice );
public:
	void Save ( CSerialFile& SFile, BOOL bPiece );
	void Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece );
	void Load_VER100 ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

public:
	DxTexEffBase* GetList()	{ return m_pTexEffList; }

protected:
	void CleanUp();

public:
	DxSingleTexMesh();
	~DxSingleTexMesh();
};
typedef DxSingleTexMesh* PDXSINGLETEXMESH;


typedef std::map<std::string,DxSingleTexMesh*>				MAPSINGLETEXMESH;
typedef std::map<std::string,DxSingleTexMesh*>::iterator	MAPSINGLETEXMESH_ITER;