#include "stdafx.h"
#include "./GLLevelEtcFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL SLEVEL_ETC_FUNC::LOAD ( basestream &SFile )
{
	DWORD dwVersion, dwSize;
	SFile >> dwVersion;
	SFile >> dwSize;

	if ( dwVersion==VERSION )
	{
		LOAD_0205( SFile, dwSize );
	}
	else if ( dwVersion==0x204 )
	{
		LOAD_0204( SFile, dwSize );
	}
	else if ( dwVersion==0x203 )
	{
		LOAD_0203( SFile, dwSize );
	}
	else if ( dwVersion==0x202 )
	{
		LOAD_0202( SFile, dwSize );
	}
	else if ( dwVersion==0x201 )
	{
		LOAD_0201( SFile, dwSize );
	}
	else if ( dwVersion==0x200 )
	{
		LOAD_0200( SFile, dwSize );
	}
	else if ( dwVersion==0x0106 )
	{
		LOAD_0106( SFile, dwSize );
	}
	else if ( dwVersion==0x0105 )
	{
		LOAD_0105( SFile, dwSize );
	}
	else if ( dwVersion==0x0104 )
	{
		LOAD_0104( SFile, dwSize );
	}
	else if ( dwVersion==0x0103 )
	{
		LOAD_0103( SFile, dwSize );
	}
	else if ( dwVersion==0x0102 )
	{
		LOAD_0102( SFile, dwSize );
	}
	else if ( dwVersion==0x0101 )
	{
		LOAD_0101( SFile, dwSize );
	}
	else if ( dwVersion==0x0100 )
	{
		LOAD_0100( SFile, dwSize );
	}
	else
	{
		CDebugSet::ErrorVersion( "SLEVEL_ETC_FUNC::LOAD", dwVersion );
		SFile.SetOffSet ( SFile.GetfTell()+dwSize );
	}

	return TRUE;
}

