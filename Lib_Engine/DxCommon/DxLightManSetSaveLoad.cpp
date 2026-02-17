#include "stdafx.h"

#include "./DxMethods.h"
#include "./DxLightMan.h"
#include "../Common/SerialFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void ColorUp( D3DCOLORVALUE& d3dColor, float fValue )
{
	float fDiffR = d3dColor.r;
	float fDiffG = d3dColor.g;
	float fDiffB = d3dColor.b;

	fDiffR *= fValue;
	fDiffG *= fValue;
	fDiffB *= fValue;

	d3dColor.r = d3dColor.r + fDiffR;
	d3dColor.g = d3dColor.g + fDiffG;
	d3dColor.b = d3dColor.b + fDiffB;

	if( d3dColor.r > 1.f )		d3dColor.r = 1.f;
	if( d3dColor.g > 1.f )		d3dColor.g = 1.f;
	if( d3dColor.b > 1.f )		d3dColor.b = 1.f;
}

void DxLightMan::Load ( CSerialFile &SFile, DWORD dwLandVer, BOOL bSETLAND )
{
	//	Note : 모든 이전 데이터를 초기화.
	//
	CleanUp ();

	BOOL bResult = FALSE;

	//	Note : 이전 버전.

	DWORD dwOldVer;
	if ( bSETLAND )	dwOldVer = 0x0025;	//	DxSetLandMan::VERSION old version,
	else			dwOldVer = 0x0104;	//	DxLandMan::VERSION old version

	if ( dwLandVer < dwOldVer )
	{
		SFile >> m_bLighting;
		m_LtDirectNight.LoadSet_Ver100( SFile );	

		//	낮 Direct Light 설정.
		m_LtDirectNoon.m_BaseDiffuse = m_LtDirectNight.m_BaseDiffuse;
		m_LtDirectNoon.m_bEnable = m_LtDirectNight.m_bEnable;
		m_LtDirectNoon.m_Light = m_LtDirectNight.m_Light;
		StringCchCopy( m_LtDirectNoon.m_szLightName, 256, m_LtDirectNight.m_szLightName );
		StringCchCat( m_LtDirectNoon.m_szLightName, 256, "[NOON]" );

		ColorUp( m_LtDirectNoon.m_Light.Diffuse, 0.6f );
		ColorUp( m_LtDirectNight.m_Light.Ambient, 0.2f );
		ColorUp( m_LtDirectNoon.m_Light.Ambient, 0.2f );

		SFile >> bResult;
		if ( bResult )
		{
			m_pLtListHead = new DXLIGHT();
			m_pLtListHead->LoadSet_Ver100( SFile );
		}

		SFile >> bResult;
		if ( bResult )
		{	
			m_pTreeHead = new DXLIGHTNODE();
			m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
		}
	}
	//	버전 관리후 버전.
	else
	{
		DWORD dwVersion;
		SFile >> dwVersion;

		switch( dwVersion )
		{
		case VERSION_LIGHTMAN:	Load_Ver200( SFile );	break;
		case 0x0110:	Load_Ver110( SFile );	break;
		case 0x0109:	Load_Ver109( SFile );	break;
		case 0x0108:	Load_Ver108( SFile );	break;
		case 0x0107:	Load_Ver107( SFile );	break;
		case 0x0106:	Load_Ver106( SFile );	break;
		case 0x0105:	Load_Ver105( SFile );	break;
		case 0x0104:	Load_Ver104( SFile );	break;
		case 0x0103:	Load_Ver103( SFile );	break;
		case 0x0102:	Load_Ver102( SFile );	break;
		case 0x0101:	Load_Ver101( SFile );	break;
		case 0x0100:	Load_Ver100( SFile );	break;

		default:
			CDebugSet::ErrorVersion( "DxLightMan::Load", dwVersion );
			break;
		};
	}

	// Note : 로드 한 빛을 벡터에 넣는다.
	ListToVectorLIGHT();
}

void DxLightMan::Save ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION_LIGHTMAN;

	SFile << m_bLighting;
	SFile << m_bWhenDayPointOff;
	SFile << m_bNightAndDay;

	SFile << unknown_var1;
	SFile << unknown_var2;
	SFile << unknown_var3;
	SFile << unknown_var4;

	m_LtDirectNoon.SaveSet ( SFile );
	m_LtDirectNight.SaveSet ( SFile );

	SFile << BOOL ( m_pLtListHead );
	if ( m_pLtListHead )
	{
		m_pLtListHead->SaveSet ( SFile );		
	}

	SFile << BOOL ( m_pTreeHead );
	if ( m_pTreeHead )
	{
		m_pTreeHead->SaveSet ( SFile );		
	}
}

