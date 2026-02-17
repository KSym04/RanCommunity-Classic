/*!
 * \file DxMaterialManager.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */

#if !defined(_DXMATERIAL_MANAGER_INCLUDED_H_)
#define _DXMATERIAL_MANAGER_INCLUDED_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/SerialFile.h"

struct SMATERIAL_SHADER_PARAM;
struct SMATERIAL_SETTING;
struct SSHADER_BASE;
class DxSingleTexMaterialMesh;

struct SMATERIAL_STATIC
{
	std::string strMaterialName;
	std::string strShaderFile;
	std::string strTechniqueLow;
	std::string strTechniqueHigh;

	std::string g_BaseTexture;
	std::string g_BaseTexture2;
	std::string g_NormalTexture;
	std::string g_CubeTexture;
	std::string g_ReflectTexRT;

	//blend
	float g_fRotate_UV;
	float g_fScaleFactor;

	//blend2
	float g_fSpecularPower1;
	float g_fSpecularIntensity1;
	float g_fSpecularPower2;
	float g_fSpecularIntensity2;

	//water-glass-cube
	D3DXVECTOR4	g_vReflectPower;
	D3DXVECTOR4	g_vMoveSpeed;
	float g_fWaveDensity;
	float g_fWaveScale;
	float g_fWaveSpeed;

	//stream
	float g_fImageScale0;
	float g_fImageScale1;
	D3DXVECTOR4	g_vMoveSpeed0;
	D3DXVECTOR4	g_vMoveSpeed1;
	float g_fColorPower0;
	float g_fColorPower1;

	//color updown
	float g_fTexColorUpDown;
	float g_fTexColorUpDownMin;
	float g_fTexColorUpDownAdd;
	float g_fTexColorUpDownSpeed;
	
	LPDIRECT3DTEXTUREQ	m_pTextureBase;
	LPDIRECT3DTEXTUREQ	m_pTextureBase2;
	LPDIRECT3DTEXTUREQ	m_pNormalTexture;
	LPDIRECT3DCUBETEXTUREQ	m_pCubeTexture;
	LPDIRECT3DTEXTUREQ	m_pReflectTexRT;

	ID3DXEffect* m_pEffect;

	SMATERIAL_STATIC();
	~SMATERIAL_STATIC();

	void LoadTexture( LPDIRECT3DDEVICEQ pd3dDevice );
	void ReleaseTexture();

	void LoadEffect( LPDIRECT3DDEVICEQ pd3dDevice );

	void FrameMove( DxSingleTexMaterialMesh* pmesh, const float fTime, const float fElapsedTime );
	BOOL ApplyMaterialShader( LPDIRECT3DDEVICEQ pd3dDevice, DxSingleTexMaterialMesh* pmesh, SMATERIAL_SETTING* pmaterial );

	BOOL Load( CSerialFile& SFile );
	BOOL Save( CSerialFile& SFile );
};

class DxMaterialManager
{
	enum 
	{
		MATERIAL_VERSION	= 0x0200,
		VERSION_ENCODE		= 0x0200,
	};
protected:
	std::string		m_strPATH;

public:
	typedef std::map<std::string,SMATERIAL_STATIC>		SMATERIAL_STATIC_MAP;
	typedef SMATERIAL_STATIC_MAP::iterator				SMATERIAL_STATIC_MAP_ITER;

	SMATERIAL_STATIC_MAP	m_mapMaterial;

public:
	void SetPath ( std::string strPATH )	{ m_strPATH = strPATH; }
	std::string GetPath ()					{ return m_strPATH.c_str(); }

	SMATERIAL_STATIC* GetMaterial( std::string strMaterial );

	//old system
	BOOL ImportMaterial( std::string strFile );
	BOOL ImportMaterialList( std::string strFile );

	BOOL LoadFile( LPDIRECT3DDEVICEQ pd3dDevice, std::string strFile );
	BOOL SaveFile( std::string strfile );

	void ClearTexture();

protected:
	DxMaterialManager();

public:
	~DxMaterialManager();

public:
	static DxMaterialManager& GetInstance();
};

#endif // !defined(_DXMATERIAL_MANAGER_INCLUDED_H_)