void SLEVEL_ETC_FUNC::LOAD_0100 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = FALSE;
			landeffect.dummy_var2 = 0.0f;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	dwRead = SFile.GetfTell() - dwRead;
	
	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0101 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0101 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = FALSE;
			landeffect.dummy_var2 = 0.0f;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	dwRead = SFile.GetfTell() - dwRead;
	
	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0101 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0102 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	bool bUnknown1(false);
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];
	SFile >> bUnknown1;

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = FALSE;
			landeffect.dummy_var2 = 0.0f;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0102 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0103 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];
	SFile >> m_bUseFunction[EMETCFUNC_NORESPAWN];

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = FALSE;
			landeffect.dummy_var2 = 0.0f;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0103 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0104 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];
	SFile >> m_bUseFunction[EMETCFUNC_NORESPAWN];

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> bdata;
			SFile >> fdata;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0104 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0105 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];
	SFile >> m_bUseFunction[EMETCFUNC_NORESPAWN];
	SFile >> m_bUseFunction[EMETCFUNC_UNKNOWN2];

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> bdata;
			SFile >> fdata;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	//unknown data
	if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0105 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0106 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];
	SFile >> m_bUseFunction[EMETCFUNC_NORESPAWN];
	SFile >> m_bUseFunction[EMETCFUNC_UNKNOWN2];

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR3 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vminpos.z;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> vmaxpos.z;
			SFile >> bdata;
			SFile >> fdata;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = min( vminpos.y, vmaxpos.y );
			landeffect.vMinPos.z = min( vminpos.z, vmaxpos.z );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = max( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.z = max( vminpos.z, vmaxpos.z );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	//unknown data
	if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0105 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0200 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR2 vminpos, vmaxpos;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = 0.0f;
			landeffect.vMinPos.z = min( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = 0.0f;
			landeffect.vMaxPos.z = max( vminpos.y, vmaxpos.y );
			landeffect.dummy_var1 = FALSE;
			landeffect.dummy_var2 = 0.0f;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0200 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0201 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	//bug, fixed in next version
	//SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );
	SFile >> m_bUseFunction[EMETCFUNC_LIMITTIME];
	SFile >> m_bUseFunction[EMETCFUNC_CONTROLCAM];
	SFile >> m_bUseFunction[EMETCFUNC_LANDEFFECT];
	SFile >> m_bUseFunction[EMETCFUNC_DIFFICULTY];
	SFile >> m_bUseFunction[EMETCFUNC_AROUNDREGEN];
	SFile >> m_bUseFunction[EMETCFUNC_MOBCHASE];
	SFile >> m_bUseFunction[EMETCFUNC_NORESPAWN];

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR3 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vminpos.z;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> vmaxpos.z;
			SFile >> bdata;
			SFile >> fdata;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = min( vminpos.y, vmaxpos.y );
			landeffect.vMinPos.z = min( vminpos.z, vmaxpos.z );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = max( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.z = max( vminpos.z, vmaxpos.z );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	if( m_bUseFunction[EMETCFUNC_NORESPAWN] )  //bug, fixed in next version
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0201 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0202 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR3 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vminpos.z;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> vmaxpos.z;
			SFile >> bdata;
			SFile >> fdata;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = min( vminpos.y, vmaxpos.y );
			landeffect.vMinPos.z = min( vminpos.z, vmaxpos.z );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = max( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.z = max( vminpos.z, vmaxpos.z );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;

			m_vecLandEffect.push_back(landeffect);
		}
	}

	if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0202 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0203 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR3 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;
			DWORD dwSchoolFlag;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vminpos.z;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> vmaxpos.z;
			SFile >> bdata;
			SFile >> fdata;
			SFile >> dwSchoolFlag;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = min( vminpos.y, vmaxpos.y );
			landeffect.vMinPos.z = min( vminpos.z, vmaxpos.z );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = max( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.z = max( vminpos.z, vmaxpos.z );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;
			landeffect.dwSchool = dwSchoolFlag;			/* land effect school logic, Juver, 2019/12/28 */

			m_vecLandEffect.push_back(landeffect);
		}
	}

	if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0203 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0204 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR3 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;
			DWORD dwSchoolFlag;
			DWORD dwCTFTeamFlag;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vminpos.z;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> vmaxpos.z;
			SFile >> bdata;
			SFile >> fdata;
			SFile >> dwSchoolFlag;
			SFile >> dwCTFTeamFlag;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = min( vminpos.y, vmaxpos.y );
			landeffect.vMinPos.z = min( vminpos.z, vmaxpos.z );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = max( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.z = max( vminpos.z, vmaxpos.z );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;
			landeffect.dwSchool = dwSchoolFlag;			/* land effect school logic, Juver, 2019/12/28 */
			landeffect.dwCTFTeam = dwCTFTeamFlag;		/* land effect ctf team requirement, Juver, 2020/09/21 */

			m_vecLandEffect.push_back(landeffect);
		}
	}

	if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0203 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

