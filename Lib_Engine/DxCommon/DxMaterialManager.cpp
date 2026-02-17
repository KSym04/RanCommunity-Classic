#include "stdafx.h"
#include "./DxMaterialManager.h"
#include "./DxShaderManager.h"

#include "../Common/IniLoader.h"

#include "./TextureManager.h"
#include "./MaterialShaderParam.h"
#include "./DxShaderManager.h"
#include "./DxSingleTexMaterialMesh.h"
#include "./DxViewPort.h"
#include "./DxFogMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SMATERIAL_STATIC::SMATERIAL_STATIC()
	: strMaterialName("")
	, strShaderFile("")
	, strTechniqueLow("")
	, strTechniqueHigh("")

	, g_BaseTexture("")
	, g_BaseTexture2("")
	, g_NormalTexture("")
	, g_CubeTexture("")

	, g_fRotate_UV(1.0f)
	, g_fScaleFactor(1.0f)
	, g_fSpecularPower1(0.0f)
	, g_fSpecularIntensity1(0.0f)
	, g_fSpecularPower2(0.0f)
	, g_fSpecularIntensity2(0.0f)

	, g_vReflectPower(0.8f,0.7f,1.0f,1.0f)
	, g_vMoveSpeed(0.0f,0.01f,1.0f,1.0f)
	, g_fWaveDensity(0.5f)
	, g_fWaveScale(0.02f)
	, g_fWaveSpeed(0.01f)

	, g_fImageScale0(1.0f)
	, g_fImageScale1(2.0f)
	, g_vMoveSpeed0(0.0f,-0.5f,1.0f,1.0f)
	, g_vMoveSpeed1(0.0f,-0.3f,1.0f,1.0f)
	, g_fColorPower0(0.3f)
	, g_fColorPower1(0.5f)

	, g_fTexColorUpDown(1.0f)
	, g_fTexColorUpDownMin(0.0f)
	, g_fTexColorUpDownAdd(1.0f)
	, g_fTexColorUpDownSpeed(5.0f)

	, m_pTextureBase(NULL)
	, m_pTextureBase2(NULL)
	, m_pNormalTexture(NULL)
	, m_pCubeTexture(NULL)
	, m_pEffect(NULL)
	, m_pReflectTexRT(NULL)
{

};

SMATERIAL_STATIC::~SMATERIAL_STATIC()
{
	ReleaseTexture();
};

void SMATERIAL_STATIC::LoadTexture( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( !pd3dDevice )	return;

	if ( !m_pTextureBase && g_BaseTexture.size() )
		TextureManager::LoadTexture( g_BaseTexture.c_str(), pd3dDevice, m_pTextureBase, 0L, 0L, FALSE );

	if ( !m_pTextureBase2 && g_BaseTexture2.size() )
		TextureManager::LoadTexture( g_BaseTexture2.c_str(), pd3dDevice, m_pTextureBase2, 0L, 0L, FALSE );


	if ( !m_pNormalTexture && g_NormalTexture.size() )
		TextureManager::LoadTexture( g_NormalTexture.c_str(), pd3dDevice, m_pNormalTexture, 0L, 0L, FALSE );


	if ( !m_pCubeTexture && g_CubeTexture.size() )
		TextureManager::LoadCubeTexture( g_CubeTexture.c_str(), pd3dDevice, m_pCubeTexture, 0, 0 );

	if ( !m_pReflectTexRT && g_ReflectTexRT.size() )
		TextureManager::LoadTexture( g_ReflectTexRT.c_str(), pd3dDevice, m_pReflectTexRT, 0, 0, FALSE );;
}

void SMATERIAL_STATIC::ReleaseTexture()
{
	if ( g_BaseTexture.size() )
		TextureManager::ReleaseTexture( g_BaseTexture.c_str(), m_pTextureBase );

	if ( g_BaseTexture2.size() )
		TextureManager::ReleaseTexture( g_BaseTexture2.c_str(), m_pTextureBase2 );

	if ( g_NormalTexture.size() )
		TextureManager::ReleaseTexture( g_NormalTexture.c_str(), m_pNormalTexture );

	if ( g_CubeTexture.size() )
		TextureManager::ReleaseTexture( g_CubeTexture.c_str(), m_pCubeTexture );

	if ( g_ReflectTexRT.size() )
		TextureManager::ReleaseTexture( g_ReflectTexRT.c_str(), m_pReflectTexRT );
}

void SMATERIAL_STATIC::LoadEffect( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( !pd3dDevice )	return;

	m_pEffect = NULL;
	m_pEffect = DxShaderManager::GetInstance().LoadFile( strShaderFile.c_str(), pd3dDevice );
}

void SMATERIAL_STATIC::FrameMove( DxSingleTexMaterialMesh* pmesh, const float fTime, const float fElapsedTime )
{
	if ( !pmesh )					return;
	if ( !m_pEffect )				return;

	m_pEffect->SetFloat( "g_fTime", (float)fTime );
}

