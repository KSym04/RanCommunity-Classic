#include "stdafx.h"
#include "./DxMaterialManager.h"
#include "../Common/IniLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL DxMaterialManager::ImportMaterial( std::string strFile )
{
	std::string	filepath(m_strPATH);
	filepath += strFile.c_str();

	CIniLoader cFILE;
	if( !cFILE.open( filepath.c_str(), true) )
	{
		CDebugSet::ErrorFile( "DxMaterialManager::ImportMaterial", strFile.c_str() );
		return FALSE;
	}

	cFILE.SetUseErrorMsgBox( FALSE );

	SMATERIAL_STATIC material;
	material.strMaterialName = strFile.substr(0, strFile.find_last_of("."));

	cFILE.getflag( "MATERIAL", "strShaderFile", 0, 1, material.strShaderFile );
	cFILE.getflag( "MATERIAL", "strTechniqueLow", 0, 1, material.strTechniqueLow );
	cFILE.getflag( "MATERIAL", "strTechniqueHigh", 0, 1, material.strTechniqueHigh );

	cFILE.getflag( "MATERIAL", "g_BaseTexture", 0, 1, material.g_BaseTexture );
	cFILE.getflag( "MATERIAL", "g_BaseTexture2", 0, 1, material.g_BaseTexture2 );
	cFILE.getflag( "MATERIAL", "g_NormalTexture", 0, 1, material.g_NormalTexture );
	cFILE.getflag( "MATERIAL", "g_CubeTexture", 0, 1, material.g_CubeTexture );
	cFILE.getflag( "MATERIAL", "g_ReflectTexRT", 0, 1, material.g_ReflectTexRT );

	cFILE.getflag( "MATERIAL", "g_fRotate_UV", 0, 1, material.g_fRotate_UV );
	cFILE.getflag( "MATERIAL", "g_fScaleFactor", 0, 1, material.g_fScaleFactor );

	cFILE.getflag( "MATERIAL", "g_fSpecularPower1", 0, 1, material.g_fSpecularPower1 );
	cFILE.getflag( "MATERIAL", "g_fSpecularIntensity1", 0, 1, material.g_fSpecularIntensity1 );
	cFILE.getflag( "MATERIAL", "g_fSpecularPower2", 0, 1, material.g_fSpecularPower2 );
	cFILE.getflag( "MATERIAL", "g_fSpecularIntensity2", 0, 1, material.g_fSpecularIntensity2 );

	cFILE.getflag( "MATERIAL", "g_vReflectPower", 0, 4, material.g_vReflectPower.x );
	cFILE.getflag( "MATERIAL", "g_vReflectPower", 1, 4, material.g_vReflectPower.y );
	cFILE.getflag( "MATERIAL", "g_vReflectPower", 2, 4, material.g_vReflectPower.z );
	cFILE.getflag( "MATERIAL", "g_vReflectPower", 3, 4, material.g_vReflectPower.w );

	cFILE.getflag( "MATERIAL", "g_vMoveSpeed", 0, 4, material.g_vMoveSpeed.x );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed", 1, 4, material.g_vMoveSpeed.y );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed", 2, 4, material.g_vMoveSpeed.z );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed", 3, 4, material.g_vMoveSpeed.w );

	cFILE.getflag( "MATERIAL", "g_fWaveDensity", 0, 1, material.g_fWaveDensity );
	cFILE.getflag( "MATERIAL", "g_fWaveScale", 0, 1, material.g_fWaveScale );
	cFILE.getflag( "MATERIAL", "g_fWaveSpeed", 0, 1, material.g_fWaveSpeed );

	cFILE.getflag( "MATERIAL", "g_fImageScale0", 0, 1, material.g_fImageScale0 );
	cFILE.getflag( "MATERIAL", "g_fImageScale1", 0, 1, material.g_fImageScale1 );

	cFILE.getflag( "MATERIAL", "g_fColorPower0", 0, 1, material.g_fColorPower0 );
	cFILE.getflag( "MATERIAL", "g_fColorPower1", 0, 1, material.g_fColorPower1 );

	cFILE.getflag( "MATERIAL", "g_vMoveSpeed0", 0, 4, material.g_vMoveSpeed0.x );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed0", 1, 4, material.g_vMoveSpeed0.y );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed0", 2, 4, material.g_vMoveSpeed0.z );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed0", 3, 4, material.g_vMoveSpeed0.w );

	cFILE.getflag( "MATERIAL", "g_vMoveSpeed1", 0, 4, material.g_vMoveSpeed1.x );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed1", 1, 4, material.g_vMoveSpeed1.y );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed1", 2, 4, material.g_vMoveSpeed1.z );
	cFILE.getflag( "MATERIAL", "g_vMoveSpeed1", 3, 4, material.g_vMoveSpeed1.w );

	cFILE.getflag( "MATERIAL", "g_fTexColorUpDown", 0, 1, material.g_fTexColorUpDown );
	cFILE.getflag( "MATERIAL", "g_fTexColorUpDownMin", 0, 1, material.g_fTexColorUpDownMin );
	cFILE.getflag( "MATERIAL", "g_fTexColorUpDownAdd", 0, 1, material.g_fTexColorUpDownAdd );
	cFILE.getflag( "MATERIAL", "g_fTexColorUpDownSpeed", 0, 1, material.g_fTexColorUpDownSpeed );

	std::transform( material.strMaterialName.begin(), material.strMaterialName.end(), material.strMaterialName.begin(), std::tolower );
	m_mapMaterial.insert( std::make_pair( material.strMaterialName, material ) );

	return TRUE;
}

BOOL DxMaterialManager::ImportMaterialList( std::string strFile )
{
	std::string	filepath(m_strPATH);
	filepath += strFile;

	CIniLoader cFILE;
	if( !cFILE.open( filepath.c_str(), true) )
	{
		return FALSE;
	}

	//m_mapMaterial.clear();

	DWORD count = cFILE.GetKeySize( "MATERIAL_LIST", "MATERIAL" );
	for ( DWORD i=0; i<count; ++i )
	{
		std::string material_file;
		cFILE.getflag( i, "MATERIAL_LIST", "MATERIAL", 0, 1, material_file );

		ImportMaterial( material_file );
	}

	return TRUE;
}


