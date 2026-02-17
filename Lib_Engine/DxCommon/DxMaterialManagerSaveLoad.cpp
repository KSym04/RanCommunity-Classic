#include "stdafx.h"
#include "./DxMaterialManager.h"
#include "./DxShaderManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL SMATERIAL_STATIC::Load( CSerialFile& SFile )
{
	SFile >> strMaterialName;
	SFile >> strShaderFile;
	SFile >> strTechniqueLow;
	SFile >> strTechniqueHigh;

	SFile >> g_BaseTexture;
	SFile >> g_BaseTexture2;
	SFile >> g_NormalTexture;
	SFile >> g_CubeTexture;
	SFile >> g_ReflectTexRT;

	//blend
	SFile >> g_fRotate_UV;
	SFile >> g_fScaleFactor;

	//blend2
	SFile >> g_fSpecularPower1;
	SFile >> g_fSpecularIntensity1;
	SFile >> g_fSpecularPower2;
	SFile >> g_fSpecularIntensity2;

	//water-glass-cube
	SFile >> g_vReflectPower;
	SFile >> g_vMoveSpeed;
	SFile >> g_fWaveDensity;
	SFile >> g_fWaveScale;
	SFile >> g_fWaveSpeed;

	//stream
	SFile >> g_fImageScale0;
	SFile >> g_fImageScale1;
	SFile >> g_vMoveSpeed0;
	SFile >> g_vMoveSpeed1;
	SFile >> g_fColorPower0;
	SFile >> g_fColorPower1;

	//color updown
	SFile >> g_fTexColorUpDown;
	SFile >> g_fTexColorUpDownMin;
	SFile >> g_fTexColorUpDownAdd;
	SFile >> g_fTexColorUpDownSpeed;

	return TRUE;
}

BOOL SMATERIAL_STATIC::Save( CSerialFile& SFile )
{
	SFile << strMaterialName;
	SFile << strShaderFile;
	SFile << strTechniqueLow;
	SFile << strTechniqueHigh;

	SFile << g_BaseTexture;
	SFile << g_BaseTexture2;
	SFile << g_NormalTexture;
	SFile << g_CubeTexture;
	SFile << g_ReflectTexRT;

	//blend
	SFile << g_fRotate_UV;
	SFile << g_fScaleFactor;

	//blend2
	SFile << g_fSpecularPower1;
	SFile << g_fSpecularIntensity1;
	SFile << g_fSpecularPower2;
	SFile << g_fSpecularIntensity2;

	//water-glass-cube
	SFile << g_vReflectPower;
	SFile << g_vMoveSpeed;
	SFile << g_fWaveDensity;
	SFile << g_fWaveScale;
	SFile << g_fWaveSpeed;

	//stream
	SFile << g_fImageScale0;
	SFile << g_fImageScale1;
	SFile << g_vMoveSpeed0;
	SFile << g_vMoveSpeed1;
	SFile << g_fColorPower0;
	SFile << g_fColorPower1;

	//color updown
	SFile << g_fTexColorUpDown;
	SFile << g_fTexColorUpDownMin;
	SFile << g_fTexColorUpDownAdd;
	SFile << g_fTexColorUpDownSpeed;

	return TRUE;
}


BOOL DxMaterialManager::LoadFile( LPDIRECT3DDEVICEQ pd3dDevice, std::string strfile )
{
	std::string strfilepath = m_strPATH;
	strfilepath += strfile;

	CSerialFile sfile;
	if ( !sfile.OpenFile ( FOT_READ, strfilepath.c_str() ) ) return FALSE;	

	m_mapMaterial.clear();

	DWORD version, datasize;
	sfile >> version;
	sfile >> datasize;

	if ( version >= VERSION_ENCODE )
		sfile.SetEncodeType( EMRCRYPT_BYTE_TYPE_MAP_MATERIAL_VER_1 );

	if ( version == MATERIAL_VERSION )
	{
		DWORD num;
		sfile >> num;
		for ( DWORD i=0; i<num; ++i )
		{
			SMATERIAL_STATIC material;
			BOOL bok = material.Load( sfile );
			if ( bok )
			{
				std::transform( material.strMaterialName.begin(), material.strMaterialName.end(), material.strMaterialName.begin(), std::tolower );
				m_mapMaterial.insert( std::make_pair( material.strMaterialName, material ) );
			}
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxMaterialManager::LoadFile", sfile.GetFileName(), version );
		DWORD dwCur = sfile.GetfTell();
		sfile.SetOffSet( dwCur+datasize );

		return FALSE;
	}

	return TRUE;
}

BOOL DxMaterialManager::SaveFile ( std::string strfile )
{
	std::string strfilepath = m_strPATH;
	strfilepath += strfile;

	CSerialFile sfile;
	if ( !sfile.OpenFile ( FOT_WRITE, strfilepath.c_str() ) )	return FALSE;

	sfile << (DWORD)MATERIAL_VERSION;
	sfile.BeginBlock();

	sfile.SetEncodeType( EMRCRYPT_BYTE_TYPE_MAP_MATERIAL_VER_1 );

	sfile << (DWORD)m_mapMaterial.size();
	for( SMATERIAL_STATIC_MAP_ITER it = m_mapMaterial.begin(); it != m_mapMaterial.end(); ++it )
	{
		(*it).second.Save( sfile );
	}

	sfile.SetEncodeType( EMRCRYPT_BYTE_TYPE_NONE );

	sfile.EndBlock();

	return TRUE;
}
