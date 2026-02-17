#include "stdafx.h"
#include "./DxLandBasic.h"

#include "../DxCommon/DxShadowMap.h"
#include "../DxCommon/DxSoftShadowMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void dxland_basic::load( CSerialFile &SFile )	
{
	DWORD dwver(0), dwsize(0);
	SFile >> dwver;
	SFile >> dwsize;

	switch( dwver )
	{
	case VERSION:
		{
			SFile >> m_blegacy_shadow;
			SFile >> m_fVS_1_1_ColorMulti;
		}break;

	case 0x0001:
		{
			SFile >> m_blegacy_shadow;
		}break;

	default:
		{
			CDebugSet::ErrorVersion( "dxland_basic::load", dwver );
			SFile.SetOffSet( SFile.GetfTell() + dwsize );
		}break;
	}

	set_shadow_type();
}

void dxland_basic::save( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;

	SFile.BeginBlock();

	SFile << m_blegacy_shadow;
	SFile << m_fVS_1_1_ColorMulti;	



	SFile.EndBlock();
}


void dxland_basic::reset()
{
	m_blegacy_shadow = false;
}

void dxland_basic::set_shadow_type()
{
	if ( m_blegacy_shadow )
	{
		DxShadowMap::GetInstance().SetUseShadow( TRUE );
		DxSoftShadowMap::GetInstance().SetUseShadow( FALSE );
	}
	else
	{
		DxShadowMap::GetInstance().SetUseShadow( FALSE );
		DxSoftShadowMap::GetInstance().SetUseShadow( TRUE );
	}
}