/*!
 * \file DxAABBOctreeOrig.h
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

class DxAABBOctreeOrig : public DxAABBOctree
{
protected:
	const static DWORD	TYPEID;
	const static DWORD	VERSION;

	virtual DWORD GetTypeID() { return TYPEID; }

public:
	void CleanUpDerived();

public:
	virtual void Create( const LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwFaceNUM, BYTE* pVertices, const DWORD dwFVF, EM_LAND_TYPE emLandType );

public:
	virtual void Save ( CSerialFile& SFile, BOOL bPiece );
	virtual void Load ( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, const BOOL bDynamicLoad, BOOL bPiece );
	
	void Load_VER100( const LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile, std::string& szName );

public:
	DxAABBOctreeOrig();
	virtual ~DxAABBOctreeOrig();
};
typedef DxAABBOctreeOrig* PDXAABBOCTREE_ORIG;