#include "stdafx.h"
#include "./VMSetting.h"

#include "../Common/IniLoader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLVMSetting& GLVMSetting::GetInstance ()
{
	static GLVMSetting _instance;
	return _instance;
}

GLVMSetting::GLVMSetting()
{
}

GLVMSetting::~GLVMSetting()
{
}

void GLVMSetting::CleanUp()
{
	m_mapData.clear();
}

void GLVMSetting::LoadFile ()
{
	CleanUp();

	std::string	strfeature_file(m_strPATH);
	strfeature_file += "vmset.ini";

	CIniLoader cFILE;
	if( !cFILE.open( strfeature_file, true) )
	{
		CDebugSet::ErrorFile( "GLVMSetting::LoadFile", strfeature_file.c_str() );
		return;
	}

	DWORD dwTotal = 0;
	cFILE.getflag ( "VMSETTING", "dwTotal", 0, 1, dwTotal );	

	for( DWORD i=0; i<dwTotal; ++i )
	{
		CString strsection;
		strsection.Format( "VM_PROP_%d", i );

		SVM_PROP_SETTING prop;
		cFILE.getflag( strsection.GetString(), "strName", 0, 1, prop.strName );
		cFILE.getflag( strsection.GetString(), "dwVMType", 0, 1, prop.dwVMType );

		cFILE.getflag( strsection.GetString(), "bWhenDayOffGlow", 0, 1, prop.bWhenDayOffGlow );
		cFILE.getflag( strsection.GetString(), "bSpeedGlow", 0, 1, prop.bSpeedGlow );

		cFILE.getflag( strsection.GetString(), "fScaleFlow", 0, 1, prop.fScaleFlow );
		cFILE.getflag( strsection.GetString(), "fAlphaFlow", 0, 1, prop.fAlphaFlow );
		cFILE.getflag( strsection.GetString(), "fSpeedGlow", 0, 1, prop.fSpeedGlow );
		cFILE.getflag( strsection.GetString(), "fAlphaGlow", 0, 1, prop.fAlphaGlow );
		cFILE.getflag( strsection.GetString(), "fSpeedFlow", 0, 1, prop.fSpeedFlow );
		cFILE.getflag( strsection.GetString(), "fSpeedRotate", 0, 1, prop.fSpeedRotate );
		cFILE.getflag( strsection.GetString(), "fSpeedFlowx", 0, 1, prop.fSpeedFlowx );
		cFILE.getflag( strsection.GetString(), "fSpeedFlowy", 0, 1, prop.fSpeedFlowy );
		cFILE.getflag( strsection.GetString(), "fDirectFlowx", 0, 1, prop.fDirectFlowx );
		cFILE.getflag( strsection.GetString(), "fDirectFlowy", 0, 1, prop.fDirectFlowy );

		cFILE.getflag( strsection.GetString(), "wTexSpec0", 0, 1, prop.wTexSpec0 );
		cFILE.getflag( strsection.GetString(), "wTexSpec1", 0, 1, prop.wTexSpec1 );
		cFILE.getflag( strsection.GetString(), "wAlphaTexFlow", 0, 1, prop.wAlphaTexFlow );
		cFILE.getflag( strsection.GetString(), "wFlowTexFlow", 0, 1, prop.wFlowTexFlow );
		cFILE.getflag( strsection.GetString(), "wTexGlow", 0, 1, prop.wTexGlow );
		cFILE.getflag( strsection.GetString(), "wTexRotate", 0, 1, prop.wTexRotate );

		std::transform(prop.strName.begin(), prop.strName.end(), prop.strName.begin(), ::tolower);

		m_mapData.insert( std::make_pair( prop.strName, prop ) );
	}
}

SVM_PROP_SETTING* GLVMSetting::GetProp( std::string strName )
{
	if( strName.empty() )	return NULL;

	std::transform(strName.begin(),strName.end(), strName.begin(), ::tolower);

	SVM_PROP_SETTING_MAP_ITER iter = m_mapData.find( strName );
	if ( iter == m_mapData.end() )	return NULL;

	return &(*iter).second;
}

