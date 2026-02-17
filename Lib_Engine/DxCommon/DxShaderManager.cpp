#include "stdafx.h"
#include "DxShaderManager.h"

#include "../Common/StlFunctions.h"

#include "./DxViewPort.h"
#include "./DxMethods.h"
#include "./DxShaderData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DxShaderManager& DxShaderManager::GetInstance()
{
	static DxShaderManager Instance;
	return Instance;
}

DxShaderManager::DxShaderManager()
	: m_bShaderUse(FALSE)
	, m_strPATH("")
{
}

DxShaderManager::~DxShaderManager()
{
}

ID3DXEffect* DxShaderManager::GetShader( std::string strShader )
{
	if ( strShader.empty() )	return NULL;

	std::transform(strShader.begin(), strShader.end(), strShader.begin(), std::tolower);

	D3DXEFFECT_MAP_ITER iter = m_mapShader.find( strShader );
	if ( iter == m_mapShader.end() )	return NULL;

	return (*iter).second;
}


ID3DXEffect* DxShaderManager::LoadFile( std::string strFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( strFile.empty() )	return NULL;
	if ( !m_bShaderUse )	return NULL;

	ID3DXEffect* peffect_exist = GetShader( strFile.c_str() );
	if ( peffect_exist )	return peffect_exist;

	std::string	filepath(m_strPATH);
	filepath += strFile.c_str();

	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* errors = NULL;
	ID3DXEffect* peffect = NULL;

	HRESULT hr = D3DXCreateEffectFromFileA( pd3dDevice, filepath.c_str(), NULL, NULL, dwShaderFlags, NULL, &peffect, &errors );
	if ( hr != D3D_OK )
	{
		std::string strerror = "error";
		if ( errors )	strerror = (char*)errors->GetBufferPointer();
		CDebugSet::ErrorCode( "DxShaderManager::LoadFile", "failed to load effect error:%s file:%s info:%s", DXGetErrorString(hr), filepath.c_str(), strerror.c_str() );
		if ( errors )	errors->Release();
		return NULL;
	}

	if ( errors )	errors->Release();

	std::string strfilename = strFile;
	std::transform(strfilename.begin(), strfilename.end(), strfilename.begin(), std::tolower);

	m_mapShader.insert( std::make_pair(strfilename, peffect ) );

	return peffect;
}

ID3DXEffect* DxShaderManager::LoadString( std::string strString, std::string strName, LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( strString.empty() )	return NULL;
	if ( strName.empty() )		return NULL;
	if ( !m_bShaderUse )		return NULL;

	ID3DXEffect* peffect_exist = GetShader( strName.c_str() );
	if ( peffect_exist )	return peffect_exist;


	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* errors = NULL;
	ID3DXEffect* peffect = NULL;

	HRESULT hr = D3DXCreateEffect( pd3dDevice, strString.c_str(), strString.size() +1, NULL, NULL, dwShaderFlags, NULL, &peffect, &errors );
	if ( hr != D3D_OK )
	{
		std::string strerror = "error";
		if ( errors )	strerror = (char*)errors->GetBufferPointer();
		CDebugSet::ErrorCode( "DxShaderManager::LoadString", "failed to load effect error:%s file:%s info:%s", DXGetErrorString(hr), strName.c_str(), strerror.c_str() );
		if ( errors )	errors->Release();
		return NULL;
	}

	if ( errors )	errors->Release();

	std::string strfilename = strName;
	std::transform(strfilename.begin(), strfilename.end(), strfilename.begin(), std::tolower);

	m_mapShader.insert( std::make_pair(strfilename, peffect ) );

	return peffect;
}

void DxShaderManager::FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime )
{
}

void DxShaderManager::CreateDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	D3DCAPSQ d3dCaps;
	pd3dDevice->GetDeviceCaps ( &d3dCaps );

	m_bShaderUse = FALSE;

	//minimum vertex shader 2 and pixel shader 2
	if ( d3dCaps.VertexShaderVersion < D3DVS_VERSION(2,0) )	return;
	if ( d3dCaps.PixelShaderVersion < D3DPS_VERSION(2,0) )	return;

	m_bShaderUse = TRUE;

	//if ( d3dCaps.VertexShaderVersion >= D3DVS_VERSION(3,0) && d3dCaps.PixelShaderVersion >= D3DPS_VERSION(3,0) )
	//	m_bShader3 = TRUE;

	DxShaderData::SHADER_DATA_MAP mapdata = DxShaderData::GetInstance().m_mapShader;
	DxShaderData::SHADER_DATA_MAP_ITER iter = mapdata.begin();
	for( ; iter != mapdata.end(); ++iter )
	{
		std::string strTitle = (*iter).first;
		std::string strShader = (*iter).second;

		LoadString( strShader, strTitle, pd3dDevice );
	}
}

void DxShaderManager::ResetDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	for ( D3DXEFFECT_MAP_ITER it = m_mapShader.begin(); it!= m_mapShader.end(); ++it )
	{
		ID3DXEffect* peffect_base = (*it).second;
		if ( peffect_base )	
			peffect_base->OnResetDevice();
	}
}

void DxShaderManager::LostDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	for ( D3DXEFFECT_MAP_ITER it = m_mapShader.begin(); it!= m_mapShader.end(); ++it )
	{
		ID3DXEffect* peffect_base = (*it).second;
		if ( peffect_base )	
			peffect_base->OnLostDevice();
	}
}

void DxShaderManager::DestroyDevice()
{
	std::for_each ( m_mapShader.begin(), m_mapShader.end(), std_afunc::ReleaseMapObject() );
	m_mapShader.clear ();
}
