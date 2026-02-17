#include "stdafx.h"
#include "./GLCharData.h"
#include "./GLItemMan.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL SCHARDATA2::GETDAILYLOGIN_BYBUF( CByteStream &ByteStream ) const
{
	ByteStream.ClearBuffer ();

	ByteStream << (DWORD)SDAILYLOGIN_CHAR_DATA::VERSION;

	ByteStream.WriteBuffer(&m_cDailyLoginTimer, sizeof(CTime));

	ByteStream << (DWORD) m_mapDailyLogin.size();
	for ( SDAILYLOGIN_CHAR_DATA_MAP_CITER pos = m_mapDailyLogin.begin(); 
		pos != m_mapDailyLogin.end(); ++pos )
	{
		const SDAILYLOGIN_CHAR_DATA* pDailyLogin = &pos->second;
		ByteStream.WriteBuffer ( (LPBYTE)pDailyLogin, sizeof(SDAILYLOGIN_CHAR_DATA) );
	}


	return TRUE;
}

BOOL SCHARDATA2::SETDAILYLOGIN_BYBUF( CByteStream &ByteStream )
{
	if ( ByteStream.IsEmpty() )	return TRUE;

	DWORD dwVersion, dwNum;
	ByteStream >> dwVersion;

	if ( dwVersion == SDAILYLOGIN_CHAR_DATA::VERSION )
	{
		ByteStream.ReadBuffer(&m_cDailyLoginTimer, sizeof(CTime));

		ByteStream >> dwNum;
		for ( DWORD i=0; i<dwNum; i++ )
		{
			SDAILYLOGIN_CHAR_DATA sDailyLogin;
			ByteStream.ReadBuffer ( (LPBYTE)&sDailyLogin, sizeof(SDAILYLOGIN_CHAR_DATA) );
			m_mapDailyLogin.insert( std::make_pair( sDailyLogin.dwDay, sDailyLogin ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "SCHARDATA2::SETDAILYLOGIN_BYBUF", dwVersion );
	}

	CTime cCurrentTime = CTime::GetCurrentTime();
	CTimeSpan cTimeDifference = cCurrentTime - m_cDailyLoginTimer;

	if (m_cDailyLoginTimer.GetDay() != cCurrentTime.GetDay())
	{
		if (cTimeDifference.GetDays() > 1)
		{
			m_dwDayStreakCounter = 0; // Reset streak
		}

		m_bDailyLogin = true;
	}
	else
	{
		m_bDailyLogin = false;
	}


	return TRUE;
}