void	DxLightMan::Load_Ver200( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	SFile >> unknown_var1;
	SFile >> unknown_var2;
	SFile >> unknown_var3;
	SFile >> unknown_var4;

	m_LtDirectNoon.LoadSet ( SFile );
	m_LtDirectNight.LoadSet ( SFile );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		m_pLtListHead = new DXLIGHT();
		m_pLtListHead->LoadSet ( SFile );
	}

	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver110( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	SFile >> unknown_var4;

	m_LtDirectNoon.LoadSet ( SFile );
	m_LtDirectNight.LoadSet ( SFile );

	SFile >> unknown_var1;
	SFile >> unknown_var2;
	SFile >> unknown_var3;

	DWORD _dwNUM(0);
	SFile >> _dwNUM;
	for ( DWORD _dwLight=0; _dwLight<_dwNUM; ++_dwLight )
	{
		DXLIGHT* pLightLoad = new DXLIGHT();
		pLightLoad->LoadSet ( SFile );

		DXLIGHT* pCur = pLightLoad;
		while( pCur )
		{
			DXLIGHT* pLightNew = new DXLIGHT;
			*pLightNew = *pCur;

			//check name exist
			if ( GetLight( pLightNew->m_szLightName ) )
			{
				char szNum[STRING_LENGTH_256] = "";
				DWORD dwNum = 0;
				while (1)
				{
					if ( dwNum >= 1000 )	StringCchPrintf( szNum, STRING_LENGTH_256, "[%d]", dwNum++ );
					else					StringCchPrintf( szNum, STRING_LENGTH_256, "[%03d]", dwNum++ );

					StringCchCopy( pLightNew->m_szLightName, STRING_LENGTH_256, "UNNAMED" );
					StringCchCat( pLightNew->m_szLightName, STRING_LENGTH_256, szNum );

					if ( !GetLight( pLightNew->m_szLightName ) )	break;
				}
			}

			AddLight( pLightNew );
			pCur = pCur->pNext;
		}

		SAFE_DELETE(pLightLoad);
		pLightLoad = NULL;
	}

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver109( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	m_LtDirectNoon.LoadSet ( SFile );
	m_LtDirectNight.LoadSet ( SFile );

	SFile >> unknown_var1;
	SFile >> unknown_var2;
	SFile >> unknown_var3;

	DWORD _dwNUM(0);
	SFile >> _dwNUM;
	for ( DWORD _dwLight=0; _dwLight<_dwNUM; ++_dwLight )
	{
		DXLIGHT* pLightLoad = new DXLIGHT();
		pLightLoad->LoadSet ( SFile );

		DXLIGHT* pCur = pLightLoad;
		while( pCur )
		{
			DXLIGHT* pLightNew = new DXLIGHT;
			*pLightNew = *pCur;

			//check name exist
			if ( GetLight( pLightNew->m_szLightName ) )
			{
				char szNum[STRING_LENGTH_256] = "";
				DWORD dwNum = 0;
				while (1)
				{
					if ( dwNum >= 1000 )	StringCchPrintf( szNum, STRING_LENGTH_256, "[%d]", dwNum++ );
					else					StringCchPrintf( szNum, STRING_LENGTH_256, "[%03d]", dwNum++ );

					StringCchCopy( pLightNew->m_szLightName, STRING_LENGTH_256, "UNNAMED" );
					StringCchCat( pLightNew->m_szLightName, STRING_LENGTH_256, szNum );

					if ( !GetLight( pLightNew->m_szLightName ) )	break;
				}
			}

			AddLight( pLightNew );
			pCur = pCur->pNext;
		}

		SAFE_DELETE(pLightLoad);
		pLightLoad = NULL;
	}

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver108( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	m_LtDirectNoon.LoadSet_Ver106 ( SFile );
	m_LtDirectNight.LoadSet_Ver106 ( SFile );

	SFile >> unknown_var1;
	SFile >> unknown_var2;
	SFile >> unknown_var3;

	DWORD _dwNUM(0);
	SFile >> _dwNUM;
	for ( DWORD _dwLight=0; _dwLight<_dwNUM; ++_dwLight )
	{
		DXLIGHT* pLightLoad = new DXLIGHT();
		pLightLoad->LoadSet_Ver106 ( SFile );

		DXLIGHT* pCur = pLightLoad;
		while( pCur )
		{
			DXLIGHT* pLightNew = new DXLIGHT;
			*pLightNew = *pCur;

			//check name exist
			if ( GetLight( pLightNew->m_szLightName ) )
			{
				char szNum[STRING_LENGTH_256] = "";
				DWORD dwNum = 0;
				while (1)
				{
					if ( dwNum >= 1000 )	StringCchPrintf( szNum, STRING_LENGTH_256, "[%d]", dwNum++ );
					else					StringCchPrintf( szNum, STRING_LENGTH_256, "[%03d]", dwNum++ );

					StringCchCopy( pLightNew->m_szLightName, STRING_LENGTH_256, "UNNAMED" );
					StringCchCat( pLightNew->m_szLightName, STRING_LENGTH_256, szNum );

					if ( !GetLight( pLightNew->m_szLightName ) )	break;
				}
			}

			AddLight( pLightNew );
			pCur = pCur->pNext;
		}

		SAFE_DELETE(pLightLoad);
	}

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver107( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	m_LtDirectNoon.LoadSet_Ver106 ( SFile );
	m_LtDirectNight.LoadSet_Ver106 ( SFile );

	SFile >> unknown_var2;
	SFile >> unknown_var3;

	DWORD _dwNUM(0);
	SFile >> _dwNUM;
	for ( DWORD _dwLight=0; _dwLight<_dwNUM; ++_dwLight )
	{
		DXLIGHT* pLightLoad = new DXLIGHT();
		pLightLoad->LoadSet_Ver106 ( SFile );

		DXLIGHT* pCur = pLightLoad;
		while( pCur )
		{
			DXLIGHT* pLightNew = new DXLIGHT;
			*pLightNew = *pCur;

			//check name exist
			if ( GetLight( pLightNew->m_szLightName ) )
			{
				char szNum[STRING_LENGTH_256] = "";
				DWORD dwNum = 0;
				while (1)
				{
					if ( dwNum >= 1000 )	StringCchPrintf( szNum, STRING_LENGTH_256, "[%d]", dwNum++ );
					else					StringCchPrintf( szNum, STRING_LENGTH_256, "[%03d]", dwNum++ );

					StringCchCopy( pLightNew->m_szLightName, STRING_LENGTH_256, "UNNAMED" );
					StringCchCat( pLightNew->m_szLightName, STRING_LENGTH_256, szNum );

					if ( !GetLight( pLightNew->m_szLightName ) )	break;
				}
			}

			AddLight( pLightNew );
			pCur = pCur->pNext;
		}

		SAFE_DELETE(pLightLoad);
	}

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver106( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	m_LtDirectNoon.LoadSet_Ver106 ( SFile );
	m_LtDirectNight.LoadSet_Ver106 ( SFile );

	DWORD _dwNUM(0);
	SFile >> _dwNUM;
	for ( DWORD _dwLight=0; _dwLight<_dwNUM; ++_dwLight )
	{
		DXLIGHT* pLightLoad = new DXLIGHT();
		pLightLoad->LoadSet_Ver106 ( SFile );

		DXLIGHT* pCur = pLightLoad;
		while( pCur )
		{
			DXLIGHT* pLightNew = new DXLIGHT;
			*pLightNew = *pCur;

			//check name exist
			if ( GetLight( pLightNew->m_szLightName ) )
			{
				char szNum[STRING_LENGTH_256] = "";
				DWORD dwNum = 0;
				while (1)
				{
					if ( dwNum >= 1000 )	StringCchPrintf( szNum, STRING_LENGTH_256, "[%d]", dwNum++ );
					else					StringCchPrintf( szNum, STRING_LENGTH_256, "[%03d]", dwNum++ );

					StringCchCopy( pLightNew->m_szLightName, STRING_LENGTH_256, "UNNAMED" );
					StringCchCat( pLightNew->m_szLightName, STRING_LENGTH_256, szNum );

					if ( !GetLight( pLightNew->m_szLightName ) )	break;
				}
			}

			AddLight( pLightNew );
			pCur = pCur->pNext;
		}

		SAFE_DELETE(pLightLoad);
	}

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver105( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	m_LtDirectNoon.LoadSet_Ver103 ( SFile );
	m_LtDirectNight.LoadSet_Ver103 ( SFile );

	DWORD _dwNUM(0);
	SFile >> _dwNUM;
	for ( DWORD _dwLight=0; _dwLight<_dwNUM; ++_dwLight )
	{
		DXLIGHT* pLightLoad = new DXLIGHT();
		pLightLoad->LoadSet_Ver103 ( SFile );

		DXLIGHT* pCur = pLightLoad;
		while( pCur )
		{
			DXLIGHT* pLightNew = new DXLIGHT;
			*pLightNew = *pCur;

			//check name exist
			if ( GetLight( pLightNew->m_szLightName ) )
			{
				char szNum[STRING_LENGTH_256] = "";
				DWORD dwNum = 0;
				while (1)
				{
					if ( dwNum >= 1000 )	StringCchPrintf( szNum, STRING_LENGTH_256, "[%d]", dwNum++ );
					else					StringCchPrintf( szNum, STRING_LENGTH_256, "[%03d]", dwNum++ );

					StringCchCopy( pLightNew->m_szLightName, STRING_LENGTH_256, "UNNAMED" );
					StringCchCat( pLightNew->m_szLightName, STRING_LENGTH_256, szNum );

					if ( !GetLight( pLightNew->m_szLightName ) )	break;
				}
			}

			AddLight( pLightNew );
			pCur = pCur->pNext;
		}

		SAFE_DELETE(pLightLoad);
	}

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver104( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bWhenDayPointOff;
	SFile >> m_bNightAndDay;

	m_LtDirectNoon.LoadSet_Ver103 ( SFile );
	m_LtDirectNight.LoadSet_Ver103 ( SFile );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		m_pLtListHead = new DXLIGHT();
		m_pLtListHead->LoadSet_Ver103 ( SFile );
	}

	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver103( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bNightAndDay;
	SFile >> m_bWhenDayPointOff;

	m_LtDirectNight.LoadSet_Ver103 ( SFile );
	m_LtDirectNoon.LoadSet_Ver103 ( SFile );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		m_pLtListHead = new DXLIGHT();
		m_pLtListHead->LoadSet_Ver103 ( SFile );
	}

	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver102( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bNightAndDay;
	SFile >> m_bWhenDayPointOff;

	m_LtDirectNight.LoadSet_Ver101( SFile );
	m_LtDirectNoon.LoadSet_Ver101( SFile );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		m_pLtListHead = new DXLIGHT();
		m_pLtListHead->LoadSet_Ver101 ( SFile );
	}

	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver101( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bNightAndDay;
	SFile >> m_bWhenDayPointOff;

	m_LtDirectNight.LoadSet_Ver101( SFile );
	m_LtDirectNoon.LoadSet_Ver101( SFile );

	ColorUp( m_LtDirectNoon.m_Light.Diffuse, 0.6f );
	ColorUp( m_LtDirectNight.m_Light.Ambient, 0.2f );
	ColorUp( m_LtDirectNoon.m_Light.Ambient, 0.2f );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		m_pLtListHead = new DXLIGHT();
		m_pLtListHead->LoadSet_Ver101 ( SFile );
	}

	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}

void	DxLightMan::Load_Ver100( CSerialFile &SFile )
{
	SFile >> m_bLighting;
	SFile >> m_bNightAndDay;
	SFile >> m_bWhenDayPointOff;

	m_LtDirectNight.LoadSet_Ver100( SFile );
	m_LtDirectNoon.LoadSet_Ver100( SFile );

	ColorUp( m_LtDirectNoon.m_Light.Diffuse, 0.6f );
	ColorUp( m_LtDirectNight.m_Light.Ambient, 0.2f );
	ColorUp( m_LtDirectNoon.m_Light.Ambient, 0.2f );

	BOOL bExist(FALSE);
	SFile >> bExist;
	if ( bExist )
	{
		m_pLtListHead = new DXLIGHT();
		m_pLtListHead->LoadSet_Ver100( SFile );
	}

	SFile >> bExist;
	if ( bExist )
	{	
		m_pTreeHead = new DXLIGHTNODE();
		m_pTreeHead->LoadSet ( SFile, m_pLtListHead );		
	}
}