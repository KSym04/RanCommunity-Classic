#include "stdafx.h"
#include "./GLCharData.h"
#include "./GLItemMan.h"
#include "./GLBattlePass.h"

#include "../../Lib_Engine/Core/NSRParam.h"


#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"

/*12-9-14, Battle Pass - CNDev*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL SCHARDATA2::GETBATTLEPASS_BYBUF( CByteStream &ByteStream ) const
{
	ByteStream.ClearBuffer ();

	ByteStream << (DWORD)SBATTLEPASS_CHAR_DATA::VERSION;

	ByteStream.WriteBuffer(&m_cBattlePassResetTimer, sizeof(CTime));

	ByteStream << (DWORD) m_mapBattlePassProg.size();
	for ( SBATTLEPASS_CHAR_DATA_MAP_CITER pos = m_mapBattlePassProg.begin(); 
		pos != m_mapBattlePassProg.end(); ++pos )
	{
		const SBATTLEPASS_CHAR_DATA* pBattlePass = &pos->second;
		ByteStream.WriteBuffer ( (LPBYTE)pBattlePass, sizeof(SBATTLEPASS_CHAR_DATA) );
	}

	ByteStream << (DWORD) m_mapBattlePassDone.size();
	for ( SBATTLEPASS_CHAR_DATA_MAP_CITER pos = m_mapBattlePassDone.begin(); 
		pos != m_mapBattlePassDone.end(); ++pos )
	{
		const SBATTLEPASS_CHAR_DATA* pBattlePass = &pos->second;
		ByteStream.WriteBuffer ( (LPBYTE)pBattlePass, sizeof(SBATTLEPASS_CHAR_DATA) );
	}

	ByteStream << (DWORD) m_mapBattlePassReward.size();
	for ( SBATTLEPASS_REWARD_CHAR_DATA_MAP_CITER pos = m_mapBattlePassReward.begin(); 
		pos != m_mapBattlePassReward.end(); ++pos )
	{
		const SBATTLEPASS_REWARD_CHAR_DATA* pBattlePass = &pos->second;
		ByteStream.WriteBuffer ( (LPBYTE)pBattlePass, sizeof(SBATTLEPASS_REWARD_CHAR_DATA) );
	}


	return TRUE;
}

BOOL SCHARDATA2::SETBATTLEPASS_BYBUF( CByteStream &ByteStream )
{
	if ( ByteStream.IsEmpty() )	return TRUE;

	DWORD dwVersion, dwNum;
	ByteStream >> dwVersion;

	if ( dwVersion == SBATTLEPASS_CHAR_DATA::VERSION )
	{
		ByteStream.ReadBuffer(&m_cBattlePassResetTimer, sizeof(CTime));

		ByteStream >> dwNum;
		for ( DWORD i=0; i<dwNum; i++ )
		{
			SBATTLEPASS_CHAR_DATA sBattlePass;
			ByteStream.ReadBuffer ( (LPBYTE)&sBattlePass, sizeof(SBATTLEPASS_CHAR_DATA) );
			m_mapBattlePassProg.insert( std::make_pair( sBattlePass.dwBattlePassID, sBattlePass ) );
		}

		ByteStream >> dwNum;
		for ( DWORD i=0; i<dwNum; i++ )
		{
			SBATTLEPASS_CHAR_DATA sBattlePass;
			ByteStream.ReadBuffer ( (LPBYTE)&sBattlePass, sizeof(SBATTLEPASS_CHAR_DATA) );
			m_mapBattlePassDone.insert( std::make_pair( sBattlePass.dwBattlePassID, sBattlePass ) );
		}

		ByteStream >> dwNum;
		for ( DWORD i=0; i<dwNum; i++ )
		{
			SBATTLEPASS_REWARD_CHAR_DATA sReward;
			ByteStream.ReadBuffer ( (LPBYTE)&sReward, sizeof(SBATTLEPASS_REWARD_CHAR_DATA) );
			m_mapBattlePassReward.insert( std::make_pair( sReward.dwLevel, sReward ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "SCHARDATA2::SETBATTLEPASS_BYBUF", dwVersion );
	}

	CTime cCurrentTime = CTime::GetCurrentTime();
	if (m_cBattlePassResetTimer.GetDay() != cCurrentTime.GetDay())
	{
		m_cBattlePassResetTimer = cCurrentTime;

		for (SBATTLEPASS_CHAR_DATA_MAP_ITER iter = m_mapBattlePassProg.begin(); iter != m_mapBattlePassProg.end();)
		{
			SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = iter->second;

			iter = m_mapBattlePassProg.erase(iter);
		}

		m_mapBattlePassDone.clear();

		std::vector<SBATTLEPASS_FILE_DATA*> vecBattlePass;

		SBATTLEPASS_FILE_DATA_MAP& sBattlePassMission = GLBattlePass::GetInstance().m_mapBattlePass;
		for (SBATTLEPASS_FILE_DATA_MAP_ITER iter = sBattlePassMission.begin(); iter != sBattlePassMission.end(); ++iter)
		{
			SBATTLEPASS_FILE_DATA& sBattlePass = iter->second;
			vecBattlePass.push_back(&sBattlePass);
		}

		typedef boost::mt19937 RNGType;
		RNGType rng(static_cast<unsigned int>(time(0)));
		boost::uniform_int<> dist(0, vecBattlePass.size() - 1);
		boost::variate_generator<RNGType&, boost::uniform_int<> > gen(rng, dist);

		for (size_t i = 0; i < vecBattlePass.size(); ++i)
		{
			int nRandomIndex = dist(rng);
			std::swap(vecBattlePass[i], vecBattlePass[nRandomIndex]);
		}


		int nMissionCount = 0;
		for (size_t i = 0; i < vecBattlePass.size(); ++i)
		{
			if (nMissionCount >= BATTLEPASS_DAILY_MISSION_MAX)
				break;

			SBATTLEPASS_FILE_DATA* pBattlePassData = vecBattlePass[i];
			SBATTLEPASS_CHAR_DATA sNewBattlePassData;
			sNewBattlePassData.Assign(*pBattlePassData);
			m_mapBattlePassProg[pBattlePassData->dwBattlePassID] = sNewBattlePassData;

			nMissionCount++;
		}
	}

	return TRUE;
}