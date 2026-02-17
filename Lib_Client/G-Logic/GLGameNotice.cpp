#include "stdafx.h"
#include "./GLGameNotice.h"

#include "./GLAgentServer.h"

#include "../../Lib_Engine/Core/NSRLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLGameNotice::GLGameNotice()
	: m_fUpdateTimer(0.0f)
{

}

GLGameNotice::~GLGameNotice()
{
	ClearData();
}

void GLGameNotice::ClearData()
{
	m_mapNotice.clear();
}

BOOL GLGameNotice::AddData( const SGAME_NOTICE& sData )
{
	GAME_NOTICE_MAP_ITER it = m_mapNotice.find( sData.dwNoticeID );
	if ( it == m_mapNotice.end() )
	{
		m_mapNotice.insert( std::make_pair( sData.dwNoticeID, sData ) );

		RLOG::LogFile( "GameNoticeAdd.txt", "ID:%d %d/%d/%d/%d/%d/%d/%d  Type:%d hour/minute:%d/%d msg:%s",
			sData.dwNoticeID, 
			sData.bDayOfWeek[0], 
			sData.bDayOfWeek[1], 
			sData.bDayOfWeek[2], 
			sData.bDayOfWeek[3], 
			sData.bDayOfWeek[4], 
			sData.bDayOfWeek[5], 
			sData.bDayOfWeek[6],
			sData.emType,
			sData.nHour,
			sData.nMinute,
			sData.szNoticeMessage );

		return TRUE;
	}

	return FALSE;
}

void GLGameNotice::FrameMove( float fTime, float fElapsedTime )
{
	m_fUpdateTimer += fElapsedTime;
	if ( m_fUpdateTimer >= GAME_NOTICE_UPDATE_TIME )
	{
		Update();

		m_fUpdateTimer = 0.0f;
	}
}

void GLGameNotice::Update()
{
	GAME_NOTICE_MAP_ITER it = m_mapNotice.begin();
	GAME_NOTICE_MAP_ITER it_e = m_mapNotice.end();

	for ( ; it != it_e; ++it )
	{
		SGAME_NOTICE& sData = (*it).second;

		const CTime cTIME_CUR = CTime::GetCurrentTime();

		//assume GetDayOfWeek returns 1~7
		int nDay = int( cTIME_CUR.GetDayOfWeek() - 1 );

		//day range should be 0~6
		if ( nDay >= 0 && nDay < 7 )
		{
			//day check
			if ( sData.bDayOfWeek[nDay] )
			{
				bool bBroadcast = false;

				if ( sData.emType == GAME_NOTICE_TYPE_PER_HOUR )
				{
					if ( cTIME_CUR.GetMinute() == sData.nMinute )
					{
						bBroadcast = true;
					}
				}
				else
				{
					if ( cTIME_CUR.GetHour() == sData.nHour && cTIME_CUR.GetMinute() == sData.nMinute )
					{
						bBroadcast = true;
					}
				}


				if ( bBroadcast )
				{
					NET_CHAT_CTRL_FB NetChatFB;
					NetChatFB.emType = CHAT_TYPE_GLOBAL;
					::StringCchCopy(NetChatFB.szChatMsg, GLOBAL_CHAT_MSG_SIZE+1, sData.szNoticeMessage);
					GLAgentServer::GetInstance().SENDTOALLCLIENT( &NetChatFB );

					RLOG::LogFile( "GameNotice.txt", "ID:%d msg:%s", sData.dwNoticeID, sData.szNoticeMessage );
				}
			}
		}	
	}
}