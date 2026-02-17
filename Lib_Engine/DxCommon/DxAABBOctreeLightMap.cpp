#include "stdafx.h"
#include "./DxAABBOctreeLightMap.h"

#include "./OBJOCTree.h"
#include "./NsOCTree.h"


#include "../DxEffect/DxTexEffMan.h"
#include "../DxEffect/DxTexEffDiffuse.h"
#include "../DxEffect/DxTexEffFlowUV.h"
#include "../DxEffect/DxTexEffRotate.h"
#include "../DxEffect/DxTexEffSpecular.h"
#include "../DxEffect/DxTexEffVisualMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD DxAABBOctreeLightMap::TYPEID = EMAABBCTREE_TYPE_LIGHTMAP;

DxAABBOctreeLightMap::DxAABBOctreeLightMap() :
	unknown_var1(0),
	strLightMapD0(""),
	strLightMapN0(""),
	strLightMapD1(""),
	strLightMapN1("")
{
};

DxAABBOctreeLightMap::~DxAABBOctreeLightMap()
{
	CleanUpDerived();
}

void DxAABBOctreeLightMap::CleanUpDerived()
{
}

void DxAABBOctreeLightMap::Create ( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, EM_LAND_TYPE emLandType )
{
}