void SLEVEL_ETC_FUNC::LOAD_0205 ( basestream &SFile, DWORD dwSIZE )
{
	DWORD dwRead = SFile.GetfTell();

	SFile.ReadBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );

	// Load Camera Control
	if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
	{
		SFile >> m_sControlCam.fUp;
		SFile >> m_sControlCam.fDown;
		SFile >> m_sControlCam.fLeft;
		SFile >> m_sControlCam.fRight;
		SFile >> m_sControlCam.fFar;
		SFile >> m_sControlCam.fNear;
		SFile >> m_sControlCam.vCamPos.x;			
		SFile >> m_sControlCam.vCamPos.y;
		SFile >> m_sControlCam.vCamPos.z;
	}

	// Load Limit Time
	if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
	{
		SFile >> m_sLimitTime.nLimitMinute;
		SFile >> m_sLimitTime.nTellTermMinute;
	}

	m_vecLandEffect.clear();
	// Load Land Effect
	if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
	{
		int iSize;
		SFile >> iSize;
		for( int i = 0; i < iSize; i++ )
		{
			DWORD dweffect_type = 0;
			float fvalue = 0.0f;
			D3DXVECTOR3 vminpos, vmaxpos;
			BOOL bdata = FALSE;
			float fdata = 0.0f;
			DWORD dwSchoolFlag;
			DWORD dwCTFTeamFlag;

			SFile >> dweffect_type;
			SFile >> fvalue;
			SFile >> vminpos.x;
			SFile >> vminpos.y;
			SFile >> vminpos.z;
			SFile >> vmaxpos.x;
			SFile >> vmaxpos.y;
			SFile >> vmaxpos.z;
			SFile >> bdata;
			SFile >> fdata;
			SFile >> dwSchoolFlag;
			SFile >> dwCTFTeamFlag;

			/* non vend area, Juver, 2021/01/17 */
			BOOL bUseYPos(FALSE);
			SFile >> bUseYPos;

			SLANDEFFECT landeffect;
			landeffect.emLandEffectType = (EMLANDEFFECT_TYPE)dweffect_type;
			landeffect.fValue = fvalue;
			landeffect.vMinPos.x = min( vminpos.x, vmaxpos.x );
			landeffect.vMinPos.y = min( vminpos.y, vmaxpos.y );
			landeffect.vMinPos.z = min( vminpos.z, vmaxpos.z );
			landeffect.vMaxPos.x = max( vminpos.x, vmaxpos.x );
			landeffect.vMaxPos.y = max( vminpos.y, vmaxpos.y );
			landeffect.vMaxPos.z = max( vminpos.z, vmaxpos.z );
			landeffect.dummy_var1 = bdata;
			landeffect.dummy_var2 = fdata;
			landeffect.dwSchool = dwSchoolFlag;			/* land effect school logic, Juver, 2019/12/28 */
			landeffect.dwCTFTeam = dwCTFTeamFlag;		/* land effect ctf team requirement, Juver, 2020/09/21 */
			landeffect.bUseYPos = bUseYPos;				/* non vend area, Juver, 2021/01/17 */

			m_vecLandEffect.push_back(landeffect);
		}
	}

	if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
	{
		SFile >> m_dummy_var1;
	}

	dwRead = SFile.GetfTell() - dwRead;

	if( dwRead != dwSIZE )
		CDebugSet::MsgBox( "SLEVEL_ETC_FUNC::LOAD_0203 Incorrect Read Size:%d Expected:%d", dwRead, dwSIZE );
}

BOOL SLEVEL_ETC_FUNC::SAVE ( CSerialFile &SFile )
{
	SFile << DWORD(VERSION);
	SFile.BeginBlock();
	{
		SFile.WriteBuffer( &m_bUseFunction, sizeof(m_bUseFunction) );

		// Save Camera Control
		if( m_bUseFunction[EMETCFUNC_CONTROLCAM] ) 
		{
			SFile << m_sControlCam.fUp;
			SFile << m_sControlCam.fDown;
			SFile << m_sControlCam.fLeft;
			SFile << m_sControlCam.fRight;
			SFile << m_sControlCam.fFar;
			SFile << m_sControlCam.fNear;
			SFile << m_sControlCam.vCamPos.x;			
			SFile << m_sControlCam.vCamPos.y;
			SFile << m_sControlCam.vCamPos.z;
		}

		// Save Limit Time
		if( m_bUseFunction[EMETCFUNC_LIMITTIME] ) 
		{
			SFile << m_sLimitTime.nLimitMinute;
			SFile << m_sLimitTime.nTellTermMinute;
		}

		// Save Land Effect
		if( m_bUseFunction[EMETCFUNC_LANDEFFECT] )
		{
			SFile << m_vecLandEffect.size();
			for( int i = 0; i < (int)m_vecLandEffect.size(); i++ )
			{
				SLANDEFFECT landEffect = m_vecLandEffect[i];
				SFile << (int)landEffect.emLandEffectType;
				SFile << landEffect.fValue;
				SFile << landEffect.vMinPos.x;
				SFile << landEffect.vMinPos.y;
				SFile << landEffect.vMinPos.z;
				SFile << landEffect.vMaxPos.x;
				SFile << landEffect.vMaxPos.y;
				SFile << landEffect.vMaxPos.z;
				SFile << landEffect.dummy_var1;
				SFile << landEffect.dummy_var2;
				SFile << landEffect.dwSchool;			/* land effect school logic, Juver, 2019/12/28 */
				SFile << landEffect.dwCTFTeam;			/* land effect ctf team requirement, Juver, 2020/09/21 */
				SFile << landEffect.bUseYPos;			/* non vend area, Juver, 2021/01/17 */
			}
		}

		if( m_bUseFunction[EMETCFUNC_UNKNOWN2] ) 
		{
			SFile << m_dummy_var1;
		}
	}
	SFile.EndBlock();

	return TRUE;
}