/*!
 * \file DxAABBOctreeLightMap.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */
#pragma once

#include "./DxCustomTypes.h"
#include "./LandTypeDef.h"
#include "../Common/SerialFile.h"
#include "./DxAABBOctree.h"

class DxTexEffBase;
class DxTexEffFlowUV;
class DxTexEffRotate;
class DxTexEffVisualMaterial;
struct OBJOCTree;

class DxAABBOctreeLightMap : public DxAABBOctree
{
protected:
	const static DWORD	TYPEID;
	const static DWORD	VERSION;

	virtual DWORD GetTypeID() { return TYPEID; }

public:
	void CleanUpDerived();

public:	//type 1 members
	DWORD		unknown_var1;
	std::string strLightMapD0;
	std::string strLightMapN0;
	std::string strLightMapD1;
	std::string strLightMapN1;

public:
	virtual void Create( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, EM_LAND_TYPE emLandType );

public:
	virtual void Save ( CSerialFile& SFile, BOOL bPiece );
	virtual void Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece );

public:
	DxAABBOctreeLightMap();
	virtual ~DxAABBOctreeLightMap();
};
typedef DxAABBOctreeLightMap* PDXAABBOCTREE_LIGHTMAP;