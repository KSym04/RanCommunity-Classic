#include "stdafx.h"
#include "./MsgCount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMsgCount::CMsgCount()
	: m_bActive(FALSE)
{
	InitializeCriticalSection(&m_CS);
}

CMsgCount::~CMsgCount()
{
	DeleteCriticalSection(&m_CS);
}

CMsgCount& CMsgCount::GetInstance ()
{
	static CMsgCount Instance;
	return Instance;
}



void CMsgCount::Start()
{
	//clear existing data
	m_bActive = TRUE;
	m_mapData.clear();
}

void CMsgCount::Stop()
{
	m_bActive = FALSE;

	VEC_MSG_COUNT vecData;
	vecData.reserve( m_mapData.size() );

	MAP_MSG_COUNT_ITER it = m_mapData.begin();
	MAP_MSG_COUNT_ITER it_e = m_mapData.end();

	for ( ; it!=it_e; ++it )
	{
		vecData.push_back( (*it).second );
	}

	std::sort( vecData.begin(), vecData.end() );

	CDebugSet::ToFile( "_msg_count.txt", "-----------------start msg count dump---------------------" );
	CDebugSet::ToFile( "_msg_count.txt", "-------------------type-size-count------------------------" );
	CDebugSet::ToFile( "_msg_count.txt", "--------------order by number of count--------------------" );

	for ( size_t i=0; i<vecData.size(); ++i )
	{
		SMSG_COUNT& sData = vecData[i];
		CDebugSet::ToFile( "_msg_count.txt", "%d %d %d", sData.dwMsgType, sData.dwMsgSize, sData.dwMsgCount );
	}
}

void CMsgCount::Add( DWORD dwType, DWORD dwSize )
{
	if ( m_bActive == FALSE )
		return;

	EnterCriticalSection(&m_CS);
	{
		MAP_MSG_COUNT_ITER it = m_mapData.find(dwType);
		if ( it != m_mapData.end() )
		{
			SMSG_COUNT& sData = (*it).second;
			sData.dwMsgCount ++;
		}
		else
		{
			SMSG_COUNT sData;
			sData.dwMsgType = dwType;
			sData.dwMsgSize = dwSize;
			sData.dwMsgCount ++;

			m_mapData.insert( std::make_pair( sData.dwMsgType, sData ) );
		}
	}
	LeaveCriticalSection(&m_CS);
}