BOOL SMATERIAL_STATIC::ApplyMaterialShader( LPDIRECT3DDEVICEQ pd3dDevice, DxSingleTexMaterialMesh* pmesh, SMATERIAL_SETTING* pmaterial )
{
	if ( !pmesh )					return FALSE;
	if ( !pmaterial )				return FALSE;
	if ( !m_pEffect )				return FALSE;

	//basic
	m_pEffect->SetMatrix( "g_matWorld", &pmaterial->g_matWorld );
	m_pEffect->SetMatrix( "g_matWV", &pmaterial->g_matWV );
	m_pEffect->SetMatrix( "g_matWVP", &pmaterial->g_matWVP );
	m_pEffect->SetVector( "g_vFOG", &pmaterial->g_vFOG );
	m_pEffect->SetVector( "g_vCameraFrom", &pmaterial->g_vCameraFrom );
	m_pEffect->SetVector( "g_vWindowSize", &pmaterial->g_vWindowSize );
	m_pEffect->SetFloat( "g_fAlpha", (float)pmaterial->g_fAlpha );
	m_pEffect->SetFloat( "g_fVS_1_1_ColorMulti", (float)pmaterial->g_fVS_1_1_ColorMulti );
	m_pEffect->SetVector( "g_vLightMapUV_Offset", &pmaterial->g_vLightMapUV_Offset );


	//material specific

	//blend
	m_pEffect->SetFloat( "g_fRotate_UV", g_fRotate_UV );
	m_pEffect->SetFloat( "g_fScaleFactor", g_fScaleFactor );

	//water-glass-cube
	m_pEffect->SetVector( "g_vReflectPower", &g_vReflectPower );
	m_pEffect->SetVector( "g_vMoveSpeed", &g_vMoveSpeed );
	m_pEffect->SetFloat( "g_fWaveDensity", g_fWaveDensity );
	m_pEffect->SetFloat( "g_fWaveScale", g_fWaveScale );
	m_pEffect->SetFloat( "g_fWaveSpeed", g_fWaveSpeed );

	//stream
	m_pEffect->SetFloat( "g_fImageScale0", g_fImageScale0 );
	m_pEffect->SetFloat( "g_fImageScale1", g_fImageScale1 );
	m_pEffect->SetFloat( "g_fColorPower0", g_fColorPower0 );
	m_pEffect->SetFloat( "g_fColorPower1", g_fColorPower1 );
	m_pEffect->SetVector( "g_vMoveSpeed0", &g_vMoveSpeed0 );
	m_pEffect->SetVector( "g_vMoveSpeed1", &g_vMoveSpeed1 );

	//color updown
	m_pEffect->SetFloat( "g_fTexColorUpDown", g_fTexColorUpDown );
	m_pEffect->SetFloat( "g_fTexColorUpDownMin", g_fTexColorUpDownMin );
	m_pEffect->SetFloat( "g_fTexColorUpDownAdd", g_fTexColorUpDownAdd );
	m_pEffect->SetFloat( "g_fTexColorUpDownSpeed", g_fTexColorUpDownSpeed );


	LoadTexture( pd3dDevice );

	if ( pmaterial->m_pLightMapDay )
		m_pEffect->SetTexture( "g_LightMapDayTex", pmaterial->m_pLightMapDay );

	if ( m_pTextureBase )
		m_pEffect->SetTexture( "g_BaseTexture", m_pTextureBase );
	else if ( pmesh->GetTexture() )
		m_pEffect->SetTexture( "g_BaseTexture", pmesh->GetTexture() );

	if ( m_pTextureBase2 )
		m_pEffect->SetTexture( "g_BaseTexture2", m_pTextureBase2 );

	if ( m_pNormalTexture )
		m_pEffect->SetTexture( "g_NormalTexture", m_pNormalTexture );

	if ( m_pCubeTexture )
		m_pEffect->SetTexture( "g_CubeTexture", m_pCubeTexture );

	if ( m_pReflectTexRT )
		m_pEffect->SetTexture( "g_ReflectTexRT", m_pReflectTexRT );

	//add option later on to switch technique by graphics quality setting
	HRESULT hr = m_pEffect->SetTechnique( strTechniqueHigh.c_str() );
	if ( FAILED(hr) )	return FALSE;

	return TRUE;
}



DxMaterialManager& DxMaterialManager::GetInstance()
{
	static DxMaterialManager Instance;
	return Instance;
}

DxMaterialManager::DxMaterialManager()
{
}

DxMaterialManager::~DxMaterialManager()
{
}

SMATERIAL_STATIC* DxMaterialManager::GetMaterial( std::string strMaterial )
{
	if ( strMaterial.empty() )	return NULL;

	std::transform(strMaterial.begin(), strMaterial.end(), strMaterial.begin(), std::tolower);

	SMATERIAL_STATIC_MAP_ITER iter = m_mapMaterial.find( strMaterial );
	if ( iter == m_mapMaterial.end() )	return NULL;

	return &(*iter).second;
}

void DxMaterialManager::ClearTexture()
{
	for ( SMATERIAL_STATIC_MAP_ITER iter = m_mapMaterial.begin(); iter != m_mapMaterial.end(); ++iter )
	{
		(*iter).second.ReleaseTexture();
	}
}
