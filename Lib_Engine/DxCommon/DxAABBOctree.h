/*!
 * \file DxAABBOctree.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma once

#include "./DxCustomTypes.h"
#include "./LandTypeDef.h"
#include "../Common/SerialFile.h"
#include "./DxShadowMeshData.h"

class DxTexEffBase;
class DxTexEffFlowUV;
class DxTexEffRotate;
class DxTexEffVisualMaterial;
struct OBJOCTree;

class DxAABBOctree
{
protected:
	D3DXVECTOR3			m_vMax;
	D3DXVECTOR3			m_vMin;
	OBJOCTree*			m_pOcTree;

public:
	void CleanUpBase();

public:
	virtual DWORD GetTypeID() { return 0xffffffff; }

	void GetAABBSize ( D3DXVECTOR3 &_vMax, D3DXVECTOR3 &_vMin )
	{
		_vMax = m_vMax; _vMin = m_vMin;
	}
	virtual void FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile );
	virtual void RenderTHREAD( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading );
	virtual void RenderTHREAD( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV );
	virtual void RenderTHREAD_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffFlowUV* pEff );
	virtual void RenderTHREAD_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffRotate* pEff );
	virtual void RenderDYNAMIC ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, CSerialFile* const SFile );
	virtual void Render ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV );
	virtual void Render ( const LPDIRECT3DDEVICEQ pd3dDevice );									// 조각파일
	virtual void Render_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffFlowUV* pEff );		// 조각파일
	virtual void Render_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffRotate* pEff );		// 조각파일

	virtual void RenderTHREAD_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffVisualMaterial* pEff );
	virtual void RenderTHREAD_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffVisualMaterial* pEff );
	virtual void Render_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffVisualMaterial* pEff );
	virtual void Render_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffVisualMaterial* pEff );	

public:
	virtual void InsertColorList( LOADINGDATALIST &listColorData );
	virtual void CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh );

public:
	virtual void Create( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, EM_LAND_TYPE emLandType ){}

public:
	virtual BOOL IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl );
	virtual void UpdateLightMapUV( float fMultiX, float fMultiY );

public:
	virtual void Save ( CSerialFile& SFile, BOOL bPiece ){}
	virtual void Load( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece ){}


public:
	DxAABBOctree();
	virtual ~DxAABBOctree();
};
typedef DxAABBOctree* PDXAABBOCTREE;