#include "stdafx.h"
#include "DxAABBOctree.h"

#include "OBJOCTree.h"
#include "NsOCTree.h"


#include "../DxEffect/DxTexEffMan.h"
#include "../DxEffect/DxTexEffDiffuse.h"
#include "../DxEffect/DxTexEffFlowUV.h"
#include "../DxEffect/DxTexEffRotate.h"
#include "../DxEffect/DxTexEffSpecular.h"
#include "../DxEffect/DxTexEffVisualMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxAABBOctree::DxAABBOctree() :
	m_vMax(-FLT_MAX,-FLT_MAX,-FLT_MAX),
	m_vMin(FLT_MAX,FLT_MAX,FLT_MAX),
	m_pOcTree(NULL)
{
};

DxAABBOctree::~DxAABBOctree()
{
	CleanUpBase();
}

void DxAABBOctree::CleanUpBase()
{
	SAFE_DELETE ( m_pOcTree );
}

void DxAABBOctree::FirstLoad( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXVECTOR3& vMax, const D3DXVECTOR3& vMin, CSerialFile* const SFile )
{
	if( !SFile )	return;

	NSOCTREE::FirstLoad ( pd3dDevice, *SFile, m_pOcTree, vMax, vMin );
}

void DxAABBOctree::RenderTHREAD ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, LOADINGDATALIST &listLoadingData, CRITICAL_SECTION &CSLockLoading )
{
	NSOCTREE::ThreadLoad( m_pOcTree, sCV, listLoadingData, CSLockLoading );
	NSOCTREE::RenderOctree( pd3dDevice, sCV, m_pOcTree );
}

void DxAABBOctree::RenderTHREAD( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	NSOCTREE::RenderOctree( pd3dDevice, sCV, m_pOcTree );
}

void DxAABBOctree::RenderTHREAD_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffFlowUV* pEff )
{
	NSOCTREE::RenderOctree_FlowUV( pd3dDevice, sCV, m_pOcTree, pEff );
}

void DxAABBOctree::RenderTHREAD_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffRotate* pEff )
{
	NSOCTREE::RenderOctree_Rotate( pd3dDevice, sCV, m_pOcTree, pEff );
}

void DxAABBOctree::RenderTHREAD_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffVisualMaterial* pEff )
{
	NSOCTREE::RenderOctree_FlowUV( pd3dDevice, sCV, m_pOcTree, pEff );
}

void DxAABBOctree::RenderTHREAD_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, DxTexEffVisualMaterial* pEff )
{
	NSOCTREE::RenderOctree_Rotate( pd3dDevice, sCV, m_pOcTree, pEff );
}

void DxAABBOctree::RenderDYNAMIC ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV, CSerialFile* const SFile )
{
	if( SFile )
	{
		NSOCTREE::DynamicLoad ( pd3dDevice, *SFile, m_pOcTree, sCV );
		NSOCTREE::RenderOctree ( pd3dDevice, sCV, m_pOcTree );
	}
	else
	{
		NSOCTREE::RenderOctree ( pd3dDevice, sCV, m_pOcTree );
	}
}

void DxAABBOctree::Render ( const LPDIRECT3DDEVICEQ pd3dDevice, const CLIPVOLUME &sCV )
{
	NSOCTREE::RenderOctree( pd3dDevice, sCV, m_pOcTree );
}

void DxAABBOctree::Render ( const LPDIRECT3DDEVICEQ pd3dDevice )
{
	NSOCTREE::RenderList( pd3dDevice, m_pOcTree );
}

void DxAABBOctree::Render_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffFlowUV* pEff )
{
	NSOCTREE::RenderList_FlowUV( pd3dDevice, m_pOcTree, pEff );
}

void DxAABBOctree::Render_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffRotate* pEff )
{
	NSOCTREE::RenderList_Rotate( pd3dDevice, m_pOcTree, pEff );
}

void DxAABBOctree::Render_FlowUV( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffVisualMaterial* pEff )
{
	NSOCTREE::RenderList_FlowUV( pd3dDevice, m_pOcTree, pEff );
}

void DxAABBOctree::Render_Rotate( const LPDIRECT3DDEVICEQ pd3dDevice, DxTexEffVisualMaterial* pEff )
{
	NSOCTREE::RenderList_Rotate( pd3dDevice, m_pOcTree, pEff );
}

void DxAABBOctree::InsertColorList( LOADINGDATALIST &listColorData )
{
	NSOCTREE::InsertColorList( m_pOcTree, listColorData );
}

BOOL DxAABBOctree::IsCollisionLine( const D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd, D3DXVECTOR3 &vColl, D3DXVECTOR3 &vNor, const BOOL bFrontColl )
{
	BOOL bColl = FALSE;
	NSOCTREE::CollisionLine( m_pOcTree, vStart, vEnd, vColl, vNor, bColl, bFrontColl );

	return bColl;
}

void DxAABBOctree::UpdateLightMapUV( float fMultiX, float fMultiY )
{
	NSOCTREE::UpdateLightMapUV( m_pOcTree, fMultiX, fMultiY );
}

void DxAABBOctree::CopyMesh( const LPDIRECT3DDEVICEQ pd3dDevice, VECDXMESH& vecMesh )
{
	NSOCTREE::CopyMesh( pd3dDevice, m_pOcTree, vecMesh );
}