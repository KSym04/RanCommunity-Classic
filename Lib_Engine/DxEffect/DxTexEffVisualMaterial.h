#pragma once

#include <string>
#include "./DxTexEff.h"
#include "../DxCommon/DxVertexFVF.h"

#define VISUAL_MATERIAL_MAX_TEXTURES  5

class CSerialFile;
struct DxMeshes;

enum EMVISUALMATERIAL_TYPE
{
	EMVISUALMATERIAL_TYPE_NONE		= 0,
	EMVISUALMATERIAL_TYPE_DIFFUSE	= 1,
	EMVISUALMATERIAL_TYPE_FLOWUV	= 2,
	EMVISUALMATERIAL_TYPE_ROTATE	= 3,
	EMVISUALMATERIAL_TYPE_SPECULAR	= 4,
};

struct TEXEFF_VISUALMATERIAL_PROPERTY
{
	//original members
	std::string		strVisualMaterial;
	DWORD			dw0; //texture num
	DWORD			dw1; //flag
	std::string		strTextures[VISUAL_MATERIAL_MAX_TEXTURES];

	void Save( CSerialFile& SFile );
	void Load_100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_102( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

	void GenerateType();

	//test
	DWORD	dwVMType;
	BOOL	bWhenDayOffGlow;
	BOOL	bSpeedGlow;
	float	fScaleFlow;
	float	fAlphaFlow;
	float	fSpeedGlow;
	float	fAlphaGlow;
	float	fSpeedFlow;	
	float	fSpeedRotate;
	D3DXVECTOR2	vSpeedFlow;
	D3DXVECTOR2	vDirectFlow;
	std::string	strTexSpec;
	std::string	strAlphaTexFlow;
	std::string	strFlowTexFlow;
	std::string	strTexGlow;
	std::string	strTexRotate;

	LPDIRECT3DTEXTUREQ	pAlphaTexFlow;
	LPDIRECT3DTEXTUREQ	pFlowTexFlow;
	LPDIRECT3DTEXTUREQ	pTexSpec;
	LPDIRECT3DTEXTUREQ	pTexGlow;
	LPDIRECT3DTEXTUREQ	pTexRotate;


	TEXEFF_VISUALMATERIAL_PROPERTY();
	~TEXEFF_VISUALMATERIAL_PROPERTY();
};

class DxTexEffVisualMaterial : public DxTexEffBase
{
public:
	const static DWORD	TYPEID;
	const static DWORD	VERSION;
	const static char	NAME[];

protected:
	TEXEFF_VISUALMATERIAL_PROPERTY	m_sProp;
	BOOL		m_bUpFlow;
	BOOL		m_bUpGlow;
	float		m_fTimeFlow;
	float		m_fTimeGlow;
	float		m_fTimeRotate;
	float		m_fRandSpeedFlow;
	float		m_fRandSpeedGlow;
	float		m_fRotate_ValueENDRotate;
	float		m_fSin_ValueRotate;
	float		m_fCos_ValueRotate;
	D3DXVECTOR2	m_vLastSpeedFlow;
	DWORD		m_dwColorFlow;
	DWORD		m_dwColorGlow;
	DWORD		m_dwColorRotate;

public:
	virtual DWORD GetTypeID()				{ return TYPEID; }
	virtual const char* GetName()			{ return NAME; }
	virtual void GetProperty( PBYTE &pProp ) { pProp = (PBYTE)&m_sProp; }
	virtual void SetProperty( LPDIRECT3DDEVICEQ pd3dDevice, PBYTE &pProp );

public:
	virtual void FrameMove( const float fElapsedTime );

public:
	static LPDIRECT3DSTATEBLOCK9	m_pSavedSBSpec;
	static LPDIRECT3DSTATEBLOCK9	m_pDrawSBSpec;
	static LPDIRECT3DSTATEBLOCK9	m_pSavedSBFlow;
	static LPDIRECT3DSTATEBLOCK9	m_pDrawSBFlow;
	static LPDIRECT3DSTATEBLOCK9	m_pSavedSBGlow;
	static LPDIRECT3DSTATEBLOCK9	m_pDrawSBGlow;
	static LPDIRECT3DSTATEBLOCK9	m_pSavedSBRotate;
	static LPDIRECT3DSTATEBLOCK9	m_pDrawSBRotate;

	static void OnCreateDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	static void OnReleaseDevice( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void RenderSpec( LPDIRECT3DDEVICEQ pd3dDevice, LPD3DXMESH pMesh, const DWORD dwAttrib );
	void SetRenderBeginSpec( LPDIRECT3DDEVICEQ pd3dDevice );
	void SetRenderEndSpec( LPDIRECT3DDEVICEQ pd3dDevice );

	void RenderFlow( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DVERTEXBUFFERQ pVB, LPDIRECT3DINDEXBUFFERQ pIB, LPD3DXATTRIBUTERANGE pAttrib, const DWORD dwAttrib, VERTEX* pVertSrc, DWORD dwVert );
	void RenderFlow( LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces, VERTEXNORCOLORTEX_850* pVertSrc );
	void SetRenderBeginFlow( LPDIRECT3DDEVICEQ pd3dDevice );
	void SetRenderEndFlow( LPDIRECT3DDEVICEQ pd3dDevice );

	void RenderGlow( LPDIRECT3DDEVICEQ pd3dDevice, LPD3DXMESH pMesh, const DWORD dwAttrib );
	void SetRenderBeginGlow( LPDIRECT3DDEVICEQ pd3dDevice );
	void SetRenderEndGlow( LPDIRECT3DDEVICEQ pd3dDevice );

	void SetRenderBeginRotate( LPDIRECT3DDEVICEQ pd3dDevice );
	void SetRenderEndRotate( LPDIRECT3DDEVICEQ pd3dDevice );
	void RenderRotate( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DVERTEXBUFFERQ pVB, LPDIRECT3DINDEXBUFFERQ pIB, LPD3DXATTRIBUTERANGE pAttrib, const DWORD dwAttrib, VERTEX* pVertSrc, DWORD dwVert );
	void RenderRotate( LPDIRECT3DDEVICEQ pd3dDevice, const DWORD dwVert, const DWORD dwFaces, VERTEXNORCOLORTEX_850* pVertSrc );

public:
	void FrameMoveMultiTexFlow( LPDIRECT3DVERTEXBUFFERQ pVB, DWORD dwVert, VERTEX* pVertSrc );
	void FrameMoveMultiTexRotate( LPDIRECT3DVERTEXBUFFERQ pVB, DWORD dwVert, VERTEX* pVertSrc );

public:
	BOOL	IsUseGlow()		{ return BOOL( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_DIFFUSE); }
	BOOL	IsUseFlow()		{ return BOOL( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_FLOWUV); }
	BOOL	IsUseRotate()	{ return BOOL( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_ROTATE); }
	BOOL	IsUseSpec()		{ return BOOL( m_sProp.dwVMType == EMVISUALMATERIAL_TYPE_SPECULAR); }

public:
	TEXEFF_VISUALMATERIAL_PROPERTY* GetProperty()	{ return &m_sProp; }

public:
	virtual void SavePSF( CSerialFile& SFile );
	virtual void LoadPSF( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

public:
	DxTexEffVisualMaterial();
	virtual ~DxTexEffVisualMaterial();
};
